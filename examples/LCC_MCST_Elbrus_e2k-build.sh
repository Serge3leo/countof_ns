#!
# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

# Example build and test for LCC, MCST Elbrus C/C++/Fortran Compiler
#
# For setup local environment see:
#   https://github.com/mrognor/lcc-env-action/blob/main/action.yml
#   https://github.com/mrognor/lcc-env-action.git
#   https://habr.com/ru/articles/898040/
#   https://gitflic.ru/project/mrognor/lcc-env.git

LANG=C ./examples-build.sh -c lcc -C l++ -- \
    -DCMAKE_CROSSCOMPILING=ON -DCMAKE_SYSTEM_NAME=Generic-ELF \
    -DCMAKE_CROSSCOMPILING_EMULATOR=e2k "$@"
