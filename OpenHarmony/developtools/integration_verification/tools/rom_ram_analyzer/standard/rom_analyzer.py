#!/usr/bin/env python3
# -*- coding: utf-8 -*-
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

# This file contains a RomAnalyzer for rom analyzation of standard device.

import argparse
import json
import os
import sys
import typing
from copy import deepcopy
from typing import Dict, Any, Iterator, List, Text
import re
import subprocess
from pkgs.rom_ram_baseline_collector import RomRamBaselineCollector
from pkgs.basic_tool import BasicTool, unit_adaptive
from pkgs.gn_common_tool import GnCommonTool, GnVariableParser
from pkgs.simple_excel_writer import SimpleExcelWriter

debug = bool(sys.gettrace())

NOTFOUND = "NOTFOUND"


class PreCollector:
    """
    collect some info that system_module_info.json dosn't contains
    """

    def __init__(self, project_path: str) -> None:
        self.info_dict: Dict[str, Any] = dict()
        self.project_path = BasicTool.get_abs_path(project_path)
        self.result_dict = dict()
    
    def collect_sa_profile(self):
        grep_kw = r"ohos_sa_profile"
        grep_cmd = f"grep -rn '{grep_kw}' --include=BUILD.gn {self.project_path}"
        content = BasicTool.execute(
            grep_cmd, post_processor=lambda x: x.split('\n'))
        for item in content:
            if not item or not item.split(':')[2].startswith("ohos_sa_profile"):
                continue
            self._process_single_sa(item, start_pattern=grep_kw)

    def _process_single_sa(self, item: str, start_pattern: str):
        gn, _, _ = item.split(':')
        with open(gn, 'r', encoding='utf-8') as f:
            content = f.read()
        p_itr: Iterator[re.Match] = BasicTool.match_paragraph(
            content=content, start_pattern=start_pattern)
        for p in p_itr:
            p_content = p.group()
            files: List[str] = GnVariableParser.list_parser(
                "sources", p_content)
            component_name, subsystem_name = GnCommonTool.find_part_subsystem(
                gn, self.project_path)
            for f in files:
                f = f.split('/')[-1]
                self.result_dict[f] = {
                    "subsystem_name": subsystem_name,
                    "component_name": component_name,
                    "gn_path": gn
                }


