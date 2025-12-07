# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

# Positive and check tests must compile and successful execute (for VLA used
# runtime asserts). They should return Ok, a small number, and debugging
# information (Warning: TODO: If there is a build error, it's still about
# "Required regular expression not found...").

file(GLOB check_hdr RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "check_*.h")
file(GLOB pos_hdr RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "pos_*.h")
set(pos_pass_regexp "Ok [0-9] TU_[A-Z_]*ASSERT_AND_RETURN")

# Negative tests must don't compile. But, for comparison purposes, we are
# trying to find out what a wrongly compiled test does.

file(GLOB neg_hdr RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "neg_*.h")
set(neg_not_terrifying_regexp "Fail 0 desired=")

function(filter_have possible cases)
    set(p "")
    foreach (cc IN ITEMS ${cases})
        get_filename_component(c ${cc} NAME_WLE)
        if (("${c}" MATCHES "_vla" AND NOT HAVE_VLA) OR
            ("${c}" MATCHES "_zla" AND NOT HAVE_ZERO_LENGTH_ARRAYS) OR
            ("${c}" MATCHES "_struct" AND NOT HAVE_EMPTY_STRUCTURE) OR
            ("${c}" MATCHES "_alone" AND NOT HAVE_ALONE_FLEXIBLE_ARRAY))
            continue ()
        endif ()
        list(APPEND p ${c})
    endforeach ()
    set("${possible}" "${p}" PARENT_SCOPE)
endfunction ()

filter_have(pos_pos "${pos_hdr}")  # TODO "${check_hdr};${pos_hdr}")
filter_have(neg_pos "${neg_hdr}")
