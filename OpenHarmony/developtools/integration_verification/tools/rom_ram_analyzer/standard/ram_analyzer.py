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

# This file is to implement the rom analyzation of standard device.
#

import argparse
import copy
import glob
import json
import os
import re
import sys
import subprocess
import typing
import xml.dom.minidom as dom
from typing import Dict
from pprint import pprint
from pkgs.basic_tool import unit_adaptive

from pkgs.simple_excel_writer import SimpleExcelWriter

debug = True if sys.gettrace() else False


class HDCTool:
    @classmethod
    def verify_hdc(cls, verify_str: str = "OpenHarmony") -> bool:
        """
        验证hdc是否可用
        True：可用
        False：不可用
        """
        cp = subprocess.run(["hdc", "--help"], capture_output=True)
        stdout = str(cp.stdout)
        stderr = str(cp.stderr)
        return verify_str in stdout or verify_str in stderr

    @classmethod
    def verify_device(cls, device_num: str) -> bool:
        """
        验证设备是否已经连接
        True：已连接
        False：未连接
        """
        cp = subprocess.run(["hdc", "list", "targets"], capture_output=True)
        stdout = str(cp.stdout)
        stderr = str(cp.stderr)
        return device_num in stderr or device_num in stdout

    @classmethod
    def exec(cls, args: list, output_from: str = "stdout"):
        cp = subprocess.run(args, capture_output=True)
        if output_from == "stdout":
            return cp.stdout.decode()
        elif output_from == "stderr":
            return cp.stderr.decode()
        else:
            print("error: 'output_from' must be stdout or stdin")


def delete_values_from_dict(target_dict: typing.Dict, key_list: typing.Iterable):
    for k in key_list:
        if k not in target_dict.keys():
            continue
        del target_dict[k]