class RomAnalyzer:
    @classmethod
    def analysis(cls, system_module_info_json: Text, product_dirs: List[str],
                 project_path: Text, product_name: Text, output_file: Text, output_execel: bool, add_baseline: bool,
                 unit_adapt: bool):
        """
        system_module_info_json: json文件
        product_dirs：要处理的产物的路径列表如["vendor", "system/"]
        project_path: 项目根路径
        product_name: eg，rk3568
        output_file: basename of output file
        """
        project_path = BasicTool.get_abs_path(project_path)
        rom_baseline_dict: Dict[str, Any] = RomRamBaselineCollector.collect(
            project_path)
        with os.fdopen(os.open("rom_ram_baseline.json", os.O_WRONLY | os.O_CREAT, mode=0o640), 'w', encoding='utf-8') as f:
            json.dump(rom_baseline_dict, f, indent=4)
        phone_dir = os.path.join(
            project_path, "out", product_name, "packages", "phone")
        product_dirs = [os.path.join(phone_dir, d) for d in product_dirs]
        pre_collector = PreCollector(project_path)
        pre_collector.collect_sa_profile()
        extra_product_info_dict: Dict[str, Dict] = pre_collector.result_dict
        product_info_dict = cls.__collect_product_info(
            system_module_info_json, project_path,
            extra_info=extra_product_info_dict)  # collect product info from json file
        result_dict: Dict[Text:Dict] = dict()
        for d in product_dirs:
            file_list: List[Text] = BasicTool.find_all_files(d)
            for f in file_list:
                size = os.path.getsize(f)
                relative_filepath = f.replace(phone_dir, "").lstrip(os.sep)
                unit: Dict[Text, Any] = product_info_dict.get(
                    relative_filepath)
                if not unit:
                    bf = f.split('/')[-1]
                    unit: Dict[Text, Any] = product_info_dict.get(bf)
                if not unit:
                    unit = dict()
                unit["size"] = size
                unit["relative_filepath"] = relative_filepath
                cls.__put(unit, result_dict, rom_baseline_dict, add_baseline)
        output_dir, _ = os.path.split(output_file)
        if len(output_dir) != 0:
            os.makedirs(output_dir, exist_ok=True)
        if unit_adapt:
            cls.result_unit_adaptive(result_dict)
        with os.fdopen(os.open(output_file + ".json", os.O_WRONLY | os.O_CREAT, mode=0o640), 'w', encoding='utf-8') as f:
            f.write(json.dumps(result_dict, indent=4))
        if output_execel:
            cls.__save_result_as_excel(result_dict, output_file, add_baseline)
    
    @classmethod
    def result_unit_adaptive(self, result_dict: Dict[str, Dict]) -> None:
        for subsystem_name, subsystem_info in result_dict.items():
            size = unit_adaptive(subsystem_info["size"])
            count = subsystem_info["file_count"]
            if "size" in subsystem_info.keys():
                del subsystem_info["size"]
            if "file_count" in subsystem_info.keys():
                del subsystem_info["file_count"]
            for component_name, component_info in subsystem_info.items():
                component_info["size"] = unit_adaptive(component_info["size"])
            subsystem_info["size"] = size
            subsystem_info["file_count"] = count

    @classmethod
    def __collect_product_info(cls, system_module_info_json: Text,
                               project_path: Text, extra_info: Dict[str, Dict]) -> Dict[Text, Dict[Text, Text]]:
        """
        根据system_module_info.json生成target字典
        format:
            {
                "{file_name}":{
                    "{subsytem_name}": abc,
                    "{component_name}": def,
                    "{gn_path}": ghi
                }
            }
        if the unit of system_module_info.json has not field "label" and the "type" is "sa_profile",
        find the subsystem_name and component_name in the BUILD.gn
        """
        with open(system_module_info_json, 'r', encoding='utf-8') as f:
            product_list = json.loads(f.read())
        project_path = BasicTool.get_abs_path(project_path)
        product_info_dict: Dict[Text, Dict[Text, Text]] = dict()
        for unit in product_list:
            cs_flag = False
            dest: List = unit.get("dest")
            if not dest:
                print("warning: keyword 'dest' not found in {}".format(
                    system_module_info_json))
                continue
            label: Text = unit.get("label")
            gn_path = component_name = subsystem_name = None
            if label:
                cs_flag = True
                gn_path = os.path.join(project_path, label.split(':')[
                    0].lstrip('/'), "BUILD.gn")
                component_name = unit.get("part_name")
                subsystem_name = unit.get("subsystem_name")
                if not component_name:
                    cn, sn = GnCommonTool.find_part_subsystem(
                        gn_path, project_path)
                    component_name = cn
                if not subsystem_name:
                    cn, sn = GnCommonTool.find_part_subsystem(
                        gn_path, project_path)
                    subsystem_name = sn
            else:
                print("warning: keyword 'label' not found in {}".format(unit))
            for target in dest:
                if cs_flag:
                    product_info_dict[target] = {
                        "component_name": component_name,
                        "subsystem_name": subsystem_name,
                        "gn_path": gn_path,
                    }
                    continue
                tmp = target.split('/')[-1]
                pre_info = extra_info.get(tmp)
                if not pre_info:
                    continue
                else:
                    product_info_dict[target] = pre_info
        return product_info_dict

    @classmethod
    def __inside_save_result_as_excel(cls, add_baseline, subsystem_name, component_name,
                                      baseline, file_name, size):
        if add_baseline:
            return [subsystem_name, component_name,
                    baseline, file_name, size]
        else:
            return [subsystem_name, component_name, file_name, size]

    @classmethod
    def __save_result_as_excel(cls, result_dict: dict, output_name: str, add_baseline: bool):
        header = ["subsystem_name", "component_name",
                  "output_file", "size(Byte)"]
        if add_baseline:
            header = ["subsystem_name", "component_name", "baseline",
                      "output_file", "size(Byte)"]
        tmp_dict = deepcopy(result_dict)
        excel_writer = SimpleExcelWriter("rom")
        excel_writer.set_sheet_header(headers=header)
        subsystem_start_row = 1
        subsystem_end_row = 0
        subsystem_col = 0
        component_start_row = 1
        component_end_row = 0
        component_col = 1
        if add_baseline:
            baseline_col = 2
        for subsystem_name in tmp_dict.keys():
            subsystem_dict = tmp_dict.get(subsystem_name)
            subsystem_size = subsystem_dict.get("size")
            subsystem_file_count = subsystem_dict.get("file_count")
            del subsystem_dict["file_count"]
            del subsystem_dict["size"]
            subsystem_end_row += subsystem_file_count

            for component_name in subsystem_dict.keys():
                component_dict: Dict[str, int] = subsystem_dict.get(
                    component_name)
                component_size = component_dict.get("size")
                component_file_count = component_dict.get("file_count")
                baseline = component_dict.get("baseline")
                del component_dict["file_count"]
                del component_dict["size"]
                if add_baseline:
                    del component_dict["baseline"]
                component_end_row += component_file_count

                for file_name, size in component_dict.items():
                    line = cls.__inside_save_result_as_excel(add_baseline, subsystem_name, component_name,
                                                             baseline, file_name, size)
                    excel_writer.append_line(line)
                excel_writer.write_merge(component_start_row, component_col, component_end_row, component_col,
                                         component_name)
                if add_baseline:
                    excel_writer.write_merge(component_start_row, baseline_col, component_end_row, baseline_col,
                                             baseline)
                component_start_row = component_end_row + 1
            excel_writer.write_merge(subsystem_start_row, subsystem_col, subsystem_end_row, subsystem_col,
                                     subsystem_name)
            subsystem_start_row = subsystem_end_row + 1
        excel_writer.save(output_name + ".xls")

    @classmethod
    def __put(cls, unit: typing.Dict[Text, Any], result_dict: typing.Dict[Text, Dict], baseline_dict: Dict[str, Any],
              baseline: bool):

        component_name = NOTFOUND if unit.get(
            "component_name") is None else unit.get("component_name")
        subsystem_name = NOTFOUND if unit.get(
            "subsystem_name") is None else unit.get("subsystem_name")

        def get_rom_baseline():
            if (not baseline_dict.get(subsystem_name)) or (not baseline_dict.get(subsystem_name).get(component_name)):
                return str()
            return baseline_dict.get(subsystem_name).get(component_name).get("rom")

        size = unit.get("size")
        relative_filepath = unit.get("relative_filepath")
        if result_dict.get(subsystem_name) is None:  # 子系统
            result_dict[subsystem_name] = dict()
            result_dict[subsystem_name]["size"] = 0
            result_dict[subsystem_name]["file_count"] = 0

        if result_dict.get(subsystem_name).get(component_name) is None:  # 部件
            result_dict[subsystem_name][component_name] = dict()
            result_dict[subsystem_name][component_name]["size"] = 0
            result_dict[subsystem_name][component_name]["file_count"] = 0
            if baseline:
                result_dict[subsystem_name][component_name]["baseline"] = get_rom_baseline(
                )

        result_dict[subsystem_name]["size"] += size
        result_dict[subsystem_name]["file_count"] += 1
        result_dict[subsystem_name][component_name]["size"] += size
        result_dict[subsystem_name][component_name]["file_count"] += 1
        result_dict[subsystem_name][component_name][relative_filepath] = size


