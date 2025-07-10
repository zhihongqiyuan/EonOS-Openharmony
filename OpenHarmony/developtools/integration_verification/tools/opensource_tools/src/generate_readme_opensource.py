#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Copyright (c) 2024 Huawei Device Co., Ltd.
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

import os
import json


def ask_question(prompt, default_value=None):
    """提示用户输入，若没有输入则使用默认值"""
    value = input(f"{prompt} [{default_value}]: ").strip()
    return value or default_value


def ask_for_list(prompt):
    """提示用户输入一个列表，以逗号分隔"""
    value = input(f"{prompt} (多个项请用逗号分隔): ").strip()
    return [item.strip() for item in value.split(",")] if value else []


def process_license_info():
    """处理许可证信息和对应的文件路径"""
    licenses = ask_question("请输入许可证名称（如有多个，用分号分隔）")
    license_files = ask_question("请输入许可证文件路径（如果有多个，请使用分号分隔）")

    license_list = (
        [license.strip() for license in licenses.split(";")] if licenses else []
    )
    license_file_list = (
        [file.strip() for file in license_files.split(";")] if license_files else []
    )

    # 检查输入是否为空
    if not license_list or not license_file_list:
        raise ValueError("许可证和许可证文件路径不能为空。")

    # 检查许可证和文件路径的匹配情况
    if len(license_list) != len(license_file_list):
        # 只有在以下两种特殊情况下允许不相等：
        # 1. 一个许可证对应多个文件
        # 2. 多个许可证对应一个文件
        if not (
            (len(license_list) == 1 and len(license_file_list) > 1)
            or (len(license_list) > 1 and len(license_file_list) == 1)
        ):
            raise ValueError(
                "许可证和许可证文件的数量不匹配，必须是一对一、一对多或多对一的关系。"
            )

    return license_list, license_file_list


def generate_readme_opensource(output_dir):
    """
    生成 README.OpenSource 文件，支持多个开源组件的信息输入。
    """
    components = []
    fields = [
        "Name",
        "License",
        "License File",
        "Version Number",
        "Owner",
        "Upstream URL",
        "Description",
        "Dependencies",
    ]

    print("请输入开源组件的信息（输入完成后，可选择继续添加另一个组件）：")
    while True:
        component = {}
        # 获取组件的基本信息
        component["Name"] = ask_question("Name: ")

        # 获取许可证信息
        license_list, license_file_list = process_license_info()
        component["License"] = "; ".join(license_list)
        component["License File"] = "; ".join(license_file_list)

        component["Version Number"] = ask_question("Version Number: ")
        component["Owner"] = ask_question("Owner: ")
        component["Upstream URL"] = ask_question("Upstream URL: ")
        component["Description"] = ask_question("Description: ")

        # 获取依赖信息（可选）
        dependencies = ask_for_list("请输入该软件的依赖项（如果有多个，请用逗号分隔）")
        if dependencies:
            component["Dependencies"] = dependencies

        # 将组件信息添加到列表
        components.append(component)

        # 是否继续添加组件
        add_more = ask_question("是否添加另一个组件？(y/n): ").lower()
        if add_more != "y":
            break

    # 确保输出目录存在
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    # 输出 README.OpenSource 文件
    readme_path = os.path.join(output_dir, "README.OpenSource")
    with open(readme_path, "w", encoding="utf-8") as f:
        json.dump(components, f, indent=2, ensure_ascii=False)
    print(f"已生成 {readme_path}")


def main():
    output_dir = ask_question("请输入输出目录（默认当前目录）：") or "."
    generate_readme_opensource(output_dir)


if __name__ == "__main__":
    main()
