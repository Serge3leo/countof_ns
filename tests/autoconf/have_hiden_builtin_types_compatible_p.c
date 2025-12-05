// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

#include "tac_defs.h"

#if defined __has_builtin
    #if __has_builtin (__builtin_types_compatible_p)
        #error "Have normal __builtin_types_compatible_p()"
    #endif
#endif

TAC_CHECK_FUNC(cntf_foo) {
    tac_static_assert(__builtin_types_compatible_p(int, int), "1");
}
