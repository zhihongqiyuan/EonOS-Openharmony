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

import string
import json
import sys
import os
import xml.etree.ElementTree as ET


def xml_node_find_by_name(node, name):
    for item in node:
        if item.tag == name:
            return item.text
    return None


class SAParser(object):
    @staticmethod
    def load(mgr, out_root_path):
        all_sa = {}
        path = os.path.join(out_root_path, "packages/phone/system/profile")
        if not os.path.exists(path):
            return

        for f in os.listdir(path):
            full_name = os.path.join(path, f)
            if os.path.isfile(full_name) and f.endswith(".json"):
                try:
                    SAParser.__parse_sa_profile(all_sa, full_name)
                except:
                    pass

        SAParser.__add_sa_info(all_sa, mgr)

    @staticmethod
    def __parse_sa_profile(all_sa, full_name):
        with open(full_name, "r") as f:
            profile = json.load(f)
        process = profile["process"]
        for sa in profile["systemability"]:
            libpath = sa["libpath"]
            sa_key = os.path.basename(libpath)
            sa["process"] = process
            all_sa[sa_key] = sa

    @staticmethod
    def __add_sa_info(all_sa, mgr):
        if not mgr:
            return
        for mod in mgr.get_all():
            mod["sa_id"] = 0
            if mod["name"] not in all_sa:
                continue
            mod["sa_id"] = int(all_sa[mod["name"]]["name"])


if __name__ == '__main__':
    parser = SAParser()
    parser.load(None, "/home/handy/qemu/out/rk3568")
