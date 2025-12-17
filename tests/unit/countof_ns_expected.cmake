# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

function (tu_countof_ns_expected expected pos_pos neg_pos)
    set(pos_base "")
    foreach (b IN ITEMS ${pos_pos})
        if (b MATCHES "vla")
            set(ints ${b}.build_fail ${b}.c11 ${b}.bltn ${b}_cxx.build_fail)
            if (HAVE_IS_SAME_CXX OR HAVE_HIDDEN_IS_SAME_CXX)
                list(APPEND ints ${b}_cxx.bltn)
            endif ()
        elseif (HAVE_VLA)
            set(ints ${b} ${b}.c11 ${b}.bltn ${b}_cxx)
            if (HAVE_IS_SAME_CXX OR HAVE_HIDDEN_IS_SAME_CXX)
                list(APPEND ints ${b}_cxx.bltn)
            endif ()
        else ()
            set(ints ${b} ${b}_cxx)
        endif ()
        foreach (i IN ITEMS ${ints})
            if (i MATCHES "struct_n0_cxx")
                list(APPEND pos_base ${i})
            elseif (i MATCHES "_vla.*_n0" AND NOT i MATCHES "[.]build_fail")
                list(APPEND pos_base ${i}.run_0_unexpected)
            elseif (i MATCHES "_n0" AND NOT i MATCHES "_cxx" AND
                    NOT i MATCHES "[.]build_fail")
                list(APPEND pos_base ${i}.build_fail)
            else ()
                list(APPEND pos_base ${i})
            endif ()
        endforeach ()
    endforeach ()

    set(neg_base "")
    foreach (b IN ITEMS ${neg_pos})
        if (HAVE_VLA)
            set(ints ${b}.build_fail ${b}.c11.build_fail ${b}.bltn.build_fail
                     ${b}_cxx.build_fail)
            if (HAVE_IS_SAME_CXX OR HAVE_HIDDEN_IS_SAME_CXX)
                list(APPEND ints ${b}_cxx.bltn.build_fail)
            endif ()
        else ()
            set(ints ${b}.build_fail ${b}_cxx.build_fail)
        endif ()
        if (NOT NO_ERROR_ON_SIZEOF_POINTER_SUBTRACTION)
            list(APPEND neg_base ${ints})
        else ()
            foreach (i IN ITEMS ${ints})
                if (i MATCHES "[.]c11" AND
                    NOT i MATCHES "__selftest|_other")
                    list(APPEND neg_base ${b}.c11.build_unexpected
                                         ${b}.c11.run_fail)
                else ()
                    list(APPEND neg_base ${i})
                endif ()
            endforeach ()
        endif ()
    endforeach ()
    if (CMAKE_C_COMPILER_ID STREQUAL NVHPC OR
        CMAKE_C_COMPILER_ID STREQUAL Intel OR
        CMAKE_C_COMPILER_ID STREQUAL LCC)
            # TODO I don't understand. Why doesn't `_Generic()` reject a VLA
            # array?  Is this a peculiarity of the C language extensions only
            # for NVHPC (pgcc) and the classic Intel (icc)? Or is it the result
            # of optimizations?
        string(REGEX REPLACE "(pos_vla_(struct|zla)_00)\.build_fail" "\\1"
                             pos_base "${pos_base}")
        string(REGEX REPLACE "(pos_vla_(struct|zla)_n0)\.build_fail"
                             "\\1.run_fail"
                             pos_base "${pos_base}")
    endif ()
    if (CMAKE_CXX_COMPILER_ID STREQUAL GNU AND
        CMAKE_CXX_COMPILER_VERSION MATCHES "^1[1-5]\.")
        string(REGEX REPLACE "(pos_vla_(alone|zla)_[0n]0_cxx(\.bltn|))"
                             "\\1.compiler_bug"
                             pos_base "${pos_base}")
    endif ()
    if ((CMAKE_CXX_COMPILER_ID STREQUAL NVHPC AND
         CMAKE_CXX_COMPILER_VERSION MATCHES "^25\.9\." ) OR
        (CMAKE_CXX_COMPILER_ID STREQUAL Intel AND
         CMAKE_CXX_COMPILER_VERSION MATCHES "^2021\." ))
        string(REGEX REPLACE "(pos_vla_cv_cxx.bltn)" "\\1.compiler_bug"
                             pos_base "${pos_base}")
    endif ()

    set(${expected} "${pos_base};${neg_base}" PARENT_SCOPE)
endfunction ()

tu_countof_ns_expected(tu_countof_ns_available "${tu_pos_pos}" "${tu_neg_pos}")
set(tu_countof_ns_params TRUE "countof_ns" "countof_ns.h"
                         tu_countof_ns_available)
list(APPEND tu_params_list tu_countof_ns_params)
