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

import pytest
import os
import json
from unittest.mock import patch
import sys

sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '../src')))
from generate_readme_opensource import (
    ask_question,
    ask_for_list,
    process_license_info,
    generate_readme_opensource
)


@pytest.fixture
def temp_output_dir(tmp_path):
    """创建临时输出目录"""
    return str(tmp_path)


def test_ask_question():
    """测试ask_question函数"""
    with patch('builtins.input', return_value='test_value'):
        assert ask_question("prompt", "default") == "test_value"

    with patch('builtins.input', return_value=''):
        assert ask_question("prompt", "default") == "default"


def test_ask_for_list():
    """测试ask_for_list函数"""
    with patch('builtins.input', return_value='item1, item2, item3'):
        result = ask_for_list("prompt")
        assert result == ['item1', 'item2', 'item3']

    with patch('builtins.input', return_value=''):
        result = ask_for_list("prompt")
        assert result == []


def test_process_license_info_single():
    """测试处理单个许可证信息"""
    with patch('builtins.input', side_effect=['MIT', 'LICENSE']):
        licenses, files = process_license_info()
        assert licenses == ['MIT']
        assert files == ['LICENSE']


def test_process_license_info_multiple():
    """测试处理多个许可证信息"""
    with patch('builtins.input', side_effect=['MIT; Apache-2.0', 'LICENSE.mit; LICENSE.apache']):
        licenses, files = process_license_info()
        assert licenses == ['MIT', 'Apache-2.0']
        assert files == ['LICENSE.mit', 'LICENSE.apache']


def test_process_license_info_one_license_multiple_files():
    """测试一个许可证对应多个文件的情况"""
    with patch('builtins.input', side_effect=['MIT', 'LICENSE.txt; COPYING.txt']):
        licenses, files = process_license_info()
        assert licenses == ['MIT']
        assert files == ['LICENSE.txt', 'COPYING.txt']


def test_process_license_info_multiple_licenses_one_file():
    """测试多个许可证对应一个文件的情况"""
    with patch('builtins.input', side_effect=['MIT; Apache-2.0', 'LICENSE']):
        licenses, files = process_license_info()
        assert licenses == ['MIT', 'Apache-2.0']
        assert files == ['LICENSE']


def test_process_license_info_error():
    """测试许可证信息不匹配的错误情况"""
    # 测试三个许可证对应两个文件的情况，这应该会引发错误
    with patch('builtins.input', side_effect=['MIT; Apache-2.0; GPL', 'LICENSE.mit; LICENSE.apache']):
        with pytest.raises(ValueError) as exc_info:
            process_license_info()
        assert "许可证和许可证文件的数量不匹配" in str(exc_info.value)


def test_generate_readme_opensource(temp_output_dir):
    """测试生成README.OpenSource文件"""
    # 模拟用户输入
    input_values = [
        'TestComponent',  # Name
        '1.0.0',  # Version
        'Test Owner',  # Owner
        'https://example.com',  # URL
        'Test Description',  # Description
        'MIT',  # License
        'LICENSE',  # License File
        'dep1, dep2',  # Dependencies
        'n'  # Don't add more components
    ]

    with patch('builtins.input', side_effect=input_values):
        generate_readme_opensource(temp_output_dir)

        # 验证文件是否创建
        readme_path = os.path.join(temp_output_dir, 'README.OpenSource')
        assert os.path.exists(readme_path)

        # 验证文件内容
        with open(readme_path, 'r', encoding='utf-8') as f:
            content = json.load(f)
            assert len(content) == 1
            component = content[0]
            assert component['Name'] == 'TestComponent'
            assert component['Version Number'] == '1.0.0'
            assert component['Owner'] == 'Test Owner'
            assert component['Upstream URL'] == 'https://example.com'
            assert component['Description'] == 'Test Description'
            assert component['License'] == 'MIT'
            assert component['License File'] == 'LICENSE'
            assert component['Dependencies'] == ['dep1', 'dep2']


def test_generate_readme_opensource_multiple_components(temp_output_dir):
    """测试生成包含多个组件的README.OpenSource文件"""
    input_values = [
        # 第一个组件
        'Component1',
        '1.0.0',
        'Owner1',
        'https://example1.com',
        'Description1',
        'MIT',
        'LICENSE1',
        '',  # 无依赖项
        'y',  # 添加另一个组件
        # 第二个组件
        'Component2',
        '2.0.0',
        'Owner2',
        'https://example2.com',
        'Description2',
        'Apache-2.0',
        'LICENSE2',
        'dep1',
        'n'  # 不再添加组件
    ]

    with patch('builtins.input', side_effect=input_values):
        generate_readme_opensource(temp_output_dir)

        # 验证文件是否创建
        readme_path = os.path.join(temp_output_dir, 'README.OpenSource')
        assert os.path.exists(readme_path)

        # 验证文件内容
        with open(readme_path, 'r', encoding='utf-8') as f:
            content = json.load(f)
            assert len(content) == 2

            # 验证第一个组件
            assert content[0]['Name'] == 'Component1'
            assert content[0]['Version Number'] == '1.0.0'
            assert 'Dependencies' not in content[0]

            # 验证第二个组件
            assert content[1]['Name'] == 'Component2'
            assert content[1]['Version Number'] == '2.0.0'
            assert content[1]['Dependencies'] == ['dep1']