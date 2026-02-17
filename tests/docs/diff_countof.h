// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2026 Сергей Леонтьев (leo@sai.msu.ru)

#include "countof_ns/countof_ns.h"

#include <assert.h>
#include <stdio.h>

int main(void) {
    int a07[0][7];
    int a00[0][0];
    int a70[7][0];
    volatile size_t n7 = 7;
    int v70[n7][0];

    assert(0 == countof_ns(a07));
    assert(0 == countof_ns(a00));  // Successful resolution of uncertainty 0/0
    #if !__cplusplus
        #if EXAMPLE_FAIL
                // The C version of the macro may not always resolve the 0/0 uncertainty.
            (void)countof_ns(a70);  // Compilation error
        #endif
    #else
        assert(7 == countof_ns(a70));
    #endif
        // For VLA, the 0/0 uncertainty cannot be detected during compilation.
    assert(0 == countof_ns(v70));  // The result differs from countof(v70)
    printf("Ok\n");
}
