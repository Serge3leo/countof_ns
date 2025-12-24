# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

# Detect default compiler capability (no options) for tests

option(TAC_ENABLE_WARNINGS
       "Enable maximum warnings (for debug only)" OFF)
option(TAC_POSITIVE_WERROR
       "Error positive test on warnings (for debug only)" OFF)
option(TAC_VERBOSE
       "Print autoconf compiler&run output" OFF)
# TODO: option(TAC_ALL_AUTOCONF
# TODO:        "Check all autoconf tests, including unused ones" OFF)

if (CMAKE_C_COMPILER_ID STREQUAL "") # TODO Pelles XXX Remove or?
    set(CXX_ENABLED FALSE)
    set(CMAKE_C_COMPILER_ID Pelles)
    #set(CMAKE_C_COMPILER_FRONTEND_VARIANT "MSVC")
    set(CMAKE_C_COMPILER_ID "${CMAKE_C_COMPILER_ID}")
    set(CMAKE_C_COMPILER_FRONTEND_VARIANT
        "${CMAKE_C_COMPILER_FRONTEND_VARIANT}")
    message("WARNING: chage to CMAKE_C_COMPILER_ID=${CMAKE_C_COMPILER_ID} "
     "CMAKE_C_COMPILER_FRONTEND_VARIANT=${CMAKE_C_COMPILER_FRONTEND_VARIANT}")
else ()
# При автоопределении, идеальный позитивный тест не должен порождать
# предупреждений при вызове компилятора по умолчанию (или с запрошенными
# ключами по TAC_ENABLE_WARNINGS).
#
# Т.е. для MSVC, получаем WARN_ERROR_ON_STRUCT_STATIC_ASSERT, а для Clang-21
# можем получить WARN_HAVE_VLA_CXX (при `-DCMAKE_CXX_FLAGS=-std=c++23`).
#
# NO_ERROR_ON_STRUCT_STATIC_ASSERT - отсутствие ошибки без `-DTAC_DONT_FAIL`;
# WARN_ERROR_ON_STRUCT_STATIC_ASSERT - предупреждений с `-DTAC_DONT_FAIL`;
# нет - отсутствие предупреждений с `-DTAC_DONT_FAIL` и ошибка без него;
#
# HAVE_VLA_CXX - тест C++ VLA прошёл без предупреждений;
# WARN_HAVE_VLA_CXX - тест C++ VLA прошёл, но с предупреждениями;
# нет - тест не прошёл;
#
# HAVE_BROKEN_VLA - тест C VLA не прошёл;
# WARN_HAVE_BROKEN_VLA - тест C VLA прошёл, но с предупреждениями;
# нет - тест прошёл.
if (MSVC)
    set(TAC_AC_WERROR -WX)
else ()
    set(TAC_AC_WERROR -Werror)
endif ()
if (TAC_POSITIVE_WERROR)
    # Для всех остальных тестов и примеров это опционально
    set(TAC_WERROR "${TAC_AC_WERROR}")
endif ()
#if (CMAKE_C_COMPILER_ID STREQUAL Intel)
#    # TODO: skip C++ for oldest Intel icpc, my local troubles XXX
#    set(CXX_ENABLED FALSE)
#else ()
    set(CXX_ENABLED TRUE)
#endif ()
if (CMAKE_C_COMPILER_ID MATCHES "SunPro")
    string(APPEND cmn_flags " -features=extensions,zla")
    string(APPEND CMAKE_C_FLAGS " -erroff=E_ZERO_SIZED_STRUCT_UNION"
                                ",E_EMPTY_INITIALIZER")
endif ()
if (MSVC)
    string(APPEND cmn_flags " -D_CRT_SECURE_NO_WARNINGS")
    if (MSVC_VERSION GREATER_EQUAL 1914)
        # https://gitlab.kitware.com/cmake/cmake/-/issues/18837
        string(APPEND cmn_flags " /Zc:__cplusplus")
    endif ()
