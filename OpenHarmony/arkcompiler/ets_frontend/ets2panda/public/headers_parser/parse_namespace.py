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
from text_tools import find_scope_borders


def parse_namespace(data: str, start: int = 0) -> Tuple[int, Dict]:
    res = {}
    namespace_name = data[data.find("namespace", start) : data.find("{", start)].replace("namespace", "").strip(" ")
    namespace_name = namespace_name[namespace_name.rfind("::") + 2 :]
    namespace_start, namespace_end = find_scope_borders(data, start)

    namespace_body = data[namespace_start + 1 : namespace_end].strip(" \n")

    from cpp_parser import CppParser  # pylint: disable=C0415

    res[namespace_name] = CppParser(namespace_body, namespace_name).parse()

    return namespace_end, res
