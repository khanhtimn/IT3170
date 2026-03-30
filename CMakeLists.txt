cmake_minimum_required(VERSION 4.0)
project(IT3170 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.13.0
)
FetchContent_MakeAvailable(Catch2)

include(CTest)
include(Catch)

file(GLOB_RECURSE CP_FILES "src/week*/*.cpp")

foreach(src_file ${CP_FILES})
    get_filename_component(prog_name ${src_file} NAME_WE)
    get_filename_component(dir_path ${src_file} DIRECTORY)
    get_filename_component(week_name ${dir_path} NAME)

    if(week_name STREQUAL "testing")
        continue()
    endif()

    add_executable(${week_name}_${prog_name} ${src_file})
    
    add_library(obj_${week_name}_${prog_name} OBJECT ${src_file})
    target_compile_definitions(obj_${week_name}_${prog_name} PRIVATE main=cp_main_${prog_name})
endforeach()

add_subdirectory(tests)
