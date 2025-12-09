# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

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
        set(CMAKE_FOLDER "${CMAKE_FOLDER}/${tu_countof_func}")
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
            set_tests_properties(${tgt} PROPERTIES
                                 PASS_REGULAR_EXPRESSION "${tu_pos_pass_regexp}")
            if (etgt MATCHES "[.]run_0_unexpected")
                set_tests_properties(${tgt} PROPERTIES
                        PASS_REGULAR_EXPRESSION "${tu_neg_not_terrifying_regexp}")
            endif ()
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
                             FAIL_REGULAR_EXPRESSION "${tu_fail_regexp}"
                             LABELS "${lbl}")
    endforeach ()
endfunction ()

include(unit_cases.cmake)

set(tu_params_list "")
if (COUNTOF_NS_TESTS)
    include (countof_ns_expected.cmake)
endif ()
if (COUNTOF_NS_COMPARISONS)
    file(GLOB cmp_cmakes "comparisons/${COUNTOF_NS_CMP_GLOB}*_expected.cmake")
    foreach (cmk IN ITEMS ${cmp_cmakes})
        include (${cmk})
    endforeach ()
endif ()

foreach (prm IN ITEMS ${tu_params_list})
    process_params("${${prm}}")
endforeach ()

enable_testing()
