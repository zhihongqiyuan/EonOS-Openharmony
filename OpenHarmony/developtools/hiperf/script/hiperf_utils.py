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
import subprocess
import argparse
import time
import os
import shutil
import platform
from ctypes import c_char_p
from ctypes import cdll

IS_DEBUG = False
HDC_NAME = "hdc"
SYMBOL_FILES_DIR = '/data/local/tmp/local_libs/'
BUILD_ID_FILE = "build_id_list"
EXPECTED_TOOLS = {
    HDC_NAME: {
        'is_binutils': False,
        'test_option': 'version',
        'path_in_tool': '../platform-tools/hdc',
    }
}


def get_lib():
    script_dir = os.path.dirname(os.path.realpath(__file__))
    system_type = platform.system().lower()
    if system_type == "windows":
        lib_path = os.path.join(script_dir, "bin", system_type,
                                "x86_64", "libhiperf_report.dll")
    elif system_type == "linux":
        lib_path = os.path.join(script_dir, "bin", system_type,
                                "x86_64", "libhiperf_report.so")
    elif system_type == "darwin":
        lib_path = os.path.join(script_dir, "bin", system_type,
                                "x86_64", "libhiperf_report.dylib")
    else:
        print("Un Support System Platform")
        raise RuntimeError
    if not os.path.exists(lib_path):
        print("{} does not exist!".format(lib_path))
        raise RuntimeError
    return cdll.LoadLibrary(lib_path)


def remove(files):
    if os.path.isfile(files):
        os.remove(files)
    elif os.path.isdir(files):
        shutil.rmtree(files, ignore_errors=True)


def is_elf_file(path):
    if os.path.isfile(path):
        with open(path, 'rb') as file_bin:
            data = file_bin.read(4)
            if len(data) == 4 and bytes_to_str(data) == '\x7fELF':
                return True
    return False


def get_architecture(elf_path):
    if is_elf_file(elf_path):
        my_lib = get_lib()
        my_lib.ReportGetElfArch.restype = c_char_p
        ret = my_lib.ReportGetElfArch(elf_path.encode())
        return ret.decode()
    return 'unknown'


def get_build_id(elf_path):
    if is_elf_file(elf_path):
        my_lib = get_lib()
        my_lib.ReportGetBuildId.restype = c_char_p
        ret = my_lib.ReportGetBuildId(elf_path.encode())
        return ret.decode()
    return ''


def get_hiperf_binary_path(arch, binary_name):
    if arch == 'aarch64':
        arch = 'arm64'
    script_dir = os.path.dirname(os.path.realpath(__file__))
    arch_dir = os.path.join(script_dir, "bin", "ohos", arch)
    if not os.path.isdir(arch_dir):
        raise Exception("can't find arch directory: %s" % arch_dir)
    binary_path = os.path.join(arch_dir, binary_name)
    if not os.path.isfile(binary_path):
        raise Exception("can't find binary: %s" % binary_path)
    return binary_path


def str_to_bytes(str_value):
    if not (sys.version_info >= (3, 0)):
        return str_value
    return str_value.encode('utf-8')


def bytes_to_str(bytes_value):
    if not bytes_value:
        return ''
    if not (sys.version_info >= (3, 0)):
        return bytes_value
    return bytes_value.decode('utf-8')


def executable_file_available(executable, option='--help'):
    try:
        print([executable, option])
        subproc = subprocess.Popen([executable, option],
                                   stdout=subprocess.PIPE,
                                   stderr=subprocess.PIPE)
        subproc.communicate(timeout=5)
        return subproc.returncode == 0
    except OSError:
        return False


def dir_check(arg):
    path = os.path.realpath(arg)
    if not os.path.isdir(path):
        raise argparse.ArgumentTypeError('{} is not a directory.'.format(path))
    return path


def file_check(arg):
    path = os.path.realpath(arg)
    if not os.path.isfile(path):
        raise argparse.ArgumentTypeError('{} is not a file.'.format(path))
    return path


def get_arg_list(arg_list):
    res = []
    if arg_list:
        for arg in arg_list:
            res += arg
    return res


def get_arch(arch):
    if 'aarch64' in arch:
        return 'arm64'
    if 'arm' in arch:
        return 'arm'
    if 'x86_64' in arch or "amd64" in arch:
        return 'x86_64'
    if '86' in arch:
        return 'x86'
    raise Exception('unsupported architecture: %s' % arch.strip())


