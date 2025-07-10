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
from log_tools import debug_log
from parse_arguments import parse_argument
from parse_define import is_known_macros
from parse_method import parse_method_or_constructor
from text_tools import (
    smart_split_by,
    find_first_not_restricted_character,
    smart_find_first_of_characters,
    find_first_of_characters,
    find_scope_borders,
)


def parse_struct_body(data: str) -> list:
    res = []
    for x in smart_split_by(data, ";"):
        if x.find("(") != -1:
            if not is_known_macros(x):
                res.append(parse_method_or_constructor(x, 0)[1])
        else:
            res.append(parse_argument(x))
    return [x for x in res if x]


def parse_struct(data: str, start: int = 0) -> Tuple[int, Dict]:
    res: Dict[str, Any] = {}
    check = data.find("struct ", start)

    if check == -1:
        raise RuntimeError("Error! No need to parse struct.")

    start_of_name = find_first_not_restricted_character(" ", data, check + len("struct "))
    end_of_name = find_first_of_characters(" ;{\n", data, start_of_name)
    struct_name = data[start_of_name:end_of_name]

    if struct_name == "":
        raise RuntimeError("Error! No name in struct\n")

    res["name"] = struct_name
    start_of_body = smart_find_first_of_characters("{", data, end_of_name)
    next_semicolon = smart_find_first_of_characters(";", data, end_of_name)

    if start_of_body == len(data) or next_semicolon < start_of_body:
        debug_log(f"Empty body in struct '{struct_name}'")
        return end_of_name, res

    start_of_body, end_of_body = find_scope_borders(data, start_of_body)

    struct_members = parse_struct_body(data[start_of_body + 1 : end_of_body])
    res["members"] = struct_members

    return end_of_body, res
