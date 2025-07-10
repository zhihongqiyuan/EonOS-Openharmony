/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the License);
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

import assert from "assert";
let val1 = 0;
let val2 = 0;
let val3 = 0;
let val4 = 0;
let val5 = 0;
if (val2 + val3 === val4) val5 += 1;
assert(val5 === 1);
if (val2 + val3 === val4) { val5 +=1 };
assert(val5 === 2);
if (val2 + val3 !== val4) val5 += 1; else val5 = 0;
assert(val5 === 0);
if (val2 + val3 === val4) { val5 += 2; } else { }
assert(val5 === 2);

function getDayName(day: number): string {
  let ans: string = ''
  switch (day) {
    case 1:
      val1++;
      return "Monday";
    case 2:
      ans = "Tuesday";
      break;
    case 3:
      return "Wednesday";
    case 4:
      return "Thursday";
    case 5:
      return "Friday";
    case 6:
      return "Saturday";
    case 7:
      return "Sunday";
    default:
      return "Invalid day";
  }
  return ans;
}
assert(getDayName(2) === 'Tuesday')
assert(getDayName(1) === 'Monday')
assert(val1 === 1)
