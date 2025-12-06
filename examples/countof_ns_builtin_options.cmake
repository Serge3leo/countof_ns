# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

set(countof_ns_builtin_options_Intel
    "-diag-error=1121"
    "-D_countof_ns_ptr_compatible_type(p,t)=(0==0*sizeof((p)-(t)(p)))")
set(countof_ns_builtin_options_MSVC
    "/std:clatest" "/wd4116" "/we4047"
    "/FI_countof_ns_ptr_compatible_type_msvc.h")
set(countof_ns_builtin_options_NVHPC
    "--diag_error=nonstandard_ptr_minus_ptr"
    "-D_countof_ns_ptr_compatible_type(p,t)=(0==0*sizeof((p)-(t)(p)))")
set(countof_ns_builtin_options_SunPro
    "-errwarn=E_BAD_POINTER_SUBTRACTION"
    "-D_countof_ns_ptr_compatible_type(p,t)=(0==0*sizeof((p)-(t)(p)))")
