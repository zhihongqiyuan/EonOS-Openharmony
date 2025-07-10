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

Description: execute test tasks
"""
import copy
import json
import logging
import os
import re
import shutil
import signal
import subprocess
import tarfile
import time
import zipfile
from contextlib import contextmanager

import json5

import options
import utils


class FullTest:
    @staticmethod
    def prepare_full_task(task, test_name):
        if test_name in task.full_compilation_info:
            full_task = task.full_compilation_info[test_name]
        else:
            full_task = options.FullCompilationInfo()
            full_task.name = test_name
            task.full_compilation_info[test_name] = full_task
        return full_task

    @staticmethod
    def full_compile(task, is_debug):
        test_name = "full_compile"
        logging.info(f"==========> Running {test_name} for task: {task.name}")
        clean_compile(task)

        full_task = FullTest.prepare_full_task(task, test_name)
        [stdout, stderr] = compile_project(task, is_debug)
        passed = validate(full_task, task, is_debug, stdout, stderr, f'{test_name}')
        if passed:
            backup_compile_output(task, is_debug)

        return passed

    @staticmethod
    def compile_full_import_ordinary_ohpm_package(task, is_debug):
        test_name = 'import_ordinary_ohpm_package'
        clean_compile(task)
        full_task = FullTest.prepare_full_task(task, test_name)
        info = full_task.debug_info if is_debug else full_task.release_info
        logging.info(f"==========> Running {test_name} for task: {task.name}")

        try:
            modify_file = os.path.join(task.path, *task.inc_modify_file)
            patch_content = (options.configs.get('patch_content').get('ohpm_package_patch')
                             .get('ordinary_package'))
            head_content = patch_content.get('head')
            tail_content = patch_content.get('tail')
            utils.add_content_to_file(modify_file, head_content, tail_content)
            build_passed, build_time = is_build_module_successful(task, is_debug, info,
                                                                  '', f'full_compile_{test_name}')
            if not build_passed:
                return
            package_name = patch_content.get('name')
            is_included = is_npm_txt_included_ohpm_package(info, task, is_debug, package_name)
            if is_included:
                info.result = options.TaskResult.passed
                info.time = build_time
        finally:
            utils.remove_content_from_file(modify_file, head_content, tail_content)

    @staticmethod
    def compile_full_import_special_ohpm_package(task, is_debug):
        test_name = 'import_special_ohpm_package'
        clean_compile(task)
        full_task = FullTest.prepare_full_task(task, test_name)
        info = full_task.debug_info if is_debug else full_task.release_info
        logging.info(f"==========> Running {test_name} for task: {task.name}")

        try:
            modify_file = os.path.join(task.path, *task.inc_modify_file)
            patch_content = (options.configs.get('patch_content').get('ohpm_package_patch')
                             .get('special_package'))
            head_content = patch_content.get('head')
            tail_content = patch_content.get('tail')
            utils.add_content_to_file(modify_file, head_content, tail_content)
            build_passed, build_time = is_build_module_successful(task, is_debug, info,
                                                                  '', f'full_compile_{test_name}')
            if not build_passed:
                logging.error(f'Test：{test_name} failed,due to full compilation failed')
                return
            package_name = patch_content.get('name')
            disasm_file_path = get_disasm_abc_file(task, info, 'Hap')
            is_contained = utils.file_contains_specified_fields(disasm_file_path, package_name)
            if is_contained:
                info.result = options.TaskResult.passed
                info.time = build_time
        finally:
            utils.remove_content_from_file(modify_file, head_content, tail_content)

    @staticmethod
    def compile_full_import_static_library(task, is_debug):
        test_name = 'import_static_library'
        clean_compile(task)
        full_task = FullTest.prepare_full_task(task, test_name)
        info = full_task.debug_info if is_debug else full_task.release_info
        logging.info(f"==========> Running {test_name} for task: {task.name}")

        with manage_module_import_and_export_handle(task, 'Har'):
            build_passed, build_time = is_build_module_successful(task, is_debug, info, '',
                                                                  f'full_compile_{test_name}')
            if not build_passed:
                logging.error(f'Test：{test_name} failed,due to full compilation failed')
                return
            pa_file = get_disasm_abc_file(task, info, 'Hap')
            if not pa_file:
                return
            is_packaged = is_package_modules_to_module_abc(task, pa_file, task.har_module)
            if is_packaged:
                info.result = options.TaskResult.passed
                info.time = build_time
            else:
                info.result = options.TaskResult.failed
                info.error_message = f'Har was not properly packaged into module abc'

    @staticmethod
    def compile_full_import_share_library(task, is_debug):
        test_name = 'import_share_library'
        clean_compile(task)
        full_task = FullTest.prepare_full_task(task, test_name)
        info = full_task.debug_info if is_debug else full_task.release_info
        logging.info(f"==========> Running {test_name} for task: {task.name}")

        with manage_module_import_and_export_handle(task, 'Hsp'):
            build_passed, build_time = is_build_module_successful(task, is_debug, info, '',
                                                                  f'full_compile_{test_name}')
            if not build_passed:
                logging.error(f'Test：{test_name} failed,due to full compilation failed')
                return
            pa_file = get_disasm_abc_file(task, info, 'Hap')
            if not pa_file:
                return
            is_packaged = is_package_modules_to_module_abc(task, pa_file, task.hsp_module)
            if not is_packaged:
                info.result = options.TaskResult.passed
                info.time = build_time
            else:
                info.result = options.TaskResult.failed
                info.error_message = f'Unexpected changes have occurred.Hsp should not be packaged into module abc'

    @staticmethod
    def compile_full_import_so_file(task, is_debug):
        test_name = 'import_so_file'
        clean_compile(task)
        full_task = FullTest.prepare_full_task(task, test_name)
        info = full_task.debug_info if is_debug else full_task.release_info
        logging.info(f"==========> Running {test_name} for task: {task.name}")

        with manage_module_import_and_export_handle(task, 'Cpp'):
            [stdout, stderr] = compile_project(task, is_debug)
            is_success, build_time = is_compile_success(stdout)
            if not is_success:
                logging.error(f'Test：{test_name} failed,due to full compilation failed')
                info.result = options.TaskResult.failed
                info.error_message = stderr
                return
            validate(full_task, task, is_debug, stdout, stderr)

    @staticmethod
    def compile_full_has_syntax_error_in_js(task, is_debug):
        test_name = 'has_syntax_error_in_js'
        clean_compile(task)
        full_task = FullTest.prepare_full_task(task, test_name)
        info = full_task.debug_info if is_debug else full_task.release_info
        logging.info(f"==========> Running {test_name} for task: {task.name}")

        try:
            add_or_delete_js_file(task, 1, True)
            patch_lines_error = options.configs.get('patch_content').get('patch_lines_error')
            expected_error = patch_lines_error.get('expected_error')
            [stdout, stderr] = compile_project(task, is_debug)
            is_passed = is_get_expected_error(info, stderr, expected_error)
            if is_passed:
                info.result = options.TaskResult.passed

        finally:
            add_or_delete_js_file(task, 0)

    @staticmethod
    def compile_full_use_normalize_ohmurl(task, is_debug):
        test_name = 'use_normalize_ohmurl'
        clean_compile(task)
        full_task = FullTest.prepare_full_task(task, test_name)
        info = full_task.debug_info if is_debug else full_task.release_info
        logging.info(f"==========> Running {test_name} for task: {task.name}")

        modify_normalize_ohmurl_options(task, 1)
        try:
            with manage_module_import_and_export_handle(task, 'Har'):
                is_build_passed, build_time = is_build_module_successful(task, is_debug, info,
                                                                         '', f'full_compile_{test_name}')
                if not is_build_passed:
                    logging.error(f'Test：{test_name},full compilation failed with use normalize ohmurl option')
                    return
                is_passed = is_normalized_ohm_url(task, is_debug, info)
                if is_passed:
                    info.result = options.TaskResult.passed
                    info.time = build_time
        finally:
            modify_normalize_ohmurl_options(task, 0)

    @staticmethod
    def compile_full_module_name_is_inconsistent(task, is_debug):
        test_name = 'module_name_is_inconsistent'
        clean_compile(task)
        full_task = FullTest.prepare_full_task(task, test_name)
        info = full_task.debug_info if is_debug else full_task.release_info
        logging.info(f"==========> Running {test_name} for task: {task.name}")

        oh_package_json_path = os.path.join(task.path, *task.hap_module_path, 'oh-package.json5')
        json_data = {}
        try:
            with open(oh_package_json_path, 'r+', encoding='utf-8') as json_file:
                json_data = json5.load(json_file)
                bak_data = copy.deepcopy(json_data)
                dependencies_dic = json_data["dependencies"]
                patch_lines = options.configs.get('patch_content').get('patch_lines_1')
                dependency_name = utils.extract_library_names(patch_lines.get('har').get('head'))
                module_name = task.har_module.capitalize()
                dependencies_dic[dependency_name] = os.path.normpath(f"file:../{module_name}")
                json_file.seek(0)
                json.dump(json_data, json_file, indent=4)
                json_file.truncate()
            sync_project(task)

            with manage_module_import_and_export_handle(task, 'Har'):
                is_build_successful, build_time = is_build_module_successful(task, is_debug, info,
                    '', 'full_compile_module_name_is_inconsistent')
                if is_build_successful:
                    info.result = options.TaskResult.passed
                    info.time = build_time
        finally:
            with open(oh_package_json_path, 'w', encoding='utf-8') as json_file:
                json.dump(bak_data, json_file, indent=4)
            sync_project(task)


class IncrementalTest:
    @staticmethod
    def validate_module_name_change(task, inc_task, is_debug, stdout, stderr, new_module_name):
        output_file = get_compile_output_file_path(task, is_debug, options.OutputType.unsigned)
        output_dir = os.path.dirname(output_file)
        output_file_name = os.path.basename(output_file)
        output_file_name_items = output_file_name.split(
            '-')  # hap name format: entry-default.hap
        output_file_name_items[0] = new_module_name
        output_file_name = '-'.join(output_file_name_items)
        new_module_name_output_file = os.path.join(
            output_dir, output_file_name)

        logging.debug(f"new module hap file: {new_module_name_output_file}")

        passed = validate(inc_task, task, is_debug, stdout,
                          stderr, 'incremental_compile_change_module_name',
                          new_module_name_output_file)
        logging.debug(f"validate new module hap file, passed {passed}")
        if not passed:
            return

        if is_debug:
            inc_info = inc_task.debug_info
        else:
            inc_info = inc_task.release_info
        uncompressed_output_file = new_module_name_output_file + '.uncompressed'
        with zipfile.ZipFile(new_module_name_output_file, 'r') as zip_ref:
            zip_ref.extractall(uncompressed_output_file)

        abc_path = os.path.join(uncompressed_output_file, 'ets')
        modules_abc_path = os.path.join(abc_path, 'modules.abc')
        modules_pa = disasm_abc(task, modules_abc_path)
        if not modules_pa or not os.path.exists(modules_pa):
            inc_info.result = options.TaskResult.failed
            inc_info.error_message = 'ark_disasm failed, module name change not verified'
            return

        func_str = ''
        with open(modules_pa, 'r', encoding='utf-8') as pa:
            line = pa.readline()
            while line:
                if '.function' in line.strip():
                    func_str = line.strip()
                    break
                line = pa.readline()

        func_define_items = func_str.split('.')
        if not new_module_name in func_define_items:
            inc_info.result = options.TaskResult.failed
            inc_info.error_message = f'expected entry name {new_module_name} in function name, \
                                     actual function name: {func_str}'

        shutil.rmtree(uncompressed_output_file)

    @staticmethod
    def is_file_in_modified_files(task_type, backup_file_relative_path, modified_cache_files):
        if 'stage' in task_type:
            return backup_file_relative_path in modified_cache_files
        else:
            non_temporary_path = backup_file_relative_path.split("temporary")[
                1].lstrip(os.path.sep)
            logging.debug(f"non_temporary_path: {non_temporary_path}")
            for file in modified_cache_files:
                logging.debug(f"modified_cache_files file: {file}")
                if non_temporary_path in file:
                    return True
        return False

    @staticmethod
    def validate_compile_incremental_file(task, inc_task, is_debug, modified_files, module=''):
        module_path = utils.get_module_path(task, module)
        if is_debug:
            cache_path = os.path.join(
                task.path, *module_path, *task.build_path, *task.cache_path, 'debug')
            backup_path = task.backup_info.cache_debug
            inc_info = inc_task.debug_info
        else:
            cache_path = os.path.join(
                task.path, *module_path, *task.build_path, *task.cache_path, 'release')
            backup_path = task.backup_info.cache_release
            inc_info = inc_task.release_info

        validate_cache_file(task, inc_info, modified_files, cache_path, backup_path)

    @staticmethod
    def prepare_incremental_task(task, test_name):
        if test_name in task.incre_compilation_info:
            inc_task = task.incre_compilation_info[test_name]
        else:
            inc_task = options.IncCompilationInfo()
            inc_task.name = test_name
            task.incre_compilation_info[test_name] = inc_task
        return inc_task

    @staticmethod
    def compile_incremental_no_modify(task, is_debug):
        test_name = 'no_change'
        inc_task = IncrementalTest.prepare_incremental_task(task, test_name)

        logging.info(f"==========> Running {test_name} for task: {task.name}")
        [stdout, stderr] = compile_project(task, is_debug)
        passed = validate(inc_task, task, is_debug, stdout, stderr, 'incremental_compile_no_change')
        if passed:
            IncrementalTest.validate_compile_incremental_file(
                task, inc_task, is_debug, [])

    @staticmethod
    def compile_incremental_add_oneline(task, is_debug):
        test_name = 'add_oneline'
        inc_task = IncrementalTest.prepare_incremental_task(task, test_name)

        logging.info(f"==========> Running {test_name} for task: {task.name}")
        modify_file_item = task.inc_modify_file
        modify_file = os.path.join(task.path, *modify_file_item)
        modify_file_backup = modify_file + ".bak"
        shutil.copyfile(modify_file, modify_file_backup)

        with open(modify_file, 'a', encoding='utf-8') as file:
            file.write(options.configs.get('patch_content').get(
                'patch_lines_2').get('tail'))

        [stdout, stderr] = compile_project(task, is_debug)
        passed = validate(inc_task, task, is_debug, stdout, stderr, 'incremental_compile_add_oneline')
        if passed:
            modified_files = [os.path.join(*modify_file_item)]
            IncrementalTest.validate_compile_incremental_file(
                task, inc_task, is_debug, modified_files)

        shutil.move(modify_file_backup, modify_file)

    @staticmethod
    def compile_incremental_add_file(task, is_debug):
        test_name = 'add_file'
        inc_task = IncrementalTest.prepare_incremental_task(task, test_name)

        logging.info(f"==========> Running {test_name} for task: {task.name}")
        modify_file_item = task.inc_modify_file
        modify_file = os.path.join(task.path, *modify_file_item)
        modify_file_backup = modify_file + ".bak"
        shutil.copyfile(modify_file, modify_file_backup)

        modify_dir = os.path.dirname(modify_file)
        if 'js' in task.type:
            patch_content = options.configs.get(
                'patch_content').get('patch_new_file_js')
            new_file_name = patch_content.get('name')
            new_file_content = patch_content.get('content')
        else:
            patch_content = options.configs.get(
                'patch_content').get('patch_new_file_ets')
            new_file_name = patch_content.get('name')
            new_file_content = patch_content.get('content')
        new_file = os.path.join(modify_dir, new_file_name)

        with open(new_file, 'w', encoding='utf-8') as file:
            file.writelines(new_file_content)

        with open(modify_file, 'r+', encoding='utf-8') as file:
            old_content = file.read()
            file.seek(0)
            patch_lines = options.configs.get(
                'patch_content').get('patch_lines_1').get('js')
            file.write(patch_lines.get('head'))
            file.write(old_content)
            file.write(patch_lines.get('tail'))

        [stdout, stderr] = compile_project(task, is_debug)
        passed = validate(inc_task, task, is_debug, stdout, stderr, 'incremental_compile_add_file')
        if passed:
            modified_files = [os.path.join(*modify_file_item)]
            IncrementalTest.validate_compile_incremental_file(
                task, inc_task, is_debug, modified_files)

        shutil.move(modify_file_backup, modify_file)
        os.remove(new_file)

    @staticmethod
    def compile_incremental_add_nonexistent_file(task, is_debug):
        test_name = 'add_nonexistent_file'
        inc_task = IncrementalTest.prepare_incremental_task(task, test_name)

        logging.info(f"==========> Running {test_name} for task: {task.name}")

        modify_file_item = task.inc_modify_file
        modify_file = os.path.join(task.path, *modify_file_item)
        modify_file_backup = modify_file + ".bak"
        shutil.copyfile(modify_file, modify_file_backup)

        with open(modify_file, 'r+', encoding='utf-8') as file:
            old_content = file.read()
            file.seek(0)
            patch_lines = options.configs.get(
                'patch_content').get('patch_lines_1').get('js')
            file.write(patch_lines.get('head'))
            file.write(old_content)
            file.write(patch_lines.get('tail'))

        info = inc_task.debug_info if is_debug else inc_task.release_info
        expected_errors = options.configs.get('patch_content').get('patch_file_error').get('expected_error')
        [stdout, stderr] = compile_project(task, is_debug)
        passed = is_get_expected_error(info, stderr, expected_errors)
        if passed:
            logging.info("The first compilation file does not exist. The compilation fails as expected")

            modify_dir = os.path.dirname(modify_file)
            if 'js' in task.type:
                patch_content = options.configs.get(
                    'patch_content').get('patch_new_file_js')
                new_file_name = patch_content.get('name')
                new_file_content = patch_content.get('content')
            else:
                patch_content = options.configs.get(
                    'patch_content').get('patch_new_file_ets')
                new_file_name = patch_content.get('name')
                new_file_content = patch_content.get('content')
            new_file = os.path.join(modify_dir, new_file_name)

            with open(new_file, 'w', encoding='utf-8') as file:
                file.writelines(new_file_content)

            [stdout, stderr] = compile_project(task, is_debug)
            passed = validate(inc_task, task, is_debug, stdout, stderr, 'incremental_compile_add_nonexistent_file')
            if passed:
                modified_files = [os.path.join(*modify_file_item)]
                IncrementalTest.validate_compile_incremental_file(
                    task, inc_task, is_debug, modified_files)
            os.remove(new_file)
        shutil.move(modify_file_backup, modify_file)

    @staticmethod
    def compile_incremental_delete_file(task, is_debug):
        test_name = 'delete_file'
        inc_task = IncrementalTest.prepare_incremental_task(task, test_name)

        logging.info(f"==========> Running {test_name} for task: {task.name}")
        # this test is after 'add_file', and in test 'add_file' already done remove file,
        # so here just call compile
        [stdout, stderr] = compile_project(task, is_debug)
        passed = validate(inc_task, task, is_debug, stdout, stderr, 'incremental_compile_delete_file')
        if passed:
            modify_file_item = task.inc_modify_file
            modified_files = [os.path.join(*modify_file_item)]
            IncrementalTest.validate_compile_incremental_file(
                task, inc_task, is_debug, modified_files)

    @staticmethod
    def compile_incremental_reverse_hap_mode(task, is_debug):
        test_name = 'reverse_hap_mode'
        inc_task = IncrementalTest.prepare_incremental_task(task, test_name)

        logging.info(f"==========> Running {test_name} for task: {task.name}")
        hap_mode = not is_debug
        [stdout, stderr] = compile_project(task, hap_mode)
        validate(inc_task, task, hap_mode, stdout, stderr, 'incremental_compile_reverse_hap_mode')

    @staticmethod
    def compile_incremental_modify_module_name(task, is_debug):
        if 'stage' not in task.type:
            return

        test_name = 'change_module_name'
        inc_task = IncrementalTest.prepare_incremental_task(task, test_name)

        logging.info(f"==========> Running {test_name} for task: {task.name}")
        # modify build-profile.json5
        profile_file = os.path.join(task.path, 'build-profile.json5')
        profile_file_backup = profile_file + ".bak"
        shutil.copyfile(profile_file, profile_file_backup)

        with open(profile_file, 'r', encoding='utf-8') as file:
            profile_data = json5.load(file)
        new_module_name = "new_entry"
        logging.debug(f"profile_data is: {profile_data}")
        for module in profile_data['modules']:
            if module['name'] == task.hap_module:
                module['name'] = new_module_name
                break
        with open(profile_file, 'w') as file:
            json5.dump(profile_data, file)

        # modify module.json5 for stage mode
        config_file_dir = os.path.join(task.path, *task.hap_module_path, 'src', 'main')
        config_file = os.path.join(config_file_dir, 'module.json5')
        config_file_backup = config_file + ".bak"
        shutil.copyfile(config_file, config_file_backup)

        with open(config_file, 'r') as file:
            config_data = json5.load(file)
        config_data['module']['name'] = new_module_name
        with open(config_file, 'w') as file:
            json5.dump(config_data, file)

        try:
            cmd = get_hvigor_compile_cmd(task, is_debug, 'Hap', new_module_name)
            [stdout, stderr] = compile_project(task, is_debug, cmd)
            IncrementalTest.validate_module_name_change(
                task, inc_task, is_debug, stdout, stderr, new_module_name)
        except Exception as e:
            logging.exception(e)
        finally:
            shutil.move(profile_file_backup, profile_file)
            shutil.move(config_file_backup, config_file)

    @staticmethod
    def compile_incremental_build_modify_error_then_fix(task, is_debug):
        test_name = 'modify_error_then_fix'
        inc_task = IncrementalTest.prepare_incremental_task(task, test_name)
        info = inc_task.debug_info if is_debug else inc_task.release_info
        logging.info(f"==========> Running {test_name} for task: {task.name}")

        modify_file = os.path.join(task.path, *task.inc_modify_file)
        patch_lines_error = options.configs.get('patch_content').get('patch_lines_error')
        error_content = patch_lines_error.get('content')
        with open(modify_file, 'a', encoding='utf-8') as file:
            file.write(error_content)

        try:
            [stdout, stderr] = compile_project(task, is_debug)
            is_passed = is_get_expected_error(info, stderr, patch_lines_error.get('expected_error'))
            if not is_passed:
                logging.error(f"task: {task.name}failed to get expected error, skip second build")
                return
        finally:
            utils.remove_content_from_file(modify_file, '',
                                           patch_lines_error.get('content'))
        is_build_successful, build_time = is_build_module_successful(task, is_debug,
            info, '', 'incremental_compile_modify_error_then_fix')
        if is_build_successful:
            info.result = options.TaskResult.passed
            info.time = build_time
            modify_file_item = task.inc_modify_file
            modified_files = [os.path.join(*modify_file_item)]
            IncrementalTest.validate_compile_incremental_file(
                task, inc_task, is_debug, modified_files)

    @staticmethod
    def compile_incremental_build_add_error_page(task, is_debug):
        test_name = 'add_error_page_then_fix'
        inc_task = IncrementalTest.prepare_incremental_task(task, test_name)
        info = inc_task.debug_info if is_debug else inc_task.release_info
        logging.info(f"==========> Running {test_name} for task: {task.name}")

        add_or_delete_page(task, 1, True)
        try:
            expected_errors = options.configs.get('patch_content').get('patch_lines_error').get('content')
            [stdout, stderr] = compile_project(task, is_debug)
            if not is_get_expected_error(info, stderr, expected_errors):
                return
            add_or_delete_page(task, 0, True)
            second_incremental, build_time = is_build_module_successful(task, is_debug, info,
                '', 'incremental_compile_add_error_page_then_fix')

            if second_incremental:
                info.result = options.TaskResult.passed
                info.time = build_time
                modify_file_item = task.inc_modify_file
                modified_files = [os.path.join(*modify_file_item)]
                IncrementalTest.validate_compile_incremental_file(
                    task, inc_task, is_debug, modified_files)
        finally:
            add_or_delete_page(task, 0)

    @staticmethod
    def compile_incremental_build_add_error_non_page(task, is_debug):
        test_name = 'add_error_non_page_then_fix'
        inc_task = IncrementalTest.prepare_incremental_task(task, test_name)
        info = inc_task.debug_info if is_debug else inc_task.release_info
        logging.info(f"==========> Running {test_name} for task: {task.name}")

        add_or_delete_js_file(task, 1, True)
        try:
            expected_errors = options.configs.get('patch_content').get('patch_lines_error').get('content')
            [stdout, stderr] = compile_project(task, is_debug)
            if not is_get_expected_error(info, stderr, expected_errors):
                return
            add_or_delete_js_file(task, 0, True)
            second_incremental, build_time = is_build_module_successful(task, is_debug, info,
                '', 'incremental_compile_add_error_non_page_then_fix')

            if second_incremental:
                info.result = options.TaskResult.passed
                info.time = build_time
                modify_file_item = task.inc_modify_file
                modified_files = [os.path.join(*modify_file_item)]
                IncrementalTest.validate_compile_incremental_file(
                    task, inc_task, is_debug, modified_files)
        finally:
            add_or_delete_js_file(task, 0)

    @staticmethod
    def compile_incremental_build_modify_sdk_version(task, is_debug):
        test_name = 'modify_sdk_version'
        inc_task = IncrementalTest.prepare_incremental_task(task, test_name)
        info = inc_task.debug_info if is_debug else inc_task.release_info
        logging.info(f"==========> Running {test_name} for task: {task.name}")

        try:
            first_incremental, first_build_time = is_build_module_successful(task, is_debug, info)
            if not first_incremental:
                return
            # The default project uses api12, change it to api11
            modify_sdk_version(task, 11)
            second_incremental, second_build_time = is_build_module_successful(task, is_debug, info,
                '', 'incremental_compile_modify_sdk_version')

            if second_incremental:
                info.result = options.TaskResult.passed
                info.time = first_build_time + second_build_time
        finally:
            modify_sdk_version(task, 12)

    @staticmethod
    def compile_incremental_build_entry_then_har(task, is_debug):
        test_name = 'build_entry_then_har'
        inc_task = IncrementalTest.prepare_incremental_task(task, test_name)
        info = inc_task.debug_info if is_debug else inc_task.release_info
        logging.info(f"==========> Running {test_name} for task: {task.name}")

        with manage_module_import_and_export_handle(task, 'Har'):
            first_incremental, first_build_time = is_build_module_successful(task, is_debug, info)
            if not first_incremental:
                return
            second_incremental, second_build_time = is_build_module_successful(task, is_debug, info,
                'Har', 'incremental_compile_build_entry_then_har')

            if second_incremental:
                info.result = options.TaskResult.passed
                info.time = first_build_time + second_build_time
                modify_file_item = task.inc_modify_file
                modified_files = [os.path.join(*modify_file_item)]
                IncrementalTest.validate_compile_incremental_file(
                    task, inc_task, is_debug, modified_files)

    @staticmethod
    def compile_incremental_build_har_then_entry(task, is_debug):
        test_name = 'build_har_then_entry'
        inc_task = IncrementalTest.prepare_incremental_task(task, test_name)
        info = inc_task.debug_info if is_debug else inc_task.release_info
        logging.info(f"==========> Running {test_name} for task: {task.name}")

        with manage_module_import_and_export_handle(task, 'Har'):
            first_incremental, first_build_time = is_build_module_successful(task, is_debug, info, 'Har')
            if not first_incremental:
                return
            second_incremental, second_build_time = is_build_module_successful(task, is_debug, info,
                '', 'incremental_compile_build_har_then_entry')

            if second_incremental:
                info.result = options.TaskResult.passed
                info.time = first_build_time + second_build_time
                modify_file_item = task.inc_modify_file
                modified_files = [os.path.join(*modify_file_item)]
                IncrementalTest.validate_compile_incremental_file(
                    task, inc_task, is_debug, modified_files)

    @staticmethod
    def compile_incremental_build_entry_then_hsp(task, is_debug):
        test_name = 'build_entry_then_hsp'
        inc_task = IncrementalTest.prepare_incremental_task(task, test_name)
        info = inc_task.debug_info if is_debug else inc_task.release_info
        logging.info(f"==========> Running {test_name} for task: {task.name}")

        with manage_module_import_and_export_handle(task, 'Hsp'):
            first_incremental, first_build_time = is_build_module_successful(task, is_debug, info)
            if not first_incremental:
                return
            second_incremental, second_build_time = is_build_module_successful(task, is_debug, info,
                'Hsp', 'incremental_compile_build_entry_then_hsp')

            if second_incremental:
                info.result = options.TaskResult.passed
                info.time = first_build_time + second_build_time
                modify_file_item = task.inc_modify_file
                modified_files = [os.path.join(*modify_file_item)]
                IncrementalTest.validate_compile_incremental_file(
                    task, inc_task, is_debug, modified_files)

    @staticmethod
    def compile_incremental_build_hsp_then_entry(task, is_debug):
        test_name = 'build_hsp_then_entry'
        inc_task = IncrementalTest.prepare_incremental_task(task, test_name)
        info = inc_task.debug_info if is_debug else inc_task.release_info
        logging.info(f"==========> Running {test_name} for task: {task.name}")

        with manage_module_import_and_export_handle(task, 'Hsp'):
            first_incremental, first_build_time = is_build_module_successful(task, is_debug, info, 'Hsp')
            if not first_incremental:
                return
            second_incremental, second_build_time = is_build_module_successful(task, is_debug, info,
                '', 'incremental_compile_build_hsp_then_entry')

            if second_incremental:
                info.result = options.TaskResult.passed
                info.time = first_build_time + second_build_time
                modify_file_item = task.inc_modify_file
                modified_files = [os.path.join(*modify_file_item)]
                IncrementalTest.validate_compile_incremental_file(
                    task, inc_task, is_debug, modified_files)

    @staticmethod
    def compile_incremental_build_hsp_then_ohos(task, is_debug):
        if not is_debug or 'ohosTest' not in task.type:
            return
        test_name = 'build_hsp_then_ohos'
        inc_task = IncrementalTest.prepare_incremental_task(task, test_name)
        info = inc_task.debug_info if is_debug else inc_task.release_info
        logging.info(f"==========> Running {test_name} for task: {task.name}")

        with manage_module_import_and_export_handle(task, 'Hsp'):
            first_incremental, first_build_time = is_build_module_successful(task, is_debug, info, 'Hsp')
            if not first_incremental:
                return
            second_incremental, second_build_time = is_build_ohos_test_successful(task, info)

            if second_incremental:
                info.result = options.TaskResult.passed
                info.time = first_build_time + second_build_time
                modify_file_item = task.inc_modify_file
                modified_files = [os.path.join(*modify_file_item)]
                IncrementalTest.validate_compile_incremental_file(
                    task, inc_task, is_debug, modified_files)

    @staticmethod
    def compile_incremental_build_entry_then_ohos(task, is_debug):
        if not is_debug:
            return
        test_name = 'build_entry_then_ohos'
        inc_task = IncrementalTest.prepare_incremental_task(task, test_name)
        info = inc_task.debug_info if is_debug else inc_task.release_info
        logging.info(f"==========> Running {test_name} for task: {task.name}")

        first_incremental, first_build_time = is_build_module_successful(task, is_debug, info)
        if not first_incremental:
            return
        second_incremental, second_build_time = is_build_ohos_test_successful(task, info)

        if second_build_time:
            info.result = options.TaskResult.passed
            info.time = first_build_time + second_build_time
            modify_file_item = task.inc_modify_file
            modified_files = [os.path.join(*modify_file_item)]
            IncrementalTest.validate_compile_incremental_file(
                task, inc_task, is_debug, modified_files)

    @staticmethod
    def compile_incremental_build_entry_then_preview_build(task, is_debug):
        test_name = 'build_entry_then_preview_build'
        inc_task = IncrementalTest.prepare_incremental_task(task, test_name)
        info = inc_task.debug_info if is_debug else inc_task.release_info
        logging.info(f"==========> Running {test_name} for task: {task.name}")

        first_incremental, first_build_time = is_build_module_successful(task, is_debug, inc_task)
        if not first_incremental:
            return
        second_incremental, second_build_time = preview_mode_build(info, task, is_debug)

        if second_incremental:
            info.result = options.TaskResult.passed
            info.time = first_build_time + second_build_time
            modify_file_item = task.inc_modify_file
            modified_files = [os.path.join(*modify_file_item)]
            IncrementalTest.validate_compile_incremental_file(
                task, inc_task, is_debug, modified_files)


class BytecodeHarTest:
    @staticmethod
    def prepare_bytecode_har_task(task, test_name):
        if test_name in task.bytecode_har_compilation_info:
            bytecode_har_task = task.bytecode_har_compilation_info[test_name]
        else:
            bytecode_har_task = options.BytecodeHarCompilationInfo()
            bytecode_har_task.name = test_name
            task.bytecode_har_compilation_info[test_name] = bytecode_har_task
        return bytecode_har_task

    @staticmethod
    def build_bytecode_har(task, is_debug):
        test_name = 'build_bytecode_har'
        clean_compile(task)
        bytecode_har_task = BytecodeHarTest.prepare_bytecode_har_task(task, test_name)
        info = bytecode_har_task.debug_info if is_debug else bytecode_har_task.release_info
        logging.info(f"==========> Running {test_name} for task: {task.name}")

        modify_bytecode_har_config(task, 1)
        try:
            is_passed, build_time = is_build_module_successful(task, is_debug, info, 'BytecodeHar')
            if is_passed:
                info.result = options.TaskResult.passed
                info.time = build_time
        finally:
            modify_bytecode_har_config(task, 0)

    @staticmethod
    def build_har_then_bytecode_har(task, is_debug):
        if is_debug:
            return
        test_name = 'build_har_then_bytecode_har'
        clean_compile(task)
        bytecode_har_task = BytecodeHarTest.prepare_bytecode_har_task(task, test_name)
        info = bytecode_har_task.release_info
        logging.info(f"==========> Running {test_name} for task: {task.name}")

        is_passed, build_time = is_build_module_successful(task, is_debug, info, 'Har')
        if not is_passed:
            logging.error(f'build {task.har_module} failed')
            return

        modify_bytecode_har_config(task, 1)
        try:
            is_passed, build_time = is_build_module_successful(task, is_debug, info, 'BytecodeHar')
            if is_passed:
                info.result = options.TaskResult.passed
                info.time = build_time
        finally:
            modify_bytecode_har_config(task, 0)

    @staticmethod
    def import_bytecode_static_library(task, is_debug):
        test_name = 'import_bytecode_static_library'
        clean_compile(task)
        bytecode_har_task = BytecodeHarTest.prepare_bytecode_har_task(task, test_name)
        info = bytecode_har_task.debug_info if is_debug else bytecode_har_task.release_info
        logging.info(f"==========> Running {test_name} for task: {task.name}")

        modify_bytecode_har_config(task, 1)
        try:
            with manage_bytecode_har_dependency(task, is_debug, info, 'Har'):
                cmd = get_hvigor_compile_cmd(task, is_debug)
                [stdout, stderr] = compile_project(task, is_debug, cmd)
                [is_success, time_string] = is_compile_success(stdout)
                if not is_success:
                    info.result = options.TaskResult.failed
                    info.error_message = f'Full compile failed due to build {task.hap_module} module.'
                    logging.error(f'build {task.hap_module} failed')
                    return
                else:
                    info.result = options.TaskResult.passed
                    info.time = collect_compile_time(time_string)
                if options.arguments.run_haps:
                    runtime_passed = run_compile_output(info, task, is_debug, 'import_bytecode_static_library')
        finally:
            modify_bytecode_har_config(task, 0)


class ExternalTest:
    @staticmethod
    def prepare_current_task(task, test_name):
        if test_name in task.external_compilation_info:
            current_task = task.external_compilation_info[test_name]
        else:
            current_task = options.ExternalCompilationInfo()
            current_task.name = test_name
            task.external_compilation_info[test_name] = current_task
        return current_task

    @staticmethod
    def get_external_task():
        external_task = options.create_test_tasks(options.configs.get('external_haps'))[0]
        return external_task

    @staticmethod
    def import_external_share_library(task, is_debug):
        test_name = 'import_external_share_library'
        external_task = ExternalTest.get_external_task()
        clean_compile(task)
        clean_compile(external_task)
        current_task = ExternalTest.prepare_current_task(task, test_name)
        info = current_task.debug_info if is_debug else current_task.release_info
        logging.info(f"==========> Running {test_name} for task: {task.name}")

        with manage_module_import_and_export_handle(task, 'ExternalHsp'):
            cmd = get_hvigor_compile_cmd(task, is_debug, '')
            [stdout, stderr] = compile_project(task, is_debug, cmd)
            [is_success, time_string] = is_compile_success(stdout)
            if not is_success:
                info.result = options.TaskResult.failed
                info.error_message = f'Full compile failed due to {task.hap_module} module'
                logging.error(f'Full compile failed due to {task.hap_module} module')
                return
            passed = validate_compile_output(info, task, is_debug, '', '')
            if not passed:
                info.result = options.TaskResult.failed
                info.error_message = f'Validate failed due to {task.hap_module} module'
                logging.error(f'Validate failed due to {task.hap_module} module')
                return
            pa_file = get_disasm_abc_file(task, info, 'Hap')
            if not pa_file:
                return
            is_packaged = is_package_modules_to_module_abc(task, pa_file, external_task.hsp_module)
            if not is_packaged:
                info.result = options.TaskResult.passed
                info.time = collect_compile_time(time_string)
            else:
                logging.error(f'Unexpected changes have occurred.OutHsp should not be packaged into module abc')
                info.result = options.TaskResult.failed
                info.error_message = f'Unexpected changes have occurred.OutHsp should not be packaged into module abc'
            if options.arguments.run_haps:
                runtime_passed = run_compile_output(info, task, is_debug, 'import_external_share_library')

    @staticmethod
    def import_external_static_library(task, is_debug):
        test_name = 'import_external_static_library'
        external_task = ExternalTest.get_external_task()
        clean_compile(task)
        clean_compile(external_task)
        current_task = ExternalTest.prepare_current_task(task, test_name)
        info = current_task.debug_info if is_debug else current_task.release_info
        logging.info(f"==========> Running {test_name} for task: {task.name}")

        with manage_module_import_and_export_handle(task, 'ExternalHar'):
            cmd = get_hvigor_compile_cmd(task, is_debug, '')
            [stdout, stderr] = compile_project(task, is_debug, cmd)
            [is_success, time_string] = is_compile_success(stdout)
            if not is_success:
                info.result = options.TaskResult.failed
                info.error_message = f'Full compile failed due to {task.hap_module} module'
                logging.error(f'Full compile failed due to {task.hap_module} module')
                return
            passed = validate_compile_output(info, task, is_debug, '', '')
            if not passed:
                info.result = options.TaskResult.failed
                info.error_message = f'Validate failed due to {task.hap_module} module'
                logging.error(f'Validate failed due to {task.hap_module} module')
                return
            pa_file = get_disasm_abc_file(task, info, 'Hap')
            if not pa_file:
                return
            is_packaged = is_package_modules_to_module_abc(task, pa_file, external_task.har_module)
            if is_packaged:
                info.result = options.TaskResult.passed
                info.time = collect_compile_time(time_string)
            else:
                logging.error(f'OutHar was not properly packaged into module abc')
                info.result = options.TaskResult.failed
                info.error_message = f'OutHar was not properly packaged into module abc'
            if options.arguments.run_haps:
                runtime_passed = run_compile_output(info, task, is_debug, 'import_external_static_library')

    @staticmethod
    def full_compile_external_static_library(task, is_debug):
        if is_debug:
            return
        test_name = 'full_compile_external_static_library'
        external_task = ExternalTest.get_external_task()
        clean_compile(task)
        clean_compile(external_task)
        current_task = ExternalTest.prepare_current_task(task, test_name)
        info = current_task.release_info
        logging.info(f"==========> Running {test_name} for task: {task.name}")

        with manage_module_import_and_export_handle(task, 'ExternalHar'):
            cmd = get_hvigor_compile_cmd(external_task, is_debug, 'Har')
            [stdout, stderr] = compile_project(external_task, is_debug, cmd)
            [is_success, time_string] = is_compile_success(stdout)
            if not is_success:
                info.result = options.TaskResult.failed
                info.error_message = f'Full compile failed due to {external_task.har_module} module'
                logging.error(f'Full compile failed due to {external_task.har_module} module')
                return
            passed = validate_compile_output(info, external_task, is_debug, '', 'Har')
            if not passed:
                info.result = options.TaskResult.failed
                info.error_message = f'Validate failed due to {external_task.har_module} module'
                logging.error(f'Validate failed due to {external_task.har_module} module')
            else:
                info.result = options.TaskResult.passed
                info.time = collect_compile_time(time_string)

    @staticmethod
    def full_compile_external_share_library(task, is_debug):
        test_name = 'full_compile_external_share_library'
        external_task = ExternalTest.get_external_task()
        clean_compile(task)
        clean_compile(external_task)
        current_task = ExternalTest.prepare_current_task(task, test_name)
        info = current_task.debug_info if is_debug else current_task.release_info
        logging.info(f"==========> Running {test_name} for task: {task.name}")

        with manage_module_import_and_export_handle(task, 'ExternalHsp'):
            cmd = get_hvigor_compile_cmd(external_task, is_debug, 'Hsp')
            [stdout, stderr] = compile_project(external_task, is_debug, cmd)
            [is_success, time_string] = is_compile_success(stdout)
            if not is_success:
                info.result = options.TaskResult.failed
                info.error_message = f'Full compile failed due to {external_task.hsp_module} module'
                logging.error(f'Full compile failed due to {external_task.hsp_module} module')
                return
            passed = validate_compile_output(info, external_task, is_debug, '', 'Hsp')
            if not passed:
                info.result = options.TaskResult.failed
                info.error_message = f'Validate failed due to {external_task.hsp_module} module'
                logging.error(f'Validate failed due to {external_task.hsp_module} module')
            else:
                info.result = options.TaskResult.passed
                info.time = collect_compile_time(time_string)


class PreviewTest:
    @staticmethod
    def validate_preview_incremental_file(task, preview_task_info, is_debug, modified_files, module=''):
        module_path = utils.get_module_path(task, module)
        cache_path = os.path.join(
            task.path, *module_path, *task.build_path, *task.preview_cache_path, 'debug')
        backup_path = task.backup_info.cache_debug

        passed = validate_cache_file(task, preview_task_info, modified_files, cache_path, backup_path)
        return passed

    @staticmethod
    def preview_compile(task, is_debug):
        test_name = "preview_compile"
        preview_task_info = options.CompilationInfo()
        task.preview_compilation_info[test_name] = preview_task_info
        clean_preview_cache(task)
        logging.info(f"==========> Running {test_name} for task: {task.name}")

        passed, build_time = preview_mode_build(preview_task_info, task, is_debug, f'preview_compile_{test_name}')
        if passed:
            preview_task_info.result = options.TaskResult.passed
            preview_task_info.time = build_time

        return passed

    @staticmethod
    def compile_preview_build_entry_then_preview(task, is_debug):
        test_name = "build_entry_then_preview"
        preview_task_info = options.CompilationInfo()
        task.preview_compilation_info[test_name] = preview_task_info
        logging.info(f"==========> Running {test_name} for task: {task.name}")

        build_entry, build_module_time = is_build_module_successful(task, is_debug, preview_task_info)
        if not build_entry:
            return
        build_preview, preview_build_time = preview_mode_build(preview_task_info, task, is_debug)

        if build_preview:
            preview_task_info.result = options.TaskResult.passed
            preview_task_info.time = preview_build_time

    @staticmethod
    def compile_preview_build_modify_file_name(task, is_debug):
        test_name = "build_modify_file_name"
        preview_task_info = options.CompilationInfo()
        task.preview_compilation_info[test_name] = preview_task_info
        logging.info(f"==========> Running {test_name} for task: {task.name}")

        modify_file = os.path.join(task.path, *task.inc_modify_file)
        patch_content = options.configs.get('patch_content')
        patch_new_file_ts = patch_content.get('patch_new_file_ts')
        ts_file_name = patch_new_file_ts.get('name')
        ts_content = patch_new_file_ts.get('content')
        modify_dic = os.path.dirname(modify_file)
        ts_file = os.path.join(modify_dic, ts_file_name)

        try:
            with open(ts_file, 'w', encoding='utf-8') as file:
                file.write(ts_content)

            path_lines = patch_content.get('patch_lines_1')
            ts_path_lines = path_lines.get('ts')
            head_contnet = ts_path_lines.get('head')
            tail_contnet = ts_path_lines.get('tail')
            utils.add_content_to_file(modify_file, head_contnet, tail_contnet)
            first_build_passed, first_build_time = is_build_module_successful(task, is_debug, preview_task_info)
            if not first_build_passed:
                return
            ts_file_new_name = patch_new_file_ts.get('new_name')
            ts_new_file = os.path.join(modify_dic, ts_file_new_name)
            os.rename(ts_file, ts_new_file)
            second_build_passed, second_build_time = is_build_module_successful(task, is_debug, preview_task_info)
            if second_build_passed:
                preview_task_info.result = options.TaskResult.passed
                preview_task_info.time = first_build_time + second_build_time
        finally:
            os.remove(ts_new_file)
            utils.remove_content_from_file(modify_file, head_contnet, tail_contnet)

    @staticmethod
    def compile_preview_build_generate_sourcemap(task, is_debug):
        test_name = "build_generate_sourcemap"
        preview_task_info = options.CompilationInfo()
        task.preview_compilation_info[test_name] = preview_task_info
        logging.info(f"==========> Running {test_name} for task: {task.name}")

        build_preview, preview_build_time = preview_mode_build(preview_task_info, task, is_debug)
        if not build_preview:
            return

        preview_path = os.path.join(task.path, *task.hap_module_path, *task.preview_path)
        preview_cache_path = os.path.join(preview_path, *task.preview_cache_path)
        source_map_path = os.path.join(preview_cache_path, 'debug', 'sourceMaps.json') if is_debug \
            else os.path.join(preview_cache_path, 'release', 'sourceMaps.json')

        if not os.path.exists(source_map_path):
            logging.error(f'task: {task.name},source map not found in {source_map_path}')
            preview_task_info.result = options.TaskResult.failed
            preview_task_info.error_message = f"Source map not found in f{source_map_path}"
            return

        preview_task_info.result = options.TaskResult.passed
        preview_task_info.time = preview_build_time

    @staticmethod
    def compile_preview_build_tigger_incremental_build(task, is_debug):
        test_name = "tigger_incremental_build"
        preview_task_info = options.CompilationInfo()
        task.preview_compilation_info[test_name] = preview_task_info
        logging.info(f"==========> Running {test_name} for task: {task.name}")

        backup_preview_cache(task, is_debug)
        inc_modify_file = os.path.join(task.path, *task.inc_modify_file)
        patch_line = options.configs.get('patch_content').get('patch_lines_2')
        utils.add_content_to_file(inc_modify_file, '', patch_line.get('tail'))

        try:
            build_preview, preview_build_time = preview_mode_build(preview_task_info, task, is_debug)
            if not build_preview:
                return

            passed = PreviewTest.validate_preview_incremental_file(task, preview_task_info, is_debug, inc_modify_file)
            if passed:
                preview_task_info.result = options.TaskResult.passed
                preview_task_info.time = preview_build_time
        finally:
            utils.remove_content_from_file(inc_modify_file, '', patch_line.get('tail'))

    @staticmethod
    def compile_preview_build_has_arkui_error(task, is_debug):
        test_name = "has_arkui_error"
        preview_task_info = options.CompilationInfo()
        task.preview_compilation_info[test_name] = preview_task_info
        logging.info(f"==========> Running {test_name} for task: {task.name}")

        add_or_delete_arkui_component(task, 1, True)
        try:
            preview_mode_build(preview_task_info, task, is_debug)
            cmd = get_preview_mode_compile_cmd(task, is_debug)
            [stdout, stderr] = compile_project(task, is_debug, cmd)
            expected_errors = options.configs.get('patch_content').get('arkui_patch').get('expected_errors')
            is_passed = is_get_expected_error(preview_task_info, stderr, expected_errors)
            if is_passed:
                preview_task_info.result = options.TaskResult.passed
        finally:
            add_or_delete_arkui_component(task, 0)

    @staticmethod
    def compile_preview_build_sdk_path_has_special_char(task, is_debug):
        test_name = "sdk_path_has_special_char"
        preview_task_info = options.CompilationInfo()
        task.preview_compilation_info[test_name] = preview_task_info
        logging.info(f"==========> Running {test_name} for task: {task.name}")

        sdk_path, api_version = '', ''
        profile_file = os.path.join(task.path, 'build-profile.json5')
        with open(profile_file, 'r', encoding='utf-8') as file:
            profile_data = json5.load(file)
            api_version = profile_data['app']['products'][0]['compatibleSdkVersion']
            if isinstance(api_version, int):
                openharmony_sdk_path = options.configs.get('deveco_openharmony_sdk_path')
                sdk_path = os.path.join(openharmony_sdk_path, str(api_version), 'ets', 'build-tools')
            else:
                harmonyos_sdk_path = options.configs.get('deveco_harmonyos_sdk_path')
                api_version_file_map = options.configs.get('api_version_file_name_map')
                file_name = api_version_file_map.get(api_version)
                sdk_path = os.path.join(harmonyos_sdk_path, file_name, 'openharmony', 'ets', 'build-tools')
        # Add a space to sdk file path
        last_folder_name = os.path.basename(sdk_path)
        new_folder_name = last_folder_name[:2] + " " + last_folder_name[2:]
        new_sdk_path = os.path.join(os.path.dirname(sdk_path), new_folder_name)
        try:
            os.rename(sdk_path, new_sdk_path)
            passed, build_time = preview_mode_build(preview_task_info, task, is_debug)

            if passed:
                preview_task_info.result = options.TaskResult.passed
                preview_task_info.time = build_time
            else:
                preview_task_info.result = options.TaskResult.failed
                logging.error(f'Test failed due to adding spaces to the SDK path')
        finally:
            os.rename(new_sdk_path, sdk_path)

    @staticmethod
    def compile_preview_build_modify_error_then_fix(task, is_debug):
        test_name = "modify_hello_world_then_fix"
        preview_task_info = options.CompilationInfo()
        task.preview_compilation_info[test_name] = preview_task_info
        logging.info(f"==========> Running {test_name} for task: {task.name}")

        add_or_delete_arkui_component(task, 1)
        arkui_patch = options.configs.get('patch_content').get('arkui_patch')
        origin_text = arkui_patch.get('origin_text')
        error_text = arkui_patch.get('error_text')
        try:
            preview_modify_file = os.path.join(task.path, *task.inc_modify_file)
            with open(preview_modify_file, 'r+', encoding='utf-8') as file:
                old_content = file.read()
                new_content = old_content.replace(origin_text, error_text)
                file.seek(0)
                file.write(new_content)
                file.truncate()
            cmd = get_preview_mode_compile_cmd(task, is_debug)
            [stdout, stderr] = compile_project(task, is_debug, cmd)
            expected_errors = arkui_patch.get('expected_errors')
            is_passed = is_get_expected_error(preview_task_info, stderr, expected_errors)
            if not is_passed:
                logging.error(f'task: {task.name}, first build did not get expected errors, skip second build')
                return
            with open(preview_modify_file, 'r+', encoding='utf-8') as file:
                old_content = file.read()
                new_content = old_content.replace(error_text, origin_text)
                file.seek(0)
                file.write(new_content)
                file.truncate()
            is_build_successful, build_time = preview_mode_build(preview_task_info, task, is_debug)
            if is_build_successful:
                preview_task_info.result = options.TaskResult.passed
                preview_task_info.time = build_time
        finally:
            add_or_delete_arkui_component(task, 0)


class OtherTest:
    @staticmethod
    def is_abc_same_in_haps(hap_1, hap_2):
        hap_1_abc_files = []
        hap_2_abc_files = []
        with zipfile.ZipFile(hap_1) as zf1, zipfile.ZipFile(hap_2) as zf2:
            for file in zf1.namelist():
                if file.endswith('.abc'):
                    hap_1_abc_files.append(file)
            for file in zf2.namelist():
                if file.endswith('.abc'):
                    hap_2_abc_files.append(file)

            hap_1_abc_files.sort()
            hap_2_abc_files.sort()

            if len(hap_1_abc_files) != len(hap_2_abc_files):
                return False

            for idx, abc_file in enumerate(hap_1_abc_files):
                with zf1.open(abc_file) as f1, zf2.open(hap_2_abc_files[idx]) as f2:
                    data1 = f1.read()
                    data2 = f2.read()
                    if data1 != data2:
                        return False

        return True

    @staticmethod
    def verify_binary_consistency(task):
        test_name = 'binary_consistency'
        test_info = options.CompilationInfo()
        task.other_tests[test_name] = test_info
        debug_consistency = True
        release_consistency = True

        logging.info(f"==========> Running {test_name} for task: {task.name}")
        if options.arguments.hap_mode in ['all', 'release']:
            # will have at lease 1 output from full compile
            if len(task.backup_info.output_release) == 1:
                compile_project(task, False)
                backup_compile_output(task, False)

            if len(task.backup_info.output_release) == 2:
                release_consistency = OtherTest.is_abc_same_in_haps(task.backup_info.output_release[0],
                                                                    task.backup_info.output_release[1])
            else:
                release_consistency = False
            logging.debug(f"release consistency: {release_consistency}")

        if options.arguments.hap_mode in ['all', 'debug']:
            if len(task.backup_info.output_debug) == 1:
                compile_project(task, True)
                backup_compile_output(task, True)

            if len(task.backup_info.output_debug) == 2:
                debug_consistency = OtherTest.is_abc_same_in_haps(task.backup_info.output_debug[0],
                                                                  task.backup_info.output_debug[1])
            else:
                debug_consistency = False
            logging.debug(f"debug consistency: {debug_consistency}")

        if debug_consistency and release_consistency:
            test_info.result = options.TaskResult.passed
        else:
            test_info.result = options.TaskResult.failed

    @staticmethod
    def execute_break_compile(task, is_debug):
        test_name = 'break_continue_compile'
        test_info = options.CompilationInfo()
        task.other_tests[test_name] = test_info

        logging.info(f"==========> Running {test_name} for task: {task.name}")
        clean_compile(task)
        cmd = get_hvigor_compile_cmd(task, is_debug)
        logging.debug(f'cmd: {cmd}')
        logging.debug(f"cmd execution path {task.path}")
        process = subprocess.Popen(cmd, shell=False, cwd=task.path,
                                   stdout=subprocess.PIPE,
                                   stderr=subprocess.STDOUT)

        for line in iter(process.stdout.readline, b''):
            if b'CompileArkTS' in line:
                logging.debug("terminate signal sent")
                process.send_signal(signal.SIGTERM)
                break

        [stdout, stderr] = process.communicate(
            timeout=options.arguments.compile_timeout)

        logging.debug("first compile: stdcout: %s",
                      stdout.decode('utf-8', errors="ignore"))

        logging.debug("another compile")
        [stdout, stderr] = compile_project(task, is_debug)

        [is_success, time_string] = is_compile_success(stdout)
        if not is_success:
            test_info.result = options.TaskResult.failed
            test_info.error_message = stderr
        else:
            passed = validate_compile_output(test_info, task, is_debug)
            if passed:
                test_info.result = options.TaskResult.passed
        if options.arguments.run_haps:
            run_compile_output(test_info, task, True, 'other_tests_break_continue_compile')

    @staticmethod
    def compile_full_with_error(task, is_debug):
        test_name = 'compile_with_error'
        test_info = options.CompilationInfo()
        task.other_tests[test_name] = test_info

        logging.info(f"==========> Running {test_name} for task: {task.name}")
        modify_file_item = task.inc_modify_file
        modify_file = os.path.join(task.path, *modify_file_item)
        modify_file_backup = modify_file + ".bak"
        shutil.copyfile(modify_file, modify_file_backup)

        patch_lines_error = options.configs.get(
            'patch_content').get('patch_lines_error')
        with open(modify_file, 'a', encoding='utf-8') as file:
            file.write(patch_lines_error.get('content'))

        [stdout, stderr] = compile_project(task, is_debug)
        expected_errors = patch_lines_error.get('expected_error')

        passed = False
        for expected_error in expected_errors:
            if expected_error in stderr:
                passed = True
                break

        if passed:
            test_info.result = options.TaskResult.passed
        else:
            test_info.result = options.TaskResult.failed
            test_info.error_message = f"expected error message: {expected_errors}, but got {stderr}"

        shutil.move(modify_file_backup, modify_file)

    @staticmethod
    def compile_with_exceed_length(task, is_debug):
        test_name = 'compile_with_exceed_length'
        test_info = options.CompilationInfo()
        task.other_tests[test_name] = test_info

        logging.info(f"==========> Running {test_name} for task: {task.name}")
        # get build-profile.json5
        profile_file = os.path.join(
            task.path, *task.hap_module_path, 'build-profile.json5')
        profile_file_backup = profile_file + ".bak"
        shutil.copyfile(profile_file, profile_file_backup)

        with open(profile_file, 'r', encoding='utf-8') as file:
            profile_data = json5.load(file)

        long_str = 'default1234567890123456789012345678901234567890123456789012345678901234567890123456789' + \
                   '012345678901234567890123456789'
        logging.debug("long_str: %s", long_str)
        profile_data['targets'][0]['name'] = long_str

        with open(profile_file, 'w', encoding='utf-8') as file:
            json5.dump(profile_data, file)

        cmd = get_hvigor_compile_cmd(task, is_debug, task.hap_module, long_str)
        [stdout, stderr] = compile_project(task, is_debug, cmd)
        # Only the Windows platform has a length limit
        if utils.is_windows():
            expected_error_message = f"Unknown module '{long_str}' in the command line"

            if expected_error_message in stderr:
                test_info.result = options.TaskResult.passed
            else:
                test_info.result = options.TaskResult.failed
                test_info.error_message = f"expected error message: {expected_error_message}, but got {stderr}"
        else:
            [is_success, time_string] = is_compile_success(stdout)
            if not is_success:
                test_info.result = options.TaskResult.failed
                test_info.error_message = stderr
            else:
                passed = validate_compile_output(test_info, task, is_debug)
                if passed:
                    test_info.result = options.TaskResult.passed

        shutil.move(profile_file_backup, profile_file)

    @staticmethod
    def compile_ohos_test(task):
        test_name = 'ohos_test'
        test_info = options.CompilationInfo()
        task.other_tests[test_name] = test_info

        logging.info(f"==========> Running {test_name} for task: {task.name}")
        # ohosTest has only debug mode
        cmd = [*get_hvigor_path(), '--mode', 'module',
               '-p', 'module=entry@ohosTest', 'assembleHap']
        [stdout, stderr] = compile_project(task, True, cmd)
        [is_success, time_string] = is_compile_success(stdout)
        if not is_success:
            test_info.result = options.TaskResult.failed
            test_info.error_message = stderr
        else:
            output_file = get_compile_output_file_path(task, '', options.OutputType.unsigned)
            output_dir = os.path.dirname(output_file)
            output_file_name = os.path.basename(output_file)

            ohos_test_str = 'ohosTest'
            output_file_name_items = output_file_name.split(
                '-')  # hap name format: entry-default-signed.hap
            # ohosTest hap format: entry-ohosTest-signed.hap
            output_file_name_items[-2] = ohos_test_str
            output_file_name = '-'.join(output_file_name_items)

            output_dir_items = output_dir.split(os.path.sep)
            output_dir_items[-1] = ohos_test_str
            if utils.is_windows():
                # for windows, need to add an empty string to mark between disk identifier and path
                output_dir_items.insert(1, os.path.sep)
            elif utils.is_mac():
                output_dir_items.insert(0, os.path.sep)
            ohos_test_output_file = os.path.join(
                *output_dir_items, output_file_name)

            passed = validate_compile_output(
                test_info, task, True, ohos_test_output_file)
            if passed:
                test_info.result = options.TaskResult.passed


def disasm_abc(task, abc_file):
    if not os.path.exists(task.ark_disasm_path):
        logging.error("ark_disasm executable not found")
        return ''

    pa_file = abc_file + '.pa'
    cmd = [task.ark_disasm_path, '--verbose', abc_file, pa_file]
    logging.debug(f'cmd: {cmd}')
    process = subprocess.Popen(
        cmd, shell=False, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    [stdout, stderr] = process.communicate(
        timeout=options.arguments.compile_timeout)

    logging.debug("disasm stdcout: %s",
                  stdout.decode('utf-8', errors="ignore"))
    logging.warning("disasm: stdcerr: %s",
                    stderr.decode('utf-8', errors="ignore"))

    return pa_file


def is_abc_debug_info_correct(task, abc_file, is_debug):
    pa_file = disasm_abc(task, abc_file)
    if not os.path.exists(pa_file):
        logging.error(f"pa file not exist: {pa_file}")
        return False

    debug_info_block_str = 'LOCAL_VARIABLE_TABLE'
    has_debug_info_block = False
    with open(pa_file, 'r', encoding='utf-8') as pa:
        line = pa.readline()
        while line:
            if debug_info_block_str in line.strip():
                has_debug_info_block = True
                break
            line = pa.readline()

    if is_debug:
        return has_debug_info_block
    else:
        return not has_debug_info_block


def validate_output_for_jsbundle(info, task, uncompressed_output_path, is_debug):
    abc_files = []
    for root, dirs, files in os.walk(uncompressed_output_path):
        for file in files:
            if file.endswith('.abc'):
                abc_files.append(os.path.join(root, file))

    total_size = 0
    for file in abc_files:
        total_size += os.path.getsize(
            os.path.join(uncompressed_output_path, file))
        if 'compatible8' not in task.type and not is_abc_debug_info_correct(task, file, is_debug):
            # skip compatible8 outputs as disasm may failed
            info.result = options.TaskResult.failed
            info.error_message = f"{file} debug info not correct"
            return False

    if total_size == 0:
        info.result = options.TaskResult.failed
        info.error_message = "abc not found or abc size is 0"
        return False
    else:
        info.abc_size = total_size

    if is_debug:
        for file in abc_files:
            sourcemap_file = file.replace('.abc', '.js.map')
            if not os.path.exists(os.path.join(uncompressed_output_path, sourcemap_file)):
                info.result = options.TaskResult.failed
                info.error_message = "sourcemap not found"
                return False

    return True


def validate_output_for_esmodule(info, task, uncompressed_output_path, is_debug, module = ''):
    abc_generated_path = os.path.join(uncompressed_output_path, 'ets')

    modules_abc_path = os.path.join(abc_generated_path, 'modules.abc')
    if not os.path.exists(modules_abc_path):
        info.result = options.TaskResult.failed
        info.error_message = "modules.abc not found"
        return False

    modules_abc_size = os.path.getsize(modules_abc_path)
    if modules_abc_size <= 0:
        info.result = options.TaskResult.failed
        info.error_message = "modules.abc size is 0"
        return False
    if not is_abc_debug_info_correct(task, modules_abc_path, is_debug):
        info.result = options.TaskResult.failed
        info.error_message = "modules.abc debug info not correct"
        return False
    info.abc_size = modules_abc_size

    if 'widget' in task.type:
        widget_abc_path = os.path.join(abc_generated_path, 'widgets.abc')
        if not os.path.exists(widget_abc_path):
            info.result = options.TaskResult.failed
            info.error_message = "widgets.abc not found"
            return False

        widgets_abc_size = os.path.getsize(widget_abc_path)
        if widgets_abc_size <= 0:
            info.result = options.TaskResult.failed
            info.error_message = "widgets.abc size is 0"
            return False
        if not is_abc_debug_info_correct(task, widget_abc_path, is_debug):
            info.result = options.TaskResult.failed
            info.error_message = "widgets.abc debug info not correct"
            return False
        info.abc_size += widgets_abc_size

    if is_debug:
        sourcemap_path = abc_generated_path
    elif module == 'Hsp':
        sourcemap_path = os.path.join(
            task.path, *task.hsp_module_path, *(task.build_path), *(task.cache_path), 'release')
    else:
        sourcemap_path = os.path.join(
            task.path, *task.hap_module_path, *(task.build_path), *(task.cache_path), 'release')
    sourcemap_file = os.path.join(sourcemap_path, 'sourceMaps.map')
    if not os.path.exists(sourcemap_file):
        info.result = options.TaskResult.failed
        info.error_message = "sourcemap not found"
        return False

    return True


def collect_compile_time(time_string):
    time_min = 0.0
    time_second = 0.0
    time_millisecond = 0.0

    time_items = time_string.split()
    for idx, item in enumerate(time_items):
        if item == 'min':
            time_min = float(time_items[idx - 1]) * 60
        if item == 's':
            time_second = float(time_items[idx - 1])
        if item == 'ms':
            time_millisecond = round(float(time_items[idx - 1]) / 1000, 3)

    return round(time_min + time_second + time_millisecond, 3)


def get_compile_output_file_path(task, module, output_type):
    module_path = utils.get_module_path(task, module)
    output_path = utils.get_output_path(task, module, output_type)
    output_file = os.path.join(task.path, *module_path, *task.build_path, *output_path)

    return output_file


def validate_compile_output_har(info, task, is_debug, output_file='', module=''):
    uncompressed_output_file = get_output_uncompressed_file(task, info, module, options.OutputType.har)

    if not uncompressed_output_file:
        return False

    return True


def validate_compile_file_bytecode_har(task, info, module):
    module_path = utils.get_module_path(task, module)
    uncompressed_path = get_output_uncompressed_file(task, info, module, options.OutputType.har)
    modules_abc_path = os.path.join(uncompressed_path, 'ets', 'modules.abc')
    if not os.path.exists(modules_abc_path):
        return False
    is_success = find_file_by_suffix(['.d.ets'], uncompressed_path,
                                     'Index.ets', '')
    if not is_success:
        return False
    ets_path = os.path.join(task.path, *module_path, 'src', 'main', 'ets')
    for root, dirs, files in os.walk(ets_path):
        relative_path = os.path.relpath(root, os.path.join(task.path, *module_path))
        for file in files:
            if file.endswith('.ets'):
                extension_list = ['.d.ets']
            elif file.endswith('.ts'):
                extension_list = ['.d.ts']
            else:
                continue
            is_success = find_file_by_suffix(extension_list, uncompressed_path, file, relative_path)
            if not is_success:
                return False
    return True


def validate_compile_file_har(task, info, module):
    module_path = utils.get_module_path(task, module)
    uncompressed_path = get_output_uncompressed_file(task, info, module, options.OutputType.har)
    is_success = find_file_by_suffix(['.d.ets', '.js'], uncompressed_path,
                                     'Index.ets', '')
    if not is_success:
        return False
    ets_path = os.path.join(task.path, *module_path, 'src', 'main', 'ets')
    for root, dirs, files in os.walk(ets_path):
        relative_path = os.path.relpath(root, os.path.join(task.path, *module_path))
        for file in files:
            if file.endswith('.ets'):
                extension_list = ['.d.ets', '.js']
            elif file.endswith('.ts'):
                extension_list = ['.d.ts', '.js']
            elif file.endswith('.js'):
                extension_list = ['.js']
            else:
                continue
            is_success = find_file_by_suffix(extension_list, uncompressed_path, file, relative_path)
            if not is_success:
                return False
    return True


def find_file_by_suffix(extension_list, uncompressed_path, filename, relative_path):
    origin_extension = os.path.splitext(filename)[-1]
    for extension in extension_list:
        new_filename = filename.replace(origin_extension, extension)
        new_filepath = os.path.join(uncompressed_path, relative_path, new_filename)
        if not os.path.exists(new_filepath):
            return False
    return True


def validate_compile_output(info, task, is_debug, output_file='', module=''):
    passed = False

    if output_file == '':
        output_file = get_compile_output_file_path(task, module, options.OutputType.unsigned)

    if module == 'BytecodeHar':
        # Har declaration files are not generated in debug mode.
        if is_debug:
            return True
        return validate_compile_file_bytecode_har(task, info, module)
    if module == 'Har':
        if is_debug:
            return True
        return validate_compile_file_har(task, info, module)

    uncompressed_output_file = output_file + '.uncompressed'
    if not os.path.exists(output_file):
        logging.error("output file for task %s not exists: %s",
                      task.name, output_file)
        passed = False

        info.result = options.TaskResult.failed
        info.error_message = f"{module} not found"
        return passed
    try:
        with zipfile.ZipFile(output_file, 'r') as zip_ref:
            zip_ref.extractall(uncompressed_output_file)
    except Exception as e:
        logging.error(f"unzip exception: {e}")
        logging.error(
            f"uncompressed output file for task {task.name} failed. output file: {output_file}")
        passed = False

        info.result = options.TaskResult.failed
        info.error_message = "Hap uncompressed failed, cannot exam build products"
        return passed

    if utils.is_esmodule(task.type):
        passed = validate_output_for_esmodule(
            info, task, uncompressed_output_file, is_debug, module)
    else:
        passed = validate_output_for_jsbundle(
            info, task, uncompressed_output_file, is_debug)

    shutil.rmtree(uncompressed_output_file)

    return passed


def run_compile_output(info, task, is_debug, picture_name='', module=''):
    hsp_output_path = task.backup_info.hsp_signed_output_debug if is_debug \
        else task.backup_info.hsp_signed_output_release
    if len(hsp_output_path) < 1:
        backup_hsp_module_compile_signed_package(task, is_debug)

    picture_suffix = 'debug'
    if not is_debug:
        picture_suffix = 'release'
    picture_name = f'{picture_name}_{picture_suffix}'

    runtime_passed = False
    # There is a certain probability of failure when taking screenshots
    try_times = 5
    for i in range(try_times):
        utils.get_running_screenshot(task, picture_name, is_debug, module)
        time.sleep(2)
        if utils.verify_runtime(task, picture_name):
            runtime_passed = True
            break
        else:
            logging.debug(f'get the preview picture failed, retry: {i}/{try_times}')

    if not runtime_passed:
        logging.error(f'The runtime of the {task.name} is inconsistent with the reference screenshot,'
                      f' when running {picture_name}')
        info.runtime_result = options.TaskResult.failed
        info.error_message = "The runtime result is inconsistent with the reference"
    else:
        info.runtime_result = options.TaskResult.passed

    return runtime_passed


# verify preview build picture
def verify_preview_picture(info, task, is_debug, picture_name, module=''):
    return True


def is_compile_success(compile_stdout):
    pattern = r"BUILD SUCCESSFUL in (\d+ min )?(\d+ s )?(\d+ ms)?"
    match_result = re.search(pattern, compile_stdout)
    if not match_result:
        return [False, '']

    return [True, match_result.group(0)]


def validate(compilation_info, task, is_debug, stdout, stderr, picture_name='', output_file=''):
    info = {}
    if is_debug:
        info = compilation_info.debug_info
    else:
        info = compilation_info.release_info

    # ret_code will be 1 if there's stderr, use "COMPILE SUCCESSFUL" as a flag to make a judge
    [is_success, time_string] = is_compile_success(stdout)
    if not is_success:
        info.result = options.TaskResult.failed
        info.error_message = stderr
        return False

    passed = validate_compile_output(info, task, is_debug, output_file)

    if options.arguments.run_haps and picture_name:
        runtime_passed = run_compile_output(info, task, is_debug, picture_name)

    if passed:
        info.time = collect_compile_time(time_string)
        info.result = options.TaskResult.passed

    return passed


def get_hvigor_path():
    hvigor = []
    deveco_path = options.configs.get('deveco_path')
    node_js_path = os.path.join(deveco_path, 'tools', 'node')
    if utils.is_windows():
        node_exe_path = os.path.join(node_js_path, 'node.exe')
        hvigor_script_path = os.path.join(deveco_path, 'tools', 'hvigor', 'bin', 'hvigorw.js')
        hvigor = [node_exe_path, hvigor_script_path]
    else:
        hvigor = [os.path.join(deveco_path, 'hvigorw')]
        utils.add_executable_permission(hvigor)
    return hvigor


def get_hvigor_compile_cmd(task, is_debug, module='', module_name='', module_target='default'):
    cmd = [*get_hvigor_path()]
    build_mode = 'debug' if is_debug else 'release'
    if not module:
        module = 'Hap'
    if module == 'BytecodeHar':
        module = 'Har'
    if not module_name:
        module_name = utils.get_module_name(task, module)
    cmd.extend(['--mode', 'module', '-p', 'product=default', '-p', f'module={module_name}@{module_target}', '-p',
                f'buildMode={build_mode}', f'assemble{module}',
                '--info', '--analyze=advanced', '--module_name', '--incremental', '--daemon'])
    return cmd


def get_preview_mode_compile_cmd(task, is_debug, module='', module_target='default'):
    cmd = [*get_hvigor_path()]
    build_mode = 'debug' if is_debug else 'release'
    module_name = utils.get_module_name(task, module)
    page = os.path.join(*task.inc_modify_file)
    if module == 'Har':
        page = os.path.join(task.har_modify_file)
    elif module == 'Hsp':
        page = os.path.join(task.hsp_modify_file)

    cmd.extend(['--mode', 'module', '-p', f'module={module_name}@{module_target}', '-p', 'product=default',
                '-p', f'buildMode={build_mode}', '-p', 'buildRoot=.preview', '-p', '-p',
                f'previewer.replace.page={page}', '-p', 'pageType=page', '-p', 'compileResInc=true',
                '-p', 'previewMode=true', 'PreviewBuild', '--watch', '--analyze', '--parallel',
                '--incremental', '--daemon'])

    return cmd


def compile_project(task, is_debug, cmd=None):
    if cmd is None:
        cmd = get_hvigor_compile_cmd(task, is_debug)

    logging.debug(f'cmd: {cmd}')
    logging.debug(f"cmd execution path {task.path}")
    process = subprocess.Popen(cmd, shell=False, cwd=task.path,
                               stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = process.communicate(
        timeout=options.arguments.compile_timeout)
    stdout_utf8 = stdout.decode("utf-8", errors="ignore")
    stderr_utf8 = stderr.decode("utf-8", errors="ignore")
    logging.debug(f"cmd stdout: {stdout_utf8}")
    logging.debug(f"cmd stderr: {stderr_utf8}")

    return [stdout_utf8, stderr_utf8]


def preview_mode_build(info, task, is_debug, picture_name='', module=''):
    cmd = get_preview_mode_compile_cmd(task, is_debug, module)
    [stdout, stderr] = compile_project(task, is_debug, cmd)

    [is_success, time_string] = is_compile_success(stdout)
    if not is_success:
        info.result = options.TaskResult.failed
        info.error_message = f'task: {task.name}, Preview compile failed'
        logging.error(f'task: {task.name}, Preview compile failed')
        return False, ''

    is_get_correct_pic = verify_preview_picture(info, task, is_debug, picture_name, module)
    if not is_get_correct_pic:
        info.result = options.TaskResult.failed
        info.error_message = f'task: {task.name}, Get incorrect picture'
        logging.error(f'task: {task.name}, Get incorrect picture')
        return False, ''

    time_string = collect_compile_time(time_string)
    return True, time_string


def clean_compile(task):
    cmd = [*get_hvigor_path(), 'clean']
    logging.debug(f'cmd: {cmd}')
    logging.debug(f"cmd execution path {task.path}")
    process = subprocess.Popen(cmd, shell=False, cwd=task.path,
                               stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    out, err = process.communicate(timeout=options.arguments.compile_timeout)


def clean_preview_cache(task, module=''):
    module_path = utils.get_module_path(task, module)

    preview_cache_path = os.path.join(task.path, *module_path, '.preview')
    if os.path.exists(preview_cache_path):
        shutil.rmtree(preview_cache_path)
        logging.debug(f"delete preview cache successfully on this path: {preview_cache_path}")


def sync_project(task):
    ohpm_bat_path = os.path.join(options.configs.get('deveco_path'), 'tools', 'ohpm', 'bin', 'ohpm.bat')
    ohpm_install_cmd_suffix = ' install --all --registry https://repo.harmonyos.com/ohpm/ --strict_ssl true'
    ohpm_install_cmd = f'"{ohpm_bat_path}"' + ohpm_install_cmd_suffix
    cmd_suffix = '--sync -p product=default -p buildMode=debug --analyze --parallel --incremental --daemon'
    cmd = [*get_hvigor_path(), cmd_suffix]
    logging.debug(f"cmd execution path {task.path}")
    logging.debug(f'ohpm install cmd: {ohpm_install_cmd}')
    subprocess.Popen(ohpm_install_cmd, shell=False, cwd=task.path,
                     stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    logging.debug(f'sync cmd: {cmd}')
    subprocess.Popen(cmd, shell=False, cwd=task.path,
                     stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    # If you don't wait it may cause: current process status is busy, cannot start a build action
    time.sleep(5)


def compile_full(task, is_debug):
    if not FullTest.full_compile(task, is_debug):
        return False

    FullTest.compile_full_import_ordinary_ohpm_package(task, is_debug)
    FullTest.compile_full_import_special_ohpm_package(task, is_debug)
    FullTest.compile_full_import_static_library(task, is_debug)
    FullTest.compile_full_import_share_library(task, is_debug)
    FullTest.compile_full_import_so_file(task, is_debug)
    FullTest.compile_full_has_syntax_error_in_js(task, is_debug)
    FullTest.compile_full_use_normalize_ohmurl(task, is_debug)
    FullTest.compile_full_module_name_is_inconsistent(task, is_debug)

    return True


def compile_incremental(task, is_debug):
    logging.info(
        f"==========> Running task: {task.name} in incremental compilation")
    clean_compile(task)
    [stdout, stderr] = compile_project(task, is_debug)

    [is_success, time_string] = is_compile_success(stdout)
    if not is_success:
        logging.error(
            "Incremental compile failed due to first compile failed!")
        return

    if options.arguments.compile_mode == 'incremental':
        passed = validate(task.full_compilation_info,
                          task, is_debug, stdout, stderr, 'incremental_compile_first')
        if not passed:
            logging.error(
                "Incremental compile failed due to first compile failed!")
            return

    backup_compile_output(task, is_debug)
    backup_compile_cache(task, is_debug)

    IncrementalTest.compile_incremental_no_modify(task, is_debug)
    IncrementalTest.compile_incremental_add_oneline(task, is_debug)
    IncrementalTest.compile_incremental_add_file(task, is_debug)
    IncrementalTest.compile_incremental_add_nonexistent_file(task, is_debug)
    IncrementalTest.compile_incremental_delete_file(task, is_debug)

    IncrementalTest.compile_incremental_build_modify_error_then_fix(task, is_debug)
    IncrementalTest.compile_incremental_build_add_error_page(task, is_debug)
    IncrementalTest.compile_incremental_build_add_error_non_page(task, is_debug)
    IncrementalTest.compile_incremental_build_entry_then_har(task, is_debug)
    IncrementalTest.compile_incremental_build_har_then_entry(task, is_debug)
    IncrementalTest.compile_incremental_build_entry_then_hsp(task, is_debug)
    IncrementalTest.compile_incremental_build_hsp_then_entry(task, is_debug)
    IncrementalTest.compile_incremental_build_hsp_then_ohos(task, is_debug)
    IncrementalTest.compile_incremental_build_entry_then_ohos(task, is_debug)
    IncrementalTest.compile_incremental_build_entry_then_preview_build(task, is_debug)

    # These tests require modifying the test files and synchronizing the project,
    # which may result in unexpected modifications
    IncrementalTest.compile_incremental_reverse_hap_mode(task, is_debug)
    IncrementalTest.compile_incremental_modify_module_name(task, is_debug)
    IncrementalTest.compile_incremental_build_modify_sdk_version(task, is_debug)


def compile_bytecode_har(task, is_debug):
    logging.info(f"==========> Running task: {task.name} in bytecode har compilation")
    clean_compile(task)

    BytecodeHarTest.build_bytecode_har(task, is_debug)
    BytecodeHarTest.build_har_then_bytecode_har(task, is_debug)
    BytecodeHarTest.import_bytecode_static_library(task, is_debug)


def compile_external(task, is_debug):
    logging.info(f"==========> Running task: {task.name} in external compilation")
    clean_compile(task)

    ExternalTest.import_external_share_library(task, is_debug)
    ExternalTest.import_external_static_library(task, is_debug)
    ExternalTest.full_compile_external_static_library(task, is_debug)
    ExternalTest.full_compile_external_share_library(task, is_debug)


def compile_preview(task, is_debug):
    clean_preview_cache(task)
    if not PreviewTest.preview_compile(task, is_debug):
        logging.error('Preview build failed, skip other preview tests')
        return

    PreviewTest.compile_preview_build_entry_then_preview(task, is_debug)
    PreviewTest.compile_preview_build_modify_file_name(task, is_debug)
    PreviewTest.compile_preview_build_generate_sourcemap(task, is_debug)
    PreviewTest.compile_preview_build_tigger_incremental_build(task, is_debug)
    PreviewTest.compile_preview_build_has_arkui_error(task, is_debug)
    PreviewTest.compile_preview_build_sdk_path_has_special_char(task, is_debug)
    PreviewTest.compile_preview_build_modify_error_then_fix(task, is_debug)


def backup_compile_output(task, is_debug):
    backup_path = task.backup_info.cache_path
    if not os.path.exists(backup_path):
        os.mkdir(backup_path)

    if is_debug:
        if len(task.backup_info.output_debug) == 2:
            return

        backup_output_path = os.path.join(backup_path, 'output', 'debug')
        if not os.path.exists(backup_output_path):
            os.makedirs(backup_output_path)

    else:
        if len(task.backup_info.output_release) == 2:
            return

        backup_output_path = os.path.join(backup_path, 'output', 'release')
        if not os.path.exists(backup_output_path):
            os.makedirs(backup_output_path)

    output_file = get_compile_output_file_path(task, '', options.OutputType.unsigned)
    shutil.copy(output_file, backup_output_path)
    backup_output = os.path.join(
        backup_output_path, os.path.basename(output_file))
    backup_time_output = backup_output + '-' + utils.get_time_string()
    shutil.move(backup_output, backup_time_output)

    if is_debug:
        task.backup_info.output_debug.append(backup_time_output)
    else:
        task.backup_info.output_release.append(backup_time_output)


def backup_hsp_module_compile_signed_package(task, is_debug):
    if not options.arguments.run_haps:
        return

    backup_path = task.backup_info.cache_path
    if not os.path.exists(backup_path):
        os.mkdir(backup_path)

    cmd = get_hvigor_compile_cmd(task, is_debug, 'Hsp')
    stdout, stderr = compile_project(task, is_debug, cmd)
    passed, build_time = is_compile_success(stdout)
    if not passed:
        logging.debug(f'cmd: {cmd}')
        logging.debug(f"cmd execution path {task.path}")
        return

    external_task = ExternalTest.get_external_task()
    cmd = get_hvigor_compile_cmd(external_task, is_debug, 'Hsp')
    stdout, stderr = compile_project(task, is_debug, cmd)
    passed, build_time = is_compile_success(stdout)
    if not passed:
        logging.debug(f'cmd: {cmd}')
        logging.debug(f"cmd execution path {task.path}")
        return

    backup_output_path = os.path.join(backup_path, 'output', 'debug') if is_debug \
        else os.path.join(backup_path, 'output', 'release')
    if not os.path.exists(backup_output_path):
        os.makedirs(backup_output_path)
    output_file = get_compile_output_file_path(task, 'Hsp', options.OutputType.signed)
    backup_output = os.path.join(backup_output_path, os.path.basename(output_file))
    shutil.copy(output_file, backup_output_path)
    output_file = get_compile_output_file_path(external_task, 'Hsp', options.OutputType.signed)
    external_hsp_backup_output = os.path.join(backup_output_path, os.path.basename(output_file))
    shutil.copy(output_file, backup_output_path)

    if is_debug:
        task.backup_info.hsp_signed_output_debug = backup_output
        task.backup_info.external_hsp_signed_output_debug = external_hsp_backup_output
    else:
        task.backup_info.hsp_signed_output_release = backup_output
        task.backup_info.external_hsp_signed_output_release = external_hsp_backup_output


def backup_preview_output(task, is_debug, module):
    backup_path = task.backup_info.cache_path
    if not os.path.exists(backup_path):
        os.mkdir(backup_path)

    if is_debug:
        # We only need two backup files to compare in the ohter tests.
        if len(task.backup_info.preview_output_debug) == 2:
            return

        backup_preview_output_path = os.path.join(backup_path, 'preview', 'debug')
        if not os.path.exists(backup_preview_output_path):
            os.makedirs(backup_preview_output_path)

    if is_debug:
        if len(task.backup_info.preview_output_release) == 2:
            return

        backup_preview_output_path = os.path.join(backup_path, 'preview', 'release')
        if not os.path.exists(backup_preview_output_path):
            os.makedirs(backup_preview_output_path)

    preview_output_path = os.path.join((task.path, module, '.preview'))
    shutil.copy(preview_output_path, backup_path)
    backup_preview_output_dir = os.path.join(
        backup_preview_output_path, os.path.basename(preview_output_path))
    preview_backup_time_out = backup_preview_output_path + utils.get_time_string()
    shutil.move(backup_preview_output_dir, preview_backup_time_out)
    if is_debug:
        task.backup_info.output_debug.append(preview_backup_time_out)
    else:
        task.backup_info.output_release.append(preview_backup_time_out)


def backup_compile_cache(task, is_debug):
    backup_path = task.backup_info.cache_path
    if not os.path.exists(backup_path):
        os.mkdir(backup_path)

    backup_cache_path = os.path.join(backup_path, 'cache')
    if not os.path.exists(backup_cache_path):
        os.mkdir(backup_cache_path)
    cache_files = os.path.join(
        task.path, *task.hap_module_path, *(task.build_path), *(task.cache_path))

    if is_debug:
        if task.backup_info.cache_debug != '':
            return

        cache_files = os.path.join(cache_files, 'debug')
        backup_cache_file = os.path.join(backup_cache_path, 'debug')
        shutil.copytree(cache_files, backup_cache_file)
        task.backup_info.cache_debug = backup_cache_file
    else:
        if task.backup_info.cache_release != '':
            return

        cache_files = os.path.join(cache_files, 'release')
        backup_cache_file = os.path.join(backup_cache_path, 'release')
        shutil.copytree(cache_files, backup_cache_file)
        task.backup_info.cache_release = backup_cache_file


def backup_preview_cache(task, is_debug, module=''):
    backup_path = task.backup_info.cache_path
    if not os.path.exists(backup_path):
        os.mkdir(backup_path)

    preview_backup_cache_path = os.path.join(backup_path, 'preview_cache')
    if not os.path.exists(preview_backup_cache_path):
        os.mkdir(preview_backup_cache_path)
    module_path = utils.get_module_path(task, module)
    preview_cache_files = os.path.join(
        task.path, *module_path, *task.preview_path, *task.preview_cache_path)

    if is_debug:
        if task.backup_info.preview_cache_debug != '':
            return

        preview_cache_files = os.path.join(preview_cache_files, 'debug')
        preview_backup_cache_file = os.path.join(preview_backup_cache_path, 'debug')
        shutil.copytree(preview_cache_files, preview_backup_cache_file)
        task.backup_info.preview_cache_debug = preview_backup_cache_file
    else:
        if task.backup_info.preview_cache_release != '':
            return

        preview_cache_files = os.path.join(preview_cache_files, 'release')
        preview_backup_cache_file = os.path.join(preview_backup_cache_path, 'release')
        shutil.copytree(preview_cache_files, preview_backup_cache_file)
        task.backup_info.preview_cache_release = preview_backup_cache_file


def execute_full_compile(task):
    logging.info(
        f"==========> Running task: {task.name} in full compilation")
    passed = True
    if options.arguments.hap_mode in ['all', 'release']:
        passed = passed and compile_full(task, False)
        clean_compile(task)
    if options.arguments.hap_mode in ['all', 'debug']:
        passed = passed and compile_full(task, True)
        clean_compile(task)
    return passed


def execute_incremental_compile(task):
    logging.info(
        f"==========> Running task: {task.name} in incremental compilation")
    if options.arguments.hap_mode in ['all', 'release']:
        compile_incremental(task, False)
        clean_compile(task)
    if options.arguments.hap_mode in ['all', 'debug']:
        compile_incremental(task, True)
        clean_compile(task)


def execute_bytecode_har_compile(task):
    logging.info(
        f"==========> Running task: {task.name} in bytecode har compilation")
    if options.arguments.hap_mode in ['all', 'release']:
        compile_bytecode_har(task, False)
        clean_compile(task)
    if options.arguments.hap_mode in ['all', 'debug']:
        compile_bytecode_har(task, True)
        clean_compile(task)


def execute_external_compile(task):
    logging.info(
        f"==========> Running task: {task.name} in external compilation")
    if options.arguments.hap_mode in ['all', 'release']:
        compile_external(task, False)
        clean_compile(task)
    if options.arguments.hap_mode in ['all', 'debug']:
        compile_external(task, True)
        clean_compile(task)


def execute_preview_compile(task):
    logging.info(
        f"==========> Running task: {task.name} in preview compilation")
    compile_preview(task, True)
    clean_compile(task)


def clean_backup(task):
    if os.path.exists(task.backup_info.cache_path):
        shutil.rmtree(task.backup_info.cache_path)
    return


def is_build_module_successful(task, is_debug, info, module='', picture_name=''):
    cmd = get_hvigor_compile_cmd(task, is_debug, module)
    [stdout, stderr] = compile_project(task, is_debug, cmd)
    [is_success, time_string] = is_compile_success(stdout)
    if not is_success:
        info.result = options.TaskResult.failed
        module_name = utils.get_module_name(task, module)
        info.error_message = f'Compile failed due to build {module_name} module.'
        logging.error(f'build {module_name} failed')
        return False, ''

    passed = validate_compile_output(info, task, is_debug, '', module)

    if options.arguments.run_haps and picture_name:
        runtime_passed = run_compile_output(info, task, is_debug, picture_name)

    time_string = collect_compile_time(time_string)
    return passed, time_string


def is_get_expected_error(info, stderr, expect_errors):
    passed = False
    for expect_error in expect_errors:
        if expect_error in stderr:
            passed = True
            break

    if not passed:
        logging.error(f"True message: {stderr}, didn't get expected error message: {expect_errors}")
        info.result = options.TaskResult.failed
        info.error_message = f'Expected error message: {expect_errors}'

    return passed


def is_build_ohos_test_successful(task, info):
    cmd = [*get_hvigor_path(), '--mode', 'module',
           '-p', 'module=entry@ohosTest', 'assembleHap']
    [stdout, stderr] = compile_project(task, True, cmd)
    [is_success, time_string] = is_compile_success(stdout)

    if not is_success:
        info.result = options.TaskResult.failed
        info.error_message = f'Compile failed due to build ohos test.'
        logging.error(f'build ohos test failed')
        return False, ''

    time_string = collect_compile_time(time_string)
    return True, time_string


def modify_main_pages_json(task, page_path, reverse):
    main_pages_json_path = os.path.join(task.path, *task.main_pages_json_path)
    with open(main_pages_json_path, 'r+', encoding='utf-8') as json_file:
        json_data = json.load(json_file)
        pages_dic = json_data['src']
        if reverse:
            if page_path not in pages_dic:
                pages_dic.append(page_path)
            logging.info(f'Page {page_path} is already in the list')
        else:
            if page_path in pages_dic:
                pages_dic.remove(page_path)
            else:
                logging.error(f'Page {page_path} not found in the list')

        json_file.seek(0)
        json.dump(json_data, json_file, indent=2)
        json_file.truncate()
    sync_project(task)


def add_or_delete_page(task, reverse, is_error=False):
    patch_content = options.configs.get(
        'patch_content').get('patch_new_file_ets')
    patch_lines_error = options.configs.get(
        'patch_content').get('patch_lines_error')
    page_path = patch_content.get('path')
    new_file_page = patch_content.get('name')
    new_file_content = patch_content.get('component')
    new_file_directory = os.path.dirname(os.path.join(task.path, *task.inc_modify_file))
    new_file_path = os.path.join(new_file_directory, new_file_page)

    if reverse:
        modify_main_pages_json(task, page_path, 1)
        with open(new_file_path, 'a', encoding='utf-8') as file:
            file.write(new_file_content)
            if is_error:
                error_content = patch_lines_error.get('content')
                file.write(error_content)
    else:
        if is_error:
            utils.remove_content_from_file(new_file_path, '', patch_lines_error.get('content'))
        else:
            modify_main_pages_json(task, page_path, 0)
            os.remove(new_file_path)


def add_or_delete_js_file(task, reverse, is_error=False):
    modify_file = os.path.join(task.path, *task.inc_modify_file)
    patch_content = options.configs.get('patch_content')

    patch_new_file_js = patch_content.get('patch_new_file_js')
    js_file_name = patch_new_file_js.get('name')
    js_content = patch_new_file_js.get('content')
    modify_dic = os.path.dirname(modify_file)
    js_file_path = os.path.join(modify_dic, js_file_name)
    path_lines = patch_content.get('patch_lines_1').get('js')

    if reverse:
        with open(js_file_path, 'a', encoding='utf-8') as file:
            file.write(js_content)
            if is_error:
                error_content = patch_content.get('patch_lines_error').get('content')
                file.write(error_content)

        utils.add_content_to_file(modify_file, path_lines.get('head'), path_lines.get('tail'))
    else:
        if is_error:
            tail_content = patch_content.get('patch_lines_error').get('content')
            utils.remove_content_from_file(js_file_path, '', tail_content)
        else:
            os.remove(js_file_path)
            utils.remove_content_from_file(modify_file, path_lines.get('head'), path_lines.get('tail'))


def modify_normalize_ohmurl_options(task, reverse):
    oh_package_json_path = os.path.join(task.path, 'build-profile.json5')
    with open(oh_package_json_path, 'r+', encoding='utf-8') as json_file:
        json_data = json5.load(json_file)
        products = json_data['app']['products'][0]
        if 'buildOption' not in products:
            products['buildOption'] = {}
        build_option = products['buildOption']
        if 'strictMode' not in build_option:
            build_option['strictMode'] = {}
        strict_mode = build_option['strictMode']
        # if 'useNormalizedOHMUrl' not in products:
        if reverse:
            strict_mode['useNormalizedOHMUrl'] = True
        else:
            strict_mode['useNormalizedOHMUrl'] = False

        json_file.seek(0)
        json5.dump(json_data, json_file, indent=4, ensure_ascii=False)
        json_file.truncate()
    sync_project(task)


def modify_module_import_handle(task, module, reverse):
    modify_file = os.path.join(task.path, *task.inc_modify_file)
    modify_file_patch = options.configs.get('patch_content').get('patch_lines_1').get(module.lower())
    if reverse:
        utils.add_content_to_file(modify_file, modify_file_patch.get('head'),
                                  modify_file_patch.get('tail'))
    else:
        utils.remove_content_from_file(modify_file, modify_file_patch.get('head'),
                                       modify_file_patch.get('tail'))


@contextmanager
def manage_module_import_and_export_handle(task, module_name):
    modify_module_import_handle(task, module_name, 1)
    try:
        yield
    finally:
        modify_module_import_handle(task, module_name, 0)


@contextmanager
def manage_bytecode_har_dependency(task, is_debug, info, module):
    modify_module_import_handle(task, module, 1)
    is_build_module_successful(task, is_debug, info, 'BytecodeHar')
    modify_bytecode_module_dependency(task, module, 1)
    try:
        yield
    finally:
        modify_bytecode_module_dependency(task, module, 0)
        modify_module_import_handle(task, module, 0)


def modify_bytecode_module_dependency(task, module, reverse):
    oh_package_json_path = os.path.join(task.path, task.hap_module, 'oh-package.json5')
    with open(oh_package_json_path, 'r+', encoding='utf-8') as json_file:
        json_data = json5.load(json_file)
        dependencies_dic = json_data["dependencies"]
        patch_lines = options.configs.get('patch_content').get('patch_lines_1')
        dependency_name = utils.extract_library_names(patch_lines.get(module.lower()).get('head'))
        if reverse:
            dependency_path = os.path.join(task.har_module, *task.build_path, *task.har_output_path_har)
        else:
            dependency_path = utils.get_module_name(task, module)
        dependencies_dic[dependency_name] = os.path.normpath(f"file:../{dependency_path}")
        json_file.seek(0)
        json.dump(json_data, json_file, indent=4)
        json_file.truncate()
    sync_project(task)


def modify_bytecode_har_config(task, reverse):
    modify_normalize_ohmurl_options(task, reverse)
    module_path = utils.get_module_path(task, 'Har')
    har_build_profile_json_path = os.path.join(task.path, *module_path, 'build-profile.json5')
    with open(har_build_profile_json_path, 'r+', encoding='utf-8') as json_file:
        json_data = json5.load(json_file)
        build_option_dic = json_data["buildOption"]
        if reverse:
            build_option_dic["arkOptions"] = {"byteCodeHar": True}
        else:
            build_option_dic["arkOptions"] = {"byteCodeHar": False}
        json_file.seek(0)
        json.dump(json_data, json_file, indent=4)
        json_file.truncate()
    sync_project(task)


def validate_cache_file(task, info, modified_files, cache_path, backup_path):
    cache_extension = utils.get_cache_extension(task.type)
    modified_cache_files = []
    # modified_files is a list of file with relative path to .../debug/release
    for file in modified_files:
        name, ext = os.path.splitext(file)
        modified_cache_files.append(name + cache_extension)

    for root, dirs, files in os.walk(cache_path):
        for file in files:
            if not file.endswith(cache_extension):
                continue
            file_absolute_path = os.path.join(root, file)
            file_relative_path = os.path.relpath(file_absolute_path, cache_path)
            backup_file = os.path.join(backup_path, file_relative_path)

            if not os.path.exists(backup_file):
                logging.debug(f"backup file not exits: {backup_file}")
                continue

            if utils.is_file_timestamps_same(file_absolute_path, backup_file):
                continue

            logging.debug(f"found file ${file_relative_path} changed")
            is_file_in_list = IncrementalTest.is_file_in_modified_files(
                task.type, file_relative_path, modified_cache_files)
            logging.debug(f"is file in list: {is_file_in_list}")
            if not is_file_in_list:
                logging.debug(f"Unexpected file modified: {file_relative_path}")
                info.result = options.TaskResult.failed
                info.error_message = f'Incremental compile found unexpected file timestamp changed. \
                                             Changed file: {file_relative_path}'
                return False

    return True


def get_output_uncompressed_file(task, info, module, output_type=options.OutputType.unsigned):
    output_file = get_compile_output_file_path(task, module, output_type)
    uncompressed_output_file = output_file + '.uncompressed'
    if not os.path.exists(output_file):
        logging.error(f"outputfile: {output_file} for task: {task.name} not found")
        info.result = options.TaskResult.failed
        return ''

    try:
        if utils.check_zip_file(output_file):
            with zipfile.ZipFile(output_file, 'r') as zip_ref:
                zip_ref.extractall(uncompressed_output_file)
        elif utils.check_gzip_file(output_file):
            with tarfile.open(output_file, 'r:gz') as tar_ref:
                tar_ref.extractall(uncompressed_output_file)
        else:
            logging.error(
                f"task: {task.name},not the expected file type for output file: {output_file}")
            info.result = options.TaskResult.failed
            return ''
    except Exception as e:
        logging.error(e)
        logging.error(
            f"uncompressed output file for task {task.name} failed. output file: {output_file}")
        info.result = options.TaskResult.failed
        return ''
    if module == 'Har' or module == 'BytecodeHar':
        uncompressed_output_file = os.path.join(uncompressed_output_file, 'package')
    return uncompressed_output_file


def get_disasm_abc_file(task, info, module, uncompressed_output_file=''):
    if not uncompressed_output_file:
        uncompressed_output_file = get_output_uncompressed_file(task, info, module)

    if not os.path.exists(uncompressed_output_file):
        info.result = options.TaskResult.failed
        info.error_message = "uncompressed file not found"
        return ''

    abc_path = ''
    if utils.is_esmodule(task.type):
        abc_path = os.path.join(uncompressed_output_file, 'ets', 'modules.abc')

    if not os.path.exists(abc_path):
        info.result = options.TaskResult.failed
        info.error_message = "abc file not found"
        return ''

    modules_abc_size = os.path.getsize(abc_path)
    if modules_abc_size <= 0:
        info.result = options.TaskResult.failed
        info.error_message = "abc file size is 0"
        return ''

    return disasm_abc(task, abc_path)


def is_package_modules_to_module_abc(task, pa_file, module):
    module_str = f'{task.hap_module}@{module}'

    return utils.file_contains_specified_fields(pa_file, module_str)


def is_normalized_ohm_url(task, is_debug, info):
    build_path = os.path.join(task.path, *task.hap_module_path, *task.build_path)
    cache_path = os.path.join(build_path, *task.cache_path, 'debug') if is_debug \
        else os.path.join(build_path, *task.cache_path, 'release')
    inc_modify_file = os.path.join(*task.inc_modify_file)
    dir_name, base_name = os.path.split(inc_modify_file)
    file_name, _ = os.path.splitext(base_name)
    ts_file_name = f'{file_name}.ts'
    ts_file_path = os.path.join(cache_path, dir_name, ts_file_name)
    url_string = '@normalized'

    passed = utils.file_contains_specified_fields(ts_file_path, url_string)
    if not passed:
        info.result = options.TaskResult.failed
        logging.error(f'{ts_file_path} does not contain {url_string}')

    return passed


def is_npm_txt_included_ohpm_package(info, task, is_debug, package_name):
    cache_file = os.path.join(task.path, *task.hap_module_path, *task.build_path, *task.cache_path)
    npm_entries_path = os.path.join(cache_file, 'debug', 'npmEntries.txt') if is_debug else \
        os.path.join(cache_file, 'release', 'npmEntries.txt')

    if not os.path.exists(npm_entries_path):
        logging.error(f'{npm_entries_path} does not exist')
        info.result = options.TaskResult.failed
        return False

    is_included = utils.file_contains_specified_fields(npm_entries_path, package_name)
    if not is_included:
        info.result = options.TaskResult.failed
        logging.error(f'{npm_entries_path} does not contain {package_name}')

    return is_included


def modify_sdk_version(task, api_version):
    build_profile_json_file = os.path.join(task.path, 'build-profile.json5')
    with open(build_profile_json_file, 'r+', encoding='utf-8') as json_file:
        json_data = json5.load(json_file)
        products = json_data["app"]["products"][0]
        compatible_sdk_version = products["compatibleSdkVersion"]
        if isinstance(compatible_sdk_version, str):
            api_version_file_name_map = options.configs.get('api_version_file_name_map')
            version_str = str(api_version)
            for key in api_version_file_name_map.keys():
                if version_str in key:
                    version_str = key
                    break
            products["compatibleSdkVersion"] = version_str
        else:
            products["compatibleSdkVersion"] = api_version

        json_file.seek(0)
        json5.dump(json_data, json_file, indent=2)
        json_file.truncate()

    sync_project(task)


# Preview files require significant modifications for comparison purposes.
# Here, we will create a relatively simple page for testing.
def add_or_delete_arkui_component(task, reverse, is_error=False):
    preview_modify_file = os.path.join(task.path, *task.inc_modify_file)
    preview_modify_file_bak = preview_modify_file + '.bak'

    if reverse:
        shutil.copy(preview_modify_file, preview_modify_file_bak)
        with open(preview_modify_file, 'r+', encoding='utf-8') as file:
            arkui_patch = options.configs.get('patch_content').get('arkui_patch')
            content = arkui_patch.get('content')
            pattern = re.compile(r'(build\(\)\s*\{\s*)([^{}]*)(\s*\})', re.DOTALL)
            component = arkui_patch.get('error_component') if is_error else arkui_patch.get('component')
            replacement = r'\1\2{}\3'.format(component)
            new_content = re.sub(pattern, replacement, content)
            file.seek(0)
            file.write(new_content)
            file.truncate()
    else:
        os.remove(preview_modify_file)
        os.rename(preview_modify_file_bak, preview_modify_file)


def execute(test_tasks):
    for task in test_tasks:
        try:
            logging.info(f"======> Running task: {task.name}")
            if options.arguments.compile_mode in ['all', 'full']:
                if not execute_full_compile(task):
                    logging.info("Full compile failed, skip other tests!")
                    continue

            if options.arguments.compile_mode in ['all', 'incremental']:
                execute_incremental_compile(task)

            if options.arguments.compile_mode in ['all', 'bytecode_har']:
                execute_bytecode_har_compile(task)

            if options.arguments.compile_mode in ['all', 'external']:
                execute_external_compile(task)

            if options.arguments.compile_mode in ['all', 'preview']:
                execute_preview_compile(task)

            OtherTest.verify_binary_consistency(task)

            # for these tests, use one hapMode maybe enough
            is_debug = True if options.arguments.hap_mode == 'debug' else False
            OtherTest.execute_break_compile(task, is_debug)
            if 'error' in task.type:
                OtherTest.compile_full_with_error(task, is_debug)

            if 'exceed_length_error' in task.type:
                OtherTest.compile_with_exceed_length(task, is_debug)

            if 'ohosTest' in task.type:
                OtherTest.compile_ohos_test(task)

            logging.info(f"======> Running task: {task.name} finished")
        except Exception as e:
            logging.exception(e)
        finally:
            clean_backup(task)
