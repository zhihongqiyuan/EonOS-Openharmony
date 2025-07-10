#!/usr/bin/env python
# -*- coding: utf-8 -*-
#   Copyright (c) 2021 Huawei Device Co., Ltd.
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.

import sys
import os.path
from ctypes import *
from hiperf_utils import get_lib

libhiperf_report = get_lib()

if libhiperf_report is not None:
    libhiperf_report.EchoLoopBack.restype = c_char_p
    libhiperf_report.Report.restype = c_bool
    libhiperf_report.Dump.restype = c_bool
    libhiperf_report.ReportGetSymbolFiles.restype = c_char_p
    libhiperf_report.ReportGetSymbolFiles.argtypes = [c_char_p]
    libhiperf_report.ReportGetBuildId.restype = c_char_p
    libhiperf_report.ReportGetElfArch.restype = c_char_p

    ret = libhiperf_report.EchoLoopBack(b'123')
    sys.stdout.flush()
    print("\nfrom Python EchoLoopBack: ", ret)

    ret = libhiperf_report.EchoLoopBack(b'test')
    sys.stdout.flush()
    print("\nfrom Python EchoLoopBack: ", ret)

    ret = libhiperf_report.EchoLoopBack(b'test123')
    sys.stdout.flush()
    print("\nfrom Python EchoLoopBack: ", ret)

    ret = libhiperf_report.EchoLoopBack(b'')
    sys.stdout.flush()
    print("\nfrom Python EchoLoopBack: ", ret)

    ret = libhiperf_report.EchoLoopBack(None)
    sys.stdout.flush()
    print("\nfrom Python EchoLoopBack: ", ret)

    ret = libhiperf_report.Report(b'perf.data', b'report.txt', b'-s')
    print("\nfrom Python Report: ", ret)

    ret = libhiperf_report.SetDebug(True)
    print("\nfrom Python SetDebug: ", ret)

    ret = libhiperf_report.ReportJson(b'perf.data', b'json.txt')
    print("\nfrom Python ReportJson: ", ret)

    ret = libhiperf_report.ReportGetSymbolFiles(b'perf.data')
    print("\nfrom Python ReportGetSymbolFiles: ", ret)

    ret = libhiperf_report.ReportGetBuildId(b'elf32_test')
    print("\nfrom Python ReportGetBuildId: ", ret)

    ret = libhiperf_report.ReportGetBuildId(b'elf32_test_stripped_nobuildid')
    print("\nfrom Python ReportGetBuildId: ", ret)

    ret = libhiperf_report.ReportGetElfArch(b'elf32_test')
    print("\nfrom Python ReportGetElfArch: ", ret)

    ret = libhiperf_report.ReportGetElfArch(b'elf_test')
    print("\nfrom Python ReportGetElfArch: ", ret)

    ret = libhiperf_report.ReportGetElfArch(b'hiperf')
    print("\nfrom Python ReportGetElfArch: ", ret)
