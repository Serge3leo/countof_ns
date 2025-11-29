# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

# TODO: need review

set(CMD_RESULT 1)
if (EXISTS "${CMD}")
    execute_process(COMMAND "${CMD}" RESULT_VARIABLE CMD_RESULT)
elseif (NOT "${SKIP_CODE}" STREQUAL "")
    set(CMD_RESULT "${SKIP_CODE}")
endif ()
cmake_language(EXIT "${CMD_RESULT}")
