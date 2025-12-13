// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

#ifdef __has_builtin
    #if __has_builtin(__is_array)
        #error "__is_array() not hidden"
    #endif
#endif
#define ia_is_array  __is_array
#include "have_is_array_cxx.cpp"
