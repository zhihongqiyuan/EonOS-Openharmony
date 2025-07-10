#!/usr/bin/env python
#coding=utf-8

#
# Copyright (c) 2022 Huawei Device Co., Ltd.
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

import string
import json
import sys
import os
import struct

# find out/rk3568/packages/phone/system/ -type f -print | file -f - | grep ELF | cut -d":" -f1 | wc -l


class ELFWalker():
    def __init__(self, product_out_path="./demo/archinfo/assets/rk3568/3.2.7.5"):
        self._files = []
        self._links = {}
        self._walked = False
        self._product_out_path = product_out_path

    def get_product_images_path(self):
        return os.path.join(self._product_out_path, "packages/phone/")

    def get_product_out_path(self):
        return self._product_out_path

    def get_link_file_map(self):
        if not self._walked:
            self.__walk_path("packages/phone/system")
            self.__walk_path("packages/phone/vendor")
        return self._links

    def get_elf_files(self):
        if not self._walked:
            self.__walk_path("packages/phone/system")
            self.__walk_path("packages/phone/vendor")
        return self._files

    def __walk_path(self, subdir):
        for root, subdirs, files in os.walk(os.path.join(self._product_out_path, subdir)):
            for _filename in files:
                _asset_file = os.path.join(root, _filename)
                if os.path.islink(_asset_file):
                    if _asset_file.find(".so") > 0:
                        target = os.readlink(_asset_file)
                        self._links[_asset_file] = target
                    continue
                if not os.path.isfile(_asset_file):
                    continue
                with open(_asset_file, "rb") as f:
                    data = f.read(4)
                    try:
                        magic = struct.unpack("Bccc", data)
                        if magic[0] == 0x7F and magic[1] == b'E' and magic[2] == b'L' and magic[3] == b'F':
                            self._files.append(_asset_file)
                    except:
                        pass

        self._walked = True

if __name__ == '__main__':
    elfFiles = ELFWalker()
    for f in elfFiles.get_elf_files():
        print(f)
    for src, target in elfFiles.get_link_file_map().items():
        print('{} -> {}'.format(str, target))
