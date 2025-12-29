// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2024 Сергей Леонтьев (leo@sai.msu.ru)
//
// Implementation _countof_ns_ptr_compatible_type for IBM XL C for AIX
//
// Usage:
//     $ xlc -qlanglvl=extc1x -qhaltonmsg=1506-068
//           -qinclude=include/_countof_ns_ptr_compatible_type_xl.h
//           -D_COUNTOF_NS_WANT_VLA_BUILTIN ...

#ifndef _COUNTOF_NS_PTR_COMPATIBLE_TYPE_XL_H_2453
#define _COUNTOF_NS_PTR_COMPATIBLE_TYPE_XL_H_2453

#define _countof_ns_ptr_compatible_type(p, type)  (0 == 0*sizeof(p - (type)p))

#endif  // _COUNTOF_NS_PTR_COMPATIBLE_TYPE_XL_H_2453
