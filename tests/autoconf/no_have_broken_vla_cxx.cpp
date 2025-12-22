// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

// Check non zero-length VLA extension

#include "tac_defs.h"

#define HAVE_VLA_CHECKS (1)
#if HAVE_VLA_CXX
    #include "have_vla.h"
#endif
TAC_CHECK_FUNC(vlac_foo) {
    #if HAVE_VLA_CXX
        have_vla();
    #else
        printf("Don't have C++ VLA extension\n");
    #endif
}
