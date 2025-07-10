#!/usr/bin/env python3
# coding=utf-8
#
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


"""This module provides structures, to save some info while parsing headers."""

import os
from typing import Any, Dict
from file_tools import print_to_yaml

statistics: Dict[str, Dict[str, Any]] = {}
custom_yamls: Dict[str, Dict[str, Any]] = {}
LIB_GEN_FOLDER = ""


def init_collections(lib_gen_folder: str) -> None:  # pylint: disable=C
    global statistics, custom_yamls, LIB_GEN_FOLDER  # pylint: disable=W
    LIB_GEN_FOLDER = lib_gen_folder

    statistics = {
        "unreachable": {
            "log_file": os.path.join(LIB_GEN_FOLDER, "./gen/logs/unreachable.txt"),
            "collection": set(),
        },
        "skip": {
            "log_file": os.path.join(LIB_GEN_FOLDER, "./gen/logs/skip.txt"),
            "collection": set()
        },
        "generated_yamls": {
            "log_file": os.path.join(LIB_GEN_FOLDER, "./gen/logs/generated_yamls.txt"),
            "collection": set(),
        },
    }

    custom_yamls = {
        "allEnums": {
            "yaml_file": os.path.join(LIB_GEN_FOLDER, "./gen/headers/allEnums.yaml"),
            "collection": {"enums": []},
        },
        "pathsToHeaders": {
            "yaml_file": os.path.join(LIB_GEN_FOLDER, "./gen/headers/pathsToHeaders.yaml"),
            "collection": {"paths": []},
        },
    }


def add_to_statistics(key: str, data: Any) -> None:
    if isinstance(statistics[key]["collection"], set): # CC-OFF(G.TYP.07) dict key exist
        statistics[key]["collection"].add(data) # CC-OFF(G.TYP.07) dict key exist
    elif isinstance(statistics[key]["collection"], list): # CC-OFF(G.TYP.07) dict key exist
        statistics[key]["collection"].append(data) # CC-OFF(G.TYP.07) dict key exist
    else:
        raise RuntimeError("Unreachable")


def add_to_custom_yamls(yaml_name: str, key: str, data: Any) -> None:
    custom_yamls[yaml_name]["collection"][key].append(data) # CC-OFF(G.TYP.07) dict key exist


def save_custom_yamls() -> None:
    headers_path = os.path.join(LIB_GEN_FOLDER, "./gen/headers")
    if not os.path.exists(headers_path):
        os.makedirs(headers_path)

    for _, value in custom_yamls.items():
        yaml_file = value["yaml_file"]
        print_to_yaml(yaml_file, value["collection"])

        statistics["generated_yamls"]["collection"].add( # CC-OFF(G.TYP.07) dict key exist
            os.path.basename(yaml_file)
        )


def save_statistics() -> None:
    logs_path = os.path.join(LIB_GEN_FOLDER, "./gen/logs")
    if not os.path.exists(logs_path):
        os.makedirs(logs_path)

    for _, value in statistics.items():
        with os.fdopen(os.open(value["log_file"], os.O_WRONLY | os.O_CREAT, mode=511), "w", encoding="utf-8") as f:
            for item in value["collection"]:
                f.write(f"{item}\n")
