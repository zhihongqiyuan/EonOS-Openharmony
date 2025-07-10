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

import os


class IdeType():
    AS = 1
    DevEco = 2


class Config():
    send_mail = True
    run_list = ["HelloWorld"]

    def __init__(self):
        # Default config settings for all projects, if it's not what you need, config them in application_configs
        # Set the path of your deveco studio
        self.deveco_path = r"xxx/DevEco Studio"
        self.cmd_prefix = os.path.join(self.deveco_path, 'tools', 'hvigor', 'bin', 'hvigorw.js')
        self.node_js_path = os.path.join(self.deveco_path, 'tools', 'node', 'node.exe')
        self.jbr_path = os.path.join(self.deveco_path, 'jbr')

        self.output_split = "_"
        self.ide_filename = ["AS", "DevEco"]
        self.debug_or_release = ["Debug", "Release"]
        self.build_type_of_log = ["full_then_incremental", "add_code_and_ui"]
        self.log_filename = ["size_all.csv", "size_avg.csv",
                             "time_all.csv", "time_avg.csv"]
        self.error_filename = 'error.log'
        self.ide = IdeType.DevEco
        self.incremental_code_str = "let index = 5 + 6\n"
        self.incremental_code_start_pos = "let index = 5 + 6\n"
        self.incremental_code_end_pos = 'this.num = num'
        self.preview_debug_suffix = ['--mode', 'module', '-p', 'module=entry@default', '-p', 'product=default', '-p',
                                     'buildMode=release', 'assembleHap', '--verbose-analyze', '--parallel',
                                     '--incremental', '--daemon', '--info']
        self.preview_release_suffix = ['--mode', 'module', '-p', 'module=entry@default', '-p', 'product=default', '-p',
                                       'buildMode=release', 'assembleHap', '--verbose-analyze', '--parallel',
                                       '--incremental', '--daemon', '--info']
        self.preview_build = False,
        self.preview_cache_path = ['entry', '.preview']
        self.cmd_debug_suffix = ['--mode', 'module', '-p', 'product=default', 'module=entry@default', '-p',
                                 'buildMode=debug', 'assembleHap', '--info', '--verbose-analyze', '--parallel',
                                 '--incremental', '--daemon']
        self.cmd_release_suffix = ['--mode', 'module', '-p', 'product=default', 'module=entry@default', '-p',
                                   'buildMode=release', 'assembleHap', '--info', '--verbose-analyze', '--parallel',
                                   '--incremental', '--daemon']
        self.debug_package_path = r'entry/build/default/outputs/default/entry-default-signed.hap'
        self.release_package_path = r'entry/build/default/outputs/default/entry-default-unsigned.hap'
        self.incremental_code_path = r'entry/src/main/ets/pages/Index.ets'
        self.json5_path = r'build-profile.json5'

        # build serveral times then calculate the average value
        self.build_times = 3
        # Debug this script fastly with skipping building and will use test data.
        # Use test_report.json to test the build succeed case.
        # Use test_error_report.json the build failed case.
        # Use '' to run the real build.
        self.developing_test_data_path = ''

    # If Default config is not what you need, you can set here!
    application_configs = dict(
        [
            (
                "FTB", dict
                    (
                        project_path=r"D:/FTB",
                        name='FTB',
                    )
            ),
            (
                "FDY", dict
                    (
                        project_path=r"D:/FDY",
                        name='FDY',
                    )
            ),
            (
                "FWX", dict
                    (
                        project_path=r"D:/FWX",
                        name='FWX',
                    )
            ),
            (
                "HelloWorld", dict
                    (
                        # The following params must be set according you environment
                        project_path=r"D:/HelloWorld",
                        name='HelloWorld',

                        # The following params is not neccessary to be modified
                        preview_build=True
                        debug_package_path=r'entry/build/default/outputs/default/entry-default-unsigned.hap',
                        release_package_path=r'entry/build/default/outputs/default/entry-default-unsigned.hap',
                        incremental_code_path=r'entry/src/main/ets/pages/Index.ets',
                        incremental_code_end_pos='build() {',
                        incremental_code_str="a: number=5 + 6\n",
                        incremental_code_start_pos="a: number=5 + 6\n",

                        # This app will show the time costs in html
                        # you can setting this as a global setting to show all applications.
                        show_time_detail_filter=["createProgram", "arkTSLinter", "tsProgramEmit",
                            "generate content and source map information", "write obfuscated source code",
                            "write source map (async)", "generate merged abc by es2abc (async)", "total build cost"
                        ]
                    )
            )
        ]
    )


