// vim:set sw=4 ts=8 et fileencoding=utf8:
// License: quote for research
// SPDX-FileCopyrightText: Microsoft Corporation. All rights reserved.
// Change log:
// 2025-11-20 00:50:45 - Quoting.
//

#ifndef MS_COUNTOF_H_9380
#define MS_COUNTOF_H_9380

#include <stddef.h>

#ifdef __cplusplus
    #ifndef _UNALIGNED
        #define _UNALIGNED
    #endif

    extern "C++"
    {
        template <typename _CountofType, size_t _SizeOfArray>
        char (*ms_countof_helper(_UNALIGNED _CountofType (&_Array)[_SizeOfArray]))[_SizeOfArray];

        #define ms_countof(_Array) (sizeof(*ms_countof_helper(_Array)) + 0)
    }
#else
    #define ms_countof(_Array) (sizeof(_Array) / sizeof(_Array[0]))
#endif

#endif  // MS_COUNTOF_H_9380
