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


from typing import Tuple, Dict
from text_tools import (
    smart_find_first_of_characters,
    find_scope_borders,
    find_first_of_characters,
    find_first_not_restricted_character,
)


def parse_friend_class(data: str, start: int) -> Tuple[int, str]:
    name_start = data.find("friend class ", start) + len("friend class ")
    name_end = data.find(";", name_start)
    friend_name = data[name_start:name_end].strip(" \n")
    return name_end, friend_name


def parse_class(data: str, start: int = 0, namespace: str = "", parent_class_name: str = "") -> Tuple[int, Dict]:
    # Extract class body
    start_of_body = smart_find_first_of_characters("{", data, start)
    start_of_body, end_of_body = find_scope_borders(data, start_of_body)
    class_body = data[start_of_body + 1 : end_of_body]

    # Inheritance
    colon_pos = find_first_of_characters(":", data, start, start_of_body)

    # Exctract class name
    start_of_name = data.find("class ", start) + len("class ")
    end_of_name = find_first_of_characters(" :{", data, start_of_name)
    class_name = data[start_of_name:end_of_name].strip(" \n")

    from cpp_parser import CppParser  # pylint: disable=C0415

    if parent_class_name != "":
        parsed_class = CppParser(class_body, namespace, parent_class_name + "::" + class_name).parse()
    else:
        parsed_class = CppParser(class_body, namespace, class_name).parse()

    # Extract inheritance
    if colon_pos != len(data):
        extends_class = data[colon_pos + 1 : start_of_body].strip(" \n")
        parsed_class["extends"] = extends_class

    # Fill
    parsed_class["name"] = class_name

    return end_of_body, parsed_class


def parse_template_prefix(data: str, start: int) -> Tuple[int, str]:
    start = find_first_not_restricted_character(" ", data, start)
    if data[start : start + len("template")] != "template":
        raise RuntimeError("Not a template!")

    start_of_template_args, end_of_template_args = find_scope_borders(data, start, "<")
    res = data[start_of_template_args + 1 : end_of_template_args]

    return end_of_template_args, res
