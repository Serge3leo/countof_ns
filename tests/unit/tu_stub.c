// vim:set sw=4 ts=8 et fileencoding=utf8::Кодировка:UTF-8[АБЁЪЯабёъя]
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

#include "tu_stub.h"

int main() {
    size_t negative = TU_UNIT();
    TU_FAIL("main", 0, negative);   
    return TU_GT_DESIRED_FAIL;
}