def find_tool_path(tool, tool_path=None):
    if tool not in EXPECTED_TOOLS:
        return None
    tool_info = EXPECTED_TOOLS[tool]
    test_option = tool_info.get('test_option', '--help')
    path_in_tool = tool_info['path_in_tool']
    path_in_tool = path_in_tool.replace('/', os.sep)

    # 1. Find tool in the given tool path.
    if tool_path:
        path = os.path.join(tool_path, path_in_tool)
        if executable_file_available(path, test_option):
            return path

    # 2. Find tool in the tool directory containing hiperf scripts.
    path = os.path.join('../..', path_in_tool)
    if executable_file_available(path, test_option):
        return path

    # 3. Find tool in $PATH.
    if executable_file_available(tool, test_option):
        return tool

    return None


class HdcInterface:
    def __init__(self, root_authority=True):
        hdc_path = find_tool_path(HDC_NAME)
        if not hdc_path:
            raise Exception("Can't find hdc in PATH environment.")
        self.hdc_path = hdc_path
        self.root_authority = root_authority

    def run_hdc_cmd(self, hdc_args, log_output=True):
        hdc_args = [self.hdc_path] + hdc_args
        if IS_DEBUG:
            print('run hdc cmd: %s' % hdc_args)
        subproc = subprocess.Popen(hdc_args, stdout=subprocess.PIPE)
        (out, _) = subproc.communicate()
        out = bytes_to_str(out)
        return_code = subproc.returncode
        result = (return_code == 0)
        if out and hdc_args[1] != 'file send' and \
                hdc_args[1] != 'file recv':
            if log_output:
                print(out)
        if IS_DEBUG:
            print('run hdc cmd: %s  [result %s]' % (hdc_args, result))
        return result, out

    def check_run(self, hdc_args):
        result, out = self.run_hdc_cmd(hdc_args)
        if not result:
            raise Exception('run "hdc %s" failed' % hdc_args)
        return out

    def _not_use_root(self):
        result, out = self.run_hdc_cmd(['shell', 'whoami'])
        if not result or 'root' not in out:
            return
        print('unroot hdc')
        self.run_hdc_cmd(['unroot'])
        time.sleep(1)
        self.run_hdc_cmd(['wait-for-device'])

    def switch_root(self):
        if not self.root_authority:
            self._not_use_root()
            return False
        result, out = self.run_hdc_cmd(['shell', 'whoami'])
        if not result:
            return False
        if 'root' in out:
            return True
        build_type = self.get_attribute('ro.build.type')
        if build_type == 'user':
            return False
        self.run_hdc_cmd(['root'])
        time.sleep(1)
        self.run_hdc_cmd(['wait-for-device'])
        result, out = self.run_hdc_cmd(['shell', 'whoami'])
        return result and 'root' in out

    def get_attribute(self, name):
        result, out = self.run_hdc_cmd(['shell', 'getprop',
                                        name])
        if result:
            return out

    def get_device_architecture(self):
        output = self.check_run(['shell', 'uname', '-m'])
        return get_arch(output)


class ElfStruct:
    def __init__(self, path, lib_name):
        self.path = path
        self.name = lib_name


