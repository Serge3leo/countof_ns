#!
# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

set -e

cc_cmp_arg=
platform_arg=
rm_arg=false
verbose=false
while getopts rvp:c:h-'?' flag ; do
    case "$flag" in
    r)  rm_arg=true;;
    v)  verbose=true;;
    c)  cc_cmp_arg="$OPTARG";;
    p)  platform_arg="$OPTARG";;
    -)  break;;
    *)  echo '' "Usage: $0 [-rv] [-p platform] [-c C-compiler] [--]" \
                     "[cmake_args]"
        echo '  -r - Remove build directory contents;'
        echo "  -t - ctest arguments string, by default: '$ctest_args';"
        echo '  -v - Verbose launcher output;'
        echo '  -p platform - Platform, by default `uname -s`;'
        echo '  -c C-compiler - If not exist, use cmake defaults;'
        echo ''
        echo 'Examples:'
        echo '  ./shortest-build.sh -v -c clang-mp-21 -- --preset debug'
        echo '  ./shortest-build.sh -p Xcode'
        exit 2
    esac
done
shift $(($OPTIND - 1))
if "$verbose" ; then
    printf "rm_arg=%s platform_arg=%s cc_cmp_arg=%s\n" \
           $rm_arg "$platform_arg" "$cc_cmp_arg"
    printf "CMake arguments: %s\n" "$*"
fi

cc="$cc_cmp_arg"
bcc=`basename "$cc"`
if [ "$bcc" != "$cc" ] ; then
    bcc="need-path-$bcc"
fi

b=`basename $0`
if [ -r CMakeLists.txt -a -r "$b" -a -d build ] && cmp -s "$0" "$b" ; then
    platform=${platform_arg:-`uname -s`}
    bdir="${platform}"_"$bcc"
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
    platform=`expr "$bdir" : "\(.*\)_[^_]*"`
    cc=`expr "$bdir" : ".*_\([^_]*\)"`
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
    if [ "$platform" == "Xcode" ] ; then
        cmake --preset default $cmake_args -G Xcode ../..
        open *.xcodeproj
    else
        if [ -z "$cc" ] ; then
            if "$verbose" ; then
                echo "cmake --preset default $cmake_args $* ../.."
            fi
            cmake --preset default $cmake_args "$@" ../..
        else
            if "$verbose" ; then
                echo "cmake --preset default -DCMAKE_C_COMPILER=\"$cc\"" \
                            "$cmake_args $* ../.."
            fi
            cmake --preset default -DCMAKE_C_COMPILER="$cc" \
                  $cmake_args "$@" ../..
        fi
        cmake --build .
        ctest
    fi
fi
