set windows-shell := ["powershell.exe", "-NoLogo", "-NoProfile", "-Command"]
set shell := ["bash", "-euo", "pipefail", "-c"]

src_dir := "src"
build_dir := "build"
test_dir := "tests"
mode := env("MODE", "release")

exe := if os_family() == "windows" { ".exe" } else { "" }

cxx := env("CXX", if os_family() == "windows" {
  "cl"
} else if os() == "macos" {
  "clang++"
} else {
  "g++"
})

base_cxxflags := if os_family() == "windows" {
  if mode == "debug" {
    "/std:c++17 /W4 /Zi /Od /EHsc"
  } else if mode == "asan" {
    "/std:c++17 /W4 /Zi /Od /EHsc /fsanitize=address"
  } else {
    "/std:c++17 /W4 /O2 /EHsc"
  }
} else {
  if mode == "debug" {
    "-std=c++17 -Wall -Wextra -g -O0"
  } else if mode == "asan" {
    "-std=c++17 -Wall -Wextra -g -O1 -fsanitize=address,undefined"
  } else {
    "-std=c++17 -Wall -Wextra -O2"
  }
}

cxxflags := env("CXXFLAGS", base_cxxflags)
includes := if os_family() == "windows" {
  "/I" + src_dir
} else {
  "-I" + src_dir
}
ldflags := env("LDFLAGS", "")
depflags := if os_family() == "windows" { "" } else { "-MMD -MP" }

test_exe := build_dir / "test" + exe

default: build-all
alias compile := b
[group('meta')]
help:
  @just --list

[group('meta')]
[unix]
list:
  #!/usr/bin/env bash
  shopt -s nullglob
  for src in {{ src_dir }}/week[0-9]*/*.cpp; do
    week="$(basename "$(dirname "$src")")"
    prog="$(basename "$src" .cpp)"
    echo "$week $prog"
  done

[group('meta')]
[windows]
list:
  Get-ChildItem -Path "{{ src_dir }}/week[0-9]*/*.cpp" -ErrorAction SilentlyContinue | ForEach-Object { Write-Host "$($_.Directory.Name) $($_.BaseName)" }

[group('build')]
[unix]
b week program:
  #!/usr/bin/env bash
  src="{{ src_dir }}/{{ week }}/{{ program }}.cpp"
  out_dir="{{ build_dir }}/{{ week }}"
  target="$out_dir/{{ program }}{{ exe }}"
  depfile="$out_dir/{{ program }}.d"

  [[ -f "$src" ]] || { echo "Source not found: $src" >&2; exit 1; }
  mkdir -p "$out_dir"

  needs_rebuild=false
  if [[ ! -f "$target" || ! -f "$depfile" ]]; then
    needs_rebuild=true
  elif [[ "$src" -nt "$target" ]]; then
    needs_rebuild=true
  else
    while IFS= read -r dep; do
      [[ -n "$dep" && -f "$dep" && "$dep" -nt "$target" ]] && {
        needs_rebuild=true
        break
      }
    done < <(
      sed -e 's/\\$//' "$depfile" |
        sed -e 's/^[^:]*: //' |
        tr ' ' '\n' |
        sed '/^$/d'
    )
  fi

  if [[ "$needs_rebuild" == false ]]; then
    exit 0
  fi

  echo "Building {{ week }}/{{ program }}"
  {{ cxx }} {{ cxxflags }} {{ includes }} {{ depflags }} \
    -MF "$depfile" \
    "$src" \
    -o "$target" \
    {{ ldflags }}

[group('build')]
[windows]
b week program:
  $src = "{{ src_dir }}/{{ week }}/{{ program }}.cpp"; \
  $outDir = "{{ build_dir }}/{{ week }}"; \
  $target = "$outDir/{{ program }}{{ exe }}"; \
  $depfile = "$outDir/{{ program }}.d"; \
  $showIncludesPrefix = "Note: including file:"; \
  if (-not (Test-Path $src)) { \
    Write-Error "Source not found: $src"; exit 1 \
  }; \
  New-Item -ItemType Directory -Force -Path $outDir | Out-Null; \
  $needsRebuild = $false; \
  if (-not (Test-Path $target) -or -not (Test-Path $depfile)) { \
    $needsRebuild = $true \
  } elseif ((Get-Item $src).LastWriteTime -gt (Get-Item $target).LastWriteTime) { \
    $needsRebuild = $true \
  } else { \
    $targetTime = (Get-Item $target).LastWriteTime; \
    $deps = Get-Content $depfile -ErrorAction SilentlyContinue; \
    foreach ($dep in $deps) { \
      if ($dep -and (Test-Path $dep)) { \
        if ((Get-Item $dep).LastWriteTime -gt $targetTime) { \
          $needsRebuild = $true; \
          break \
        } \
      } \
    } \
  }; \
  if (-not $needsRebuild) { return }; \
  Write-Host "Building {{ week }}/{{ program }}"; \
  $output = & {{ cxx }} {{ cxxflags }} {{ includes }} /nologo /showIncludes /Fe"$target" "$src" 2>&1; \
  if ($LASTEXITCODE -ne 0) { \
    $output | ForEach-Object { Write-Host $_ }; \
    exit $LASTEXITCODE \
  }; \
  $deps = New-Object System.Collections.Generic.List[string]; \
  $deps.Add((Resolve-Path $src).Path); \
  foreach ($line in $output) { \
    $text = [string]$line; \
    if ($text.TrimStart().StartsWith($showIncludesPrefix)) { \
      $dep = $text.Substring($text.IndexOf($showIncludesPrefix) + $showIncludesPrefix.Length).Trim(); \
      if ($dep -and (Test-Path $dep)) { \
        try { \
          $resolved = (Resolve-Path $dep).Path; \
          if (-not $deps.Contains($resolved)) { \
            $deps.Add($resolved) \
          } \
        } catch {} \
      } \
    } else { \
      Write-Host $text \
    } \
  }; \
  Set-Content -Path $depfile -Value $deps

