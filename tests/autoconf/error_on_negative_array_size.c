// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

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

// DISCLAIMER: WARNING:
//     The expression MUST be a constant expression!
//
//     J.1 Unspecified behavior
//     (72) The size expression in an array declaration is not a constant
//          expression and evaluates at program execution time to a nonpositive
//          value (6.7.6.2).

#include "tac_defs.h"

#define nas_bug_on_negative(s)  sizeof(char [s])

TAC_CHECK_FUNC(nas_foo) {
    const int good = nas_bug_on_negative(1);
    (void)good;
    tac_static_assert(1 == nas_bug_on_negative(1), "Check return value");
    #if !TAC_DONT_FAIL
        const int abort_compiler = nas_bug_on_negative(-1);
        (void)abort_compiler;
    #endif
}
