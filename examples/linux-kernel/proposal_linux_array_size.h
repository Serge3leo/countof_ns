// vim:set sw=8 noet fileencoding=utf8:
// SPDX-License-Identifier: GPL
// SPDX-FileCopyrightText: 2026 Сергей Леонтьев (leo@sai.msu.ru)

#ifndef PROPOSAL_LINUX_ARRAY_SIZE_H_6798
#define PROPOSAL_LINUX_ARRAY_SIZE_H_6798

#include <linux/compiler.h>

#ifdef MIN_FIX  // Minimal fix, eliminates warnings about division by zero.

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
	#define ARRAY_SIZE(arr)  (sizeof(arr) /				\
				  (sizeof((arr)[0]) ?: 2*sizeof(void*))	\
				  + __must_be_array(arr))
#else
	#define ARRAY_SIZE(arr)  ((0 == sizeof(arr) ? 0 :		\
				   sizeof(arr) / sizeof((arr)[0])	\
				  ) + __must_be_array(arr))
#endif

#else // May be need modified `__must_be_array(arr)` for check array length?

/**
 * ARRAY_SIZE - get the number of elements in array @arr
 * @arr: array to be sized
 */
#define ARRAY_SIZE(arr)  (__unsafe_countof(arr) +			\
			  __must_be_array_size((arr),			\
					       __unsafe_countof(arr)))

/**
 * __unsafe_countof - by estimating the number of array elements, avoids
 *                    zero-by-zero uncertainty (in this case, it returns 0).
 * @arr: array to be sized
 */
#if __STDC_VERSION__ >= 202601L || \
    (defined(__GNUC__) && __GNUC__ >= 16) || \
    (defined(__clang_major__) && __clang_major__ >= 21) || \
    (defined(__INTEL_LLVM_COMPILER) && __INTEL_LLVM_COMPILER >= 20250300)
	#define __unsafe_countof(arr)  (_Countof(arr))  // C2Y working draft
#elif defined(__GNUC__)
	// __GNUC__ => Have Elvis operator (Clang, GCC, Intel, ...)
	// 0 == sizeof(arr[0]) => sizeof(arr) == 0 or sizeof(void*)
	#define __unsafe_countof(arr)  (sizeof(arr) /			\
				(sizeof((arr)[0]) ?: 2*sizeof(void*)))
#else
	#define __unsafe_countof(arr)  ((0 == sizeof(arr) ? 0 :		\
					sizeof(arr) / sizeof((arr)[0]))
#endif

/**
 * __must_be_array_size - constraint `arr` is fixed array with `size` elements
 *                        or VLA
 * @arr: array to be checked
 * @size: required number elements of array
 */
#define __must_be_array_size(arr, size)  BUILD_BUG_ON_ZERO(		\
				!__builtin_types_compatible_p(		\
					typeof(&(arr)),			\
					typeof((arr)[0])(*)[size]))

#endif

#endif  // PROPOSAL_LINUX_ARRAY_SIZE_H_6798
