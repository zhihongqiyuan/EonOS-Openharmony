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


class SaRule(BaseRule):
    RULE_NAME = "NO-Depends-On-SA"

    def check(self):
        return self.__check_depends_on_sa()

    def __check_depends_on_sa(self):
        lists = self.get_white_lists()

        passed = True

        sa_without_shlib_type = []
        non_sa_with_sa_shlib_type = []

        # Check if any napi modules has dependedBy
        for mod in self.get_mgr().get_all():
            is_sa = False
            if "sa_id" in mod and mod["sa_id"] > 0:
                is_sa = True
            # Collect non SA modules with shlib_type of value "sa"
            if not is_sa and ("shlib_type" in mod and mod["shlib_type"] == "sa"):
                non_sa_with_sa_shlib_type.append(mod)

            # Collect SA modules without shlib_type with value of "sa"
            if is_sa and ("shlib_type" not in mod or mod["shlib_type"] != "sa"):
                if mod["name"] not in lists:
                    sa_without_shlib_type.append(mod)

            if not is_sa:
                continue

            if len(mod["dependedBy"]) == 0:
                continue

            if mod["name"] in lists:
                continue

            # If sa module has version_script to specify exported symbols, it can be depended by others
            if "version_script" in mod:
                continue

            # Check if SA modules is depended by other modules
            self.error("sa module %s depended by:" % mod["name"])
            for dep in mod["dependedBy"]:
                caller = dep["caller"]
                self.log("   module [%s] defined in [%s]" % (caller["name"], caller["labelPath"]))
            passed = False

        if len(sa_without_shlib_type) > 0:
            for mod in sa_without_shlib_type:
                self.warn('sa module %s has no shlib_type="sa", add it in %s' % (mod["name"], mod["labelPath"]))

        if len(non_sa_with_sa_shlib_type) > 0:
            for mod in non_sa_with_sa_shlib_type:
                self.warn('non sa module %s with shlib_type="sa", %s' % (mod["name"], mod["labelPath"]))

        return passed
