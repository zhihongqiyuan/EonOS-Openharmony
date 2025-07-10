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

enum D {
  D1,
  D2,
  D3
}

namespace ns {
  var ns1 = {a: 1};
  enum E {
    E1 = ns1.a,
    E2 = 2,
    E3
  }
  let ns2 = 6;
  enum F {
    F1,
    F2,
    F3 = ns2
  }
}