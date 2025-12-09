// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

#include "tac_defs.h"

#ifdef __has_builtin
    #if __has_builtin(__is_same_as)
        #define isa_is_same_as  __is_same_as
    #endif
#endif

TAC_CHECK_FUNC(isa_foo) {
    int a;
    static_assert(isa_is_same_as(decltype(a), int));
    (void)a;
}