def get_args():
    version_num = 2.0
    parser = argparse.ArgumentParser(
        description=f"analyze rom size of component.\n")
    parser.add_argument("-v", "-version", action="version",
                        version=f"version {version_num}")
    parser.add_argument("-p", "--project_path", type=str, required=True,
                        help="root path of openharmony. eg: -p ~/openharmony")
    parser.add_argument("-j", "--module_info_json", required=True, type=str,
                        help="path of out/{product_name}/packages/phone/system_module_info.json")
    parser.add_argument("-n", "--product_name", required=True,
                        type=str, help="product name. eg: -n rk3568")
    parser.add_argument("-d", "--product_dir", required=True, action="append",
                        help="subdirectories of out/{product_name}/packages/phone to be counted."
                             "eg: -d system -d vendor")
    parser.add_argument("-b", "--baseline", action="store_true",
                        help="add baseline of component to the result(-b) or not.")
    parser.add_argument("-o", "--output_file", type=str, default="rom_analysis_result",
                        help="basename of output file, default: rom_analysis_result. eg: demo/rom_analysis_result")
    parser.add_argument("-u", "--unit_adaptive",
                        action="store_true", help="unit adaptive")
    parser.add_argument("-e", "--excel", type=bool, default=False,
                        help="if output result as excel, default: False. eg: -e True")
    args = parser.parse_args()
    return args


if __name__ == '__main__':
    args = get_args()
    module_info_json = args.module_info_json
    project_origin_path = args.project_path
    product = args.product_name
    product_dirs = args.product_dir
    output_file_name = args.output_file
    output_excel = args.excel
    baseline_path = args.baseline
    unit_adaptiv = args.unit_adaptive
    RomAnalyzer.analysis(module_info_json, product_dirs,
                         project_origin_path, product, output_file_name, output_excel, baseline_path, unit_adaptiv)
