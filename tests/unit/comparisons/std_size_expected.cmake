# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

if (NOT CXX_ENABLED)
    return ()
endif ()

try_compile(HAVE_STD_SIZE SOURCE_FROM_CONTENT have_std_size.cpp [=[
            #include <vector>
            int a[1];
            static_assert(1 == std::size(a));
            int main(void) {}
            ]=])

if (NOT HAVE_STD_SIZE)
    message("Don't HAVE_STD_SIZE")
    return ()
endif ()

function (tu_std_size_expected expected pos_pos neg_pos)
    set(pos_base "")
    foreach (b IN ITEMS ${pos_pos})
        if (NOT b MATCHES "_vla")  # TODO extension C++ VLA
            if (b MATCHES "_zla" AND NOT b MATCHES "_zla_n0")
                list(APPEND pos_base ${b}_cxx.build_fail)
            else ()
                list(APPEND pos_base ${b}_cxx)
            endif ()
        endif ()
    endforeach ()
    set(neg_base "")
    foreach (b IN ITEMS ${neg_pos})
        if (NOT b MATCHES "_vla")  # TODO extension C++ VLA
            list(APPEND neg_base ${b}_cxx.build_fail)
        endif ()
    endforeach ()
    set(${expected} "${pos_base};${neg_base}" PARENT_SCOPE)
endfunction ()

tu_std_size_expected(tu_std_size_available
                     "${tu_pos_pos}" "${tu_neg_pos}")
set(tu_std_size_params FALSE
                       "std_size" "_comparisons/std_size.h"
                       tu_std_size_available)
list(APPEND tu_params_list tu_std_size_params)
