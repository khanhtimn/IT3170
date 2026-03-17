set windows-shell := ["powershell.exe", "-NoLogo", "-NoProfile", "-Command"]
set shell := ["bash", "-euo", "pipefail", "-c"]

src_dir := "src"
build_dir := "build"
test_dir := "tests"

exe := if os_family() == "windows" { ".exe" } else { "" }

cc := if os_family() == "windows" { "cl" } else if os() == "macos" { "clang++" } else { "gcc" }

cflags := if os_family() == "windows" {
    "/std:c++17 /W4 /O2 /EHsc"
} else if os() == "macos" {
    "-std=c++17 -Wall -O2"
} else {
    "-std=c++17 -Wall -O2 -x c++"
}

ldflags := if os_family() == "windows" {
    ""
} else if os() == "macos" {
    env("LDFLAGS", "-lm")
} else {
    env("LDFLAGS", "-lstdc++ -lm")
}

test_exe := build_dir / "test" + exe

test_sources := src_dir / "testing/test.cpp " + \
                src_dir / "testing/test_runner.cpp " + \
                src_dir / "testing/test_parser.cpp " + \
                src_dir / "testing/test_result.cpp " + \
                src_dir / "testing/output_formatter.cpp " + \
                src_dir / "testing/process_executor.cpp"

# Build everything
default: build-all build-test

# Build a single program
[group('build')]
[unix]
build week program:
    #!/usr/bin/env bash
    src="{{ src_dir / week / program }}.cpp"
    target="{{ build_dir / week / program }}{{ exe }}"
    if [ -f "$target" ] && [ "$target" -nt "$src" ]; then
        echo "Skipping $src (up to date)"
        exit 0
    fi
    mkdir -p "{{ build_dir / week }}"
    echo "Compiling $src..."
    {{ cc }} {{ cflags }} "$src" -o "$target" {{ ldflags }}

# Build a single program
[group('build')]
[windows]
build week program:
    $src = "{{ src_dir / week / program }}.cpp"; \
    $target = "{{ build_dir / week / program }}{{ exe }}"; \
    if (Test-Path $target) { \
        if ((Get-Item $target).LastWriteTime -ge (Get-Item $src).LastWriteTime) { \
            Write-Host "Skipping $src (up to date)"; \
            return \
        } \
    }; \
    New-Item -ItemType Directory -Force -Path "{{ build_dir / week }}" | Out-Null; \
    Write-Host "Compiling $src..."; \
    {{ cc }} {{ cflags }} /Fo"{{ build_dir / week }}/" /Fe"$target" "$src"

# Build all week*/program targets
[group('build')]
[unix]
build-all:
    #!/usr/bin/env bash
    for src in {{ src_dir }}/week*/*.cpp; do
        [ -f "$src" ] || continue
        week="$(basename "$(dirname "$src")")"
        prog="$(basename "$src" .cpp)"
        just build "$week" "$prog"
    done

# Build all week*/program targets
[group('build')]
[windows]
build-all:
    Get-ChildItem -Path "{{ src_dir }}/week*/*.cpp" | ForEach-Object { \
        $week = $_.Directory.Name; \
        $prog = $_.BaseName; \
        just build $week $prog \
    }

# Build the test framework
[group('build')]
[unix]
build-test:
    #!/usr/bin/env bash
    sources=({{ test_sources }})
    target="{{ test_exe }}"
    needs_rebuild=false
    if [ ! -f "$target" ]; then
        needs_rebuild=true
    else
        for src in "${sources[@]}"; do
            if [ "$src" -nt "$target" ]; then
                needs_rebuild=true
                break
            fi
        done
    fi
    if [ "$needs_rebuild" = false ]; then
        echo "Skipping test framework (up to date)"
        exit 0
    fi
    mkdir -p "{{ build_dir }}"
    echo "Compiling test framework..."
    {{ cc }} {{ cflags }} {{ test_sources }} -o {{ test_exe }} {{ ldflags }}

# Build the test framework
[group('build')]
[windows]
build-test:
    $sources = @({{ test_sources }}); \
    $target = "{{ test_exe }}"; \
    $needsRebuild = $false; \
    if (-not (Test-Path $target)) { \
        $needsRebuild = $true \
    } else { \
        $targetTime = (Get-Item $target).LastWriteTime; \
        foreach ($src in $sources) { \
            if ((Get-Item $src).LastWriteTime -gt $targetTime) { \
                $needsRebuild = $true; \
                break \
            } \
        } \
    }; \
    if (-not $needsRebuild) { \
        Write-Host "Skipping test framework (up to date)"; \
        return \
    }; \
    New-Item -ItemType Directory -Force -Path "{{ build_dir }}" | Out-Null; \
    Write-Host "Compiling test framework..."; \
    {{ cc }} {{ cflags }} /Fo"{{ build_dir }}/" /Fe"$target" {{ test_sources }}

# Run test for a single program
[group('test')]
test week program: (build week program) build-test
    {{ test_exe }} {{ week }} {{ program }}

# Run all tests
[group('test')]
[unix]
test-all: build-all build-test
    #!/usr/bin/env bash
    for src in {{ src_dir }}/week*/*.cpp; do
        [ -f "$src" ] || continue
        week="$(basename "$(dirname "$src")")"
        prog="$(basename "$src" .cpp)"
        echo ""
        echo "Testing $week/$prog..."
        just test "$week" "$prog"
    done

# Run all tests
[group('test')]
[windows]
test-all: build-all build-test
    Get-ChildItem -Path "{{ src_dir }}/week*/*.cpp" | ForEach-Object { \
        $week = $_.Directory.Name; \
        $prog = $_.BaseName; \
        Write-Host "`nTesting $week/$prog..."; \
        just test $week $prog \
    }

# Clean build artifacts and test outputs
[group('util')]
[confirm("Remove build directory and .out files?")]
[unix]
clean:
    {{ if path_exists(build_dir) == "true" { "echo 'Cleaning " + build_dir + "...' && rm -rf " + build_dir } else { "echo 'Nothing to clean.'" } }}
    find "{{ test_dir }}" -name "*.out" -type f -delete 2>/dev/null || true

# Clean build artifacts and test outputs
[group('util')]
[confirm("Remove build directory and .out files?")]
[windows]
clean:
    if (Test-Path "{{ build_dir }}") { Write-Host "Cleaning {{ build_dir }}..."; Remove-Item -Recurse -Force "{{ build_dir }}" } else { Write-Host "Nothing to clean." }
    Get-ChildItem -Path "{{ test_dir }}" -Filter "*.out" -Recurse -ErrorAction SilentlyContinue | Remove-Item -Force

# Force rebuild everything
[group('build')]
rebuild: clean default
