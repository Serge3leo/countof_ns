# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

add_subdirectory(unit)
add_subdirectory(complex)
if (COUNTOF_NS_DOCTESTS)
    add_subdirectory(docs)
endif ()

enable_testing()
