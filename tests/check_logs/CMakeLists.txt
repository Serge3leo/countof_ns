# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

set(examples_logs_dir "${CMAKE_SOURCE_DIR}/examples/junit-logs")
# WARNING: `ctest --output-junit...` truncates the output to 1024 bytes for
# successful tests
add_test(_check_logs_config ${CMAKE_COMMAND}
    -P ${CMAKE_CURRENT_SOURCE_DIR}/echo_3.cmake
    "CL_TAG='${CL_TAG}'
    CMAKE_C_COMPILER_ID='${CMAKE_C_COMPILER_ID}'
    CMAKE_C_COMPILER_VERSION='${CMAKE_C_COMPILER_VERSION}'
    CMAKE_C_FLAGS='${CMAKE_C_FLAGS}'
    TAC_WERROR='${TAC_WERROR}'
    CXX_ENABLED='${CXX_ENABLED}'
    ")
set_tests_properties(_check_logs_config PROPERTIES LABELS "default;check_logs")

set(selftest_code -1)
find_package(Python3)
if (Python3_FOUND)
    set(post_check ${Python3_EXECUTABLE}
                   ${CMAKE_CURRENT_SOURCE_DIR}/post_check.py)
    execute_process(COMMAND ${post_check} --selftest
                    RESULT_VARIABLE selftest_code)
endif ()
if (NOT selftest_code EQUAL 0)
    message("Skip check_logs tests")
else()
    file(GLOB logs "${examples_logs_dir}/*.xml")
    foreach (log IN ITEMS ${logs})
        get_filename_component(base ${log} NAME_WLE)
        set(test "post_check_${base}")
        add_test("${test}" ${post_check} ${log})
        set_tests_properties("${test}" PROPERTIES LABELS check_logs)
        if ("${base}" STREQUAL "${CMAKE_C_COMPILER_ID}-${CL_TAG}")
            set_tests_properties("${test}" PROPERTIES LABELS default)
        endif ()
    endforeach ()
endif ()

enable_testing()
