// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)
size_t TU_UNIT(void) {
    typedef int a_t[1917];
    a_t *pa;
    TU_STATIC_ASSERT_AND_RETURN(1, (size_t)(
                0 != _countof_ns_ptr_compatible_type(&pa, a_t **)));
}
