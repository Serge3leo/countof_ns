// vim:set sw=4 ts=8 et fileencoding=utf8::Кодировка:UTF-8[АБЁЪЯабёъя]
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)
// История:
// 2025-11-18 15:30:49 - Создан.

#if __STDC_VERSION__ < 201112L
    #warning "Seems before C11, __STDC_VERSION__ < 201112L"
#endif

// C11 6.7.10 Static assertions
// C23 6.7.11 Static assertions
// Constraints
// (2) The constant expression shall compare unequal to 0.

#define sa_bug_on_zero(e)  sizeof(struct{_Static_assert((e), "Check"); short foo;})

void sa_foo(void) {
    const int good = sa_bug_on_zero(1);
    _Static_assert(sizeof(short) == sa_bug_on_zero(1), "Check return value");
    const int abort_compiler = sa_bug_on_zero(0);

    (void)good, (void)abort_compiler;
}
