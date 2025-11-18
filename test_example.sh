#!
# vim:set sw=4 ts=8 fileencoding=utf8::Кодировка:UTF-8[АБЁЪЯабёъя]
# Лицензия: BSD
# Автор: Сергей Леонтьев (leo@sai.msu.ru)
# История:
# 2025-11-17 04:33:32 - Создан.
#

set -e

gccwarn="-Wno-unknown-warning-option -Wduplicated-branches -Wduplicated-cond -Wcast-qual -Wconversion -Wsign-conversion -Wlogical-op"
#gccwarn="-Wno-unknown-warning-option"
gxxwarn="-Wsign-promo"
common="-g -Wall -Wextra $gccwarn"
cflags="$common"
cxxflags="$common $gxxwarn -Wno-deprecated"

ccs="clang gcc-mp-12 gcc-mp-15 clang-mp-17 clang-mp-21"
cc_stds_clang="c99 c11 c17 c2x"
cc_stds_gcc_mp_12="c99 c11 c17 c2x"
cc_stds_gcc_mp_15="c99 c23 c2y"
cc_stds_clang_mp_17="c99 c2x"
cc_stds_clang_mp_21="c99 c23 c2y"

cxxs="clang++ g++-mp-12 g++-mp-15 mclang++-mp-17 mclang++-mp-21"
cxx_stds_clang__="c++11 c++20 c++2b"
cxx_stds_g___mp_12="c++11 c++23"
cxx_stds_g___mp_15="c++11 c++23 c++2c"
cxx_stds_mclang___mp_17="c++11 c++23"
cxx_stds_mclang___mp_21="c++11 c++23 c++2c"

for cc in $ccs ; do
    xcc=`echo "$cc" | tr "+-" "__"`
    for std in `eval echo \\$cc_stds_"$xcc"` ; do
	for addf in "" "-DDISABLE_VLA_EXAMPLE"; do
	    echo "$cc $addf $cflags --std=$std example.c && ./a.out"
	    "$cc" $addf $cflags --std="$std" example.c
	    ./a.out
	done
    done
done

for cxx in $cxxs ; do
    xcxx=`echo "$cxx" | tr "+-" "__"`
    for std in `eval echo \\$cxx_stds_"$xcxx"` ; do
	echo "$cxx $cxxflags --std=$std example.c && ./a.out"
	"$cxx" $cxxflags --std="$std" example.c
	./a.out
    done
done

echo "Хорь"

