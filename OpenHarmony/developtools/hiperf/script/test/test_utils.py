#!/usr/bin/env python
# -*- coding: utf-8 -*-
#   Copyright (c) 2021 Huawei Device Co., Ltd.
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.

import os
import unittest
from hiperf_utils import get_lib
from hiperf_utils import remove
from hiperf_utils import is_elf_file
from hiperf_utils import get_hiperf_binary_path
from hiperf_utils import str_to_bytes
from hiperf_utils import bytes_to_str
from hiperf_utils import executable_file_available
from hiperf_utils import find_tool_path


class TestUtils(unittest.TestCase):

    def setUp(self):
        print("set up")

    def tearDown(self):
        print("tear down")

    def test_get_lib(self):
        lib = get_lib()
        result = True if lib else False
        self.assertEqual(result, True)

    def test_is_elf_file(self):
        result = is_elf_file('libhiperf_report.z.so')
        self.assertEqual(result, True)

    def test_remove(self):
        os.mkdir('remove_test')
        remove('remove_test')
        result = True if not os.path.exists('remove_test') else False
        self.assertEqual(result, True)

    def test_get_hiperf_binary_path(self):
        result = get_hiperf_binary_path('arm', 'hiperf')
        print(result)
        self.assertEqual(result, 'D:\\tool_third\perf_data\\'
                                 'new_report\OHOS\\arm\hiperf')

    def test_str_to_bytes(self):
        result = str_to_bytes('test')
        self.assertEqual(result, b'test')

    def test_bytes_to_str(self):
        result = bytes_to_str(b'test')
        self.assertEqual(result, 'test')

    def test_executable_file_available(self):
        result = executable_file_available('hdc')
        self.assertEqual(result, True)

    def test_find_tool_path(self):
        result = find_tool_path('hdc')
        self.assertEqual(result, 'hdc')
