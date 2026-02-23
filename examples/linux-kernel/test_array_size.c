// vim:set sw=8 noet fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0
// SPDX-FileCopyrightText: 2026 Сергей Леонтьев (leo@sai.msu.ru)

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Serguei E. Leontiev (leo@sai.msu.ru)");
MODULE_DESCRIPTION("Test ARRAY_SIZE fail");
MODULE_VERSION("0.5");

// Usage:
//
// Test fail for int work_int(size_t m, size_t n, int (*arr)[m][n]):
// $ make
// $ sudo insmod test_array_size.ko test=10
// $ sudo dmesg | tail -70
// $ sudo reboot
//
// Test fail for size_t work_rec(size_t m, size_t n, struct rec (*arr)[m][n]):
// $ sudo insmod test_array_size.ko test=20
// $ sudo dmesg | tail -70
// $ sudo reboot
//
// Test fail for multidimensional VLA:
// $ make
// $ sudo insmod test_array_size.ko test=0
// $ sudo dmesg | tail -70
// $ sudo reboot
//
// Test fail for array of zero size elenents:
// $ sudo insmod test_array_size.ko test=-10
// $ sudo dmesg | tail -70
// $ sudo reboot
//
// Test proposal:
// $ make clean
// $ make PROPOSAL_LINUX_ARRAY_SIZE=1
// $ sudo insmod test_array_size.ko test=0
// $ sudo -s sh -c 'echo -10 > /sys/module/test_array_size/parameters/test'
// $ sudo -s sh -c 'echo 10 > /sys/module/test_array_size/parameters/test'
// $ sudo -s sh -c 'echo 20 > /sys/module/test_array_size/parameters/test'

#ifdef PROPOSAL_LINUX_ARRAY_SIZE
	#undef ARRAY_SIZE
	#include "proposal_linux_array_size.h"

	#ifdef MIN_FIX
		#define TAS_VARIANT  ("MIN_FIX PROPOSAL_LINUX_ARRAY_SIZE")
	#else
		#define TAS_VARIANT  ("PROPOSAL_LINUX_ARRAY_SIZE")
	#endif
#else
	#define TAS_VARIANT  ("")
#endif

static int work_int(size_t m, size_t n, int (*arr)[m][n])
{
	// Need `-O0` for test fail
	// For `-O` or above need three dimensions
	int ret = 0;
	for (size_t i = 0; i < ARRAY_SIZE(*arr); i++) {
		for (size_t j = 0; j < ARRAY_SIZE((*arr)[0]); j++) {
			ret += (*arr)[i][j];
		}
	}
	return ret;
}

struct rec {
	#ifdef REC_A
		size_t a;
	#endif
	#ifdef REC_B
		size_t b;
	#endif
};

static size_t work_rec(size_t m, size_t n, struct rec (*arr)[m][n])
{
	size_t ret = 0;
	for (size_t j = 0; j < ARRAY_SIZE((*arr)[0]); j++) {
		for (size_t i = 0; i < ARRAY_SIZE(*arr); i++) {
			#ifdef REC_A
				ret += (*arr)[i][j].a;
			#endif
			#ifdef REC_B
				ret += (*arr)[i][j].b;
			#endif
			ret++;
		}
	}
	return ret;
}

static int tas_test_set_int(const char *val, const struct kernel_param *kp)
{
	int *pv = kp->arg;
	int res = param_set_int(val, kp);
	if (0 == res) {
		pr_info("%s: *pv=%d %s\n", __func__, *pv, TAS_VARIANT);
		if (30 <= *pv) {
			pr_info("%s: skip\n", __func__);
		} else if (20 <= *pv) {
			#define RM1917  (1917)
			#define RN10  (10)
			struct rec recs[RM1917][RN10] = {};
			pr_info("work_rec(RM1917, RN10, &recs) = %zu\n",
				work_rec(RM1917, RN10, &recs));
		} else if (10 <= *pv) {
			size_t im = 25;
			size_t in = *pv - 10;
			int (*ints)[im][in] = kzalloc(sizeof(*ints),
						      GFP_KERNEL);
			pr_info("work_int(im, in, ints) = %d\n",
				work_int(im, in, ints));
			kfree(ints);
		} else if (-10 >= *pv) {
			struct {} a[0];
			pr_info("%zu, %zu, ARRAY_SIZE(a)=%zu\n",
				sizeof(a), sizeof(a[0]), ARRAY_SIZE(a));
		} else {
			int v[*pv][*pv][*pv];  // With `-O0` may two dimensions
			pr_info("%zu, %zu, ARRAY_SIZE(v)=%zu\n",
				sizeof(v), sizeof(v[0]), ARRAY_SIZE(v));
		}
	}
	return res;
}

const struct kernel_param_ops tas_test_ops_int = {
	.set = &tas_test_set_int,
	.get = &param_get_int,
};

int test = 25;
module_param_cb(test, &tas_test_ops_int, &test, 0644);
MODULE_PARM_DESC(test, "0 -> VLA of VLA0, -10 -> fix of zero size");

static __init int tas_init(void)
{
	pr_info("%s: test=%d\n", __func__, test);
	return 0;
}

static __exit void tas_exit(void)
{
	pr_info("%s: test=%d\n", __func__, test);
}

module_init(tas_init);
module_exit(tas_exit);
