# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

if (NOT (HAVE_TYPEOF OR HAVE___TYPEOF__))
    return ()
endif ()

function (tu_jzmg_array_len_expected expected pos_pos neg_pos)
    set(pos_base "")
    foreach (b IN ITEMS ${pos_pos})
        if (b MATCHES
            "(pos_struct_n0|pos_zla_00|pos_zla_alone_00|pos_zla_struct_00)")
            list(APPEND pos_base ${b}_cxx)
        elseif (b MATCHES "_vla.*_[n0]0")
            list(APPEND pos_base ${b}.run_fail)
        elseif (b MATCHES "_[n0]0")
            list(APPEND pos_base ${b}.build_fail ${b}_cxx.build_fail)
        elseif (b MATCHES "_vla")
            list(APPEND pos_base ${b})
        else ()
            list(APPEND pos_base ${b} ${b}_cxx)
        endif ()
    endforeach ()
    set(neg_base "")
    foreach (b IN ITEMS ${neg_pos})
        if (ERROR_ON_SIZEOF_POINTER_SUBTRACTION OR
            b MATCHES "(__selftest$|_other$)")
            list(APPEND neg_base ${b}.build_fail ${b}_cxx.build_fail)
        else ()
            list(APPEND neg_base ${b}.build_unexpected ${b}.run_fail
                                 ${b}_cxx.build_fail)
        endif ()
    endforeach ()
    if (CMAKE_C_COMPILER_ID STREQUAL NVHPC AND HAVE_BROKEN_VLA)
        string(REGEX REPLACE "pos_vla_0n" "pos_vla_0n.run_fail"
                             pos_base "${pos_base}")
    endif ()
    set(${expected} "${pos_base};${neg_base}" PARENT_SCOPE)
endfunction ()

tu_jzmg_array_len_expected(tu_jzmg_array_len_available
                           "${tu_pos_pos}" "${tu_neg_pos}")
set(tu_jzmg_array_len_params FALSE
                             "JZMG_ARRAY_LEN" "_comparisons/JZMG_ARRAY_LEN.h"
                             tu_jzmg_array_len_available)
list(APPEND tu_params_list tu_jzmg_array_len_params)
