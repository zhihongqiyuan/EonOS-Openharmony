/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

let a = {
  a: {
    get(a) {
      print(a);
      return {
        c() {print(1)},
        d: 3
      };
    },
  },
  b: 2
};

(a.a?.get(a.b)?.['c'] as D)?.();

((a.a?.get(a.b)?.['c'] as D)!<C>)!<E>?.();

let b = a.a?.get(a.b)?.['c'] as D;
b();

try {
  (a.a?.set(a.b)?.['c'] as D)?.();
} catch (e) {
  print(e.name);
}

print(a.a?.get(a.b)?.d);
