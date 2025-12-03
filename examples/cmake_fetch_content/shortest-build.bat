rem @echo off
rem vim:set sw=4 ts=8 et fileencoding=utf8:
rem SPDX-License-Identifier: BSD-2-Clause
rem SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

if not "%1"=="" (
    if not "%1"=="nmake" (
        echo Usage: example-build.bat [nmake] [cmake args...]
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
set build_type=Release
if "%1"=="nmake" (
    rem TODO XXX shift?
    cmake --preset default -G %generator% ..\.. %2 %3 %4 %5 %6 %7 %8 %9
    cmake --build .
    ctest
) else (
    cmake -B . -DCMAKE_CXX_COMPILER=cl -DCMAKE_C_COMPILER=cl ^
          -DCMAKE_BUILD_TYPE=%build_type% -S ..\.. --preset default %*
    cmake --build . --config %build_type%
    ctest --build-config %build_type%
)
