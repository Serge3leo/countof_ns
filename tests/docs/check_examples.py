#!/usr/bin/env python3
# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

"""
Check examples from markdown file
"""

from collections import defaultdict
import re

class Md:
    def __init__(self, name: str):
        self.name = name
        with open(self.name, encoding='utf-8') as f:
            self.content = f.readlines()

# Начало сегмента примера: <!-- example: "<имя>" -->
# Конец сегмента: <!-- endexample "<имя>" -->

class MdExamples:
    rx_example = re.compile(r'<!--\s+example:\s+"([^"]+)"\s*-->')
    rx_endexample = re.compile(r'<!--\s+endexample:\s+"([^"]+)"\s*-->')
    def __init__(self, md: Md):
        self.examples = defaultdict(list)
        bm, b = None, None
        for i in range(len(md.content)):
            if not bm:
                bm = self.rx_example.search(md.content[i])
                if bm:
                    b = i
                else:
                    bm = None
                continue
            em = self.rx_endexample.search(md.content[i])
            if not em:
                continue
            e = i
            if bm.group(1) != em.group(1):
                raise ValueError(
                        f"__init__(): {md.name}: {b}-{i}: error:"
                        f" несоответствие example/endexample {bm, em=}")
            self.examples[bm.group(1)].append(md.content[b + 1:e])
            bm = None

ce_comment = re.compile(r'//.*')

def check_example(name: str, body: list[str], content: list[str]) -> bool:
    ret = True
    prev_i = 0
    prev_s = "<BEGIN>"
    prev_c = "<BEGIN>"
    c = 0
    for b in body:
        s = b.strip().replace("\N{NBSP}", " ")
        if "```" in s:
            continue
        s = ce_comment.sub("", s)
        if not s:
            continue
        for i in range(c, len(content)):
            if s in content[i]:
                c = i + 1
                prev_i = i
                prev_s = s
                prev_c = content[i]
                break
        else:
            print(f"{name}: Not found '{s}'"
                  f" {c, len(content), prev_i, prev_s, prev_c=}")
            ret = False
    return ret

class ExampleNotFound(Exception):
    pass

def get_content(path: list[str], example: str):
    for p in path:
        try:
            with open(os.path.join(p, example), encoding='utf-8') as f:
                return f.readlines()
        except FileNotFoundError:
            continue
    raise ExampleNotFound(example)

if __name__ == '__main__':
    import argparse
    import os
    from pprint import pprint
    import sys

    sys.stdout.reconfigure(encoding='utf-8')
    sys.stderr.reconfigure(encoding='utf-8')
    parser = argparse.ArgumentParser()
    parser.add_argument("examples", nargs='+', help="Examples list")
    parser.add_argument("--file", help="Doc file",
                        default=os.path.join(os.path.dirname(__file__),
                                "../../docs/Long-awaited_Countof.ru.md"))
    parser.add_argument("--examples-dir", help="Examples directory",
                        default=[os.path.dirname(__file__)],
                        action='append')
    parser.add_argument("--verbose", "-v", help="verbose output",
                        action='count', default=0)
    args = parser.parse_args()
    md_file = Md(args.file)
    ex_path = args.examples_dir + [os.path.dirname(args.file), "."]
    if args.verbose:
        print(f"{md_file.name, len(md_file.content), ex_path=}")
    md_exmpls = MdExamples(md_file)
    if args.verbose:
        print(f"Examples: {md_exmpls.examples.keys()}")
    err = False
    for e in args.examples:
        if not e in md_exmpls.examples:
            print(f"{args.file}: Not found example '{e}'")
            err = True
            continue
        try:
            content = get_content(ex_path, e)
        except ExampleNotFound as err:
            print(f"Error: {e, err=}")
            err = True
            continue
        for ce in md_exmpls.examples[e]:
            if not check_example(e, ce, content):
                err = True
        del md_exmpls.examples[e]
    if md_exmpls.examples:
        print(f"{args.file}: Have more examples: {md_exmpls.examples.keys()=}")
        err = True
    if args.verbose:
        print("Fail" if err else "Ok")
    exit(err)
