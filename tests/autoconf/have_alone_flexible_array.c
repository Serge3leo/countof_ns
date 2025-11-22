// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

#include "tac_defs.h"

TAC_CHECK_FUNC(afa_foo) {
    struct {
        int flex[];
    } sf;
    tac_static_assert(0 == sizeof(sf), "Alone flexible array members");
    (void)sf;
}
