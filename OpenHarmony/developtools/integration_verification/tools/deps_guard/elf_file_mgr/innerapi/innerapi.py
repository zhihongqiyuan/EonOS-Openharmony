#!/usr/bin/env python
#coding=utf-8

#
# Copyright (c) 2022 Huawei Device Co., Ltd.
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

import os
import json


class InnerAPILoader(object):
    @staticmethod
    def load(mgr, product_out_path):
        print("Loading innerapis now ...")
        try:
            innerapis = InnerAPILoader.__load_innerapi_modules(product_out_path)
        except:
            innerapis = []

        if not mgr:
            return

        for elf in mgr.get_all():
            if elf["labelPath"] in innerapis:
                elf["innerapi_declared"] = True

    def __load_innerapi_modules(product_out_path):
        inner_kits_info = os.path.join(product_out_path, "build_configs/parts_info/inner_kits_info.json")
        with open(inner_kits_info, "r") as f:
            info = json.load(f)

        innerapis = []
        for name, component in info.items():
            for mod_name, innerapi in component.items():
                innerapis.append(innerapi["label"])

        return innerapis
