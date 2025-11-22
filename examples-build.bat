@echo off
rem vim:set sw=4 ts=8 fileencoding=utf8:
rem SPDX-License-Identifier: BSD-2-Clause
rem SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

set platform=vs%VisualStudioVersion%
md build\%platform%
cd build\%platform%
cmake ..\..
start .
