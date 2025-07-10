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
import sys
import string

DEBUG_NORMAL = 1
DEBUG_VERBOSE = 2
DEBUG_SPAM = 3

debuglevel = DEBUG_NORMAL


def debug(level, *msg):
    if debuglevel >= level:
        print(' '.join(msg))


# return a list of lines of output of the command
def command(command, *args):
    debug(DEBUG_SPAM, "calling", command, ' '.join(args))
    pipe = os.popen(command + ' ' + ' '.join(args), 'r')
    output = pipe.read().strip()
    status = pipe.close() 
    if status is not None and os.WEXITSTATUS(status) != 0:
        print("Command failed with status", os.WEXITSTATUS(status), ":", \
               command, ' '.join(args))
        print("With output:", output)
        sys.exit(1)
    return [i for i in output.split('\n') if i]
