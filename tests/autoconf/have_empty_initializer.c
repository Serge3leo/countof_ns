// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

#include "tac_defs.h"

const char *messages[] = {
    #if TAC_EI_NOT_EMPTY
        "Message"
    #endif
};

typedef const char messages_t[sizeof(messages)];

TAC_CHECK_FUNC(ei_foo) {
    int a[] = {};
    tac_static_assert(0 == sizeof(a), "Empty initializer");
    messages_t m1[1] = {};
    tac_static_assert(0 == sizeof(m1) && 0 == sizeof(m1[0]),
                      "Empty initializer 1");
    messages_t m0[] = {};
    tac_static_assert(0 == sizeof(m0) && 0 == sizeof(m0[0]),
                      "Empty initializer 0");
    (void)a, (void)m1, (void)m0;
}
