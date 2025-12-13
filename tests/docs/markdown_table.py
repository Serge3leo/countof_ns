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
        with open(name, encoding='utf-8') as f:
            self.content = f.readlines()

class MdTable:
    def __init__(self, md: Md, begin: str, end: str):
        b, e = None, None
        for i in range(len(md.content)):
            if md.content[i].startswith(begin):
                b = i + 1
            if md.content[i].startswith(end):
                e = i
                break
        self.headers = None
        self.header_len = 0
        for r in csv.reader(md.content[b:e], dialect="markdown"):
            if re.match(r"^([-:]+ *)+$", "".join(r)):
                break
            self.headers = [c.strip() for c in r]
            self.header_len += 1
        tbl = [r for r in csv.DictReader(md.content[b:e],
                                         fieldnames=self.headers,
                                         dialect="markdown")]
        self.header = tbl[:self.header_len]
        self.table = tbl[self.header_len:]

if __name__ == '__main__':
    import os

    comparison_table_fn = os.path.join(
            os.path.dirname(__file__), "../../docs/comparison_table.md")
    comparison_table = Md(comparison_table_fn)
    compiler_id = MdTable(comparison_table, "### Compiler ID",
                                            "## Discussion")
    for r in compiler_id.table:
        print(r)
