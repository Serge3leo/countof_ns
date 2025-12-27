#!/usr/bin/env python3
# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

"""
Read table from markdown file
"""

import csv
import re

class markdown_dialect(csv.Dialect):
    delimiter = "|"
    escapechar = "\\"
    quotechar = None
    quoting = csv.QUOTE_NONE
    lineterminator = "\n"
    skipinitialspace = True
csv.register_dialect("markdown", markdown_dialect)

class Md:
    def __init__(self, name: str):
        self.name = name
        with open(self.name, encoding='utf-8') as f:
            self.content = f.readlines()

# Начало сегмента таблицы: <!-- span: "<имя>" keys: "<ключ>", "<ключ>" -->
# Конец сегмента: <!-- endspan -->
#
# Столбцы сегментов объединяются.
# Строки с совпадающими ключами - конкатенируются с существующими.
# Строки с различными ключами - добавляются в таблицу.
#
# При считывании сегмента, если ключ - пустая строка, то берётся ключ от
# предыдущей строки.

class MdTable:
    rx_span = re.compile(
        r'<!--\s+span:\s+"([^"]+)"\s+keys:((\s+"([^"]+)"\s*[,]?)+)\s*-->')
    rx_keys = re.compile(r'"([^"]+)"')
    rx_endspan = re.compile(r'<!--\s+endspan:\s+"([^"]+)"\s*-->')
    def add_keys(self, keys: "iterator"):
        for k in keys:
            if k not in self.keys:
                self.keys[k] = dict()
    def add(self, r: dict):
        s = None
        for h, hv in r.items():
            if not h in self.keys:
                continue
            if not hv in self.keys[h]:
                s = None
                break
            if s is None:
                s = self.keys[h][hv].copy()
            else:
                s &= self.keys[h][hv]
        if s is not None:
            if 1 == len(s):
                self.table[s.pop()].update(r)
                self.headers |= r.keys()
                return
            elif 1 < len(s):
                raise ValueError(f"add(): {self.name}: error:"
                                 f" неоднозначно: add({r=})")
        self.table.append(r)
        for h, hv in r.items():
            if not h in self.keys:
                continue
            if hv in self.keys[h]:
                self.keys[h][hv].add(len(self.table) - 1)
            else:
                self.keys[h][hv] = {len(self.table) - 1}
        self.headers |= r.keys()
    def iter(self, **kwargs) -> list[dict]:
        s = None
        for k, kv in kwargs.items():
            if not k in self.keys:
                raise ValueError(f"iter(): {self.name}: error:"
                                 f" нет ключа {k}, {kwargs=}")
            if not kv in self.keys[k]:
                raise ValueError(f"iter(): {self.name}: error:"
                                 f" нет значения {kv}, {kwargs=}")
            ks = self.keys[k][kv]  # множество номеров строк
            if s is None:
                s = ks.copy()
            else:
                s &= ks
        return [self.table[n] for n in s]
    def get(self, **kwargs) -> dict:
        rs = self.iter(**kwargs)
        if 1 == len(rs):
            return rs[0]
        if 1 < len(rs):
            raise ValueError(f"get(): {self.name}: error:"
                             f" неоднозначно: get({kwargs=}), {rs=}")
        raise ValueError(f"get(): {self.name}: error:"
                         f" не найден: get({kwargs=}), {rs=}")
    def __init__(self, md: Md, name: str):
        self.name = name  # Имя таблицы
        self.headers = set()  # Множество заголовков
        self.keys = dict()  # Элемент - словарь по значению ключа, содержащий
                            # множество номеров строк
        self.table =  list()  # Список строк, каждая строка - словарь по
                              # колонкам
        bm, b = None, None
        for i in range(len(md.content)):
            if not bm:
                bm = self.rx_span.search(md.content[i])
                if bm and bm.group(1) == name:
                    b = i
                else:
                    bm = None
                continue
            em = self.rx_endspan.search(md.content[i])
            if not em:
                continue
            e = i
            if bm.group(1) != em.group(1):
                raise ValueError(
                        f"__init__(): {md.name}: {b}-{i}: {self.name}: error:"
                        f" несоответствие span/endspan {bm, em=}")
            assert bm.group(1) == name, f"{name, bm, em=}"
            headers = None
            header_len = 0
            for r in csv.reader(md.content[b:e], dialect="markdown"):
                if not r:
                    continue
                if re.match(r"^([-:]+ *)+$", "".join(r)):
                    break
                headers = [c.strip() for c in r]
                header_len += 1
            tbl = [r for r in csv.DictReader(md.content[b:e],
                                             fieldnames=headers,
                                             dialect="markdown")]
            assert(re.match(r"^([-:]+ *)+$",
                            "".join(tbl[header_len].values())))
            header_len += 1
            header = tbl[:header_len]
            table = tbl[header_len:]
            keys = {m.group(1) for m in self.rx_keys.finditer(bm.group(2))}
            self.add_keys(keys)
            prev = None
            for r in table:
                # Skip empty rows
                for c in r:
                    if r[c].strip():
                        break
                else:
                    continue
                # Inherit empty keys from prevision row
                for k in keys:
                    if k in r:
                        r[k] = r[k].strip()
                        if r[k] == "":
                            if prev is None:
                                raise ValueError(
                                            f"__init__(): {md.name}: error:"
                                            f" not prev for {k, r=}")
                            r[k] = prev[k]
                self.add(r)
                prev = r
            bm, b = None, None
        if bm:
            raise ValueError(f"__init__(): {md.name}: {b}: {self.name}: error:"
                             f" не завершён: {bm=}")

if __name__ == '__main__':
    import argparse
    import os
    from pprint import pprint
    import sys

    sys.stdout.reconfigure(encoding='utf-8')
    sys.stderr.reconfigure(encoding='utf-8')
    parser = argparse.ArgumentParser()
    parser.add_argument("--file", help="Tables file",
                        default=os.path.join(os.path.dirname(__file__),
                                             "../../docs/comparison_table.md"))
    parser.add_argument("--name", help="Name of table",
                        default="compiler-id")
    parser.add_argument("--get", default='ID="LCC"')
    parser.add_argument("--iter", default='CXX="pgc++"')

    args = parser.parse_args()
    md_file = Md(args.file)
    md_tbl = MdTable(md_file, args.name)
    for r in md_tbl.table:
        print(r)
    print(f"---- md_tbl.keys:")
    pprint(md_tbl.keys)
    print(f"==== get({args.get})")
    print(eval(f"md_tbl.get({args.get})"))
    print(f"==== iter({args.iter})")
    for r in eval(f"md_tbl.iter({args.iter})"):
        print(r)
