/**
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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
 * Log Util
 *
 * standard:
 * 1. define TAG, recommend class name.
 * 2. switch IS_DEBUG_ON as true, when debugging.
 * 3. msg should be short and valuable.
 * 4. choose appropriate function.
 * 5. the function execute many times can not print.
 * 6. uniqueness.
 */

import hiLog from '@ohos.hilog';

const DOMAIN: number = 0x0007;
const TAG = 'useriam_auth_widget';
const SYMBOL = ' --> ';

/**
 *  log package tool class
 */
export class LogUtils {
  /**
   * Outputs debug-level logs.
   *
   * @param tag Identifies the log tag.
   * @param format Indicates the log format string.
   */
  debug(tag: string, format: string): void {
    if (LogUtils.isLogGable(hiLog.LogLevel.DEBUG)) {
      hiLog.debug(DOMAIN, TAG, tag + SYMBOL + format);
    }
  }

  /**
   * Outputs info-level logs.
   *
   * @param tag Identifies the log tag.
   * @param format Indicates the log format string.
   */
  info(tag: string, format: string): void {
    if (LogUtils.isLogGable(hiLog.LogLevel.INFO)) {
      hiLog.info(DOMAIN, TAG, tag + SYMBOL + format);
    }
  }

  /**
   * Outputs warning-level logs.
   *
   * @param tag Identifies the log tag.
   * @param format Indicates the log format string.
   */
  warn(tag: string, format: string): void {
    if (LogUtils.isLogGable(hiLog.LogLevel.WARN)) {
      hiLog.warn(DOMAIN, TAG, tag + SYMBOL + format);
    }
  }

  /**
   * Outputs error-level logs.
   *
   * @param tag Identifies the log tag.
   * @param format Indicates the log format string.
   */
  error(tag: string, format: string): void {
    if (LogUtils.isLogGable(hiLog.LogLevel.ERROR)) {
      hiLog.error(DOMAIN, TAG, tag + SYMBOL + format);
    }
  }

  /**
   * Outputs fatal-level logs.
   *
   * @param tag Identifies the log tag.
   * @param format Indicates the log format string.
   */
  fatal(tag: string, format: string): void {
    if (LogUtils.isLogGable(hiLog.LogLevel.FATAL)) {
      hiLog.fatal(DOMAIN, TAG, tag + SYMBOL + format);
    }
  }

  /**
   * Checks whether logs of the specified tag, and level can be printed.
   *
   * @param tag Identifies the log tag.
   * @param level log level
   */
  private static isLogGable(level: hiLog.LogLevel): boolean {
    return hiLog.isLoggable(DOMAIN, TAG, level);
  }
}

let mLogUtil = new LogUtils();

export default mLogUtil as LogUtils;

