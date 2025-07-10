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
export default class SPLogger {
  //控制所有
  private static isPrintLog = true;
  //控制debug
  private static isPrintDebugLog = true;
  //控制info
  private static isPrintInfoLog = true;
  //控制warn
  private static isPrintWarnLog = true;
  //控制error
  private static isPrintErrorLog = true;

  private static bastTag = 'SmartPerf:';

  //debug debug日志
  static DEBUG(tag: string, msg: string): void {
    if (SPLogger.isPrintLog && SPLogger.isPrintDebugLog) {
      console.debug(SPLogger.bastTag + tag + ',' + msg);
    }
  }

  //info 级别日志
  static INFO(tag: string, msg: string): void {
    if (SPLogger.isPrintLog && SPLogger.isPrintInfoLog) {
      console.info(SPLogger.bastTag + tag + ',' + msg);
    }
  }

  //warn 级别日志
  static WARN(tag: string, msg: string): void {
    if (SPLogger.isPrintLog && SPLogger.isPrintWarnLog) {
      console.warn(SPLogger.bastTag + tag + ',' + msg);
    }
  }

  //error 级别日志
  static ERROR(tag: string, msg: string): void {
    if (SPLogger.isPrintLog && SPLogger.isPrintErrorLog) {
      console.error(SPLogger.bastTag + tag + ',' + msg);
    }
  }
}
