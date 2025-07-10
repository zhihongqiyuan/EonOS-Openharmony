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


class NapiRule(BaseRule):
    RULE_NAME = "NO-Depends-On-NAPI"

    def check(self):
        return self.__check_depends_on_napi()

    def __check_depends_on_napi(self):
        lists = self.get_white_lists()

        passed = True

        # Check if any napi modules has dependedBy
        for mod in self.get_mgr().get_all():
            if not mod["napi"]:
                continue

            if len(mod["dependedBy"]) == 0:
                continue

            target_name = mod["labelPath"][mod["labelPath"].find(":")+1:]
            if target_name in lists:
                continue

            self.error("napi module %s depended by:" % mod["name"])
            for dep in mod["dependedBy"]:
                caller = dep["caller"]
                self.log("   module [%s] defined in [%s]" % (caller["name"], caller["labelPath"]))
            passed = False

        return passed
