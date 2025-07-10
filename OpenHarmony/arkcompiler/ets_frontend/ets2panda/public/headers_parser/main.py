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


"""
Usage: python3 main.py <path_to_ets2panda> <path_to_gen_dir> <path_to_header_or_dir> [<path_to_header_or_dir> ...]
Description: Parse c++ headers to yaml files.
"""

import os.path
import traceback
from arg_parser import parse_arguments, check_arguments

# pylint: disable=W0401,W0614
from cpp_parser import CppParser
from log_tools import init_log, error_log, parsing_failed_msg, warning_log
from prepare_header import remove_comments, extract_and_remove_includes
from file_tools import print_to_yaml
from runtime_collections import (
    init_collections,
    add_to_statistics,
    add_to_custom_yamls,
    save_custom_yamls,
    save_statistics,
)


def parse_header(src_path: str, dest_path: str) -> None:
    """
    Parse one .h file to .yaml.
    """
    with open(src_path, "r", encoding="utf-8") as file:
        data = file.read()
        data = remove_comments(data)
        data, includes = extract_and_remove_includes(data)

        try:
            res = CppParser(data).parse()

            if len(includes) != 0:
                res["include"] = includes

            if not os.path.exists(os.path.dirname(dest_path)):
                os.makedirs(os.path.dirname(dest_path))

            print_to_yaml(dest_path, res)

            # Collect statistics
            add_to_statistics("generated_yamls", dest_path)
            # Collect custom yamls
            add_to_custom_yamls("pathsToHeaders", "paths", src_path)

        except Exception:  # pylint: disable=W0718
            os.fdopen(os.open(dest_path, os.O_CREAT, mode=511), "w", encoding="utf-8").close()

            warning_log(f"Can't parse '{src_path}'")
            error_log(f"Error! Can't parse '{src_path}'\n{traceback.format_exc()}\n")
            parsing_failed_msg(src_path)


if __name__ == "__main__":
    args = check_arguments(parse_arguments())

    init_collections(args.lib_gen_dir)
    init_log(args.lib_gen_dir)

    result_dir = os.path.join(args.lib_gen_dir, "gen/headers")
    os.makedirs(result_dir, exist_ok=True)

    for header in args.headers:
        dst = os.path.join(result_dir, f"{os.path.splitext(os.path.relpath(header, args.es2panda_root))[0]}.yaml")
        parse_header(header, dst)

    save_custom_yamls()
    save_statistics()
