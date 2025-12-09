# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

function (tu_jms_countof_expected expected pos_pos neg_pos)
    set(pos_base "")
    foreach (b IN ITEMS ${pos_pos})
        if (b MATCHES "pos_struct_n0")
            list(APPEND pos_base ${b}_cxx)
        elseif (b MATCHES "_vla.*_[n0]0")
            list(APPEND pos_base ${b}.run_fail)
        elseif (b MATCHES "_n0")
            list(APPEND pos_base ${b}.build_fail ${b}_cxx)
        elseif (b MATCHES "_00")
            list(APPEND pos_base ${b}.build_fail ${b}_cxx.build_fail)
        elseif (b MATCHES "_0n")
            list(APPEND pos_base ${b} ${b}_cxx.build_fail)
        elseif (b MATCHES "_vla")
            list(APPEND pos_base ${b})
        else ()
            list(APPEND pos_base ${b} ${b}_cxx)
        endif ()
    endforeach ()
    set(neg_base "")
    foreach (b IN ITEMS ${neg_pos})
        if (b MATCHES "(__selftest$|_other$|_alone_ptr$)")
            list(APPEND neg_base ${b}.build_fail ${b}_cxx.build_fail)
        else ()
            list(APPEND neg_base ${b}.build_unexpected ${b}.run_fail
                                 ${b}_cxx.build_fail)
        endif ()
    endforeach ()
    if (CMAKE_C_COMPILER_ID STREQUAL NVHPC AND HAVE_BROKEN_VLA)
        string(REGEX REPLACE "pos_vla_0n(;|$)" "pos_vla_0n.run_fail\\1"
                             pos_base "${pos_base}")
    endif ()
    if (CMAKE_C_COMPILER_ID STREQUAL Intel)
            # Zero is result of zero-by-zero division on old Intel.
        string(REGEX REPLACE "_00.run_fail" "_00.run_0_unexpected"
                             pos_base "${pos_base}")
    endif ()
    set(${expected} "${pos_base};${neg_base}" PARENT_SCOPE)
endfunction ()

tu_jms_countof_expected(tu_jms_countof_available
                           "${tu_pos_pos}" "${tu_neg_pos}")
set(tu_jms_countof_params FALSE
                          "ms_countof" "_comparisons/ms_countof.h"
                          tu_jms_countof_available)
list(APPEND tu_params_list tu_jms_countof_params)
