// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

#include "tac_defs.h"

#ifdef __has_builtin
    #if __has_builtin(__is_same)
        #define is_is_same  __is_same
    #endif
#endif

TAC_CHECK_FUNC(is_foo) {
    int a;
    static_assert(is_is_same(decltype(a), int));
    (void)a;
}
