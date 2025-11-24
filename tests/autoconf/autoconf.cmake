# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

# Detect default compiler capability (no options) for tests

set(TAC_HAVE_ADD_DEFINITIONS "-DTAC_DONT_FAIL")
if (MSVC)
    list(APPEND TAC_HAVE_ADD_DEFINITIONS -WX)
else()
    set(TAC_ADD_DEFINITIONS -Wall -Wextra -)
    list(APPEND TAC_HAVE_ADD_DEFINITIONS -Werror)
endif()
if (MSVC)
    set(TAC_ADD_DEFINITIONS -W4 -D_CRT_SECURE_NO_WARNINGS)
    if (MSVC_VERSION GREATER_EQUAL 1914)
            # https://gitlab.kitware.com/cmake/cmake/-/issues/18837
        list(APPEND TAC_HAVE_ADD_DEFINITIONS "/Zc:__cplusplus")
    endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(TAC_ADD_DEFINITIONS -Wall -Wextra)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang$" OR
       CMAKE_CXX_COMPILER_ID MATCHES "^Intel")
    set(TAC_ADD_DEFINITIONS -Wall -Wextra -pedantic -Wno-unknown-warning-option
                            -Wno-c23-extensions  # TODO
                            -Wno-c2y-extensions  # countof()
                            -Wno-c99-extensions  # C++ flexible array members
                            -Wno-flexible-array-extensions
                            -Wno-gnu-empty-initializer
                            -Wno-gnu-empty-struct
                            -Wno-gnu-flexible-array-union-member
                            -Wno-zero-length-array)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "SunPro")
    set(TAC_ADD_DEFINITIONS -Wall -Wextra -pedantic)
    set(TAC_ADD_C_DEFINITIONS -errtags
            -erroff=E_KW_IS_AN_EXTENSION_OF_ANSI,E_NONPORTABLE_BIT_FIELD_TYPE)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "NVHPC")
    set(TAC_ADD_DEFINITIONS -Wall -Wextra -pedantic
                            --diag_suppress warning_directive)
else()
    set(TAC_ADD_DEFINITIONS -Wall -Wextra -pedantic)
endif()
message("CMAKE_CXX_COMPILER_ID=${CMAKE_CXX_COMPILER_ID}")

set(tac_checks        have_zero_length_arrays have_alone_flexible_array
                      have_countof  # have_countof_zla have_countof_vla
                      have_cv_typeof have_empty_initializer
                      have_empty_structure have_vla)

set(tac_error_checks  error_on_negative_array_size
                      # error_on_pointer_subtraction  # deprecated, !constexpr
                      error_on_sizeof_pointer_subtraction
                      error_on_struct_bit_field error_on_struct_static_assert)

foreach(cchk IN ITEMS ${tac_checks} ${tac_error_checks})
    set(src "${TAC_SOURCE_DIR}/${cchk}.c")
    set(cchks ${cchk})
    if("${cchk}" MATCHES "^error_")
        list(PREPEND cchks "have_${cchk}")
    endif()
    foreach(chk IN ITEMS ${cchks})
        set(cd ${TAC_ADD_DEFINITIONS} ${TAC_DEFINITIONS})
        if("${chk}" MATCHES "^have_")
            list(PREPEND cd ${TAC_HAVE_ADD_DEFINITIONS}
                 ${TAC_ADD_C_DEFINITIONS})
        endif()
        # message("before ${chk}=${chk} run_${chk}=${run_${chk}} compile_${chk}=${compile_${chk}}")
        if("${compile_${chk}}" STREQUAL "")
            message("Detecting ${chk}")
            try_run(run_${chk} compile_${chk} "${CMAKE_CURRENT_BINARY_DIR}"
                    SOURCES "${src}"
                    COMPILE_DEFINITIONS ${cd})
            # message("after ${cchk}=${chk} run_${chk}=${run_${chk}} compile_${chk}=${compile_${chk}}")
        endif()
    endforeach()
    string(TOUPPER "${cchk}" CCHK)
    if("${cchk}" MATCHES "^error_")
        # message("run_have_${cchk}=${run_have_${cchk}} ompile_have_${cchk}={compile_have_${cchk}} compile_${cchk}=${compile_${cchk}}")
        if(("${run_have_${cchk}}" EQUAL 0) AND "${compile_have_${cchk}}"
           AND NOT "${compile_${cchk}}")
            list(APPEND TAC_DEFINITIONS "-D${CCHK}=1")
            string(APPEND TAC_REPORT "${CCHK}\n")
        endif()
    else()
        if("${run_${cchk}}" EQUAL 0)
            list(APPEND TAC_DEFINITIONS "-D${CCHK}=1")
            string(APPEND TAC_REPORT "${CCHK}\n")
        endif()
    endif()
endforeach()
message("${TAC_REPORT}")
