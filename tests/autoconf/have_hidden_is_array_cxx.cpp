// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

#include "tac_defs.h"

#ifdef __has_builtin
    #if __has_builtin(__is_array)
        #error "__is_array() not hidden"
    #endif
#endif

TAC_CHECK_FUNC(hia_foo) {
    int a[2];
    tac_static_assert(__is_array(decltype(a)));
    (void)a;
}
