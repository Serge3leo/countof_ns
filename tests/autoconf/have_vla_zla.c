// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

#include "tac_defs.h"

#if __STDC_NO_VLA__
    #error "VLA don't supported"
#endif

#if !HAVE_EMPTY_STRUCTURE
    #error "Empty structures don't supported"
#endif

typedef struct {} T0;

TAC_CHECK_FUNC(vla_foo) {
    tac_static_assert(0 == sizeof(T0), "0 == sizeof(T0)");

    size_t n0 = 0;
    T0 a1[n0];
    assert(n0 == sizeof(a1));
    assert(sizeof(T0) == sizeof(a1[0]));

    size_t m2 = 42;
    T0 a2[m2];
    assert(n0 == sizeof(a2));
    assert(sizeof(T0) == sizeof(a2[0]));
    for (size_t i = 0; i < m2; i++) {
        assert(sizeof(T0) == sizeof(a2[i]));
    }

    (void)a1; (void)a2;
}
