// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

// Check VLA with or without clang/gcc/intel(?)/nvidia/... extension for
// zero-length

#include "tac_defs.h"

#if __STDC_NO_VLA__
    #error "VLA don't supported"
#endif

TAC_CHECK_FUNC(vla_foo) {
    for(size_t n = 1; n < 10; n++) {
        int a[n];
        (void)a;
    }
}
