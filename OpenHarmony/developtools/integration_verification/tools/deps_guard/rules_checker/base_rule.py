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


class BaseRule(object):
    RULE_NAME = ""

    def __init__(self, mgr, args):
        self._mgr = mgr
        self._args = args
        self.__white_lists = self.load_files("whitelist.json")

    def load_files(self, name):
        rules_dir = []
        rules_dir.append(os.path.join(os.path.dirname(os.path.realpath(__file__)), "../rules"))
        if self._args and self._args.rules:
            rules_dir = rules_dir + self._args.rules

        res = []
        for d in rules_dir:
            rules_file = os.path.join(d, self.__class__.RULE_NAME, name)
            try:
                with open(rules_file, "r") as f:
                    jsonstr = "".join([line.strip() for line in f if not line.strip().startswith("//")])
                    res = res + json.loads(jsonstr)
            except:
                pass

        return res

    def get_mgr(self):
        return self._mgr

    def get_white_lists(self):
        return self.__white_lists

    def log(self, info):
        print(info)

    def warn(self, info):
        print("\033[35m[WARNING]\x1b[0m: %s" % info)

    def error(self, info):
        print("\033[91m[NOT ALLOWED]\x1b[0m: %s" % info)

    def get_help_url(self):
        return "https://gitee.com/openharmony/developtools_integration_verification/tree/master/tools/deps_guard/rules/%s/README.md" % self.__class__.RULE_NAME

    # To be override
    def check(self):
        # Default pass
        return True

    def check_if_deps_correctly(self, check_modules, valid_mod_tags, valid_dep_tags):
        # check if mod and callee have wrong innerapi tags 
        passed = True
        for mod in check_modules:
            innerapi_tags = mod["innerapi_tags"]
            if innerapi_tags and all(item in valid_mod_tags for item in innerapi_tags):
                for dep in mod["deps"]:
                    callee = dep["callee"]

                    dep_innerapi_tags = callee["innerapi_tags"]
                    if dep_innerapi_tags and all(item in valid_dep_tags for item in dep_innerapi_tags):
                        continue
                    elif not dep_innerapi_tags:
                        continue

                    passed = False
                    wrong_tags = [item for item in dep_innerapi_tags if item not in valid_dep_tags]
                    self.error("module %s with %s contains wrong dep innerapi_tags [%s] in innerapi_tags [%s]" 
                        %(callee["name"], callee["labelPath"], ",".join(wrong_tags), ",".join(dep_innerapi_tags)))
            else:
                wrong_tags = [item for item in innerapi_tags if item not in valid_mod_tags]
                self.error("module %s with %s contains wrong mod innerapi_tags [%s] in innerapi_tags [%s]" 
                           %(mod["name"], mod["labelPath"], ",".join(wrong_tags), ",".join(innerapi_tags)))
                return False

        return passed