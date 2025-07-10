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

"""Keywords from headers, to correctly recognize them in parser."""

known_macroses = [
    "NO_COPY_OPERATOR",
    "NO_MOVE_SEMANTIC",
    "NO_COPY_SEMANTIC",
    "DEFINE_BITOPS",
    "DECLARE_NODE_TYPES",
    "DEFAULT_MOVE_SEMANTIC",
    "TYPE_MAPPING",
    "AST_NODE_MAPPING",
    "AST_NODE_REINTERPRET_MAPPING",
    "DECLARE_FLAG_OPERATIONS",
    "SCOPE_TYPES",
    "DEFAULT_COPY_SEMANTIC",
    "VARIABLE_TYPES",
    "DECLARATION_KINDS",
]

modifiers_list = [
    "const",
    "constexpr",
    "explicit",
    "export",
    "extern",
    "inline",
    "mutable",
    "noexcept",
    "template",
    "throw",
    "typename",
    "virtual",
    "volatile",
    "[[maybe_unused]]",
    "[[nodiscard]]",
    "[[noreturn]]",
    "static",
]
