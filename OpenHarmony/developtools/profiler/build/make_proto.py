#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
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
import shutil


def remove_lite_option(target_path, protofilepath):
    target_file_path = os.path.basename(protofilepath)
    target_file_path = target_path + '/' + target_file_path.replace(".proto", "_standard.proto")
    shutil.copyfile(protofilepath, target_file_path)
    # replease lite flag, import file name, add package name
    with open(target_file_path, 'r+') as content:
        newcontent = content.read()
        newcontent = newcontent.replace('option optimize_for = LITE_RUNTIME;\n', '')
        newcontent = newcontent.replace('syntax = "proto3";', 'syntax = "proto3";\npackage ForStandard;')
        newcontent = newcontent.replace('.proto";', '_standard.proto";')
        content.seek(0, 0)
        content.write(newcontent)
        content.truncate()
        content.close()


def add_lite_option(target_path, protofilepath):
    target_file_path = os.path.basename(protofilepath)
    target_file_path = target_path + '/' + target_file_path.replace(".proto", "_lite.proto")
    shutil.copyfile(protofilepath, target_file_path)
    # replease lite flag, import file name, add package name
    with open(target_file_path, 'r+') as content:
        newcontent = content.read()
        if newcontent.find('option optimize_for = LITE_RUNTIME') == -1:
            newcontent = newcontent.replace('syntax = "proto3";',
                                            'syntax = "proto3";\n\noption optimize_for = LITE_RUNTIME;')
        newcontent = newcontent.replace('syntax = "proto3";', 'syntax = "proto3";\n\npackage LITE;')
        newcontent = newcontent.replace('.proto";', '_lite.proto";')
        content.seek(0, 0)
        content.write(newcontent)
        content.truncate()
        content.close()


def main():
    target_dir = sys.argv[1]
    i = 2
    while i < len(sys.argv):
        proto_file_path = sys.argv[i]
        remove_lite_option(target_dir, proto_file_path)
        add_lite_option(target_dir, proto_file_path)
        i += 1


if __name__ == '__main__':
    main()
