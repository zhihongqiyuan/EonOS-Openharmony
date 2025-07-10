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
import subprocess


class HdiParser(object):
    @staticmethod
    def load(mgr, product_out_path):
        # Decode hcb file to get hcs file
        hdi_tool = os.path.join(product_out_path, "obj/drivers/hdf_core/framework/tools/hc-gen/hc-gen")
        hcs_file = os.path.join(product_out_path, "packages/phone/vendor/etc/hdfconfig/hdf_default.hcb")
        out_file = os.path.join(product_out_path, "device_info.hcs")
        if os.path.exists(hcs_file) and os.path.exists(hdi_tool):
            subprocess.Popen([hdi_tool, "-d", hcs_file, "-o", out_file]).wait()
        else:
            print("{} or {} not exist".format(hcs_file, hdi_tool))
        try:
            with open(out_file) as f:
                lines = f.readlines()
        except:
            try:
                out_file = os.path.join(product_out_path, "device_info.d.hcs")
                with open(out_file) as f:
                    lines = f.readlines()
            except:
                return

        modules = []
        for line in lines:
            line = line.strip()
            if line.find("moduleName") < 0:
                continue
            parts = line.split("=")
            parts = [p.strip() for p in parts]
            if len(parts) < 2:
                continue
            name = parts[1]
            if name.endswith(";"):
                name = name[:-1]
            name = name.strip('"')
            name = name.strip("'")
            if name == "":
                continue

            if not name.endswith(".so"):
                name = "lib%s.z.so" % name
            modules.append(name)

        if not mgr:
            return

        for elf in mgr.get_all():
            if elf["name"] in modules:
                elf["hdiType"] = "hdi_service"

if __name__ == "__main__":
    parser = HdiParser()
    parser.load(None, "/home/ohos/vendor/hihope/rk3568/hdf_config/uhdf/device_info.hcs")
