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

import json

from .base_rule import BaseRule


class HdiRule(BaseRule):
    RULE_NAME = "NO-Depends-On-HDI"

    def check(self):
        return self.__check_depends_on_hdi()

    def __check_depends_on_hdi(self):
        lists = self.get_white_lists()

        passed = True

        hdi_without_shlib_type = []
        non_hdi_with_hdi_shlib_type = []

        # Check if any napi modules has dependedBy
        for mod in self.get_mgr().get_all():
            is_hdi = False
            if "hdiType" in mod and mod["hdiType"] == "hdi_service":
                is_hdi = True
            # Collect non HDI modules with shlib_type of value "hdi"
            if not is_hdi and ("shlib_type" in mod and mod["shlib_type"] == "hdi"):
                non_hdi_with_hdi_shlib_type.append(mod)

            # Collect HDI modules without shlib_type with value of "hdi"
            if is_hdi and ("shlib_type" not in mod or mod["shlib_type"] != "hdi"):
                if mod["name"] not in lists:
                    hdi_without_shlib_type.append(mod)

            if self.__ignore_mod(mod, is_hdi, lists):
                continue

            # Check if HDI modules is depended by other modules
            self.error("hdi module %s depended by:" % mod["name"])
            for dep in mod["dependedBy"]:
                caller = dep["caller"]
                self.log("   module [%s] defined in [%s]" % (caller["name"], caller["labelPath"]))
            passed = False

        if len(hdi_without_shlib_type) > 0:
            for mod in hdi_without_shlib_type:
                if mod["name"] not in lists:
                    passed = False
                    self.error('hdi module %s has no shlib_type="hdi", add it in %s' % (mod["name"], mod["labelPath"]))

        if len(non_hdi_with_hdi_shlib_type) > 0:
            for mod in non_hdi_with_hdi_shlib_type:
                self.warn('non hdi module %s with shlib_type="hdi", %s' % (mod["name"], mod["labelPath"]))

        return passed

    def __ignore_mod(self, mod, is_hdi, lists):
        ignore_flag = False
        if not is_hdi:
            ignore_flag = True
            return ignore_flag

        if len(mod["dependedBy"]) == 0:
            ignore_flag = True
            return ignore_flag

        if mod["name"] in lists:
            ignore_flag = True
            return ignore_flag

        # If hdi module has version_script to specify exported symbols, it can be depended by others
        if "version_script" in mod:
            ignore_flag = True
        
        return ignore_flag
