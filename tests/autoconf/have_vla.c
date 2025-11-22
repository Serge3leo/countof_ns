// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

// Check VLA with or without clang/gcc/intel(?)/nvidia/... extension for
// zero-length

#include "tac_defs.h"

#ifdef HAVE_ZLA
    #define VLA_MIN  (0)
#else
    #define VLA_MIN  (1)
#endif
#define VLA_1D  (2)
#define VLA_2D  (3)
#define VLA_3D  (4)
volatile size_t vla_min = VLA_MIN;
volatile size_t vla_1d = VLA_1D;
volatile size_t vla_2d = VLA_2D;
volatile size_t vla_3d = VLA_3D;

TAC_CHECK_FUNC(vla_foo) {
    int fsa[2];
    const size_t obviously = sizeof(fsa);
    assert(2*sizeof(int) == obviously);
    tac_static_assert(sizeof(&fsa - (int(*)[2])&fsa), "Check FSA");
    #if !TAC_DONT_FAIL
        int vla[vla_1d];
        const size_t unobvious = sizeof(vla);
        assert(VLA_1D*sizeof(int) == unobvious);
        assert(0 == (&vla - (int(*)[3])&vla));
        assert(0 == (&vla - (int(*)[4])&vla));
        int (*pvla)[vla_1d] = &vla;
        tac_static_assert(sizeof(&pvla - (int(**)[VLA_MIN])&pvla),
                          "Check VLA ptr[VLA_MIN]");
        tac_static_assert(sizeof((int(**)[VLA_MIN])&pvla - &pvla),
                          "Check VLA ptr[VLA_MIN] 1");
        tac_static_assert(sizeof(&pvla - (int(**)[9])&pvla),
                          "Check VLA ptr[9]");
        int vlm[vla_min];
        const size_t unobvious_m = sizeof(vlm);
        assert(VLA_MIN*sizeof(int) == unobvious_m);
        int (*pvlm)[vla_min] = &vlm;
        tac_static_assert(sizeof(&pvlm - (int(**)[VLA_MIN])&pvlm),
                          "Check VLA ptr[VLA_MIN]");
        tac_static_assert(sizeof((int(**)[VLA_MIN])&pvlm - &pvlm),
                          "Check VLA ptr[VLA_MIN] 1");
        tac_static_assert(sizeof(&pvlm - (int(**)[VLA_1D])&pvlm),
                          "Check VLA ptr[VLA_1D]");
        tac_static_assert(sizeof(&pvlm - (int(**)[8])&pvlm),
                          "Check VLA ptr[8]");

        size_t sum1 = 0;
        int o1[vla_min];
        int o2[vla_1d];
        int m1[vla_min][vla_min];
        int m2[vla_1d][vla_min];
        int m3[vla_min][vla_2d];
        int m4[vla_1d][vla_2d];
        int t1[vla_min][vla_min][vla_min];
        int t2[vla_1d][vla_min][vla_min];
        int t3[vla_min][vla_2d][vla_min];
        int t4[vla_1d][vla_2d][vla_min];
        int t5[vla_min][vla_min][vla_3d];
        int t6[vla_1d][vla_min][vla_3d];
        int t7[vla_min][vla_2d][vla_3d];
        int t8[vla_1d][vla_2d][vla_3d];
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
        printf("sizeof(int) = %zu vla_min = %zu sum1 = %zu\n",
                sizeof(int), vla_min, sum1);
        size_t sum2 = 0;
        for(size_t n = vla_min; n < 4; n++) {
            int vlo[n];
            sum2 += sizeof(vlo);
            for(size_t m = vla_min; m < 4; m++) {
                int vlm[n][m];
                sum2 += sizeof(vlm);
                sum2 += sizeof(vlm[0]);
                sum2 += sizeof(*vlm);
                sum2 += sizeof(vlm[0][0]);
                sum2 += sizeof(*(vlm[0]));
                for(size_t l = vla_min; l < 4; l++) {
                    int vlt[n][m][l];
                    sum2 += sizeof(vlt);
                    sum2 += sizeof(vlt[0]);
                    sum2 += sizeof(*vlt);
                    sum2 += sizeof(vlt[0][0]);
                    sum2 += sizeof(*(vlt[0]));
                    sum2 += sizeof(vlt[0][0][0]);
                    sum2 += sizeof(*(vlt[0][0]));
                    sum2 += sizeof(*(vlt[0][0]));
                }
            }
        }
        printf("sizeof(int) = %zu vla_min = %zu sum2 = %zu\n",
                sizeof(int), vla_min, sum2);
        assert((vla_min ? 157 : 92)*sizeof(int) == sum1);
        assert((vla_min ? 717 : 1010)*sizeof(int) == sum2);
    #endif
}
