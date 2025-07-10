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
import argparse
import logging
from typing import List, Dict, Optional

REQUIRED_FIELDS = [
    "Name",
    "License",
    "License File",
    "Version Number",
    "Owner",
    "Upstream URL",
    "Description"
]


class OpenSourceValidator:
    def __init__(
            self,
            project_root: str,
            log_file: Optional[str] = None,
            reference_data: Optional[List[Dict[str, str]]] = None
    ):
        self.project_root = project_root
        self.reference_data = reference_data or []
        self.log_file = log_file

        # 设置日志配置
        if self.log_file:
            logging.basicConfig(
                filename=self.log_file,
                level=logging.INFO,
                format="%(asctime)s - %(levelname)s - %(message)s",
            )
        else:
            logging.basicConfig(
                level=logging.INFO,
                format="%(asctime)s - %(levelname)s - %(message)s",
            )

    def find_all_readmes(self) -> List[str]:
        """递归查找所有 README.OpenSource 文件"""
        readme_paths = []
        for dirpath, _, filenames in os.walk(self.project_root):
            if "README.OpenSource" in filenames:
                readme_paths.append(os.path.join(dirpath, "README.OpenSource"))
        return readme_paths

    def validate_format(self, readme_path: str) -> bool:
        """验证 README.OpenSource 文件的格式和必需字段"""
        errors = []
        try:
            with open(readme_path, 'r', encoding='utf-8') as f:
                data = json.load(f)
            if not isinstance(data, list):
                errors.append("The file does not contain a JSON array.")
                return False
            for idx, component in enumerate(data):
                for field in REQUIRED_FIELDS:
                    if field not in component:
                        errors.append(f"Component {idx + 1} is missing required field: {field}")

                # 校验 Dependencies 字段是否存在并且是一个数组
                if "Dependencies" in component:
                    if not isinstance(component["Dependencies"], list):
                        errors.append(f"Component {idx + 1} 'Dependencies' field must be an array.")
                    else:
                        for dep in component["Dependencies"]:
                            if not isinstance(dep, str):
                                errors.append(f"Component {idx + 1} 'Dependencies' contains a non-string value: {dep}")

        except json.JSONDecodeError as e:
            errors.append(f"JSON decode error: {e}")
            return False
        except Exception as e:
            errors.append(f"Unexpected error: {e}")
            return False

        if errors:
            for error in errors:
                logging.error(f"{readme_path}: {error}")
            return False
        else:
            logging.info(f"{readme_path} format is valid.")
            return True

    def load_reference_data(self, reference_data_path: str):
        """从 JSON 配置文件中加载参考数据"""
        try:
            with open(reference_data_path, "r", encoding='utf-8') as f:
                self.reference_data = json.load(f)
        except Exception as e:
            raise ValueError(
                f"Failed to load reference data from '{reference_data_path}': {e}"
            )

    def find_reference_data(self, name: str) -> Optional[Dict[str, str]]:
        """在参考数据中根据名称查找对应的开源软件信息"""
        for reference in self.reference_data:
            if reference.get("Name") == name:
                return reference
        return None

    def validate_content(self, readme_path: str) -> bool:
        """校验 README.OpenSource 文件的内容，并与参考数据进行比对"""
        # Step 1: 读取 JSON 文件
        try:
            with open(readme_path, "r", encoding='utf-8') as f:
                readme_data = json.load(f)
            if not isinstance(readme_data, list):
                logging.error(f"{readme_path}: JSON data is not an array.")
                return False
        except json.JSONDecodeError as e:
            logging.error(f"{readme_path}: JSON decode error: {e}")
            return False

        # Step 2: 校验 JSON 数组中的每个开源软件元数据
        all_valid = True
        for software_data in readme_data:
            name = software_data.get("Name")
            if not name:
                logging.error(f"{readme_path}: Missing 'Name' field in software data.")
                all_valid = False
                continue

            reference_data = self.find_reference_data(name)

            if reference_data is None:
                logging.error(
                    f"{readme_path}: Software '{name}' not found in reference data."
                )
                all_valid = False
                continue

            # 比对 "Name", "License", "Version Number", "Upstream URL"
            for field in ["Name", "License", "Version Number", "Upstream URL"]:
                expected_value = reference_data.get(field)
                actual_value = software_data.get(field)
                if actual_value != expected_value:
                    logging.error(
                        f"{readme_path}: Field '{field}' mismatch for '{name}'. Expected: '{expected_value}', Found: '{actual_value}'"
                    )
                    all_valid = False

            # 校验 "License File" 路径是否存在
            if not self.validate_license_file(readme_path, software_data.get("License File")):
                all_valid = False

            # 校验依赖项（Dependencies）是否正确
            if not self.validate_dependencies(software_data.get("Dependencies"), readme_path):
                all_valid = False

        if all_valid:
            logging.info(f"{readme_path}: Content validation passed.")
        else:
            logging.error(f"{readme_path}: Content validation failed.")
        return all_valid

    def validate_license_file(self, readme_path: str, license_file: str) -> bool:
        """校验 LICENSE 文件是否存在，路径相对于 README.OpenSource 文件所在目录"""
        if not license_file:
            logging.error(f"{readme_path}: 'License File' field is missing.")
            return False

        # 支持多个许可证文件路径，以分号分隔
        license_files = license_file.split(';')
        readme_dir = os.path.dirname(readme_path)
        all_valid = True

        for file in license_files:
            license_file_path = os.path.join(readme_dir, file.strip())
            if not os.path.exists(license_file_path):
                logging.error(
                    f"{readme_path}: License file '{file.strip()}' not found at: {license_file_path}"
                )
                all_valid = False
            else:
                logging.info(f"{readme_path}: License file '{file.strip()}' exists.")

        return all_valid

    def validate_dependencies(self, dependencies: Optional[List[str]], readme_path: str) -> bool:
        """校验 Dependencies 字段是否符合预期"""
        if dependencies is None:
            return True  # 没有依赖项是合法的

        if not isinstance(dependencies, list):
            logging.error(f"{readme_path}: 'Dependencies' should be an array.")
            return False

        for dep in dependencies:
            if not isinstance(dep, str):
                logging.error(f"{readme_path}: 'Dependencies' contains non-string value: {dep}")
                return False

        logging.info(f"{readme_path}: 'Dependencies' field is valid.")
        return True

    def run_validation(self, validate_format: bool = True, validate_content: bool = False):
        """运行完整的校验流程，递归处理所有 README.OpenSource 文件"""
        try:
            readme_paths = self.find_all_readmes()
            if not readme_paths:
                logging.error("No README.OpenSource files found in the project directory.")
                return

            for readme_path in readme_paths:
                logging.info(f"Validating: {readme_path}")
                if validate_format:
                    if not self.validate_format(readme_path):
                        logging.error(f"{readme_path}: Format validation failed.")
                        continue  # 如果格式验证失败，跳过内容验证
                if validate_content:
                    if not self.validate_content(readme_path):
                        logging.error(f"{readme_path}: Content validation failed.")

            logging.info("Validation process completed.")

        except Exception as e:
            logging.error(f"Validation failed: {e}")


def main():
    parser = argparse.ArgumentParser(
        description="Validate README.OpenSource files in a project."
    )
    parser.add_argument("project_root", help="The root directory of the project.")
    parser.add_argument(
        "--validate-format", action='store_true', help="Validate the format of README.OpenSource files."
    )
    parser.add_argument(
        "--validate-content", action='store_true', help="Validate the content of README.OpenSource files against reference data."
    )
    parser.add_argument(
        "--reference-data", help="Path to the reference data JSON file (required for content validation)."
    )
    parser.add_argument("--log-file", help="Path to the log file for validation results.")

    args = parser.parse_args()

    if args.validate_content and not args.reference_data:
        parser.error("--reference-data is required for content validation.")

    # 初始化验证器对象
    validator = OpenSourceValidator(
        project_root=args.project_root,
        log_file=args.log_file
    )

    if args.validate_content:
        # 从配置文件中加载参考数据
        validator.load_reference_data(args.reference_data)

    # 执行校验流程
    validator.run_validation(
        validate_format=args.validate_format,
        validate_content=args.validate_content
    )


if __name__ == "__main__":
    main()