def get_config(index):
    config = Config()
    res = config.application_configs.get(index)
    if not res:
        print("No key in config, please check: " + index)
        return res
    for k in res:
        setattr(config, k, res[k])
    if not hasattr(config, 'name'):
        setattr(config, 'name', os.path.basename(config.project_path))
    return config


def get_html_prefix():
    return '<html><body><table width="100%" border=1 cellspacing=0 cellpadding=0 align="center">' + \
           '<tr><th bgcolor="SlateBlue"><font size="5">Daily Performance Test</font></th></tr></table>' + \
           '<font size="5" color=red>{}' + \
           '<img src="cid:performance10"><img src="cid:performance11">' + \
           '<img src="cid:performance00"><img src="cid:performance01">' + \
           '<img src="cid:performance02"><img src="cid:performance12">' + \
           '<img src="cid:performance30"><img src="cid:performance31">' + \
           '<img src="cid:performance40"><img src="cid:performance41">'

def get_html_suffix():
    return '</body></html>'


class BuildMode():
    DEBUG = 0
    RELEASE = 1


class LogType():
    FULL = 0
    INCREMENTAL = 1
    PREVIEW = 2
    SIZE = 3
    OBFUSCATION = 4


class MailPicConfig():
    mail_data_path = os.path.join(
        os.path.dirname(os.path.abspath(__file__)),
        'mail_data',
    )

    html_file_path = os.path.join(
        mail_data_path,
        "email_msg.html"
    )

    attach_path = os.path.join(
        mail_data_path,
        "performance_logs.zip"
    )

    # Count of days which will be add into the email picture
    mail_pic_table_name = {
        BuildMode.DEBUG: {
            LogType.FULL: os.path.join(mail_data_path, 'debug_full_time.csv'),
            LogType.INCREMENTAL: os.path.join(mail_data_path, 'debug_incremental_time.csv'),
            LogType.PREVIEW: os.path.join(mail_data_path, 'debug_preview_model_build_time.csv'),
            LogType.SIZE: os.path.join(mail_data_path, 'debug_size.csv')
        },
        BuildMode.RELEASE: {
            LogType.FULL: os.path.join(mail_data_path, 'release_full_time.csv'),
            LogType.INCREMENTAL: os.path.join(mail_data_path, 'release_incremental_time.csv'),
            LogType.PREVIEW: os.path.join(mail_data_path, 'release_preview_model_build_time.csv'),
            LogType.SIZE: os.path.join(mail_data_path, 'release_size.csv')
        }
    }

    mail_pic_name = {
        BuildMode.DEBUG: {
            LogType.FULL: os.path.join(mail_data_path, 'debug_full_time.jpg'),
            LogType.INCREMENTAL: os.path.join(mail_data_path, 'debug_incremental_time.jpg'),
            LogType.PREVIEW: os.path.join(mail_data_path, 'debug_preview_model_build_time.jpg'),
            LogType.SIZE: os.path.join(mail_data_path, 'debug_size.jpg')
        },
        BuildMode.RELEASE: {
            LogType.FULL: os.path.join(mail_data_path, 'release_full_time.jpg'),
            LogType.INCREMENTAL: os.path.join(mail_data_path, 'release_incremental_time.jpg'),
            LogType.PREVIEW: os.path.join(mail_data_path, 'release_preview_model_build_time.jpg'),
            LogType.SIZE: os.path.join(mail_data_path, 'release_size.jpg')
        }
    }

    mail_pic_table_label = {
        BuildMode.DEBUG: {
            LogType.FULL: 'Debug Full Build',
            LogType.INCREMENTAL: 'Debug Incremental Build',
            LogType.PREVIEW: 'Debug Preview Model Build',
            LogType.SIZE: 'Debug Full Build size'
        },
        BuildMode.RELEASE: {
            LogType.FULL: 'Release Full Build',
            LogType.INCREMENTAL: 'Release Incremental Time',
            LogType.PREVIEW: 'Release Preview Model Build',
            LogType.SIZE: 'Release Full Build size'
        }
    }