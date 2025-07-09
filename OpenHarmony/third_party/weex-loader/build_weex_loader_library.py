#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Copyright (c) 2023 Huawei Device Co., Ltd.
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
    parser.add_argument('--babel-js', help='path to babel.js')
    parser.add_argument("--weex-loader-src-dir",
                        help='path to weex-loader/src')
    parser.add_argument('--babel-config-js', help='path babel.config.js')
    parser.add_argument('--module-source-js', help='path module-source.js')
    parser.add_argument('--uglify-source-js', help='path uglify-source.js')
    parser.add_argument('--output-dir', help='path to output')

    options = parser.parse_args()
    return options


def do_build(build_cmd, copy_cmd, uglify_cmd):
    for cmd in [build_cmd, copy_cmd, uglify_cmd]:
        build_utils.check_output(cmd)


def main():
    options = parse_args()
    build_cmd = [
        options.node, options.babel_js, options.weex_loader_src_dir
    ]
    build_cmd.extend(['--out-dir', options.output_dir])
    build_cmd.extend(['--config-file', options.babel_config_js])
    depfile_deps = [options.node, options.babel_js, options.babel_config_js]
    depfile_deps.extend(build_utils.get_all_files(options.weex_loader_src_dir))

    copy_cmd = [options.node, options.module_source_js, options.output_dir]
    depfile_deps.append(options.module_source_js)

    uglify_cmd = [options.node, options.uglify_source_js, options.output_dir]
    depfile_deps.append(options.uglify_source_js)

    build_utils.call_and_write_depfile_if_stale(
        lambda: do_build(build_cmd, copy_cmd, uglify_cmd),
        options,
        depfile_deps=depfile_deps,
        input_paths=depfile_deps,
        output_paths=([options.output_dir]))


if __name__ == '__main__':
    sys.exit(main())
