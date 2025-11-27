# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

# Detect default compiler capability (no options) for tests

if(CMAKE_C_COMPILER_ID STREQUAL "") # TODO Pelles XXX Remove or?
    set(CXX_ENABLED FALSE)
    set(CMAKE_C_COMPILER_ID Pelles)
    #set(CMAKE_C_COMPILER_FRONTEND_VARIANT "MSVC")
    set(CMAKE_C_COMPILER_ID "${CMAKE_C_COMPILER_ID}")
    set(CMAKE_C_COMPILER_FRONTEND_VARIANT
        "${CMAKE_C_COMPILER_FRONTEND_VARIANT}")
    message("WARNING: chage to CMAKE_C_COMPILER_ID=${CMAKE_C_COMPILER_ID} "
     "CMAKE_C_COMPILER_FRONTEND_VARIANT=${CMAKE_C_COMPILER_FRONTEND_VARIANT}")
else()

if(CMAKE_C_COMPILER_ID STREQUAL Intel)
    # TODO: skip C++ for oldest Intel icpc, my local troubles XXX
    set(CXX_ENABLED FALSE)
else()
    set(CXX_ENABLED TRUE)
endif()

if (MSVC)
    # TODO set_property(my_app COMPILE_WARNING_AS_ERROR ON)
    set(TAC_WERROR -WX)
else()
    set(TAC_WERROR -Werror)
endif()
if (MSVC)
    string(APPEND cmn_flags "-W4 -D_CRT_SECURE_NO_WARNINGS")
    if (MSVC_VERSION GREATER_EQUAL 1914)
        # https://gitlab.kitware.com/cmake/cmake/-/issues/18837
        string(APPEND cmn_flags " /Zc:__cplusplus")
    endif()
elseif(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    string(APPEND cmn_flags "-Wall -Wextra")
elseif(CMAKE_C_COMPILER_ID MATCHES "Clang$" OR
       CMAKE_C_COMPILER_ID MATCHES "^Intel")
    string(APPEND cmn_flags "-Wall -Wextra -pedantic"
                            " -Wno-unknown-warning-option"
                            " -Wno-c23-extensions"  # TODO
                            " -Wno-c2y-extensions"  # countof()
                            " -Wno-c99-extensions"  # C++ flexible array members
                            " -Wno-flexible-array-extensions"
                            " -Wno-gnu-empty-initializer"
                            " -Wno-gnu-empty-struct"
                            " -Wno-gnu-flexible-array-union-member"
                            " -Wno-zero-length-array"
                            " -ferror-limit=9999")
elseif(CMAKE_C_COMPILER_ID MATCHES "SunPro")
    string(APPEND cmn_flags "-Wall -Wextra -pedantic")
    string(APPEND CMAKE_C_FLAGS " -errtags"
                                " -erroff=E_KW_IS_AN_EXTENSION_OF_ANSI"
                                        ",E_NONPORTABLE_BIT_FIELD_TYPE")
elseif(CMAKE_C_COMPILER_ID MATCHES "NVHPC")
    string(APPEND cmn_flags "-Wall -Wextra -pedantic"
                            " --diag_suppress warning_directive")
else()
    string(APPEND cmn_flags "-Wall -Wextra -pedantic")
endif()
string(APPEND CMAKE_C_FLAGS " ${cmn_flags}")
string(APPEND CMAKE_CXX_FLAGS " ${cmn_flags}")

endif() # TODO Pelles XXX Remove or?

set(tac_checks        have_zero_length_arrays have_alone_flexible_array
                      have_countof  # have_countof_zla have_countof_vla
                      have_cv_typeof have_empty_initializer
                      have_empty_structure have_vla)

set(tac_error_checks  error_on_negative_array_size
                      # error_on_pointer_subtraction  # deprecated, !constexpr
                      error_on_sizeof_pointer_subtraction
                      error_on_struct_bit_field error_on_struct_static_assert)

function(tac_register var)
    set(${var} TRUE PARENT_SCOPE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D${var}=1" PARENT_SCOPE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D${var}=1" PARENT_SCOPE)
    # TODO list(APPEND COMPILE_DEFINITIONS ${CCHK})
    # TODO add_compile_definitions(${CCHK})
endfunction()

function(tac_report rep)
    foreach(chk IN ITEMS ${tac_checks} ${tac_error_checks})
        string(TOUPPER "${chk}" CHK)
        if(${${CHK}})
            string(APPEND out "${CHK}\n")
        endif()
    endforeach()
    set("${rep}" "${out}" PARENT_SCOPE)
endfunction()

foreach(cchk IN ITEMS ${tac_checks} ${tac_error_checks})
    set(src "${TAC_SOURCE_DIR}/${cchk}.c")
    set(cchks ${cchk})
    if("${cchk}" MATCHES "^error_")
        list(PREPEND cchks "have_${cchk}")
    endif()
    foreach(chk IN ITEMS ${cchks})
        if("${chk}" MATCHES "^have_")
            set(df "${TAC_WERROR} -DTAC_DONT_FAIL")
        else()
            set(df "")
        endif()
        # message("before ${chk}=${chk} run_${chk}=${run_${chk}} compile_${chk}=${compile_${chk}}")
        if("${compile_${chk}}" STREQUAL "")
            if(NOT ${chk} MATCHES "^have_error_")
                message("Detecting ${chk}")
            endif()
            try_run(run_${chk} compile_${chk} "${CMAKE_CURRENT_BINARY_DIR}"
                    SOURCES "${src}"
                    COMPILE_DEFINITIONS "${df}"
                    # COMPILE_OUTPUT_VARIABLE cout
                    # RUN_OUTPUT_VARIABLE rout
                    )
            # message("after ${cchk}=${chk} run_${chk}=${run_${chk}} compile_${chk}=${compile_${chk}}")
            # message("\n===\n${chk}, TAC_WERROR=${TAC_WERROR}, df=${df} cout:\n${cout}\n--- rout:\n${rout}\n===")
        endif()
    endforeach()
    string(TOUPPER "${cchk}" CCHK)
    if("${cchk}" MATCHES "^error_")
        # message("run_have_${cchk}=${run_have_${cchk}} ompile_have_${cchk}={compile_have_${cchk}} compile_${cchk}=${compile_${cchk}}")
        if(("${run_have_${cchk}}" EQUAL 0) AND "${compile_have_${cchk}}"
           AND NOT "${compile_${cchk}}")
            tac_register(${CCHK})
        endif()
    else()
        if("${run_${cchk}}" EQUAL 0)
            tac_register(${CCHK})
        endif()
    endif()
endforeach()

message("CMAKE_C_COMPILER_ID=${CMAKE_C_COMPILER_ID} "
     "CMAKE_C_COMPILER_FRONTEND_VARIANT=${CMAKE_C_COMPILER_FRONTEND_VARIANT}")
tac_report(rep)
message("${rep}")
