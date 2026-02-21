// vim:set sw=8 noet fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0
// SPDX-FileCopyrightText: 2026 Сергей Леонтьев (leo@sai.msu.ru)

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Serguei E. Leontiev (leo@sai.msu.ru)");
MODULE_DESCRIPTION("Test ARRAY_SIZE fail");
MODULE_VERSION("0.4");

// Usage:
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

static int tas_test_set_int(const char *val, const struct kernel_param *kp)
{
	int *pv = kp->arg;
	int res = param_set_int(val, kp);
	if (0 == res) {
		pr_info("%s: *pv=%d %s\n", __func__, *pv, TAS_VARIANT);
		if (0 < *pv) {
			pr_info("%s: skip\n", __func__);
		} else if (-10 >= *pv) {
			struct {} a[0];
			pr_info("%zu, %zu, ARRAY_SIZE(a)=%zu\n",
				sizeof(a), sizeof(a[0]), ARRAY_SIZE(a));
		} else {
			int v[*pv][*pv][*pv];
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
