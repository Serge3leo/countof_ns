// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2024 Сергей Леонтьев (leo@sai.msu.ru)
//
// Implementation _countof_ns_must_compatible for IBM XL C for AIX
//
// Usage:
//     $ xlc -qlanglvl=extc1x -qhaltonmsg=1506-068
//           -qinclude=include/_countof_ns_must_compatible_xl.h
//           -D_COUNTOF_NS_WANT_VLA_BUILTIN ...

#ifndef _COUNTOF_NS_MUST_COMPATIBLE_XL_H_2453
#define _COUNTOF_NS_MUST_COMPATIBLE_XL_H_2453

#define _countof_ns_must_compatible(p, t1, t2)  (0*sizeof((t1)(p) - (t2)(p)))

#endif  // _COUNTOF_NS_MUST_COMPATIBLE_XL_H_2453
