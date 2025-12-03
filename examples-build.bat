@echo off
rem vim:set sw=4 ts=8 fileencoding=utf8:
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
set jlt=Testing\Temporary\LastTest-junit.xml
set post_check=..\..\tests\check_logs\post_check.py
python "%post_check%" --selftest
if not errorlevel 1 (
    set pc=1
) else (
    set pc=0
    echo ""
    echo "No python, venv or modules, skip %post_check% %jlt%"
    echo "See tests\check_log\venv.bat"
)
if "%1"=="nmake" (
    rem TODO XXX shift?
    cmake --preset default -G %generator% ..\.. %2 %3 %4 %5 %6 %7 %8 %9
    cmake --build .
    ctest --output-junit "%jlt%" -E post_check_
) else (
    rem cmake --preset default ..\.. %*
    rem start .
    cmake -B . -DCMAKE_CXX_COMPILER=cl -DCMAKE_C_COMPILER=cl -DCMAKE_BUILD_TYPE=%build_type% -S ..\.. --preset default
    cmake --build . --config %build_type%
    ctest --output-junit "%jlt%" --build-config %build_type%
)
if "%pc%"=="1" (
    python "%post_check%" "%jlt%"
)
