/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
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

export default class ArrayUtils {
  static copyOfRange(arr: number[], fromIndex: number, toIndex: number): number[] {
    //需将copya的类型指定为any，这样才能够使其能够任意添加属性
    var copya: number[];
    //最关键的是注意 下面 当a中有方法时，json.parse(json.stringify(xx))是无效的
    for (var i = 0; i < arr.length; i++) {
      if (typeof arr[i] == 'function') {
        return [];
      } else {
        copya[i] = JSON.parse(JSON.stringify(arr[i]));
      }
    }
    return copya;
  }
}
