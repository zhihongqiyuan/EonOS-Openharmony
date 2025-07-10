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

// Do not obfuscate as enum member when enum members are reserved
namespace ns {
  var obj1 = {prop: 1};
  var obj2 = {prop: 1};
  var obj3 = {prop: 1};
  var obj4 = {prop: 1};
  var obj5 = {prop: 1};

  enum testEnum {
    a = obj1.prop,
    b = obj2.prop,
    c = obj3.prop,
    d = obj4.prop,
    e = obj5.prop
  }
}