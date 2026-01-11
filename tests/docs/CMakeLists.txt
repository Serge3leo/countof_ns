# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

find_package(Python3)

set(CMAKE_FOLDER "${CMAKE_FOLDER}/docs")

add_test(NAME Python3_FOUND
         COMMAND ${Python3_EXECUTABLE} "-c" "exit(0)")

    # egrep -r -i ' [0-9a-f]{32,} '
set(git_tags ${CMAKE_SOURCE_DIR}/README.md
             ${CMAKE_SOURCE_DIR}/README.ru.md
             ${CMAKE_SOURCE_DIR}/examples/cmake_fetch_content/CMakeLists.txt)
add_test(NAME git_tags
         COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/git_tags.py
                 ${git_tags})

add_test(NAME compiler_id
         COMMAND ${Python3_EXECUTABLE}
                 ${CMAKE_CURRENT_SOURCE_DIR}/compiler_id.py
                 --id ${CMAKE_C_COMPILER_ID})

tac_report(rep)
add_test(NAME compiler_versions
         COMMAND ${Python3_EXECUTABLE}
                 ${CMAKE_CURRENT_SOURCE_DIR}/compiler_versions.py
                 --id ${CMAKE_C_COMPILER_ID}
                 --version ${CMAKE_C_COMPILER_VERSION}
                 ${rep})

include(../unit/unit_cases.cmake)

set(tu_params_list "")
include (../unit/cntfn_expected.cmake)
include (../unit/comparisons/ms_countof_expected.cmake)
include (../unit/comparisons/stdc_countof_expected.cmake)
include (../unit/comparisons/std_size_expected.cmake)
include (../unit/comparisons/JZMG_ARRAY_LEN_expected.cmake)
include (../unit/comparisons/LNX_ARRAY_SIZE_expected.cmake)
include (../unit/comparisons/ALX_COUNTOF_expected.cmake)

foreach (prm IN ITEMS ${tu_params_list})
    string(REGEX REPLACE "^tu_(.*)_params$" "\\1" cmp "${prm}")
    list(GET ${prm} 3 ${cmp}_available)
endforeach ()

add_test(NAME expected_tables
         COMMAND ${Python3_EXECUTABLE}
                 ${CMAKE_CURRENT_SOURCE_DIR}/expected_tables.py
                 -v
                 --id ${CMAKE_C_COMPILER_ID}
                 --version ${CMAKE_C_COMPILER_VERSION}
                 --countof_ns "${${cntfn_available}}"
                 --ms_countof "${${ms_countof_available}}"
                 --stdc_countof "${${stdc_countof_available}}"
                 --std_size "${${std_size_available}}"
                 --jzmg_array_len "${${jzmg_array_len_available}}"
                 --lnx_array_size "${${lnx_array_size_available}}"
                 --alx_countof "${${alx_countof_available}}"
                ${rep})

add_test(NAME compare.en.ru
         COMMAND ${Python3_EXECUTABLE}
                 ${CMAKE_CURRENT_SOURCE_DIR}/compare.en.ru.py
                 #--verbose
                 )

add_test(NAME check_examples
         COMMAND ${Python3_EXECUTABLE}
                 ${CMAKE_CURRENT_SOURCE_DIR}/check_examples.py
                 --verbose
                 warning.h countof_c.c sizeof_evaluate.h vla0.c zla.c
                 )

set(gcc_warn "stringop-overflow")
set(clang_gcc_icx_warn "sizeof-array-argument" "sizeof-array-div"
                       "sizeof-array-decay" "sizeof-pointer-div"
                       "sizeof-pointer-memaccess")
foreach (w IN ITEMS warning_cxx warning_c)
    if (w MATCHES "_cxx")
        set(src "${CMAKE_SOURCE_DIR}/docs/${w}.cpp")
    else ()
        set(src "${CMAKE_SOURCE_DIR}/docs/${w}.c")
    endif ()
    add_executable(${w} ${src} "${CMAKE_SOURCE_DIR}/docs/warning.h")
    add_test(NAME ${w} COMMAND ${w})
    if (CMAKE_C_COMPILER_ID STREQUAL "Clang" OR
        CMAKE_C_COMPILER_ID STREQUAL "GNU" OR
        CMAKE_C_COMPILER_ID STREQUAL "IntelLLVM")
        add_executable(${w}_chk ${src} "${CMAKE_SOURCE_DIR}/docs/warning.h")
        target_compile_options(${w}_chk PRIVATE "-Werror")
        set_target_properties(${w}_chk PROPERTIES EXCLUDE_FROM_ALL TRUE)
        add_test(NAME ${w}_chk
                 COMMAND ${CMAKE_COMMAND}
                        --build .
                        --target ${w}_chk
                        --config "${CMAKE_BUILD_TYPE}"
                 WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
        foreach (warn IN LISTS clang_gcc_icx_warn)
            target_compile_options(${w} PRIVATE "-Wno-${warn}")
        endforeach ()
        set_tests_properties(${w}_chk PROPERTIES PASS_REGULAR_EXPRESSION
            "-Wsizeof-array-argument.*-Wsizeof-pointer-div.*-Wsizeof-pointer-memaccess.*-Wsizeof-array-div.*-Wsizeof-array-decay.*5 errors generated"
        )
    endif ()
    if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
        foreach (warn IN LISTS gcc_warn)
            target_compile_options(${w} PRIVATE "-Wno-${warn}")
        endforeach ()
        target_compile_options(${w}_chk PRIVATE "-Wall")
        if (w MATCHES "_cxx")
            set_tests_properties(${w}_chk PROPERTIES PASS_REGULAR_EXPRESSION
                "-Werror=sizeof-array-argument.*-Werror=sizeof-pointer-div.*-Werror=sizeof-pointer-memaccess.*-Werror=sizeof-array-div"
            )
        else ()
            set_tests_properties(${w}_chk PROPERTIES PASS_REGULAR_EXPRESSION
                "-Werror=sizeof-array-argument.*-Werror=sizeof-pointer-div.*-Werror=sizeof-pointer-memaccess.*-Werror=sizeof-array-div.*-Werror=stringop-overflow"
            )
        endif ()
    endif ()
endforeach ()

foreach (s IN ITEMS sizeof_evaluate_c sizeof_evaluate_cxx)
    if (s MATCHES "_cxx")
        set(src "${CMAKE_SOURCE_DIR}/docs/${s}.cpp")
    else ()
        set(src "${CMAKE_SOURCE_DIR}/docs/${s}.c")
    endif ()
    add_executable(${s} ${src} "${CMAKE_SOURCE_DIR}/docs/sizeof_evaluate.h")
    add_test(NAME ${s} COMMAND ${s})
    if (CMAKE_C_COMPILER_ID STREQUAL "Clang" OR
        CMAKE_C_COMPILER_ID STREQUAL "IntelLLVM")
        target_compile_options(${s} PRIVATE "-Wno-unevaluated-expression")
    endif ()
endforeach ()
if (HAVE_VLA)
    add_executable(vla0 "${CMAKE_SOURCE_DIR}/docs/vla0.c")
    add_test(NAME vla0 COMMAND vla0)
endif ()
add_executable(zla "${CMAKE_SOURCE_DIR}/docs/zla.c")
add_test(NAME zla COMMAND zla)

if (HAVE_COUNTOF)
    add_executable(countof_c "${CMAKE_SOURCE_DIR}/docs/countof_c.c")
    add_test(NAME countof_c COMMAND countof_c)
endif ()

enable_testing()
