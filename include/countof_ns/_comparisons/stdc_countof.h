// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

#ifndef STDC_COUNTOF_H_8470
#define STDC_COUNTOF_H_8470

#ifdef __has_include
    #if __has_include(<stdcountof.h>)
        #include <stdcountof.h>
    #endif
#endif

#define stdc_countof(a)  (countof(a))

#endif  // STDC_COUNTOF_H_8470
