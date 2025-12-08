// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2024 Сергей Леонтьев (leo@sai.msu.ru)
//
// Implementation _countof_ns_ptr_compatible_type for MSVC
//
// Usage
//     > cl /std:clatest /wd4116 /we4047 ^
//         /FI_countof_ns_ptr_compatible_type_msvc.h ^

#ifndef _COUNTOF_NS_PTR_COMPATIBLE_TYPE_MSVC_H_1210
#define _COUNTOF_NS_PTR_COMPATIBLE_TYPE_MSVC_H_1210

#define _countof_ns_ptr_compatible_type(p, type)  (0 == 0*sizeof(p - (type)p))

#endif  // _COUNTOF_NS_PTR_COMPATIBLE_TYPE_MSVC_H_1210
