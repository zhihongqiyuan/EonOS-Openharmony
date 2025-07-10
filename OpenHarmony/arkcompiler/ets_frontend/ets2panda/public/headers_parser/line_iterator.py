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


"""Provides line iterator class."""

from text_tools import find_first_of_characters


class LineIterator:  # pylint: disable=C0115
    def __init__(self, data_raw: str, start: int = 0):
        self.data = data_raw

        self.start = start
        self.end = find_first_of_characters("\n", self.data, start)

        self.current_line = self.data[self.start : self.end].strip(" \n")

        self.next_parenthese = find_first_of_characters("(", self.data, start)
        self.next_semicolon = find_first_of_characters(";", self.data, start)
        self.next_equal = find_first_of_characters("=", self.data, start)

        self.processed_first_line = False

    def next_line(self) -> bool:
        if not self.processed_first_line:
            self.processed_first_line = True
            if self.current_line != "":
                return True

        self.end = find_first_of_characters("\n", self.data, self.end)

        if self.end == len(self.data):
            return False

        self.start = self.end + 1
        self.end = find_first_of_characters("\n", self.data, self.start)
        self.current_line = self.data[self.start : self.end].strip(" \n")

        self.next_parenthese = find_first_of_characters("(", self.data, self.start)
        self.next_semicolon = find_first_of_characters(";", self.data, self.start)
        self.next_equal = find_first_of_characters("=", self.data, self.start)

        if self.current_line == "":
            return self.next_line()

        return True

    def is_skip_line(self) -> bool:
        return (
            self.current_line.find("#ifndef") != -1
            or self.current_line.find("#undef") != -1
            or self.current_line.find("#endif") != -1
        )

    def is_template(self) -> bool:
        return self.current_line.find("template") != -1

    def is_namespace(self) -> bool:
        return self.current_line.find("namespace") != -1

    def is_enum(self) -> bool:
        return self.current_line.find("enum ") != -1

    def is_struct(self) -> bool:
        return self.current_line.find("struct ") != -1

    def is_using(self) -> bool:
        return self.current_line.find("using ") != -1

    def is_define_macro(self) -> bool:
        return self.current_line.find("#define ") != -1

    def is_access_modifier(self) -> bool:
        return self.current_line in ["private:", "public:", "protected:"]

    def is_firend_class(self) -> bool:
        return self.current_line.find("friend class ") != -1

    def is_class_forward_decl(self) -> bool:
        return self.current_line.find("class ") != -1 and self.current_line.find(";") != -1

    def is_class_definition(self) -> bool:
        return self.current_line.find("class ") != -1 and not self.is_class_forward_decl()

    def is_function_or_field(self) -> bool:
        return self.next_parenthese < self.end or self.next_semicolon < self.end or self.next_equal < self.end

    def is_method_or_constructor(self) -> bool:
        return (
            self.is_function_or_field()
            and self.next_parenthese < self.next_semicolon
            and (self.next_parenthese < self.next_equal or self.current_line.find("operator==") != -1)
        )

    def is_field(self) -> bool:
        return self.is_function_or_field()
