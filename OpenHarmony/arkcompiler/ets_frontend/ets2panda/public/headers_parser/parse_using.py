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
from text_tools import find_first_not_restricted_character, find_first_of_characters
from parse_arguments import parse_type


def parse_using(data: str, start: int = 0) -> Tuple[int, Dict]:
    res: Dict[str, Any] = {}

    start_of_name = find_first_not_restricted_character(" ", data, data.find("using ", start) + len("using "))
    end_of_name = find_first_of_characters(" =;\n", data, start_of_name)

    res["name"] = data[start_of_name:end_of_name]

    if data[end_of_name] == ";":
        return end_of_name, res

    var_start = find_first_not_restricted_character(" ", data, data.find("=", start))
    var_end = data.find(";", start)

    if var_end == -1:
        var_end = len(data)

    value = parse_type(data[var_start:var_end].strip(" ;="))
    res["type"] = value # NOTE(morlovsky): Not doing res["value"] = parse_argument(value)

    return var_end, res
