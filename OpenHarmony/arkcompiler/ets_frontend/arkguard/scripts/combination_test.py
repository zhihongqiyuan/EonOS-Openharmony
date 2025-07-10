#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
Copyright (c) 2024 Huawei Device Co., Ltd.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""

from grammar_test import Runner, parse_args, Task
import os
import json
import copy
import shutil
import stat

combination_config_path = os.path.join(os.path.dirname(__file__), 'combination_config.json')
config_temp_dir = os.path.join(os.path.dirname(__file__), '../test/local/temp_configs')
INDENTATION = 2
TEST_TYPE = 'combinations'

DEFAULT_CONFIG = {
    'mCompact': False,
    'mRemoveComments': False,
    'mOutputDir': '',
    'mDisableConsole': False,
    'mSimplify': False,
    'mNameObfuscation': {
        'mEnable': True,
        'mNameGeneratorType': 1,
        'mDictionaryList': [],
        'mRenameProperties': False,
        'mKeepStringProperty': True,
        'mTopLevel': False,
        'mKeepParameterNames': False
    },
    'mExportObfuscation': False,
    'mEnableSourceMap': False,
    'mEnableNameCache': False,
    'mKeepFileSourceCode': {
        'mKeepSourceOfPaths': [],
        'mkeepFilesAndDependencies': []
    },
    'mRenameFileName': {
        'mEnable': False,
        'mNameGeneratorType': 1,
        'mReservedFileNames': [],
        'mOhmUrlUseNormalized': False
    }
}

# This alias is used as the output directory for the current obfuscation options.
CONFIG_ALIAS = {
    'mCompact': 'comp',
    'mRemoveComments': 'rmComments',
    'mDisableConsole': 'con',
    'mNameObfuscation': {
        'mEnable': 'localVar',
        'mRenameProperties': 'prop',
        'mKeepStringProperty': 'strProp',
        'mTopLevel': 'top',
        'mKeepParameterNames': 'keepPara'
    },
    'mExportObfuscation': 'export',
    'mEnableSourceMap': 'sourcemap',
    'mEnableNameCache': 'namecache'
}


class CombinationRunner(Runner):
    def __init__(self, test_filter, root_dir, test_type):
        super().__init__(test_filter, root_dir, test_type)
        self.configs = []
        self.combinations = []
        self.temp_files = []
        self.obfscated_cache_root_dir = os.path.normpath(root_dir)
        if not os.path.exists(config_temp_dir):
            os.makedirs(config_temp_dir)

    def prepare_task(self, input_dirs, config_path):
        for input_dir in input_dirs:
            input_abs_dir = os.path.normpath(os.path.join(os.path.dirname(__file__), input_dir))
            for one_case_path in os.listdir(input_abs_dir):
                input_path = os.path.join(input_abs_dir, one_case_path)
                task = Task(input_path, config_path, TEST_TYPE)
                self.obfuscation_tasks.append(task)

    def recursive_search(self, config_part, input_part, result):
        for key in input_part:
            if key not in config_part:
                continue
            input_val = input_part[key]
            config_val = config_part[key]

            if isinstance(input_val, dict):
                self.recursive_search(config_val, input_val, result)
            else:
                result.append(config_val)

    def get_alias_from_config(self, config, input_data):
        result = []
        self.recursive_search(config, input_data, result)
        return '+'.join(result) if result else 'default'

    def merge_deep(self, target, source):
        for key, value in source.items():
            if isinstance(value, dict) and key in target:
                target[key] = self.merge_deep(target.get(key, {}), value)
            else:
                target[key] = value
        return target

    def merge_config(self, options, options_alias, whitelist, output_dir):
        whitelist_config = whitelist.get(options_alias, {})
        output_config = {'mOutputDir': output_dir}
        option_config = self.merge_deep(copy.deepcopy(whitelist_config), copy.deepcopy(options))
        option_config = self.merge_deep(option_config, output_config)
        merged_config = self.merge_deep(copy.deepcopy(DEFAULT_CONFIG), option_config)
        return merged_config

    def generate_config(self, combinations, input_dirs, output_dir, whitelist):
        output_abs_dir = os.path.join(os.path.dirname(combination_config_path), output_dir)
        for combination in combinations:
            alias_str = self.get_alias_from_config(CONFIG_ALIAS, combination)
            output_dir_for_current_option = os.path.normpath(os.path.join(output_abs_dir, alias_str))
            temp_config_path = os.path.normpath(os.path.join(config_temp_dir, alias_str + '_config.json'))
            merged_config = self.merge_config(combination, alias_str, whitelist, str(output_dir_for_current_option))
            flags = os.O_WRONLY | os.O_CREAT | os.O_TRUNC
            modes = stat.S_IRUSR | stat.S_IWUSR
            # write temp config file
            with os.fdopen(os.open(temp_config_path, flags, modes), 'w') as config_file:
                json.dump(merged_config, config_file, indent=INDENTATION)
            self.temp_files.append(temp_config_path)
            self.prepare_task(input_dirs, temp_config_path)


    def delete_temp_config_files(self):
        for path in self.temp_files:
            os.remove(path)
        self.temp_files.clear()


    def combine(self, current, remaining_keys, configs, result):
        if not remaining_keys:
            return

        key = remaining_keys[0]
        value = configs[key]

        if isinstance(value, dict):
            sub_keys = value.keys()
            sub_combinations = []

            for sub_key in sub_keys:
                sub_value = value[sub_key]
                current_sub_combinations = sub_combinations[0:]
                for comb in current_sub_combinations:
                    new_comb = {**comb, sub_key: sub_value}
                    sub_combinations.append(new_comb)
                sub_combinations.append({sub_key: sub_value})

            for sub_comb in sub_combinations:
                new_comb = {**current, key: sub_comb}
                result.append(new_comb)
                self.combine(new_comb, remaining_keys[1:], configs, result)
        else:
            new_comb = {**current, key: value}
            result.append(new_comb)
            self.combine(new_comb, remaining_keys[1:], configs, result)

        self.combine(current, remaining_keys[1:], configs, result)

    def generate_combinations(self, configs):
        result = [{}]  # Initialize with the empty object
        keys = list(configs.keys())
        self.combine({}, keys, configs, result)
        return result

    def parse_configs_and_execute(self):
        with open(combination_config_path, 'r', encoding='utf-8') as file:
            configs = json.load(file)
        # Parse each configuration in combination_config.json and then execute the test.
        for key, config in configs.items():
            enable_options = config.get('enableOptions', {})
            input_dirs = config.get('inputDirs', [])
            output_dir = config.get('outputDir', '')
            whitelist = config.get('whitelist', {})
            combinations = self.generate_combinations(enable_options)
            print('Obfuscation option combinations count:', len(combinations))
            self.generate_config(combinations, input_dirs, output_dir, whitelist)
            self.obfuscate()
            self.delete_temp_config_files()

    def try_remove_cache(self):
        if not self.has_failed_cases():
            shutil.rmtree(self.obfscated_cache_root_dir)


def main():
    args = parse_args()
    local_root_dir = os.path.join(os.path.dirname(__file__), "../test/local/combinations")
    runner = CombinationRunner(args.test_filter, local_root_dir, TEST_TYPE)
    runner.parse_configs_and_execute()
    runner.run_with_node()
    runner.content_compare()
    runner.print_summary()

    runner.try_remove_cache()


if __name__ == '__main__':
    main()
