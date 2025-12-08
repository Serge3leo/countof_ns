# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

if (NOT (HAVE_BUILTIN_TYPES_COMPATIBLE_P AND HAVE_TYPEOF AND
         HAVE_EMPTY_STRUCTURE))
    return ()
endif ()

function (tu_lnx_array_size_expected expected pos_pos neg_pos)
    set(pos_base "")
    foreach (b IN ITEMS ${pos_pos})
        if (b MATCHES "_vla.*_[n0]0")
            list(APPEND pos_base ${b}.run_fail)
        elseif (b MATCHES "_[n0]0")
            list(APPEND pos_base ${b}.build_fail)
        else ()
            list(APPEND pos_base ${b})
        endif ()
    endforeach ()
    set(neg_base "")
    foreach (b IN ITEMS ${neg_pos})
        list(APPEND neg_base ${b}.build_fail)
    endforeach ()
    if (CMAKE_C_COMPILER_ID STREQUAL NVHPC AND HAVE_BROKEN_VLA)
        string(REGEX REPLACE "pos_vla_0n" "pos_vla_0n.run_fail"
                             pos_base "${pos_base}")
    endif ()
    set(${expected} "${pos_base};${neg_base}" PARENT_SCOPE)
endfunction ()

tu_lnx_array_size_expected(tu_lnx_array_size_available
                           "${tu_pos_pos}" "${tu_neg_pos}")
set(tu_lnx_array_size_params FALSE
                             "LNX_ARRAY_SIZE" "_comparisons/lnx_array_size.h"
                             tu_lnx_array_size_available)
list(APPEND tu_params_list tu_lnx_array_size_params)
