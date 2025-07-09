#!/usr/bin/env python
# -*- coding: utf-8 -*-

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

import argparse
import json
import os
import re


class ConfigHeader:

    def __init__(self, config_json_path):
        self.read_buffer = []
        self.write_buffer = []
        with os.fdopen(os.open(config_json_path, os.O_RDONLY, 0o755), 'r') as file:
            self.configs = json.load(file)

    def parse_file(self, file_path):
        with os.fdopen(os.open(file_path, os.O_RDONLY, 0o755), 'r') as file:
            self.read_buffer = file.readlines()
        for item in self.read_buffer:
            if not item.startswith("#cmakedefine") and not item.startswith("#define"):
                self.write_buffer.append(item)
                continue

            value = item
            if item.startswith("#cmakedefine"):
                value = item.replace("#cmakedefine", "#define")
            match_object = re.search(r' \w+ ', value)
            if match_object is None:
                self.write_buffer.append("/* " + item + " */")
                continue
            key = match_object.group()
            key_index = key.replace(" ", "")
            if key_index not in self.configs:
                self.write_buffer.append("/* " + item + " */")
                continue

            match_object = re.search(r'@\w+@', value)
            if match_object is None:
                self.write_buffer.append(value)
                continue
            key = match_object.group()
            key_index = key.replace("@", "")
            if key_index not in self.configs:
                self.write_buffer.append("/* " + item + " */")
                continue
            value = value.replace(key, self.configs[key_index])
            self.write_buffer.append(value)

    def write_file(self, file_path):
        self.write_buffer.append("#define HAVE_SNPRINTF 1\n")
        with os.fdopen(os.open(file_path, os.O_WRONLY | os.O_CREAT, 0o755), 'w') as file:
            file.truncate(0)
            file.writelines(self.write_buffer)


class XmlVersionHeader:

    def __init__(self, xmlversion_json_path):
        self.read_buffer = []
        self.write_buffer = []
        with os.fdopen(os.open(xmlversion_json_path, os.O_RDONLY, 0o755), 'r') as file:
            self.configs = json.load(file)
    
    def parse_file(self, file_path):
        with os.fdopen(os.open(file_path, os.O_RDONLY, 0o755), 'r') as file:
            self.read_buffer = file.readlines()
        for item in self.read_buffer:
            if not item.startswith("#if") and not item.startswith("#define"):
                self.write_buffer.append(item)
                continue

            match_object = re.search(r'@\w+@', item)
            if match_object is None:
                self.write_buffer.append(item)
                continue
            key = match_object.group()
            index_key = key.replace("@", "")
            value = ""
            if index_key in self.configs:
                value = item.replace(key, self.configs[index_key])
            else:
                value = item.replace(key, "0")
            self.write_buffer.append(value)
    
    def write_file(self, file_path):
        with os.fdopen(os.open(file_path, os.O_WRONLY | os.O_CREAT, 0o755), 'w') as file:
            file.truncate(0)
            file.writelines(self.write_buffer)

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('--config-input-path', help='input path to config.h')
    parser.add_argument('--config-path', help='output path to config.h')
    parser.add_argument('--xmlversion-input-path', help='input path to xmlversion.h')
    parser.add_argument('--xmlversion-path', help='output path to xmlversion.h')
    parser.add_argument('--config-json', help='config value for config.h')
    parser.add_argument('--xmlversion-json', help='config value for xmlversion.h')
    options = parser.parse_args()
    config_header = ConfigHeader(options.config_json)
    config_header.parse_file(options.config_input_path)
    config_header.write_file(options.config_path)

    xmlversion_header = XmlVersionHeader(options.xmlversion_json)
    xmlversion_header.parse_file(options.xmlversion_input_path)
    xmlversion_header.write_file(options.xmlversion_path)
