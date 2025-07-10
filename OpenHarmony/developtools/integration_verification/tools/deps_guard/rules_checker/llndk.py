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


class LLndkRule(BaseRule):
    RULE_NAME = "LLndk"

    def __init__(self, mgr, args):
        super().__init__(mgr, args)
        self.__out_path = mgr.get_product_out_path()
        self.__white_lists = self.load_llndk_json("llndk_info.json")
        self.__ignored_tags = ["platformsdk", "sasdk", "platformsdk_indirect", "ndk"]
        self.__valid_mod_tags = ["llndk"] + self.__ignored_tags

    def get_white_lists(self):
        return self.__white_lists

    def get_out_path(self):
        return self.__out_path

    def load_llndk_json(self, name):
        rules_dir = []
        rules_dir.append(os.path.join(os.path.dirname(os.path.realpath(__file__)), "../rules"))
        if self._args and self._args.rules:
            self.log("****add more llndk info in:{}****".format(self._args.rules))
            rules_dir = rules_dir + self._args.rules

        llndk_rules_path = self.get_out_path().replace("out", "out/products_ext")
        if os.path.exists(llndk_rules_path):
            self.log("****add more llndk info in dir:{}****".format(llndk_rules_path))
            rules_dir.append(llndk_rules_path)
        else:
            self.warn("****add llndk_rules_path path not exist: {}****".format(llndk_rules_path))
        res = []
        for d in rules_dir:
            rules_file = os.path.join(d, self.__class__.RULE_NAME, name)
            if os.path.isfile(rules_file):
                res = self.__parser_rules_file(rules_file, res)
            else:
                self.warn("****rules path not exist: {}****".format(rules_file))

        return res

    def check(self):
        self.__modules_with_llndk_tag = []

        for mod in self.get_mgr().get_all():
            if self.__is_llndk_tagged(mod):
                self.__modules_with_llndk_tag.append(mod)

        # Check if all llndk modules are correctly tagged by innerapi_tags
        passed = self.__check_if_tagged_correctly()
        self.log(f"****check_if_tagged_correctly result:{passed}****")
        if not passed:
            return passed
        
        passed = self.check_if_deps_correctly(
            self.__modules_with_llndk_tag, self.__valid_mod_tags, self.__valid_mod_tags)
        self.log(f"****check_if_deps_correctly result:{passed}****")
        if not passed:
            return passed

        return True

    def __parser_rules_file(self, rules_file, res):
        try:
            self.log("****Parsing rules file in {}****".format(rules_file))
            with open(rules_file, "r") as f:
                contents = f.read()
            if not contents:
                self.log("****rules file {} is null****".format(rules_file))
                return res
            json_data = json.loads(contents)
            for so in json_data:
                so_file_name = so.get("so_file_name")
                if so_file_name and so_file_name not in res:
                    res.append(so_file_name)
        except (FileNotFoundError, IOError, UnicodeDecodeError) as file_open_or_decode_err:
            self.error(file_open_or_decode_err)
        
        return res

    def __is_llndk_tagged(self, mod):
        if not "innerapi_tags" in mod:
            return False
        if "llndk" in mod["innerapi_tags"]:
            return True
        return False

    def __check_if_tagged_correctly(self):
        passed = True

        for mod in self.__modules_with_llndk_tag:
            if mod["name"] not in self.get_white_lists():
                passed = False
                self.error('non llndk module %s with innerapi_tags="llndk", %s'
                           % (mod["name"], mod["labelPath"]))

        return passed
