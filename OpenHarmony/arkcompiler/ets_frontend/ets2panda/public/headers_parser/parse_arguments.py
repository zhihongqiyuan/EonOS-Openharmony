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


"""
Provides:
- parse_type
- parse_argument
- parse_arguments
"""

from typing import Tuple, List, Any, Dict
from cpp_keywords import modifiers_list
from text_tools import (
    find_first_of_characters,
    find_first_not_restricted_character,
    find_scope_borders,
    smart_find_first_of_characters,
    rfind_first_not_restricted_character,
    smart_split_by,
    rfind_first_of_characters,
)


def parse_type(data: str) -> dict:
    data = data.strip(" \n")

    if data == "":
        return {}

    res: Dict[str, Any] = {}
    current_pos = extract_type_name(data, res)

    for modifier in modifiers_list:
        if data.find(modifier, current_pos) != -1:
            if "other_modifiers" not in res:
                res["other_modifiers"] = ""

            res["other_modifiers"] = f"{res['other_modifiers']} {modifier}".strip(" ")

    # NOTE(@Zhelyapov Aleksey) Weakness (<>)
    start_of_parenthes = data.find("(")
    if start_of_parenthes != -1:
        start_of_parenthes, end_of_parenthes = find_scope_borders(data, start_of_parenthes, "(")
        res["cast_from"] = data[start_of_parenthes + 1 : end_of_parenthes]
        current_pos = end_of_parenthes + 1

    # Template in arg
    template_open_pos = data.find("<", current_pos)
    if template_open_pos != -1:
        template_open_pos, template_close_pos = find_scope_borders(data, 0, "<")

        offset, res["template_args"] = parse_arguments(data[template_open_pos + 1 : template_close_pos], 0, "types")
        current_pos = template_open_pos + 1 + offset + 1

    # Ptr
    ptr_start = data.find("*", current_pos)
    if ptr_start != -1:

        ptr_end = find_first_not_restricted_character("*", data, ptr_start)

        res["ptr_depth"] = ptr_end - ptr_start
        current_pos = ptr_end

    # Ref
    ref_start = data.find("&", current_pos)
    if ref_start != -1:

        ref_end = find_first_not_restricted_character("&", data, ref_start)

        res["ref_depth"] = ref_end - ref_start
        current_pos = ref_end

    postfix = data[current_pos:].strip(" \n")
    if postfix != "":
        res["postfix"] = postfix

    return res


def extract_type_name(data: str, res: Dict[str, Any]) -> int:
    prefix_modifiers = ""
    type_name_start = 0
    type_name_end = find_first_of_characters(" <([{&*", data, type_name_start)
    type_name = data[type_name_start:type_name_end].strip(" \n")

    # Extract type name
    while type_name in modifiers_list or type_name == "":
        prefix_modifiers += f" {type_name}"

        type_name_start = find_first_not_restricted_character(" <*", data, type_name_end)
        type_name_end = find_first_of_characters(" <(*", data, type_name_start)

        if type_name_start == len(data):
            type_name = ""
            break

        type_name = data[type_name_start:type_name_end].strip(" \n")

    # 'varbinder::LocalVariable' -> 'LocalVariable'
    if type_name.find("::") != -1:
        namespace = type_name[: type_name.rfind("::")]
        type_name = type_name[type_name.rfind("::") + 2 :]
        res["namespace"] = namespace

    if type_name != "":
        res["name"] = type_name

    prefix_modifiers = prefix_modifiers.strip(" ")
    if prefix_modifiers != "":
        res["prefix"] = prefix_modifiers

    return type_name_end


def parse_argument(arg: str, mode: str = "args") -> Dict:
    """
    modes:
        - args: <type> <var_name>
        - types: <only_type>
    """

    res: Dict[str, Any] = {}
    arg = arg.strip(" \n")

    if arg == "":
        return {}

    # Default value
    equally_pos = smart_find_first_of_characters("=", arg, 0)
    if equally_pos != len(arg):
        default_value_start = find_first_not_restricted_character(" ", arg, equally_pos + 1)
        default_value_end = rfind_first_not_restricted_character("\n; ", arg, len(arg) - 1)
        res["default_value"] = arg[default_value_start : default_value_end + 1]

        arg = arg[:equally_pos].strip(" \n")

    # Default constructor
    if smart_find_first_of_characters("{", arg, 0) != len(arg):
        start_of_constr, end_of_constr = find_scope_borders(arg)
        res["default_constructor"] = arg[start_of_constr + 1 : end_of_constr]
        arg = arg[:start_of_constr].strip(" \n")

    # Name
    if mode == "args":
        name_start = rfind_first_of_characters(" *&>)}", arg, len(arg) - 1)
        if name_start != len(arg) and name_start != len(arg) - 1:
            name = arg[name_start + 1 :].strip(" \n")
            if name != "":
                res["name"] = name
            else:
                raise RuntimeError("Error! Argument without name!")

        res["type"] = parse_type(arg[: name_start + 1])
    else:
        res["type"] = parse_type(arg)

    return res


def parse_arguments(data: str, start: int = 0, mode: str = "args") -> Tuple[int, List]:
    """
    data:
        - (some arguments, ...) ... some code ...
        - arg1, arg2, ..., arg_k
    mode:
        - args: TYPE ARG_NAME, ...
        - types: TYPE, ...
    """
    res = []

    start_of_args = 0
    end_of_args = len(data)

    if data[start] == "(":
        start_of_args, end_of_args = find_scope_borders(data, start, "(")
        start_of_args += 1

    args = data[start_of_args:end_of_args]
    args_list = smart_split_by(args, ",")

    for arg in args_list:
        parsed_arg = parse_argument(arg, mode)

        if parsed_arg:
            res.append(parsed_arg)

    return end_of_args, res
