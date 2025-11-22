// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

// Check clang/gcc/intel(?)/nvidia/... extension for zero-length array

#include "tac_defs.h"

#define ZLA_MIN  (0)
#define ZLA_1D  (2)
#define ZLA_2D  (3)
#define ZLA_3D  (4)

#define zla_have_size(s)  sizeof(char [s])

TAC_CHECK_FUNC(zla_foo) {
    const size_t obviously = zla_have_size(1);
    (void)obviously;
    tac_static_assert(1 == zla_have_size(1), "Check return value");
    const size_t abort_compiler = zla_have_size(0);
    (void)abort_compiler;

    int zla[ZLA_1D];
    const size_t obviously_1d = sizeof(zla);
    (void)obviously_1d;
    tac_static_assert(ZLA_1D*sizeof(int) == sizeof(zla), "zla size");
    tac_static_assert(sizeof(&zla - (int(*)[ZLA_1D])&zla), "Check zla");
    int (*pzla)[ZLA_1D] = &zla;
    (void)pzla;
    tac_static_assert(sizeof(&pzla - (int(**)[ZLA_1D])&pzla),
                      "Check ZLA ptr[ZLA_1D]");
    tac_static_assert(sizeof((int(**)[ZLA_1D])&pzla - &pzla),
                      "Check ZLA ptr[ZLA_1D] 1");
    int zlm[ZLA_MIN];
    const size_t obviously_m = sizeof(zlm);
    (void)obviously_m;
    tac_static_assert(ZLA_MIN*sizeof(int) == sizeof(zlm), "zlm size");
    int (*pzlm)[ZLA_MIN] = &zlm;
    (void)pzlm;
    tac_static_assert(sizeof(&pzlm - (int(**)[ZLA_MIN])&pzlm),
                      "Check ZLA ptr[ZLA_MIN]");
    tac_static_assert(sizeof((int(**)[ZLA_MIN])&pzlm - &pzlm),
                      "Check ZLA ptr[ZLA_MIN] 1");

    size_t sum1 = 0;
    int o1[ZLA_MIN];
    int o2[ZLA_1D];
    int m1[ZLA_MIN][ZLA_MIN];
    int m2[ZLA_1D][ZLA_MIN];
    int m3[ZLA_MIN][ZLA_2D];
    int m4[ZLA_1D][ZLA_2D];
    int t1[ZLA_MIN][ZLA_MIN][ZLA_MIN];
    int t2[ZLA_1D][ZLA_MIN][ZLA_MIN];
    int t3[ZLA_MIN][ZLA_2D][ZLA_MIN];
    int t4[ZLA_1D][ZLA_2D][ZLA_MIN];
    int t5[ZLA_MIN][ZLA_MIN][ZLA_3D];
    int t6[ZLA_1D][ZLA_MIN][ZLA_3D];
    int t7[ZLA_MIN][ZLA_2D][ZLA_3D];
    int t8[ZLA_1D][ZLA_2D][ZLA_3D];
    sum1 += sizeof(o1);
    sum1 += sizeof(o2);
    sum1 += sizeof(o1[0]);
    sum1 += sizeof(o2[0]);
    sum1 += sizeof(m1);
    sum1 += sizeof(m2);
    sum1 += sizeof(m3);
    sum1 += sizeof(m4);
    sum1 += sizeof(m1[0]);
    sum1 += sizeof(m2[0]);
    sum1 += sizeof(m3[0]);
    sum1 += sizeof(m4[0]);
    sum1 += sizeof(m1[0][0]);
    sum1 += sizeof(m2[0][0]);
    sum1 += sizeof(m3[0][0]);
    sum1 += sizeof(m4[0][0]);
    sum1 += sizeof(t1);
    sum1 += sizeof(t2);
    sum1 += sizeof(t3);
    sum1 += sizeof(t4);
    sum1 += sizeof(t5);
    sum1 += sizeof(t6);
    sum1 += sizeof(t7);
    sum1 += sizeof(t8);
    sum1 += sizeof(t1[0]);
    sum1 += sizeof(t2[0]);
    sum1 += sizeof(t3[0]);
    sum1 += sizeof(t4[0]);
    sum1 += sizeof(t5[0]);
    sum1 += sizeof(t6[0]);
    sum1 += sizeof(t7[0]);
    sum1 += sizeof(t8[0]);
    sum1 += sizeof(t1[0][0]);
    sum1 += sizeof(t2[0][0]);
    sum1 += sizeof(t3[0][0]);
    sum1 += sizeof(t4[0][0]);
    sum1 += sizeof(t5[0][0]);
    sum1 += sizeof(t6[0][0]);
    sum1 += sizeof(t7[0][0]);
    sum1 += sizeof(t8[0][0]);
    sum1 += sizeof(t1[0][0][0]);
    sum1 += sizeof(t2[0][0][0]);
    sum1 += sizeof(t3[0][0][0]);
    sum1 += sizeof(t4[0][0][0]);
    sum1 += sizeof(t5[0][0][0]);
    sum1 += sizeof(t6[0][0][0]);
    sum1 += sizeof(t7[0][0][0]);
    sum1 += sizeof(t8[0][0][0]);
    printf("sizeof(int) = %zu ZLA_MIN = %d sum1 = %zu\n",
            sizeof(int), ZLA_MIN, sum1);
    assert((ZLA_MIN ? 157 : 92)*sizeof(int) == sum1);
}
