/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

import {FILE_TYPE as BUNDLE_FILE_TYPE, FILE_TYPE_LET as BUNDLE_FILE_TYPE_LET} from "@bundle:myapp/test-export-bundle";
import {FILE_TYPE as PACKAGE_FILE_TYPE, FILE_TYPE_VAR as PACKAGE_FILE_TYPE_VAR} from "@package:myapp/test-export-package";
import {FILE_TYPE as NORMALIZED_FILE_TYPE_1, FILE_TYPE_LET as NORMALIZED_FILE_TYPE_LET_1} from "@normalized:N&modulename&&myapp/test-export-normalized&";
import {FILE_TYPE as NORMALIZED_FILE_TYPE_2, FILE_TYPE_LET as NORMALIZED_FILE_TYPE_LET_2} from "@normalized:N&&&myapp/test-export-normalized&";
import {FILE_TYPE as NORMALIZED_FILE_TYPE_3} from "@normalized:N&&bundlename&myapp/test-export-normalized-bundlename&";
import {FILE_TYPE as NONURL_FILE_TYPE} from "test-export-non-ohmurl";

function testUrlType() {
    if (BUNDLE_FILE_TYPE !== "bundle"
        || PACKAGE_FILE_TYPE !== "package"
        || NORMALIZED_FILE_TYPE_1 !== "normalized"
        || NORMALIZED_FILE_TYPE_2 !== "normalized"
        || NORMALIZED_FILE_TYPE_3 !== "normalized-bundlename") {
        print("bad");
    }
    if (NONURL_FILE_TYPE !== "none") {
        print("good1");
    } else {
        print("good2");
    }
    if (BUNDLE_FILE_TYPE_LET !== "bundle") {
        print("good3");
    }
    if (PACKAGE_FILE_TYPE_VAR !== "package") {
        print("good4");
    }
    if (NORMALIZED_FILE_TYPE_LET_1 !== "normalized") {
        print("good5");
    }
    if (NORMALIZED_FILE_TYPE_LET_2 !== "normalized") {
        print("good6");
    }
}