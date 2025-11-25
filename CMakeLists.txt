# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

cmake_minimum_required (VERSION 3.25)
project(countof_ns
    VERSION 0.1.0
    DESCRIPTION "C2y countof() platform independent implementation C23/C++11"
    LANGUAGES C CXX)
add_library(CounofNS INTERFACE include/countof_ns.h)
add_library(CounofNS::CounofNS ALIAS CounofNS)

set(CMAKE_C_STANDARD_REQUIRED 99)  # With C extension `__typeof__()` et al.
set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED 11)
set(CMAKE_CXX_STANDARD 23)

target_include_directories(CounofNS INTERFACE include)

set(TAC_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tests/autoconf")
include("${TAC_SOURCE_DIR}/autoconf.cmake")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)  # Warning: use ALL_BUILD/RUN_TESTS
add_subdirectory(examples)
set(CMAKE_FOLDER "tests")
add_subdirectory(tests)

enable_testing()
