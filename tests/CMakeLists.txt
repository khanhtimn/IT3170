file(GLOB_RECURSE CP_FILES "${CMAKE_SOURCE_DIR}/src/week*/*.cpp")

foreach(src_file ${CP_FILES})
    get_filename_component(prog_name ${src_file} NAME_WE)
    get_filename_component(dir_path ${src_file} DIRECTORY)
    get_filename_component(week_name ${dir_path} NAME)

    if(week_name STREQUAL "testing")
        continue()
    endif()

    set(TEST_DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${week_name}/${prog_name}")
    file(GLOB TEST_CASES "${TEST_DATA_DIR}/test_case_*.txt")

    if(TEST_CASES)
        set(TEST_CASES_GEN "")
        foreach(tc_file ${TEST_CASES})
            file(TO_CMAKE_PATH "${tc_file}" tc_file_clean)
            string(APPEND TEST_CASES_GEN "    RUN_IO_TEST(\"${tc_file_clean}\")\n")
        endforeach()

        set(PROG_NAME "${prog_name}")
        set(WEEK_NAME "${week_name}")
        set(PROG_MAIN "cp_main_${prog_name}")

        set(OUT_CPP "${CMAKE_CURRENT_BINARY_DIR}/gen_test_${week_name}_${prog_name}.cpp")
        
        configure_file("test_runner.cpp.in" "${OUT_CPP}" @ONLY)

        add_executable(test_${week_name}_${prog_name} "${OUT_CPP}")
        
        target_link_libraries(test_${week_name}_${prog_name} PRIVATE 
            Catch2::Catch2WithMain
            obj_${week_name}_${prog_name}
        )

        target_include_directories(test_${week_name}_${prog_name} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

        catch_discover_tests(test_${week_name}_${prog_name} WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
    endif()
endforeach()
