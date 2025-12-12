#!/usr/bin/env sh
# vim:set sw=4 ts=8 fileencoding=utf8::Кодировка:UTF-8[АБЁЪЯабёъя]
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)
# История:
# 2025-12-11 06:44:01 - Создан.

# echo "pre-commit.sh: 0=$0 args: $*" > /tmp/pre-comit.log

# Если нет аргументов `*.yml|*.yaml`, то `yamllint` не будет вызван.

for f in "$@" ; do
    case "${f}" in
     *.yml|*.yaml)
	 printf "${f}\0"
	 ;;
    esac
done | xargs -0 yamllint
