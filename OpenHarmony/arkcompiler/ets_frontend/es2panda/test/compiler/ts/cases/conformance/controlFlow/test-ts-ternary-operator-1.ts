/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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


var a = 3, b = 2, c = 1;
const res1 = a < b ? (a < c ? a : c) : (b < c ? b : c);
print(res1);

const res2 = (a > b) ? ((a << 16)) : ((b << 16));
print(res2);

const res3 = (a < b) ? (1) : (b << 2);
print(res3);

const res4 = (a > b) ? (1 << 2) : (b << 2);
print(res4);

const res5 = (a > b) ? ((a << 16) | b) : ((b << 16) | a);
print(res5);