class RamAnalyzer:
    @classmethod
    def analysis(cls, cfg_path: str, json_path: str, rom_result_json: str, device_num: str,
                 output_file: str, ss: str, output_excel: bool, baseline_file: str, unit_adapt: bool):
        """
        process size subsystem/component so so_size
        """
        if not HDCTool.verify_hdc():
            print("error: Command 'hdc' not found")
            return
        if not HDCTool.verify_device(device_num):
            print("error: {} is inaccessible or not found".format(device_num))
            return
        with open(rom_result_json, 'r', encoding='utf-8') as f:
            rom_result_dict: typing.Dict = json.loads(f.read())
        # 从rom的分析结果中将需要的elf信息重组
        so_info_dict: typing.Dict[
            str, typing.Dict[str["component_name|subsystem_name|size"], str]] = cls.get_elf_info_from_rom_result(
            rom_result_json)
        process_elf_dict: typing.Dict[str, typing.List[str]] = cls.get_process_so_relationship(cfg_path,
                                                                                               json_path)
        process_size_dict: typing.Dict[str, int] = cls.process_hidumper_info(
            device_num, ss)
        result_dict: typing.Dict[str, typing.Dict[str, typing.Any]] = dict()
        result_dict = cls.result_process4(result_dict, process_size_dict, rom_result_dict, process_elf_dict,
                                          so_info_dict)
        base_dir, _ = os.path.split(output_file)
        if len(base_dir) != 0 and not os.path.isdir(base_dir):
            os.makedirs(base_dir, exist_ok=True)
        with os.fdopen(os.open(output_file + ".json", os.O_WRONLY | os.O_CREAT, mode=0o640), 'w', encoding='utf-8') as f:
            json.dump(result_dict, f, indent=4)
        refactored_result: Dict[str, Dict] = refacotr_result(result_dict)
        if unit_adapt:
            cls.refactored_result_unit_adaptive(refactored_result)
        if baseline_file:
            cls.add_baseline(refactored_result, baseline_file)
        with os.fdopen(os.open(f"refactored_{output_file}.json", os.O_WRONLY | os.O_CREAT, mode=0o640), 'w', encoding='utf-8') as f:
            json.dump(refactored_result, f, indent=4)
        if output_excel:
            cls.__save_result_as_excel(
                refactored_result, output_file + ".xls", ss, baseline_file, unit_adapt)

    __ss_dict: typing.Dict[str, int] = {
        "Pss": 2,
        "Vss": 3,
        "Rss": 4,
        "Uss": 5
    }

    @classmethod
    def process_hidumper_info(cls, device_num: str, ss: str) -> typing.Dict[str, int]:
        """
        处理进程名与对应进程大小
        """

        def exec_once() -> typing.Dict[str, int]:
            stdout = HDCTool.exec(
                ["hdc", "-t", device_num, "shell", "hidumper", "--mem"])
            name_size_dict = cls.__parse_hidumper_mem(stdout, device_num, ss)
            return name_size_dict

        if not HDCTool.verify_hdc():
            print("error: Command 'hdc' not found")
            return dict()
        if not HDCTool.verify_device(device_num):
            print("error: {} is inaccessible or not found".format(device_num))
            return dict()

        return exec_once()

    @classmethod
    def get_elf_info_from_rom_result(cls, rom_result_json: str) -> typing.Dict[str, typing.Dict[str, str]]:
        """
        利用rom_analyzer.py的分析结果，重组成
        {file_base_name: {"subsystem_name":subsystem_name, "component_name":component_name}}
        的形式
        """
        with open(rom_result_json, 'r', encoding='utf-8') as f:
            rom_info_dict = json.load(f)
        elf_info_dict: typing.Dict[str, typing.Dict[str, str]] = dict()
        for subsystem_name in rom_info_dict.keys():
            sub_val_dict: typing.Dict[str, typing.Any] = rom_info_dict.get(
                subsystem_name)
            delete_values_from_dict(sub_val_dict, ["size", "file_count"])
            for component_name in sub_val_dict.keys():
                component_val_dict: typing.Dict[str, str] = sub_val_dict.get(
                    component_name)
                delete_values_from_dict(component_val_dict, [
                    "size", "file_count"])
                for file_name, size in component_val_dict.items():
                    file_basename: str = os.path.split(file_name)[-1]
                    elf_info_dict[file_basename] = {
                        "subsystem_name": subsystem_name,
                        "component_name": component_name,
                        "size": size
                    }

        return elf_info_dict

    @classmethod
    def get_process_so_relationship(cls, cfg_path: str, profile_path: str) -> typing.Dict[
        str, typing.List[str]]:
        """
        parse the relationship between process and elf file
        """
        # 从merged_sa里面收集
        process_elf_dict: typing.Dict[str, typing.List[str]] = dict()
        cfg_list = glob.glob(cfg_path + os.sep + "*.cfg", recursive=True)
        for cfg in cfg_list:
            if debug:
                print("parsing: ", cfg)
            try:
                cls.__parse_process_cfg(cfg, profile_path, process_elf_dict)
            except:
                print("parse '{}' failed".format(cfg))
            finally:
                ...
        return process_elf_dict

    @classmethod
    def find_elf_size_from_rom_result(cls, service_name: str, subsystem_name: str, component_name: str,
                                      evaluator: typing.Callable, rom_result_dict: typing.Dict[str, typing.Dict]) -> \
            typing.Tuple[
                bool, str, str, int]:
        """
        全局查找进程的相关elf文件
        subsystem_name与component_name可明确指定，或为*以遍历整个dict
        evaluator：评估elf文件的从phone下面开始的路径与service_name的关系，评判如何才是找到了
        returns: 是否查找到，elf文件名，部件名，size
        """
        subsystem_name_list = [
            subsystem_name] if subsystem_name != "*" else rom_result_dict.keys()
        for sn in subsystem_name_list:
            sub_val_dict = rom_result_dict.get(sn)
            component_name_list = [
                component_name] if component_name != '*' else sub_val_dict.keys()
            for cn in component_name_list:
                if cn == "size" or cn == "file_count":
                    continue
                component_val_dict: typing.Dict[str,
                int] = sub_val_dict.get(cn)
                for k, v in component_val_dict.items():
                    if k == "size" or k == "file_count":
                        continue
                    if not evaluator(service_name, k):
                        continue
                    return True, os.path.split(k)[-1], sn, cn, v
        return False, str(), str(), str(), int()

    @classmethod
    def add_baseline(self, refactored_result_dict: Dict, baseline_file: str) -> None:
        with open(baseline_file, 'r', encoding='utf-8') as f:
            baseline_dict = json.load(f)
        for subsystem_name, subsystem_info in refactored_result_dict.items():
            for component_name, component_info in subsystem_info.items():
                if component_name == "size":
                    continue
                if not baseline_dict.get(subsystem_name):
                    continue
                if not baseline_dict[subsystem_name].get(component_name):
                    continue
                component_info["baseline"] = baseline_dict[subsystem_name][component_name].get(
                    "ram")

    @classmethod
    def inside_refactored_result_unit_adaptive(cls, process_info):
        for elf_name, elf_size in process_info["elf"].items():
            process_info["elf"][elf_name] = unit_adaptive(elf_size)
        return process_info

    @classmethod
    def refactored_result_unit_adaptive(cls, result_dict: Dict[str, Dict]) -> None:
        for subsystem_name, subsystem_info in result_dict.items():
            sub_size = unit_adaptive(subsystem_info["size"])
            del subsystem_info["size"]
            for component_name, component_info in subsystem_info.items():
                com_size = unit_adaptive(component_info["size"])
                del component_info["size"]
                for process_name, process_info in component_info.items():
                    pro_size = unit_adaptive(process_info["size"])
                    del process_info["size"]
                    process_info = cls.inside_refactored_result_unit_adaptive(process_info)
                    process_info["size"] = pro_size
                component_info["size"] = com_size
            subsystem_info["size"] = sub_size

    @classmethod
    def result_process1(cls, result_dict, process_name, process_size, elf, size):
        result_dict[process_name] = dict()
        result_dict[process_name]["size"] = process_size
        result_dict[process_name]["startup"] = dict()
        result_dict[process_name]["startup"]["init"] = dict()
        result_dict[process_name]["startup"]["init"][elf if len(
            elf) != 0 else "UNKNOWN"] = size
        return result_dict

    @classmethod
    def result_process2(cls, result_dict, process_name, subsystem_name, process_size, component_name, hap_name, size):
        result_dict[process_name] = dict()
        result_dict[process_name]["size"] = process_size
        result_dict[process_name][subsystem_name] = dict()
        result_dict[process_name][subsystem_name][component_name] = dict()
        result_dict[process_name][subsystem_name][component_name][hap_name if len(
            hap_name) != 0 else "UNKNOWN"] = size
        return result_dict

    @classmethod
    def result_process3(cls, result_dict, process_name, process_size):
        result_dict[process_name] = dict()
        result_dict[process_name]["size"] = process_size
        result_dict[process_name]["UNKNOWN"] = dict()
        result_dict[process_name]["UNKNOWN"]["UNKNOWN"] = dict()
        result_dict[process_name]["UNKNOWN"]["UNKNOWN"]["UNKNOWN"] = int()
        return result_dict

    @classmethod
    def result_process4(cls, result_dict, process_size_dict, rom_result_dict, process_elf_dict, so_info_dict):
        def get(key: typing.Any, dt: typing.Dict[str, typing.Any]):
            for k, v in dt.items():
                if k.startswith(key) or (len(v) > 0 and key == v[0]):
                    # 要么uinput_inject的对应key为mmi_uinput_inject。对于此类特殊处理，即：如果service_name找不到，但是直接执行的bin等于这个名字，也认为找到
                    return v

        for process_name, process_size in process_size_dict.items():  # 从进程出发
            if not process_name:
                print("warning: an empty 'process_name' has been found.")
                continue
            # 如果部件是init,特殊处理
            if process_name == "init":
                _, elf, _, _, size = cls.find_elf_size_from_rom_result(process_name, "startup", "init",
                                                                       lambda x, y: os.path.split(y)[
                                                                                        -1].lower() == x.lower(),
                                                                       rom_result_dict)
                result_dict = cls.result_process1(result_dict, process_name, process_size, elf, size)
                continue
            # 如果是hap，特殊处理
            if (process_name.startswith("com.") or process_name.startswith("ohos.")):
                _, hap_name, subsystem_name, component_name, size = cls.find_elf_size_from_rom_result(process_name, "*",
                                                                                                      "*",
                                                                                                      lambda x, y: len(
                                                                                                          y.split(
                                                                                                              '/')) >= 3 and x.lower().startswith(
                                                                                                          y.split('/')[
                                                                                                              2].lower()),
                                                                                                      rom_result_dict)
                result_dict = cls.result_process2(result_dict, process_name, subsystem_name, process_size,
                                                  component_name, hap_name, size)
                continue
            # 得到进程相关的elf文件list
            so_list: list = get(process_name, process_elf_dict)
            if so_list is None:
                print("warning: process '{}' not found in .json or .cfg".format(
                    process_name))
                result_dict = cls.result_process3(result_dict, process_name, process_size)
                continue
            result_dict[process_name] = dict()
            result_dict[process_name]["size"] = process_size
            for so in so_list:
                unit = so_info_dict.get(so)
                if unit is None:
                    result_dict[process_name]["UNKNOWN"] = dict()
                    result_dict[process_name]["UNKNOWN"]["UNKNOWN"] = dict()
                    result_dict[process_name]["UNKNOWN"]["UNKNOWN"][so] = int()
                    print("warning: '{}' in {} not found in json from rom analysis result".format(
                        so, process_name))
                    continue
                component_name = unit.get("component_name")
                subsystem_name = unit.get("subsystem_name")
                so_size = unit.get("size")
                if result_dict.get(process_name).get(subsystem_name) is None:
                    result_dict[process_name][subsystem_name] = dict()
                if result_dict.get(process_name).get(subsystem_name).get(component_name) is None:
                    result_dict[process_name][subsystem_name][component_name] = dict()
                result_dict[process_name][subsystem_name][component_name][so] = so_size
        return result_dict

    @classmethod
    def __hidumper_mem_line_process(cls, content: typing.Text) -> typing.List[typing.Text]:
        """
        将hidumper的拥有的数据行进行分割，得到
        [pid, name, pss, vss, rss, uss]格式的list
        """
        trival_pattern = re.compile(r"kB|\(.*\)(?#去除单位kB以及小括号内的任意数据，包括小括号)")
        content = re.sub(trival_pattern, "", content)
        blank_pattern = re.compile(r"\s+(?#匹配一个或多个空格)")
        return re.sub(blank_pattern, ' ', content.strip()).split()

    @classmethod
    def __parse_hidumper_mem(cls, content: typing.Text, device_num: str, ss: str = "Pss") -> typing.Dict[
        typing.Text, int]:
        """
        解析：hidumper --meme的结果
        返回{process_name: pss}形式的字典
        '248  	samgr              1464(0 in SwapPss) kB    15064 kB     6928 kB     1072 kB\r'
        """

        def find_full_process_name(hname: str) -> str:
            for lname in __process_name_list:
                if lname.startswith(hname):
                    return lname
            return str()

        def process_ps_ef(content: str) -> list:
            line_list = content.strip().split("\n")[1:]
            process_name_list = list()
            for line in line_list:
                process_name = line.split()[7]
                if process_name.startswith('['):
                    continue
                process_name_list.append(process_name)
            return process_name_list

        if ss not in cls.__ss_dict.keys():
            print("error: {} is not a valid parameter".format(ss))
            return dict()
        output = content.split('\n')
        process_pss_dict = dict()
        __process_name_list: typing.List[str] = process_ps_ef(
            HDCTool.exec(["hdc", "-t", device_num, "shell", "ps", "-ef"]))
        for line in output:
            if "Total Memory Usage by Size" in line:
                break
            if line.isspace():
                continue
            processed: typing.List[typing.Text] = cls.__hidumper_mem_line_process(
                line)
            # 如果第一列不是数字（pid），就过
            if not processed or not processed[0].isnumeric():
                continue
            name = processed[1]  # 否则的话就取名字，和对应的size
            size = int(processed[cls.__ss_dict.get(ss)]) * \
                   1024  # kilo byte to byte
            full_process_name = find_full_process_name(name)
            if not full_process_name:
                print(
                    f"warning: process \"{full_process_name}\" not found in the result of command \"ps -ef\"")
                continue
            process_pss_dict[full_process_name] = size
        return process_pss_dict

    @classmethod
    def __parse_process_json(cls, file_path: str, result_dict: typing.Dict[str, typing.List[str]]):
        """
        解析json文件，结存存入 result_dict中，格式：{process_name: os_list}
        其中，so_list中是so的base_name
        """
        if not (os.path.isfile(file_path) and file_path.endswith(".json")):
            print("warning: {} not exist or not a json file".format(file_path))
            return
        with open(file_path, 'r', encoding='utf-8') as f:
            j_content: typing.Dict[str, typing.Any] = json.load(f)
        if "process" not in j_content.keys() or "systemability" not in j_content.keys():
            print(
                f"warning: {file_path} has no field 'process' or 'systemability'")
            return
        process_name: str = j_content.get("process")
        elf_list: typing.List[str] = list()
        for sa in j_content.get("systemability"):
            libpath: str = sa.get("libpath")
            if not libpath:
                continue
            elf_list.append(libpath)
        result_dict[process_name] = elf_list

    @classmethod
    def __parse_process_cfg(cls, cfg_path: str, profile_path: str, result_dict: dict):
        """
        解析cfg，因为有的cfg会拉起xml中的进程，所以也可能会去解析xml
        """
        with open(cfg_path, 'r', encoding='utf-8') as f:
            cfg_dict = json.loads(f.read())
        services = cfg_dict.get("services")
        if services is None:
            print("warning: 'services' not in {}".format(cfg_path))
            return
        for service in services:
            process_name = service.get("name")
            first, *path_list = service.get("path")
            if first.endswith("sa_main"):
                # 由sa_main去来起进程
                xml_base_name = os.path.split(path_list[0])[-1]
                cls.__parse_process_json(os.path.join(
                    profile_path, xml_base_name), result_dict)
            else:
                # 直接执行
                if result_dict.get(process_name) is None:
                    result_dict[process_name] = list()
                result_dict.get(process_name).append(os.path.split(first)[-1])

    @classmethod
    def __inside_save_result_as_excel(cls, baseline_file, subsystem_name, component_name, component_size,
                                      component_baseline, process_name, process_size, elf_name, elf_size):
        if baseline_file:
            return [subsystem_name, component_name, component_size,
                    component_baseline, process_name, process_size, elf_name, elf_size]
        else:
            return [subsystem_name, component_name, component_size,
                    process_name, process_size, elf_name, elf_size]

    @classmethod
    def __save_result_as_excel(cls, data_dict: dict, filename: str, ss: str, baseline_file: str, unit_adapt: bool):
        """
        保存结果到excel中
        子系统：{
            "size": 1234,
            部件：{
                "size":123,
                "base_line":124,
                进程：{
                    "size":12,
                    "elf":{
                        "elf_file_1":elf_size,
                        ...
                    }
                }
            }
        }
        """
        tmp_dict = copy.deepcopy(data_dict)
        writer = SimpleExcelWriter("ram_info")
        header_unit = "" if unit_adapt else ", Byte"
        header = [
            "subsystem_name", "component_name", f"component_size(ram{header_unit})", "process_name",
            f"process_size({ss}{header_unit})", "elf", f"elf_size{'' if unit_adapt else '(Byte)'}"
        ]
        if baseline_file:
            header = [
                "subsystem_name", "component_name", f"component_size(ram{header_unit})", "baseline", "process_name",
                f"process_size({ss}{header_unit})", "elf", f"elf_size{'' if unit_adapt else '(Byte)'}"
            ]
        writer.set_sheet_header(header)
        subsystem_c = 0
        subsystem_start_r = 1
        subsystem_end_r = 0

        component_c = 1
        component_start_r = 1
        component_end_r = 0
        component_size_c = 2
        baseline_c = 3

        process_start_r = 1
        process_end_r = 0
        process_c = 4
        process_size_c = 5
        if not baseline_file:
            process_c -= 1
            process_size_c -= 1
        for subsystem_name, subsystem_info in tmp_dict.items():
            subsystem_size = subsystem_info.get("size")
            if subsystem_size:
                del subsystem_info["size"]
            for component_name, component_info in subsystem_info.items():
                component_size = component_info.get("size")
                component_baseline = component_info.get("baseline")
                if "size" in component_info.keys():
                    del component_info["size"]
                if "baseline" in component_info.keys():
                    del component_info["baseline"]
                for process_name, process_info in component_info.items():
                    process_size = process_info.get("size")
                    elf_info = process_info.get("elf")
                    for elf_name, elf_size in elf_info.items():
                        line = cls.__inside_save_result_as_excel(baseline_file, subsystem_name, component_name,
                                                                 component_size,
                                                                 component_baseline, process_name, process_size,
                                                                 elf_name, elf_size)
                        writer.append_line(line)
                    elf_count = len(elf_info)
                    process_end_r += elf_count
                    component_end_r += elf_count
                    subsystem_end_r += elf_count
                    writer.write_merge(
                        process_start_r, process_c, process_end_r, process_c, process_name)
                    writer.write_merge(
                        process_start_r, process_size_c, process_end_r, process_size_c, process_size)
                    process_start_r = process_end_r + 1
                writer.write_merge(component_start_r, component_c,
                                   component_end_r, component_c, component_name)
                writer.write_merge(component_start_r, component_size_c,
                                   component_end_r, component_size_c, component_size)
                if baseline_file:
                    writer.write_merge(component_start_r, baseline_c,
                                       component_end_r, baseline_c, component_baseline)
                component_start_r = component_end_r + 1
            writer.write_merge(subsystem_start_r, subsystem_c,
                               subsystem_end_r, subsystem_c, subsystem_name)
            subsystem_start_r = subsystem_end_r + 1
        writer.save(filename)


