/*
 * Copyright (c) 2022-2023 Shenzhen Kaihong Digital Industry Development Co., Ltd.
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
export class XTools {
  constructor() {}
  static MOUSE_POS = {
    x: 0,
    y: 0,
  };
  static InRect(xx, yy, x, y, w, h) {
    if (xx < x) {
      return false;
    }
    if (yy < y) {
      return false;
    }
    if (xx > x + w) {
      return false;
    }
    if (yy > y + h) {
      return false;
    }
    return true;
  }
}
export function fAngle(x, y) {
  return (Math.atan2(-y, x) * 180) / Math.PI;
}
export function iDistance(x, y) {
  return Math.sqrt(x * x + y * y);
}

export var timeMs = 0;
export function freshTime() {
  let t = new Date();
  timeMs = t.getTime();
}
freshTime();
export function TimeMS() {
  let t = new Date();
  return t.getTime();
}
export function RandInt(min = 0, max = 100) {
  return Math.floor(Math.random() * (max - min)) + min;
}

export function GetURL() {
  if ('undefined' !== typeof wx) {
    return 'https://7465-testegg-19e3c9-1301193145.tcb.qcloud.la/';
  } else {
    return '';
  }
}
