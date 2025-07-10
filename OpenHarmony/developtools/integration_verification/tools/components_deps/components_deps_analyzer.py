#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# Copyright (c) 2023 Huawei Device Co., Ltd.
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

# This file provide the detection tool for unconditional dependence of required components on optional components.

import argparse
import json
import os
import re


class Analyzer:
    @classmethod
    def analysis(cls, gn_path_list, new_line_nums, gn_name, config_path: str, open_components_path,
                 result_json_name: str):
        if not os.path.exists(config_path):
            print("error: {} is inaccessible or not found".format(config_path))
            return
        if not os.path.exists(open_components_path):
            print("error: {} is inaccessible or not found".format(open_components_path))
            return
        if len(gn_path_list) != len(new_line_nums):
            print("error: The new_line_nums and the gn_path are not in one-to-one correspondence.")
            return
        if len(gn_path_list) != len(gn_name):
            print("error: The gn_path and gn_name are not in one-to-one correspondence.")
            return
        required_components = cls.__get_required_components(config_path)
        open_components, gn_name_list, white_list = cls.__get_open_components(open_components_path)
        gn_name2component = dict(zip(gn_name_list, open_components))
        optional_components = list()
        for components in open_components:
            if components not in required_components:
                optional_components.append(components)
        result = list()
        for i, _ in enumerate(gn_path_list):
            one_result = dict()
            one_result["file_path"] = gn_path_list[i]
            if gn_name[i] in gn_name_list and gn_name2component[gn_name[i]] in required_components:
                one_result["error"] = cls.__judge_deps(gn_path_list[i], new_line_nums[i], open_components,
                                                       optional_components, white_list)
            else:
                one_result["error"] = []
            result.append(one_result)
        with os.fdopen(os.open(result_json_name + ".json", os.O_WRONLY | os.O_CREAT, mode=0o640), "w",
                       encoding='utf-8') as fd:
            json.dump(result, fd, indent=4, ensure_ascii=False)

    @classmethod
    def __get_open_components(cls, xml_path):
        open_components = list()
        gn_name = list()
        white_components_list = ["common", "hilog", "ylong_runtime"]
        with open(xml_path, 'r', encoding='utf-8') as r:
            xml_info = r.readlines()
        for line in xml_info:
            if "path=" in line:
                one_component = re.findall('path="(.*?)"', line)[0].split('/')[-1]
                open_components.append(one_component)
                one_name = re.findall('name="(.*?)"', line)[0]
                gn_name.append(one_name)
        return open_components, gn_name, white_components_list

    @classmethod
    def __deal_config_json(cls, config_json):
        components = list()
        for subsystem in config_json['subsystems']:
            for component in subsystem['components']:
                if component not in components:
                    components.append(component['component'])
        return components

    @classmethod
    def __get_required_components(cls, config_path: str):
        required_components = list()
        files = os.listdir(config_path)
        for file in files:
            if file.endswith(".json"):
                with open(os.path.join(config_path, file), 'r', encoding='utf-8') as r:
                    config_json = json.load(r)
                required_components += cls.__deal_config_json(config_json)
        return required_components

    @classmethod
    def __get_line(cls, txt_list, key_words: str):
        for i, txt in enumerate(txt_list):
            if key_words in txt:
                return i + 1
        return 0

    @classmethod
    def __judge_deps(cls, gn_path: str, new_line_num: str, open_components_list, optional_components, white_names):
        deps = list()
        new_line_num = [int(i) for i in new_line_num.split('_')]
        with open(gn_path, 'r', encoding='utf-8') as r:
            gn_lines = [line.strip("\n") for line in r.readlines()]
        dependent_close = True
        txt = ''
        for line in gn_lines:
            txt += line
        for component in open_components_list:
            if dependent_close == True:
                if component in txt:
                    dependent_close = False
        scan_line_num = cls.__get_scan_line_num(gn_lines, new_line_num)
        for i in scan_line_num:
            if '/' in gn_lines[i - 1]:
                dep_info = re.findall('(.*?):', gn_lines[i - 1].split("/")[-1])[0]
            else:
                dep_info = re.findall('"(.*?):', gn_lines[i - 1])[0]
            for component in optional_components:
                if component not in white_names and component == dep_info:
                    deps.append((component, i))
        error = list()
        if dependent_close == True and re.findall('deps =', txt):
            line = cls.__get_line(gn_lines, 'deps =')
            error.append(
                {"line": line, "code": gn_lines[line - 1].strip(), "rule": "depend close component",
                 "detail": "可能依赖闭源部件，请检查deps中的内容"})
        for one_dep in deps:
            error.append(
                {"line": one_dep[1], "code": gn_lines[one_dep[1] - 1].strip(), "rule": "depend optional component",
                 "detail": "依赖开源部件中的非必选部件{}，请检查deps中的内容".format(one_dep[0])})
        return error

    @classmethod
    def __get_scan_line_num(cls, gn_lines, new_line_num):
        add_line_txt = ''
        line_num = 0
        for line in gn_lines:
            line_num += 1
            add_line_txt += '@' + str(line_num) + '@' + line
        in_if_txt = re.findall('if \(.+?\{(.*?)\}', add_line_txt)
        in_if_line_num = cls.__get_line_num(in_if_txt)
        in_dep_txt = re.findall('deps = \[(.*?)\]', add_line_txt) + re.findall('deps += \[(.*?)\]', add_line_txt)
        in_dep_line_num = cls.__get_line_num(in_dep_txt)
        for line_num, line in enumerate(gn_lines):
            if ('deps = ' in line or 'deps += ' in line) and ']' in line and (line_num + 1) not in in_dep_line_num:
                in_dep_line_num.append(line_num + 1)
        scan_line = list()
        for num in new_line_num:
            if num not in in_if_line_num and num in in_dep_line_num:
                scan_line.append(num)
        return scan_line

    @classmethod
    def __get_line_num(cls, txt_line_list):
        line_num = list()
        for one_txt in txt_line_list:
            one_line_list = re.findall('@(.*?)@', one_txt)
            if one_line_list != ['']:
                line_num += one_line_list
        line_num = [int(i) for i in line_num]
        return line_num


def get_args():
    parser = argparse.ArgumentParser(
        description=f"analyze components deps.\n")
    parser.add_argument("-p", "--components_gn_path_list", required=True, type=str,
                        help="path of pr BUILD.gn")
    parser.add_argument("-n", "--new_line_nums_list", required=True, type=str,
                        help="eg: 1_2_3,4_5")
    parser.add_argument("-g", "--gn_name", required=True, type=str,
                        help="gn file corresponding name")
    parser.add_argument("-c", "--config_path", required=True, type=str,
                        help="path of config_file")
    parser.add_argument("-o", "--open_component_xml_path", required=True, type=str,
                        help="open component name set")
    parser.add_argument("-r", "--result_json_name", type=str, default="result",
                        help="name of output_json")
    return parser.parse_args()


if __name__ == '__main__':
    args = get_args()
    gn_path_list_name = args.components_gn_path_list.split(',')
    new_line_nums_list = args.new_line_nums_list.split(',')
    gn_component_name = args.gn_name.split(',')
    args_config_path = args.config_path
    open_components_xml_path = args.open_component_xml_path
    result_json = args.result_json_name
    Analyzer.analysis(gn_path_list_name, new_line_nums_list, gn_component_name, args_config_path, open_components_xml_path,
                      result_json)