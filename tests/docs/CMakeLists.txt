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
include (../unit/comparisons/N3369_NITEMS_expected.cmake)

foreach (prm IN ITEMS ${tu_params_list})
    string(REGEX REPLACE "^tu_(.*)_params$" "\\1" cmp "${prm}")
    list(GET ${prm} 3 ${cmp}_available)
endforeach ()

add_test(NAME expected_tables
         COMMAND ${Python3_EXECUTABLE}
                 ${CMAKE_CURRENT_SOURCE_DIR}/expected_tables.py
                 #-v
                 --id ${CMAKE_C_COMPILER_ID}
                 --version ${CMAKE_C_COMPILER_VERSION}
                 --countof_ns "${${cntfn_available}}"
                 --ms_countof "${${ms_countof_available}}"
                 --stdc_countof "${${stdc_countof_available}}"
                 --std_size "${${std_size_available}}"
                 --jzmg_array_len "${${jzmg_array_len_available}}"
                 --lnx_array_size "${${lnx_array_size_available}}"
                 --n3369_nitems "${${n3369_nitems_available}}"
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
                 --examples-dir ${CMAKE_SOURCE_DIR}/include/countof_ns
                 countof_ns.h
                 warning.h countof_c.c sizeof_evaluate.h vla0.c zla.c div0.c
                 types_compatible_p.c zla_cxx.cpp c++26.cpp
                 )

add_test(NAME README_examples
         COMMAND ${Python3_EXECUTABLE}
                 ${CMAKE_CURRENT_SOURCE_DIR}/check_examples.py
                 --verbose
                 --file ${CMAKE_SOURCE_DIR}/README.md
                 diff_countof.h
                 )

add_test(NAME READMEru_examples
         COMMAND ${Python3_EXECUTABLE}
                 ${CMAKE_CURRENT_SOURCE_DIR}/check_examples.py
                 --verbose
                 --file ${CMAKE_SOURCE_DIR}/README.ru.md
                 diff_countof.h
                 )

set(gcc_warn "stringop-overflow")
set(clang_gcc_icx_warn "sizeof-array-argument" "sizeof-array-div"
                       "sizeof-array-decay" "sizeof-pointer-div"
                       "sizeof-pointer-memaccess")
foreach (w IN ITEMS warning_cxx warning_c)
    if (w MATCHES "_cxx")
        set(src "${w}.cpp")
    else ()
        set(src "${w}.c")
    endif ()
    add_executable(${w} ${src} "warning.h")
    add_test(NAME ${w} COMMAND ${w})
    if (CMAKE_C_COMPILER_ID STREQUAL "Clang" OR
        CMAKE_C_COMPILER_ID STREQUAL "GNU" OR
        CMAKE_C_COMPILER_ID STREQUAL "IntelLLVM")
        add_executable(${w}_chk ${src} "warning.h")
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
        set(src "${s}.cpp")
    else ()
        set(src "${s}.c")
    endif ()
    add_executable(${s} ${src} "sizeof_evaluate.h")
    add_test(NAME ${s} COMMAND ${s})
    if (CMAKE_C_COMPILER_ID STREQUAL "Clang" OR
        CMAKE_C_COMPILER_ID STREQUAL "IntelLLVM")
        target_compile_options(${s} PRIVATE "-Wno-unevaluated-expression")
    endif ()
endforeach ()
if (HAVE_VLA)
    add_executable(vla0 "vla0.c")
    add_test(NAME vla0 COMMAND vla0)
endif ()
add_executable(zla "zla.c")
add_test(NAME zla COMMAND zla)
if (HAVE_ZLA)
    add_executable(zla_cxx "zla_cxx.cpp")
    add_test(NAME zla_cxx COMMAND zla_cxx)
endif ()
if (NOT HAVE___STDC_NO_VLA__ AND HAVE_EMPTY_STRUCTURE)
    add_executable(div0 "div0.c")
    add_test(div0 "${CMAKE_CTEST_COMMAND}"
                  --build-and-test "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}"
                  --build-generator "${CMAKE_GENERATOR}"
                  --build-makeprogram "${CMAKE_MAKE_PROGRAM}"
                  --build-config "${CMAKE_BUILD_TYPE}"
                  --build-nocmake
                  --build-noclean
                  --build-target div0
                  --test-command ${CMAKE_CROSSCOMPILING_EMULATOR}
                  "${CMAKE_CURRENT_BINARY_DIR}/div0${CMAKE_EXECUTABLE_SUFFIX}")
    set_tests_properties(div0 PROPERTIES
        FAIL_REGULAR_EXPRESSION "sizeof.v./sizeof.v.0..=1917"
        PASS_REGULAR_EXPRESSION
            "sizeof.v./sizeof.v.0..=(0|[0-9][0-9][0-9][0-9][0-9])|[Ee]xception|[Ff]loating-point|[Ss]egmentation"
        )
endif ()
if (HAVE_COUNTOF)
    add_executable(countof_c "countof_c.c")
    add_test(NAME countof_c COMMAND countof_c)
endif ()
if (HAVE_ZLA AND HAVE_VLA_ZLA)
    add_executable(diff_countof_c "diff_countof_c.c" "diff_countof.h")
    target_link_libraries(diff_countof_c PRIVATE CountofNS::CountofNS)
    add_test(NAME diff_countof_c COMMAND diff_countof_c)
    add_executable(diff_countof_cxx "diff_countof_cxx.cpp" "diff_countof.h")
    if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
            # TODO XXX countof_ns.h:322, struct has_size
            # sorry, unimplemented: mangling save_expr (all gcc 11...16)
        target_compile_options(diff_countof_cxx PRIVATE "-std=c++14")
    endif ()
    target_link_libraries(diff_countof_cxx PRIVATE CountofNS::CountofNS)
    add_test(NAME diff_countof_cxx COMMAND diff_countof_cxx)
endif ()

enable_testing()
