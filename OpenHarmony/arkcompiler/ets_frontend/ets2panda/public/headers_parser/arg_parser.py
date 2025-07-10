#!/usr/bin/env python3
# coding: utf-8
# Copyright (c) 2024 Huawei Device Co., Ltd.
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


import argparse
import os


def parse_arguments() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Cpp headers parser to .yaml")

    parser.add_argument(
        "--lib-gen-dir",
        "-d",
        type=str,
        required=True,
        help="Path to es2panda_lib generation directory (aka <build>/tools/es2panda/es2panda_lib)",
    )
    parser.add_argument(
        "--es2panda-root",
        "-e",
        type=str,
        required=True,
        help="Path to es2panda_root (aka <runtime_core>/static_core/tools/es2panda)",
    )
    parser.add_argument(
        "--headers",
        "-l",
        type=str,
        required=True,
        nargs="+",
        help="List of headers to be parsed. Full paths, via <runtime_core>/static_core/tools/es2panda",
    )

    return parser.parse_args()


def check_arguments(args: argparse.Namespace) -> argparse.Namespace:
    if not os.path.isdir(args.lib_gen_dir):
        raise RuntimeError(f"Bad path to lib_gen_dir: '{args.lib_gen_dir}'\nSee --help for more.")
    if not os.path.isdir(args.es2panda_root):
        raise RuntimeError(f"Bad path to es2panda_root: '{args.es2panda_root}'\nSee --help for more.")
    for header in args.headers:
        if not os.path.isfile(header) or os.path.splitext(header)[1] != ".h":
            raise RuntimeError(f"Bad path to header: '{header}'. See --help for more.")

    return args
