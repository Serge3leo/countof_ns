# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

# Positive tests must compile and successful execute (for VLA used runtime
# asserts). They should return Ok, a small number, and debugging information
# (Warning: TODO: If there is a build error, it's still about "Required regular
# expression not found...").

file(GLOB tu_pos_hdr RELATIVE "${CMAKE_CURRENT_LIST_DIR}"
                              "${CMAKE_CURRENT_LIST_DIR}/pos_*.h")
set(tu_pos_pass_regexp "Ok [0-9] TU_[A-Z_]*ASSERT_AND_RETURN")

# Negative tests must don't compile. But, for comparison purposes, we are
# trying to find out what a wrongly compiled test does.

file(GLOB tu_neg_hdr RELATIVE "${CMAKE_CURRENT_LIST_DIR}"
                              "${CMAKE_CURRENT_LIST_DIR}/neg_*.h")
set(tu_neg_not_terrifying_regexp "(Fail 0 desired=|Ok 0 TU_)")
set(tu_fail_regexp
    "(Ok .[0-9]|Ok[^ ]|Ok [^0-9]|Fail[^ ]|Fail [^0]|Fail 0[^ ])") # TODO

function(tu_filter_have possible cases)
    set(p "")
    foreach (cc IN ITEMS ${cases})
        get_filename_component(c ${cc} NAME_WLE)
        if (("${c}" MATCHES "_vla" AND HAVE___STDC_NO_VLA__) OR
            ("${c}" MATCHES "_zla" AND NOT HAVE_ZLA) OR
            ("${c}" MATCHES "_struct" AND NOT HAVE_EMPTY_STRUCTURE) OR
            ("${c}" MATCHES "_alone" AND NOT HAVE_ALONE_FLEXIBLE_ARRAY) OR
            ("${c}" MATCHES "_(zla|alone|struct).*[0n][0n]" AND
             NOT HAVE_ZLA_ZLA))
            continue ()
        endif ()
        list(APPEND p ${c})
    endforeach ()
    set("${possible}" "${p}" PARENT_SCOPE)
endfunction ()

tu_filter_have(tu_pos_pos "${tu_pos_hdr}")
tu_filter_have(tu_neg_pos "${tu_neg_hdr}")