[group('build')]
[unix]
build-all:
  #!/usr/bin/env bash
  shopt -s nullglob
  found=0
  for src in {{ src_dir }}/week[0-9]*/*.cpp; do
    found=1
    week="$(basename "$(dirname "$src")")"
    prog="$(basename "$src" .cpp)"
    just b "$week" "$prog"
  done
  if [[ $found -eq 0 ]]; then
    echo "No programs found."
  fi

[group('build')]
[windows]
build-all:
  $files = Get-ChildItem -Path "{{ src_dir }}/week[0-9]*/*.cpp" -ErrorAction SilentlyContinue; \
  if (-not $files) { Write-Host "No programs found."; exit 0 }; \
  $files | ForEach-Object { just b $_.Directory.Name $_.BaseName }

[group('build')]
[unix]
build-test:
  #!/usr/bin/env bash
  out_dir="{{ build_dir }}/testing"
  target="{{ test_exe }}"
  depfile="$out_dir/test.d"
  sources=(
    "{{ src_dir }}/testing/test.cpp"
    "{{ src_dir }}/testing/test_runner.cpp"
    "{{ src_dir }}/testing/test_parser.cpp"
    "{{ src_dir }}/testing/test_result.cpp"
    "{{ src_dir }}/testing/output_formatter.cpp"
    "{{ src_dir }}/testing/process_executor.cpp"
  )

  mkdir -p "$out_dir"

  for src in "${sources[@]}"; do
    [[ -f "$src" ]] || { echo "Missing test source: $src" >&2; exit 1; }
  done

  needs_rebuild=false
  if [[ ! -f "$target" || ! -f "$depfile" ]]; then
    needs_rebuild=true
  else
    for src in "${sources[@]}"; do
      if [[ "$src" -nt "$target" ]]; then
        needs_rebuild=true
        break
      fi
    done

    if [[ "$needs_rebuild" == false ]]; then
      while IFS= read -r dep; do
        [[ -n "$dep" && -f "$dep" && "$dep" -nt "$target" ]] && {
          needs_rebuild=true
          break
        }
      done < <(
        sed -e 's/\\$//' "$depfile" |
          sed -e 's/^[^:]*: //' |
          tr ' ' '\n' |
          sed '/^$/d'
      )
    fi
  fi

  if [[ "$needs_rebuild" == false ]]; then
    exit 0
  fi

  echo "Building test framework"
  {{ cxx }} {{ cxxflags }} {{ includes }} {{ depflags }} \
    -MF "$depfile" \
    "${sources[@]}" \
    -o "$target" \
    {{ ldflags }}

[group('build')]
[windows]
build-test:
  $sources = @( \
    "{{ src_dir }}/testing/test.cpp", \
    "{{ src_dir }}/testing/test_runner.cpp", \
    "{{ src_dir }}/testing/test_parser.cpp", \
    "{{ src_dir }}/testing/test_result.cpp", \
    "{{ src_dir }}/testing/output_formatter.cpp", \
    "{{ src_dir }}/testing/process_executor.cpp" \
  ); \
  $outDir = "{{ build_dir }}/testing"; \
  $target = "{{ test_exe }}"; \
  $depfile = "$outDir/test.d"; \
  $showIncludesPrefix = "Note: including file:"; \
  foreach ($src in $sources) { \
    if (-not (Test-Path $src)) { \
      Write-Error "Missing test source: $src"; exit 1 \
    } \
  }; \
  New-Item -ItemType Directory -Force -Path $outDir | Out-Null; \
  $needsRebuild = $false; \
  if (-not (Test-Path $target) -or -not (Test-Path $depfile)) { \
    $needsRebuild = $true \
  } else { \
    $targetTime = (Get-Item $target).LastWriteTime; \
    foreach ($src in $sources) { \
      if ((Get-Item $src).LastWriteTime -gt $targetTime) { \
        $needsRebuild = $true; \
        break \
      } \
    }; \
    if (-not $needsRebuild) { \
      $deps = Get-Content $depfile -ErrorAction SilentlyContinue; \
      foreach ($dep in $deps) { \
        if ($dep -and (Test-Path $dep)) { \
          if ((Get-Item $dep).LastWriteTime -gt $targetTime) { \
            $needsRebuild = $true; \
            break \
          } \
        } \
      } \
    } \
  }; \
  if (-not $needsRebuild) { return }; \
  Write-Host "Building test framework"; \
  $output = & {{ cxx }} {{ cxxflags }} {{ includes }} /nologo /showIncludes /Fe"$target" $sources 2>&1; \
  if ($LASTEXITCODE -ne 0) { \
    $output | ForEach-Object { Write-Host $_ }; \
    exit $LASTEXITCODE \
  }; \
  $deps = New-Object System.Collections.Generic.List[string]; \
  foreach ($src in $sources) { \
    $resolved = (Resolve-Path $src).Path; \
    if (-not $deps.Contains($resolved)) { \
      $deps.Add($resolved) \
    } \
  }; \
  foreach ($line in $output) { \
    $text = [string]$line; \
    if ($text.TrimStart().StartsWith($showIncludesPrefix)) { \
      $dep = $text.Substring($text.IndexOf($showIncludesPrefix) + $showIncludesPrefix.Length).Trim(); \
      if ($dep -and (Test-Path $dep)) { \
        try { \
          $resolved = (Resolve-Path $dep).Path; \
          if (-not $deps.Contains($resolved)) { \
            $deps.Add($resolved) \
          } \
        } catch {} \
      } \
    } else { \
      Write-Host $text \
    } \
  }; \
  Set-Content -Path $depfile -Value $deps

