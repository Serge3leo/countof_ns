# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

# ./examples-build.sh -c . -- "-DCOUNTOF_NS_EXAMPLES=ON" "-DCOUNTOF_NS_TESTS=ON"
# suncc - 60
# pgcc - 104
# icx - 121
# gcc-11 - 104
# clang-14 - 104
# gcc-mp-15 - 121
# calng-mp-21 - 121
# icc - 104
# MSVC - 26

include(${CMAKE_SOURCE_DIR}/examples/countof_ns_builtin_options.cmake)

function (process_params params)
    list(GET params 0 dflt)
    list(GET params 1 tu_countof_func)
    list(GET params 2 tu_countof_inc)
    list(GET params 3 expected)

    set(lbl ${tu_countof_func})
    if (dflt)
        list(APPEND lbl "default")
    else ()
        set(CMAKE_FOLDER tests/${tu_countof_func})
    endif ()
    foreach (etgt IN ITEMS ${${expected}})
        get_filename_component(base ${etgt} NAME_WE)
        if (dflt)
            set(tgt ${etgt})
        else ()
            set(tgt "${tu_countof_func}-${etgt}")
        endif ()
        if (base MATCHES "_cxx")
            string(REGEX REPLACE "_cxx" "" base ${base})
            set(src tu_stub_cxx.cpp)
        else ()
            set(src tu_stub.c)
        endif ()
        list(APPEND src tu_stub.h ${base}.h)
        set(inc "-DTU_UNIT_INC=\"${base}.h\""
                "-DTU_COUNTOF_FUNC=${tu_countof_func}"
                "-DTU_COUNTOF_INC=\"${tu_countof_inc}\"")
        add_executable(${tgt} ${src} ${base}.h
                              "${CMAKE_SOURCE_DIR}/include/${tu_countof_inc}")
        target_link_libraries(${tgt} PRIVATE CountofNS::CountofNS)
        if (etgt MATCHES "[.]c11")
            target_compile_definitions(${tgt} PRIVATE ${inc}
                                       "_COUNTOF_NS_WANT_VLA_C11")
            set(vla_fail_regex "_COUNTOF_NS_VLA_UNSUPPORTED")
        elseif (etgt MATCHES "[.]bltn")
            target_compile_definitions(${tgt} PRIVATE ${inc}
                                       "_COUNTOF_NS_WANT_VLA_BUILTIN")
            target_compile_options(${tgt} PRIVATE
                    ${countof_ns_builtin_options_${CMAKE_C_COMPILER_ID}})
            set(vla_fail_regex "_COUNTOF_NS_VLA_UNSUPPORTED")
        else ()
            set(vla_fail_regex "_COUNTOF_NS_WANT_VLA_")
        endif ()
        target_compile_options(${tgt} PRIVATE ${inc})
        if (NOT etgt MATCHES "^neg")
            target_compile_options(${tgt} PRIVATE "${TAC_WERROR}")
        endif ()
        if (etgt MATCHES "[.]build_.*")
            set_target_properties(${tgt} PROPERTIES EXCLUDE_FROM_ALL TRUE)
            add_test(NAME ${tgt}
                     COMMAND ${CMAKE_COMMAND}
                        --build .
                        --target ${tgt}
                        --config "${CMAKE_BUILD_TYPE}"
                     WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
            if (etgt MATCHES "[.][^.]*_fail")
                set_tests_properties(${tgt} PROPERTIES WILL_FAIL TRUE)
            endif ()
        elseif (NOT etgt MATCHES "[.][^.]*_fail")
            add_test(NAME ${tgt} COMMAND ${tgt})
        else ()
            set_target_properties(${tgt} PROPERTIES EXCLUDE_FROM_ALL TRUE)
            add_test(${tgt} "${CMAKE_CTEST_COMMAND}"
                     --build-and-test "${CMAKE_SOURCE_DIR}"
                                      "${CMAKE_BINARY_DIR}"
                     --build-generator "${CMAKE_GENERATOR}"
                     --build-makeprogram "${CMAKE_MAKE_PROGRAM}"
                     --build-config "${CMAKE_BUILD_TYPE}"
                     --build-nocmake
                     --build-noclean
                     --build-target ${tgt}
                     --test-command "${CMAKE_CURRENT_BINARY_DIR}/${tgt}")
            set_tests_properties(${tgt} PROPERTIES WILL_FAIL TRUE)
        endif ()
        set_tests_properties(${tgt} PROPERTIES
                             FAIL_REGULAR_EXPRESSION "${vla_fail_regex}"
                             LABELS "${lbl}")
    endforeach ()
endfunction ()

include(unit_cases.cmake)

set(tu_params_list "")
include (countof_ns_expected.cmake)
if (COUNTOF_NS_COMPARISONS)
    file(GLOB cmp_cmakes "comparisons/*_expected.cmake")
    foreach (cmk IN ITEMS ${cmp_cmakes})
        # TODO message("cmk=${cmk}")
        include (${cmk})
        # TODO message("tu_params_list=${tu_params_list}")
    endforeach ()
endif ()

foreach (prm IN ITEMS ${tu_params_list})
    # TODO message("prm=${prm}")
    # TODO message("prm=${${prm}}")
    process_params("${${prm}}")
endforeach ()

enable_testing()
