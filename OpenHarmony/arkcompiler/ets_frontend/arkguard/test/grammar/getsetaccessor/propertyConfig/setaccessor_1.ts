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

abstract class SettingType {
  private constructor() {}
  static get INT(): SettingType & {
      set name_set_prop_2(para: number);
      set orignal_set_prop_2(para: number);
  }{
      return new Int;
  }
  static get DOUBLE(): SettingType & {
      name_set_prop_2(): "DOUBLE";
      orignal_set_prop_2(): 1;
  } {
      return new DOUBLE;
  }
}

class Int {
  set name_set_prop_2(para1: number) {
  }
  set orignal_set_prop_2(para2: number) {
    return;
  }
}

class DOUBLE {
  name_set_prop_2(): "DOUBLE" {
      return "DOUBLE"
  }
  orignal_set_prop_2():1 {
      return 1
  }
}