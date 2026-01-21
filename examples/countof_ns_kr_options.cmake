# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2026 Сергей Леонтьев (leo@sai.msu.ru)

# Options for check correct use K&R idiom

set(countof_ns_kr_c_options_GNU
    "-Werror=sizeof-pointer-div" "-Werror=sizeof-array-argument")

set(countof_ns_kr_c_options_Clang
    "-Werror=sizeof-pointer-div" "-Werror=sizeof-array-argument"
    "-Werror=sizeof-array-decay")

set(countof_ns_kr_c_options_IntelLLVM
    "-Werror=sizeof-pointer-div" "-Werror=sizeof-array-argument"
    "-Werror=sizeof-array-decay")
