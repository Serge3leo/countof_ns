# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

find_package(Python3)

add_test(NAME Python3_FOUND
         COMMAND ${Python3_EXECUTABLE} "-c" "exit(0)")

    # egrep -r -i ' [0-9a-f]{32,} '
set(git_tags ${CMAKE_SOURCE_DIR}/README.md
             ${CMAKE_SOURCE_DIR}/README.ru.md
             ${CMAKE_SOURCE_DIR}/examples/cmake_fetch_content/CMakeLists.txt)
add_test(NAME git_tags
         COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/git_tags.py
                 ${git_tags})

add_test(NAME compiler_id
         COMMAND ${Python3_EXECUTABLE}
                 ${CMAKE_CURRENT_SOURCE_DIR}/compiler_id.py
                 --id ${CMAKE_C_COMPILER_ID})

tac_report(rep)
add_test(NAME compiler_versions
         COMMAND ${Python3_EXECUTABLE}
                 ${CMAKE_CURRENT_SOURCE_DIR}/compiler_versions.py
                 --id ${CMAKE_C_COMPILER_ID}
                 --version ${CMAKE_C_COMPILER_VERSION}
                 ${rep})

include(../unit/unit_cases.cmake)

set(tu_params_list "")
include (../unit/countof_ns_expected.cmake)
include (../unit/comparisons/ms_countof_expected.cmake)
include (../unit/comparisons/stdc_countof_expected.cmake)
include (../unit/comparisons/std_size_expected.cmake)
include (../unit/comparisons/JZMG_ARRAY_LEN_expected.cmake)
include (../unit/comparisons/LNX_ARRAY_SIZE_expected.cmake)
include (../unit/comparisons/ALX_COUNTOF_expected.cmake)

foreach (prm IN ITEMS ${tu_params_list})
    string(REGEX REPLACE "^tu_(.*)_params$" "\\1" cmp "${prm}")
    list(GET ${prm} 3 ${cmp}_available)
endforeach ()

add_test(NAME expected_tables
         COMMAND ${Python3_EXECUTABLE}
                 ${CMAKE_CURRENT_SOURCE_DIR}/expected_tables.py
                 -v
                 --id ${CMAKE_C_COMPILER_ID}
                 --version ${CMAKE_C_COMPILER_VERSION}
                 --countof_ns "${${countof_ns_available}}"
                 --ms_countof "${${ms_countof_available}}"
                 --stdc_countof "${${stdc_countof_available}}"
                 --std_size "${${std_size_available}}"
                 --jzmg_array_len "${${jzmg_array_len_available}}"
                 --lnx_array_size "${${lnx_array_size_available}}"
                 --alx_countof "${${alx_countof_available}}"
                ${rep})

add_test(NAME compare.en.ru
         COMMAND ${Python3_EXECUTABLE}
                 ${CMAKE_CURRENT_SOURCE_DIR}/compare.en.ru.py
                 #--verbose
                 )
enable_testing()
