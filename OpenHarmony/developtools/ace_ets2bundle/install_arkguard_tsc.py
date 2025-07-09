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

import json
import os
import sys
import subprocess
import shutil
import tarfile


def extract(package_path, dest_path, package_name, current_os):
    dest_package_path = os.path.join(dest_path, package_name)
    temp_package_path = os.path.join(dest_path, current_os + package_name)
    if (os.path.exists(dest_package_path) or os.path.exists(temp_package_path)):
        return
    os.makedirs(temp_package_path, exist_ok=True)
    try:
        with tarfile.open(package_path, 'r:gz') as tar:
            tar.extractall(path=temp_package_path)
    except tarfile.TarError as e:
        print(f'Error extracting files: {e}')
    package_path = os.path.join(temp_package_path, 'package')
    if not (os.path.exists(dest_package_path)):
        # The default name of the decompressed npm package is package. it needs to be renamed to the specified name.
        shutil.copytree(package_path, dest_package_path, symlinks=True, dirs_exist_ok=True)
    run_cmd(['rm', '-rf', temp_package_path])


def run_cmd(cmd, execution_ath=None):
    proc = subprocess.Popen(cmd, stdout=subprocess.PIPE,
                           stdin=subprocess.PIPE,
                           stderr=subprocess.PIPE,
                           cwd=execution_ath)
    stdout, stderr = proc.communicate(timeout=60)
    if proc.returncode != 0:
        raise Exception(stderr.decode())


def run(args):
    tsc_path = args[0]
    arkguard_path = args[1]
    source_path = args[2]
    current_os = args[3]
    node_modules_path = os.path.join(source_path, "node_modules")
    extract(tsc_path, node_modules_path, 'typescript', current_os)
    extract(arkguard_path, node_modules_path, 'arkguard', current_os)


if __name__ == "__main__":
    run(sys.argv[1:])