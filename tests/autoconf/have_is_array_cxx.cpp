// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

#include "tac_defs.h"

#ifdef __has_builtin
    #if __has_builtin(__is_array)
        #define ia_is_array  __is_array
    #endif
#endif

TAC_CHECK_FUNC(ia_foo) {
    int a[2];
    static_assert(ia_is_array(decltype(a)));
    (void)a;
}
