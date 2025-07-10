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

(function () {
  function f1() {
    f3();
  }
  function f3() {}
  function f5() {}
  function f6() {
    let v31 = 20;
    function f19() {
      loop1: for (var v32 = 0; v32++ < 10;) {
        let v36 = 21;
        let v37 = function () {
          return v36;
        };
        try {} catch (v38) {} finally {
          f5(v31 === 20);
        }
        loop2: for (var v33 = 0; v33++ < 10;) {
          let v38 = 21;
          let v39 = function () {
            return v38;
          };
          try {} catch (v38) {} finally {
            f5(v31 === 20);
            continue loop1;
          }
        }
      }
    }
    f19();
  }
  for (var v1 = 0; v1 < 10; v1++) {
    f6();
  }
})();