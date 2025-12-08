// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

// Check non zero-length VLA extension

#include "have_vla.h"

TAC_CHECK_FUNC(vla_foo) {
    have_vla();
}