class LocalLibDownload:

    def __init__(self, device_arch, hdc):
        self.hdc = hdc
        self.device_arch = device_arch

        self.build_id_map_of_host = {}
        self.build_id_map_of_device = {}
        self.host_lib_count_map = {}
        self.request_architectures = self.__get_need_architectures(device_arch)

    def get_host_local_libs(self, local_lib_dir):
        self.build_id_map_of_host.clear()
        for root, dirs, files in os.walk(local_lib_dir):
            for name in files:
                if name.endswith('.so'):
                    self.__append_host_local_lib(os.path.join(root, name),
                                                 name)

    def get_device_local_libs(self):
        self.build_id_map_of_device.clear()
        if os.path.exists(BUILD_ID_FILE):
            remove(BUILD_ID_FILE)
        self.hdc.check_run(['shell', 'mkdir', '-p', SYMBOL_FILES_DIR])
        self.hdc.run_hdc_cmd(['file recv', SYMBOL_FILES_DIR + BUILD_ID_FILE])
        if os.path.isfile(BUILD_ID_FILE):
            with open(BUILD_ID_FILE, 'rb') as file_bin:
                for line in file_bin.readlines():
                    line = bytes_to_str(line).strip()
                    items = line.split('=')
                    if len(items) == 2:
                        self.build_id_map_of_device[items[0]] = items[1]
            remove(BUILD_ID_FILE)

    def update_device_local_libs(self):
        # Send local libs to device.
        for build_id in self.build_id_map_of_host:
            if build_id not in self.build_id_map_of_device:
                elf_struct = self.build_id_map_of_host[build_id]
                self.hdc.check_run(['file send', elf_struct.path,
                                    SYMBOL_FILES_DIR + elf_struct.name])

        # Remove Device lib while local libs not exist on host.
        for build_id in self.build_id_map_of_device:
            if build_id not in self.build_id_map_of_host:
                name = self.build_id_map_of_device[build_id]
                self.hdc.run_hdc_cmd(['shell', 'rm', SYMBOL_FILES_DIR + name])

        # Send new build_id_list to device.
        with open(BUILD_ID_FILE, 'wb') as file_bin:
            for build_id in self.build_id_map_of_host:
                str_bytes = str_to_bytes('%s=%s\n' % (build_id,
                                          self.build_id_map_of_host[
                                              build_id].name))
                file_bin.write(str_bytes)

        self.hdc.check_run(['file send', BUILD_ID_FILE,
                            SYMBOL_FILES_DIR + BUILD_ID_FILE])
        remove(BUILD_ID_FILE)

    def __append_host_local_lib(self, path, name):
        build_id = get_build_id(path)
        if not build_id:
            return
        arch = get_architecture(path)
        if arch not in self.request_architectures:
            return

        elf_struct = self.build_id_map_of_host.get(build_id)
        if not elf_struct:
            count = self.host_lib_count_map.get(name, 0)
            self.host_lib_count_map[name] = count + 1
            if count == 0:
                unique_name = name
            else:
                unique_name = name + '_' + count
            self.build_id_map_of_host[build_id] = ElfStruct(path,
                                                            unique_name)
        else:
            elf_struct.path = path

    @classmethod
    def __get_need_architectures(self, device_arch):
        if device_arch == 'x86_64':
            return ['x86', 'x86_64']
        if device_arch == 'x86':
            return ['x86']
        if device_arch == 'arm64':
            return ['arm', 'arm64']
        if device_arch == 'arm':
            return ['arm']
        return []


