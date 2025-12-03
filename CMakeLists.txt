# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

cmake_minimum_required (VERSION 3.25)
project(countof_ns
    VERSION 0.1.0
    DESCRIPTION "C2y countof() platform independent implementation C23/C++11"
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
if (CMAKE_C_STANDARD_REQUIRED LESS 99 OR CMAKE_C_STANDARD LESS 99 OR
    CMAKE_CXX_STANDARD_REQUIRED LESS 11 OR CMAKE_CXX_STANDARD LESS 11)
    message("Warning: countof_ns() recommended use for C11/C++11 or later"
            "(strictly required C99 or later)")
endif ()

target_include_directories(CountofNS INTERFACE include)

if (NOT DEFINED CL_TAG)
    # We were connected via FetchContent
    # TODO:
    # https://stackoverflow.com/a/79261858/8585880
    # https://habr.com/ru/articles/683204/
    # et all
    message("CL_TAG = ${CL_TAG}")
    message("COMPARISONS = ${COMPARISONS}")
    message("TAC_ENABLE_WARNINGS = ${TAC_ENABLE_WARNINGS}")
    message("TAC_POSITIVE_WERROR = ${TAC_POSITIVE_WERROR}")
    message("CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")
else ()
    set(TAC_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tests/autoconf")
    include("${TAC_SOURCE_DIR}/autoconf.cmake")

    set_property(GLOBAL PROPERTY USE_FOLDERS ON)  # Warning: use ALL_BUILD/RUN_TESTS
    add_subdirectory(examples)
    set(CMAKE_FOLDER "tests")
    add_subdirectory(tests)

    enable_testing()
endif ()
