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

import * as BUNDLE from "@bundle:myapp/test-export-bundle";
import * as PACKAGE from "@package:myapp/test-export-package";
import * as NORMALIZED_1 from "@normalized:N&modulename&&myapp/test-export-normalized&";
import * as NORMALIZED_2 from "@normalized:N&&&myapp/test-export-normalized&";
import * as NORMALIZED_LEXVAR from "@normalized:N&&&myapp/test-export-normalized&";
import * as NONURL from "test-import";

if (BUNDLE.FILE_TYPE !== "bundle"
    || PACKAGE.FILE_TYPE !== "package"
    || NORMALIZED_1.FILE_TYPE !== "normalized"
    || NORMALIZED_2.FILE_TYPE !== "normalized") {
    print("bad");
}
if (NONURL.FILE_TYPE !== "none") {
    print("good1");
} else {
    print("good2");
}
if (BUNDLE.FILE_TYPE_LET !== "bundle") {
    print("good3");
}
if (PACKAGE.FILE_TYPE_VAR !== "package") {
    print("good4");
}
if (NORMALIZED_1.FILE_TYPE_LET !== "normalized") {
    print("good5");
}
if (NORMALIZED_2.FILE_TYPE_LET !== "normalized") {
    print("good6");
}
if (NORMALIZED_LEXVAR.FILE_TYPE !== "normalized") {
    print("good7");
}
function namespaceAsLexVar() {
    if (NORMALIZED_LEXVAR.FILE_TYPE !== "normalized") {
        print("good8");
    }
}