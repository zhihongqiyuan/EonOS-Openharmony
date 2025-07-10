#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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

from glob import glob
from os import path
from enum import Enum
import argparse
import fnmatch
import multiprocessing
import os
import re
import shutil
import subprocess
import sys
from config import API_VERSION_MAP, ARK_JS_VM_LIST, MIN_SUPPORT_BC_VERSION, MIX_COMPILE_ENTRY_POINT


def is_directory(parser, arg):
    if not path.isdir(arg):
        parser.error("The directory '%s' does not exist" % arg)

    return path.abspath(arg)


def is_file(parser, arg):
    if not path.isfile(arg):
        parser.error("The file '%s' does not exist" % arg)

    return path.abspath(arg)

def prepare_tsc_testcases(test_root):
    third_party_tsc = path.join(test_root, "TypeScript")
    ohos_third_party_tsc = path.join(test_root, "../../../../third_party/typescript")

    if not path.isdir(third_party_tsc):
        if (path.isdir(ohos_third_party_tsc)):
            return path.abspath(ohos_third_party_tsc)
        subprocess.run(
            f"git clone https://gitee.com/openharmony/third_party_typescript.git {third_party_tsc}",
            shell=True,
            stdout=subprocess.DEVNULL,
        )
    else:
        subprocess.run(
            f"cd {third_party_tsc} && git clean -f > /dev/null 2>&1",
            shell=True,
            stdout=subprocess.DEVNULL,
        )
    return third_party_tsc

def check_timeout(value):
    ivalue = int(value)
    if ivalue <= 0:
        raise argparse.ArgumentTypeError(
            "%s is an invalid timeout value" % value)
    return ivalue


def get_args():
    parser = argparse.ArgumentParser(description="Regression test runner")
    parser.add_argument(
        'build_dir', type=lambda arg: is_directory(parser, arg),
        help='panda build directory')
    parser.add_argument(
        '--error', action='store_true', dest='error', default=False,
        help='capture stderr')
    parser.add_argument(
        '--abc-to-asm', action='store_true', dest='abc_to_asm',
        default=False, help='run abc2asm tests')
    parser.add_argument(
        '--regression', '-r', action='store_true', dest='regression',
        default=False, help='run regression tests')
    parser.add_argument(
        '--compiler', '-c', action='store_true', dest='compiler',
        default=False, help='run compiler tests')
    parser.add_argument(
        '--tsc', action='store_true', dest='tsc',
        default=False, help='run tsc tests')
    parser.add_argument(
        '--no-progress', action='store_false', dest='progress', default=True,
        help='don\'t show progress bar')
    parser.add_argument(
        '--no-skip', action='store_false', dest='skip', default=True,
        help='don\'t use skiplists')
    parser.add_argument(
        '--update', action='store_true', dest='update', default=False,
        help='update skiplist')
    parser.add_argument(
        '--no-run-gc-in-place', action='store_true', dest='no_gip', default=False,
        help='enable --run-gc-in-place mode')
    parser.add_argument(
        '--filter', '-f', action='store', dest='filter',
        default="*", help='test filter regexp')
    parser.add_argument(
        '--es2panda-timeout', type=check_timeout,
        dest='es2panda_timeout', default=60, help='es2panda translator timeout')
    parser.add_argument(
        '--paoc-timeout', type=check_timeout,
        dest='paoc_timeout', default=600, help='paoc compiler timeout')
    parser.add_argument(
        '--timeout', type=check_timeout,
        dest='timeout', default=10, help='JS runtime timeout')
    parser.add_argument(
        '--gc-type', dest='gc_type', default="g1-gc", help='Type of garbage collector')
    parser.add_argument(
        '--aot', action='store_true', dest='aot', default=False,
        help='use AOT compilation')
    parser.add_argument(
        '--no-bco', action='store_false', dest='bco', default=True,
        help='disable bytecodeopt')
    parser.add_argument(
        '--jit', action='store_true', dest='jit', default=False,
        help='use JIT in interpreter')
    parser.add_argument(
        '--arm64-compiler-skip', action='store_true', dest='arm64_compiler_skip', default=False,
        help='use skiplist for tests failing on aarch64 in AOT or JIT mode')
    parser.add_argument(
        '--arm64-qemu', action='store_true', dest='arm64_qemu', default=False,
        help='launch all binaries in qemu aarch64')
    parser.add_argument(
        '--arm32-qemu', action='store_true', dest='arm32_qemu', default=False,
        help='launch all binaries in qemu arm')
    parser.add_argument(
        '--test-list', dest='test_list', default=None, type=lambda arg: is_file(parser, arg),
        help='run tests listed in file')
    parser.add_argument(
        '--aot-args', action='append', dest='aot_args', default=[],
        help='Additional arguments that will passed to ark_aot')
    parser.add_argument(
        '--verbose', '-v', action='store_true', dest='verbose', default=False,
        help='Enable verbose output')
    parser.add_argument(
        '--js-runtime', dest='js_runtime_path', default=None, type=lambda arg: is_directory(parser, arg),
        help='the path of js vm runtime')
    parser.add_argument(
        '--LD_LIBRARY_PATH', dest='ld_library_path', default=None, help='LD_LIBRARY_PATH')
    parser.add_argument(
        '--tsc-path', dest='tsc_path', default=None, type=lambda arg: is_directory(parser, arg),
        help='the path of tsc')
    parser.add_argument('--hotfix', dest='hotfix', action='store_true', default=False,
        help='run hotfix tests')
    parser.add_argument('--hotreload', dest='hotreload', action='store_true', default=False,
        help='run hotreload tests')
    parser.add_argument('--coldfix', dest='coldfix', action='store_true', default=False,
        help='run coldfix tests')
    parser.add_argument('--coldreload', dest='coldreload', action='store_true', default=False,
        help='run coldreload tests')
    parser.add_argument('--base64', dest='base64', action='store_true', default=False,
        help='run base64 tests')
    parser.add_argument('--bytecode', dest='bytecode', action='store_true', default=False,
        help='run bytecode tests')
    parser.add_argument('--debugger', dest='debugger', action='store_true', default=False,
        help='run debugger tests')
    parser.add_argument('--debug', dest='debug', action='store_true', default=False,
        help='run debug tests')
    parser.add_argument('--enable-arkguard', action='store_true', dest='enable_arkguard', default=False,
        help='enable arkguard for compiler tests')
    parser.add_argument('--aop-transform', dest='aop_transform', action='store_true', default=False,
        help='run debug tests')
    parser.add_argument('--version-control', action='store_true', dest='version_control', default=False,
        help='run api version control tests')

    return parser.parse_args()


def run_subprocess_with_beta3(test_obj, cmd):
    has_target_api = False
    has_version_12 = False
    has_sub_version = False
    is_es2abc_cmd = False

    for param in cmd:
        if "es2abc" in param:
            is_es2abc_cmd = True
        if "--target-api-sub-version" in param:
            has_sub_version = True
        if "--target-api-version" in param:
            has_target_api = True
        if "12" in param:
            has_version_12 = True
    if is_es2abc_cmd and (not has_target_api or (has_version_12 and not has_sub_version)):
        cmd.append("--target-api-sub-version=beta3")
    if test_obj:
        test_obj.log_cmd(cmd)
    return subprocess.Popen(
        cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)


class Test:
    def __init__(self, test_path, flags):
        self.path = test_path
        self.flags = flags
        self.output = None
        self.error = None
        self.passed = None
        self.skipped = None
        self.reproduce = ""

    def log_cmd(self, cmd):
        self.reproduce += "\n" + ' '.join(cmd)

    def get_path_to_expected(self):
        if self.path.find(".d.ts") == -1:
            return "%s-expected.txt" % (path.splitext(self.path)[0])
        return "%s-expected.txt" % (self.path[:self.path.find(".d.ts")])

    def run(self, runner):
        test_abc_name = ("%s.abc" % (path.splitext(self.path)[0])).replace("/", "_")
        test_abc_path = path.join(runner.build_dir, test_abc_name)
        cmd = runner.cmd_prefix + [runner.es2panda]
        cmd.extend(self.flags)
        cmd.extend(["--output=" + test_abc_path])
        cmd.append(self.path)
        process = run_subprocess_with_beta3(self, cmd)
        out, err = process.communicate()
        self.output = out.decode("utf-8", errors="ignore") + err.decode("utf-8", errors="ignore")

        expected_path = self.get_path_to_expected()
        try:
            with open(expected_path, 'r') as fp:
                expected = fp.read()
            self.passed = expected == self.output and process.returncode in [
                0, 1]
        except Exception:
            self.passed = False

        if not self.passed:
            self.error = err.decode("utf-8", errors="ignore")

        if os.path.exists(test_abc_path):
            os.remove(test_abc_path)

        return self


class TSCTest(Test):
    def __init__(self, test_path, flags):
        Test.__init__(self, test_path, flags)
        self.options = self.parse_options()

    def parse_options(self):
        test_options = {}

        with open(self.path, "r", encoding="latin1") as f:
            lines = f.read()
            options = re.findall(r"//\s?@\w+:.*\n", lines)

            for option in options:
                separated = option.split(":")
                opt = re.findall(r"\w+", separated[0])[0].lower()
                value = separated[1].strip().lower()

                if opt == "filename":
                    if opt in options:
                        test_options[opt].append(value)
                    else:
                        test_options[opt] = [value]

                elif opt == "lib" or opt == "module":
                    test_options[opt] = [each.strip()
                                         for each in value.split(",")]
                elif value == "true" or value == "false":
                    test_options[opt] = value.lower() == "true"
                else:
                    test_options[opt] = value

            # TODO: Possibility of error: all exports will be catched, even the commented ones
            if 'module' not in test_options and re.search(r"export ", lines):
                test_options['module'] = []

        return test_options

    def run(self, runner):
        cmd = runner.cmd_prefix + [runner.es2panda, '--parse-only']
        cmd.extend(self.flags)
        if "module" in self.options:
            cmd.append('--module')
        cmd.append(self.path)
        process = run_subprocess_with_beta3(self, cmd)
        out, err = process.communicate()
        self.output = out.decode("utf-8", errors="ignore")

        self.passed = True if process.returncode == 0 else False

        if not self.passed:
            self.error = err.decode("utf-8", errors="ignore")

        return self


class TestAop:
    def __init__(self, cmd, compare_str, compare_abc_str, remove_file):
        self.cmd = cmd
        self.compare_str = compare_str
        self.compare_abc_str = compare_abc_str
        self.remove_file = remove_file
        self.path = ''
        self.output = None
        self.error = None
        self.passed = None
        self.skipped = None
        self.reproduce = ""

    def log_cmd(self, cmd):
        self.reproduce += ''.join(["\n", ' '.join(cmd)])

    def run(self, runner):
        cmd = self.cmd
        process = run_subprocess_with_beta3(self, cmd)
        out, err = process.communicate()
        self.output = out.decode("utf-8", errors="ignore") + err.decode("utf-8", errors="ignore")

        if self.compare_str == '':
            self.passed = True
        else :
            self.passed = self.output.startswith(self.compare_str) and process.returncode in [0, 1]
            if self.remove_file != '' and os.path.exists(self.remove_file):
                os.remove(self.remove_file)

        if not self.passed:
            self.error = err.decode("utf-8", errors="ignore")

        abc_path = path.join(os.getcwd(), 'test_aop.abc')
        if os.path.exists(abc_path):
            if self.compare_abc_str != '':
                with open(abc_path, "r") as abc_file:
                    self.passed = self.passed and abc_file.read() == self.compare_abc_str
            os.remove(abc_path)

        return self


class Runner:
    def __init__(self, args, name):
        self.test_root = path.dirname(path.abspath(__file__))
        self.args = args
        self.name = name
        self.tests = []
        self.failed = 0
        self.passed = 0
        self.es2panda = path.join(args.build_dir, 'es2abc')
        self.build_dir = args.build_dir
        self.cmd_prefix = []
        self.ark_js_vm = ""
        self.ark_aot_compiler = ""
        self.ld_library_path = ""

        if args.js_runtime_path:
            self.ark_js_vm = path.join(args.js_runtime_path, 'ark_js_vm')
            self.ark_aot_compiler = path.join(args.js_runtime_path, 'ark_aot_compiler')

        if args.ld_library_path:
            self.ld_library_path = args.ld_library_path

        if args.arm64_qemu:
            self.cmd_prefix = ["qemu-aarch64", "-L", "/usr/aarch64-linux-gnu/"]

        if args.arm32_qemu:
            self.cmd_prefix = ["qemu-arm", "-L", "/usr/arm-linux-gnueabi"]

        if not path.isfile(self.es2panda):
            raise Exception("Cannot find es2panda binary: %s" % self.es2panda)

    def add_directory(self, directory, extension, flags):
        pass

    def test_path(self, src):
        pass

    def run_test(self, test):
        return test.run(self)

    def run(self):
        pool = multiprocessing.Pool()
        result_iter = pool.imap_unordered(
            self.run_test, self.tests, chunksize=32)
        pool.close()

        if self.args.progress:
            from tqdm import tqdm
            result_iter = tqdm(result_iter, total=len(self.tests))

        results = []
        for res in result_iter:
            results.append(res)

        self.tests = results
        pool.join()

    def deal_error(self, test):
        path_str = test.path
        err_col = {}
        if test.error:
            err_str = test.error.split('[')[0] if "patchfix" not in test.path else " patchfix throw error failed"
            err_col = {"path" : [path_str], "status": ["fail"], "error" : [test.error], "type" : [err_str]}
        else:
            err_col = {"path" : [path_str], "status": ["fail"], "error" : ["Segmentation fault"],
                        "type" : ["Segmentation fault"]}
        return err_col

    def summarize(self):
        print("")
        fail_list = []
        success_list = []

        for test in self.tests:
            assert(test.passed is not None)
            if not test.passed:
                fail_list.append(test)
            else:
                success_list.append(test)

        if len(fail_list):
            if self.args.error:
                import pandas as pd
                test_list = pd.DataFrame(columns=["path", "status", "error", "type"])
            for test in success_list:
                suc_col = {"path" : [test.path], "status": ["success"], "error" : ["success"], "type" : ["success"]}
                if self.args.error:
                    test_list = pd.concat([test_list, pd.DataFrame(suc_col)])
            print("Failed tests:")
            for test in fail_list:
                print(self.test_path(test.path))

                if self.args.error:
                    print("steps:", test.reproduce)
                    print("error:")
                    print(test.error)
                    print("\n")
                    err_col = self.deal_error(test)
                    test_list = pd.concat([test_list, pd.DataFrame(err_col)])

            if self.args.error:
                test_list.to_csv('test_statistics.csv', index=False)
                test_list["type"].value_counts().to_csv('type_statistics.csv', index_label="error")
                print("Type statistics:\n", test_list["type"].value_counts())
            print("")

        print("Summary(%s):" % self.name)
        print("\033[37mTotal:   %5d" % (len(self.tests)))
        print("\033[92mPassed:  %5d" % (len(self.tests) - len(fail_list)))
        print("\033[91mFailed:  %5d" % (len(fail_list)))
        print("\033[0m")

        return len(fail_list)