class PerformanceProfile:
    """Class of all Profilers."""

    def __init__(self, args, control_module=""):
        self.args = args
        self.hdc = HdcInterface(root_authority=not args.not_hdc_root)
        self.device_root = self.hdc.switch_root()
        self.device_arch = self.hdc.get_device_architecture()
        self.record_subproc = None
        self.is_control = bool(control_module)
        self.control_mode = control_module

    def profile(self):
        if not self.is_control or self.control_mode == "prepare":
            print('prepare profiling')
            self.download()
            print('start profiling')
            if not self.combine_args():
                return
        else:
            self.exec_control()
        self.profiling()

        if not self.is_control:
            print('pull profiling data')
            self.get_profiling_data()
        if self.control_mode == "stop":
            self.wait_data_generate_done()
        print('profiling is finished.')

    def download(self):
        """Prepare recording. """
        if self.args.local_lib_dir:
            self.download_libs()

    def download_libs(self):
        executor = LocalLibDownload(self.device_arch, self.hdc)
        executor.get_host_local_libs(self.args.local_lib_dir)
        executor.get_device_local_libs()
        executor.update_device_local_libs()

    def combine_args(self):
        if self.args.package_name:
            if self.args.ability:
                self.kill_process()
            self.start_profiling(['--app', self.args.package_name])
            if self.args.ability:
                ability = self.args.package_name + '/' + self.args.ability
                start_cmd = ['shell', 'aa', 'start', '-a', ability]
                result = self.hdc.run_hdc_cmd(start_cmd)
                if not result:
                    self.record_subproc.terminate()
                    print("Can't start ability %s" % ability)
                    return False
            # else: no need to start an ability.
        elif self.args.local_program:
            pid = self.hdc.check_run(['shell', 'pidof',
                                      self.args.local_program])
            if not pid:
                print("Can't find pid of %s" % self.args.local_program)
                return False
            pid = int(pid)
            self.start_profiling(['-p', str(pid)])
        elif self.args.cmd:
            cmds = self.args.cmd.split(' ')
            cmd = [cmd.replace("'", "") for cmd in cmds]
            self.start_profiling(cmd)
        elif self.args.pid:
            self.start_profiling(['-p', ','.join(self.args.pid)])
        elif self.args.tid:
            self.start_profiling(['-t', ','.join(self.args.tid)])
        elif self.args.system_wide:
            self.start_profiling(['-a'])
        return True

    def kill_process(self):
        if self.get_app_process():
            self.hdc.check_run(['shell', 'aa', 'force-stop', self.args.app])
            count = 0
            while True:
                time.sleep(1)
                pid = self.get_app_process()
                if not pid:
                    break
                count += 1
                # 3 seconds exec kill
                if count >= 3:
                    self.run_in_app_dir(['kill', '-9', str(pid)])

    def get_app_process(self):
        result, output = self.hdc.run_hdc_cmd(
            ['shell', 'pidof', self.args.package_name])
        return int(output) if result else None

    def run_in_app_dir(self, args):
        if self.device_root:
            hdc_args = ['shell', 'cd /data/data/' + self.args.package_name +
                        ' && ' + (' '.join(args))]
        else:
            hdc_args = ['shell', 'run-as', self.args.package_name] + args
        return self.hdc.run_hdc_cmd(hdc_args, log_output=False)

    def start_profiling(self, selected_args):
        """Start hiperf reocrd process on device."""
        record_options = self.args.record_options.split(' ')
        record_options = [cmd.replace("'", "") for cmd in record_options]
        if self.is_control:
            args = ['hiperf', 'record',
                    '--control', self.control_mode, '-o',
                    '/data/local/tmp/perf.data'] + record_options
        else:
            args = ['hiperf', 'record', '-o',
                    '/data/local/tmp/perf.data'] + record_options
        if self.args.local_lib_dir and self.hdc.run_hdc_cmd(
                ['shell', 'ls', SYMBOL_FILES_DIR]):
            args += ['--symbol-dir', SYMBOL_FILES_DIR]
        args += selected_args
        hdc_args = [self.hdc.hdc_path, 'shell'] + args
        print('run hdc cmd: %s' % hdc_args)
        self.record_subproc = subprocess.Popen(hdc_args)

    def profiling(self):
        """
        Wait until profiling finishes, or stop profiling when user
        presses Ctrl-C.
        """

        try:
            return_code = self.record_subproc.wait()
        except KeyboardInterrupt:
            self.end_profiling()
            self.record_subproc = None
            return_code = 0
        print('profiling result [%s]' % (return_code == 0))
        if return_code != 0:
            raise Exception('Failed to record profiling data.')

    def end_profiling(self):
        """
        Stop profiling by sending SIGINT to hiperf, and wait until it exits
        to make sure perf.data is completely generated.
        """
        has_killed = False
        while True:
            (result, out) = self.hdc.run_hdc_cmd(
                ['shell', 'pidof', 'hiperf'])
            if not out:
                break
            if not has_killed:
                has_killed = True
                self.hdc.run_hdc_cmd(['shell', 'pkill',
                                      '-l', '2', 'hiperf'])
            time.sleep(1)

    def get_profiling_data(self):
        current_path = os.getcwd()
        full_path = os.path.join(current_path, self.args.output_perf_data)
        self.hdc.check_run(['file recv', '/data/local/tmp/perf.data',
                            full_path])
        self.hdc.run_hdc_cmd(['shell', 'rm',
                              '/data/local/tmp/perf.data'])

    def exec_control(self):
        hdc_args = [self.hdc.hdc_path, 'shell',
                    'hiperf', 'record',
                    '--control', self.control_mode]
        print('run hdc cmd: %s' % hdc_args)
        self.record_subproc = subprocess.Popen(hdc_args)

    def wait_data_generate_done(self):
        last_size = 0
        while True:
            (result, out) = self.hdc.run_hdc_cmd(
                ['shell', 'du', 'data/local/tmp/perf.data'])
            if "du" not in out:
                current_size = out.split(" ")[0]
                if current_size == last_size:
                    self.get_profiling_data()
                    break
                else:
                    last_size = current_size
            else:
                print("not generate perf.data")
                break

            time.sleep(1)
