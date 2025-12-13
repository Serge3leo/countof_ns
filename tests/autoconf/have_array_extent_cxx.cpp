// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

#include "tac_defs.h"

#ifdef __has_builtin
    #if __has_builtin(__array_extent)
        #define ae_array_extent  __array_extent
    #endif
#endif

TAC_CHECK_FUNC(ae_foo) {
    int a[1917][10][25];
    static_assert(1917 == ae_array_extent(decltype(a), 0));
    auto d1 = ae_array_extent(decltype(a), 0);
    auto d2 = ae_array_extent(decltype(a), 1);
    auto d3 = ae_array_extent(decltype(a), 2);
    auto ff = ae_array_extent(int *, 0);
    printf("a[1917][10][25] = %zu %zu %zu, int %zu\n", d1, d2, d3, ff);
    (void)a;
    #if HAVE_VLA_CXX
        size_t n = 1917;
        int v[n];
        auto dv1 = ae_array_extent(decltype(v), 0);
        printf("v[n] = %zu\n", dv1);
        size_t m = 10;
        int vv[n][m];
        auto dvv1 = ae_array_extent(decltype(vv), 0);
        auto dvv2 = ae_array_extent(decltype(vv), 1);
        printf("v[n][m] = %zu %zu\n", dvv1, dvv2);
        (void)v; (void)vv;
    #endif
    #if HAVE_ZERO_LENGTH_ARRAYS
        int z[25][0];
        auto dz1 = ae_array_extent(decltype(z), 0);
        auto dz2 = ae_array_extent(decltype(z), 1);
        printf("z[25][0] = %zu %zu\n", dz1, dz2);
        (void)z;
    #endif
}
