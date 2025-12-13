# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

if (FALSE)

set(countof_ns_builtin_options_Intel
    "-diag-error=1121"
    "-D_countof_ns_ptr_compatible_type(p,t)=(0==0*sizeof((p)-(t)(p)))"
#     "-D_countof_ns_ptr_compatible_type(p,t)=__builtin_types_compatible_p(__typeof__(p),t)"
    )
set(countof_ns_builtin_options_LCC
# TODO "-Werror-????"
#      "-D_countof_ns_ptr_compatible_type(p,t)=(0==0*sizeof((p)-(t)(p)))"
# TODO ошибка #28: выражение должно иметь константное значение
#      "-D_countof_ns_ptr_compatible_type(p,t)=__builtin_types_compatible_p(__typeof__(p),t)"
# TODO Workaround hack
"-D_countof_ns_ptr_compatible_type(ppa,t)=(!__builtin_types_compatible_p(__typeof__(&*(**(ppa))), __typeof__(**(ppa))))"
   )
endif ()

set(countof_ns_builtin_options_MSVC
    "/std:clatest" "/wd4116" "/we4047"
    "/FI_countof_ns_ptr_compatible_type_msvc.h")

set(countof_ns_builtin_options_NVHPC
    "--diag_error=nonstandard_ptr_minus_ptr"
    "-D_countof_ns_ptr_compatible_type(p,t)=(0==0*sizeof((p)-(t)(p)))")

set(countof_ns_builtin_options_SunPro
    "-errwarn=E_BAD_POINTER_SUBTRACTION"
    "-D_countof_ns_ptr_compatible_type(p,t)=(0==0*sizeof((p)-(t)(p)))")
