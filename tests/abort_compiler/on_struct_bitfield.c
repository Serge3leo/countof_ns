// vim:set sw=4 ts=8 et fileencoding=utf8::Кодировка:UTF-8[АБЁЪЯабёъя]
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)
// История:
// 2025-11-18 16:13:34 - Создан.

// C99 6.7.2.1 Structure and union specifiers
// Constraints
// (3) The expression that specifies the width of a bit-field shall be an
// integer constant expression that has nonnegative value that shall not exceed
// the number of bits in an object of the type that is specified if the colon
// and expression are omitted. If the value is zero, the declaration shall have
// no declarator.

// C11/C23
// (4) The expression that specifies the width of a bit-field shall be an
// integer constant expression with a nonnegative value that does not exceed
// the width of an object of the type that would be specified were the colon
// and expression omitted[143]). If the value is zero, the declaration shall
// have no declarator.
//
// 143) While the number of bits in a bool object is at least CHAR_BIT, the
// width of a bool is just 1 bit.

#define bug_on_negative(s)  sizeof(struct{unsigned short foo:(s);})

void foo(void) {
    const int good = bug_on_negative(1);
    #if __STDC_VERSION__ >= 201112L
        _Static_assert(sizeof(unsigned short) == bug_on_negative(1),
                       "Check return value");
    #endif
    const int abort_compiler = bug_on_negative(-1);

    (void)good, (void)abort_compiler;
}
