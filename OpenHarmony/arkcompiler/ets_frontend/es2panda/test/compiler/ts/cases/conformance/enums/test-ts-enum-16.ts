/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


enum s1 {
    a = "str",
    b = "str" + "str"
}

enum s2 {
    a = "str",
    c = a,
}

enum s2_1 {
    a = s1.a,
    b = a+s2.a,
    c = s2_1.b+"str"
}

enum s3 {
    a = ("str"),
    b = (("str") + ("str"+"str")),
    c = "str" + ("str") + s2_1.b
}

print(s2.c);
print(s2["str"]);

print(s2_1.a);
print(s2_1.b);
print(s2_1.c);
print(s2_1["strstrstr"]);

print(s3.b);
print(s3.c);
print(s3["strstrstr"]);