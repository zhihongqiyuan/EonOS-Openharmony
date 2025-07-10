#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# Copyright (c) 2023 Huawei Device Co., Ltd.
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
import shutil

import yaml
    
if __name__ == "__main__":
    with open(r".\config.yaml", 'r') as f:
        data = yaml.safe_load(f.read())

    path = data['path_xts_report']
    file_list = os.listdir(path)

    summary_report = os.path.join(path, file_list[-1], "summary_report.html")
    if (os.path.exists(summary_report)):
        shutil.copy2(summary_report, "result\\")
    
    details_report = os.path.join(path, file_list[-1], "details_report.html")
    if (os.path.exists(details_report)):
        shutil.copy2(details_report, "result\\")
        
    failures_report = os.path.join(path, file_list[-1], "failures_report.html")
    if (os.path.exists(failures_report)):
        shutil.copy2(failures_report, "result\\")
