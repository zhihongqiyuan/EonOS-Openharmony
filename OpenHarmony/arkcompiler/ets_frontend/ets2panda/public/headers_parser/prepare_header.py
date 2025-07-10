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


from typing import Tuple, List
from log_tools import debug_log
from text_tools import find_first_of_characters, find_scope_borders


def remove_comments(data: str) -> str:
    """
    Returns data without c++ comments.
    """
    # Remove one-line comment
    double_slash_pos = data.find("//")

    while double_slash_pos != -1:
        end_of_line = data.find("\n", double_slash_pos)

        if end_of_line == -1:
            end_of_line = len(data)
            debug_log(f"Removing single-line comment at end of file:\n'{data[double_slash_pos:end_of_line]}'")

        data = data[:double_slash_pos] + data[end_of_line:]
        double_slash_pos = data.find("//")

    # Remove multi-line comment
    multiline_comment_start = data.find("/*")

    while multiline_comment_start != -1:
        multiline_comment_end = data.find("*/", multiline_comment_start)

        if multiline_comment_end == -1:
            raise RuntimeError("Error find end of multiline-comment")

        data = data.replace(data[multiline_comment_start : multiline_comment_end + 2], "")
        multiline_comment_start = data.find("/*")

    return data.strip(" \n")


def extract_and_remove_includes(data: str) -> Tuple[str, List[str]]:
    """
    Returns data without includes and list of includes in file in format like:
    [ '<ir/expression.h>', '"es2panda.h"' ]
    """
    current_pos = data.find("#include")
    res = []

    while current_pos != -1:
        include_start = find_first_of_characters('"<', data, current_pos)

        if data[include_start] == '"':
            include_end = data.find('"', include_start + 1)

        else:
            include_start, include_end = find_scope_borders(data, current_pos, "<")

        if include_start != -1 and include_end != -1:
            include = data[include_start : include_end + 1]
            res.append(include)
            data = data[:current_pos] + data[include_end + 1 :]

            current_pos = data.find("#include", current_pos)

        else:
            raise RuntimeError("Error while parsing includes")

    return data.strip(" \n"), res
