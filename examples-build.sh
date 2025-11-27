#!
# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

set -e

cc_cmp_arg=
ctest_args=
cxx_cmp_arg=
platform_arg=
rm_arg=false
verbose=false
while getopts rvp:c:C:t:h-'?' flag ; do
    case "$flag" in
    r)  rm_arg=true;;
    v)  verbose=true;;
    C)  cxx_cmp_arg="$OPTARG";;
    c)  cc_cmp_arg="$OPTARG";;
    p)  platform_arg="$OPTARG";;
    t)  ctest_args="$OPTARG";;
    -)  break;;
    *)  echo '' "Usage: $0 [-rv] [-p platform] [-c C-compiler] \\"
        echo '' "    [-C C++-compiler] [-t string for ctest] [--]" \
                     "[cmake_args]"
        echo '  -r - Remove build directory contents;'
        echo '  -v - Verbose launcher output;'
        echo '  -p platform - Platform, by default `uname -s`;'
        echo '  -c C-compiler - If not exist, use `cmake` defaults;'
        echo '  -C C++-compiler - If not exist, detected by C compiler;'
        echo ''
        echo 'Examples:'
        echo '  ./examples-build.sh -v -c clang-mp-21' \
                        '-t '\''-R "_0[0n]"'\'' -- -DENABLE_COMPARISONS'
        echo '  ./examples-build.sh -p Xcode'
        exit 2
    esac
done
shift $(($OPTIND - 1))
if "$verbose" ; then
    printf "rm_arg=%s platform_arg=%s cc_cmp_arg=%s cxx_cmp_arg=%s\n" \
           $rm_arg "$platform_arg" "$cc_cmp_arg" "$cxx_cmp_arg"
    printf "CMake arguments: %s\n" "$*"
    if [ "$platform" != "Xcode" ] ; then
        printf "CTest arguments: "
        echo "$ctest_args" | xargs printf "'%s' "
        printf "\n"
    fi
fi

cc="$cc_cmp_arg"
cxx="$cxx_cmp_arg"
if [ -z "$cc_cmp_arg" ] ; then
    if [ "$cxx_cmp_arg" ] ; then
        echo "$0: If C++ compiler defined, must define C compiler" 1>&2
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
if [ "$bcc" != "$cc" ] ; then
    bcc="need-path-$bcc"
fi
bcxx=`basename "$cxx"`
if [ "$bcxx" != "$cxx" ] ; then
    bcc="need-path-$bcxx"
fi

default_cmpl() {
    make
    if [ -z "$ctest_args" ] ; then
        ctest
    else
        echo "$ctest_args" | xargs ctest
    fi
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
        echo "$0: Must be launch either from the build directory, " \
                  "either from the source directory" 1>&2
        exit 2
    }
    [ -z "$platform_arg" -a -z "$c_cmp_arg" -a -z "$cxx_cmp_arg"] || {
        echo "$0: Options [-p/с/C platform/C/C++-compiler]" \
                  " must be ommited" 1>&2
        exit 2
    }
    bdir=$(basename `pwd`)
    platform=`expr "$bdir" : "\(.*\)_[^_]*_[^_]*"`
    cc=`expr "$bdir" : ".*_\([^_]*\)_[^_]*"`
    cxx=`expr "$bdir" : ".*_[^_]*_\([^_]*\)"`
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
    "${platform}_args"
    if "$verbose" ; then
        echo "cmake -DCMAKE_C_COMPILER=\"$cc\"" \
                    "-DCMAKE_CXX_COMPILER=\"$cxx\" $cmake_args $* ../.."
    fi
    cmake -DCMAKE_C_COMPILER="$cc" -DCMAKE_CXX_COMPILER="$cxx" \
          $cmake_args "$@" ../..
    "${platform}_cmpl"
fi
exit 0
