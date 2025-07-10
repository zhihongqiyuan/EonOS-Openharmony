#!/usr/bin/env python3
# coding=utf-8
#
# Copyright (c) 2024 Huawei Device Co., Ltd.
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


"""Module provides system functions like logging and open files."""
import logging
import os
import sys

logging.basicConfig(stream=sys.stdout, level=logging.DEBUG, format="%(levelname)s:[%(name)s]: %(message)s", force=True)
logging.root.name = "h_parser"

LOGGING = 0
DEBUG_LOGGING = 0
INFO_LOGGING = 1
DEBUG_LOGGING = 0
WARNING_LOGGING = 1
ERROR_LOGGIN = 1
PARSING_LOGGING = 0


LIB_GEN_FOLDER: str = ""


def init_log(lib_gen_folder: str) -> None:
    global LIB_GEN_FOLDER  # pylint: disable=W0603
    LIB_GEN_FOLDER = lib_gen_folder
    os.makedirs(os.path.join(LIB_GEN_FOLDER, "./gen/logs"), exist_ok=True)


def info_log(msg: str) -> None:
    if INFO_LOGGING:
        logging.info(msg)


def debug_log(msg: str) -> None:
    if DEBUG_LOGGING:
        logging.debug(msg)


def warning_log(msg: str) -> None:
    if WARNING_LOGGING:
        logging.warning(msg)


def parsing_failed_msg(file: str) -> None:
    logging.info("NON FATAL ERROR: Headers parser failed on {file}.\n"
                 "To reproduce locally: run 'ninja gen_yamls' and check "
                 "<es2panda_lib::binary_root>/gen/logs/error_logs.txt")


def error_log(msg: str) -> None:
    path = os.path.join(LIB_GEN_FOLDER, "./gen/logs/error_logs.txt")
    if ERROR_LOGGIN:
        with os.fdopen(os.open(path, os.O_WRONLY | os.O_CREAT | os.O_APPEND, mode=511), "a", encoding="utf-8") as f:
            f.write(msg + "\n")
