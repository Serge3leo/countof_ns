// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2026 Сергей Леонтьев (leo@sai.msu.ru)

#if BROKEN_ERROR_ON_GENERIC || NO_ERROR_ON_GENERIC
    #error "BROKEN_ERROR_ON_GENERIC || NO_ERROR_ON_GENERIC"
#endif

#include "tac_defs.h"

TAC_CHECK_FUNC(ss_foo) {
    volatile size_t n = 1917;
    int v[n];
    tac_static_assert(_Generic(&v, int (*)[sizeof(v)/sizeof(*v)] : 1));
}
