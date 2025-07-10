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

abstract class SettingType_2 {
  private constructor() {}
  static get INT(): SettingType_2 & {
      get name_get_prop_2(): "INT";
      get orignal_get_prop_2(): 0;
  }{
      return new Int_2;
  }
  static get DOUBLE(): SettingType_2 & {
      get name_get_prop_2(): "DOUBLE";
      get orignal_get_prop_2(): 1;
  }{
      return new DOUBLE_2;
  }
}

class Int_2 {
  get name_get_prop_2(): "INT" {
      return "INT"
  }
  get orignal_get_prop_2():0 {
      return 0
  }
}

class DOUBLE_2 {
  get name_get_prop_2(): "DOUBLE" {
      return "DOUBLE"
  }
  get orignal_get_prop_2():1 {
      return 1
  }
}