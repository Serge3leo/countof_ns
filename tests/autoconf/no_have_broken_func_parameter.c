// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2026 Сергей Леонтьев (leo@sai.msu.ru)

#include "tac_defs.h"

#define BFP_N  (1917)
#if !_MSC_VER
    size_t bfp_boo(size_t n, int arr[static n]) {
#else
    size_t bfp_boo(size_t n, int arr[BFP_N]) {
#endif
    #if HAVE_TYPEOF
        return sizeof(typeof(arr));
    #elif HAVE___TYPEOF__
        return sizeof(__typeof__(arr));
    #else
        return sizeof(arr);
    #endif
}

TAC_CHECK_FUNC(bfp_foo) {
    int a[BFP_N];
    size_t r = bfp_boo(BFP_N, a);
    if (sizeof(&*a) != r) {
        printf("%s: bfp_boo(BFP_N, a) = %zu, %s\n", __func__, r,
               (sizeof(a) == r ? "OrangeC" : "FAIL"));
        tac_exit_failure();
    }
}
