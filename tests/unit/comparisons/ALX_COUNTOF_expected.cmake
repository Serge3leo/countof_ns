# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

if (NOT (HAVE_BUILTIN_TYPES_COMPATIBLE_P AND HAVE_TYPEOF))
    return ()
endif ()

try_compile(HAVE_ALX_COUNTOF
                SOURCE_FROM_CONTENT builtin_types_compatible_p.c [=[
            #include <assert.h>
            #if __has_builtin(__builtin_types_compatible_p)
                // ALX_COUNTOF() use `static_assert()`, not `_Static_assert()`
                static_assert(1);
                int main(void) {}
            #endif
            ]=])
if (NOT HAVE_ALX_COUNTOF)
    message("Don't HAVE_ALX_COUNTOF")
endif ()

function (tu_alx_countof_expected expected pos_pos neg_pos)
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

tu_alx_countof_expected(tu_alx_countof_available
                           "${tu_pos_pos}" "${tu_neg_pos}")
set(tu_alx_countof_params FALSE
                          "ALX_COUNTOF" "_comparisons/ALX_COUNTOF.h"
                          tu_alx_countof_available)
list(APPEND tu_params_list tu_alx_countof_params)