class RegressionRunner(Runner):
    def __init__(self, args):
        Runner.__init__(self, args, "Regression")

    def add_directory(self, directory, extension, flags, func=Test):
        glob_expression = path.join(
            self.test_root, directory, "*.%s" % (extension))
        files = glob(glob_expression)
        files = fnmatch.filter(files, self.test_root + '**' + self.args.filter)

        self.tests += list(map(lambda f: func(f, flags), files))

    def test_path(self, src):
        return src


class AbcToAsmRunner(Runner):
    def __init__(self, args, is_debug):
        Runner.__init__(self, args, "Abc2asm" if not is_debug else "Abc2asmDebug")
        self.is_debug = is_debug

    def add_directory(self, directory, extension, flags, func=Test):
        glob_expression = path.join(
            self.test_root, directory, "*.%s" % (extension))
        files = glob(glob_expression)
        files = fnmatch.filter(files, self.test_root + '**' + self.args.filter)

        self.tests += list(map(lambda f: AbcToAsmTest(f, flags, self.is_debug), files))

    def test_path(self, src):
        return os.path.basename(src)


class AbcToAsmTest(Test):
    def __init__(self, test_path, flags, is_debug):
        Test.__init__(self, test_path, flags)
        self.is_debug = is_debug

    def run(self, runner):
        output_abc_file = ("%s.abc" % (path.splitext(self.path)[0])).replace("/", "_")
        # source code compilation, generate an abc file
        gen_abc_cmd = runner.cmd_prefix + [runner.es2panda]
        if (self.is_debug):
            gen_abc_cmd.extend(["--debug-info"])
        gen_abc_cmd.extend(["--module", "--dump-normalized-asm-program", "--output=" + output_abc_file])
        gen_abc_cmd.append(self.path)
        process_gen_abc = run_subprocess_with_beta3(self, gen_abc_cmd)
        gen_abc_out, gen_abc_err = process_gen_abc.communicate()
        gen_abc_output = gen_abc_out.decode("utf-8", errors="ignore")

        # If no abc file is generated, an error occurs during parser, but abc2asm function is normal.
        if not os.path.exists(output_abc_file):
            self.passed = True
            return self

        # abc file compilation
        abc_to_asm_cmd = runner.cmd_prefix + [runner.es2panda]
        if (self.is_debug):
            abc_to_asm_cmd.extend(["--debug-info"])
        abc_to_asm_cmd.extend(["--module", "--dump-normalized-asm-program", "--enable-abc-input"])
        abc_to_asm_cmd.append(output_abc_file)
        process_abc_to_asm = run_subprocess_with_beta3(self, abc_to_asm_cmd)
        abc_to_asm_out, abc_to_asm_err = process_abc_to_asm.communicate()
        abc_to_asm_output = abc_to_asm_out.decode("utf-8", errors="ignore")

        self.passed = gen_abc_output == abc_to_asm_output and process_abc_to_asm.returncode in [0, 1]
        if not self.passed:
            self.error = "Comparison of dump results between source code compilation and abc file compilation failed."
            if gen_abc_err:
                self.error += "\n" + gen_abc_err.decode("utf-8", errors="ignore")
            if abc_to_asm_err:
                self.error += "\n" + abc_to_asm_err.decode("utf-8", errors="ignore")

        os.remove(output_abc_file)
        return self


class TSCRunner(Runner):
    def __init__(self, args):
        Runner.__init__(self, args, "TSC")

        if self.args.tsc_path:
            self.tsc_path = self.args.tsc_path
        else :
            self.tsc_path = prepare_tsc_testcases(self.test_root)

        self.add_directory("conformance", [])
        self.add_directory("compiler", [])

    def add_directory(self, directory, flags):
        ts_suite_dir = path.join(self.tsc_path, 'tests/cases')

        glob_expression = path.join(
            ts_suite_dir, directory, "**/*.ts")
        files = glob(glob_expression, recursive=True)
        files = fnmatch.filter(files, ts_suite_dir + '**' + self.args.filter)

        for f in files:
            test_name = path.basename(f.split(".ts")[0])
            negative_references = path.join(
                self.tsc_path, 'tests/baselines/reference')
            is_negative = path.isfile(path.join(negative_references,
                                                test_name + ".errors.txt"))
            test = TSCTest(f, flags)

            if 'target' in test.options:
                targets = test.options['target'].replace(" ", "").split(',')
                for target in targets:
                    if path.isfile(path.join(negative_references,
                                             test_name + "(target=%s).errors.txt" % (target))):
                        is_negative = True
                        break

            if is_negative or "filename" in test.options:
                continue

            with open(path.join(self.test_root, 'test_tsc_ignore_list.txt'), 'r') as failed_references:
                if self.args.skip:
                    if path.relpath(f, self.tsc_path) in failed_references.read():
                        continue

            self.tests.append(test)

    def test_path(self, src):
        return src


class CompilerRunner(Runner):
    def __init__(self, args):
        Runner.__init__(self, args, "Compiler")

    def add_directory(self, directory, extension, flags):
        if directory.endswith("projects"):
            projects_path = path.join(self.test_root, directory)
            for project in os.listdir(projects_path):
                glob_expression = path.join(projects_path, project, "**/*.%s" % (extension))
                files = glob(glob_expression, recursive=True)
                files = fnmatch.filter(files, self.test_root + '**' + self.args.filter)
                self.tests.append(CompilerProjectTest(projects_path, project, files, flags))
        elif directory.endswith("protobin"):
            test_path = path.join(self.test_root, directory)
            for project in os.listdir(test_path):
                self.tests.append(CompilerProtobinTest(path.join(test_path, project), flags))
        elif directory.endswith("abc2program"):
            test_path = path.join(self.test_root, directory)
            for project in os.listdir(test_path):
                self.tests.append(CompilerAbcFileTest(path.join(test_path, project), flags))
        else:
            glob_expression = path.join(
                self.test_root, directory, "**/*.%s" % (extension))
            files = glob(glob_expression, recursive=True)
            files = fnmatch.filter(files, self.test_root + '**' + self.args.filter)
            self.tests += list(map(lambda f: CompilerTest(f, flags), files))

    def test_path(self, src):
        return src


