// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2026 Сергей Леонтьев (leo@sai.msu.ru)

#include <assert.h>
#include <stdio.h>

int main(void) {
    size_t r = 0;
    int  a[1917][10];
    int  (*pa)[1917][10] = &a;
    r += sizeof(*pa++[0]);  // W: Clang/IntelLLVM: unevaluated-expression
    assert(&a == pa);
    #if (!__cplusplus && !__STDC_NO_VLA__) || (__cplusplus && HAVE_VLA_CXX)
        size_t n = 25;
        int v[n];
        __typeof__(v) *pv = &v;
        r += sizeof(*pv++[0]);  // W: Clang/IntelLLVM: unevaluated-expression
        assert(&v == pv);
        size_t m = 7;
        int w[m][n];
        __typeof__(w) *pw = &w;
        r += sizeof(*pw++[0]);
        #if !__cplusplus && !__NVCOMPILER && !__ORANGEC__
            assert(&w != pw);
        #else
            (void)(&w != pw);
        #endif
    #endif
    printf("Ok %zu\n", r);
}
