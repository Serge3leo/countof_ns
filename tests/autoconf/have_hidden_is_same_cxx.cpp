// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

#include "tac_defs.h"

#ifdef __has_builtin
    #if __has_builtin(__is_same)
        #error "__is_same() not hidden"
    #endif
#endif

TAC_CHECK_FUNC(his_foo) {
    int a;
    tac_static_assert(__is_same(decltype(a), int));
    (void)a;
}
