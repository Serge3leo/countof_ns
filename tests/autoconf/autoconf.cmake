# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

# Detect default compiler capability (no options) for tests

set(TAC_PRE_CHECK "-DTAC_DONT_FAIL")
if (MSVC)
    set(TAC_ADD_DEFINITIONS -W4 -D_CRT_SECURE_NO_WARNINGS)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(TAC_ADD_DEFINITIONS -Wall -Wextra -pedantic -Wno-unknown-warning-option
                            -Wno-gnu-empty-struct -Wno-c2y-extensions
                            -Wno-c23-extensions -Wno-zero-length-array
                            -Wno-gnu-empty-initializer)
else()
    set(TAC_ADD_DEFINITIONS -Wall -Wextra)
endif()

set(tac_checks        have_zero_length_arrays
                      have_alone_flexible_array have_countof have_countof_zla
                      have_countof_vla have_cv_typeof have_empty_initializer
                      have_empty_structure have_vla)

set(tac_error_checks  error_on_negative_array_size
                      # error_on_pointer_subtraction  # deprecated, !constexpr
                      error_on_sizeof_pointer_subtraction
                      error_on_struct_bit_field error_on_struct_static_assert)


function(check_cmpl status res out)
    if(NOT "${res}")
        set(sts "FAIL")
    else()
        string(TOLOWER "${out}" lout)
        # TODO: A false negative result may be detected, workaround:
        # ...gmake[1]: warning:  Clock skew detected...
        # ...gmake[1]: Warning: File ... has
        # modification time 0.047 s in the future...
        string(REGEX REPLACE "make[^\n]*warning" "" lout "${lout}")
        if("${lout}" MATCHES " error[: ]")
            set(sts "ERRORS")
        elseif("${lout}" MATCHES " warning[: ]")
            set(sts "WARNINGS")
        else()
            set(sts "OK")
        endif()
    endif()
    set(${status} "${sts}" PARENT_SCOPE)
endfunction()

foreach(chk IN ITEMS ${tac_checks} ${tac_error_checks})
    if("${chk}" MATCHES "^error_")
        try_run(run cmpl SOURCES "${TAC_SOURCE_DIR}/${chk}.c"
            COMPILE_DEFINITIONS ${TAC_PRE_CHECK}
                                ${TAC_ADD_DEFINITIONS} ${TAC_DEFINITIONS}
                COMPILE_OUTPUT_VARIABLE cmpl_out
                RUN_OUTPUT_VARIABLE run_out
                )
        check_cmpl(sts "${cmpl}" "${cmpl_out}")
        # message("\n====\n${chk} sts=${sts} cmpl=${cmpl}\n${cmpl_out}\n====")
        if((NOT "${sts}" STREQUAL OK) OR (NOT "${run}" EQUAL 0))
            message("${chk}: SKIP on pre-check")
            continue()
        endif()
    endif()
    try_run(run cmpl SOURCES "${TAC_SOURCE_DIR}/${chk}.c"
            COMPILE_DEFINITIONS ${TAC_ADD_DEFINITIONS} ${TAC_DEFINITIONS}
            COMPILE_OUTPUT_VARIABLE cmpl_out
            RUN_OUTPUT_VARIABLE run_out
            )
    string(TOUPPER "${chk}" def)
    check_cmpl(sts "${cmpl}" "${cmpl_out}")
    # message("\n====\n${chk} sts=${sts} cmpl=${cmpl}\n${cmpl_out}\n====")
    if((("${chk}" MATCHES "^error_") AND ("${sts}" STREQUAL FAIL)) OR
       (("${chk}" MATCHES "^have_") AND
        ("${sts}" STREQUAL OK) AND ("${run}" EQUAL 0)))
        list(APPEND TAC_DEFINITIONS "-D${def}")
        message("${chk}: ok")
    else()
        message("${chk}: NONE")
    endif()
endforeach()
