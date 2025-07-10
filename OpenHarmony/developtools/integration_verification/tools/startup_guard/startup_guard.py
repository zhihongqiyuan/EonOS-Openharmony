#!/usr/bin/env python
#coding=utf-8

#
# Copyright (c) 2023-2024 Huawei Device Co., Ltd.
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

from config_parser_mgr import ConfigParserMgr


def __create_arg_parser():
    import argparse
    parser = argparse.ArgumentParser(description='Check startup architecture information from compiled output files.')
    parser.add_argument('-i', '--input',
                        help='input config files base directory example "out/rk3568" ', required=True)
    parser.add_argument('-c', '--target_cpu',
                    help='target_cpu cpu type" ', required=True)
    parser.add_argument('-r', '--rules', action='append',
                        help='rules directory', required=False)
    parser.add_argument('-n', '--no_fail',
                        help='force to pass all rules', required=False)
    return parser

def startup_guard(out_path, target_cpu ,args=None):
    mgr = ConfigParserMgr()
    mgr.load_all_parser(out_path, target_cpu)

    from startup_checker import check_all_rules
    passed = check_all_rules(mgr, args)
    if passed:
        print("All rules passed")
    else:
        print("Please modify according to README.md")

if __name__ == '__main__':
    parser = __create_arg_parser()
    args = parser.parse_args()
    startup_guard(args.input, args.target_cpu, args)
