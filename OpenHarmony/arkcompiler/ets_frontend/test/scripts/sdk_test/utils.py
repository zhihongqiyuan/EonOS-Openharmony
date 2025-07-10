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

Description: utils for test suite
"""

import gzip
import logging
import os
import re
import shutil
import subprocess
import sys
import time
import zipfile

from PIL import Image

import options


def get_log_level(arg_log_level):
    log_level_dict = {
        'debug': logging.DEBUG,
        'info': logging.INFO,
        'warn': logging.WARN,
        'error': logging.ERROR
    }
    if arg_log_level not in log_level_dict.keys():
        return logging.ERROR  # use error as default log level
    else:
        return log_level_dict[arg_log_level]


def init_logger(log_level, log_file):
    logging.basicConfig(filename=log_file,
                        level=get_log_level(log_level),
                        encoding=get_encoding(),
                        format='[%(asctime)s %(filename)s:%(lineno)d]: [%(levelname)s] %(message)s')
    logging.info("Test command:")
    logging.info(" ".join(sys.argv))


def get_encoding():
    if is_windows():
        return 'utf-8'
    else:
        return sys.getfilesystemencoding()


def check_zip_file(file_path):
    try:
        if zipfile.is_zipfile(file_path):
            with zipfile.ZipFile(file_path, 'r'):
                return True
        else:
            return False
    except Exception as e:
        print(e)
        return False


def check_gzip_file(file_path):
    try:
        with gzip.open(file_path, 'rb') as gzfile:
            gzfile.read(1)
    except Exception as e:
        print(e)
        return False
    return True


def is_windows():
    return sys.platform == 'win32' or sys.platform == 'cygwin'


def is_mac():
    return sys.platform == 'darwin'


def is_linux():
    return sys.platform == 'linux'


def get_time_string():
    return time.strftime('%Y%m%d-%H%M%S')


def is_esmodule(hap_type):
    # if hap_type is stage, it's esmodule.
    # if hap_type is js, fa, compatible 8, it's js_bundle
    return 'stage' in hap_type


def is_file_timestamps_same(file_a, file_b):
    file_a_mtime = os.stat(file_a).st_mtime
    file_b_mtime = os.stat(file_b).st_mtime
    return file_a_mtime == file_b_mtime


def is_file_name_same(file_a, file_b):
    file_a_name = os.path.basename(file_a)
    file_b_name = os.path.basename(file_b)
    return file_a_name == file_b_name


def add_executable_permission(file_path):
    current_mode = os.stat(file_path).st_mode
    new_mode = current_mode | 0o111
    os.chmod(file_path, new_mode)


def replace_file_content(file_path, old_content, new_content):
    with open(file_path, 'r+', encoding='utf-8') as file:
        content = file.read()
        content = content.replace(old_content, new_content)
        file.seek(0)
        file.write(content)
        file.truncate()


def run_cmd(cmd):
    logging.debug(f'cmd: {cmd}')
    result = subprocess.run(cmd, shell=False, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
    logging.debug(f'cmd stdout: {result.stdout}')
    logging.error(f'cmd stderr: {result.stderr}')
    return result


def move_picture(task, image_name):
    pic_save_dic = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'pictures')
    if not os.path.exists(pic_save_dic):
        os.mkdir(pic_save_dic)

    pic_save_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), f'pictures/{task.name}')
    if not os.path.exists(pic_save_path):
        os.mkdir(pic_save_path)

    pic_file_path = os.path.join(pic_save_path, f'{image_name}.jpeg')
    if os.path.exists(pic_file_path):
        os.remove(pic_file_path)

    shutil.move(f'{image_name}.jpeg', pic_save_path)


def get_running_screenshot(task, image_name, is_debug, module=''):
    logging.debug(f'Getting runtime screenshot of {task.name}')
    run_cmd(['hdc', 'shell', 'power-shell', 'wakeup;power-shell', 'setmode 602'])
    run_cmd(['hdc', 'shell', 'uinput', '-T', '-m', '420', '1000', '420',
             '400;uinput', '-T', '-m', '420', '400', '420', '1000'])

    module_path = get_module_path(task, module)
    output_path_signed = get_output_path(task, module, options.OutputType.signed)
    build_path = os.path.join(task.path, *module_path, *task.build_path)
    out_path = os.path.join(build_path, *output_path_signed)

    result = run_cmd(['hdc', 'install', f'{out_path}'])
    # After importing Hsp, Hap needs to install the Hsp package first before installing the Hap package.
    not_hsp_error_message = 'Failed to install the HAP or HSP because the dependent module does not exist'
    if not_hsp_error_message in result.stdout:
        hsp_output_path = task.backup_info.hsp_signed_output_debug if is_debug \
            else task.backup_info.hsp_signed_output_release
        run_cmd(['hdc', 'install', f'{hsp_output_path}'])
        time.sleep(3)
    not_out_hsp_error_message = 'outHsp does not exist'
    if not_out_hsp_error_message in result.stdout:
        external_hsp_output_path = task.backup_info.external_hsp_signed_output_debug if is_debug \
            else task.backup_info.external_hsp_signed_output_release
        run_cmd(['hdc', 'install', f'{external_hsp_output_path}'])
        time.sleep(3)

    if not_hsp_error_message in result.stdout or not_out_hsp_error_message in result.stdout:
        run_cmd(['hdc', 'install', f'{out_path}'])

    run_cmd(['hdc', 'shell', 'aa', 'start', '-a', f'{task.ability_name}', '-b', f'{task.bundle_name}'])
    time.sleep(3)

    screen_path = f'/data/local/tmp/{image_name}.jpeg'
    run_cmd(['hdc', 'shell', 'snapshot_display', '-f', f'{screen_path}'])
    time.sleep(3)

    run_cmd(['hdc', 'file', 'recv', f'{screen_path}', f'{image_name}.jpeg'])
    run_cmd(['hdc', 'shell', 'aa', 'force-stop', f'{task.bundle_name}'])
    run_cmd(['hdc', 'shell', 'bm', 'uninstall', '-n', f'{task.bundle_name}'])

    move_picture(task, image_name)


def compare_screenshot(runtime_picture_path, picture_reference_path, threshold=0.95):
    try:
        runtime_picture = Image.open(runtime_picture_path).convert('RGB')
        picture_reference_path = Image.open(picture_reference_path).convert('RGB')
    except Exception:
        logging.error(f'open image {runtime_picture_path} failed')
        return False
    runtime_picture.thumbnail((256, 256))
    picture_reference_path.thumbnail((256, 256))

    runtime_pixel = runtime_picture.load()
    reference_pixel = picture_reference_path.load()
    width, height = runtime_picture.size

    similar_pixels = 0
    total_pixels = width * height

    for x in range(width):
        for y in range(height):
            if runtime_pixel[x, y] == reference_pixel[x, y]:
                similar_pixels += 1

    similarity = similar_pixels / total_pixels

    if similarity >= threshold:
        return True
    else:
        return False


def verify_runtime(task, picture_name):
    pic_path = os.path.join(os.path.dirname(os.path.abspath(__file__)),
                            f'pictures/{task.name}/{picture_name}.jpeg')
    pic_path_reference = os.path.join(os.path.dirname(os.path.abspath(__file__)),
                                      f'pictures_reference/{task.name}/{picture_name}.jpeg')
    passed = compare_screenshot(pic_path, pic_path_reference, threshold=0.95)
    if not passed:
        logging.error(f'{task.name} get error when running')
        return False
    return True


def add_content_to_file(file_path, head_content, tail_content):
    if not head_content and not tail_content:
        logging.error('Both head_content and tail_content are missing,please check!')
        return

    with open(file_path, 'r+', encoding='utf-8') as file:
        old_content = file.read()
        file.seek(0)
        if head_content:
            file.write(head_content)
        file.write(old_content)
        if tail_content:
            file.write(tail_content)
        file.truncate()


def remove_content_from_file(file_path, head_content, tail_content):
    if not head_content and not tail_content:
        logging.error('Both head_content and tail_content are missing,please check!')
        return

    with open(file_path, 'r+', encoding='utf-8') as file:
        old_content = file.read()
        if head_content and old_content.startswith(head_content):
            old_content = old_content[len(head_content):]
        elif head_content:
            logging.debug(f'Cannot find the head content to remove in {file_path}')

        if tail_content and old_content.endswith(tail_content):
            old_content = old_content[:-len(tail_content)]
        elif tail_content:
            logging.debug(f'Cannot find the tail content to remove in {file_path}')

        file.seek(0)
        file.write(old_content)
        file.truncate()


def extract_library_names(import_statement):
    pattern = r"import\s+{[^}]+}\s+from\s+'([^']+)';"
    matches = re.findall(pattern, import_statement)

    return matches[0]


def get_module_name(task, module=''):
    module_mapping = {
        'Hap': task.hap_module,
        'Har': task.har_module,
        'BytecodeHar': task.har_module,
        'Hsp': task.hsp_module,
        'Cpp': task.cpp_module
    }

    return module_mapping.get(module, module_mapping['Hap'])


def get_module_path(task, module=''):
    module_mapping = {
        'Hap': task.hap_module_path,
        'Har': task.har_module_path,
        'BytecodeHar': task.har_module_path,
        'Hsp': task.hsp_module_path,
        'Cpp': task.cpp_module_path
    }

    return module_mapping.get(module, module_mapping['Hap'])


def get_output_path_unsigned(task, module=''):
    output_path_mapping = {
        "Hap": task.hap_output_path,
        "Hsp": task.hsp_output_path,
        "Cpp": task.cpp_output_path
    }
    return output_path_mapping.get(module, output_path_mapping['Hap'])


def get_output_path_signed(task, module=''):
    output_path_mapping = {
        "Hap": task.hap_output_path_signed,
        "Hsp": task.hsp_output_path_signed,
        "Cpp": task.cpp_output_path_signed
    }
    return output_path_mapping.get(module, output_path_mapping['Hap'])


def get_output_path_har(task, module=''):
    output_path_mapping = {
        "Har": task.har_output_path_har,
        "BytecodeHar": task.har_output_path_har,
        "Hsp": task.hsp_output_path_har
    }
    return output_path_mapping.get(module, output_path_mapping['Har'])


def get_output_path(task, module, output_type):
    if output_type == options.OutputType.unsigned:
        return get_output_path_unsigned(task, module)
    elif output_type == options.OutputType.signed:
        return get_output_path_signed(task, module)
    else:
        return get_output_path_har(task, module)


def get_cache_extension(task_type):
    cache_extension = ''
    if 'stage' in task_type:
        cache_extension = '.protoBin'
    elif 'fa' in task_type or 'compatible8' in task_type:
        cache_extension = '.temp.abc'
    elif 'js' in task_type:
        cache_extension = '.abc'

    return cache_extension


def file_contains_specified_fields(file_path, fields):
    if not os.path.exists(file_path):
        logging.error(f"File {file_path} doesn't exist")
        return False
    with open(file_path, 'r', encoding='utf-8') as file:
        line = file.readline()
        while line:
            if fields in line:
                return True
            line = file.readline()
    return False
