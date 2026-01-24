// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2026 Сергей Леонтьев (leo@sai.msu.ru)

#include <stdio.h>
#include <string.h>

#if __cplusplus || _MSC_VER
    size_t foo(size_t n, int va[]) {
#else
    size_t foo(size_t n, int va[static n]) {
#endif
    size_t r = n;
    r += sizeof(va);  // W: Clang/GNU/IntelLLVM: sizeof-array-argument
    return r;
}
size_t boo(size_t n, int *p) {
    size_t r = n;
    r += sizeof(p);
    r += sizeof(p)/sizeof(p[0]);  // W: Clang/IntelLLVM: sizeof-pointer-div
                                  // W: GNU (with -Wall): sizeof-pointer-div
    r += sizeof(p)/(sizeof(p[0]));  // Clang/IntelLLVM: Ok
                                    // W: GNU11 (with -Wall): sizeof-pointer-div
    r += sizeof(p)/(sizeof(p[0]) + 0);  // Ok
    memset(p, 0, sizeof(p));  // W: Clang/IntelLLVM: sizeof-pointer-memaccess
                              // W: GNU (with -Wall): sizeof-pointer-memaccess
    return r;
}
int main(void) {
    size_t r = 0;
    int a[1917] = { 10 };
    r += foo(1918, a);  // W: GNU (11 and above): stringop-overflow
    r += boo(1918, a);
    r += sizeof(a)/sizeof(short);  // W: Clang/IntelLLVM: sizeof-array-div
                                   // W: GNU (with -Wall): sizeof-array-div
    r += sizeof(a)/(sizeof(short));  // Ok
    r += sizeof(a + 25);  // W: Clang/IntelLLVM: sizeof-array-decay
    printf("Ok %zu\n", r);
}
