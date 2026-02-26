@echo off
rem vim:set sw=4 ts=8 et fileencoding=utf8:
rem SPDX-License-Identifier: BSD-2-Clause
rem SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

set cc=cl
set cxx=cl
set platform=vs%VisualStudioVersion%
set generator=Visual Studio xx yyyy
if "%1"=="%cc%" (
    rem TODO shift
) else if "%1"=="--" (
    rem TODO shift
) else if "%1"=="" (
    rem TODO shift
) else if "%1"=="nmake" (
    set platform=vs%VisualStudioVersion%-nmake
    set generator=NMake Makefiles
) else if "%1"=="occ" (
    set cc=occ
    set cxx=occ
    set platform=occ
    set generator=MSYS Makefiles
) else (
    echo "Usage: example-build.bat [[--|cl|nmake|occ] [cmake args...]]"
    echo     nmake - If exist, then use "NMake Makefiles" cmake generator
    echo     occ - Use OrangeC with "MSYS Makefiles" cmake generator
    echo .
    echo     Without argument - use default generator for Visual Studio
    exit /b 1
)
if "%generator%"=="Visual Studio xx yyyy" (
    if "%VisualStudioVersion%"=="16.0" (
        set generator=Visual Studio 16 2019
    ) else if "%VisualStudioVersion%"=="17.0" (
        set generator=Visual Studio 17 2022
    ) else (
        set generator=Visual Studio 18 2026
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
if not "%generator:~0,6%"=="Visual" (
    cmake -B . -DCMAKE_CXX_COMPILER=%cxx% -DCMAKE_C_COMPILER=%cc% ^
          -G "%generator%" ^
          -S ..\.. --preset %prst% %2 %3 %4 %5 %6 %7 %8 %9
    if errorlevel 1 exit /b
    cmake --build .
    if errorlevel 1 exit /b
    ctest --output-on-failure %CTEST_ARGS%
    if errorlevel 1 exit /b
) else (
    cmake -B . -DCMAKE_CXX_COMPILER=%cxx% -DCMAKE_C_COMPILER=%cc% ^
          -G "%generator%" ^
          -DCMAKE_BUILD_TYPE=%build_type% -S ..\.. --preset %prst% %2 %3 %4 %5 %6 %7 %8 %9
    if errorlevel 1 exit /b
    cmake --build . --config %build_type%
    if errorlevel 1 exit /b
    ctest --output-on-failure --build-config %build_type% %CTEST_ARGS%
    if errorlevel 1 exit /b
)
