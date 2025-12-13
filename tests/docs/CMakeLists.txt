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
enable_testing()
