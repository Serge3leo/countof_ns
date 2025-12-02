#!
# vim:set sw=4 ts=8 fileencoding=utf8::Кодировка:UTF-8[АБЁЪЯабёъя]
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)
# История:
# 2025-12-02 16:43:01 - Создан.

sts="(failure|fpe|segmentation_fault|divide_by_zero_warning|warning)"
act="(ignore|must_not|stable|random)"
arr="^[^#\"']*(skip|${act}_${sts})"
sec="\\[[a-z]{4,}\\]"
end="^[^#\"']*\]"
com="# (Internal|countof_ns|Comparisons)"
pat="$arr|$sec|$end|$com"
echo "# pat=$pat"
echo "#"
egrep -i "$pat" "$@"