def inside_refacotr_result(component_info, refactored_ram_dict, subsystem_name, component_name, process_name,
                           process_size):
    for elf_name, elf_size in component_info.items():
        if not refactored_ram_dict.get(subsystem_name):
            refactored_ram_dict[subsystem_name] = dict()
            refactored_ram_dict[subsystem_name]["size"] = 0
        if not refactored_ram_dict[subsystem_name].get(component_name):
            refactored_ram_dict[subsystem_name][component_name] = dict(
            )
            refactored_ram_dict[subsystem_name][component_name]["size"] = 0
        refactored_ram_dict[subsystem_name][component_name][process_name] = dict(
        )
        refactored_ram_dict[subsystem_name][component_name][process_name]["size"] = process_size
        refactored_ram_dict[subsystem_name][component_name][process_name]["elf"] = dict(
        )
        refactored_ram_dict[subsystem_name][component_name][process_name]["elf"][elf_name] = elf_size
        refactored_ram_dict[subsystem_name]["size"] += process_size
        refactored_ram_dict[subsystem_name][component_name]["size"] += process_size
    return refactored_ram_dict


def refacotr_result(ram_result: Dict[str, Dict]) -> Dict[str, Dict]:
    refactored_ram_dict: Dict[str, Dict] = dict()
    for process_name, process_info in ram_result.items():
        process_size = process_info.get("size")
        del process_info["size"]
        for subsystem_name, subsystem_info in process_info.items():
            for component_name, component_info in subsystem_info.items():
                refactored_ram_dict = inside_refacotr_result(component_info, refactored_ram_dict, subsystem_name,
                                                             component_name, process_name, process_size)
    return refactored_ram_dict


