# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

cmake_minimum_required (VERSION 3.25)
project(countof_ns
    VERSION 0.1.0
    DESCRIPTION "C2y countof() platform independent implementation C23/C++11"
    HOMEPAGE_URL https://github.com/Serge3leo/countof_ns.git
    LANGUAGES C CXX)
add_library(CountofNS INTERFACE include/countof_ns.h)
add_library(CountofNS::CountofNS ALIAS CountofNS)

if (NOT DEFINED CMAKE_C_STANDARD_REQUIRED)
    set(CMAKE_C_STANDARD_REQUIRED 99)  # With C extension `__typeof__()` et al.
endif ()
if (NOT DEFINED CMAKE_C_STANDARD)
    set(CMAKE_C_STANDARD 23)
endif ()
if (NOT DEFINED CMAKE_CXX_STANDARD_REQUIRED)
    set(CMAKE_CXX_STANDARD_REQUIRED 11)
endif ()
if (NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 23)
endif ()
if (CMAKE_C_STANDARD_REQUIRED MATCHES "[78].*|9[0-8]" OR
    CMAKE_C_STANDARD MATCHES "[78].*|9[0-8]" OR
    CMAKE_CXX_STANDARD_REQUIRED LESS 11 OR CMAKE_CXX_STANDARD LESS 11)
    message("Warning: countof_ns() recommended use for C11/C++11 or later"
            "(strictly required C99 or later)")
endif ()

target_include_directories(CountofNS INTERFACE include)

if (NOT PROJECT_IS_TOP_LEVEL)
    return ()
endif ()

option(COUNTOF_NS_SHORT_EXAMPLE "Build test for short_example" ON)
option(COUNTOF_NS_EXAMPLES "Build test for other detailed examples" OFF)
option(COUNTOF_NS_TESTS "Build tests" OFF)
option(COUNTOF_NS_COMPARISONS "Build comparisons tests" OFF)
set(CACHE COUNTOF_NS_CMP_GLOB
    TYPE STRING
    HELP "GLOB pattern of comparisons tests"
    VALUE "*")  # TODO not set default? Workaround: add "*" in tests/unit.

if (COUNTOF_NS_EXAMPLES OR COUNTOF_NS_TESTS OR COUNTOF_NS_COMPARISONS)
    set(TAC_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tests/autoconf")
    include("${TAC_SOURCE_DIR}/autoconf.cmake")
endif ()
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
if (COUNTOF_NS_SHORT_EXAMPLE OR COUNTOF_NS_EXAMPLES)
    add_subdirectory(examples)
endif ()
if (COUNTOF_NS_TESTS OR COUNTOF_NS_COMPARISONS)
    set(CMAKE_FOLDER "tests")
    add_subdirectory(tests)
endif ()

enable_testing()
