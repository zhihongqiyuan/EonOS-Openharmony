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

from typing import Any, Dict, Union
from line_iterator import LineIterator
from runtime_collections import add_to_statistics, add_to_custom_yamls
from parse_namespace import parse_namespace
from parse_enum import parse_enum_class
from parse_struct import parse_struct
from parse_using import parse_using
from parse_define import parse_define_macros, is_known_macros
from parse_class import parse_class, parse_friend_class, parse_template_prefix
from parse_method import parse_method_or_constructor
from parse_arguments import parse_argument
from log_tools import debug_log


def deep_copy(data: Any) -> Any:
    if isinstance(data, (dict, list)):
        return data.copy()
    return data


class CppParser:
    def __init__(self, data: str, namespace: str = "", parent_class_name: str = ""):
        self.it = LineIterator(data)
        self.parsed: Any = None
        self.res: Dict[str, Any] = {}
        self.template: Union[str, None] = None

        self.parent_class_name = parent_class_name
        self.namespace = namespace
        self.current_modifier = ""

    def parse(self) -> Dict[str, Any]:  # pylint: disable=R0912

        while self.it.next_line():

            if self.it.is_access_modifier():
                self.update_access_modifier()

            elif self.it.is_skip_line() or self.current_modifier in ["private", "protected"]:
                add_to_statistics("skip", self.it.current_line)

            elif self.it.is_template():
                self.it.end, self.template = parse_template_prefix(self.it.data, self.it.start)

            elif self.it.is_namespace():
                self.it.end, self.parsed = parse_namespace(self.it.data, self.it.start)
                self.res_update()

            elif self.it.is_enum():
                self.it.end, self.parsed = parse_enum_class(self.it.data, self.it.start)
                self.res_append_namespace()

            elif self.it.is_struct():
                self.it.end, self.parsed = parse_struct(self.it.data, self.it.start)
                self.res_append("structs")

            elif self.it.is_using():
                self.it.end, self.parsed = parse_using(self.it.data, self.it.start)
                self.res_append_in_modifier("usings")

            elif self.it.is_define_macro():
                self.it.end, self.parsed = parse_define_macros(self.it.data, self.it.start)
                self.res_append("macros")

            elif is_known_macros(self.it.current_line):
                self.parsed = self.it.current_line
                self.res_append("known_macroses")

            elif self.it.is_firend_class():
                self.it.end, self.parsed = parse_friend_class(self.it.data, self.it.start)
                self.res_append("friends")

            elif self.it.is_class_forward_decl():
                self.parsed = self.it.current_line.replace("class", "").strip(" ;")
                self.res_append("class_forward_declaration")

            elif self.it.is_class_definition():
                self.it.end, self.parsed = parse_class(
                    self.it.data, self.it.start, self.namespace, self.parent_class_name
                )
                self.res_append_class_definition()

            elif self.it.is_method_or_constructor():
                self.it.end, self.parsed = parse_method_or_constructor(self.it.data, self.it.start)
                self.res_append_method_or_constructor()

            elif self.it.is_field():
                self.parsed = parse_argument(self.it.data[self.it.start : self.it.next_semicolon])
                self.it.end = self.it.next_semicolon
                self.res_append_field()

            else:
                add_to_statistics("unreachable", self.it.current_line)

        return self.res

    def res_append(self, key: str) -> None:
        if not self.parsed:
            return

        self.parsed_update_template()
        if key not in self.res:
            self.res[key] = []
        self.res[key].append(deep_copy(self.parsed))

    def res_append_in_modifier(self, key: str) -> None:
        if not self.parsed:
            return

        self.parsed_update_template()
        if self.current_modifier == "":
            if key == "usings":
                self.res_append("usings")
                return
            raise RuntimeError("Unreachable")

        if key not in self.res[self.current_modifier]: # CC-OFF(G.TYP.07) dict key exist
            self.res[self.current_modifier][key] = [] # CC-OFF(G.TYP.07) dict key exist

        self.res[self.current_modifier][key].append(deep_copy(self.parsed)) # CC-OFF(G.TYP.07) dict key exist

    def res_update(self) -> None:
        if self.parsed:
            self.parsed_update_template()
            self.res.update(self.parsed)

    def res_append_namespace(self) -> None:
        self.parsed["namespace"] = self.namespace

        if self.parent_class_name != "":
            self.parsed["parent_class_name"] = self.parent_class_name

        if "flags" in self.parsed or "flag_unions" in self.parsed:
            self.res_append("enums")
            add_to_custom_yamls("allEnums", "enums", self.parsed)

    def update_access_modifier(self) -> None:
        if self.parent_class_name == "":
            raise RuntimeError("Found modifier not in class")
        self.current_modifier = self.it.current_line.strip(" :")
        if self.current_modifier not in self.res:
            self.res[self.current_modifier] = {}

    def res_append_method_or_constructor(self) -> None:
        # Constructor
        if self.parsed["name"] == self.parent_class_name: # CC-OFF(G.TYP.07) dict key exist
            self.res_append_in_modifier("constructors")

        # Destructor
        elif self.parsed["name"] == "~" + self.parent_class_name: # CC-OFF(G.TYP.07) dict key exist
            self.res_append_in_modifier("destructors")

        # Method
        elif self.current_modifier != "":
            self.res_append_in_modifier("methods")

        # Function
        else:
            self.res_append("functions")

    def res_append_field(self) -> None:
        # Class field
        if self.current_modifier != "":
            self.res_append_in_modifier("fields")

        # Top level variable
        else:
            self.res_append("vars")

    def res_append_class_definition(self) -> None:
        self.res_append("class_definitions")

    def parsed_update_template(self) -> None:
        if self.template and self.parsed:
            if isinstance(self.parsed, dict):
                self.parsed["template"] = self.template
                self.template = None
            else:
                debug_log(f"Skipping template for '{self.parsed}'")
