#!/usr/bin/env python3
# coding: utf-8

"""
Copyright (c) 2023 Huawei Device Co., Ltd.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

Description: entrance to run sdk test suite
"""

import logging
import os
import sys
import time

from execution import execute
from options import process_options
from preparation import prepare_test_env
from result import process_test_result


def run():
    old_env = os.environ.copy()
    try:
        start_time = time.time()
        test_tasks = process_options()
        if not test_tasks:
            logging.error("No test task found, test suite exit!")
            sys.exit(1)

        if not prepare_test_env():
            logging.error("Prepare test environment failed, test suite exit!")
            sys.exit(1)

        execute(test_tasks)
        process_test_result(test_tasks, start_time)
    except Exception as e:
        logging.exception(e)
    finally:
        os.environ.clear()
        os.environ.update(old_env)


if __name__ == '__main__':
    run()