endif ()
if (TAC_ENABLE_WARNINGS)
    if (MSVC)
        string(APPEND cmn_flags " /W4 /wd4127 ")
    elseif (CMAKE_C_COMPILER_ID STREQUAL "GNU")
        string(APPEND cmn_flags " -Wall -Wextra -Wno-vla")
    elseif (CMAKE_C_COMPILER_ID MATCHES "Clang$" OR
            CMAKE_C_COMPILER_ID MATCHES "IntelLLVM")
        string(APPEND cmn_flags " -Wall -Wextra -pedantic"
                                " -Wno-unknown-warning-option"
                                " -Wno-c23-extensions"  # TODO
                                " -Wno-c2y-extensions"  # countof()
                                " -Wno-c99-extensions"  # C++ flexible array members
                                " -Wno-division-by-zero"  # TODO
                                " -Wno-flexible-array-extensions"
                                " -Wno-gnu-empty-initializer"
                                " -Wno-gnu-empty-struct"
                                " -Wno-gnu-flexible-array-union-member"
                                " -Wno-vla-cxx-extension"
                                " -Wno-zero-length-array"
                                " -ferror-limit=9999")
    elseif (CMAKE_C_COMPILER_ID MATCHES "SunPro")
        string(APPEND cmn_flags " -Wall -Wextra -pedantic -errtags")
        string(APPEND CMAKE_C_FLAGS " -errtags"
                                    " -erroff=E_KW_IS_AN_EXTENSION_OF_ANSI"
                                            ",E_NONPORTABLE_BIT_FIELD_TYPE"
                                            ",E_END_OF_LOOP_CODE_NOT_REACHED"
                                            ",E_STATEMENT_NOT_REACHED")
    elseif (CMAKE_C_COMPILER_ID MATCHES "NVHPC")
        string(APPEND cmn_flags " -Wall -Wextra -pedantic"
                                " --diag_suppress warning_directive"
                                " --diag_suppress no_named_fields")
    else ()
        string(APPEND cmn_flags " -Wall -Wextra -pedantic")
    endif ()
endif ()
string(APPEND CMAKE_C_FLAGS " ${cmn_flags}")
string(APPEND CMAKE_CXX_FLAGS " ${cmn_flags}")
endif () # TODO Pelles XXX Remove or?

set(tac_checks        have_typeof
                      have___typeof__
                      have_broken___typeof__
                      have___typeof_unqual__
                      have_alone_flexible_array
                      have_array_extent_cxx
                      have_builtin_constant_p_cxx
                      have_hidden_builtin_constant_p_cxx
                      no_have_broken_builtin_constant_p_cxx
                      have_strange_builtin_constant_p_cxx
                      have_builtin_types_compatible_p
                      have_hidden_builtin_types_compatible_p
                      no_have_broken_builtin_types_compatible_p
                      have_countof
                      have_countof_cxx
                      have_empty_structure
                      have_is_array_cxx
                      have_hidden_is_array_cxx
                      have_is_same_as_cxx
                      have_hidden_is_same_as_cxx
                      have_is_same_cxx
                      have_hidden_is_same_cxx
                      have_is_same
                      have_hidden_is_same
                      have___stdc_no_vla__
                      no_have_broken_vla
                      have_vla0
                      have_vla0_cxx
                      have_vla_cxx
                      no_have_broken_vla_cxx
                      have_vla_zla
                      have_zla
                      have_zla_empty_initializer
                      have_zla_empty_initializer_cxx
                      have_zla_zla)

set(tac_error_checks  error_on_generic
                      error_on_negative_array_size
                      #error_on_pointer_subtraction  # deprecated, !constexpr
                      error_on_sizeof_pointer_subtraction
                      error_on_struct_bit_field
                      error_on_struct_static_assert)

function (tac_register var)
    # TODO message("tac_register ${var}")
    set(${var} TRUE PARENT_SCOPE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D${var}" PARENT_SCOPE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D${var}" PARENT_SCOPE)
    # TODO list(APPEND COMPILE_DEFINITIONS ${CCHK})
    # TODO add_compile_definitions(${CCHK})
endfunction ()

function (tac_report rep)
    foreach (chk IN ITEMS ${tac_checks} ${tac_error_checks})
        string(TOUPPER "${chk}" CHK)
        if (${${CHK}})
            string(APPEND out "${CHK}\n")
        endif ()
        if (${NO_${CHK}})
            string(APPEND out "NO_${CHK}\n")
        endif ()
        if (${WARN_${CHK}})
            string(APPEND out "WARN_${CHK}\n")
        endif ()
        string(REGEX REPLACE "^NO_" "" NO_NO_CHK "${CHK}")
        if (${${NO_NO_CHK}})
            if (NOT CHK STREQUAL NO_NO_CHK)
                string(APPEND out "${NO_NO_CHK}\n")
            endif ()
        endif ()
    endforeach ()
    set("${rep}" "${out}" PARENT_SCOPE)
endfunction ()

