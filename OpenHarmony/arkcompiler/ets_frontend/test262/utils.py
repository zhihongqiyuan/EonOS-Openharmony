#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
Copyright (c) 2021-2024 Huawei Device Co., Ltd.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

Description: Implement the public interface in the 262 use case
"""

import os
import signal
import sys
import subprocess
import datetime
import time
import shutil
import platform
import re
from config import DEFAULT_TIMEOUT, DEFAULT_RETRIES

TERM_NORMAL = '\033[0m'
TERM_YELLOW = '\033[1;33m'
TERM_BLUE = '\033[1;34m'
TERM_RED = '\033[1;31m'
TERM_FUCHSIA = '\033[1;35m'

ABC_EXT = ".abc"
TEMP_ABC_EXT = ".temp.abc"
TXT_EXT = ".txt"
TEMP_TXT_EXT = ".temp.txt"


def output(retcode, msg):
    if retcode == 0:
        if msg != '':
            print(str(msg))
    elif retcode == -6:
        sys.stderr.write("Aborted (core dumped)")
    elif retcode == -4:
        sys.stderr.write("Aborted (core dumped)")
    elif retcode == -11:
        sys.stderr.write("Segmentation fault (core dumped)")
    elif msg != '':
        sys.stderr.write(str(msg))
    else:
        sys.stderr.write("Unknown Error: " + str(retcode))


def filter_arm_specific_errors(errs_str):
    list_errs = []
    for err in errs_str.split("\n"):
        if err:
            if ("memset will be used instead" not in err and
                "This is the expected behaviour if you are running under QEMU" not in err and
                "Can't connect to server" not in err):
                list_errs.append(err)

    if len(list_errs) != 0:
        output(1, " ".join(list_errs))
        return False

    return True


def exec_command(cmd_args, timeout=DEFAULT_TIMEOUT, custom_cwd=None):
    proc = subprocess.Popen(cmd_args,
                            stderr=subprocess.PIPE,
                            stdout=subprocess.PIPE,
                            close_fds=True,
                            start_new_session=True,
                            cwd=custom_cwd)
    cmd_string = " ".join(cmd_args)
    code_format = 'utf-8'
    if platform.system() == "Windows":
        code_format = 'gbk'

    try:
        (output_res, errs) = proc.communicate(timeout=timeout)
        ret_code = proc.poll()

        errs_str = errs.decode(code_format, 'ignore')
        if filter_arm_specific_errors(errs_str):
            errs = None
        else:
            return 1

        if ret_code and ret_code != 1:
            code = ret_code
            msg = f"Command {cmd_string}: \n"
            msg += f"error: {errs_str}"
        else:
            code = 0
            msg = str(output_res.decode(code_format, 'ignore'))

    except subprocess.TimeoutExpired:
        proc.kill()
        proc.terminate()
        os.kill(proc.pid, signal.SIGTERM)
        code = 1
        msg = f"Timeout:'{cmd_string}' timed out after' {str(timeout)} seconds"
    except Exception as err:
        code = 1
        msg = f"{cmd_string}: unknown error: {str(err)}"
    output(code, msg)
    return code


def print_command(cmd_args):
    sys.stderr.write("\n")
    sys.stderr.write(" ".join(cmd_args))
    sys.stderr.write("\n")


# for debug use, to keep aot file
def run_command(cmd_args):
    return subprocess.run(" ".join(cmd_args))


def get_formated_path(path):
    # In the case of Windows, it is necessary to convert ' \\' to '/', otherwise there will be a crash or the file cannot be found
    # Maintain consistent interface path with DevEco Studio 
    if platform.system() == "Windows":
        return path.replace("\\", "/")
    return path


def current_time():
    return datetime.datetime.now().strftime('%m-%d %H:%M:%S.%f')


class Logging():
    def __init__(self):
        self.is_logging = True

    def debug(self, info):
        if self.is_logging:
            print(
                f'{current_time()} D:>>>  {TERM_BLUE}{str(info)}{TERM_NORMAL}')

    def info(self, info):
        if self.is_logging:
            if len(str(info)) > 100:
                print(f'{current_time()} I:>>> \n{str(info)} ')
            else:
                print(f'{current_time()} I:>>>    {str(info)} ')


LOGGING = Logging()


class Command():
    def __init__(self, cmd):
        self.cmd = cmd

    def run(self):
        LOGGING.debug("command: " + self.cmd)
        out = os.popen(self.cmd).read()
        LOGGING.info(out)
        return out


def run_cmd(command):
    cmd = Command(command)
    return cmd.run()


class CommandCwd():
    def __init__(self, cmds, cwd):
        self.cmds = cmds
        self.cwd = cwd

    def run(self):
        cmd = " ".join(self.cmds)
        LOGGING.debug("command: " + cmd + " | " + "dir: " + self.cwd)
        if platform.system() == "Windows" :
            proc = subprocess.Popen(self.cmds, cwd=self.cwd ,shell=True)
        else :
            proc = subprocess.Popen(self.cmds, cwd=self.cwd)
        return proc.wait()


def run_cmd_cwd(commands, cwd=os.getcwd()):
    cmd = CommandCwd(commands, cwd)
    return cmd.run()


def sleep(duration):
    LOGGING.debug("sleeping %d" % duration)
    time.sleep(duration)


def write_file(save_file, result):
    LOGGING.debug(f"write file:{save_file}")
    with open(save_file, "a+") as file:
        file.write(result + "\n")
        file.flush()


def remove_dir(path):
    if os.path.exists(path):
        shutil.rmtree(path)


def remove_file(file):
    if os.path.exists(file):
        os.remove(file)


def mkdir(path):
    if not os.path.exists(path):
        os.makedirs(path)


def report_command(cmd_type, cmd, env=None):
    sys.stderr.write(f'{TERM_BLUE}{cmd_type}{TERM_NORMAL}\n')
    if env is not None:
        sys.stderr.write(''.join(f'{TERM_BLUE}{var}={val} \\{TERM_NORMAL}\n'
                                 for var, val in sorted(env.items())))
    cmd_str = (f'{TERM_NORMAL}\n\t{TERM_BLUE}').join(cmd)
    sys.stderr.write(f'\t{TERM_BLUE}{cmd_str}{TERM_NORMAL}\n')
    sys.stderr.write("\n")


def git_clone(git_url, code_dir):
    cmd = ['git', 'clone', git_url, code_dir]
    retries = 1
    while retries <= DEFAULT_RETRIES:
        ret = run_cmd_cwd(cmd)
        if ret == 0:
            break
        else:
            print(f"\n warning: Atempt: #{retries} to clone '{git_url}' failed. Try cloining again")
            retries += 1
    assert not ret, f"\n error: Cloning '{git_url}' failed."


def git_checkout(git_bash, cwd):
    cmd = ['git', 'checkout', '-f', git_bash]
    ret = run_cmd_cwd(cmd, cwd)
    assert not ret, f"\n error: git checkout '{git_bash}' failed."


def git_apply(patch_file, cwd):
    cmd = ['git', 'apply', patch_file]
    ret = run_cmd_cwd(cmd, cwd)
    assert not ret, f"\n error: Failed to apply '{patch_file}'"


def git_clean(cwd):
    cmd = ['git', 'checkout', '--', '.']
    run_cmd_cwd(cmd, cwd)


def npm_install(cwd):
    cmd = ['npm', 'install']
    ret = run_cmd_cwd(cmd, cwd)
    assert not ret, f"\n error: Failed to 'npm install'"


def search_dependency(file, directory):
    for root, dirs, files in os.walk(directory, topdown=True):
        for f in files:
            if f == file:
                return os.path.join(root, f)
    return "FILE_NOT_FOUND"


def collect_module_dependencies(file, directory, traversedDependencies):
    dependencies = []
    traversedDependencies.append(file)
    with open(file, 'r', encoding='utf-8') as f:
        content = f.read()
        module_import_list = re.findall(r'(import|from)(?:\s*)\(?(\'(\.\/.*)\'|"(\.\/.*)")\)?', content)

        for result in list(set(module_import_list)):
            specifier = (result[2] if len(result[2]) != 0 else result[3]).lstrip('./')
            if os.path.basename(file) is not specifier:
                dependency = search_dependency(specifier, directory)
                if dependency == "FILE_NOT_FOUND":
                    continue

                if dependency not in traversedDependencies:
                    dependencies.extend(collect_module_dependencies(dependency, directory,
                                                                    list(set(traversedDependencies))))
                dependencies.append(dependency)

    return dependencies