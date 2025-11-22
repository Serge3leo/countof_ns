// vim:set sw=4 ts=8 et fileencoding=utf8::Кодировка:UTF-8[АБЁЪЯабёъя]
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

// Compilers with C11 semantics:
//  - clang
//  - GCC
//  - Pelles C 13.0
//  - nvcc
//
// Compilers with C99 semantics:
//  - Sun C, Oracle Studio
//  - pgcc11
//
// Only warnings, no errors at any cases pointers incompatibles:
//  - MSVC (Visual Studio)
//  - pgcc18
//  - pgcc (aka nvc) 25.9-0
//
// Have options to enable C11 semantics:
//  - pgcc18 -A
//  - pgcc --diag_error=nonstandard_ptr_minus_ptr,incompatible_operands

// Formally, the result is not an integer constant expression anyway. There may
// be unwanted spam warnings from `-pedantic`, `-Wgnu-folding-constant` or
// similar if used in `static_assert()`, etc.

// ===============
// C11/C23 6.5.6 Additive operators
// Constraints
// (3, clause 2)
// - both operands are pointers to qualified or unqualified versions of
//   compatible complete object types; or

// C99 6.5.6 Additive operators
// Constraints
// (3, clause 2)
// - both operands are pointers to qualified or unqualified versions of
//   compatible object types; or

// For compassion:
// ===============
// C99/C23 6.5.8 Relational operators
// Constraints
// (2, clause 2)
// - both operands are pointers to qualified or unqualified versions of
//   compatible object types; or

// C99/C23 6.5.9 Equality operators
// Constraints
// (2, clause 2)
// - both operands are pointers to qualified or unqualified versions of
//   compatible types;

#include "tac_defs.h"

#define ps_bug_on_ptr_minus_ptr(p, q)  (!sizeof((p) - (q)))

#define unsafe(a) (sizeof(*(a)) ? sizeof(a)/sizeof(*(a)) : 0)
#define ps_bug_on_not_array(base_t, array_t, a) ps_bug_on_ptr_minus_ptr( \
                                                (array_t **)&(a), \
                                                (base_t(**)[unsafe(a)])&(a))

typedef int base1_t;
typedef base1_t array1_t[42];

typedef int base2_t;
typedef base2_t *pointer2_t;

TAC_CHECK_FUNC(ps_foo) {
    array1_t a1;
    const int good = ps_bug_on_not_array(base1_t, array1_t, a1);
    (void)good;
    tac_static_assert(!ps_bug_on_not_array(base1_t, array1_t, a1),
                      "Check return value");
    #if !TAC_DONT_FAIL
        pointer2_t p2 = a1;
        const int abort_compiler = ps_bug_on_not_array(base2_t,
                                                       pointer2_t, p2);
        (void)abort_compiler;
    #endif
    #ifdef DEBUG_incompatible_operands
        double d;
        char c;
        int i = (&d - &c);
        (void)i;
        // pgcc18 19.10-0 64-bit target on macOS -tp haswell
        // ...warning: subtraction of pointer types "double *" and "char *" is nonstandard
        // Но при флаге `pgcc18 -A`
        //
        // pgcc11 19.10-0 64-bit target on macOS -tp haswell
        // PGC-S-0102-Illegal operand types for - operator (...)
    #endif
}
