// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

#include "tac_defs.h"

#define HAVE_VLA_CHECKS (1)
#if HAVE_VLA
    #include "have_vla.h"
#endif
TAC_CHECK_FUNC(vla_foo) {
    #if HAVE_VLA
        have_vla();
    #endif
}
