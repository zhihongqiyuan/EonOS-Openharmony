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

import subprocess
import sys


def main():
    cmd = sys.argv[1:]
    print(f'cmd {cmd} start')
    result = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    print(f'result.returncode = {result.returncode}')
    print(f'result.stdout = {result.stdout.decode("utf-8", errors="ignore")}')
    if (result.returncode != 0):
        raise Exception(f'cmd {cmd} failed')
    else:
        print(f'cmd {cmd} ok')

if __name__ == '__main__':
    sys.exit(main())
