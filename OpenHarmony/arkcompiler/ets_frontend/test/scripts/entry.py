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

import argparse
import os
import subprocess
import time

import schedule

from utils.download_sdk_and_image.download import get_the_image


def prepare_test():
    arguments = parse_args()
    prepared = True
    if not arguments.skip_download_sdk:
        sdk_prepared = get_the_image('sdk', '', None, None)
        prepared = prepared and sdk_prepared
    if not arguments.skip_download_dayu:
        dayu_prepared = get_the_image('dayu', '', None, None)
        job(['python', './utils/flash_image/burn_image.py'])
        prepared = prepared and dayu_prepared
    job(['python', './utils/commit_message/get_commit_message.py'])
    return prepared


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('--skipDownloadSdk', dest='skip_download_sdk', action='store_true', default=False,
                        help='specify whether to skip the download sdk or not')
    parser.add_argument('--skipDownloadDayu', dest='skip_download_dayu', action='store_true', default=False,
                        help='specify whether to skip the download dayu or not')

    return parser.parse_args()


def job(cmd):
    subprocess.run(cmd, shell=False)


def run():
    if not prepare_test():
        return

    job(os.path.join(".", "auto_xts_test", "run.bat"))
    job(f'python {os.path.join(".", "sdk_test", "entry.py")}')
    job(f'python {os.path.join(".", "performance_test", "performance_entry.py")}')
    job(f'python {os.path.join(".", "utils", "send_email", "send_email.py")}')


if __name__ == '__main__':
    schedule.every().day.at("02:10").do(run)
    run()
    while True:
        schedule.run_pending()
        time.sleep(1)


