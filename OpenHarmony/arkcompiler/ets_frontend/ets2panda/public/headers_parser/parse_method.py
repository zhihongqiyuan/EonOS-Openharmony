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


from typing import Tuple, Dict, List, Any
from text_tools import (
    find_first_of_characters,
    rfind_first_of_characters,
    smart_find_first_of_characters,
    find_scope_borders,
    find_first_not_restricted_character,
)
from parse_arguments import parse_arguments, parse_type


def parse_method_or_constructor(data: str, start: int = 0) -> Tuple[int, Dict]:
    """
    Returns end-pos, dict-parsed method or constructor
    Note: 'function' in names of variables is alias for 'method or constructor'
    """
    res: Dict[str, Any] = {}
    end_of_args = parse_declaration_without_postfix(data, start, res)

    # Defines is it declaration or definition:
    next_semicolon = find_first_of_characters(";", data, start)  # <---  for declaration
    start_of_body = smart_find_first_of_characters("{", data, start)  # <---  for definition

    if next_semicolon <= start_of_body:  # <---  declaration case
        end_of_function_declaration = next_semicolon
        end_of_function = next_semicolon

    elif start_of_body != len(data):  # <---  definition case
        start_of_body, end_of_body = find_scope_borders(data, start_of_body)
        end_of_function_declaration = start_of_body
        end_of_function = end_of_body

    else:
        raise RuntimeError("Error! End of function declaration not found\n")

    # Defines is it constructor or method
    colon_pos = find_first_of_characters(":", data, end_of_args, end_of_function_declaration)

    # Function postfix
    if colon_pos == len(data):
        # Postfix if method
        function_declaration_postfix = data[end_of_args + 1 : end_of_function_declaration].strip(" \n")
        res["raw_declaration"] = data[start:end_of_function_declaration].strip(" \n")
    else:
        # Postfix if constructor
        function_declaration_postfix = data[end_of_args + 1 : colon_pos].strip(" \n")
        res["raw_declaration"] = data[start:colon_pos].strip(" \n")

        start_of_body, initializers = parse_initializers(data, colon_pos + 1)
        start_of_body, end_of_body = find_scope_borders(data, start_of_body, "{")
        end_of_function = end_of_body
        # CC-OFFNXT(G.TYP.07) dict key exist
        updated_args, other_initializers = extract_init_args(res["args"], initializers)

        if updated_args != []:
            res["args"] = updated_args
        if other_initializers != []:
            res["other_initializers"] = other_initializers

    if len(function_declaration_postfix):
        res["postfix"] = function_declaration_postfix

    return end_of_function + 1, res


def parse_declaration_without_postfix(data: str, start: int, res: Dict[str, Any]) -> int:
    # Arguments
    start_of_args = find_first_of_characters("(", data, start)

    if start_of_args >= len("operator") and data[start_of_args - len("operator") : start_of_args] == "operator":
        start_of_args = find_first_of_characters("(", data, start_of_args + 1)

    if (
        start_of_args > find_first_of_characters(";", data, start)
        and data[start : data.find("\n", start)].find("operator==") == -1
    ):
        raise RuntimeError("Not method or constructor!")

    end_of_args, res["args"] = parse_arguments(data, start_of_args)

    # Name
    start_of_function_name = rfind_first_of_characters(" *&\n", data, start_of_args - 1) + 1
    if start_of_function_name > len(data):
        start_of_function_name = 0
    res["name"] = data[start_of_function_name:start_of_args]

    if res["name"].isupper():
        raise RuntimeError("New macros found: '" + res["name"] + "'. Please add it to list.")

    # Prefix
    res["return_type"] = parse_type(data[start:start_of_function_name].strip(" \n"))
    if not res["return_type"]:
        del res["return_type"]

    return end_of_args


def parse_initializer(init: str) -> dict:

    """
    Note ' left (left init) ' ---> 'class field': 'left', 'init value': 'left init'
    """
    init = init.strip(" \n")
    res = {}

    parenthese_open = find_first_of_characters("{(", init, 0)

    if parenthese_open != -1:
        parenthese_open, parenthese_close = find_scope_borders(init, parenthese_open, "")
    else:
        raise RuntimeError("Error! Can't find '(' or '{' in parse_initializer: '" + init + "'")

    res["class_field"] = init[:parenthese_open].strip(" \n")
    res["init_value"] = init[parenthese_open + 1 : parenthese_close]

    return res


def parse_initializers(data: str, start: int = 0) -> Tuple[int, List]:  # pylint: disable=C0116
    res = []
    current_pos = find_first_not_restricted_character(" \n", data, start)
    parethese_open = find_first_of_characters("{(", data, current_pos)

    while data[current_pos] != "{" and parethese_open != -1:

        parethese_open, parenthese_close = find_scope_borders(data, parethese_open, "")
        res.append(parse_initializer(data[current_pos : parenthese_close + 1]))

        current_pos = find_first_not_restricted_character(", \n", data, parenthese_close + 1)
        parethese_open = find_first_of_characters("{(", data, current_pos)

    return current_pos, res


def extract_init_args(args: list, initializers: list) -> Tuple[List, List]:
    """
    If some argument is init value for class field, it adds property for this argument
    and remove it from initializer list:

    Constructor(char *a, ...) : class_field_(a), ... {}

    Args list:
    Before: {'name': 'a', 'type': 'char *'}
    After:  {'name': 'a', 'type': 'char *', 'initializer_for': 'class_field_'}

    Initializers list:
    Before: {'class_field': 'class_field_', 'init_value': 'a'}
    After:  {}
    """

    i = 0
    j = 0

    while i < len(initializers):
        while j < len(args) and i < len(initializers):

            if args[j]["name"] == initializers[i]["init_value"]:
                args[j]["initializer_for"] = initializers[i]["class_field"]
                initializers.pop(i)
                j = 0
                continue

            j += 1

        i += 1
        j = 0

    return args, initializers
