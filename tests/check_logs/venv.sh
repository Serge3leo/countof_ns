#!
# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

# Create, clean or activate optional pythons venv for check cmake logs

VENV_SH=${VENV_SH:-venv.sh}
PYTHON=${PYTHON:-python3}
NODE=${NODE:-`uname -n`}

_venv_b=`basename -- $0`
_venv_mypath="tests/check_logs"
_venv_l="$_venv_mypath/$_venv_b"

[ -r CMakeLists.txt -a -d venv -a -d build ] || {
    echo "$0: Must be launch from the source directory" 1>&2
    _venv_dir_false=true
}

_venv_dir="venv/$NODE"/countof_ns

if [ "$_venv_b" != "${VENV_SH}" ] ; then
    . "$_venv_dir"/bin/activate
else
    set -e

    [ -z "$_venv_dir_false" -a -r "$_venv_l" ] && cmp "$0" "$_venv_l" || {
        echo "$0: Must be launch from the source directory" 1>&2
        exit 1
    }

    case "$1" in
     create)
        mkdir -p "$_venv_dir"
        "$PYTHON" -m venv "$_venv_dir"
        . "$_venv_dir"/bin/activate
        pip install --upgrade pip
        pip install junitparser
        ;;
     clean)
        rm -rf "$_venv_dir"
        ;;
     *)
        echo "Usage: $0 [create | clean]"
        echo "  For create or clean '$_venv_dir'"
        echo ""
        echo "Or: . $0"
        echo "  For activate"
        exit 2
        ;;
    esac
fi
