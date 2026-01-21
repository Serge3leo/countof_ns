// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2026 Сергей Леонтьев (leo@sai.msu.ru)
size_t TU_UNIT(void) {
    #if !_COUNTOF_NS_USE_KR
        #error "_COUNTOF_NS_USE_BUILTIN don't default"
        tu_assert(0);
    #elif _COUNTOF_NS_VLA_UNSUPPORTED
        #error "Defined _COUNTOF_NS_VLA_UNSUPPORTED"
        tu_assert(0);
    #else
        TU_STATIC_ASSERT_AND_RETURN(1, sizeof(char));
    #endif
}
