@echo off
rem vim:set sw=4 ts=8 fileencoding=utf8:
rem SPDX-License-Identifier: BSD-2-Clause
rem SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

if not "%1"=="" (
    if not "%1"=="nmake" (
	echo Usage: example-build.bat [nmake]
	echo     nmake - If exist, then use "NMake Makefiles" cmake generator
	echo .     
	echo     Without argument - use default generator for Visual Studio
	exit /b 1
    )
    set platform=vs%VisualStudioVersion%-nmake
    set generator="NMake Makefiles"
) else (
    set platform=vs%VisualStudioVersion%
)
md build\%platform%
cd build\%platform%
if not "%1"=="" (
    cmake -G %generator% ..\..
    nmake
    ctest
) else (
    cmake ..\..
    start .
)
