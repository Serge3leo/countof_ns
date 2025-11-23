# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

add_definitions(${TAC_HAVE_ADD_DEFINITIONS}  # TODO: remove for EXAMPLE_FAIL
                ${TAC_ADD_DEFINITIONS}
                ${TAC_DEFINITIONS})
message("TAC_DEFINITIONS=${TAC_DEFINITIONS} "
        "TAC_ADD_DEFINITIONS=${TAC_ADD_DEFINITIONS} "
        "TAC_ADD_C_DEFINITIONS=${TAC_ADD_C_DEFINITIONS} "
        "TAC_HAVE_ADD_DEFINITIONS=${TAC_HAVE_ADD_DEFINITIONS} ")

foreach(sfx IN ITEMS "" "_cxx")
    if(sfx STREQUAL "" AND
       "${TAC_DEFINITIONS}" MATCHES "HAVE_VLA")
        set(novla "_vla")
    else()
        set(novla "")
    endif()
    foreach(vla IN ITEMS "" ${novla})
        foreach(pfx IN ITEMS short long)
            set(base ${pfx}_example${sfx})
            set(tgt ${base}${vla})
            if(sfx STREQUAL "")
                add_executable(${tgt} ${base}.c)
                target_compile_options(${tgt} PRIVATE ${TAC_ADD_C_DEFINITIONS})
            else()
                add_executable(${tgt} ${base}.cpp)
            endif()
            if(NOT vla STREQUAL "_vla")
                target_compile_options(${tgt} PRIVATE "-DDISABLE_VLA_EXAMPLE")
            endif()
            target_link_libraries(${tgt} PRIVATE CounofNS::CounofNS)
        endforeach()
    endforeach()
endforeach()
