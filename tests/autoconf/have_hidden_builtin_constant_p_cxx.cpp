// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

#ifdef __has_builtin
    #if __has_builtin(__builtin_constant_p)
        #error "__builtin_constant_p() not hidden"
    #endif
#endif
#define bc_builtin_constant_p __builtin_constant_p
#include "have_builtin_constant_p_cxx.cpp"
