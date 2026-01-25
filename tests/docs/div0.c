// vim:set sw=4 ts=8 et fileencoding=utf8::Кодировка:UTF-8[АБЁЪЯабёъя]
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2026 Сергей Леонтьев (leo@sai.msu.ru)

#include <assert.h>
#include <stdio.h>
#ifdef __has_include
    #if __has_include(<stdcountof.h>)
        #include <stdcountof.h>
        #ifndef HAVE_COUNTOF
            #define HAVE_COUNTOF  (1)
        #endif
    #endif
#endif

int main(void) {
#if !__STDC_NO_VLA__
    size_t n = 1917;
    struct{} v[n][n];
    #if HAVE_COUNTOF
        assert(0 == sizeof(v));
        assert(1917 == countof(v));
        assert(1917 == countof(v[0]));
    #endif
    printf("sizeof(v)/sizeof(v[0])=%zu\n", sizeof(v)/sizeof(v[0]));
#endif
}
