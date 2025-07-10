#!/usr/bin/env python3
# -*- coding: utf-8 -*-
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

API_VERSION_MAP = {
    "API9": "9.0.0.0",
    "API10": "9.0.0.0",
    "API11": "11.0.2.0",
    "API12beta1": "12.0.2.0",
    "API12beta2": "12.0.2.0",
    "API12beta3": "12.0.6.0",
    "API13": "12.0.6.0",
    "API14": "12.0.6.0",
    "API15": "12.0.6.0",
    "API16": "12.0.6.0",
    "API17": "12.0.6.0",
    "API18": "13.0.1.0",
    "API19": "13.0.1.0",
    "API20": "13.0.1.0",
}

ARK_JS_VM_LIST = {"API9", "API10", "API11", "API12beta1", "API12beta3", "API18"}

MIN_SUPPORT_BC_VERSION = "API12beta1"

MIX_COMPILE_ENTRY_POINT = "import"
