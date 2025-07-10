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


import os
import yaml


def print_to_yaml(yaml_path: str, data_yaml: dict) -> None:

    with os.fdopen(os.open(yaml_path, os.O_WRONLY | os.O_CREAT | os.O_TRUNC, mode=511), 'w', encoding="utf-8") as file:
        yaml.dump(data_yaml, file)
