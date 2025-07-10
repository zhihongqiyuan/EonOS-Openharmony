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


function f(target, propertyKey) {
  print("test-ts-decorators-8");
  print(propertyKey);
}

class C {
  @f
  static [1] : string = "123";
  /*
   *  TODO(xucheng): test computer property when class support it
   *  @f
   *  [2] : string = "123";
   */
}

print(C[1]);
