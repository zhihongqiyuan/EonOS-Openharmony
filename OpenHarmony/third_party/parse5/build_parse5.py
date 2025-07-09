#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Copyright (c) 2020 Huawei Device Co., Ltd.
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
import sys
import subprocess
import argparse

standard_system_build_dir = os.path.join(os.path.dirname(__file__), os.pardir,
    os.pardir, 'build', 'scripts', 'util')
build_dir = os.path.join(os.path.dirname(__file__), os.pardir, os.pardir,
    os.pardir, os.pardir, os.pardir, 'build', 'maple', 'java', 'util')
if os.path.exists(standard_system_build_dir):
    sys.path.append(
        os.path.join(standard_system_build_dir, os.pardir, os.pardir))
    from scripts.util import build_utils  # noqa: E402
if os.path.exists(build_dir):
    sys.path.append(os.path.join(build_dir, os.pardir, os.pardir, os.pardir))
    from maple.java.util import build_utils  # noqa: E402


def parse_args():

    parser = argparse.ArgumentParser()
    build_utils.add_depfile_option(parser)

    parser.add_argument('--node', help='path to nodejs exetuable')
    parser.add_argument('--tsc-js', help='path to parse5 module tsc')
    parser.add_argument('--parse5-project', help='path to parse5 project')
    parser.add_argument('--parse5-output-dir', help='path to parse5 output')
    parser.add_argument('--uglify-source-js', help='path uglify-source.js')

    options = parser.parse_args()
    return options


def do_build(build_cmd, uglify_cmd):
    for cmd in [build_cmd, uglify_cmd]:
        build_utils.check_output(cmd)


def main():
    options = parse_args()

    build_cmd = [
        options.node, options.tsc_js
    ]
    build_cmd.extend(['--project', options.parse5_project])
    build_cmd.extend(['--outDir', options.parse5_output_dir])
    build_cmd.extend(['--module', 'CommonJS'])
    build_cmd.extend(['--target', 'ES6'])
    depfile_deps = [options.node, options.tsc_js]
    depfile_deps.extend(build_utils.get_all_files(options.parse5_project))

    uglify_cmd = [options.node, options.uglify_source_js, options.parse5_output_dir]
    depfile_deps.append(options.uglify_source_js)

    build_utils.call_and_write_depfile_if_stale(
        lambda: do_build(build_cmd, uglify_cmd),
        options,
        depfile_deps=depfile_deps,
        input_paths=depfile_deps,
        output_paths=([options.parse5_output_dir]))


if __name__ == '__main__':
    sys.exit(main())
