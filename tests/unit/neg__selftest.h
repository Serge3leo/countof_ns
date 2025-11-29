// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)
/*
 * For selftest CMakeLists.txt error detection:
 *  $ ./examples-build.sh -r
 *  $ CXXFLAGS=-DTU_SELFTEST_FAIL=1 \
 *  $ CFLAGS=-DTU_SELFTEST_FAIL=1 ./examples-build.sh
 */
size_t TU_UNIT(void) {
    #ifndef TU_SELFTEST_FAIL
        tu_static_assert(0);
    #endif
    TU_FAIL("neg__selftest.h", 0, sizeof(char));
}
