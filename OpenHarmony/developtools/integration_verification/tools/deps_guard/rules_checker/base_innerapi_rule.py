#!/usr/bin/env python
#coding=utf-8

#
# Copyright (c) 2025 Huawei Device Co., Ltd.
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

from .base_rule import BaseRule


class BaseInnerapiRule(BaseRule):
    RULE_NAME = "BaseInnerApi"

    def __init__(self, mgr, args):
        super().__init__(mgr, args)
        self.__ignored_tags = ["platformsdk", "sasdk", "platformsdk_indirect", "ndk"]
        self.__valid_system_tags = ["llndk", "chipsetsdk", "chipsetsdk_indirect", "chipsetsdk_sp"
                                    "chipsetsdk_sp_indirect"] + self.__ignored_tags
        self.__valid_vendor_tags = ["chipsetsdk", "chipsetsdk_indirect", "chipsetsdk_sp", "llndk", "passthrough",
                                 "chipsetsdk_sp_indirect", "passthrough_indirect"] + self.__ignored_tags

    def is_only(self, ignored_tags, mod):   
        innerapi_tags = mod["innerapi_tags"]
        # check whether each innerapi_tag of mod in ignored tags
        if innerapi_tags:
            for innerapi_tag in innerapi_tags:
                if innerapi_tag not in ignored_tags:
                    return False
        # check mod belongs to system module or vendor module
        if mod["path"].startswith("system"):
            return "system"
        else:
            return "vendor" 

    def check(self):
        passed = True

        for mod in self.get_mgr().get_all():
            innerapi_tags = mod["innerapi_tags"]
            # mod is system only scene
            if self.is_only(self.__ignored_tags, mod) == "system" and \
                    all(item in self.__valid_system_tags for item in innerapi_tags):
                for dep in mod["deps"]:
                    callee = dep["callee"]
                    callee_innerapi_tags = callee["innerapi_tags"]
                    if self.is_only(self.__ignored_tags, callee) == "system" or \
                            all(item in self.__valid_system_tags for item in callee_innerapi_tags):
                        continue
                    else:
                        self.error("system only module %s depends on wrong module as %s in %s" 
                                   %(mod["name"], callee["name"], mod["labelPath"]))
                        passed = False
            # mod is vendor only scene
            elif self.is_only(self.__ignored_tags, mod) == "vendor" and \
                    all(item in self.__valid_vendor_tags for item in innerapi_tags):
                for dep in mod["deps"]:
                    callee = dep["callee"]
                    callee_innerapi_tags = callee["innerapi_tags"]
                    if self.is_only(self.__ignored_tags, callee) == "vendor" or \
                            all(item in self.__valid_vendor_tags for item in callee_innerapi_tags):
                        continue
                    else:
                        self.error("vendor only module %s depends on wrong module as %s in %s" 
                                   %(mod["name"], callee["name"], mod["labelPath"]))
                        passed = False
        return passed