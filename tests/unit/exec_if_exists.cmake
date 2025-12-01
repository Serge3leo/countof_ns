# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

# TODO: need review

set(CMD_RESULT 1)
if (EXISTS "${CMD}")
    execute_process(COMMAND "${CMD}" RESULT_VARIABLE CMD_RESULT)
    if (NOT CMD_RESULT MATCHES "[0-9]+")
        message("${CMD} EXIT not integer CMD_RESULT=${CMD_RESULT}")
        if (CMD_RESULT MATCHES "[Ff]loating.point.*exception")
            set(CMD_RESULT 136)  # Common Unix: 128 + SIGFPE
        elseif (CMD_RESULT MATCHES "[Ss]egmentation.*fault")
            set(CMD_RESULT 139)  # Common Unix: 128 + SIGSEGV
        else ()
            set(CMD_RESULT 134)  # Common Unix: 128 + SIGABRT
        endif ()
    endif ()
elseif (NOT "${SKIP_CODE}" STREQUAL "")
    set(CMD_RESULT "${SKIP_CODE}")
endif ()
cmake_language(EXIT "${CMD_RESULT}")
