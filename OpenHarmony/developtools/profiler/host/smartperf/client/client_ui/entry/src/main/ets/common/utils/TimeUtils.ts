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

/**
 * 日期格式化
 * @param Number time 时间戳
 * @param String format 格式
 */
export function dateFormat(time?: number, format: string = 'Y-m-d h:i:s'): string {
  const t = new Date(time);
  // 日期格式
  format = format || 'Y-m-d h:i:s';
  let year = t.getFullYear();
  // 由于 getMonth 返回值会比正常月份小 1
  let month = t.getMonth() + 1;
  let day = t.getDate();
  let hours = t.getHours();
  let minutes = t.getMinutes();
  let seconds = t.getSeconds();

  const hash = {
    y: year,
    m: month,
    d: day,
    h: hours,
    i: minutes,
    s: seconds,
  };
  // 是否补 0
  const isAddZero = (o): boolean => {
    return /m|d|h|i|s/.test(o);
  };
  return format.replace(/\w/g, (o) => {
    let rt = hash[o.toLocaleLowerCase()];
    return rt > 9 || !isAddZero(o) ? rt : `0${rt}`;
  });
}
/**
 * 计时器 转 时分秒字符串 HH:mm:ss
 * @param time
 */
export function secToTime(time: number): String {
  let timeStr: String = null;
  let hour: number = 0;
  let minute: number = 0;
  let second: number = 0;
  if (time <= 0) {
    return '00:00';
  } else {
    minute = parseInt((time / 60).toString());
    if (minute < 60) {
      second = time % 60;
      timeStr = unitFormat(minute) + ':' + unitFormat(second);
    } else {
      hour = parseInt((minute / 60).toString());
      minute = minute % 60;
      second = time - hour * 3600 - minute * 60;
      timeStr =
        unitFormat(hour) + ':' + unitFormat(minute) + ':' + unitFormat(second);
    }
  }
  return timeStr;
}

export function unitFormat(i: number): String {
  let retStr: String;
  if (i >= 0 && i < 10) {
    retStr = '0' + i.toString();
  } else {
    retStr = '' + i.toString();
  }
  return retStr;
}
