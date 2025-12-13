// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

#include "tac_defs.h"

#ifdef __has_builtin
    #if __has_builtin(__is_array)
        #define ia_is_array  __is_array
    #endif
#endif
#ifndef ia_is_array
    #error "Don't have __is_array()"
#endif

TAC_CHECK_FUNC(ia_foo) {
    printf("int *ptr = %d\n", ia_is_array(int*));
    int a[1917];
    static_assert(ia_is_array(decltype(a)));
    printf("int a[1917] = %d\n", ia_is_array(decltype(a)));
    (void)a;
    #if HAVE_VLA_CXX
        size_t n = 10;
        int v[n];
        printf("int v[n] = %d\n", ia_is_array(decltype(v)));
        (void)v;
    #endif
    #if HAVE_ZERO_LENGTH_ARRAYS
        int z[0];
        printf("int z[0] = %d\n", ia_is_array(decltype(z)));
        (void)z;
        int zz[25][0];
        printf("int zz[25][0] = %d\n", ia_is_array(decltype(zz)));
        (void)zz;
    #endif
}
