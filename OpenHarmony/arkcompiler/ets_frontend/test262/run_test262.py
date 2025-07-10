#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
Copyright (c) 2021 Huawei Device Co., Ltd.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

Description: Use ark to execute test 262 test suite
"""

import argparse
import datetime
import collections
import json
import os
import shutil
import sys
import subprocess
from multiprocessing import Pool
import platform
from typing import List

from utils import *
from config import *


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('--dir', metavar='DIR',
                        help='Directory to test. And support multiple directories and files separated by ":"')
    parser.add_argument('--file', metavar='FILE',
                        help='File to test')
    parser.add_argument('--mode',
                        nargs='?', choices=[1, 2, 3], type=int,
                        help='selection information as: ' +
                        '1: only default \n ' +
                        '2: only strict mode \n' +
                        '3: both default and strict mode\n')
    parser.add_argument('--es51', action='store_true',
                        help='Run test262 ES5.1 version')
    parser.add_argument('--es2021', default=False, const='all',
                        nargs='?', choices=['all', 'only', 'other'],
                        help='Run test262 - ES2021. ' +
                        'all: Contains all use cases for es5_tests and es2015_tests and es2021_tests and intl_tests' +
                        'only: Only include use cases for ES2021' +
                        'other: Contains all use cases for es5_tests and es2015_tests and es2021_tests and intl_tests' +
                        'and other_tests')
    parser.add_argument('--sendable', default=False, const='sendable',
                        nargs='?', choices=['sendable'],
                        help='Run test262 - sendable. ' +
                        'sendable: Contains all use cases for sendable')
    parser.add_argument('--es2022', default=False, const='all',
                        nargs='?', choices=['all', 'only', 'other'],
                        help='Run test262 - ES2022. ' +
                        'all: Contains all use cases for es5_tests and es2015_tests and es2021_tests' +
                        'and es2022_tests and intl_tests' +
                        'only: Only include use cases for ES2022' +
                        'other: Contains all use cases for es5_tests and es2015_tests and es2021_tests' +
                        'and es2022_tests and intl_tests and other_tests')
    parser.add_argument('--es2023', default=False, const='all',
                        nargs='?', choices=['all', 'only', 'other'],
                        help='Run test262 - ES2023. ' +
                        'all: Contains all use cases for es5_tests and es2015_tests and es2021_tests' +
                        'and es2022_tests and es2023_tests and intl_tests' +
                        'only: Only include use cases for ES2023' +
                        'other: Contains all use cases for es5_tests and es2015_tests and es2021_tests' +
                        'and es2022_tests and es2023_tests and intl_tests and other_tests')
    parser.add_argument('--intl', default=False, const='intl',
                        nargs='?', choices=['intl'],
                        help='Run test262 - Intltest. ' +
                        'intl: Only include use cases for intlcsae')
    parser.add_argument('--other', default=False, const='other',
                        nargs='?', choices=['other'],
                        help='Run test262 - other_tests ' +
                        'other_tests: Only include use cases for other_tests')
    parser.add_argument('--es2015', default=False, const='es2015',
                        nargs='?', choices=['es2015'],
                        help='Run test262 - es2015. ' +
                        'es2015: Only include use cases for es2015')
    parser.add_argument('--ci-build', action='store_true',
                        help='Run test262 ES2015 filter cases for build version')
    parser.add_argument('--esnext', action='store_true',
                        help='Run test262 - ES.next.')
    parser.add_argument('--test-list', metavar='FILE', dest="test_list", default=None,
                        help='File with list of tests to run')
    parser.add_argument('--engine', metavar='FILE',
                        help='Other engine binarys to run tests(as:d8,qjs...)')
    parser.add_argument('--babel', action='store_true',
                        help='Whether to use Babel conversion')
    parser.add_argument('--skip-list', action='append', dest='skip_list',
                        help='Use explicitly specify skip list in txt format. Can be set several times')
    parser.add_argument('--timeout', default=DEFAULT_TIMEOUT, type=int,
                        help='Set a custom test timeout in milliseconds !!!\n')
    parser.add_argument('--threads', default=DEFAULT_THREADS, type=int,
                        help="Run this many tests in parallel.")
    parser.add_argument('--hostArgs',
                        help="command-line arguments to pass to eshost host\n")
    parser.add_argument('--ark-tool',
                        help="ark's binary tool")
    parser.add_argument('--ark-aot', action='store_true',
                        help="Run test262 with aot")
    parser.add_argument('--ark-aot-tool',
                        help="ark's aot tool")
    parser.add_argument("--libs-dir",
                        help="The path collection of dependent so has been divided by':'")
    parser.add_argument('--ark-frontend',
                        nargs='?', choices=ARK_FRONTEND_LIST, type=str,
                        help="Choose one of them")
    parser.add_argument('--ark-frontend-binary',
                        help="ark frontend conversion binary tool")
    parser.add_argument('--icu-data-path',
                        help="ark frontend conversion binary tool")
    parser.add_argument('--ark-arch',
                        default=DEFAULT_ARK_ARCH,
                        nargs='?', choices=ARK_ARCH_LIST, type=str,
                        help="Choose one of them")
    parser.add_argument('--ark-arch-root',
                        default=DEFAULT_ARK_ARCH,
                        help="the root path for qemu-aarch64 or qemu-arm")
    parser.add_argument('--opt-level',
                        default=DEFAULT_OPT_LEVEL,
                        help="the opt level for es2abc")
    parser.add_argument('--es2abc-thread-count',
                        default=DEFAULT_ES2ABC_THREAD_COUNT,
                        help="the thread count for es2abc")
    parser.add_argument('--merge-abc-binary',
                        help="frontend merge abc binary tool")
    parser.add_argument('--merge-abc-mode',
                        help="run test for merge abc mode")
    parser.add_argument('--product-name',
                        default=DEFAULT_PRODUCT_NAME,
                        help="ark's product name")
    parser.add_argument('--run-pgo', action='store_true',
                        help="Run test262 with aot pgo")
    parser.add_argument('--enable-litecg', action='store_true',
                        help="Run test262 with aot litecg enabled")
    parser.add_argument('--run-jit', action='store_true',
                        help="Run test262 with JIT")
    parser.add_argument('--run-baseline-jit', action='store_true',
                        help="Run test262 with baseline JIT")
    parser.add_argument('--enable-rm', action='store_true',
                        help="Enable force remove of the data directory")
    parser.add_argument('--abc2program', action='store_true',
                        help="Use abc2prog to generate abc, aot or pgo is not supported yet under this option")
    parser.add_argument('--stub-file',
                        default=DEFAULT_STUB_FILE,
                        help="stub file")
    parser.add_argument('--disable-force-gc', action='store_true',
                        help="Run test262 with close force-gc")
    parser.add_argument('--enable-arkguard', action='store_true',
                        help="enable arkguard for 262 tests")
    
    args = parser.parse_args()
    if args.abc2program and (args.run_pgo or args.ark_aot):
        sys.exit("Error: '--abc2program' used together with  '--ark-aot' or '--run-pgo' is not supported")
    return args


def run_check(runnable, env=None):
    report_command('Test command:', runnable, env=env)

    if env is not None:
        full_env = dict(os.environ)
        full_env.update(env)
        env = full_env

    proc = subprocess.Popen(runnable, env=env)
    proc.wait()
    return proc.returncode


def excuting_npm_install(args):
    ark_frontend = DEFAULT_ARK_FRONTEND
    if args.ark_frontend:
        ark_frontend = args.ark_frontend

    if ark_frontend != ARK_FRONTEND_LIST[0]:
        return

    ark_frontend_binary = os.path.join(ARK_FRONTEND_BINARY_LIST[0])
    if args.ark_frontend_binary:
        ark_frontend_binary = os.path.join(args.ark_frontend_binary)

    ts2abc_build_dir = os.path.join(os.path.dirname(
        os.path.realpath(ark_frontend_binary)), "..")

    if not os.path.exists(os.path.join(ts2abc_build_dir, "package.json")) and \
        not os.path.exists(os.path.join(ts2abc_build_dir, "..", "package.json")):
        return

    if os.path.exists(os.path.join(ts2abc_build_dir, "..", "package.json")) and \
        not os.path.exists(os.path.join(ts2abc_build_dir, "package.json")):
        ts2abc_build_dir = os.path.join(ts2abc_build_dir, "..")
    # copy deps/ohos-typescript
    deps_dir = os.path.join(ts2abc_build_dir, "deps")
    mkdir(deps_dir)

    shutil.copyfile(OHOS_TYPESCRIPT_TGZ_PATH, os.path.join(deps_dir, OHOS_TYPESCRIPT))

    npm_install(ts2abc_build_dir)


def init(args):
    remove_dir(BASE_OUT_DIR)
    remove_dir(TEST_ES5_DIR)
    remove_dir(TEST_ES2015_DIR)
    remove_dir(TEST_INTL_DIR)
    remove_dir(TEST_ES2021_DIR)
    remove_dir(TEST_SENDABLE_DIR)
    remove_dir(TEST_ES2022_DIR)
    remove_dir(TEST_ES2023_DIR)
    remove_dir(TEST_CI_DIR)
    get_all_skip_tests(args)
    excuting_npm_install(args)


def get_all_skip_tests(args):
    # !!! plz correct the condition when changing the default frontend
    if args.ark_frontend and args.ark_frontend == ARK_FRONTEND_LIST[1]:
        SKIP_LIST_FILES.append(ES2ABC_SKIP_LIST_FILE)
    else:
        SKIP_LIST_FILES.append(TS2ABC_SKIP_LIST_FILE)
    
    if args.skip_list:
        SKIP_LIST_FILES.append(os.path.join("test262", args.skip_list[0]))

    for file in SKIP_LIST_FILES:
        if file.endswith('.txt'):
            with open(file, 'r') as txtfile:
                for line in txtfile:
                    # skip "#"
                    line = line.strip()
                    if line and not line.startswith('#'):
                        new_line = "/".join(line.split("/")[3:])
                        ALL_SKIP_TESTS.append(new_line)
        else:
            with open(file) as jsonfile:
                json_data = json.load(jsonfile)
                for key in json_data:
                    ALL_SKIP_TESTS.extend(key["files"])


def collect_files(path):
    if os.path.isfile(path):
        yield path
        return

    if not os.path.isdir(path):
        raise ValueError(f'Not found: "{path}"')

    for root, _, file_names in os.walk(path):
        for file_name in file_names:
            if file_name.startswith('.') or not file_name.endswith(".js"):
                continue

            yield os.path.join(root, file_name)


def mkdstdir(file, src_dir, dist_dir):
    idx = file.rfind(src_dir)
    if idx == -1:
        raise SystemExit(f'{file} can not found in {src_dir}')

    fpath, fname = os.path.split(file[idx:])
    fpath = fpath.replace(src_dir, dist_dir)
    mkdir(fpath)


class TestPrepare():
    def __init__(self, args):
        self.args = args
        self.out_dir = BASE_OUT_DIR

    @staticmethod
    def get_tests_from_file(file):
        with open(file) as fopen:
            files = [line.strip() for line in fopen.readlines() if not line.startswith("#") and line.strip()]
        return files

    def prepare_test262_code(self):
        if not os.path.isdir(os.path.join(DATA_DIR, '.git')):
            if self.args.run_jit:
                git_clone(TEST262_JIT_GIT_URL, DATA_DIR)
                git_checkout(TEST262_JIT_GIT_HASH, DATA_DIR)
            elif self.args.sendable == "sendable":
                git_clone(SENDABLE_GIT_URL, DATA_DIR)
                git_checkout(SENDABLE_GIT_HASH, DATA_DIR)
            else:
                git_clone(TEST262_GIT_URL, DATA_DIR)
                git_checkout(TEST262_GIT_HASH, DATA_DIR)

        if self.args.enable_rm and self.args.run_jit and not os.path.isfile(TEST262_JIT_LABEL):
            remove_dir(DATA_DIR)
            git_clone(TEST262_JIT_GIT_URL, DATA_DIR)
            git_checkout(TEST262_JIT_GIT_HASH, DATA_DIR)

        if not os.path.isdir(os.path.join(ESHOST_DIR, '.git')):
            git_clone(ESHOST_GIT_URL, ESHOST_DIR)
            git_checkout(ESHOST_GIT_HASH, ESHOST_DIR)
            git_apply('../eshost.patch', ESHOST_DIR)

        npm_install(ESHOST_DIR)

        if not os.path.isdir(os.path.join(HARNESS_DIR, '.git')):
            git_clone(HARNESS_GIT_URL, HARNESS_DIR)
            git_checkout(HARNESS_GIT_HASH, HARNESS_DIR)
            git_apply('../harness.patch', HARNESS_DIR)

        npm_install(HARNESS_DIR)

    def prepare_clean_data(self):
        git_clean(DATA_DIR)
        if self.args.run_jit:
            git_checkout(TEST262_JIT_GIT_HASH, DATA_DIR)
        elif self.args.sendable == "sendable":
            git_checkout(SENDABLE_GIT_HASH, DATA_DIR)
        else:
            git_checkout(TEST262_GIT_HASH, DATA_DIR)

    def patching_the_plugin(self):
        remove_file(os.path.join(ESHOST_DIR, "lib/agents/panda.js"))
        remove_file(os.path.join(ESHOST_DIR, "runtimes/panda.js"))

        git_clean(ESHOST_DIR)
        git_apply("../eshost.patch", ESHOST_DIR)
        git_clean(HARNESS_DIR)
        git_apply("../harness.patch", HARNESS_DIR)

    def prepare_args_es51_es2021(self):
        if self.args.dir:
            if TEST_ES5_DIR in self.args.dir:
                self.args.es51 = True
            elif TEST_ES2015_DIR in self.args.dir:
                self.args.es2015 = "es2015"
            elif TEST_INTL_DIR in self.args.dir:
                self.args.intl = "intl"
            elif TEST_ES2021_DIR in self.args.dir:
                self.args.es2021 = "all"
            elif TEST_ES2022_DIR in self.args.dir:
                self.args.es2022 = "all"
            elif TEST_ES2023_DIR in self.args.dir:
                self.args.es2023 = "all"
            elif TEST_OTHERTESTS_DIR in self.args.dir:
                self.args.other = "other"

        if self.args.file:
            if TEST_ES5_DIR in self.args.file:
                self.args.es51 = True
            elif TEST_ES2015_DIR in self.args.file:
                self.args.es2015 = "es2015"
            elif TEST_INTL_DIR in self.args.file:
                self.args.intl = "intl"
            elif TEST_ES2021_DIR in self.args.file:
                self.args.es2021 = "all"
            elif TEST_ES2022_DIR in self.args.file:
                self.args.es2022 = "all"
            elif TEST_ES2023_DIR in self.args.file:
                self.args.es2023 = "all"
            elif TEST_OTHERTESTS_DIR in self.args.file:
                self.args.other = "other"

    def prepare_out_dir(self):
        if self.args.es51:
            self.out_dir = os.path.join(BASE_OUT_DIR, "test_es51")
        elif self.args.es2015:
            self.out_dir = os.path.join(BASE_OUT_DIR, "test_es2015")
        elif self.args.intl:
            self.out_dir = os.path.join(BASE_OUT_DIR, "test_intl")
        elif self.args.es2021:
            self.out_dir = os.path.join(BASE_OUT_DIR, "test_es2021")
        elif self.args.es2022:
            self.out_dir = os.path.join(BASE_OUT_DIR, "test_es2022")
        elif self.args.es2023:
            self.out_dir = os.path.join(BASE_OUT_DIR, "test_es2023")
        elif self.args.ci_build:
            self.out_dir = os.path.join(BASE_OUT_DIR, "test_CI")
        elif self.args.sendable:
            self.out_dir = os.path.join(BASE_OUT_DIR, "test_sendable")
        elif self.args.other:
            self.out_dir = os.path.join(BASE_OUT_DIR, "other_tests")
        else:
            self.out_dir = os.path.join(BASE_OUT_DIR, "test")

    def prepare_args_testdir(self):
        if self.args.dir:
            return

        if self.args.es51:
            self.args.dir = TEST_ES5_DIR
        elif self.args.es2015:
            self.args.dir = TEST_ES2015_DIR
        elif self.args.intl:
            self.args.dir = TEST_INTL_DIR
        elif self.args.es2021:
            self.args.dir = TEST_ES2021_DIR
        elif self.args.sendable:
            self.args.dir = TEST_SENDABLE_DIR
        elif self.args.es2022:
            self.args.dir = TEST_ES2022_DIR
        elif self.args.es2023:
            self.args.dir = TEST_ES2023_DIR
        elif self.args.other:
            self.args.dir = TEST_OTHERTESTS_DIR
        elif self.args.ci_build:
            self.args.dir = TEST_CI_DIR
        else:
            self.args.dir = os.path.join(DATA_DIR, "test")

    def copyfile(self, file, all_skips):
        dstdir = os.path.join(DATA_DIR, "test")
        file = file.strip()
        file = file.strip('\n')
        file = file.replace("\\", "/")
        if file in all_skips:
            return

        srcdir = os.path.join(DATA_DIR, "test", file)
        if self.args.es51:
            dstdir = os.path.join(TEST_ES5_DIR, file)
        elif self.args.es2015:
            dstdir = os.path.join(TEST_ES2015_DIR, file)
        elif self.args.intl:
            dstdir = os.path.join(TEST_INTL_DIR, file)
        elif self.args.es2021:
            dstdir = os.path.join(TEST_ES2021_DIR, file)
        elif self.args.sendable:
            dstdir = os.path.join(TEST_SENDABLE_DIR, file)
        elif self.args.es2022:
            dstdir = os.path.join(TEST_ES2022_DIR, file)
        elif self.args.es2023:
            dstdir = os.path.join(TEST_ES2023_DIR, file)
        elif self.args.other:
            dstdir = os.path.join(TEST_OTHERTESTS_DIR, file)
        elif self.args.ci_build:
            dstdir = os.path.join(TEST_CI_DIR, file)

        if os.path.isfile(srcdir):
            shutil.copyfile(srcdir, dstdir)


    def collect_tests(self):
        files = []
        origin_dir = os.path.join(DATA_DIR, "test/")
        file_names = collect_files(origin_dir)
        esid = ""
        if self.args.es51:
            esid = "es5id"
        elif self.args.es2021 or self.args.es2022 or self.args.es2023:
            esid = "es6id"

        for file_name in file_names:
            with open(file_name, 'r', encoding='utf-8') as file:
                file_content = file.read()
                if esid in file_content:
                    files.append(file_name.split(origin_dir)[1])
        return files

    def prepare_es2021_tests(self):
        files = []
        files = self.collect_tests()
        files.extend(self.get_tests_from_file(ES2021_LIST_FILE))
        if self.args.es2021 == "all":
            files.extend(self.get_tests_from_file(ES5_LIST_FILE))
            files.extend(self.get_tests_from_file(INTL_LIST_FILE))
            files.extend(self.get_tests_from_file(ES2015_LIST_FILE))
        if self.args.es2021 == "other":
            files.extend(self.get_tests_from_file(ES5_LIST_FILE))
            files.extend(self.get_tests_from_file(INTL_LIST_FILE))
            files.extend(self.get_tests_from_file(ES2015_LIST_FILE))
            files.extend(self.get_tests_from_file(OTHER_LIST_FILE))
        return files

    def prepare_sendable_tests(self):
        files = []
        if self.args.sendable == "sendable":
            files.extend(self.get_tests_from_file(SENDABLE_LIST_FILE))
        return files

    def prepare_es2022_tests(self):
        files = []
        files.extend(self.get_tests_from_file(ES2022_LIST_FILE))
        if self.args.es2022 == "all":
            files.extend(self.get_tests_from_file(ES5_LIST_FILE))
            files.extend(self.get_tests_from_file(INTL_LIST_FILE))
            files.extend(self.get_tests_from_file(ES2015_LIST_FILE))
            files.extend(self.collect_tests())
            files.extend(self.get_tests_from_file(ES2021_LIST_FILE))
        if self.args.es2022 == "other":
            files.extend(self.get_tests_from_file(ES5_LIST_FILE))
            files.extend(self.get_tests_from_file(INTL_LIST_FILE))
            files.extend(self.get_tests_from_file(ES2015_LIST_FILE))
            files.extend(self.collect_tests())
            files.extend(self.get_tests_from_file(ES2021_LIST_FILE))
            files.extend(self.get_tests_from_file(OTHER_LIST_FILE))
        return files

    def prepare_es2023_tests(self):
        files = []
        files.extend(self.get_tests_from_file(ES2023_LIST_FILE))
        if self.args.es2023 == "all":
            files.extend(self.get_tests_from_file(ES5_LIST_FILE))
            files.extend(self.get_tests_from_file(INTL_LIST_FILE))
            files.extend(self.get_tests_from_file(ES2015_LIST_FILE))
            files.extend(self.collect_tests())
            files.extend(self.get_tests_from_file(ES2021_LIST_FILE))
            files.extend(self.get_tests_from_file(ES2022_LIST_FILE))
        if self.args.es2023 == "other":
            files.extend(self.get_tests_from_file(ES5_LIST_FILE))
            files.extend(self.get_tests_from_file(INTL_LIST_FILE))
            files.extend(self.get_tests_from_file(ES2015_LIST_FILE))
            files.extend(self.collect_tests())
            files.extend(self.get_tests_from_file(ES2021_LIST_FILE))
            files.extend(self.get_tests_from_file(ES2022_LIST_FILE))
            files.extend(self.get_tests_from_file(OTHER_LIST_FILE))
        return files

    def prepare_intl_tests(self):
        files = []
        files = self.collect_tests()
        if self.args.intl:
            files = self.get_tests_from_file(INTL_LIST_FILE)
        return files

    def prepare_other_tests(self):
        files = []
        files = self.collect_tests()
        if self.args.other:
            files = self.get_tests_from_file(OTHER_LIST_FILE)
        return files
        
    def prepare_es2015_tests(self):
        files = []
        files = self.collect_tests()
        if self.args.es2015:
            files = self.get_tests_from_file(ES2015_LIST_FILE)
        return files

    def prepare_test_suit(self):
        files = []
        test_dir = ""
        if self.args.es51:
            test_dir = TEST_ES5_DIR
            files = self.get_tests_from_file(ES5_LIST_FILE)
        elif self.args.es2015:
            test_dir = TEST_ES2015_DIR
            files = self.prepare_es2015_tests()
        elif self.args.intl:
            test_dir = TEST_INTL_DIR
            files = self.prepare_intl_tests()
        elif self.args.other:
            test_dir = TEST_OTHERTESTS_DIR
            files = self.prepare_other_tests()
        elif self.args.es2021:
            test_dir = TEST_ES2021_DIR
            files = self.prepare_es2021_tests()
        elif self.args.es2022:
            test_dir = TEST_ES2022_DIR
            files = self.prepare_es2022_tests()
        elif self.args.es2023:
            test_dir = TEST_ES2023_DIR
            files = self.prepare_es2023_tests()
        elif self.args.sendable:
            test_dir = TEST_SENDABLE_DIR
            files = self.prepare_sendable_tests()
        elif self.args.ci_build:
            test_dir = TEST_CI_DIR
            files = self.get_tests_from_file(CI_LIST_FILE)

        for file in files:
            path = os.path.split(file)[0]
            if not path.startswith(test_dir):
                path = os.path.join(test_dir, path)
            mkdir(path)

            self.copyfile(file, ALL_SKIP_TESTS)

    def prepare_test262_test(self):
        src_dir = TEST_FULL_DIR
        if self.args.es51:
            self.prepare_test_suit()
            src_dir = TEST_ES5_DIR
        elif self.args.es2015:
            self.prepare_test_suit()
            src_dir = TEST_ES2015_DIR
        elif self.args.intl:
            self.prepare_test_suit()
            src_dir = TEST_INTL_DIR
        elif self.args.other:
            self.prepare_test_suit()
            src_dir = TEST_OTHERTESTS_DIR
        elif self.args.es2021:
            self.prepare_test_suit()
            src_dir = TEST_ES2021_DIR
        elif self.args.sendable:
            self.prepare_test_suit()
            src_dir = TEST_SENDABLE_DIR
        elif self.args.es2022:
            self.prepare_test_suit()
            src_dir = TEST_ES2022_DIR
        elif self.args.es2023:
            self.prepare_test_suit()
            src_dir = TEST_ES2023_DIR
        elif self.args.ci_build:
            self.prepare_test_suit()
            src_dir = TEST_CI_DIR
        elif self.args.esnext:
            git_checkout(ESNEXT_GIT_HASH, DATA_DIR)
        else:
            if self.args.run_jit:
                git_checkout(TEST262_JIT_GIT_HASH, DATA_DIR)
            else:
                git_checkout(TEST262_GIT_HASH, DATA_DIR)

        if self.args.file:
            mkdstdir(self.args.file, src_dir, self.out_dir)
            return

        files = []
        if ':' in self.args.dir:
            path = self.args.dir.split(':')
            for p in path:
                files.extend(collect_files(p))
        else:
            files = collect_files(self.args.dir)
        for file in files:
            mkdstdir(file, src_dir, self.out_dir)

    def get_code(self):
        self.prepare_test262_code()
        self.prepare_clean_data()
        self.patching_the_plugin()

    def run(self):
        self.prepare_args_es51_es2021()
        self.prepare_out_dir()
        self.prepare_args_testdir()
        self.prepare_test262_test()


def modetype_to_string(mode):
    if mode == 1:
        return "only default"
    if mode == 2:
        return "only strict mode"
    return "both default and strict mode"


def run_test262_mode(args):
    if args.mode:
        return modetype_to_string(args.mode)
    return modetype_to_string(DEFAULT_MODE)


def get_execute_arg(args) -> list[str]:
    execute_args = []

    if args.file:
        execute_args.append(args.file)
    else:
        path = args.dir.split(':')
        for p in path:
            if not p.endswith('.js'):
                execute_args.append(os.path.join(p, "**", "*.js"))
            else:
                execute_args.append(p)
    return execute_args


def get_host_path_type(args):
    host_path = DEFAULT_HOST_PATH
    host_type = DEFAULT_HOST_TYPE
    if args.engine:
        host_path = args.engine
        host_type = os.path.split(args.engine.strip())[1]
    return host_path, host_type


def get_timeout(args, threads):
    timeout = DEFAULT_TIMEOUT * threads
    if args.timeout:
        timeout = args.timeout
    return timeout


def get_threads(args):
    threads = DEFAULT_THREADS
    if args.threads:
        threads = args.threads
    return threads


def get_host_args_of_product_name(args):
    product_name = args.product_name
    ark_dir = f"{ARGS_PREFIX}{product_name}/{ARK_DIR_SUFFIX}"
    icui_dir = f"{ARGS_PREFIX}{product_name}/{ICUI_DIR_SUFFIX}"
    ark_js_runtime_dir = f"{ARGS_PREFIX}{product_name}/{ARK_JS_RUNTIME_DIR_SUFFIX}"
    zlib_dir = f"{ARGS_PREFIX}{product_name}/{ZLIB_DIR_SUFFIX}"

    ark_tool = os.path.join(ark_js_runtime_dir, "ark_js_vm")
    libs_dir = f"{icui_dir}:{LLVM_DIR}:{ark_js_runtime_dir}:{zlib_dir}"
    ark_aot_tool = os.path.join(ark_js_runtime_dir, "ark_aot_compiler")
    merge_abc_binary = os.path.join(ark_dir, "merge_abc")

    return ark_tool, libs_dir, ark_aot_tool, merge_abc_binary


def get_host_args_of_host_type(args, host_args, ark_tool, ark_aot_tool, libs_dir, ark_frontend,
                               ark_frontend_binary, opt_level, es2abc_thread_count,
                               merge_abc_binary, merge_abc_mode, product_name, icu_data_path):
    host_args = f"-B test262/run_sunspider.py "
    host_args += f"--ark-tool={ark_tool} "
    if args.ark_aot:
        host_args += f"--ark-aot "
    if args.run_pgo:
        host_args += f"--run-pgo "
    if args.enable_litecg:
        host_args += f"--enable-litecg "
    if args.run_jit:
        host_args += f"--run-jit "
    if args.run_baseline_jit:
        host_args += f"--run-baseline-jit "
    host_args += f"--ark-aot-tool={ark_aot_tool} "
    host_args += f"--libs-dir={libs_dir} "
    host_args += f"--ark-frontend={ark_frontend} "
    host_args += f"--ark-frontend-binary={ark_frontend_binary} "
    host_args += f"--opt-level={opt_level} "
    host_args += f"--icu-data-path={icu_data_path} "
    host_args += f"--es2abc-thread-count={es2abc_thread_count} "
    host_args += f"--merge-abc-binary={merge_abc_binary} "
    host_args += f"--merge-abc-mode={merge_abc_mode} "
    host_args += f"--product-name={product_name} "
    if args.abc2program:
        host_args = f"{host_args}--abc2program "
    if args.enable_arkguard:
        host_args = f"{host_args}--enable-arkguard "
    return host_args


def get_host_args_of_ark_arch(args, host_args):
    host_args += f"--ark-arch={args.ark_arch} "
    host_args += f"--ark-arch-root={args.ark_arch_root} "

    return host_args


def get_disable_force_gc(host_args, args):
    host_args += f"--disable-force-gc "

    return host_args


def get_host_args_of_stub_file(args, host_args):
    host_args += f"--stub-file={args.stub_file} "

    return host_args


def get_host_args(args, host_type):
    host_args = ""
    ark_tool = DEFAULT_ARK_TOOL
    ark_aot_tool = DEFAULT_ARK_AOT_TOOL
    libs_dir = DEFAULT_LIBS_DIR
    ark_frontend = DEFAULT_ARK_FRONTEND
    ark_frontend_binary = DEFAULT_ARK_FRONTEND_BINARY
    ark_arch = DEFAULT_ARK_ARCH
    stub_file = DEFAULT_STUB_FILE
    opt_level = DEFAULT_OPT_LEVEL
    es2abc_thread_count = DEFAULT_ES2ABC_THREAD_COUNT
    merge_abc_binary = DEFAULT_MERGE_ABC_BINARY
    merge_abc_mode = DEFAULT_MERGE_ABC_MODE
    product_name = DEFAULT_PRODUCT_NAME
    icu_data_path = DEFAULT_ICU_PATH

    if args.product_name:
        ark_tool, libs_dir, ark_aot_tool, merge_abc_binary = get_host_args_of_product_name(args)

    if args.hostArgs:
        host_args = args.hostArgs

    if args.ark_tool:
        ark_tool = args.ark_tool

    if args.ark_aot_tool:
        ark_aot_tool = args.ark_aot_tool

    if args.libs_dir:
        libs_dir = args.libs_dir

    if args.ark_frontend:
        ark_frontend = args.ark_frontend

    if args.ark_frontend_binary:
        ark_frontend_binary = args.ark_frontend_binary

    if args.opt_level:
        opt_level = args.opt_level

    if args.es2abc_thread_count:
        es2abc_thread_count = args.es2abc_thread_count

    if args.merge_abc_binary:
        merge_abc_binary = args.merge_abc_binary

    if args.icu_data_path:
        icu_data_path = args.icu_data_path
    if args.merge_abc_mode:
        merge_abc_mode = args.merge_abc_mode

    if host_type == DEFAULT_HOST_TYPE:
        host_args = get_host_args_of_host_type(args, host_args, ark_tool, ark_aot_tool, libs_dir, ark_frontend,
                                               ark_frontend_binary, opt_level, es2abc_thread_count,
                                               merge_abc_binary, merge_abc_mode, product_name, icu_data_path)

    if args.ark_arch != ark_arch:
        host_args = get_host_args_of_ark_arch(args, host_args)

    if args.stub_file != stub_file:
        host_args = get_host_args_of_stub_file(args, host_args)

    if args.disable_force_gc:
        host_args = get_disable_force_gc(host_args, args)

    return host_args


def run_test262_test(args):
    execute_args = get_execute_arg(args)
    host_path, host_type = get_host_path_type(args)
    host_args = get_host_args(args, host_type)
    threads = get_threads(args)
    timeout = get_timeout(args, threads)

    test_cmd = ["node", TEST262_RUNNER_SCRIPT]
    test_cmd.append(f"--hostType={host_type}")
    test_cmd.append(f"--hostPath={host_path}")
    if host_args != "":
        test_cmd.append(f"--hostArgs='{host_args}'")
    test_cmd.append(f"--threads={threads}")
    test_cmd.append(f"--mode={run_test262_mode(args)}")
    test_cmd.append(f"--timeout={timeout}")
    if platform.system() == "Windows" :
        global BASE_OUT_DIR
        global DATA_DIR
        BASE_OUT_DIR = BASE_OUT_DIR.replace("/","\\")
        DATA_DIR = DATA_DIR.replace("/","\\")
        execute_args = [p.replace("/","\\") for p in execute_args]
    test_cmd.append(f"--tempDir={BASE_OUT_DIR}")
    test_cmd.append(f"--test262Dir={DATA_DIR}")
    if args.test_list:
        test_cmd.append("--isTestListSet")
    if args.babel:
        test_cmd.append("--preprocessor='test262/babel-preprocessor.js'")
    test_cmd.append(DEFAULT_OTHER_ARGS)
    test_cmd.extend(execute_args)

    run_check(test_cmd)

Check = collections.namedtuple('Check', ['enabled', 'runner', 'arg'])


def prepare_test_list(args) -> List[str]:
    if not os.path.exists(args.test_list):
        args.test_list = os.path.join("test262", args.test_list)
    test_list = TestPrepare.get_tests_from_file(args.test_list)
    dirs: List[str] = []
    for test in test_list:
        parts = test.split(os.path.sep)[:3]
        dirs.append(os.path.sep.join(parts))
    return list(set(dirs))


def reset_args(args):
    args.es51 = None
    args.es2015 = None
    args.intl = None
    args.other = None
    args.es2021 = None
    args.es2022 = None
    args.es2023 = None
    args.ci_build = None
    args.esnext = None
    args.dir = None


def prepare_file_from_test_list(args, test_prepare):
    folders = prepare_test_list(args)
    for folder in folders:
        reset_args(args)
        args.dir = folder
        test_prepare.run()
    args.file = args.test_list


def run(args):
    init(args)

    test_prepare = TestPrepare(args)
    test_prepare.get_code()
    if args.test_list:
        prepare_file_from_test_list(args, test_prepare)
    else:
        test_prepare.run()
    check = Check(True, run_test262_test, args)
    check.runner(check.arg)


def main(args):
    print("\nWait a moment..........\n")
    starttime = datetime.datetime.now()
    run(args)
    endtime = datetime.datetime.now()
    print(f"used time is: {str(endtime - starttime)}")


if __name__ == "__main__":
    #  Script returns 0 if it's completed despite whether there are some failed tests or no
    sys.exit(main(parse_args()))
