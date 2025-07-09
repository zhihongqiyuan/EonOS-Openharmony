#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Copyright (c) 2021 Huawei Device Co., Ltd.
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

import json
import os
import argparse


class SysResource:
    def __init__(self):
        self.records = {}
        self.json_records = None

    def read_json_file(self, json_file):
        with open(os.path.join(json_file), 'r') as fp:
            self.json_records = json.load(fp)["record"]

    def process_ids(self):
        for item in self.json_records:
            if ("flags" in item) and item["flags"].find("private") > -1:
                continue

            item_type = item["type"]
            if item_type != "color" \
                and item_type != "float" \
                and item_type != "media" \
                and item_type != "string" \
                and item_type != "symbol" \
                and item_type != "plural":
                continue

            item_name = item["name"]
            item_id = item["order"] + 0x7800000
            if item_type not in self.records:
                self.records[item_type] = dict()
            self.records[item_type][item_name] = item_id

    def write_to_file(self, js_file):
        output = open(js_file, 'w+')
        output.write("module.exports.sys = {\n")
        first_item = True
        for (item_type, dataset) in self.records.items():
            if first_item:
                first_item = False
                output.write("    %s: {\n" % (item_type))
            else:
                output.write(",\n    %s: {\n" % (item_type))
            first_line = True
            for (res_name, res_id) in dataset.items():
                if first_line:
                    first_line = False
                    output.write("        %s: %d" % (res_name, res_id))
                else:
                    output.write(",\n        %s: %d" % (res_name, res_id))
            output.write("\n    }")
        output.write("\n}\n")
        output.close()

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('--input-json', help='input path to id_defined.json')
    parser.add_argument('--output-js', help='output path to sysResource.js')
    options = parser.parse_args()

    processor = SysResource()
    processor.read_json_file(options.input_json)
    processor.process_ids()
    processor.write_to_file(options.output_js)

