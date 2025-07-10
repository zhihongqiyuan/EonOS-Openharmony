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

import unittest
from unittest.mock import patch, MagicMock
import os
import json
from src.validate_readme_opensource import OpenSourceValidator, REQUIRED_FIELDS


class TestOpenSourceValidator(unittest.TestCase):

    @patch("os.walk")
    def test_find_all_readmes(self, mock_os_walk):
        # 模拟 os.walk 返回值
        mock_os_walk.return_value = [
            ("/project", ["subdir1", "subdir2"], ["README.OpenSource"]),
            ("/project/subdir1", [], ["README.OpenSource"]),
            ("/project/subdir2", [], ["README.OpenSource"]),
        ]

        validator = OpenSourceValidator(project_root="/project")
        readme_paths = validator.find_all_readmes()

        # 断言所有的 README.OpenSource 文件都被正确地找到
        self.assertEqual(readme_paths, [
            "/project/README.OpenSource",
            "/project/subdir1/README.OpenSource",
            "/project/subdir2/README.OpenSource"
        ])

    @patch("builtins.open", new_callable=MagicMock)
    def test_validate_format_valid(self, mock_open):
        # 模拟文件内容是一个包含正确格式的 JSON 数据
        mock_open.return_value.__enter__.return_value.read.return_value = json.dumps([
            {
                "Name": "Software A",
                "License": "MIT",
                "License File": "LICENSE",
                "Version Number": "1.0.0",
                "Owner": "Owner A",
                "Upstream URL": "https://example.com",
                "Description": "A software project",
                "Dependencies": []
            }
        ])

        validator = OpenSourceValidator(project_root="/project")
        valid = validator.validate_format("/project/README.OpenSource")

        # 断言格式验证通过
        self.assertTrue(valid)

    @patch("builtins.open", new_callable=MagicMock)
    def test_validate_format_invalid_missing_field(self, mock_open):
        # 模拟文件内容是一个包含缺失字段的 JSON 数据
        mock_open.return_value.__enter__.return_value.read.return_value = json.dumps([
            {
                "Name": "Software A",
                "License": "MIT",
                "License File": "LICENSE",
                "Version Number": "1.0.0",
                "Owner": "Owner A",
                "Upstream URL": "https://example.com",
                # "Description" 字段缺失
                "Dependencies": []
            }
        ])

        validator = OpenSourceValidator(project_root="/project")
        valid = validator.validate_format("/project/README.OpenSource")

        # 断言格式验证失败
        self.assertFalse(valid)

    @patch("builtins.open", new_callable=MagicMock)
    def test_validate_content_valid(self, mock_open):
        # 模拟读取到的 README 文件数据
        mock_open.return_value.__enter__.return_value.read.return_value = json.dumps([
            {
                "Name": "Software A",
                "License": "MIT",
                "License File": "LICENSE",
                "Version Number": "1.0.0",
                "Owner": "Owner A",
                "Upstream URL": "https://example.com",
                "Description": "A software project",
                "Dependencies": []
            }
        ])

        # 模拟参考数据
        reference_data = [
            {
                "Name": "Software A",
                "License": "MIT",
                "License File": "LICENSE",
                "Version Number": "1.0.0",
                "Owner": "Owner A",
                "Upstream URL": "https://example.com",
                "Description": "A software project"
            }
        ]

        validator = OpenSourceValidator(project_root="/project")
        validator.reference_data = reference_data  # 设置参考数据

        valid = validator.validate_content("/project/README.OpenSource")

        # 断言内容验证通过
        self.assertTrue(valid)

    @patch("builtins.open", new_callable=MagicMock)
    @patch("os.path.exists", return_value=True)  # 模拟许可证文件存在
    def test_validate_content_valid(self, mock_exists, mock_open):
        # 模拟读取到的 README 文件数据
        mock_open.return_value.__enter__.return_value.read.return_value = json.dumps([
            {
                "Name": "Software A",
                "License": "MIT",
                "License File": "LICENSE",  # 许可证文件
                "Version Number": "1.0.0",
                "Owner": "Owner A",
                "Upstream URL": "https://example.com",
                "Description": "A software project",
                "Dependencies": []
            }
        ])

        # 模拟参考数据
        reference_data = [
            {
                "Name": "Software A",
                "License": "MIT",
                "License File": "LICENSE",
                "Version Number": "1.0.0",
                "Owner": "Owner A",
                "Upstream URL": "https://example.com",
                "Description": "A software project"
            }
        ]

        validator = OpenSourceValidator(project_root="/project")
        validator.reference_data = reference_data  # 设置参考数据

        valid = validator.validate_content("/project/README.OpenSource")

        # 断言内容验证通过
        self.assertTrue(valid)

    @patch("os.path.exists")
    def test_validate_license_file_valid(self, mock_exists):
        # 模拟文件存在
        mock_exists.return_value = True

        validator = OpenSourceValidator(project_root="/project")
        valid = validator.validate_license_file("/project/README.OpenSource", "LICENSE")

        # 断言许可证文件存在并且校验通过
        self.assertTrue(valid)

    @patch("os.path.exists")
    def test_validate_license_file_invalid(self, mock_exists):
        # 模拟文件不存在
        mock_exists.return_value = False

        validator = OpenSourceValidator(project_root="/project")
        valid = validator.validate_license_file("/project/README.OpenSource", "LICENSE")

        # 断言许可证文件不存在并且校验失败
        self.assertFalse(valid)

    @patch("builtins.open", new_callable=MagicMock)
    @patch("os.path.exists")
    def test_validate_dependencies_valid(self, mock_exists, mock_open):
        # 模拟文件内容是有效的 JSON 数据
        mock_open.return_value.__enter__.return_value.read.return_value = json.dumps([
            {
                "Name": "Software A",
                "License": "MIT",
                "License File": "LICENSE",
                "Version Number": "1.0.0",
                "Owner": "Owner A",
                "Upstream URL": "https://example.com",
                "Description": "A software project",
                "Dependencies": ["dep1", "dep2"]
            }
        ])
        # 模拟依赖文件存在
        mock_exists.return_value = True

        validator = OpenSourceValidator(project_root="/project")
        valid = validator.validate_dependencies(["dep1", "dep2"], "/project/README.OpenSource")

        # 断言依赖项验证通过
        self.assertTrue(valid)

    @patch("builtins.open", new_callable=MagicMock)
    def test_validate_dependencies_invalid(self, mock_open):
        # 模拟文件内容是包含非法依赖项格式的 JSON 数据
        mock_open.return_value.__enter__.return_value.read.return_value = json.dumps([
            {
                "Name": "Software A",
                "License": "MIT",
                "License File": "LICENSE",
                "Version Number": "1.0.0",
                "Owner": "Owner A",
                "Upstream URL": "https://example.com",
                "Description": "A software project",
                "Dependencies": ["dep1", 123]  # 非字符串依赖项
            }
        ])

        validator = OpenSourceValidator(project_root="/project")
        valid = validator.validate_dependencies(["dep1", 123], "/project/README.OpenSource")

        # 断言依赖项验证失败
        self.assertFalse(valid)


if __name__ == "__main__":
    unittest.main()

