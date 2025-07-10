/*
 * Copyright (C) 2024 Huawei Device Co., Ltd.
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
export let LogLevel;
(function (LogLevel) {
	LogLevel.OFF = Number.MAX_VALUE;
	LogLevel.ERROR = 4000;
	LogLevel.WARN = 3000;
	LogLevel.INFO = 2000;
	LogLevel.DEBUG = 1000;
	LogLevel.TRACE = 500;
	LogLevel.ALL = Number.MIN_VALUE;
  })(LogLevel || (LogLevel = {}));
export const error = (message, ...optionalParams) => {
  ConsoleLog.logger(LogLevel.ERROR, message, ...optionalParams);
};
export const warn = (message, ...optionalParams) => {
  ConsoleLog.logger(LogLevel.WARN, message, ...optionalParams);
};
export const info = (message, ...optionalParams) => {
  ConsoleLog.logger(LogLevel.INFO, message, ...optionalParams);
};
export const debug = (message, ...optionalParams) => {
  ConsoleLog.logger(LogLevel.DEBUG, message, ...optionalParams);
};
export const trace = (message, ...optionalParams) => {
  ConsoleLog.logger(LogLevel.TRACE, message, ...optionalParams);
};
export const log = (message) => {
  ConsoleLog.logger(LogLevel.TRACE, message);
};

class ConsoleLog {
  static getNowLogLevel() {
    return this.nowLogLevel;
  }

  static setLogLevel(logLevel) {
    ConsoleLog.nowLogLevel = logLevel;
  }

  static now() {
    const now = new Date();
    const timeString = `${now.getHours()}:${now.getMinutes()}:${now.getSeconds()}:${now.getMilliseconds()}`;
    return timeString;
  }

  static logger(logLevel, message, ...optionalParams) {
    if (logLevel >= ConsoleLog.nowLogLevel) {
      switch (logLevel) {
        case LogLevel.ERROR:
          console.error(message, ...optionalParams, this.now());
          break;
        case LogLevel.WARN:
          console.warn(message, ...optionalParams, this.now());
          break;
        case LogLevel.INFO:
          console.info(message, ...optionalParams, this.now());
          break;
        case LogLevel.DEBUG:
          console.debug(message, ...optionalParams, this.now());
          break;
        case LogLevel.TRACE:
          console.trace(message, ...optionalParams, this.now());
          break;
        default:
          console.log(message, this.now());
      }
    }
  }
}

ConsoleLog.nowLogLevel = LogLevel.OFF;
