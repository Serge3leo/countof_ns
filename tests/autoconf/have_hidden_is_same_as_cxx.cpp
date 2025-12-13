// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

#ifdef __has_builtin
    #if __has_builtin(__is_same_as)
        #error "__is_same_as() not hidden"
    #endif
#endif
#define isa_is_same_as  __is_same_as
#include "have_is_same_as_cxx.cpp"
