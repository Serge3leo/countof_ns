// vim:set sw=4 ts=8 et fileencoding=utf8::Кодировка:UTF-8[АБЁЪЯабёъя]
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)
// История:
// 2025-11-18 15:13:35 - Создан.

// С99 6.7.5.2 Array declarators
// С11/23 6.7.6.2 Array declarators
// Constraints
// (1) In addition to optional type qualifiers and the keyword static, the [
//     and ] may delimit an expression or *. If they delimit an expression
//     (which specifies the size of an array), the expression shall have an
//     integer type.  If the expression is a constant expression, it shall have
//     a value greater than zero. The element type shall not be an incomplete
//     or function type. The optional type qualifiers and the keyword static
//     shall appear only in a declaration of a function parameter with an array
//     type, and then only in the outermost array type derivation.

#define a_bug_on_negative(s)  sizeof(char [s])

void a_foo(void) {
    const int good = a_bug_on_negative(1);
    #if __STDC_VERSION__ >= 201112L
        _Static_assert(1 == a_bug_on_negative(1), "Check return value");
    #endif
    const int abort_compiler = a_bug_on_negative(-1);

    (void)good, (void)abort_compiler;
}
