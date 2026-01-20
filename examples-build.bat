rem @echo off
rem vim:set sw=4 ts=8 et fileencoding=utf8:
rem SPDX-License-Identifier: BSD-2-Clause
rem SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

if "%1"=="nmake" (
    rem TODO shift
    set platform=vs%VisualStudioVersion%-nmake
    set generator="NMake Makefiles"
) else if "%1"=="--" (
    rem TODO shift
    set platform=vs%VisualStudioVersion%
    set generator="Visual Studio xx yyyy"
) else if "%1"=="" (
    set platform=vs%VisualStudioVersion%
    set generator="Visual Studio xx yyyy"
) else (
    echo "Usage: example-build.bat [[nmake|--] [cmake args...]]"
    echo     nmake - If exist, then use "NMake Makefiles" cmake generator
    echo .
    echo     Without argument - use default generator for Visual Studio
    exit /b 1
)
if %generator%=="Visual Studio xx yyyy" (
    if "%VisualStudioVersion%"=="16.0" (
        set generator="Visual Studio 16 2019"
    ) else if "%VisualStudioVersion%"=="17.0" (
        set generator="Visual Studio 17 2022"
    ) else (
        set generator="Visual Studio 18 2026"
    )
)
if "%PRESET%"=="" (
    set prst=default
) else (
    set prst=%PRESET%
)
md build\%platform%
cd build\%platform%
set build_type=Release
if %generator%=="NMake Makefiles" (
    cmake --preset %prst% -G %generator% ..\.. %2 %3 %4 %5 %6 %7 %8 %9
    cmake --build .
    ctest --output-on-failure %CTEST_ARGS%
) else (
    cmake -B . -DCMAKE_CXX_COMPILER=cl -DCMAKE_C_COMPILER=cl ^
          -G %generator% ^
          -DCMAKE_BUILD_TYPE=%build_type% -S ..\.. --preset %prst% %2 %3 %4 %5 %6 %7 %8 %9
    cmake --build . --config %build_type%
    ctest --output-on-failure --build-config %build_type% %CTEST_ARGS%
)
