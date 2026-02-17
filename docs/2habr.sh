#!
# vim:set et sw=4 ts=8 fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2026 Сергей Леонтьев (leo@sai.msu.ru)

file=${1:-`dirname "$0"`/Long-awaited_Countof.ru.md}

clipboard() {
    pbcopy
    # less
}

cvt2habr() {
    sed '1,/<!-- Habr cut -->/d
        /<!-- Habr cut -->/,/<!-- Habr cut -->/d
        /^```c[[:space:]]*$/s//```cpp/
        /^```c++[[:space:]]*$/s//```cpp/
        /^```sh[[:space:]]*$/s//```bash/
        /!.big_array_size_survey_map.png](big_array_size_survey_map.png)/s##![География участников опроса](https://habrastorage.org/webt/4h/4h/70/4h4h70t3vpbl4bmdropqzs6mf1a.png)#
        /^# Оператор `_Countof`/i\
\
<anchor>оператор-_countof</anchor>\

        /^## Этимология названия/i\
\
<anchor>этимология-названия</anchor>\

        /^## Реализация/i\
\
<anchor>реализация</anchor>\

        /^# Терминология/i\
\
<anchor>терминология</anchor>\

        /^# Жизнь до `_Countof`/i\
\
<anchor>жизнь-до-_countof</anchor>\

        /^## Идиома количества элементов массива/i\
\
<anchor>идиома-количества-элементов-массива</anchor>\

        /^## Подводные камни оператора `sizeof`/i\
\
<anchor>подводные-камни-оператора-sizeof</anchor>\

        /^## Альтернативы без `sizeof`/i\
\
<anchor>альтернативы-без-sizeof</anchor>\

        /^# Переход к `countof()`/i\
\
<anchor>переход-к-countof</anchor>\

        /^# Реализация `countof_ns()`/i\
\
<anchor>реализация-countof_ns</anchor>\

        /^## Без поддержки С23 и расширения `__typeof__()`/i\
\
<anchor>без-поддержки-с23-и-расширения-__typeof__</anchor>\

        /^## С23 или расширение `__typeof__()`/i\
\
<anchor>с23-или-расширение-__typeof__</anchor>\

        /^# Реализация `countof_ns()` в C++/i\
\
<anchor>реализация-countof_ns-в-c</anchor>\

        /^## ZLA и шаблоны/i\
\
<anchor>zla-и-шаблоны</anchor>\

        /^## VLA и `__is_same()`/i\
\
<anchor>vla-и-__is_same</anchor>\

        /^## Статическая рефлексия C++26/i\
\
<anchor>статическая-рефлексия-c26</anchor>\

        /^# Заключение/i\
\
<anchor>заключение</anchor>\

        /^# Ссылки/i\
\
<anchor>ссылки</anchor>\

         ' "$file"
}

check_language_tags() {
    d=$(
        grep '```' "$file" |
        egrep -v '^```($|c|c\+\+|cpp|sh|bash)$'
        )
    if [ -n "$d" ] ; then
       printf "%s: invalid language tags: %s\n" "$file" "$d"
       exit 1
    fi
}

check_language_tags
cvt2habr | clipboard
