# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

if (CMAKE_C_COMPILER_ID STREQUAL LCC AND
    ((NOT "$ENV{LANG}" STREQUAL "C") OR
     (DEFINED ENV{"LC_ALL"} AND NOT "$ENV{LC_ALL}" STREQUAL "C") OR
     (DEFINED ENV{"LC_MESSAGES"} AND NOT "$ENV{LC_MESSAGES}" STREQUAL "C")))
    message("WARNING: Your locale settings are not 'C'."
            " Compiler message checking may be inaccurate."
            " (LANG='$ENV{LANG}' LC_ALL='$ENV{LC_ALL}'"
            " LC_MESSAGES=$ENV{LC_MESSAGES})")
endif ()

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
        if (etgt MATCHES "\\.(gen|tmpl)")
            target_compile_definitions(${tgt} PRIVATE ${inc}
                                       "_COUNTOF_NS_REFUSE_VLA")
            set(vla_fail_regex
                "(_COUNTOF_NS_USE_SUBTRACTION|_COUNTOF_NS_USE_BUILTIN)")
        elseif (etgt MATCHES "\\.c11")
            target_compile_definitions(${tgt} PRIVATE ${inc}
                                       "_COUNTOF_NS_WANT_VLA_C11")
            set(vla_fail_regex "_COUNTOF_NS_VLA_UNSUPPORTED")
        elseif (etgt MATCHES "\\.bltn")
            target_compile_definitions(${tgt} PRIVATE ${inc}
                                       "_COUNTOF_NS_WANT_VLA_BUILTIN")
            if (etgt MATCHES "_cxx($|\\.)")
                target_compile_options(${tgt} PRIVATE
                    ${countof_ns_builtin_cxx_options_${CMAKE_CXX_COMPILER_ID}})
            else ()
                target_compile_options(${tgt} PRIVATE
                    ${countof_ns_builtin_c_options_${CMAKE_C_COMPILER_ID}})
            endif ()
            set(vla_fail_regex "_COUNTOF_NS_VLA_UNSUPPORTED")
        else ()
            set(vla_fail_regex "_COUNTOF_NS_WANT_VLA_")
        endif ()
        target_compile_options(${tgt} PRIVATE ${inc})
        if (NOT etgt MATCHES "^neg")
            target_compile_options(${tgt} PRIVATE "${TAC_WERROR}")
        endif ()
        set(nxre "~=_ Non-existent regular expression _=~")
        set(exre "[Ee]xception")
        set(fpere "[Ff]loating-point|qemu: exception")
        set(segvre "[Ss]egmentation")
        set(div0re "[Dd]ivision.*[Bb]y.*[Zz]ero")
        set(pass_regexp "(${nxre})")
        set(fail_regexp "(${exre}|${fpere}|${segvre}|${div0re}|${nxre})")
        if (etgt MATCHES "\\.disable")
            set_target_properties(${tgt} PROPERTIES EXCLUDE_FROM_ALL TRUE)
            add_test(NAME ${tgt} COMMAND ${tgt})
            set_tests_properties(${tgt} PROPERTIES DISABLED TRUE)
        elseif (etgt MATCHES "\\.compiler_bug")
            set_target_properties(${tgt} PROPERTIES EXCLUDE_FROM_ALL TRUE)
            add_test(NAME ${tgt}
                     COMMAND ${CMAKE_COMMAND}
                        --build .
                        --target ${tgt}
                        --config "${CMAKE_BUILD_TYPE}"
                     WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
            set_tests_properties(${tgt} PROPERTIES
                        WILL_FAIL TRUE
                            # Fail to WILL_FAIL test - pass
                        FAIL_REGULAR_EXPRESSION "${pass_regexp}"
                            # Pass to WILL_FAIL test - fail
                        PASS_REGULAR_EXPRESSION "${fail_regexp}")
        elseif (etgt MATCHES "\\.build_.*")
            if (etgt MATCHES "\\.build_.*\\.build_DIV0")
                string(REPLACE "${div0re}|" "" fail_regexp "${fail_regexp}")
                string(REPLACE "${nxre}" "${div0re}|${nxre}"
                       pass_regexp "${pass_regexp}")
            endif ()
            set_target_properties(${tgt} PROPERTIES EXCLUDE_FROM_ALL TRUE)
            add_test(NAME ${tgt}
                     COMMAND ${CMAKE_COMMAND}
                        --build .
                        --target ${tgt}
                        --config "${CMAKE_BUILD_TYPE}"
                     WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
            if (etgt MATCHES "\\.[^.]*_fail" OR
                (etgt MATCHES "\\.gen\\.build_c2y" AND
                 NOT HAVE_GENERIC_C2Y))
                set_tests_properties(${tgt} PROPERTIES
                        WILL_FAIL TRUE
                            # Fail to WILL_FAIL test - pass
                        FAIL_REGULAR_EXPRESSION "${pass_regexp}"
                            # Pass to WILL_FAIL test - fail
                        #PASS_REGULAR_EXPRESSION "${fail_regexp}"
                        )
            else ()
                set_tests_properties(${tgt} PROPERTIES
                        FAIL_REGULAR_EXPRESSION "${fail_regexp}")
                        #PASS_REGULAR_EXPRESSION "${pass_regexp}")
            endif ()
        elseif (NOT etgt MATCHES "\\.[^.]*_fail")
            add_test(NAME ${tgt} COMMAND ${tgt})
            set_tests_properties(${tgt} PROPERTIES
                                PASS_REGULAR_EXPRESSION "${tu_pos_pass_regexp}"
                                FAIL_REGULAR_EXPRESSION "${fail_regexp}")
                                #PASS_REGULAR_EXPRESSION "${pass_regexp}")
            if (etgt MATCHES "\\.run_0_unexpected")
                set_tests_properties(${tgt} PROPERTIES
                    PASS_REGULAR_EXPRESSION "${tu_neg_not_terrifying_regexp}")
            endif ()
        else ()  # run_fail
            if (etgt MATCHES "\\.run_fail.*\\.run_FPE")
                string(REPLACE "${exre}|" "" fail_regexp "${fail_regexp}")
                string(REPLACE "${fpere}|" "" fail_regexp "${fail_regexp}")
                string(REPLACE "${nxre}" "${exre}.*${fpere}|${nxre}"
                       pass_regexp "${pass_regexp}")
            endif ()
            if (etgt MATCHES "\\.run_fail.*\\.run_SEGV")
                string(REPLACE "${exre}|" "" fail_regexp "${fail_regexp}")
                string(REPLACE "${segvre}|" "" fail_regexp "${fail_regexp}")
                string(REPLACE "${nxre}" "${exre}.*${segvre}|${nxre}"
                       pass_regexp "${pass_regexp}")
            endif ()
            if (etgt MATCHES "\\.run_fail.*\\.run_DIV0")
                string(REPLACE "${div0re}|" "" fail_regexp "${fail_regexp}")
                string(REPLACE "${nxre}" "${div0re}|${nxre}"
                       pass_regexp "${pass_regexp}")
            endif ()
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
                     --test-command ${CMAKE_CROSSCOMPILING_EMULATOR}
                "${CMAKE_CURRENT_BINARY_DIR}/${tgt}${CMAKE_EXECUTABLE_SUFFIX}")
            set_tests_properties(${tgt} PROPERTIES
                        WILL_FAIL TRUE
                            # Fail to WILL_FAIL test - pass
                        FAIL_REGULAR_EXPRESSION "${pass_regexp}"
                            # Pass to WILL_FAIL test - fail
                        PASS_REGULAR_EXPRESSION "${fail_regexp}")
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
    include (cntfn_expected.cmake)
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
