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


"""Module provides custom text tools for parsing."""

from typing import Tuple, Dict
from log_tools import debug_log


MAX_LEN = 10000000


def find_first_not_restricted_character(restricted: str, data: str, pos: int = 0, pos_end: int = MAX_LEN) -> int:
    for i in range(pos, min(len(data), pos_end)):
        if data[i] not in restricted:
            return i
    return len(data)


def rfind_first_not_restricted_character(restricted: str, data: str, pos: int, pos_end: int = 0) -> int:
    """pos_end includes in searching"""
    if pos > len(data):
        pos = len(data) - 1
    while pos >= max(0, pos_end):
        if data[pos] not in restricted:
            return pos
        pos -= 1
    return len(data)


def find_first_of_characters(characters: str, data: str, pos: int = 0, pos_end: int = MAX_LEN) -> int:
    for i in range(pos, min(len(data), pos_end)):
        if data[i] in characters:
            return i
    return len(data)


def rfind_first_of_characters(characters: str, data: str, pos: int, pos_end: int = 0) -> int:
    """pos_end includes in searching"""
    if pos > len(data):
        pos = len(data) - 1
    while pos >= max(0, pos_end):
        if data[pos] in characters:
            return pos
        pos -= 1
    return len(data)


def find_scope_borders(data: str, start: int = 0, opening: str = "{") -> Tuple[int, int]:
    """
    Returns pos of opening and closing brackets in 'data'.
    If it can't find proper scope -> raise error.
    """
    brackets_match: Dict[str, str] = {
        "{": "}",
        "(": ")",
        "<": ">",
        "[": "]",
    }

    if opening == "":
        opening_pos = find_first_of_characters("({<[", data, start)
        if opening_pos == len(data):
            raise RuntimeError("Error while finding end of scope in ANY mode")
        opening = data[opening_pos]

    closing = brackets_match[opening] # CC-OFF(G.TYP.07) dict key exist
    start_of_scope = data.find(opening, start)

    if start_of_scope == -1:
        raise RuntimeError("No opening bracket found!")

    end_of_scope = start_of_scope + 1

    def check_opening_closing() -> bool:
        openings = data[start_of_scope : end_of_scope + 1].count(opening)
        closings = data[start_of_scope : end_of_scope + 1].count(closing)
        return openings == closings

    while not check_opening_closing():
        end_of_scope = data.find(closing, end_of_scope + 1)

        if end_of_scope == -1:
            raise RuntimeError("Error while finding end of scope.")

    return start_of_scope, end_of_scope


def smart_split_by(data: str, delim: str = ",") -> list:
    data = data.strip(" \n")

    res = []
    segment_start = 0

    while segment_start < len(data):

        next_delim = smart_find_first_of_characters(delim, data, segment_start)

        segment = data[segment_start:next_delim].strip(" \n")
        if segment != "":
            res.append(segment)
        else:
            debug_log("Empty segment in smart_split_by.")

        segment_start = find_first_not_restricted_character(f"{delim} \n", data, next_delim)

    return res


def smart_find_first_of_characters(characters: str, data: str, pos: int) -> int:
    i = pos
    while i < len(data):
        if data[i] in characters:
            return i

        if data[i] in "<({[":
            _, close_bracket = find_scope_borders(data, i, "")
            i = close_bracket

        elif data[i] == '"':
            i = data.find('"', i + 1)
            while i != -1 and data[i] == '"' and i != 0 and data[i - 1] == "\\":
                i = data.find('"', i + 1)

        elif data[i] == "'":
            i = data.find("'", i + 1)

        i += 1

    return len(data)


def check_cpp_name(data: str) -> bool:
    data = data.lower()
    forbidden_chars = " ~!@#$%^&*()-+=[]\\{}|;:'\",./<>?"
    return find_first_of_characters(forbidden_chars, data) == len(data)
