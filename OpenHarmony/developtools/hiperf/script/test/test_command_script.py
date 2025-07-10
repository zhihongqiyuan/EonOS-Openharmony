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
import argparse
from command_script import HostElfEntry
from command_script import parser_add_argument
from command_script import check_args
from command_script import main


def fake_parser():
    parser = argparse.ArgumentParser()
    parser.add_argument('-p', '--pid')
    parser.add_argument('-app', '--package_name')
    parser.add_argument('-l', '--local_program')
    parser.add_argument('-c', '--cmd')
    parser.add_argument('-t', '--tid')
    parser.add_argument('-sw', '--system_wide', action='store_true')
    parser.add_argument('-a', '--activity')
    parser.add_argument('-n', '--not_hdc_root')
    parser.add_argument('-lib', '--local_lib_dir')
    parser.add_argument('-r', '--record_options', default='-d 1')
    parser.add_argument('-o', '--output_perf_data', default='perf.data')
    return parser


class TestCommandScript(unittest.TestCase):

    def setUp(self):
        print("set up")

    def tearDown(self):
        print("tear down")

    def test_host_elf_entry(self):
        result = HostElfEntry('test_path', 'test_lib_name').__str__()
        self.assertEqual(result, '[path: test_path, name test_lib_name,'
                                 ' score test_lib_score]')

    def test_parser_add_argument(self):
        self.parser = fake_parser()
        args = self.parser.parse_args(['-p', '14'])
        result = args.pid
        self.assertEqual(result, '14')

    def test_check_args(self):
        self.args = parser_add_argument()
        result = check_args(self.args)
        self.assertEqual(result, True)

    def test_main(self):
        self.parser = fake_parser()
        args = self.parser.parse_args(['-sw'])
        main(args)
        if os.path.exists('perf.data'):
            result = True
        else:
            result = False
        self.assertEqual(result, True)

