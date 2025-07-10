#!/usr/bin/env python
# -*- coding: utf-8 -*-
#   Copyright (c) 2025 Huawei Device Co., Ltd.
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.
#
import pytest
import re
import os


class TestCoverageStat:
    def setup(self):
        print("test_coverage_stat setup")

    @pytest.mark.L0
    def test_coverage_stat(self):
        rootcmd = 'hdc shell hiperf stat -d 5 -e hw-cpu-cycles,hw-instructions,hw-stalled-cycles-frontend,hw-stalled-cycles-backend -a --per-core'
        output = os.popen(rootcmd).read()
        matches = re.findall(r'\d+(?=%)', output)
        assert len(matches) != 0
        for match in matches:
            percent = int(match.strip('%'))
            assert percent <= 100
