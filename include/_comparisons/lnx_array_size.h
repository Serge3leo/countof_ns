// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: GPL-2.0
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)
// История:
// 2025-11-20 00:24:41 - Создан.
//

#ifndef LNX_ARRAY_SIZE_H_6776
#define LNX_ARRAY_SIZE_H_6776

// https://github.com/torvalds/linux/blob/23cb64fb76257309e396ea4cec8396d4a1dbae68/include/linux/compiler_types.h#L536C1-L538C1
// linux/include/linux/compiler_types.h

/* Are two types/vars the same type (ignoring qualifiers)? */
#define __same_type(a, b) __builtin_types_compatible_p(typeof(a), typeof(b))


// https://github.com/torvalds/linux/blob/23cb64fb76257309e396ea4cec8396d4a1dbae68/include/linux/compiler.h#L202
// include/linux/compiler.h

#define __BUILD_BUG_ON_ZERO_MSG(e, msg, ...) ((int)sizeof(struct {_Static_assert(!(e), msg);}))

/* &a[0] degrades to a pointer: a different type from an array */
#define __is_array(a)           (!__same_type((a), &(a)[0]))
#define __must_be_array(a)      __BUILD_BUG_ON_ZERO_MSG(!__is_array(a), \
                                                        "must be array")


// https://github.com/torvalds/linux/blob/23cb64fb76257309e396ea4cec8396d4a1dbae68/include/linux/array_size.h#L11
// include/linux/array_size.h
// #include <linux/compiler.h>

/**
 * LNX_ARRAY_SIZE - get the number of elements in array @arr
 * @arr: array to be sized
 */
#define LNX_ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]) + __must_be_array(arr))

#endif  // LNX_ARRAY_SIZE_H_6776
