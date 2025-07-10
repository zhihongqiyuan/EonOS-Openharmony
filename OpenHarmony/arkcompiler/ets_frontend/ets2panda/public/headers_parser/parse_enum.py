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
from text_tools import (
    find_first_of_characters,
    find_first_not_restricted_character,
    smart_find_first_of_characters,
    find_scope_borders,
)


def parse_enum_union(data: str) -> list:
    equally_pos = data.find("=")
    if equally_pos == -1:
        raise RuntimeError("Can't find '='.")

    if data.find("~") != -1 or data.find("&") != -1:
        union = [] # NOTE(morlovsky): instead of using [data[equally_pos + 1:].strip(" \n")]
    else:
        union = [x for x in data[equally_pos + 1 :].split(" ") if x.strip(" \n") != "" and x.strip(" \n") != "|"]
    return union


def is_union_value(data: str) -> bool:
    if data.find("=") == -1 or data.find("<<") != -1:
        return False
    if data.find("|") != -1:
        return True

    value = data[data.find("=") + 1 :].strip(" ")
    if value[0].isdigit() or value[0] == "(" and value[1].isdigit():
        return False

    if find_first_of_characters("1234567890-+", value, 0) != len(value):
        return False

    return True


def parse_enum_class_body(data: str) -> dict:
    res: Dict[str, Any] = {}

    value_start = 0
    value_end = data.find(",", value_start)

    if data.find("#define") != -1:
        debug_log(f"Defines in enum not realized yet. Can't parse enum body with define:\n---\n{data}\n---\n")
        return {}

    if value_end == -1:
        value_end = len(data)

    while value_start != -1 and value_start < len(data):
        value = data[value_start:value_end].strip(" \n")

        if value != "" and (is_union_value(value) == False):
            if "flags" not in res:
                res["flags"] = []
            res["flags"].append(get_name_of_enum_value(value))

        elif value != "" and (is_union_value(value) == True):

            union_flag: Dict[str, Any] = {}
            union_flag["name"] = get_name_of_enum_value(value)
            union_flag["flags"] = parse_enum_union(value)

            if (union_flag["flags"] != []) and ("flag_unions" not in res):
                res["flag_unions"] = []
                res["flag_unions"].append(union_flag)
            elif (union_flag["flags"] != []) and ("flag_unions" not in res) == False:
                res["flag_unions"].append(union_flag)

        if value_end == len(data):
            break

        value_start = value_end + 1
        value_end = data.find(",", value_start)

        if value_end == -1:
            value_end = len(data)

    return res


def parse_enum_class(data: str, start: int = 0) -> Tuple[int, Dict]:
    res = {}

    start_of_name = data.find("enum ", start)

    if data.find("enum class", start_of_name) == start_of_name:
        start_of_name = find_first_not_restricted_character(" ", data, start_of_name + len("enum class"))
    else:
        start_of_name = find_first_not_restricted_character(" ", data, start_of_name + len("enum"))

    end_of_name = find_first_of_characters(" ;{\n", data, start_of_name)
    enum_name = data[start_of_name:end_of_name]

    if enum_name == "":
        raise RuntimeError("Error! No name in enum\n")

    res["name"] = enum_name
    start_of_body = smart_find_first_of_characters("{", data, end_of_name)

    if start_of_body == len(data):
        return find_first_of_characters(";", data, end_of_name), res

    start_of_body, end_of_body = find_scope_borders(data, start_of_body)

    if data.find("<<", start_of_body, end_of_body) != -1:
        res["kind"] = "flags"
    else:
        res["kind"] = "simple"

    parsed_flags = parse_enum_class_body(data[start_of_body + 1 : end_of_body])

    if "flags" in parsed_flags:
        res["flags"] = parsed_flags["flags"]

    if "flag_unions" in parsed_flags:
        res["flag_unions"] = parsed_flags["flag_unions"]

    end_of_body = data.find(";", end_of_body)

    return end_of_body, res


def get_name_of_enum_value(data: str) -> str:
    equally_pos = data.find("=")

    if equally_pos == -1:
        return data

    return data[:equally_pos].strip(" ")