foreach (cchk IN ITEMS ${tac_checks} ${tac_error_checks})
    if (cchk MATCHES "_cxx$")
        set(src "${TAC_SOURCE_DIR}/${cchk}.cpp")
    else ()
        set(src "${TAC_SOURCE_DIR}/${cchk}.c")
    endif ()
    set(cchks ${cchk})
    if ("${cchk}" MATCHES "^error_")
        list(PREPEND cchks "have_${cchk}")
    endif ()
    foreach (chk IN ITEMS ${cchks})
        if ("${compile_${chk}}" STREQUAL "")
            if ("${chk}" MATCHES "^have_")
                set(df "${TAC_AC_WERROR} -DTAC_DONT_FAIL")
            else ()
                set(df "")
            endif ()
            if (NOT ${chk} MATCHES "^have_error_")
                message("Detecting ${chk}")
            endif ()
            try_run(run_${chk} compile_${chk} "${CMAKE_CURRENT_BINARY_DIR}"
                    SOURCES "${src}"
                    COMPILE_DEFINITIONS "${df}"
                    COMPILE_OUTPUT_VARIABLE cml_out
                    RUN_OUTPUT_VARIABLE run_out
                    )
            if (TAC_VERBOSE)
                message("==== run_${chk}=${run_${chk}}"
                        " compile_${chk}=${compile_${chk}}"
                        " compile:\n${cml_out}"
                        "---- run_${chk} run:\n${run_out}")
            endif ()
            if ("${chk}" MATCHES "^have_" AND
                (NOT "${run_${chk}}" EQUAL 0 OR NOT "${compile_${chk}}"))
                try_run(run_warn_${chk} compile_warn_${chk}
                        "${CMAKE_CURRENT_BINARY_DIR}"
                        SOURCES "${src}"
                        COMPILE_DEFINITIONS "-DTAC_DONT_FAIL"
                        COMPILE_OUTPUT_VARIABLE cml_warn_out
                        RUN_OUTPUT_VARIABLE run_warn_out
                        )
                if (TAC_VERBOSE)
                    message("==== run_warn_${chk}=${run_warn_${chk}}"
                            " compile_warn_${chk}=${compile_warn_${chk}}"
                            " compile:\n${cml_warn_out}"
                            "---- run_warn_${chk} run:\n${run_warn_out}")
                endif ()
            endif ()
        endif ()
    endforeach ()
    string(TOUPPER "${cchk}" CCHK)
    if ("${cchk}" MATCHES "^error_")
        if ("${compile_${cchk}}")
            tac_register(NO_${CCHK})
        elseif (NOT "${run_have_${cchk}}" EQUAL 0 OR
                NOT "${compile_have_${cchk}}")
            if ("${run_warn_have_${cchk}}" EQUAL 0 AND
                "${compile_warn_have_${cchk}}")
                tac_register(WARN_${CCHK})
            else ()
                tac_register(UNDETECTED_${CCHK})
            endif ()
        endif ()
    elseif (NOT cchk MATCHES "^no_")
        if ("${run_${cchk}}" EQUAL 0 AND "${compile_${cchk}}")
            tac_register(${CCHK})
        elseif ("${run_warn_${cchk}}" EQUAL 0 AND "${compile_warn_${cchk}}")
            tac_register(WARN_${CCHK})
        endif ()
    else ()
        string(REGEX REPLACE "^NO_" "" NO_NO_CCHK "${CCHK}")
        if (NOT "${run_${cchk}}" EQUAL 0 OR NOT "${compile_${cchk}}")
            if (NOT "${run_warn_${cchk}}" EQUAL 0 OR
                NOT "${compile_warn_${cchk}}")
                tac_register(${NO_NO_CCHK})
            else ()
                tac_register(WARN_${NO_NO_CCHK})
            endif ()
        endif ()
    endif ()
endforeach ()

message("CMAKE_C_COMPILER_ID=${CMAKE_C_COMPILER_ID}"
        " CMAKE_C_COMPILER_VERSION=${CMAKE_C_COMPILER_VERSION}"
        " CMAKE_C_COMPILER_FRONTEND_VARIANT=${CMAKE_C_COMPILER_FRONTEND_VARIANT}")
tac_report(rep)
message("${rep}")
if (NOT HAVE_TYPEOF AND NOT HAVE___TYPEOF__)
    message(FATAL_ERROR "Don't have `typeof()` or `__typeof__()`")
endif ()
if (HAVE_BROKEN___TYPEOF__)
    message("TODO: ADD -D_COUNTOF_NS_BROKEN_TYPEOF_WORKAROUND")
    string(APPEND CMAKE_C_FLAGS " -D_COUNTOF_NS_BROKEN_TYPEOF_WORKAROUND")  # TODO XXX
endif ()
