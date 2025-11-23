#!
# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

set -e

platform_arg=
rm_arg=false
verbose=false
cc_cmp_arg=
cxx_cmp_arg=
while getopts rvp:c:C:h'?' flag ; do
    case "$flag" in
    r)  rm_arg=true;;
    v)  verbose=true;;
    p)  platform_arg="$OPTARG";;
    c)  cc_cmp_arg="$OPTARG";;
    C)  cxx_cmp_arg="$OPTARG";;
    *)  echo '' "Usage: $0 [-rv] [-p platform] [-c C-compiler]" \
                        "[-C C++-compiler] [--] [cmake_args]"
        echo '  -r - Удалить содержимое директории сборки;'
        echo '  -v - Расширенная печать;'
        echo '  -p platform - Платформа, по умолчанию `uname -s`;'
        echo '  -c C-compiler - Если не задан, то `cmake` сам его ищет;'
        echo '  -C C++-compiler - Если не задан, то определяется по C компилятору;'
        echo ''
        echo 'Пример: ./mybuild.sh -p Xcode'
        exit 2
    esac
done
shift $(($OPTIND - 1))
if "$verbose" ; then
    printf "rm_arg=%s platform_arg=%s cc_cmp_arg=%s cxx_cmp_arg=%s\n" \
           $rm_arg "$platform_arg" "$cc_cmp_arg" "$cxx_cmp_arg"
    printf "CMake arguments: %s\n" "$*"
fi

cc="$cc_cmp_arg"
cxx="$cxx_cmp_arg"
if [ -z "$cc_cmp_arg" ] ; then
    if [ "$cxx_cmp_arg" ] ; then
        echo "$0: Если задан C++ компилятор, то должен быть задан C компилятор" 1>&2
        exit 2
    fi
elif [ -z "$cxx_cmp_arg" ] ; then
    bcc=`basename "$cc_cmp_arg"`
    if [ "$cc" = "$cc_cmp_arg" ]; then
        pcc=
    else
        pcc=`dirname "$cc_cmp_arg"`/
    fi
    case "$bcc" in
     cc)
        try="CC c++"
        ;;
     icx)
        try=icpx
        ;;
     icc)
        try=icpc
        ;;
     nvc)
        try=nvc++
        ;;
     pgcc)
        try=pgc++
        ;;
     suncc)
        try=sunCC
        ;;
     *clang*)
        try=`echo "$bcc" | sed 's/clang/clang++/'`
        ;;
     *gcc*)
        try=`echo "$bcc" | sed 's/gcc/g++/'`
        ;;
    esac
    for t in $try ; do
        if type "$pcc$t" > /dev/null ; then
            cxx="$pcc$t"
            break
        fi
    done
    if [ -z "$cxx" ] ; then
        echo "$0: Don't detect C++ compiler" 1>&2
        exit 2
    fi
    if "$verbose" ; then
        echo "Detect: cc=$cc; cxx=$cxx"
    fi
fi

bcc=`basename "$cc"`
bcxx=`basename "$cxx"`

default_cmpl() {
    make
    ctest
}
Xcode_args() {
    cmake_args="$cmake_args -G Xcode"
}
Xcode_cmpl() {
    open *.xcodeproj
}
Darwin_args() { true; }
Darwin_cmpl() { default_cmpl; }
FreeBSD_args() { true; }
FreeBSD_cmpl() { default_cmpl; }
Linux_args() { true; }
Linux_cmpl() { default_cmpl; }

b=`basename $0`
if [ -r CMakeLists.txt -a -r "$b" -a -d build ] && cmp -s "$0" "$b" ; then
    platform=${platform_arg:-`uname -s`}
    bdir="${platform}"_"$bcc"_"$bcxx"
    if [ -z "$bcc" ] ; then
        bdir="$platform"
    fi
    src_dir=true
else
    ([ -r ../../CMakeLists.txt -a -r ../../"$b" -a -d ../../build ] &&
     cmp -s "$0" ../../"$b") || {
        echo "$0: Необходимо находиться, либо в сборочном, " \
             "либо в исходном каталоге" 1>&2
        exit 2
    }
    [ -z "$platform_arg" -a -z "$c_cmp_arg" -a -z "$cxx_cmp_arg"] || {
        echo "$0: Ключи [-p/с/C platform/C/C++-compiler] должны быть опущены" 1>&2
        exit 2
    }
    bdir=$(basename `pwd`)
    platform=`expr "$bdir" : "\(.*\)_[^_]*_[^_]*"`
    src_dir=false
fi
if "$verbose" ; then
    printf "src_dir=%s platform=%s bdir=%s\n" \
           "$src_dir" "$platform" "$bdir"
fi
if "$src_dir" ; then
    mkdir -p "build/$bdir"
    cd "build/$bdir"
fi
if "$rm_arg" ; then
    if "$verbose" ; then
        echo -n "pwd="; pwd
        echo "rm -rf ../../build/$bdir/*"
    fi
    rm -rf "../../build/$bdir"/*
else
    if "$verbose" ; then
        echo "cmake $cmake_args $* ../.."
    fi
    CC="$cc" CXX="$cxx" cmake $cmake_args "$@" ../..
    "${platform}_cmpl"
fi
exit 0
