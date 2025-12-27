// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)
size_t TU_UNIT(void) {
    #ifndef _COUNTOF_NS_USE_BUILTIN
        #error "_COUNTOF_NS_USE_BUILTIN don't default"
        assert(0);
    #else
        TU_STATIC_ASSERT_AND_RETURN(1, sizeof(char));
    #endif
}
