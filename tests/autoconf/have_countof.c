// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

#include "tac_defs.h"

#if __has_include(<stdcountof.h>)
    #include <stdcountof.h>
#endif

TAC_CHECK_FUNC(cntf_foo) {
    int a[42];
    size_t c = countof(a);
    (void)c;
}
