@echo off
rem vim:set sw=4 ts=8 fileencoding=utf8:
rem SPDX-License-Identifier: BSD-2-Clause
rem SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

set venv_dir=venv\vs%VisualStudioVersion%-%COMPUTERNAME%

if "%1"=="" (
    call "%venv_dir%\Scripts\activate.bat"
) else if "%1"=="create" (
    python -m venv "%venv_dir%"
    call "%venv_dir%\Scripts\activate.bat"
    python -m pip install --upgrade pip
    python -m pip install junitparser
) else if "%1"=="clean" (
    rd "%venv_dir%"/s
) else (
    echo "Usage: venv.bat [create | clean]"
    echo "  For create or clean '%venv_dir%'"
    echo ""
    echo "Or: venv.bat"
    echo "  For activate"
)
