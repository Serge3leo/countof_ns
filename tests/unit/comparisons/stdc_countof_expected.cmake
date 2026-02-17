# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

if (NOT HAVE_COUNTOF)
    return ()
endif ()

function (tu_stdc_countof_expected expected pos_pos neg_pos)
    set(nexp "")
    foreach (n IN ITEMS ${neg_pos})
        list(APPEND nexp "${n}.build_fail")
    endforeach ()
    set(${expected} "${pos_pos};${nexp}" PARENT_SCOPE)
endfunction ()

tu_stdc_countof_expected(tu_stdc_countof_available
                         "${tu_pos_pos}" "${tu_neg_pos}")
set(tu_stdc_countof_params FALSE
                           "stdc_countof"
                           "countof_ns/_comparisons/stdc_countof.h"
                           tu_stdc_countof_available)
list(APPEND tu_params_list tu_stdc_countof_params)
