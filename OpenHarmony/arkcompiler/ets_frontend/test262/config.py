#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
Copyright (c) 2021-2022 Huawei Device Co., Ltd.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

Description: Execute 262 test suite configuration file
"""


import os
import sys
import platform
from multiprocessing import cpu_count

IS_LINUX_ARM64 = (sys.platform == "linux" and platform.machine().lower() == "aarch64")
CLANG_TOOLCHAIN = "clang_arm64" if IS_LINUX_ARM64 else "clang_x64"

DATA_DIR = os.path.join("test262", "data")
ESHOST_DIR = os.path.join("test262", "eshost")
HARNESS_DIR = os.path.join("test262", "harness")

BASE_OUT_DIR = os.path.join("out", "test262")

CUR_FILE_DIR = os.path.dirname(__file__)
CODE_ROOT = os.path.abspath(os.path.join(CUR_FILE_DIR, "../../.."))
if IS_LINUX_ARM64:
    LLVM_DIR = f"{CODE_ROOT}/prebuilts/clang/ohos/linux-aarch64/llvm/lib/"
else:
    LLVM_DIR = f"{CODE_ROOT}/prebuilts/clang/ohos/linux-x86_64/llvm/lib/"

DEFAULT_MODE = 2

TEST_FULL_DIR = os.path.join(DATA_DIR, "test")
TEST_ES5_DIR = os.path.join(DATA_DIR, "test_es51")
TEST_ES2015_DIR = os.path.join(DATA_DIR, "test_es2015")
TEST_ES2021_DIR = os.path.join(DATA_DIR, "test_es2021")
TEST_OTHERTESTS_DIR = os.path.join(DATA_DIR, "other_tests")
TEST_ES2022_DIR = os.path.join(DATA_DIR, "test_es2022")
TEST_ES2023_DIR = os.path.join(DATA_DIR, "test_es2023")
TEST_INTL_DIR = os.path.join(DATA_DIR, "test_intl")
TEST_CI_DIR = os.path.join(DATA_DIR, "test_CI")
TEST_SENDABLE_DIR = os.path.join(DATA_DIR, "test_sendable")

HISPARK_TAURUS_PRODUCT_NAME = "hispark_taurus"
RK3568_PRODUCT_NAME = "rk3568"
DEFAULT_PRODUCT_NAME = HISPARK_TAURUS_PRODUCT_NAME

ARGS_PREFIX = f"{CODE_ROOT}/out/"
ARK_DIR_SUFFIX = "clang_x64/arkcompiler/ets_frontend"
ICUI_DIR_SUFFIX = "clang_x64/thirdparty/icu"
ARK_JS_RUNTIME_DIR_SUFFIX = "clang_x64/arkcompiler/ets_runtime"
ZLIB_DIR_SUFFIX = "clang_x64/thirdparty/zlib"

DEFAULT_ARK_DIR = f"{CODE_ROOT}/out/hispark_taurus/clang_x64/arkcompiler/ets_frontend"
DEFAULT_ICUI_DIR = f"{CODE_ROOT}/out/hispark_taurus/clang_x64/thirdparty/icu"
DEFAULT_ARK_JS_RUNTIME_DIR = f"{CODE_ROOT}/out/hispark_taurus/clang_x64/arkcompiler/ets_runtime"
DEFAULT_ZLIB_DIR = f"{CODE_ROOT}/out/hispark_taurus/clang_x64/thirdparty/zlib"
DEFAULT_ICU_PATH = f"{CODE_ROOT}/third_party/icu/ohos_icu4j/data"

DEFAULT_ARK_TOOL = os.path.join(DEFAULT_ARK_JS_RUNTIME_DIR, "ark_js_vm")
DEFAULT_LIBS_DIR = f"{DEFAULT_ICUI_DIR}:{LLVM_DIR}:{DEFAULT_ARK_JS_RUNTIME_DIR}:{DEFAULT_ZLIB_DIR}"
DEFAULT_STUB_FILE = f""
DEFAULT_ARK_AOT_TOOL = os.path.join(DEFAULT_ARK_JS_RUNTIME_DIR, "ark_aot_compiler")

DEFAULT_HOST_TYPE = "panda"
DEFAULT_HOST_PATH = "python3"
DEFAULT_THREADS = min(cpu_count(), 32)
DEFAULT_OTHER_ARGS = "--saveCompiledTests"
TEST262_RUNNER_SCRIPT = os.path.join(HARNESS_DIR, "bin", "run.js")
DEFAULT_TIMEOUT = 60000

ES5_LIST_FILE = os.path.join("test262", "es5_tests.txt")
ES2015_LIST_FILE = os.path.join("test262", "es2015_tests.txt")
INTL_LIST_FILE = os.path.join("test262", "intl_tests.txt")
ES2021_LIST_FILE = os.path.join("test262", "es2021_tests.txt")
SENDABLE_LIST_FILE = os.path.join("test262", "sendable_tests.txt")
OTHER_LIST_FILE = os.path.join("test262", "other_tests.txt")
ES2022_LIST_FILE = os.path.join("test262", "es2022_tests.txt")
ES2023_LIST_FILE = os.path.join("test262", "es2023_tests.txt")
CI_LIST_FILE = os.path.join("test262", "CI_tests.txt")
MODULE_LIST = []
DYNAMIC_IMPORT_LIST = []
with open(os.path.join("test262", "module_tests.txt")) as m_file:
    MODULE_LIST = m_file.read().splitlines()
with open(os.path.join("test262", "dynamicImport_tests.txt")) as d_file:
    DYNAMIC_IMPORT_LIST = d_file.read().splitlines()

SENDABLE_GIT_HASH = "a0fa6f3636ffd11120d5465193b9d38455dcd102"
TEST262_GIT_HASH = "6f4601d095a3899d6102f2c320b671495cbe8757"
HARNESS_GIT_HASH = "9c499f028eb24e67781435c0bb442e00343eb39d"
ESHOST_GIT_HASH = "fa2d4d27d9d6152002bdef36ee2d17e98b886268"
ESNEXT_GIT_HASH = "281eb10b2844929a7c0ac04527f5b42ce56509fd"

SENDABLE_GIT_URL = "https://gitee.com/arkcompiler_sendable/test262_sendable.git"
TEST262_GIT_URL = "https://gitee.com/hufeng20/test262.git"
ESHOST_GIT_URL = "https://gitee.com/hufeng20/eshost.git"
HARNESS_GIT_URL = "https://gitee.com/hufeng20/test262-harness.git"

TEST262_JIT_GIT_URL = "https://gitee.com/lxy0115/test262.git"
TEST262_JIT_GIT_HASH = "1c923f96779aabbd4bed35a00e7078054c465334"
TEST262_JIT_LABEL = os.path.join("test262", "data", "preheated.txt")

SKIP_LIST_FILE = os.path.join("test262", "skip_tests.json")
ES2ABC_SKIP_LIST_FILE = os.path.join("test262", "es2abc_skip_tests.json")
TS2ABC_SKIP_LIST_FILE = os.path.join("test262", "ts2abc_skip_tests.json")
INTL_SKIP_LIST_FILE = os.path.join("test262", "intl_skip_tests.json")
TS2ABC_SKIP_FORCE_GC_LIST_FILE = os.path.join("test262", "force_gc_skip_tests.json")
SKIP_LIST_FILES = [SKIP_LIST_FILE, INTL_SKIP_LIST_FILE]
ALL_SKIP_TESTS = []
INTL_SKIP_TESTS = []
SKIP_FORCE_GC_LIST_FILES = []
FORCE_GC_SKIP_TESTS = []

ARK_FRONTEND_LIST = [
    "ts2panda",
    "es2panda"
]

ARK_FRONTEND_BINARY_LIST = [
    os.path.join(DEFAULT_ARK_DIR, "build", "src", "index.js"),
    os.path.join(DEFAULT_ARK_DIR, "es2abc")
]

DEFAULT_ARK_FRONTEND = ARK_FRONTEND_LIST[0]
DEFAULT_ARK_FRONTEND_BINARY = ARK_FRONTEND_BINARY_LIST[0]
DEFAULT_MERGE_ABC_BINARY = os.path.join(DEFAULT_ARK_DIR, "merge_abc")

ARK_ARCH_LIST = [
    "x64",
    "aarch64",
    "arm"
]

DEFAULT_ARK_ARCH = ARK_ARCH_LIST[0]
DEFAULT_OPT_LEVEL = 2
DEFAULT_ES2ABC_THREAD_COUNT = 0
DEFAULT_MERGE_ABC_MODE = 1

OHOS_TYPESCRIPT = "ohos-typescript-4.2.3-r2.tgz"
OHOS_TYPESCRIPT_TGZ_PATH = f"{CODE_ROOT}/third_party/typescript/build_package/{OHOS_TYPESCRIPT}"

DEFAULT_RETRIES = 3
