// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)
size_t TU_UNIT(void) {
    typedef const int cp_t[sizeof(int*)/sizeof(int)];
    const int **cpp;
    // A compilation error is also allowed instead of returning 0.
    TU_STATIC_ASSERT_AND_RETURN(0, (size_t)(
                0 != _countof_ns_must_compatible(&cpp,
                                const int ***, cp_t **)));
}
