// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)
size_t TU_UNIT(void) {
    #ifndef _COUNTOF_NS_USE_TEMPLATE
        #error "_COUNTOF_NS_USE_TEMPLATE don't default"
        assert(0);
    #elif !defined(_COUNTOF_NS_VLA_UNSUPPORTED)
        #error "Don't defined _COUNTOF_NS_VLA_UNSUPPORTED"
        assert(0);
    #else
        TU_STATIC_ASSERT_AND_RETURN(1, sizeof(char));
    #endif
}
