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


class PassthroughRule(BaseRule):
    RULE_NAME = "Passthrough"

    def __init__(self, mgr, args):
        super().__init__(mgr, args)
        self.__out_path = mgr.get_product_out_path()
        self.__white_lists = self.load_passthrough_json("passthrough_info.json")
        self.__ignored_tags = ["platformsdk", "sasdk", "platformsdk_indirect", "ndk"]
        self.__valid_mod_tags = ["llndk", "chipsetsdk_sp", "passthrough", "passthrough_indirect"] + self.__ignored_tags

    def get_white_lists(self):
        return self.__white_lists

    def get_out_path(self):
        return self.__out_path

    def load_passthrough_json(self, name):
        rules_dir = []
        rules_dir.append(os.path.join(os.path.dirname(os.path.realpath(__file__)), "../rules"))
        if self._args and self._args.rules:
            self.log("****add more ChipsetSDK info in:{}****".format(self._args.rules))
            rules_dir = rules_dir + self._args.rules

        chipsetsdk_rules_path = self.get_out_path().replace("out", "out/products_ext")
        if os.path.exists(chipsetsdk_rules_path):
            self.log("****add more ChipsetSDK info in dir:{}****".format(chipsetsdk_rules_path))
            rules_dir.append(chipsetsdk_rules_path)
        else:
            self.warn("****add chipsetsdk_rules_path path not exist: {}****".format(chipsetsdk_rules_path))
        res = []
        for d in rules_dir:
            rules_file = os.path.join(d, self.__class__.RULE_NAME, name)
            if os.path.isfile(rules_file):
                res = self.__parser_rules_file(rules_file, res)
            else:
                self.warn("****rules path not exist: {}****".format(rules_file))

        return res

    def check(self):
        self.__load_passthrough_indirects()

        # Check if all chipset modules depends on chipsetsdk modules only
        passed = self.__check_depends_on_passthrough()
        self.log(f"****check_depends_on_chipsetsdk result:{passed}****")
        if not passed:
            return passed

        # Check if all chipsetsdk module depends on chipsetsdk or chipsetsdk_indirect modules only
        passed = self.__check_passthrough_indirect()
        self.log(f"****check_chipsetsdk_indirect result:{passed}****")
        if not passed:
            return passed

        # Check if all ChipsetSDK modules are correctly tagged by innerapi_tags
        passed = self.__check_if_passthrough_tagged_correctly()
        self.log(f"****check_if_tagged_correctly result:{passed}****")
        if not passed:
            return passed
        
        passed = self.check_if_deps_correctly(
            self.__modules_with_passthrough_tag, self.__valid_mod_tags, self.__valid_mod_tags)
        self.log(f"****check_if_deps_correctly result:{passed}****")
        if not passed:
            return passed
        
        passed = self.check_if_deps_correctly(
            self.__modules_with_passthrough_tag, self.__valid_mod_tags, self.__valid_mod_tags)
        self.log(f"****check_if_deps_correctly indirect result:{passed}****")
        if not passed:
            return passed

        self.__write_innerkits_header_files()

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

    def __is_passthrough_tagged(self, mod):
        if not "innerapi_tags" in mod:
            return False
        if "passthrough" in mod["innerapi_tags"]:
            return True
        return False

    def __is_passthrough_indirect(self, mod):
        if not "innerapi_tags" in mod:
            return False
        if "passthrough_indirect" in mod["innerapi_tags"]:
            return True
        return False

    def __write_innerkits_header_files(self):
        inner_kits_info = os.path.join(self.get_mgr().get_product_out_path(), 
                                       "build_configs/parts_info/inner_kits_info.json")
        with open(inner_kits_info, "r") as f:
            info = json.load(f)

        headers = []
        for sdk in self.__passthroughs:
            path = sdk["labelPath"][:sdk["labelPath"].find(":")]
            target_name = sdk["labelPath"][sdk["labelPath"].find(":") + 1:]
            item = {"name": sdk["componentName"] + ":" + target_name, "so_file_name":
                    sdk["name"], "path": sdk["labelPath"], "headers": []}
            if sdk["componentName"] not in info:
                headers.append(item)
                continue

            for name, innerapi in info[sdk["componentName"]].items():
                if innerapi["label"] != sdk["labelPath"]:
                    continue
                got_headers = True
                base = innerapi["header_base"]
                for f in innerapi["header_files"]:
                    item["headers"].append(os.path.join(base, f))
            headers.append(item)

        try:
            with os.fdopen(os.open(os.path.join(self.get_mgr().get_product_images_path(),
                                                "passthrough_info.json"),
                                    os.O_WRONLY | os.O_CREAT, 0o644), "w") as f:
                json.dump(headers, f, indent=4)
        except:
            pass

        return headers

    def __check_passthrough_indirect(self):
        passed = True
        for mod in self.__passthroughs:
            for dep in mod["deps"]:
                callee = dep["callee"]

                # Passthrough is OK
                if callee["name"] in self.get_white_lists():
                    continue

                # Passthrough_indirect module is OK
                if self.__is_passthrough_indirect(callee) or callee["name"] in self.__indirects:
                    continue

                # Not correct
                passed = False
                self.error('Passthrough module %s should not depends on non Passthrough module \
                           %s in %s with "chipsetsdk_indirect"' % (mod["name"], callee["name"], callee["labelPath"]))

        return passed

    def __check_depends_on_passthrough(self):
        lists = self.get_white_lists()

        passed = True

        self.__passthroughs = []
        self.__modules_with_passthrough_tag = []
        self.__modules_with_passthrough_indirect_tag = []

        # Check if any napi modules has dependedBy
        for mod in self.get_mgr().get_all():
            # Collect all modules with passthrough tag
            if self.__is_passthrough_tagged(mod):
                self.__modules_with_passthrough_tag.append(mod)

            # Collect all modules with passthrough_indirect tag
            if self.__is_passthrough_indirect(mod):
                self.__modules_with_passthrough_indirect_tag.append(mod)

            # Check chipset modules only
            if mod["path"].startswith("system"):
                continue

            # Check chipset modules depends
            for dep in mod["deps"]:
                callee = dep["callee"]

                # If callee is chipset module, it is OK
                if not callee["path"].startswith("system"):
                    continue

                # Add to list
                if callee not in self.__passthroughs:
                    if "hdiType" not in callee or callee["hdiType"] != "hdi_proxy":
                        self.__passthroughs.append(callee)

                # If callee is in passthroughwhite list module, it is OK
                if callee["name"] in lists:
                    continue

                # If callee is asan library, it is OK
                if callee["name"].endswith(".asan.so"):
                    continue

                # If callee is hdi proxy module, it is OK
                if "hdiType" in callee and callee["hdiType"] == "hdi_proxy":
                    continue

                # Not allowed
                passed = False
                self.error("chipset module %s depends on non Chipset SDK module %s in %s"
                           % (mod["name"], callee["name"], mod["labelPath"]))

        return passed

    def __check_if_passthrough_tagged_correctly(self):
        passed = True
        for mod in self.__passthroughs:
            if not self.__is_passthrough_tagged(mod):
                self.warn('passthrough module %s has no innerapi_tags with "passthrough", add it in %s'
                          % (mod["name"], mod["labelPath"]))

        for mod in self.__modules_with_passthrough_tag:
            if mod["name"] not in self.get_white_lists():
                passed = False
                self.error('non passthrough %s with innerapi_tags="passthrough", %s'
                           % (mod["name"], mod["labelPath"]))

        for mod in self.__modules_with_passthrough_indirect_tag:
            if mod["name"] not in self.__indirects and mod["name"] not in self.get_white_lists():
                self.warn('non passthrough_indirect module %s with innerapi_tags="passthrough_indirect", %s'
                          % (mod["name"], mod["labelPath"]))

        return passed

    def __load_passthrough_indirects(self):
        self.__indirects = self.load_passthrough_json("passthrough_indirect.json")
