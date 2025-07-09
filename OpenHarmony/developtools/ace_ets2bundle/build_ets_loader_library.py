#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Copyright (c) 2021 Huawei Device Co., Ltd.
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
    parser.add_argument('--ets-loader-src-dir', help='path to compiler/src')
    parser.add_argument('--babel-config-js', help='path babel.config.js')
    parser.add_argument('--uglify-source-js', help='path uglify-source.js')
    parser.add_argument('--output-dir', help='path to output')
    parser.add_argument('--copy-json-files-js', help='path copy_json_files.js')
    parser.add_argument('--copy-src-dir', help='path to copy src dir')
    parser.add_argument('--copy-dest-dir', help='path to copy dest dir')
    parser.add_argument('--declarations-file-dir',
        help='path declarations file')
    parser.add_argument('--build-declarations-file-js',
        help='path build_declarations_file.js')
    parser.add_argument('--output-declarations-dir',
        help='path declarations file to output')
    parser.add_argument('--output-component-config-file',
        help='path component config file to output')
    parser.add_argument('--output-form-config-file',
        help='path form config file to output')
    parser.add_argument('--output-build-config-file',
        help='path form config file to output')
    parser.add_argument('--kit-configs-file-dir',
        help='path kit configs file')
    parser.add_argument('--build-kit-configs-file-js',
        help='path build_kit_configs_file.js')
    parser.add_argument('--output-kit-configs-dir',
        help='path kit configs file to output')
    parser.add_argument('--kit-apis-file-dir',
        help='path apis file')
    parser.add_argument('--arkts-apis-file-dir',
        help='path apis file')
    parser.add_argument('--build-public-sdk',
        help='build public sdk or not')

    options = parser.parse_args()
    return options


def do_build(build_cmd, uglify_cmd, copy_json_cmd, build_declarations_file_cmd, build_kit_configs_file_cmd):
    for cmd in [build_cmd, uglify_cmd, copy_json_cmd, build_declarations_file_cmd, build_kit_configs_file_cmd]:
        build_utils.check_output(cmd)


def main():
    options = parse_args()
    build_cmd = [
        options.node, options.babel_js, options.ets_loader_src_dir
    ]
    build_cmd.extend(['--out-dir', options.output_dir])
    build_cmd.extend(['--extensions', '.ts'])
    build_cmd.extend(['--config-file', options.babel_config_js])
    depfile_deps = [options.node, options.babel_js, options.babel_config_js]
    depfile_deps.extend(build_utils.get_all_files(options.ets_loader_src_dir))

    uglify_cmd = [options.node, options.uglify_source_js, options.output_dir]
    depfile_deps.append(options.uglify_source_js)

    copy_json_cmd = [options.node, options.copy_json_files_js, options.copy_src_dir, options.copy_dest_dir]
    depfile_deps.append(options.copy_json_files_js)

    build_declarations_file_cmd = [
        options.node,
        options.build_declarations_file_js,
        options.declarations_file_dir,
        options.output_declarations_dir,
        options.output_dir,
        options.build_public_sdk
    ]
    depfile_deps.append(options.build_declarations_file_js)

    build_kit_configs_file_cmd = [
        options.node,
        options.build_kit_configs_file_js,
        options.kit_configs_file_dir,
        options.output_kit_configs_dir,
        options.kit_apis_file_dir,
        options.arkts_apis_file_dir,
        options.output_dir
    ]
    depfile_deps.append(options.build_kit_configs_file_js)

    build_utils.call_and_write_depfile_if_stale(
        lambda: do_build(build_cmd, uglify_cmd, copy_json_cmd, build_declarations_file_cmd, build_kit_configs_file_cmd),
        options,
        depfile_deps=depfile_deps,
        input_paths=depfile_deps,
        output_paths=([
            options.output_dir,
            options.output_declarations_dir,
            options.output_component_config_file,
            options.output_form_config_file,
            options.output_build_config_file,
            options.output_kit_configs_dir]))

if __name__ == '__main__':
    sys.exit(main())
