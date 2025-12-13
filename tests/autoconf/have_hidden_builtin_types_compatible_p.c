// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

#if defined __has_builtin
    #if __has_builtin (__builtin_types_compatible_p)
        #error "__builtin_types_compatible_p() not hidden"
    #endif
#endif
#define btc_builtin_types_compatible_p  __builtin_types_compatible_p
#include "have_builtin_types_compatible_p.c"
