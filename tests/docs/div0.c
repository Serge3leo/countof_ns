// vim:set sw=4 ts=8 et fileencoding=utf8::Кодировка:UTF-8[АБЁЪЯабёъя]
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2026 Сергей Леонтьев (leo@sai.msu.ru)

#include <stdio.h>

int main(void) {
#if !__STDC_NO_VLA__ && HAVE_EMPTY_STRUCTURE
    size_t n = 1917;
    struct{} v[n][n];
    printf("sizeof(v)/sizeof(v[0])=%zu\n", sizeof(v)/sizeof(v[0]));
#endif
}