def get_args():
    version_num = 1.0
    parser = argparse.ArgumentParser(
        description="analyze ram size of component"
    )
    parser.add_argument("-v", "-version", action="version",
                        version=f"version {version_num}")
    parser.add_argument("-s", "--json_path", type=str, required=True,
                        help="path of sa json file. eg: -x ~/openharmony/out/rk3568/packages/phone/system/profile")
    parser.add_argument("-c", "--cfg_path", type=str, required=True,
                        help="path of cfg files. eg: -c ./cfgs/")
    parser.add_argument("-j", "--rom_result", type=str, default="./rom_analysis_result.json",
                        help="json file produced by rom_analyzer_v1.0.py, default: ./rom_analysis_result.json."
                             "eg: -j ./demo/rom_analysis_result.json")
    parser.add_argument("-n", "--device_num", type=str, required=True,
                        help="device number to be collect hidumper info. eg: -n 7001005458323933328a01fce16d3800")
    parser.add_argument("-b", "--baseline_file", type=str, default="",
                        help="baseline file of rom and ram generated by rom analysis.")
    parser.add_argument("-o", "--output_filename", default="ram_analysis_result", type=str,
                        help="base name of output file, default: ram_analysis_result. eg: -o ram_analysis_result")
    parser.add_argument("-u", "--unit_adaptive",
                        action="store_true", help="unit adaptive")
    parser.add_argument("-e", "--excel", type=bool, default=False,
                        help="if output result as excel, default: False. eg: -e True")
    args = parser.parse_args()
    return args


def abspath(path: str) -> str:
    return os.path.abspath(os.path.expanduser(path))


if __name__ == '__main__':
    args = get_args()
    cfg_path_name = abspath(args.cfg_path)
    profile_path_name = abspath(args.json_path)
    rom_result = args.rom_result
    device = args.device_num
    output_filename = args.output_filename
    baseline = args.baseline_file
    output_excel_path = args.excel
    unit_adaptiv = args.unit_adaptive
    RamAnalyzer.analysis(cfg_path_name, profile_path_name, rom_result,
                         device_num=device, output_file=output_filename, ss="Pss", output_excel=output_excel_path,
                         baseline_file=baseline, unit_adapt=unit_adaptiv)