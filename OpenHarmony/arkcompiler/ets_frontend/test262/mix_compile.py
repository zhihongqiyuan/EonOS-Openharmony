#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
Copyright (c) 2024 Huawei Device Co., Ltd.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

Description: generate abc with mix compilation
"""

import os
import stat
from utils import *


class MixCompiler:
    def __init__(self, out_file, files_info_list, opt_level, file_threads, 
                 abc_class_threads, function_threads, frontend_tool) -> None:
        self.out_file = out_file
        self.files_info_list = files_info_list
        self.opt_level = opt_level
        self.file_threads = file_threads
        self.abc_class_threads = abc_class_threads
        self.function_threads = function_threads
        self.frontend_tool = frontend_tool
        self.abc_outputs = []

    def mix_compile(self, step=1, size=1):
        flags = os.O_WRONLY | os.O_CREAT
        modes = stat.S_IWUSR | stat.S_IRUSR
        files_info_list = self.files_info_list
        out_file = self.out_file
        count = 0
        return_code = 0
        # For each testcase, we will firstly select some files to generate a merged abc,
        # and then mix compile it with the remained files to generate the final abc. Will do this for a cycle.
        for i in range(0, len(files_info_list), step):
            count = count + 1
            right_bound = min(i + size, len(files_info_list))
            selected_file = files_info_list[i : right_bound]
            remained_file = files_info_list[0 : i]
            remained_file.extend(files_info_list[right_bound : len(files_info_list)])

            temp_abc = out_file.replace(ABC_EXT, f'_{count}{TEMP_ABC_EXT}')
            temp_files_info_path = out_file.replace(ABC_EXT, f'_{count}{TEMP_TXT_EXT}')
            with os.fdopen(os.open(temp_files_info_path, flags, modes), 'w') as f:
                f.write("".join(selected_file))
            cmd = [self.frontend_tool, '--opt-level', str(self.opt_level),
                   '--file-threads', str(self.file_threads),
                   '--function-threads', str(self.function_threads),
                   '--output', temp_abc, '--merge-abc', f'@{temp_files_info_path}']
            return_code = exec_command(cmd)
            if return_code:
                print_command(cmd)
                return return_code

            remained_file.append(f'{temp_abc}\n')
            abc = out_file.replace(ABC_EXT, f'_{count}{ABC_EXT}')
            files_info_path = out_file.replace(ABC_EXT, f'_{count}{TXT_EXT}')
            with os.fdopen(os.open(files_info_path, flags, modes), 'w') as f:
                f.write("".join(remained_file))
            cmd = [self.frontend_tool, '--opt-level', str(self.opt_level),
                   '--file-threads', str(self.file_threads),
                   '--abc-class-threads', str(self.abc_class_threads),
                   '--function-threads', str(self.function_threads),
                   '--output', abc, '--merge-abc', '--enable-abc-input', f'@{files_info_path}']
            return_code = exec_command(cmd)
            if return_code:
                print_command(cmd)
                return return_code

            self.abc_outputs.append(abc)

        return return_code
