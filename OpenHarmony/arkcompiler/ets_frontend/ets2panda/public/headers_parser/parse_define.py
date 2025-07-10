#!/usr/bin/env python3
# coding=utf-8
#
# Copyright (c) 2024 Huawei Device Co., Ltd.
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


from typing import Tuple, Dict, Any
from text_tools import find_first_not_restricted_character, find_scope_borders, find_first_of_characters, smart_split_by
from line_iterator import LineIterator
from cpp_keywords import known_macroses

# Weakness: define define !endif! endif


def parse_define_macros(data: str, start: int = 0) -> Tuple[int, Dict]:
    res: Dict[str, Any] = {}

    pos, res["name"] = parse_define_macros_name(data, start)
    end_of_line = find_first_of_characters("\n", data, start)

    if data[pos] == "(":
        open_parenthesis, close_parenthesis = find_scope_borders(data, pos, "(")
        res["arguments"] = data[open_parenthesis + 1 : close_parenthesis]
        pos = close_parenthesis + 1

    backslash_pos = find_first_of_characters("\\", data, pos)
    if backslash_pos + 1 != end_of_line:
        if data[pos:end_of_line].strip(" ") != "":
            res["body"] = data[pos:end_of_line].strip(" ")
        return end_of_line, res

    it = LineIterator(data, end_of_line + 1)
    need_to_parse = it.current_line.strip(" ").startswith("_(")

    if need_to_parse:
        res["values"] = []

        while it.next_line() and it.data[it.end - 1] == "\\":
            res["values"].append(parse_mapping_value(it.current_line))
        res["values"].append(parse_mapping_value(it.current_line))
        res["values"] = list(filter(None, res["values"]))
    else:
        while it.next_line():
            if it.data[it.end - 1] != "\\":
                break

    return it.end, res


def parse_mapping_value(data: str) -> list:
    data = data.strip(" _()\\")
    return smart_split_by(data)


def parse_define_macros_name(data: str, start: int) -> Tuple[int, str]:
    start_of_name = find_first_not_restricted_character(" \n", data, data.find("#define", start) + len("#define"))
    end_of_name = find_first_of_characters(" \n({[", data, start_of_name)

    if end_of_name == len(data) or start_of_name == 0:
        raise RuntimeError("Can't find macros name.")

    return end_of_name, data[start_of_name:end_of_name].strip(" \n")


def is_known_macros(data: str) -> bool:
    for name in known_macroses:
        if data.find(name) != -1:
            return True
    return False
