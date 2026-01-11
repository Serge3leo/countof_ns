// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2026 Сергей Леонтьев (leo@sai.msu.ru)

#include <stdio.h>

int main(void) {
#if !_MSC_VER && !__ORANGEC__ && !__POCC__ && !__IBMC__
     struct {} s0;
    _Static_assert(0 == sizeof(s0), "0 == sizeof(s0)");
    int a0[] = {};
    _Static_assert(0 == sizeof(a0), "0 == sizeof(a0)");
    (void)a0;
    struct {} a1917s0[1917];
    _Static_assert(0 == sizeof(a1917s0), "0 == sizeof(a1917s0)");
    printf("Ok\n");
#endif
}
