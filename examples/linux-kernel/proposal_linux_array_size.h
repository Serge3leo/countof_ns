// vim:set sw=8 noet fileencoding=utf8:
// SPDX-License-Identifier: GPL
// SPDX-FileCopyrightText: 2026 Сергей Леонтьев (leo@sai.msu.ru)

#ifndef PROPOSAL_LINUX_ARRAY_SIZE_H_6798
#define PROPOSAL_LINUX_ARRAY_SIZE_H_6798

#include <linux/compiler.h>

/**
 * ARRAY_SIZE - get the number of elements in array @arr
 * @arr: array to be sized
 */
#if __STDC_VERSION__ >= 202601L || \
    (defined(__GNUC__) && __GNUC__ >= 16) || \
    (defined(__clang_major__) && __clang_major__ >= 21) || \
    (defined(__INTEL_LLVM_COMPILER) && __INTEL_LLVM_COMPILER >= 20250300)
	#define ARRAY_SIZE(arr)  (_Countof(arr))  // C2Y working draft
#elif defined(__GNUC__)
	// __GNUC__ => Have Elvis operator (Clang, GCC, Intel, ...)
	// 0 == sizeof(arr[0]) => sizeof(arr) == 0 or sizeof(void*)
	#define ARRAY_SIZE(arr)  (sizeof(arr) / \
				  (sizeof((arr)[0]) ?: 2*sizeof(void*)) \
				  + __must_be_array(arr))
#else
	#define ARRAY_SIZE(arr)  ((0 == sizeof(arr) ? 0 : \
				   sizeof(arr) / sizeof((arr)[0]) \
				  ) + __must_be_array(arr))
#endif

// TODO
// The conditional operator eliminates warnings about division by zero.
//
// May be need modified `__must_be_array(arr)` for check array length?
#undef __must_be_array
#if __GNUC__
	#define __unsafe_countof(arr)  (sizeof(arr) / \
					(sizeof((arr)[0]) ?: 2*sizeof(void*)))
#else
	#define __unsafe_countof(arr)  (0 == sizeof(arr) ? 0 : \
					sizeof(arr) / sizeof((arr)[0]))
#endif
#define __must_be_array(arr)  BUILD_BUG_ON_ZERO(!__builtin_types_compatible_p( \
				typeof(&(arr)), \
				typeof((arr)[0])(*)[__unsafe_countof(arr)]))

#endif  // PROPOSAL_LINUX_ARRAY_SIZE_H_6798
