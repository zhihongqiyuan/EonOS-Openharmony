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

import os
from stat import ST_SIZE

from .utils import command


class ElfFile(dict):
    def __init__(self, file, prefix):
        self._f = file
        self._f_safe = "'%s'" % file

        self["name"] = os.path.basename(file)
        self["size"] = os.stat(self._f)[ST_SIZE]
        if self["name"].find(".so") > 0:
            self["type"] = "lib"
        else:
            self["type"] = "bin"
        self["path"] = file[len(prefix):]

    def __eq__(self, other):
        if not isinstance(other, ElfFile):
            return NotImplemented

        return self["path"] == other["path"]

    def is_library(self):
        if self["name"].find(".so") > 0:
            return True
        return False

    def get_file(self):
        return self._f

    # Return a set of libraries the passed objects depend on.
    def library_depends(self):
        if not os.access(self._f, os.F_OK):
            raise Exception("Cannot find lib: " + self._f)
        dynamics = command("readelf", "--dynamic", self._f_safe)
        res = []
        for line in dynamics:
            pos = line.find("(NEEDED)")
            if pos <= 0:
                continue
            line = line[pos + 8:]
            line = line.strip()
            if not line.startswith("Shared library:"):
                continue
            line = line[15:]
            line = line.strip()
            if line.startswith("["):
                line = line[1:]
            if line.endswith("]"):
                line = line[:-1]
            line = line.strip()
            res.append(line)
        return res

    def __extract_soname(self):
        soname_data = command("mklibs-readelf", "--print-soname", self._f_safe)
        if soname_data:
            return soname_data.pop()
        return ""

    def __extract_elf_size(self):
        size_data = command("size", self._f_safe)
        if not size_data or len(size_data) < 2:
            self["text_size"] = 0
            self["data_size"] = 0
            self["bss_size"] = 0
            return 0

        vals = size_data[1].split()
        self["text_size"] = int(vals[0])
        self["data_size"] = int(vals[1])
        self["bss_size"] = int(vals[2])
        return ""

if __name__ == '__main__':
    import elf_walker

    cnt = 0
    elfFiles = elf_walker.ELFWalker()
    for f in elfFiles.get_elf_files():
        if f.find("libskia_ohos.z.so") < 0:
            continue
        elf = ElfFile(f, elfFiles.get_product_images_path())
        print(f)
