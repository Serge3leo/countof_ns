// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

#include "tac_defs.h"

#ifdef __has_builtin
    #if __has_builtin(__builtin_constant_p)
        #define bc_builtin_constant_p __builtin_constant_p
    #endif
#endif

TAC_CHECK_FUNC(bc_foo) {
    int a[1917];
    printf("int a[1917] sizeof - %d\n", bc_builtin_constant_p(sizeof(a)));
    (void)a;
    #if HAVE_VLA_CXX
        volatile size_t n = 10;
        int v[n];
        printf("int v[n] sizeof - %d\n", bc_builtin_constant_p(sizeof(v)));
        (void)v;
    #endif
    #if HAVE_ZLA
        int z[0];
        printf("int z[0] sizeof - %d\n", bc_builtin_constant_p(sizeof(z)));
        (void)z;
    #endif
}
