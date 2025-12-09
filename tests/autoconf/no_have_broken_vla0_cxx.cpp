// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

#include "tac_defs.h"

#define HAVE_VLA0_2DIM (1)
#if HAVE_VLA0_CXX
    #include "have_vla0.h"
#endif
TAC_CHECK_FUNC(vla0c_foo) {
    #if HAVE_VLA0_CXX
        have_vla0();
    #endif
}