class CompilerTest(Test):
    def __init__(self, test_path, flags):
        Test.__init__(self, test_path, flags)

    def execute_arkguard(self, runner):
        input_file_path = self.path
        arkguard_root_dir = os.path.join(runner.test_root, "../../arkguard")
        arkgurad_entry_path = os.path.join(arkguard_root_dir, "lib/cli/SecHarmony.js")
        config_path = os.path.join(arkguard_root_dir, "test/compilerTestConfig.json")
        arkguard_cmd = [
            'node',
            '--no-warnings',
            arkgurad_entry_path,
            input_file_path,
            '--config-path',
            config_path,
            '--inplace'
        ]
        self.log_cmd(arkguard_cmd)
        process = subprocess.Popen(arkguard_cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        out, err = process.communicate()
        process.wait()
        success = True
        if err or process.returncode != 0:
            success = False
            self.passed = False
            self.error = err.decode("utf-8", errors="ignore")
        return success

    def run(self, runner):
        test_abc_name = ("%s.abc" % (path.splitext(self.path)[0])).replace("/", "_")
        test_abc_path = path.join(runner.build_dir, test_abc_name)
        es2abc_cmd = runner.cmd_prefix + [runner.es2panda]
        es2abc_cmd.extend(self.flags)
        es2abc_cmd.extend(["--output=" + test_abc_path])
        es2abc_cmd.append(self.path)
        enable_arkguard = runner.args.enable_arkguard
        if enable_arkguard:
            success = self.execute_arkguard(runner)
            if not success:
                return self

        process = run_subprocess_with_beta3(self, es2abc_cmd)
        out, err = process.communicate()
        if "--dump-assembly" in self.flags:
            pa_expected_path = "".join([self.get_path_to_expected()[:self.get_path_to_expected().rfind(".txt")],
                                       ".pa.txt"])
            self.output = out.decode("utf-8", errors="ignore") + err.decode("utf-8", errors="ignore")
            try:
                with open(pa_expected_path, 'r') as fp:
                    expected = fp.read()
                self.passed = expected == self.output and process.returncode in [0, 1]
            except Exception:
                self.passed = False
            if not self.passed:
                self.error = err.decode("utf-8", errors="ignore")
                if os.path.exists(test_abc_path):
                    os.remove(test_abc_path)
                return self
        if "--dump-debug-info" in self.flags:
            self.output = out.decode("utf-8", errors="ignore") + err.decode("utf-8", errors="ignore")
            try:
                with open(self.get_path_to_expected(), 'r') as fp:
                    expected = fp.read()
                self.passed = expected == self.output and process.returncode in [0, 1]
                if os.path.exists(test_abc_path):
                    os.remove(test_abc_path)
                return self
            except Exception:
                self.passed = False
            if not self.passed:
                self.error = err.decode("utf-8", errors="ignore")
                if os.path.exists(test_abc_path):
                    os.remove(test_abc_path)
                return self
        if err:
            self.passed = False
            self.error = err.decode("utf-8", errors="ignore")
            return self

        ld_library_path = runner.ld_library_path
        os.environ.setdefault("LD_LIBRARY_PATH", ld_library_path)
        run_abc_cmd = [runner.ark_js_vm, '--enable-force-gc=false', test_abc_path]
        self.log_cmd(run_abc_cmd)

        process = subprocess.Popen(run_abc_cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        out, err = process.communicate()
        self.output = out.decode("utf-8", errors="ignore") + err.decode("utf-8", errors="ignore")
        expected_path = self.get_path_to_expected()
        try:
            with open(expected_path, 'r') as fp:
                expected = fp.read()
            self.passed = expected == self.output and process.returncode in [0, 1]
        except Exception:
            self.passed = False

        if not self.passed:
            self.error = err.decode("utf-8", errors="ignore")

        os.remove(test_abc_path)

        return self


class CompilerAbcFileTest(Test):
    def __init__(self, test_dir, flags):
        Test.__init__(self, test_dir, flags)
        self.test_dir = test_dir
        self.generated_path = os.path.join(self.test_dir, "gen")
        if not path.exists(self.generated_path):
            os.makedirs(self.generated_path)
        self.original_abc_path = os.path.join(self.generated_path, "original.abc")
        self.output_path = os.path.join(self.generated_path, "result.abc")
        self.original_test = os.path.join(self.test_dir, "base.ts")
        self.expected_path = os.path.join(self.test_dir, "expected.txt")

    def remove_test_build(self, runner):
        if path.exists(self.generated_path):
            shutil.rmtree(self.generated_path)

    def gen_abc(self, runner, test_path, output_path, flags):
        es2abc_cmd = runner.cmd_prefix + [runner.es2panda]
        es2abc_cmd.extend(['%s%s' % ("--output=", output_path)])
        es2abc_cmd.extend(flags)
        es2abc_cmd.append(test_path)
        process = run_subprocess_with_beta3(self, es2abc_cmd)
        out, err = process.communicate()
        if err:
            self.passed = False
            self.error = err.decode("utf-8", errors="ignore")
            self.remove_test_build(runner)
            return self

    def run(self, runner):
        new_flags = self.flags
        # Generate 'abc' from the source file
        self.gen_abc(runner, self.original_test, self.original_abc_path, new_flags)
        # Generate 'abc' from the abc file
        new_flags = self.flags
        compile_context_info_path = path.join(self.test_dir, "compileContextInfo.json")
        if path.exists(compile_context_info_path):
            new_flags.append("%s%s" % ("--compile-context-info=", compile_context_info_path))
        es2abc_cmd = runner.cmd_prefix + [runner.es2panda]
        es2abc_cmd.append('%s%s' % ("--output=", self.output_path))
        es2abc_cmd.append(self.original_abc_path)
        es2abc_cmd.extend(new_flags)
        process = run_subprocess_with_beta3(self, es2abc_cmd)
        out, err = process.communicate()
        self.output = out.decode("utf-8", errors="ignore") + err.decode("utf-8", errors="ignore")
        try:
            with open(self.expected_path, 'r') as fp:
                expected = fp.read()
            self.passed = expected == self.output and process.returncode in [0, 1]
        except Exception:
            self.passed = False
        if not self.passed:
            self.remove_test_build(runner)
            return self
        self.remove_test_build(runner)
        return self


class CompilerProtobinTest(Test):
    def __init__(self, test_dir, flags):
        Test.__init__(self, test_dir, flags)
        self.test_dir = test_dir
        self.generated_path = os.path.join(self.test_dir, "gen")
        if not path.exists(self.generated_path):
            os.makedirs(self.generated_path)
        self.protobin_path = os.path.join(self.generated_path, "cache.protobin")
        self.original_abc_path = os.path.join(self.generated_path, "base.abc")
        self.output_path = os.path.join(self.generated_path, "module.abc")
        self.original_test = os.path.join(self.test_dir, "base.ts")
        self.modify_test = os.path.join(self.test_dir, "base_mod.ts")
        self.expected_path = os.path.join(self.test_dir, "expected.txt")

    def remove_test_build(self, runner):
        if path.exists(self.generated_path):
            shutil.rmtree(self.generated_path)

    def gen_merge_abc(self, runner, test_path, need_cache, output_path):
        es2abc_cmd = runner.cmd_prefix + [runner.es2panda]
        es2abc_cmd.extend(["--merge-abc"])
        if need_cache:
           es2abc_cmd.extend(["--enable-abc-input", '%s%s' % ("--cache-file=", self.protobin_path)])
        es2abc_cmd.extend(['%s%s' % ("--output=", output_path)])
        es2abc_cmd.append(test_path)
        process = run_subprocess_with_beta3(self, es2abc_cmd)
        out, err = process.communicate()
        if err:
            self.passed = False
            self.error = err.decode("utf-8", errors="ignore")
            self.remove_test_build(runner)
            return self

    def run(self, runner):
        # Generate 'abc' from the source file before modifying it
        self.gen_merge_abc(runner, self.original_test, False, self.original_abc_path)
        # Generate protobin from the abc file before modifying it
        self.gen_merge_abc(runner, self.original_abc_path, True, self.output_path)
        # Modify the original abc file
        self.gen_merge_abc(runner, self.modify_test, False, self.original_abc_path)
        # Compile based on the modified abc file
        self.gen_merge_abc(runner, self.original_abc_path, True, self.output_path)
        ld_library_path = runner.ld_library_path
        os.environ.setdefault("LD_LIBRARY_PATH", ld_library_path)
        run_abc_cmd = [runner.ark_js_vm, '--entry-point=base', self.output_path]
        self.log_cmd(run_abc_cmd)

        process = subprocess.Popen(run_abc_cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        out, err = process.communicate()
        self.output = out.decode("utf-8", errors="ignore") + err.decode("utf-8", errors="ignore")
        try:
            with open(self.expected_path, 'r') as fp:
                expected = fp.read()
            self.passed = expected == self.output and process.returncode in [0, 1]
        except Exception:
            self.passed = False
        if not self.passed:
            self.remove_test_build(runner)
            return self
        self.remove_test_build(runner)
        return self


class CompilerProjectTest(Test):
    def __init__(self, projects_path, project, test_paths, flags):
        Test.__init__(self, "", flags)
        self.projects_path = projects_path
        self.project = project
        self.test_paths = test_paths
        self.files_info_path = os.path.join(os.path.join(self.projects_path, self.project), 'filesInfo.txt')
        self.files_info_mod_path = os.path.join(os.path.join(self.projects_path, self.project), 'filesInfoModify.txt')
        # Skip execution if --dump-assembly exists in flags
        self.requires_execution = "--dump-assembly" not in self.flags
        self.file_record_mapping = None
        self.generated_abc_inputs_path = os.path.join(os.path.join(self.projects_path, self.project), "abcinputs_gen")
        self.abc_input_filenames = None
        self.protoBin_file_path = ""
        self.record_names_path = os.path.join(os.path.join(self.projects_path, self.project), 'recordnames.txt')
        self.abc_inputs_path = os.path.join(os.path.join(self.projects_path, self.project), 'abcinputs')
        # Modify the hap file path
        self.project_mod_path = os.path.join(os.path.join(self.projects_path, self.project), 'mod')
        self.modules_cache_path = os.path.join(os.path.join(self.projects_path, self.project), 'modulescache.cache')
        self.deps_json_path = os.path.join(os.path.join(self.projects_path, self.project), 'deps-json.json')
        # Merge hap need to modify package name
        self.modifyPkgNamePath = os.path.join(os.path.join(self.projects_path, self.project), 'modify_pkg_name.txt')

    def remove_project(self, runner):
        project_path = runner.build_dir + "/" + self.project
        if path.exists(project_path):
            shutil.rmtree(project_path)
        if path.exists(self.files_info_path):
            os.remove(self.files_info_path)
        if path.exists(self.files_info_mod_path):
            os.remove(self.files_info_mod_path)
        if path.exists(self.generated_abc_inputs_path):
            shutil.rmtree(self.generated_abc_inputs_path)
        if path.exists(self.protoBin_file_path):
            os.remove(self.protoBin_file_path)
        if path.exists(self.modules_cache_path):
            self.remove_cache_files()

    def remove_cache_files(self):
        if path.exists(self.modules_cache_path):
            with open(self.modules_cache_path) as cache_fp:
                cache_lines = cache_fp.readlines()
                for cache_line in cache_lines:
                    cache_file_path = cache_line[:-1].split(";")[1]
                    if path.exists(cache_file_path):
                        os.remove(cache_file_path)
            os.remove(self.modules_cache_path)

    def get_file_absolute_path_and_name(self, runner):
        sub_path = self.path[len(self.projects_path):]
        file_relative_path = path.split(sub_path)[0]
        file_name = path.split(sub_path)[1]
        file_absolute_path = runner.build_dir + "/" + file_relative_path
        return [file_absolute_path, file_name]

    def gen_single_abc(self, runner):
        for test_path in self.test_paths:
            self.path = test_path
            [file_absolute_path, file_name] = self.get_file_absolute_path_and_name(runner)
            if not path.exists(file_absolute_path):
                os.makedirs(file_absolute_path)

            test_abc_name = ("%s.abc" % (path.splitext(file_name)[0]))
            test_abc_path = path.join(file_absolute_path, test_abc_name)
            es2abc_cmd = runner.cmd_prefix + [runner.es2panda]
            es2abc_cmd.extend(self.flags)
            es2abc_cmd.extend(['%s%s' % ("--output=", test_abc_path)])
            es2abc_cmd.append(self.path)
            process = run_subprocess_with_beta3(self, es2abc_cmd)
            out, err = process.communicate()
            if err:
                self.passed = False
                self.error = err.decode("utf-8", errors="ignore")
                self.remove_project(runner)
                return self

    def collect_record_mapping(self):
        # Collect record mappings from recordnames.txt, file format:
        # 'source_file_name:record_name\n' * n
        if path.exists(self.record_names_path):
            with open(self.record_names_path) as mapping_fp:
                mapping_lines = mapping_fp.readlines()
                self.file_record_mapping = {}
                for mapping_line in mapping_lines:
                    cur_mapping = mapping_line[:-1].split(":")
                    self.file_record_mapping[cur_mapping[0]] = cur_mapping[1]

    def get_record_name(self, test_path):
        record_name = os.path.relpath(test_path, os.path.dirname(self.files_info_path)).split('.')[0]
        if (self.file_record_mapping is not None and record_name in self.file_record_mapping):
            record_name = self.file_record_mapping[record_name]
        return record_name

    def collect_abc_inputs(self, runner):
        # Collect abc input information from the 'abcinputs' directory. Each txt file in the directory
        # will generate a merged abc file with the same filename and serve as the final abc input.
        # file format: 'source_file_name.ts\n' * n
        if not path.exists(self.abc_inputs_path):
            return
        if not path.exists(self.generated_abc_inputs_path):
            os.makedirs(self.generated_abc_inputs_path)
        self.abc_input_filenames = {}
        filenames = os.listdir(self.abc_inputs_path)
        for filename in filenames:
            if not filename.endswith('.txt'):
                self.remove_project(runner)
                raise Exception("Invalid abc input file: %s, only txt files are allowed in abcinputs directory: %s"
                                % (filename, self.abc_inputs_path))
            with open(path.join(self.abc_inputs_path, filename)) as abc_inputs_fp:
                abc_inputs_lines = abc_inputs_fp.readlines()
                for abc_input_line in abc_inputs_lines:
                    # filename is 'xxx.txt', remove '.txt' here
                    self.abc_input_filenames[abc_input_line[:-1]] = filename[:-len('.txt')]

    def get_belonging_abc_input(self, test_path):
        filename = os.path.relpath(test_path, os.path.dirname(self.files_info_path))
        if (self.abc_input_filenames is not None and filename in self.abc_input_filenames):
            return self.abc_input_filenames[filename]
        return None

    def gen_abc_input_files_infos(self, runner, abc_files_infos, final_file_info_f, mod_files_info):
        for abc_files_info_name in abc_files_infos:
            abc_files_info = abc_files_infos[abc_files_info_name]
            if len(abc_files_info) != 0:
                abc_input_path = path.join(self.generated_abc_inputs_path, abc_files_info_name)
                abc_files_info_path = ("%s-filesInfo.txt" % (abc_input_path))
                abc_files_info_fd = os.open(abc_files_info_path, os.O_RDWR | os.O_CREAT | os.O_TRUNC)
                abc_files_info_f = os.fdopen(abc_files_info_fd, 'w')
                abc_files_info_f.writelines(abc_files_info)
                abc_line = '%s-abcinput.abc;;;;%s;\n' % (abc_input_path, abc_files_info_name)
                mod_files_info.append(abc_line)
                final_file_info_f.writelines(abc_line)


    def gen_files_info(self, runner):
        # After collect_record_mapping, self.file_record_mapping stores {'source file name' : 'source file record name'}
        self.collect_record_mapping()
        # After collect_abc_inputs, self.abc_input_filenames stores {'source file name' : 'belonging abc input name'}
        self.collect_abc_inputs(runner)

        fd = os.open(self.files_info_path, os.O_RDWR | os.O_CREAT | os.O_TRUNC)
        f = os.fdopen(fd, 'w')
        mod_files_info = []
        abc_files_infos = {}
        for test_path in self.test_paths:
            record_name = self.get_record_name(test_path)
            module_kind = 'esm'
            if (os.path.basename(test_path).startswith("commonjs")):
                module_kind = 'commonjs'
            is_shared_module = 'false'
            if (os.path.basename(test_path).startswith("sharedmodule")):
                is_shared_module = 'true'
            file_info = ('%s;%s;%s;%s;%s;%s\n' % (test_path, record_name, module_kind,
                                               os.path.relpath(test_path, self.projects_path), record_name,
                                               is_shared_module))
            belonging_abc_input = self.get_belonging_abc_input(test_path)
            if belonging_abc_input is not None:
                if not belonging_abc_input in abc_files_infos:
                    abc_files_infos[belonging_abc_input] = []
                abc_files_infos[belonging_abc_input].append(file_info)
            elif test_path.startswith(self.project_mod_path):
                mod_files_info.append(file_info)
            else:
                mod_files_info.append(file_info)
                f.writelines(file_info)
        if (os.path.exists(self.deps_json_path)):
            record_name = self.get_record_name(self.deps_json_path)
            file_info = ('%s;%s;%s;%s;%s;%s\n' % (self.deps_json_path, record_name, 'esm',
                                               os.path.relpath(self.deps_json_path, self.projects_path), record_name,
                                               'false'))
            f.writelines(file_info)
        self.gen_abc_input_files_infos(runner, abc_files_infos, f, mod_files_info)
        f.close()
        if (os.path.exists(self.project_mod_path)):
            mod_fd = os.open(self.files_info_mod_path, os.O_RDWR | os.O_CREAT | os.O_TRUNC)
            mod_f = os.fdopen(mod_fd, 'w')
            for file_line in mod_files_info:
                mod_f.writelines(file_line)
            mod_f.close()

    def gen_modules_cache(self, runner):
        if "--cache-file" not in self.flags or "--file-threads=0" in self.flags:
            return
        fd = os.open(self.modules_cache_path, os.O_RDWR | os.O_CREAT | os.O_TRUNC)
        f = os.fdopen(fd, 'w')
        abc_files = set()
        for test_path in self.test_paths:
            cache_info = ('%s;%s\n' % (test_path, f"{test_path.rsplit('.', 1)[0]}.protobin"))
            belonging_abc_input = self.get_belonging_abc_input(test_path)
            if belonging_abc_input is not None:
                abc_files.add(belonging_abc_input)
            else:
                f.writelines(cache_info)
        for abc_path in abc_files:
            abc_input_path = f"{path.join(self.generated_abc_inputs_path, abc_path)}-abcinput.abc"
            cache_info = ('%s;%s\n' % (abc_input_path, f"{abc_input_path.rsplit('.', 1)[0]}.protobin"))
            f.writelines(cache_info)
        f.close()

    def gen_es2abc_cmd(self, runner, input_file, output_file):
        es2abc_cmd = runner.cmd_prefix + [runner.es2panda]

        new_flags = self.flags
        if "--cache-file" in new_flags and len(self.test_paths) == 1:
            # Generate cache-file test case in single thread 
            new_flags.remove("--cache-file")
            protobin_path = f"{self.test_paths[0].rsplit('.', 1)[0]}.protobin"
            self.protoBin_file_path = protobin_path
            es2abc_cmd.append('--cache-file=%s' % (protobin_path))
        elif "--cache-file" in self.flags and output_file.endswith("-abcinput.abc"):
            # Generate abc for bytecode har
            new_flags = list(filter(lambda x: x != "--cache-file", new_flags))
        elif "--cache-file" in self.flags:
            new_flags = list(filter(lambda x: x != "--cache-file", new_flags))
            es2abc_cmd.append('--cache-file')
            es2abc_cmd.append('@%s' % (self.modules_cache_path))

        es2abc_cmd.extend(new_flags)
        es2abc_cmd.extend(['%s%s' % ("--output=", output_file)])
        es2abc_cmd.append(input_file)
        return es2abc_cmd

    def gen_merged_abc_for_abc_input(self, runner, files_info_name):
        self.passed = True
        if not files_info_name.endswith(".txt"):
            return
        abc_input_files_info_path = path.join(self.generated_abc_inputs_path, files_info_name)
        abc_input_merged_abc_path = path.join(self.generated_abc_inputs_path,
                                              '%s-abcinput.abc' % (files_info_name[:-len('-filesInfo.txt')]))

        abc_input_file_path = '@' + abc_input_files_info_path
        if "unmerged_abc_input" in self.generated_abc_inputs_path:
            self.flags.remove("--merge-abc")
            with open(abc_input_files_info_path, 'r') as fp:
                abc_input_file_path = fp.read().split(';')[0]

        es2abc_cmd = self.gen_es2abc_cmd(runner, abc_input_file_path, abc_input_merged_abc_path)
        process = run_subprocess_with_beta3(self, es2abc_cmd)
        out, err = process.communicate()
        if err:
            self.passed = False
            self.error = err.decode("utf-8", errors="ignore")

    def gen_merged_abc(self, runner):
        # Generate abc inputs
        if (os.path.exists(self.generated_abc_inputs_path)):
            files_info_names = os.listdir(self.generated_abc_inputs_path)
            for filename in files_info_names:
                self.gen_merged_abc_for_abc_input(runner, filename)
                if (not self.passed):
                    self.remove_project(runner)
                    return self
        # Generate the abc to be tested
        for test_path in self.test_paths:
            self.path = test_path
            if (self.path.endswith("-exec.ts")) or (self.path.endswith("-exec.js")):
                exec_file_path = self.path
                [file_absolute_path, file_name] = self.get_file_absolute_path_and_name(runner)
                if not path.exists(file_absolute_path):
                    os.makedirs(file_absolute_path)
                test_abc_name = ("%s.abc" % (path.splitext(file_name)[0]))
                output_abc_name = path.join(file_absolute_path, test_abc_name)
        if "merge_hap" in self.projects_path:
            exec_file_path = os.path.join(self.projects_path, self.project)
            exec_file_path = os.path.join(exec_file_path, "main_hap")
            [file_absolute_path, file_name] = self.get_file_absolute_path_and_name(runner)
            if not path.exists(file_absolute_path):
                os.makedirs(file_absolute_path)
            output_abc_name = path.join(file_absolute_path, "merge_hap.abc")
        # reverse merge-abc flag
        if "merge_abc_consistence_check" in self.path:
            if "--merge-abc" in self.flags:
                self.flags.remove("--merge-abc")
            else:
                self.flags.append("--merge-abc")

        es2abc_cmd = self.gen_es2abc_cmd(runner, '@' + self.files_info_path, output_abc_name)
        if "--cache-file" in self.flags and len(self.test_paths) == 1:
            es2abc_cmd = self.gen_es2abc_cmd(runner, self.test_paths[0], output_abc_name)
        else:
            es2abc_cmd = self.gen_es2abc_cmd(runner, '@' + self.files_info_path, output_abc_name)
        compile_context_info_path = path.join(path.join(self.projects_path, self.project), "compileContextInfo.json")
        if path.exists(compile_context_info_path):
            es2abc_cmd.append("%s%s" % ("--compile-context-info=", compile_context_info_path))
        if path.exists(self.modifyPkgNamePath):
            with open(self.modifyPkgNamePath, 'r') as file:
                modifyPkgName = file.readline().rstrip('\n')
                pkgNames = modifyPkgName.split(":")
                es2abc_cmd.append("--src-package-name=%s" % pkgNames[0])
                es2abc_cmd.append("--dst-package-name=%s" % pkgNames[1])
        process = run_subprocess_with_beta3(self, es2abc_cmd)
        self.path = exec_file_path
        out, err = [None, None]

        # Check single-thread execution timeout when required
        if "--file-threads=0" in self.flags:
            try:
                out, err = process.communicate(timeout=60)
            except:
                process.kill()
                print("Generating the abc file timed out.")
        else:
            out, err = process.communicate()
        
        if "--cache-file" in self.flags:
            # Firstly generate cache file, and generate abc from cache file
            if (os.path.exists(self.project_mod_path)):
                es2abc_cmd = self.gen_es2abc_cmd(runner, '@' + self.files_info_mod_path, output_abc_name)
                compile_context_info_path = path.join(path.join(self.projects_path, self.project), "compileContextInfo.json")
                if path.exists(compile_context_info_path):
                    es2abc_cmd.append("%s%s" % ("--compile-context-info=", compile_context_info_path))
                process = run_subprocess_with_beta3(self, es2abc_cmd)
                out, err = process.communicate()
            else:
                process = run_subprocess_with_beta3(self, es2abc_cmd)
                out, err = process.communicate()

        # restore merge-abc flag
        if "merge_abc_consistence_check" in self.path and "--merge-abc" not in self.flags:
            self.flags.append("--merge-abc")

        # Check dump-assembly outputs when required
        if "--dump-assembly" in self.flags:
            pa_expected_path = "".join([self.get_path_to_expected()[:self.get_path_to_expected().rfind(".txt")],
                                        ".pa.txt"])
            self.output = out.decode("utf-8", errors="ignore") + err.decode("utf-8", errors="ignore")
            if "merge_abc_consistence_check" in self.path:
                self.output = self.output.split('.')[0]
            try:
                with open(pa_expected_path, 'r') as fp:
                    expected = fp.read()
                self.passed = expected == self.output and process.returncode in [0, 1]
            except Exception:
                self.passed = False
            if not self.passed:
                self.error = err.decode("utf-8", errors="ignore")
                self.remove_project(runner)
                return self
            else:
                return self

        if err:
            self.passed = False
            self.error = err.decode("utf-8", errors="ignore")
            self.remove_project(runner)
            return self

    def run(self, runner):
        # Compile all ts source files in the project to abc files.
        if ("--merge-abc" in self.flags):
            self.gen_files_info(runner)
            self.gen_modules_cache(runner)
            self.gen_merged_abc(runner)
        else:
            self.gen_single_abc(runner)

        if (not self.requires_execution):
            self.remove_project(runner)
            return self

        # Run test files that need to be executed in the project.
        for test_path in self.test_paths:
            self.path = test_path
            if self.path.endswith("-exec.ts"):
                [file_absolute_path, file_name] = self.get_file_absolute_path_and_name(runner)

                entry_point_name = path.splitext(file_name)[0]
                test_abc_name = ("%s.abc" % entry_point_name)
                test_abc_path = path.join(file_absolute_path, test_abc_name)

                ld_library_path = runner.ld_library_path
                os.environ.setdefault("LD_LIBRARY_PATH", ld_library_path)
                run_abc_cmd = [runner.ark_js_vm]
                if ("--merge-abc" in self.flags):
                    run_abc_cmd.extend(["--entry-point", entry_point_name])
                run_abc_cmd.extend([test_abc_path])
                self.log_cmd(run_abc_cmd)

                process = subprocess.Popen(run_abc_cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
                out, err = process.communicate()
                self.output = out.decode("utf-8", errors="ignore") + err.decode("utf-8", errors="ignore")
                expected_path = self.get_path_to_expected()
                try:
                    with open(expected_path, 'r') as fp:
                        expected = fp.read()
                    self.passed = expected == self.output and process.returncode in [0, 1]
                except Exception:
                    self.passed = False

                if not self.passed:
                    self.error = err.decode("utf-8", errors="ignore")
                    self.remove_project(runner)
                    return self

            self.passed = True

        self.remove_project(runner)
        return self


class FilesInfoRunner(Runner):
    def __init__(self, args):
        Runner.__init__(self, args, "FilesInfo")

    def add_directory(self, directory, extension, flags):
        projects_path = path.join(self.test_root, directory)
        test_projects = ["base", "mod"]
        for project in test_projects:
            filesinfo_path = path.join(projects_path, project, "filesInfo.txt")
            self.tests.append(FilesInfoTest(projects_path, project, filesinfo_path, flags))

    def test_path(self, src):
        return src


class FilesInfoTest(Test):
    def __init__(self, projects_path, project, filesinfo_path, flags):
        Test.__init__(self, "", flags)
        self.projects_path = projects_path
        self.output_path = path.join(projects_path, "output")
        self.project = project
        self.origin_filesinfo_path = filesinfo_path
        self.files_info_path = path.join(self.output_path, self.project + "filesInfo.txt")
        self.path = path.join(self.projects_path, self.project)
        self.symbol_table_file = os.path.join(self.output_path, 'base.map')
        self.output_abc_name = path.join(self.output_path, self.project + ".abc")
        self.output_abc_name_of_input_abc = path.join(self.output_path, self.project + "_input.abc")

        if not path.exists(self.output_path):
            os.makedirs(self.output_path)

    def gen_files_info(self):
        with open(self.origin_filesinfo_path, 'r') as src, open(self.files_info_path, 'w') as dst:
            for line in src:
                dst.write(f"{path.join(self.projects_path, self.project)}/{line}")

    def remove_output(self):
        shutil.rmtree(self.output_path)

    def remove_project(self, runner):
        if self.project == "mod": # clear after all tests
            self.remove_output()

    def gen_es2abc_cmd(self, runner, input_file, output_file):
        es2abc_cmd = runner.cmd_prefix + [runner.es2panda]
        es2abc_cmd.extend(self.flags)
        es2abc_cmd.extend(['%s%s' % ("--output=", output_file)])
        es2abc_cmd.append(input_file)
        if ("base" == self.project):
            es2abc_cmd.extend(['--dump-symbol-table', self.symbol_table_file])
        else:
            es2abc_cmd.extend(['--input-symbol-table', self.symbol_table_file])
        return es2abc_cmd

    def gen_es2abc_cmd_input_abc(self, runner, input_file, output_file):
        es2abc_cmd = runner.cmd_prefix + [runner.es2panda]
        es2abc_cmd.extend(self.flags)
        es2abc_cmd.extend(['%s%s' % ("--output=", output_file), "--enable-abc-input"])
        es2abc_cmd.append(input_file)
        return es2abc_cmd

    def gen_merged_abc(self, runner):
        # Generate the abc to be tested
        es2abc_cmd = self.gen_es2abc_cmd(runner, '@' + self.files_info_path, self.output_abc_name)
        process = run_subprocess_with_beta3(self, es2abc_cmd)
        out, err = process.communicate()

        # Gen abc and verify it
        pa_expected_path = "".join([self.get_path_to_expected()[:self.get_path_to_expected().rfind(".txt")],
                                    ".pa.txt"])
        self.output = out.decode("utf-8", errors="ignore") + err.decode("utf-8", errors="ignore")
        try:
            with open(pa_expected_path, 'r') as fp:
                expected = fp.read()
            self.passed = expected == self.output and process.returncode in [0, 1]
        except Exception:
            self.passed = False
        if not self.passed:
            self.error = err.decode("utf-8", errors="ignore")
            return self

        # Input abc and verify it when it is base.
        if self.project == "base":
            self.input_abc(runner)
        return self

    def input_abc(self, runner):
        es2abc_cmd = self.gen_es2abc_cmd_input_abc(runner, self.output_abc_name, self.output_abc_name_of_input_abc)
        process = run_subprocess_with_beta3(self, es2abc_cmd)
        out, err = process.communicate()
        pa_expected_path = "".join([self.path, "input_base-expected.pa.txt"])
        self.output = out.decode("utf-8", errors="ignore") + err.decode("utf-8", errors="ignore")
        try:
            with open(pa_expected_path, 'r') as fp:
                expected = fp.read()
            self.passed = expected == self.output and process.returncode in [0, 1]
        except Exception:
            self.passed = False
        if not self.passed:
            self.error = err.decode("utf-8", errors="ignore")
            return self


    def run(self, runner):
        self.gen_files_info()
        self.gen_merged_abc(runner)
        self.remove_project(runner)
        return self


class TSDeclarationTest(Test):
    def get_path_to_expected(self):
        file_name = self.path[:self.path.find(".d.ts")]
        return "%s-expected.txt" % file_name


class BcVersionRunner(Runner):
    def __init__(self, args):
        Runner.__init__(self, args, "Target bc version")
        self.ts2abc = path.join(self.test_root, '..', 'scripts', 'ts2abc.js')

    def add_cmd(self):
        api_sub_version_list = ["beta1", "beta2", "beta3"]
        for api_version in range(8, 17):
            cmd = self.cmd_prefix + [self.es2panda]
            cmd += ["--target-bc-version"]
            cmd += ["--target-api-version"]
            cmd += [str(api_version)]
            self.tests += [BcVersionTest(cmd, api_version)]
            node_cmd = ["node"] + [self.ts2abc]
            node_cmd += ["".join(["es2abc=", self.es2panda])]
            node_cmd += ["--target-api-version"]
            node_cmd += [str(api_version)]
            self.tests += [BcVersionTest(node_cmd, api_version)]

            # Add tests for "--target-api-sub-version" option
            if api_version == 12:
                for api_sub_version in api_sub_version_list:
                    new_cmd = cmd.copy()
                    new_cmd += ["--target-api-sub-version", api_sub_version]
                    self.tests += [BcVersionTest(new_cmd, str(api_version) + '_' + api_sub_version)]
                    new_node_cmd = node_cmd.copy()
                    new_node_cmd += ["--target-api-sub-version", api_sub_version]
                    self.tests += [BcVersionTest(new_node_cmd, str(api_version) + '_' + api_sub_version)]

    def run(self):
        for test in self.tests:
            test.run()


class BcVersionTest(Test):
    def __init__(self, cmd, api_version):
        Test.__init__(self, "", 0)
        self.cmd = cmd
        self.api_version = api_version
        # To avoid problems when api version is upgraded abruptly,
        # the corresponding bytecode version of the api version not written in isa.yaml is alaways the newest version.
        self.bc_version_expect = {
            8: "13.0.1.0",
            9: "9.0.0.0",
            10: "9.0.0.0",
            11: "11.0.2.0",
            12: "12.0.2.0",
            "12_beta1": "12.0.2.0",
            "12_beta2": "12.0.2.0",
            "12_beta3": "12.0.6.0",
            13: "12.0.6.0",
            14: "12.0.6.0",
            15: "12.0.6.0",
            16: "12.0.6.0",
            17: "12.0.6.0",
            18: "13.0.1.0",
            19: "13.0.1.0",
            20: "13.0.1.0",
        }
        self.es2abc_script_expect = {
            8: "0.0.0.2",
            9: "9.0.0.0",
            10: "9.0.0.0",
            11: "11.0.2.0",
            12: "12.0.2.0",
            "12_beta1": "12.0.2.0",
            "12_beta2": "12.0.2.0",
            "12_beta3": "12.0.6.0",
            13: "12.0.6.0",
            14: "12.0.6.0",
            15: "12.0.6.0",
            16: "12.0.6.0",
            17: "12.0.6.0",
            18: "13.0.1.0",
            19: "13.0.1.0",
            20: "13.0.1.0",
        }

    def run(self):
        process = subprocess.Popen(self.cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        out, err = process.communicate()
        self.output = out.decode("utf-8", errors="ignore") + err.decode("utf-8", errors="ignore")
        if self.cmd[0] == "node":
            self.passed = self.es2abc_script_expect.get(self.api_version) == self.output and process.returncode in [0, 1]
        else:
            self.passed = self.bc_version_expect.get(self.api_version) == self.output and process.returncode in [0, 1]
        if not self.passed:
            self.error = err.decode("utf-8", errors="ignore")
        return self


class TransformerRunner(Runner):
    def __init__(self, args):
        Runner.__init__(self, args, "Transformer")

    def add_directory(self, directory, extension, flags):
        glob_expression = path.join(
            self.test_root, directory, "**/*.%s" % (extension))
        files = glob(glob_expression, recursive=True)
        files = fnmatch.filter(files, self.test_root + '**' + self.args.filter)

        self.tests += list(map(lambda f: TransformerTest(f, flags), files))

    def test_path(self, src):
        return src


class TransformerInTargetApiVersion10Runner(Runner):
    def __init__(self, args):
        Runner.__init__(self, args, "TransformerInTargetApiVersion10")

    def add_directory(self, directory, extension, flags):
        glob_expression = path.join(
            self.test_root, directory, "**/*.%s" % (extension))
        files = glob(glob_expression, recursive=True)
        files = fnmatch.filter(files, self.test_root + '**' + self.args.filter)

        self.tests += list(map(lambda f: TransformerTest(f, flags), files))

    def test_path(self, src):
        return src


class TransformerTest(Test):
    def __init__(self, test_path, flags):
        Test.__init__(self, test_path, flags)

    def get_path_to_expected(self):
        return "%s-transformed-expected.txt" % (path.splitext(self.path)[0])

    def run(self, runner):
        cmd = runner.cmd_prefix + [runner.es2panda]
        cmd.extend(self.flags)
        cmd.append(self.path)
        process = run_subprocess_with_beta3(self, cmd)
        out, err = process.communicate()
        self.output = out.decode("utf-8", errors="ignore") + err.decode("utf-8", errors="ignore")

        expected_path = self.get_path_to_expected()
        try:
            with open(expected_path, 'r') as fp:
                expected = fp.read()
            self.passed = expected == self.output and process.returncode in [0, 1]
        except Exception:
            self.passed = False

        if not self.passed:
            self.error = err.decode("utf-8", errors="ignore")

        return self


class PatchTest(Test):
    def __init__(self, test_path, mode_arg, target_version, preserve_files):
        Test.__init__(self, test_path, "")
        self.mode = mode_arg
        self.target_version = target_version
        self.preserve_files = preserve_files

    def is_ts_test_cases(self):
        return 'ts_test_cases' in os.path.normpath(self.path).split(os.sep)

    def need_to_merge(self):
        # If the test case is in the 'export and import' directory, it needs to be merged.
        if os.path.split(self.path)[-2] == 'export-and-import':
            return True
        else:
            return False

    def gen_files_infos(self, modified=False):
        if not self.need_to_merge():
            return

        file_suffix = '.ts' if self.is_ts_test_cases() else '.js'
        file_name_list = []
        for file_name in os.listdir(self.path):
            if file_name.endswith(file_suffix):
                if '_mod' in file_name and modified:
                    file_name_list.append(file_name)
                elif not '_mod' in file_name and not modified:
                    file_name_list.append(file_name)

        files_info_txt = os.path.join(self.path, 'filesInfo.txt')
        with open(files_info_txt, 'w', encoding='utf-8') as file:
            for file_name in file_name_list:
                file_path = os.path.join(self.path, file_name)
                file_prev, file_extension = os.path.splitext(file_name)
                abc_file_path = os.path.join(self.path, f"{file_prev}.abc")
                file.write(f'{file_path};{file_name};{file_name};esm;{file_name};false\n')

    def gen_cmd(self, runner):
        symbol_table_file = os.path.join(self.path, 'base.map')
        origin_input_file = 'base.js' if not self.is_ts_test_cases() else 'base.ts'
        origin_output_abc = os.path.join(self.path, 'base.abc')
        modified_input_file = 'base_mod.js' if not self.is_ts_test_cases() else 'base_mod.ts'
        modified_output_abc = os.path.join(self.path, 'patch.abc')
        files_info_txt = os.path.join(self.path, 'filesInfo.txt')
        target_version_cmd = ""
        if self.target_version > 0:
            target_version_cmd = "--target-api-version=" + str(self.target_version)

        gen_base_cmd = runner.cmd_prefix + [runner.es2panda, '--module', target_version_cmd]
        if self.need_to_merge():
            gen_base_cmd = gen_base_cmd.append('--merge-abc')
        if 'record-name-with-dots' in os.path.basename(self.path):
            gen_base_cmd.extend(['--merge-abc', '--record-name=record.name.with.dots'])
        gen_base_cmd.extend(['--dump-symbol-table', symbol_table_file])
        gen_base_cmd.extend(['--output', origin_output_abc])
        if not self.need_to_merge():
            gen_base_cmd.extend([os.path.join(self.path, origin_input_file)])
        else:
            gen_base_cmd.extend(['--debug-info', f'@{files_info_txt}'])
        self.log_cmd(gen_base_cmd)

        if self.mode == 'hotfix':
            mode_arg = ["--generate-patch"]
        elif self.mode == 'hotreload':
            mode_arg = ["--hot-reload"]
        elif self.mode == 'coldfix':
            mode_arg = ["--generate-patch", "--cold-fix"]
        elif self.mode == 'coldreload':
            mode_arg = ["--cold-reload"]

        patch_test_cmd = runner.cmd_prefix + [runner.es2panda, '--module', target_version_cmd]
        if self.need_to_merge():
            patch_test_cmd = patch_test_cmd.append('--merge-abc')
        patch_test_cmd.extend(mode_arg)
        patch_test_cmd.extend(['--input-symbol-table', symbol_table_file])
        patch_test_cmd.extend(['--output', modified_output_abc])
        if not self.need_to_merge():
            patch_test_cmd.extend([os.path.join(self.path, modified_input_file)])
        else:
            patch_test_cmd.extend(['--debug-info', f'@{files_info_txt}'])
        if 'record-name-with-dots' in os.path.basename(self.path):
            patch_test_cmd.extend(['--merge-abc', '--record-name=record.name.with.dots'])
        dump_assembly_testname = [
            'modify-anon-content-keep-origin-name',
            'modify-class-memeber-function',
            'exist-lexenv-3',
            'lexenv-reduce',
            'lexenv-increase']
        for name in dump_assembly_testname:
            if name in os.path.basename(self.path):
                patch_test_cmd.extend(['--dump-assembly'])
        self.log_cmd(patch_test_cmd)

        return gen_base_cmd, patch_test_cmd, symbol_table_file, origin_output_abc, modified_output_abc

    def run(self, runner):
        gen_base_cmd, patch_test_cmd, symbol_table_file, origin_output_abc, modified_output_abc = self.gen_cmd(runner)

        self.gen_files_infos(False)
        process_base = run_subprocess_with_beta3(None, gen_base_cmd)
        stdout_base, stderr_base = process_base.communicate(timeout=runner.args.es2panda_timeout)
        if stderr_base:
            self.passed = False
            self.error = stderr_base.decode("utf-8", errors="ignore")
            self.output = stdout_base.decode("utf-8", errors="ignore") + stderr_base.decode("utf-8", errors="ignore")
        else:
            self.gen_files_infos(True)
            process_patch = run_subprocess_with_beta3(None, patch_test_cmd)
            process_patch = subprocess.Popen(patch_test_cmd, stdout=subprocess.PIPE,
                stderr=subprocess.PIPE)
            stdout_patch, stderr_patch = process_patch.communicate(timeout=runner.args.es2panda_timeout)
            if stderr_patch:
                self.passed = False
                self.error = stderr_patch.decode("utf-8", errors="ignore")
            self.output = stdout_patch.decode("utf-8", errors="ignore") + stderr_patch.decode("utf-8", errors="ignore")

        expected_path = os.path.join(self.path, 'expected.txt')
        try:
            with open(expected_path, 'r') as fp:
                # ignore license description lines and skip leading blank lines
                expected = (''.join((fp.readlines()[12:]))).lstrip()
            self.passed = expected == self.output
        except Exception:
            self.passed = False

        if not self.passed:
            self.error = "expected output:" + os.linesep + expected + os.linesep + "actual output:" + os.linesep +\
                self.output
        files_info_txt = os.path.join(self.path, 'filesInfo.txt')
        if not self.preserve_files:
            os.remove(symbol_table_file)
            os.remove(origin_output_abc)
            if (os.path.exists(modified_output_abc)):
                os.remove(modified_output_abc)
            if (os.path.exists(files_info_txt)):
                os.remove(files_info_txt)
        return self


class PatchRunner(Runner):
    def __init__(self, args, name):
        Runner.__init__(self, args, name)
        self.preserve_files = args.error
        self.tests_in_dirs = []
        dirs = os.listdir(path.join(self.test_root, "patch"))
        for target_version_path in dirs:
            self.add_tests(target_version_path, name)

    def add_tests(self, target_version_path, name):
        name_dir = os.path.join(self.test_root, "patch", target_version_path, name)
        if not os.path.exists(name_dir):
            return
        target_version = 0
        if target_version_path.isdigit():
            target_version = int(target_version_path)
        for sub_path in os.listdir(name_dir):
            test_base_path = os.path.join(name_dir, sub_path)
            if name != "coldreload":
                for dirpath, dirnames, filenames in os.walk(test_base_path):
                    if not dirnames:
                        self.tests_in_dirs.append(dirpath)
                        self.tests.append(PatchTest(dirpath, name, target_version, self.preserve_files))
            else:
                self.tests_in_dirs.append(test_base_path)
                self.tests.append(PatchTest(test_base_path, name, target_version, self.preserve_files))

    def test_path(self, src):
        return os.path.basename(src)


class HotfixRunner(PatchRunner):
    def __init__(self, args):
        PatchRunner.__init__(self, args, "hotfix")


class HotreloadRunner(PatchRunner):
    def __init__(self, args):
        PatchRunner.__init__(self, args, "hotreload")


class ColdfixRunner(PatchRunner):
    def __init__(self, args):
        PatchRunner.__init__(self, args, "coldfix")


class ColdreloadRunner(PatchRunner):
    def __init__(self, args):
        PatchRunner.__init__(self, args, "coldreload")


class DebuggerTest(Test):
    def __init__(self, test_path, mode):
        Test.__init__(self, test_path, "")
        self.mode = mode

    def run(self, runner):
        cmd = runner.cmd_prefix + [runner.es2panda, "--module"]
        input_file_name = 'base.js'
        if self.mode == "debug-mode":
            cmd.extend(['--debug-info'])
        cmd.extend([os.path.join(self.path, input_file_name)])
        cmd.extend(['--dump-assembly'])
        process = run_subprocess_with_beta3(self, cmd)
        stdout, stderr = process.communicate(timeout=runner.args.es2panda_timeout)
        if stderr:
            self.passed = False
            self.error = stderr.decode("utf-8", errors="ignore")
            return self

        self.output = stdout.decode("utf-8", errors="ignore")

        expected_path = os.path.join(self.path, 'expected.txt')
        try:
            with open(expected_path, 'r') as fp:
                expected = (''.join((fp.readlines()[12:]))).lstrip()
            self.passed = expected == self.output
        except Exception:
            self.passed = False

        if not self.passed:
            self.error = "expected output:" + os.linesep + expected + os.linesep + "actual output:" + os.linesep +\
                self.output

        if os.path.exists("base.abc"):
            os.remove("base.abc")

        return self


class DebuggerRunner(Runner):
    def __init__(self, args):
        Runner.__init__(self, args, "debugger")
        self.test_directory = path.join(self.test_root, "debugger")
        self.add_test()

    def add_test(self):
        self.tests = []
        self.tests.append(DebuggerTest(os.path.join(self.test_directory, "debugger-in-debug"), "debug-mode"))
        self.tests.append(DebuggerTest(os.path.join(self.test_directory, "debugger-in-release"), "release-mode"))


class Base64Test(Test):
    def __init__(self, test_path, input_type):
        Test.__init__(self, test_path, "")
        self.input_type = input_type

    def run(self, runner):
        cmd = runner.cmd_prefix + [runner.es2panda, "--base64Output"]
        if self.input_type == "file":
            input_file_name = 'input.js'
            cmd.extend(['--source-file', input_file_name])
            cmd.extend([os.path.join(self.path, input_file_name)])
        elif self.input_type == "string":
            input_file = os.path.join(self.path, "input.txt")
            try:
                with open(input_file, 'r') as fp:
                    base64_input = (''.join((fp.readlines()[12:]))).lstrip()  # ignore license description lines
                    cmd.extend(["--base64Input", base64_input])
            except Exception:
                self.passed = False
        elif self.input_type == "targetApiVersion":
            # base64 test for all available target api version.
            version = os.path.basename(self.path)
            cmd.extend(['--target-api-version', version])
            if version == "12":
                cmd.append("--target-api-sub-version=beta3")
            input_file = os.path.join(self.path, "input.txt")
            try:
                with open(input_file, 'r') as fp:
                    base64_input = (''.join((fp.readlines()[12:]))).lstrip()  # ignore license description lines
                    cmd.extend(["--base64Input", base64_input])
            except Exception:
                self.passed = False
        else:
            self.error = "Unsupported base64 input type"
            self.passed = False
            return self

        version = os.path.basename(self.path)
        if not self.input_type == "targetApiVersion":
            cmd.append("--target-api-sub-version=beta3")

        self.log_cmd(cmd)

        process = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        stdout, stderr = process.communicate(timeout=runner.args.es2panda_timeout)
        if stderr:
            self.passed = False
            self.error = stderr.decode("utf-8", errors="ignore")
            return self

        self.output = stdout.decode("utf-8", errors="ignore")

        expected_path = os.path.join(self.path, 'expected.txt')
        try:
            with open(expected_path, 'r') as fp:
                expected = (''.join((fp.readlines()[12:]))).lstrip()
            self.passed = expected == self.output
        except Exception:
            self.passed = False

        if not self.passed:
            self.error = "expected output:" + os.linesep + expected + os.linesep + "actual output:" + os.linesep +\
                self.output

        return self


class Base64Runner(Runner):
    def __init__(self, args):
        Runner.__init__(self, args, "Base64")
        self.test_directory = path.join(self.test_root, "base64")
        self.add_test()

    def add_test(self):
        self.tests = []
        self.tests.append(Base64Test(os.path.join(self.test_directory, "inputFile"), "file"))
        self.tests.append(Base64Test(os.path.join(self.test_directory, "inputString"), "string"))
        # current target api version is 12, once a new version is addded, a new testcase should be added here.
        current_version = 12
        available_target_api_versions = [9, 10, 11, current_version]
        for version in available_target_api_versions:
            self.tests.append(Base64Test(os.path.join(self.test_directory, "availableTargetApiVersion", str(version)),
                "targetApiVersion"))

    def test_path(self, src):
        return os.path.basename(src)


class BytecodeRunner(Runner):
    def __init__(self, args):
        Runner.__init__(self, args, "Bytecode")

    def add_directory(self, directory, extension, flags, func=Test):
        glob_expression = path.join(
            self.test_root, directory, "**/*.%s" % (extension))
        files = glob(glob_expression, recursive=True)
        files = fnmatch.filter(files, self.test_root + '**' + self.args.filter)
        self.tests += list(map(lambda f: func(f, flags), files))

    def test_path(self, src):
        return src


class ArkJsVmDownload:  # Obtain different versions of ark_js_vm and their dependent libraries
    def __init__(self, args):
        self.build_dir = args.build_dir
        self.url = "https://gitee.com/zhongmingwei123123/ark_js_vm_version.git"
        self.local_path = path.join(self.build_dir, "ark_js_vm_version")
        self.max_retries = 3

    def run_cmd_cwd(self, cmd):
        try:
            proc = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            _, _ = proc.communicate()
            return proc.wait()
        except Exception as e:
            print(f"Error executing command: {e}")
            return -1

    def git_clone(self, git_url, code_dir):
        cmd = ["git", "clone", git_url, code_dir, "--depth=1"]
        retries = 1
        while retries <= self.max_retries:
            ret = self.run_cmd_cwd(cmd)
            if ret == 0:
                break
            else:
                print(f"\n warning: Atempt: #{retries} to clone '{git_url}' failed. Try cloining again")
                retries += 1
        assert not ret, f"\n error: Cloning '{git_url}' failed."

    def run(self):
        if not os.path.exists(self.local_path):
            print("\nstart downLoad ark_js_vm_version ...\n")
            self.git_clone(self.url, self.local_path)
            print("\ndownload finish.\n")


class AbcTestCasesPrepare:
    def __init__(self, args):
        self.test_root = path.dirname(path.abspath(__file__))
        self.es2panda = path.join(args.build_dir, "es2abc")
        self.args = args
        self.valid_mode_list = ["non_merge_mode", "merge_mode"]
        self.test_abc_path_list = set()

    @staticmethod
    def split_api_version(version_str):
        parts = version_str.split("API")[1].split("beta")
        main_part = parts[0]
        beta_part = "beta%s" % parts[1] if len(parts) > 1 else ""
        return (main_part, beta_part)

    def add_abc_directory(self, directory, extension):
        test_directory = path.join(self.test_root, directory)
        glob_expression = path.join(test_directory, "*.%s" % (extension))
        files = glob(glob_expression)
        files = fnmatch.filter(files, self.test_root + "**" + self.args.filter)
        return files

    def gen_abc_versions(self, flags, source_path):
        for api_version in API_VERSION_MAP:
            main_version, beta_version = AbcTestCasesPrepare.split_api_version(api_version)
            output_path = "%s_version_API%s%s.abc" % (
                path.splitext(source_path)[0],
                main_version,
                beta_version,
            )
            self.test_abc_path_list.add(output_path)
            _, stderr = self.compile_for_target_version(flags, source_path, output_path, main_version, beta_version)
            if stderr:
                raise RuntimeError(f"abc generate error: " % (stderr.decode("utf-8", errors="ignore")))

    def gen_abc_tests(self, directory, extension, flags, abc_mode):
        if abc_mode not in self.valid_mode_list:
            raise ValueError(f"Invalid abc_mode value: {abc_mode}")
        test_source_list = self.add_abc_directory(directory, extension)
        for input_path in test_source_list:
            self.gen_abc_versions(flags, input_path)

    def compile_for_target_version(self, flags, input_path, output_path, target_api_version, target_api_sub_version=""):
        cmd = []
        cmd.append(self.es2panda)
        cmd.append(input_path)
        cmd.extend(flags)
        cmd.append("--target-api-version=%s" % (target_api_version))
        cmd.extend(["--output=%s" % (output_path)])
        if target_api_version != "":
            cmd.append("--target-api-sub-version=%s" % (target_api_sub_version))
        process = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        stdout, stderr = process.communicate(timeout=10)
        if stderr:
            stderr = "Error executing command: %s\n%s" % (cmd, stderr)
        return stdout, stderr

    def remove_abc_tests(self):
        for abc_path in self.test_abc_path_list:
            if path.exists(abc_path):
                os.remove(abc_path)


class AbcVersionControlRunner(Runner):
    def __init__(self, args):
        super().__init__(args, "AbcVersionControl")
        self.valid_mode_list = ["non_merge_mode", "merge_mode", "mix_compile_mode"]

    def add_directory(self, directory, extension, flags, abc_mode, is_discard=False):
        if abc_mode not in self.valid_mode_list:
            raise ValueError(f"Invalid abc_mode value: {abc_mode}")
        glob_expression = path.join(self.test_root, directory, "*.%s" % (extension))
        files = glob(glob_expression)
        files = fnmatch.filter(files, self.test_root + "**" + self.args.filter)
        if abc_mode == "mix_compile_mode":
            files = [f for f in files if not f.endswith("-expected.txt")]
        self.tests += list(map(lambda f: TestAbcVersionControl(f, flags, abc_mode, is_discard), files))

    def test_path(self, src):
        return src

    def run(self):
        for test in self.tests:
            test.run(self)
        self.args.abc_tests_prepare.remove_abc_tests()


class VersionControlRunner(Runner):
    def __init__(self, args):
        Runner.__init__(self, args, "VersionControl")

    def add_directory(self, directory, extension, flags, test_version, feature_type, module_dir=None, func=Test):
        glob_expression = path.join(self.test_root, directory, "*.%s" % (extension))
        files = glob(glob_expression)
        files = fnmatch.filter(files, self.test_root + "**" + self.args.filter)
        module_path_list = []
        if module_dir is not None:
            module_path_list = self.add_module_path(module_dir)
        self.tests += list(
            map(lambda f: TestVersionControl(f, flags, test_version, feature_type, module_path_list), files)
        )

    def add_module_path(self, module_dir):
        module_path_list = []
        glob_expression_ts = path.join(self.test_root, module_dir, "*.%s" % ("ts"))
        glob_expression_js = path.join(self.test_root, module_dir, "*.%s" % ("js"))
        module_path_list = glob(glob_expression_ts)
        module_path_list.extend(glob(glob_expression_js))
        module_path_list = fnmatch.filter(module_path_list, self.test_root + "**" + self.args.filter)
        return module_path_list

    def test_path(self, src):
        return src

    def run(self):
        for test in self.tests:
            test.run(self)


class TestAbcVersionControl(Test):
    def __init__(self, test_path, flags, abc_mode, is_discard):
        super().__init__(test_path, flags)
        self.min_support_version_number = API_VERSION_MAP.get(MIN_SUPPORT_BC_VERSION)
        self.abc_mode = abc_mode
        self.is_discard = is_discard
        self.output = None
        self.process = None
        self.is_support = False
        self.test_abc_list = list()
        self.test_input = None
        self.target_abc_path = None
        self.entry_point = self.get_entry_point()

    @staticmethod
    def compare_version_number(version1, version2):
        v1 = TestAbcVersionControl.version_number_to_tuple(version1)
        v2 = TestAbcVersionControl.version_number_to_tuple(version2)
        for num1, num2 in zip(v1, v2):
            if num1 > num2:
                return 1
            elif num1 < num2:
                return -1
        return 0

    @staticmethod
    def version_number_to_tuple(version):
        return tuple(int(part) for part in version.split("."))

    def get_entry_point(self):
        if self.abc_mode == "merge_mode":
            base_name = os.path.basename(self.path)
            return os.path.splitext(base_name)[0]
        elif self.abc_mode == "mix_compile_mode":
            return MIX_COMPILE_ENTRY_POINT
        return ""

    def get_path_to_expected(self, is_support=False, test_stage=""):
        support_name = "supported_" if is_support else "unsupported_"
        if self.abc_mode == "mix_compile_mode" and test_stage != "runtime":
            support_name = ""
        expected_name = path.splitext(self.path)[0].split("_version_API")[0]
        expected_path = "%s_%s%s-expected.txt" % (expected_name, support_name, test_stage)
        return expected_path

    def run_process(self, cmd):
        self.process = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        stdout, stderr = self.process.communicate()
        self.output = stdout.decode("utf-8", errors="ignore") + stderr.decode("utf-8", errors="ignore").split("\n")[0]
        if stderr:
            stderr = "Error executing command: %s\n%s" % (cmd, stderr)
        return stdout, stderr

    def compile_for_target_version(
        self, runner, input_path, output_path, target_api_version, target_api_sub_version=""
    ):
        cmd = []
        cmd.append(runner.es2panda)
        if self.abc_mode == "mix_compile_mode":
            input_path = "@%s" % (input_path)
        cmd.append(input_path)
        cmd.extend(self.flags)
        cmd.append("--target-api-version=%s" % (target_api_version))
        cmd.extend(["--output=%s" % (output_path)])
        if target_api_version != "":
            cmd.append("--target-api-sub-version=%s" % (target_api_sub_version))
        stdout, stderr = self.run_process(cmd)
        return stdout, stderr

    def generate_abc(self, runner, target_api_version, target_api_sub_version=""):
        compile_expected_path = None
        target_abc_name = (
            "%s_target_%s%s.abc" % (path.splitext(self.path)[0], target_api_version, target_api_sub_version)
        ).replace("/", "_")
        self.target_abc_path = path.join(runner.build_dir, target_abc_name)
        _, stderr = self.compile_for_target_version(
            runner, self.path, self.target_abc_path, target_api_version, target_api_sub_version
        )
        format_content = ""
        self.is_support = False

        # Extract the API versions of the input abc files from the file name of the test case.
        input_api_versions = self.extract_api_versions(path.split(self.path)[1])
        input_version_numbers = [API_VERSION_MAP.get(api) for api in input_api_versions]
        sorted(input_version_numbers, key=TestAbcVersionControl.version_number_to_tuple)
        min_input_version_number = input_version_numbers[0]
        max_input_version_number = input_version_numbers[-1]
        target_version = "API" + target_api_version + target_api_sub_version
        target_version_number = API_VERSION_MAP.get(target_version)

        if TestAbcVersionControl.compare_version_number(target_version_number, self.min_support_version_number) < 0:
            compile_expected_path = self.get_path_to_expected(
                self.is_support, "compile_target_version_below_min_support"
            )
            format_content = target_api_version
        elif (
            TestAbcVersionControl.compare_version_number(min_input_version_number, self.min_support_version_number) < 0
        ):
            compile_expected_path = self.get_path_to_expected(self.is_support, "compile_cur_version_below_min_support")
            format_content = self.path
        elif TestAbcVersionControl.compare_version_number(target_version_number, max_input_version_number) < 0:
            compile_expected_path = self.get_path_to_expected(self.is_support, "compile_target_version_below_cur")
            format_content = self.path
        elif self.is_discard:
            compile_expected_path = self.get_path_to_expected(self.is_support, "compile_discard")
        else:
            self.is_support = True
            if stderr:
                self.passed = False
            return stderr

        try:
            with open(compile_expected_path, "r") as fp:
                expected = fp.read()
                self.passed = expected.format(format_content) in self.output and self.process.returncode in [0, 1]
        except Exception:
            self.passed = False
        return stderr

    def execute_abc(self, runner, vm_version, entry_point=""):
        cmd = []
        ark_js_vm_dir = os.path.join(
            runner.build_dir,
            "ark_js_vm_version",
            vm_version,
        )
        ld_library_path = os.path.join(ark_js_vm_dir, "lib")
        os.environ["LD_LIBRARY_PATH"] = ld_library_path
        ark_js_vm_path = os.path.join(ark_js_vm_dir, "ark_js_vm")
        cmd.append(ark_js_vm_path)
        if entry_point != "":
            cmd.append("--entry-point=%s" % entry_point)
        cmd.append(self.target_abc_path)
        stdout, stderr = self.run_process(cmd)
        return stdout, stderr

    def test_abc_execution(self, runner, target_api_version, target_api_sub_version=""):
        stderr = None
        target_version = "API" + target_api_version + target_api_sub_version
        target_version_number = API_VERSION_MAP.get(target_version)
        for vm_version in ARK_JS_VM_LIST:
            vm_version_number = API_VERSION_MAP.get(vm_version)
            _, stderr = self.execute_abc(runner, vm_version, self.entry_point)
            self.is_support = (
                TestAbcVersionControl.compare_version_number(vm_version_number, target_version_number) >= 0
            )
            runtime_expect_path = self.get_path_to_expected(self.is_support, "runtime")
            try:
                with open(runtime_expect_path, "r") as fp:
                    expected = fp.read()
                    if self.is_support and self.abc_mode != "merge_mode":
                        self.passed = expected == self.output and self.process.returncode in [0, 1, 255]
                    else:
                        self.passed = expected in self.output
                    pass
            except Exception:
                self.passed = False
            if not self.passed:
                return stderr
        return stderr

    def extract_api_versions(self, file_name):
        pattern = r"(API\d+)(beta\d+)?"
        matches = re.findall(pattern, file_name)
        api_versions = [f"{api}{f'{beta}' if beta else ''}" for api, beta in matches]
        return api_versions

    def remove_abc(self, abc_path):
        if path.exists(abc_path):
            os.remove(abc_path)

    def run(self, runner):
        for api_version in API_VERSION_MAP:
            target_api_version, target_api_sub_version = AbcTestCasesPrepare.split_api_version(api_version)
            stderr = self.generate_abc(runner, target_api_version, target_api_sub_version)
            if not self.passed:
                self.error = stderr.decode("utf-8", errors="ignore")
                return self
            if stderr:
                continue
            stderr = self.test_abc_execution(runner, target_api_version, target_api_sub_version)
            self.remove_abc(self.target_abc_path)
            if not self.passed:
                self.error = stderr.decode("utf-8", errors="ignore")
                return self
        return self


class TestVersionControl(Test):
    def __init__(self, test_path, flags, test_version, feature_type, module_path_list):
        Test.__init__(self, test_path, flags)
        self.beta_version_default = 3
        self.version_with_sub_version_list = ["12"]
        self.target_api_version_list = ["9", "10", "11", "12", "18", "20"]
        self.target_api_sub_version_list = ["beta1", "beta2", "beta3"]
        self.specific_api_version_list = ["API11", "API12beta3"]
        self.output = None
        self.process = None
        self.test_version = test_version
        self.test_abc_path = None
        self.feature_type = feature_type
        self.module_path_list = module_path_list
        self.module_abc_path_set = set()

    def split_version(self, version_str):
        parts = version_str.split("API")[1].split("beta")
        main_part = int(parts[0])
        beta_part = int(parts[1]) if len(parts) > 1 else self.beta_version_default
        return (main_part, beta_part)

    def compare_two_versions(self, version1, version2):
        version1_parsed = self.split_version(version1)
        version2_parsed = self.split_version(version2)

        if version1_parsed < version2_parsed:
            return -1
        elif version1_parsed > version2_parsed:
            return 1
        else:
            return 0

    def get_relative_path(self, from_dir, to_dir):
        from_dir = os.path.normpath(from_dir)
        to_dir = os.path.normpath(to_dir)
        from_dir = os.path.abspath(from_dir)
        to_dir = os.path.abspath(to_dir)
        from_parts = from_dir.split(os.sep)
        to_parts = to_dir.split(os.sep)
        common_prefix_length = 0
        for part1, part2 in zip(from_parts, to_parts):
            if part1 == part2:
                common_prefix_length += 1
            else:
                break
        relative_parts = [".."] * (len(from_parts) - common_prefix_length) + to_parts[common_prefix_length:]
        relative_path = os.path.join(*relative_parts)
        return relative_path

    def generate_single_module_abc(self, runner, module_path, target_version):
        cmd = []
        cmd.append(runner.es2panda)
        cmd.append(module_path)
        cmd.append("--module")
        main_version, sub_version = self.split_version(target_version)
        cmd.append("--target-api-version=%s" % (main_version))
        if main_version == 12:
            cmd.append("--target-api-sub-version=beta%s" % (sub_version))

        basename = os.path.basename(module_path)
        module_abc_name = "%s.abc" % (path.splitext(basename)[0])
        relative_path = self.get_relative_path(path.split(self.path)[0], path.split(module_path)[0])
        module_abc_dir = path.join(runner.build_dir, relative_path)
        if not os.path.exists(module_abc_dir):
            os.makedirs(module_abc_dir)
        module_abc_path = path.join(module_abc_dir, module_abc_name)
        self.module_abc_path_set.add(module_abc_path)
        cmd.extend(["--output=%s" % (module_abc_path)])

        self.process = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        proc = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        _, stderr = proc.communicate()
        proc.wait()
        if stderr:
            print(stderr.decode("utf-8", errors="ignore"))

    def generate_module_abc(self, runner, target_version):
        for module_path in self.module_path_list:
            self.generate_single_module_abc(runner, module_path, target_version)

    def remove_module_abc(self):
        for module_abc_path in self.module_abc_path_set:
            if path.exists(module_abc_path):
                os.remove(module_abc_path)

    def get_path_to_expected(
        self, is_support, expected_stage, target_api_version="", specific_api_version="", dump_type=""
    ):
        support_name = "supported" if is_support else "unsupported"
        api_name = ""
        # Higher than the specific API version, expected results may differ
        if target_api_version != "" and specific_api_version != "":
            if self.compare_two_versions(target_api_version, "API18") >= 0:
                api_name = "for_higher_or_equal_to_%s_" % "API18"
            else:
                if self.compare_two_versions(target_api_version, specific_api_version) >= 0:
                    api_name = "for_higher_or_equal_to_%s_" % (specific_api_version)
                else:
                    api_name = "for_below_%s_" % (specific_api_version)
        if dump_type == "ast":
            dump_type = ""
        elif dump_type == "asm":
            dump_type = "asm_"
        expected_path = "%s_%s_%s_%s%sversion-expected.txt" % (
            path.splitext(self.path)[0],
            support_name,
            expected_stage,
            api_name,
            dump_type,
        )
        return expected_path

    def get_path_to_runtime_output_below_version_expected(self):
        expected_path = "%s_runtime_below_abc_api_version-expected.txt" % (
            path.splitext(self.path)[0])
        return expected_path

    def get_path_to_runtime_output_expected(self, is_support, target_api_version, is_below_abc_api_version):
        path_expected = None
        if is_below_abc_api_version:
            path_expected = self.get_path_to_runtime_output_below_version_expected()
            return path_expected
        for specific_api_version in self.specific_api_version_list:
            if self.compare_two_versions(target_api_version, specific_api_version) > 0:
                continue
            path_expected = self.get_path_to_expected(is_support, "runtime", target_api_version, specific_api_version)
            if path.exists(path_expected):
                return path_expected
        return self.get_path_to_expected(is_support, "runtime", target_api_version)

    def get_path_to_compile_ast_output_expected(self, is_support):
        return self.get_path_to_expected(is_support, "compile")

    def get_path_to_compile_asm_output_expected(self, is_support, target_api_version):
        path_expected = None
        for specific_api_version in self.specific_api_version_list:
            path_expected = self.get_path_to_expected(
                is_support, "compile", target_api_version, specific_api_version, "asm"
            )
            if path.exists(path_expected):
                return path_expected
        return self.get_path_to_expected(is_support, "compile", "", "", "asm")

    def run_process(self, cmd):
        self.process = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        stdout, stderr = self.process.communicate()
        self.output = stdout.decode("utf-8", errors="ignore") + stderr.decode("utf-8", errors="ignore")
        return stdout, stderr

    def run_process_compile(self, runner, target_api_version, target_api_sub_version="bata3", dump_type=""):
        cmd = []
        cmd.append(runner.es2panda)
        cmd.append(self.path)
        cmd.extend(self.flags)
        cmd.append("--target-api-version=%s" % (target_api_version))
        test_abc_name = ("%s.abc" % (path.splitext(self.path)[0])).replace("/", "_")
        self.test_abc_path = path.join(runner.build_dir, test_abc_name)
        cmd.extend(["--output=%s" % (self.test_abc_path)])
        if target_api_version == "12":
            cmd.append("--target-api-sub-version=%s" % (target_api_sub_version))
        if dump_type == "ast":
            cmd.append("--dump-ast")
        elif dump_type == "assembly":
            cmd.append("--dump-assembly")
        stdout, stderr = self.run_process(cmd)
        return stdout, stderr

    def generate_ast_of_target_version(self, runner, target_api_version, target_api_sub_version="bata3"):
        return self.run_process_compile(runner, target_api_version, target_api_sub_version, dump_type="ast")

    def generate_asm_of_target_version(self, runner, target_api_version, target_api_sub_version="bata3"):
        return self.run_process_compile(runner, target_api_version, target_api_sub_version, dump_type="assembly")

    def runtime_for_target_version(self, runner, target_api_version, target_api_sub_version="bata3"):
        cmd = []
        if target_api_version != "12":
            target_api_sub_version = ""
        # there is no virtual machine with version api12beta2 available.
        # We have chosen api12beta1 as a replacement.
        if target_api_version == "12" and target_api_sub_version == "beta2":
            target_api_sub_version = "beta1"
        ark_js_vm_dir = os.path.join(
            runner.build_dir,
            "ark_js_vm_version",
            "API%s%s" % (target_api_version, target_api_sub_version),
        )
        ld_library_path = os.path.join(ark_js_vm_dir, "lib")
        os.environ["LD_LIBRARY_PATH"] = ld_library_path
        ark_js_vm_path = os.path.join(ark_js_vm_dir, "ark_js_vm")
        cmd.append(ark_js_vm_path)
        cmd.append(self.test_abc_path)
        self.process = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        stdout, stderr = self.process.communicate()
        self.output = stdout.decode("utf-8", errors="ignore") + stderr.decode("utf-8", errors="ignore").split("\n")[0]
        return stdout, stderr

    def run_for_single_version(self, runner, target_api_version, target_api_sub_version=""):
        cur_api_version = "API" + target_api_version + target_api_sub_version
        is_support = True if self.compare_two_versions(cur_api_version, self.test_version) >= 0 else False
        compile_expected_path = None
        stderr = None
        if self.feature_type == "syntax_feature":
            compile_expected_path = self.get_path_to_compile_ast_output_expected(is_support)
            _, stderr = self.generate_ast_of_target_version(runner, target_api_version, target_api_sub_version)
        elif self.feature_type == "bytecode_feature":
            compile_expected_path = self.get_path_to_compile_asm_output_expected(is_support, cur_api_version)
            _, stderr = self.generate_asm_of_target_version(
                runner, target_api_version, target_api_sub_version
            )
        try:
            with open(compile_expected_path, "r") as fp:
                expected = fp.read()
                self.passed = expected == self.output and self.process.returncode in [0, 1]
        except Exception:
            self.passed = False
        if not self.passed or (stderr and self.passed):
            return stderr
        cur_api_version_number = API_VERSION_MAP.get(cur_api_version)
        for api_version in self.target_api_version_list:
            # The interception capability of API9 version of ark_js_vm has not yet been launched.
            if api_version == "9":
                continue
            for api_sub_version in self.target_api_sub_version_list:
                if not api_version in self.version_with_sub_version_list and api_sub_version != "beta3":
                    continue
                elif not api_version in self.version_with_sub_version_list:
                    api_sub_version = ""
                cur_runtime_api_version = "API" + api_version + api_sub_version
                cur_runtime_api_version_number = API_VERSION_MAP.get(cur_runtime_api_version)
                is_below_abc_version = (
                    False
                    if TestAbcVersionControl.compare_version_number(
                        cur_runtime_api_version_number,
                        cur_api_version_number,
                    )
                    >= 0
                    else True
                )
                self.generate_module_abc(runner, cur_runtime_api_version)
                _, stderr = self.runtime_for_target_version(runner, api_version, api_sub_version)
                runtime_expected_path = self.get_path_to_runtime_output_expected(
                    is_support, cur_api_version, is_below_abc_version
                )
                self.remove_module_abc()
                try:
                    with open(runtime_expected_path, "r") as fp:
                        expected = fp.read()
                    if is_below_abc_version:
                        self.passed = expected in self.output
                    else:
                        self.passed = expected == self.output
                except Exception:
                    self.passed = False
                if not self.passed:
                    return stderr
        return stderr

    def run(self, runner):
        for target_api_version in self.target_api_version_list:
            stderr = None
            if target_api_version == "12":
                for target_api_sub_version in self.target_api_sub_version_list:
                    stderr = self.run_for_single_version(runner, target_api_version, target_api_sub_version)
                    if path.exists(self.test_abc_path):
                        os.remove(self.test_abc_path)
                    if not self.passed:
                        self.error = stderr.decode("utf-8", errors="ignore")
                        return self
            else:
                stderr = self.run_for_single_version(runner, target_api_version)
                if not self.passed:
                    self.error = stderr.decode("utf-8", errors="ignore")
                    return self
        return self


class CompilerTestInfo(object):
    def __init__(self, directory, extension, flags):
        self.directory = directory
        self.extension = extension
        self.flags = flags

    def update_dir(self, prefiex_dir):
        self.directory = os.path.sep.join([prefiex_dir, self.directory])


# Copy compiler directory to test/.local directory, and do inplace obfuscation.
def prepare_for_obfuscation(compiler_test_infos, test_root):
    tmp_dir_name = ".local"
    tmp_path = os.path.join(test_root, tmp_dir_name)
    if not os.path.exists(tmp_path):
        os.mkdir(tmp_path)

    test_root_dirs = set()
    for info in compiler_test_infos:
        root_dir = info.directory.split("/")[0]
        test_root_dirs.add(root_dir)

    for test_dir in test_root_dirs:
        src_dir = os.path.join(test_root, test_dir)
        target_dir = os.path.join(tmp_path, test_dir)
        if os.path.exists(target_dir):
            shutil.rmtree(target_dir)
        shutil.copytree(src_dir, target_dir)

    for info in compiler_test_infos:
        info.update_dir(tmp_dir_name)


def add_directory_for_version_control(runners, args):
    ark_js_vm_prepared = ArkJsVmDownload(args)
    ark_js_vm_prepared.run()
    runner = VersionControlRunner(args)
    runner.add_directory(
        "version_control/API11/syntax_feature",
        "js",
        ["--module"],
        "API11",
        "syntax_feature",
    )
    runner.add_directory(
        "version_control/API11/syntax_feature",
        "ts",
        ["--module"],
        "API11",
        "syntax_feature",
    )
    runner.add_directory(
        "version_control/API12beta1_and_beta2/syntax_feature",
        "ts", ["--module"],
        "API12beta1",
        "syntax_feature",
    )
    runner.add_directory(
        "version_control/API12beta1_and_beta2/syntax_feature",
        "js",
        ["--module"],
        "API12beta1",
        "syntax_feature",
    )
    runner.add_directory(
        "version_control/API12beta3/syntax_feature",
        "ts",
        ["--module"],
        "API12beta3",
        "syntax_feature",
        "version_control/API12beta3/syntax_feature/import_target",
    )
    runner.add_directory(
        "version_control/API12beta3/syntax_feature",
        "js",
        ["--module"],
        "API12beta3",
        "syntax_feature",
        "version_control/API12beta3/syntax_feature/import_target",
    )
    runner.add_directory(
        "version_control/API11/bytecode_feature",
        "ts",
        ["--module"],
        "API11",
        "bytecode_feature",
    )
    runner.add_directory(
        "version_control/API11/bytecode_feature",
        "js",
        ["--module"],
        "API11",
        "bytecode_feature",
    )
    runner.add_directory(
        "version_control/API12beta1_and_beta2/bytecode_feature",
        "ts",
        ["--module"],
        "API12beta1",
        "bytecode_feature",
        "version_control/API12beta1_and_beta2/bytecode_feature/import_target",
    )
    runner.add_directory(
        "version_control/API12beta1_and_beta2/bytecode_feature",
        "js",
        ["--module"],
        "API12beta1",
        "bytecode_feature",
        "version_control/API12beta1_and_beta2/bytecode_feature/import_target",
    )
    runner.add_directory(
        "version_control/API12beta3/bytecode_feature",
        "ts",
        ["--module"],
        "API12beta3",
        "bytecode_feature",
        "version_control/API12beta3/bytecode_feature/import_target",
    )
    runner.add_directory(
        "version_control/API12beta3/bytecode_feature",
        "js",
        ["--module"],
        "API12beta3",
        "bytecode_feature",
        "version_control/API12beta3/bytecode_feature/import_target",
    )
    runner.add_directory(
        "version_control/API18/bytecode_feature",
        "js",
        [],
        "API18",
        "bytecode_feature",
    )
    runner.add_directory(
        "version_control/API18/bytecode_feature",
        "ts",
        ["--module"],
        "API18",
        "bytecode_feature",
    )
    runner.add_directory(
        "version_control/API20/bytecode_feature",
        "ts",
        ["--module", "--enable-annotations"],
        "API20",
        "bytecode_feature",
    )
    runners.append(runner)

    abc_tests_prepare = AbcTestCasesPrepare(args)
    abc_tests_prepare.gen_abc_tests(
        "version_control/bytecode_version_control/non_merge_mode",
        "js",
        ["--module"],
        "non_merge_mode",
    )
    abc_tests_prepare.gen_abc_tests(
        "version_control/bytecode_version_control/merge_mode",
        "js",
        ["--module", "--merge-abc"],
        "merge_mode",
    )
    abc_tests_prepare.gen_abc_tests(
        "version_control/bytecode_version_control/mixed_compile",
        "js",
        ["--module", "--merge-abc"],
        "merge_mode",
    )

    args.abc_tests_prepare = abc_tests_prepare
    abc_version_control_runner = AbcVersionControlRunner(args)
    abc_version_control_runner.add_directory(
        "version_control/bytecode_version_control/non_merge_mode",
        "abc",
        ["--module", "--enable-abc-input"],
        "non_merge_mode",
    )
    abc_version_control_runner.add_directory(
        "version_control/bytecode_version_control/merge_mode",
        "abc",
        ["--module", "--enable-abc-input", "--merge-abc"],
        "merge_mode",
    )
    abc_version_control_runner.add_directory(
        "version_control/bytecode_version_control/mixed_compile",
        "txt",
        ["--module", "--enable-abc-input", "--merge-abc"],
        "mix_compile_mode",
    )
    runners.append(abc_version_control_runner)

def add_directory_for_regression(runners, args):
    runner = RegressionRunner(args)
    runner.add_directory("parser/concurrent", "js", ["--module", "--dump-ast"])
    runner.add_directory("parser/js", "js", ["--parse-only", "--dump-ast"])
    runner.add_directory("parser/script", "ts", ["--parse-only", "--dump-ast"])
    runner.add_directory("parser/ts", "ts",
                         ["--parse-only", "--module", "--dump-ast"])
    runner.add_directory("parser/ts/type_checker", "ts",
                         ["--parse-only", "--enable-type-check", "--module", "--dump-ast"])
    runner.add_directory("parser/ts/cases/declaration", "d.ts",
                         ["--parse-only", "--module", "--dump-ast"], TSDeclarationTest)
    runner.add_directory("parser/commonjs", "js", ["--commonjs", "--parse-only", "--dump-ast"])
    runner.add_directory("parser/binder", "js", ["--dump-assembly", "--dump-literal-buffer", "--module", "--target-api-sub-version=beta3"])
    runner.add_directory("parser/binder", "ts", ["--dump-assembly", "--dump-literal-buffer", "--module", "--target-api-sub-version=beta3"])
    runner.add_directory("parser/binder/noModule", "ts", ["--dump-assembly", "--dump-literal-buffer", "--target-api-sub-version=beta3"])
    runner.add_directory("parser/binder/api12beta2", "js", ["--dump-assembly", "--target-api-version=12", "--target-api-sub-version=beta2"])
    runner.add_directory("parser/binder/debugInfo", "ts", ["--dump-assembly", "--dump-literal-buffer", "--debug-info", "--module"])
    runner.add_directory("parser/js/emptySource", "js", ["--dump-assembly"])
    runner.add_directory("parser/js/language/arguments-object", "js", ["--parse-only"])
    runner.add_directory("parser/js/language/statements/for-statement", "js", ["--parse-only", "--dump-ast"])
    runner.add_directory("parser/js/language/expressions/optional-chain", "js", ["--parse-only", "--dump-ast"])
    runner.add_directory("parser/js/language/import/syntax/api18", "js",
                         ["--parse-only", "--module", "--target-api-version=18"])
    runner.add_directory("parser/js/language/import/syntax/api12/beta3", "js",
                         ["--parse-only", "--module", "--target-api-version=12", "--target-api-sub-version=beta3"])
    runner.add_directory("parser/js/language/import/syntax/api12/beta2", "js",
                         ["--parse-only", "--module", "--target-api-version=12", "--target-api-sub-version=beta2"])
    runner.add_directory("parser/js/language/import", "ts",
                         ["--dump-assembly", "--dump-literal-buffer", "--module", "--target-api-version=12",
                          "--target-api-sub-version=beta3"])
    runner.add_directory("parser/sendable_class", "ts",
                         ["--dump-assembly", "--dump-literal-buffer", "--module", "--target-api-sub-version=beta3"])
    runner.add_directory("parser/sendable_class/api12beta2", "ts",
                         ["--dump-assembly", "--dump-literal-buffer", "--module", "--target-api-version=12", "--target-api-sub-version=beta2"])
    runner.add_directory("parser/unicode", "js", ["--parse-only"])
    runner.add_directory("parser/ts/stack_overflow", "ts", ["--parse-only", "--dump-ast"])
    runner.add_directory("parser/js/module-record/module-record-field-name-option.js", "js",
                         ["--module-record-field-name=abc", "--source-file=abc", "--module", "--dump-normalized-asm-program"])
    runner.add_directory("parser/annotations", "ts", ["--module", "--dump-ast", "--enable-annotations"])
    runner.add_directory("parser/ts/inline-property", "ts", ["--dump-assembly", "--module"])

    runners.append(runner)

    transformer_runner = TransformerRunner(args)
    transformer_runner.add_directory("parser/ts/transformed_cases", "ts",
                                     ["--parse-only", "--module", "--dump-transformed-ast",
                                     "--check-transformed-ast-structure"])

    runners.append(transformer_runner)

    bc_version_runner = BcVersionRunner(args)
    bc_version_runner.add_cmd()

    runners.append(bc_version_runner)

    transformer_api_version_10_runner = TransformerInTargetApiVersion10Runner(args)
    transformer_api_version_10_runner.add_directory("parser/ts/transformed_cases_api_version_10", "ts",
                                                    ["--parse-only", "--module", "--target-api-version=10",
                                                    "--dump-transformed-ast"])

    runners.append(transformer_api_version_10_runner)

def add_directory_for_asm(runners, args, mode=""):
    runner = AbcToAsmRunner(args, True if mode == "debug" else False)
    runner.add_directory("abc2asm/js", "js", [])
    runner.add_directory("abc2asm/ts", "ts", [])
    runner.add_directory("compiler/js", "js", [])
    runner.add_directory("compiler/ts/cases/compiler", "ts", [])
    runner.add_directory("compiler/ts/projects", "ts", ["--module"])
    runner.add_directory("compiler/ts/projects", "ts", ["--module", "--merge-abc"])
    runner.add_directory("compiler/dts", "d.ts", ["--module", "--opt-level=0"])
    runner.add_directory("compiler/commonjs", "js", ["--commonjs"])
    runner.add_directory("parser/concurrent", "js", ["--module"])
    runner.add_directory("parser/js", "js", [])
    runner.add_directory("parser/script", "ts", [])
    runner.add_directory("parser/ts", "ts", ["--module"])
    runner.add_directory("parser/ts/type_checker", "ts", ["--enable-type-check", "--module"])
    runner.add_directory("parser/commonjs", "js", ["--commonjs"])
    runner.add_directory("parser/binder", "js", ["--dump-assembly", "--dump-literal-buffer", "--module"])
    runner.add_directory("parser/binder", "ts", ["--dump-assembly", "--dump-literal-buffer", "--module"])
    runner.add_directory("parser/binder/noModule", "ts", ["--dump-assembly", "--dump-literal-buffer"])
    runner.add_directory("parser/js/emptySource", "js", [])
    runner.add_directory("parser/js/language/arguments-object", "js", [])
    runner.add_directory("parser/js/language/statements/for-statement", "js", [])
    runner.add_directory("parser/js/language/expressions/optional-chain", "js", [])
    runner.add_directory("parser/sendable_class", "ts", ["--module"])
    runner.add_directory("parser/unicode", "js", [])
    runner.add_directory("parser/ts/stack_overflow", "ts", [])

    runners.append(runner)


def add_directory_for_compiler(runners, args):
    runner = CompilerRunner(args)
    compiler_test_infos = []
    compiler_test_infos.append(CompilerTestInfo("compiler/js", "js", ["--module"]))
    compiler_test_infos.append(CompilerTestInfo("compiler/ts/cases", "ts", []))
    compiler_test_infos.append(CompilerTestInfo("compiler/ts/projects", "ts", ["--module"]))
    compiler_test_infos.append(CompilerTestInfo("compiler/ts/projects", "ts", ["--module", "--merge-abc"]))
    compiler_test_infos.append(CompilerTestInfo("compiler/annotations-projects", "ts", ["--module", "--enable-annotations", "--merge-abc"]))
    compiler_test_infos.append(CompilerTestInfo("compiler/dts", "d.ts", ["--module", "--opt-level=0"]))
    compiler_test_infos.append(CompilerTestInfo("compiler/commonjs", "js", ["--commonjs"]))
    compiler_test_infos.append(CompilerTestInfo("compiler/interpreter/lexicalEnv", "js", []))
    compiler_test_infos.append(CompilerTestInfo("compiler/sendable", "ts", ["--module", "--target-api-sub-version=beta3"]))
    compiler_test_infos.append(CompilerTestInfo("optimizer/js/branch-elimination", "js",
                                                ["--module", "--branch-elimination", "--dump-assembly"]))
    compiler_test_infos.append(CompilerTestInfo("optimizer/js/opt-try-catch-func", "js",
                                                ["--module", "--dump-assembly"]))
    compiler_test_infos.append(CompilerTestInfo("optimizer/js/unused-inst-opt", "js",
                                                ["--module", "--dump-assembly"]))
    compiler_test_infos.append(CompilerTestInfo("compiler/debugInfo/", "js",
                                                ["--debug-info", "--dump-debug-info", "--source-file", "debug-info.js"]))
    compiler_test_infos.append(CompilerTestInfo("compiler/js/module-record-field-name-option.js", "js",
                                                ["--module", "--module-record-field-name=abc"]))
    compiler_test_infos.append(CompilerTestInfo("compiler/annotations", "ts", ["--module", "--enable-annotations"]))
    compiler_test_infos.append(CompilerTestInfo("compiler/generateCache-projects", "ts",
                                                ["--merge-abc", "--file-threads=0", "--cache-file"]))
    # Following directories of test cases are for dump-assembly comparison only, and is not executed.
    # Check CompilerProjectTest for more details.
    compiler_test_infos.append(CompilerTestInfo("optimizer/ts/branch-elimination/projects", "ts",
                                                ["--module", "--branch-elimination", "--merge-abc", "--dump-assembly",
                                                "--file-threads=8"]))
    compiler_test_infos.append(CompilerTestInfo("compiler/bytecodehar/projects", "ts",
                                                ["--merge-abc", "--dump-assembly", "--enable-abc-input",
                                                 "--dump-deps-info", "--remove-redundant-file", "--enable-annotations",
                                                 "--dump-literal-buffer", "--dump-string", "--abc-class-threads=4"]))
    compiler_test_infos.append(CompilerTestInfo("compiler/bytecodehar/js/projects", "js",
                                                ["--merge-abc", "--dump-assembly", "--enable-abc-input",
                                                 "--dump-deps-info", "--remove-redundant-file",
                                                 "--dump-literal-buffer", "--dump-string", "--abc-class-threads=4"]))
    compiler_test_infos.append(CompilerTestInfo("compiler/bytecodehar/merge_abc_consistence_check/projects", "js",
                                                ["--merge-abc", "--dump-assembly", "--enable-abc-input",
                                                 "--abc-class-threads=4"]))
    compiler_test_infos.append(CompilerTestInfo("compiler/cache_projects", "ts",
                                                ["--merge-abc", "--dump-assembly", "--enable-abc-input",
                                                 "--dump-deps-info", "--remove-redundant-file", "--enable-annotations",
                                                 "--dump-literal-buffer", "--dump-string", "--abc-class-threads=4",
                                                 "--cache-file"]))

    compiler_test_infos.append(CompilerTestInfo("compiler/ts/shared_module/projects", "ts",
                                                ["--module", "--merge-abc", "--dump-assembly"]))
    compiler_test_infos.append(CompilerTestInfo("compiler/protobin", "ts", []))
    compiler_test_infos.append(CompilerTestInfo("compiler/merge_hap/projects", "ts",
                                                ["--merge-abc", "--dump-assembly", "--enable-abc-input",
                                                 "--dump-literal-buffer", "--dump-string", "--abc-class-threads=4"]))
    compiler_test_infos.append(CompilerTestInfo("compiler/abc2program", "ts",
                                                ["--merge-abc", "--module", "--dump-assembly", "--enable-abc-input",
                                                 "--dump-literal-buffer", "--dump-string", "--source-file=source.ts",
                                                 "--module-record-field-name=source"]))

    if args.enable_arkguard:
        prepare_for_obfuscation(compiler_test_infos, runner.test_root)

    for info in compiler_test_infos:
        runner.add_directory(info.directory, info.extension, info.flags)
    
    filesinfo_compiler_infos  = []
    filesinfo_runner = FilesInfoRunner(args)
    filesinfo_compiler_infos.append(CompilerTestInfo("compiler/filesInfoTest/sourceLang", "txt",
                                                ["--module", "--merge-abc", "--dump-assembly"]))

    for info in filesinfo_compiler_infos:
        filesinfo_runner.add_directory(info.directory, info.extension, info.flags)
    
    runners.append(runner)
    runners.append(filesinfo_runner)


def add_directory_for_bytecode(runners, args):
    runner = BytecodeRunner(args)
    runner.add_directory("bytecode/commonjs", "js", ["--commonjs", "--dump-assembly"])
    runner.add_directory("bytecode/js", "js", ["--dump-assembly"])
    runner.add_directory("bytecode/ts/cases", "ts", ["--dump-assembly"])
    runner.add_directory("bytecode/ts/ic", "ts", ["--dump-assembly"])
    runner.add_directory("bytecode/ts/api11", "ts", ["--dump-assembly", "--module", "--target-api-version=11"])
    runner.add_directory("bytecode/ts/api12", "ts", ["--dump-assembly", "--module", "--target-api-version=12"])
    runner.add_directory("bytecode/ts/api18", "ts", ["--dump-assembly", "--module", "--target-api-version=18"])
    runner.add_directory("bytecode/watch-expression", "js", ["--debugger-evaluate-expression", "--dump-assembly"])

    runners.append(runner)


def add_directory_for_debug(runners, args):
    runner = RegressionRunner(args)
    runner.add_directory("debug/parser", "js", ["--parse-only", "--dump-ast"])

    runners.append(runner)


def add_cmd_for_aop_transform(runners, args):
    runner = AopTransform(args)

    aop_file_path = path.join(runner.test_root, "aop")
    lib_suffix = '.so'
    #cpp src, deal type, result compare str, abc compare str
    msg_list = [
        ["correct_modify.cpp", "compile", "aop_transform_start", "new_abc_content"],
        ["correct_no_modify.cpp", "compile", "aop_transform_start", ""],
        ["exec_error.cpp", "compile", "Transform exec fail", ""],
        ["no_func_transform.cpp", "compile", "os::library_loader::ResolveSymbol get func Transform error", ""],
        ["error_format.cpp", "copy_lib", "os::library_loader::Load error", ""],
        ["".join(["no_exist", lib_suffix]), "dirct_use", "Failed to find file", ""],
        ["error_suffix.xxx", "direct_use", "aop transform file suffix support", ""]
    ]
    for msg in msg_list:
        cpp_file = path.join(aop_file_path, msg[0])
        if msg[1] == 'compile':
            lib_file = cpp_file.replace('.cpp', lib_suffix)
            remove_file = lib_file
            runner.add_cmd(["g++", "--share", "-o", lib_file, cpp_file], "", "", "")
        elif msg[1] == 'copy_lib':
            lib_file = cpp_file.replace('.cpp', lib_suffix)
            remove_file = lib_file
            if not os.path.exists(lib_file):
                with open(cpp_file, "r") as source_file:
                    fd = os.open(lib_file, os.O_RDWR | os.O_CREAT | os.O_TRUNC)
                    target_file = os.fdopen(fd, 'w')
                    target_file.write(source_file.read())
        elif msg[1] == 'direct_use':
            lib_file = cpp_file
            remove_file = ""

        js_file = path.join(aop_file_path, "test_aop.js")
        runner.add_cmd([runner.es2panda, "--merge-abc", "--transform-lib", lib_file, js_file], msg[2], msg[3], remove_file)

    runners.append(runner)


class AopTransform(Runner):
    def __init__(self, args):
        Runner.__init__(self, args, "AopTransform")

    def add_cmd(self, cmd, compare_str, compare_abc_str, remove_file, func=TestAop):
        self.tests += [func(cmd, compare_str, compare_abc_str, remove_file)]

    def test_path(self, src):
        return src


def main():
    args = get_args()

    runners = []

    if args.regression:
        add_directory_for_regression(runners, args)

    if args.abc_to_asm:
        add_directory_for_asm(runners, args)
        add_directory_for_asm(runners, args, "debug")

    if args.tsc:
        runners.append(TSCRunner(args))

    if args.compiler:
        add_directory_for_compiler(runners, args)

    if args.hotfix:
        runners.append(HotfixRunner(args))

    if args.hotreload:
        runners.append(HotreloadRunner(args))

    if args.coldfix:
        runners.append(ColdfixRunner(args))

    if args.coldreload:
        runners.append(ColdreloadRunner(args))

    if args.debugger:
        runners.append(DebuggerRunner(args))

    if args.base64:
        runners.append(Base64Runner(args))

    if args.bytecode:
        add_directory_for_bytecode(runners, args)

    if args.aop_transform:
        add_cmd_for_aop_transform(runners, args)

    if args.debug:
        add_directory_for_debug(runners, args)

    if args.version_control:
        add_directory_for_version_control(runners, args)

    failed_tests = 0

    for runner in runners:
        runner.run()
        failed_tests += runner.summarize()

    if failed_tests > 0:
        exit(1)
    exit(0)


if __name__ == "__main__":
    main()
