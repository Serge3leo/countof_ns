// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2024 Сергей Леонтьев (leo@sai.msu.ru)

#ifndef PROPOSAL_LINUX_ARRAY_SIZE_H_6798
#define PROPOSAL_LINUX_ARRAY_SIZE_H_6798

/* Exist Linux kernel macros, if success - return 0 */
#define __PR_BUILD_BUG_ON_ZERO_MSG(e, msg, ...)  \
            ((int)sizeof(struct {_Static_assert(!(e), msg);}))

/* Avoid uncertain zero-by-zero divide and warnings */
#define __pr_array_size_unsafe(a)  (!sizeof(*(a)) ? 0 \
            : sizeof(a)/( sizeof(*(a)) ? sizeof(*(a)) : (size_t)-1 ))

/* Check `a` - is array with `_pr_array_size_unsafe(a)` elements */
#define __pr_is_array(a)  (__builtin_types_compatible_p( \
            typeof(a) *, typeof(*(a))(*)[_pr_array_size_unsafe(a)]))
#define __pr_must_be_array(a)  __PR_BUILD_BUG_ON_ZERO_MSG(!__is_array(a), \
                                                          "must be array")

#define PR_ARRAY_SIZE(arr) (__pr_array_size_unsafe(arr) + \
                             (size_t)__pr_must_be_array(arr))


/* May be more sufficient for Linux kernel/QEMM source */
/* Check `a` - is not VLA array with `_pr_array_size_unsafe(a)` elements */
#define __is_fix_array(a)  (__builtin_types_compatible_p( \
            typeof(a), typeof(*(a))[_pr_array_size_unsafe(a)]))
#define __must_be_fix_array(a)  __BUILD_BUG_ON_ZERO_MSG(!__is_fix_array(a), \
                                        "must be fixed array (not VLA)")

/* Avoid uncertain zero-by-zero divide and warnings */
#define __pr_array_size_unsafe(a)  (0 == sizeof(*(a)) ? 0 \
            : sizeof(a)/( sizeof(*(a)) ? sizeof(*(a)) : (size_t)-1 ))

/*
 * Check `a` - is not variably modified type array with
 * `_pr_array_size_unsafe(a)` elements
 */
#define __must_be_fix_array(a)  \
            (_Generic(&(a), typeof(*(a))(*)[_pr_array_size_unsafe(a)]: 0))

#define FIX_ARRAY_SIZE(arr) (__pr_array_size_unsafe(arr) + \
                             (size_t)__must_be_fix_array(arr))

/*
 * Check `a` - is not variably modified type array with
 * `_pr_array_size_unsafe(a)` elements
 */
#define __must_be_fix_array(a, s)  (_Generic(&(a), typeof(*(a))(*)[s]: 0))

#define FIX_ARRAY_SIZE(arr) (__pr_array_size_unsafe(arr) + \
            (size_t)__must_be_fix_array((arr), __pr_array_size_unsafe(arr)))

#endif  // PROPOSAL_LINUX_ARRAY_SIZE_H_6798