[group('test')]
test: build-all build-test test-all

[group('test')]
[unix]
test-all:
  #!/usr/bin/env bash
  shopt -s nullglob
  found=0
  failed=0
  for src in {{ src_dir }}/week[0-9]*/*.cpp; do
    found=1
    week="$(basename "$(dirname "$src")")"
    prog="$(basename "$src" .cpp)"
    echo
    echo "Testing $week/$prog"
    if ! "{{ test_exe }}" "$week" "$prog"; then
      failed=1
    fi
  done
  if [[ $found -eq 0 ]]; then
    echo "No programs found."
  fi
  if [[ $failed -ne 0 ]]; then
    exit 1
  fi

[group('test')]
[windows]
test-all:
  $files = Get-ChildItem -Path "{{ src_dir }}/week[0-9]*/*.cpp" -ErrorAction SilentlyContinue; \
  if (-not $files) { Write-Host "No programs found."; exit 0 }; \
  $failed = $false; \
  $files | ForEach-Object { \
    Write-Host "`nTesting $($_.Directory.Name)/$($_.BaseName)"; \
    & "{{ test_exe }}" $_.Directory.Name $_.BaseName; \
    if ($LASTEXITCODE -ne 0) { $failed = $true } \
  }; \
  if ($failed) { exit 1 }

[group('test')]
[unix]
check week program:
  #!/usr/bin/env bash
  just b "{{ week }}" "{{ program }}"
  just build-test
  "{{ test_exe }}" "{{ week }}" "{{ program }}"

[group('test')]
[windows]
check week program:
  just b "{{ week }}" "{{ program }}"; \
  just build-test; \
  & "{{ test_exe }}" "{{ week }}" "{{ program }}"

[group('run')]
[unix]
run week program:
  #!/usr/bin/env bash
  just b "{{ week }}" "{{ program }}"
  "{{ build_dir }}/{{ week }}/{{ program }}{{ exe }}"

[group('run')]
[windows]
run week program:
  just b "{{ week }}" "{{ program }}"; \
  & "{{ build_dir }}/{{ week }}/{{ program }}{{ exe }}"

[group('util')]
[confirm("Remove build directory and .out files?")]
[unix]
clean:
  #!/usr/bin/env bash
  if [[ -d "{{ build_dir }}" ]]; then
    echo "Cleaning {{ build_dir }}"
    rm -rf "{{ build_dir }}"
  else
    echo "Nothing to clean."
  fi
  find "{{ test_dir }}" -name "*.out" -type f -delete 2>/dev/null || true

[group('util')]
[confirm("Remove build directory and .out files?")]
[windows]
clean:
  if (Test-Path "{{ build_dir }}") { \
    Write-Host "Cleaning {{ build_dir }}"; \
    Remove-Item -Recurse -Force "{{ build_dir }}" \
  } else { \
    Write-Host "Nothing to clean." \
  }; \
  Get-ChildItem -Path "{{ test_dir }}" -Filter "*.out" -Recurse -ErrorAction SilentlyContinue | Remove-Item -Force

[group('util')]
[unix]
clean-force:
  #!/usr/bin/env bash
  rm -rf "{{ build_dir }}"
  find "{{ test_dir }}" -name "*.out" -type f -delete 2>/dev/null || true

[group('util')]
[windows]
clean-force:
  if (Test-Path "{{ build_dir }}") { Remove-Item -Recurse -Force "{{ build_dir }}" }; \
  Get-ChildItem -Path "{{ test_dir }}" -Filter "*.out" -Recurse -ErrorAction SilentlyContinue | Remove-Item -Force

[group('build')]
rebuild: clean-force test
