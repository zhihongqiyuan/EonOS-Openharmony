#!/usr/bin/env python
# -*- coding: utf-8 -*-
#   Copyright (c) 2025 Huawei Device Co., Ltd.
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
#
import pytest
import argparse
import os
import shutil

from hiperf_utils import HdcInterface, get_build_id
from hiperf_utils import dir_check
from hiperf_utils import file_check
from hiperf_utils import get_lib
from hiperf_utils import get_arg_list

ABS_PATH = os.path.split(os.path.realpath(__file__))[0]


class GetLibFiles(object):
    """Collect all binaries needed by perf.data in binary_cache."""

    def __init__(self):
        self.local_cache_dir = os.path.join(ABS_PATH, 'binary_cache')
        if not os.path.isdir(self.local_cache_dir):
            os.makedirs(self.local_cache_dir)
        self.binary_map = {}

    def recv_binary_cache(self, perf_data, lib_dirs, copy):
        self.get_used_binaries(perf_data)
        self.copy_binaries_from_lib_dirs(lib_dirs)
        self.hdc = HdcInterface()
        if copy:
            self.recv_binaries_from_device()
        self.recv_kernel_symbols()

    def get_used_binaries(self, perf_data):
        # A dict mapping from binary name to build_id
        lib_hiperf_report = get_lib()
        lib_hiperf_report.ReportGetSymbolFiles.restype = c_char_p
        lib_hiperf_report.ReportGetSymbolFiles.argtypes = [c_char_p]
        ret = lib_hiperf_report.ReportGetSymbolFiles(perf_data.encode("utf-8"))

        dso_build_id = str(ret, encoding="utf-8", errors='ignore')
        dso_list = dso_build_id.split('],[')
        if dso_list is None or len(dso_list) == 0:
            return
        dso_list[0] = dso_list[0][1:]
        dso_list[-1] = dso_list[-1][0:-1]
        dso_dict = {}
        for i in dso_list:
            group = i.split(',')
            if len(group) == 2:
                dso_dict[group[0]] = group[1]
            else:
                dso_dict[group[0]] = None
        self.binary_map = dso_dict

    # open source
    def copy_binaries_from_lib_dirs(self, lib_dirs):
        """collect all files in lib_dirs."""
        if not lib_dirs:
            return

        # key is binary name , value is path list
        file_dict = {}
        self.get_local_bin_map(file_dict)

        for lib_dir in lib_dirs:
            for root, _, files in os.walk(lib_dir):
                self.confirm_copy_file(root, files, file_dict)

    def get_local_bin_map(self, file_dict):
        for bin_file in self.binary_map:
            # if path is /system/lib/libhi_irq.so , get libhi_irq.so
            filename = bin_file[bin_file.rfind('/') + 1:]
            if file_dict.get(filename) is None:
                file_dict[filename] = [bin_file]
            else:
                file_dict[filename].append(bin_file)

    def confirm_copy_file(self, root, files, file_dict):
        for filename in files:
            paths = file_dict.get(filename)
            if not paths:
                continue

            build_id = get_build_id(os.path.join(root, filename))
            if not build_id:
                continue

            for bin_file in paths:
                req_build_id = self.binary_map.get(bin_file)
                if req_build_id == build_id:
                    self.copy_to_binary_cache(
                        os.path.join(root, filename), bin_file)

    def copy_to_binary_cache(self, from_path, target_file):
        if target_file[0] == '/':
            target_file = target_file[1:]

        target_file = target_file.replace('/', os.sep)
        target_file = os.path.join(self.local_cache_dir, target_file)
        target_dir = os.path.dirname(target_file)

        if not os.path.isdir(target_dir):
            os.makedirs(target_dir)
        print('copy to binary_cache: %s to %s' % (from_path, target_file))
        shutil.copy(from_path, target_file)

    def recv_binaries_from_device(self):
        """pull binaries needed in perf.data to binary_cache."""
        for binary in self.binary_map:
            # [kernel.kallsyms] or something can't find binary.
            if not binary.startswith('/') or \
                    binary.startswith("/dev/" or binary == "//anon"):
                continue
            # fit all platform
            binary_cache_file = binary[1:].replace('/', os.sep)
            local_cache_file = os.path.join(self.local_cache_dir,
                                            binary_cache_file)
            self.check_and_recv_binary(binary, local_cache_file)

    def check_and_recv_binary(self, binary, local_cache_file):
        """If the binary_cache_file exists and has the expected_build_id, there
           is no need to pull the binary from device. Otherwise, pull it.
        """
        req_build_id = self.binary_map[binary]
        need_pull = True
        # compare with build id adjust is match file
        if os.path.isfile(local_cache_file):
            need_pull = False
            build_id = get_build_id(local_cache_file)
            if req_build_id != build_id:
                print('local file build id is %s is not request build %s'
                      % (build_id, req_build_id))
                need_pull = True

        if need_pull:
            target_dir = os.path.dirname(local_cache_file)
            if not os.path.isdir(target_dir):
                os.makedirs(target_dir)
            if os.path.isfile(local_cache_file):
                os.remove(local_cache_file)
            self.pull_file_from_device(binary, local_cache_file)
            print('recv file to binary_cache: %s to %s' % (binary,
                                                           local_cache_file))
            self.confirm_del_file(binary, local_cache_file)
        else:
            print('not need recv, use host file in binary_cache: %s' %
                  local_cache_file)

    def pull_file_from_device(self, device_path, host_path):
        if self.hdc.run_hdc_cmd(['file recv', device_path, host_path]):
            return True
        # In non-root device, we can't pull /data/app/XXX/base.odex directly.
        # Instead, we can first copy the file to /data/local/tmp, then pull it.
        filename = device_path[device_path.rfind('/') + 1:]
        if (self.hdc.run_hdc_cmd(['shell', 'cp', device_path,
                                  '/data/local/tmp']) and
                self.hdc.run_hdc_cmd(['file recv',
                                      os.path.join('/data/local/tmp/', filename),
                                      host_path])):
            self.hdc.run_hdc_cmd(['shell', 'rm',
                                  os.path.join('/data/local/tmp/', filename)])
            return True
        print('failed to pull %s from device' % device_path)
        return False

    def confirm_del_file(self, device_path, host_path):
        build_id = get_build_id(os.path.join(host_path))
        if not build_id:
            return
        req_build_id = self.binary_map.get(device_path)
        if not req_build_id == build_id:
            print('recv file %s build id %s not match request build id %s,'
                  ' delete it' % (host_path, build_id, req_build_id))
            os.remove(host_path)


    def recv_kernel_symbols(self):
        file_path = os.path.join(self.local_cache_dir, 'kallsyms')
        if os.path.isfile(file_path):
            os.remove(file_path)
        if self.hdc.switch_root():
            old_kptr_restrict = os.popen('hdc shell cat /proc/sys/kernel/kptr_restrict').read()
            self.hdc.run_hdc_cmd(['shell',
                                  '"echo 0 >/proc/sys/kernel/kptr_restrict"'])
            self.hdc.run_hdc_cmd(['file recv', '/proc/kallsyms', file_path])
            self.hdc.run_hdc_cmd(['shell',
                                  '"echo ' + old_kptr_restrict[0] + ' >/proc/sys/kernel/kptr_restrict"'])


class TestRecvBinaryCache:
    def setup(self):
        print("TestRecvBinaryCache setup")

    @pytest.mark.L0
    def test_recv_binary_cache(self):
        if os.path.exists('./testModule/binary_cache/system/lib/libc++.so') == True:
            os.remove('./testModule/binary_cache/system/lib/libc++.so')
        parser = argparse.ArgumentParser(description=""" Recv binaries needed by
         perf.data from device to binary_cache directory.""")
        parser.add_argument('-i', '--perf_data', default='perf.data',
                            type=file_check, help=""" The path of profiling 
                            data.""")
        parser.add_argument('-l', '--local_lib_dir', type=dir_check, nargs='+',
                            help="""Path to find debug version of local shared
                            libraries used in the app.""", action='append')
        parser.add_argument('-c', '--copy_symbol_from_device', default='0',
                            help=""" Copy symbol files from device.""")
        args = parser.parse_args()

        recver = GetLibFiles()
        lib_dirs = get_arg_list(args.local_lib_dir)
        recver.recv_binary_cache(args.perf_data, lib_dirs,
                                 args.copy_symbol_from_device == '0' or args.copy_symbol_from_device == 'False')
        assert os.path.exists('./testModule/binary_cache/system/lib64/libc++.so') == True
