// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)
size_t TU_UNIT(void) {
    typedef const int ca_t[1917];
    typedef const volatile int cva_t[1917];
    typedef volatile int va_t[1917];
    ca_t *pca;
    cva_t *pcva;
    va_t *pva;
    tu_static_assert(_countof_ns_ptr_compatible_type(&pcva, cva_t **));
    tu_static_assert(_countof_ns_ptr_compatible_type(&pva, va_t **));
    TU_STATIC_ASSERT_AND_RETURN(1, (size_t)(
                0 != _countof_ns_ptr_compatible_type(&pca, ca_t **)));
}
