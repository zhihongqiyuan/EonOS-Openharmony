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

import { GEN_ABC_PLUGIN_NAME } from './common/ark_define';
import {
  ErrorCode,
  SubsystemCode,
  ES2ABC_ERROR_MAPPING
} from './error_code';

export class CommonLogger {
  private static instance: CommonLogger;
  private logger: Object;
  private hvigorConsoleLoggerMap: { [key in SubsystemCode]?: Object };
  throwArkTsCompilerError: Object;

  private constructor(rollupObject: Object) {
    this.logger = rollupObject.share.getLogger(GEN_ABC_PLUGIN_NAME);
    this.throwArkTsCompilerError = rollupObject.share.throwArkTsCompilerError;
    this.hvigorConsoleLoggerMap = this.initializeHvigorConsoleLoggers(rollupObject);
  }

  private initializeHvigorConsoleLoggers(rollupObject: Object): { [key in SubsystemCode]?: Object } {
    const loggerMap: { [key in SubsystemCode]?: Object } = {};
    if (typeof rollupObject.share.getHvigorConsoleLogger === 'function') {
      loggerMap[SubsystemCode.ETS2BUNDLE] = rollupObject.share.getHvigorConsoleLogger(SubsystemCode.ETS2BUNDLE);
      loggerMap[SubsystemCode.ABC2PROGRAM] = rollupObject.share.getHvigorConsoleLogger(SubsystemCode.ABC2PROGRAM);
      loggerMap[SubsystemCode.ES2ABC] = rollupObject.share.getHvigorConsoleLogger(SubsystemCode.ES2ABC);
    }
    return loggerMap;
  }

  static getInstance(rollupObject: Object): CommonLogger {
    if (!CommonLogger.instance) {
      CommonLogger.instance = new CommonLogger(rollupObject);
    }
    return CommonLogger.instance;
  }

  static destroyInstance(): void {
    CommonLogger.instance = undefined;
  }

  info(...args: string[]): void {
    this.logger.info(...args);
  }

  debug(...args: string[]): void {
    this.logger.debug(...args);
  }

  warn(...args: string[]): void {
    this.logger.warn(...args);
  }

  error(...args: string[]): void {
    this.logger.error(...args);
  }

  printError(error: LogData | string): void {
    if (typeof error === 'string') {
      this.logger.error(error);
      return;
    }
    const hvigorConsoleLogger = this.getLoggerFromErrorCode(error.code);
    if (hvigorConsoleLogger) {
      hvigorConsoleLogger.printError(error);
    } else {
      this.logger.error(error.toString());
    }
  }

  printErrorAndExit(error: LogData | string): void {
    if (typeof error === 'string') {
      this.throwArkTsCompilerError(error);
      return;
    }
    const hvigorConsoleLogger = this.getLoggerFromErrorCode(error.code);
    if (hvigorConsoleLogger) {
      hvigorConsoleLogger.printErrorAndExit(error);
    } else {
      this.throwArkTsCompilerError(error.toString());
    }
  }

  private isValidErrorCode(errorCode: string): boolean {
    return /^\d{8}$/.test(errorCode);
  }

  private getLoggerFromErrorCode(errorCode: string): Object | undefined {
    if (!this.isValidErrorCode(errorCode)) {
      return undefined;
    }
    const subsystemCode = errorCode.slice(0, 3);
    return this.hvigorConsoleLoggerMap[subsystemCode];
  }
}

export class LogDataFactory {

  static newInstance(
    code: ErrorCode,
    description: string,
    cause: string = '',
    position: string = '',
    solutions: string[] = []
  ): LogData {
    const data: LogData = new LogData(code, description, cause, position, solutions);
    return data;
  }

  /**
 * Parses an es2abc error string and returns a LogData instance.
 * @param error The es2abc error string to parse.
 * @returns A LogData instance or undefined if no matching error is found.
 */
  static newInstanceFromEs2AbcError(error: string): LogData | undefined {
    for (const prefix in ES2ABC_ERROR_MAPPING) {
      if (error.startsWith(prefix)) {
        const { code, description, solutions } = ES2ABC_ERROR_MAPPING[prefix];
        const cause = error.replace(prefix, '');
        return LogDataFactory.newInstance(code, description, cause, '', solutions);
      }
    }
    return undefined;
  }

  static newInstanceFromBytecodeObfuscation(errorOutput: string, statusCode: number): LogData | undefined {
    const trimmedOutput = errorOutput?.trim();
    if (!trimmedOutput) {
      return LogDataFactory.newInstance(
        ErrorCode.BYTECODE_OBFUSCATION_COMMON_ERROR,
        'Bytecode program terminated abnormally',
        `Status code: ${statusCode}`
      );
    }
  
    const parseErrorLines = (output: string): Record<string, string> => 
      output
        .split('\n')
        .reduce((acc: Record<string, string>, line) => {
          const [key, ...values] = line.split(':').map(part => part.trim());
          return key && values.length ? { ...acc, [key]: values.join(':').trim() } : acc;
        }, {});
  
    const parsedErrors = parseErrorLines(trimmedOutput);
  
    const getErrorInfo = (): {
      code: ErrorCode;
      description: string;
      cause: string;
      position: string;
      solutions: string[];
      details: string;
    } => {
      if (Object.keys(parsedErrors).length === 0) {
        return {
          code: ErrorCode.BYTECODE_OBFUSCATION_COMMON_ERROR,
          description: trimmedOutput,
          cause: '',
          position: '',
          solutions: [],
          details: `Status code: ${statusCode}`,
        };
      }
  
      return {
        code: parsedErrors['[ErrorCode]'] ?? ErrorCode.BYTECODE_OBFUSCATION_COMMON_ERROR,
        description: parsedErrors['[Description]'] ?? parsedErrors['[Cause]'] ?? 'Unknown error',
        cause: parsedErrors['[Cause]'] ?? '',
        position: parsedErrors['[Position]'] ?? '',
        solutions: parsedErrors['[Solutions]']?.split(',').filter(Boolean) ?? [],
        details: `Status code: ${statusCode}`,
      };
    };
  
    const { code, description, cause, position, solutions, details } = getErrorInfo();
    return LogDataFactory.newInstance(
      code,
      description,
      details ?? cause,
      position,
      solutions
    );
  }
}

export class LogData {

  code: string;
  description: string;
  cause: string;
  position: string;
  solutions: string[];

  constructor(
    code: ErrorCode,
    description: string,
    cause: string = '',
    position: string = '',
    solutions: string[] = []
  ) {
    this.code = code;
    this.description = description;
    this.cause = cause;
    this.position = position;
    this.solutions = solutions;
  }

  toString(): string {
    let errorString = `ERROR Code: ${this.code} ${this.description}\n`;

    if (this.cause || this.position) {
      errorString += `Error Message: ${this.cause}`;
      if (this.position) {
        errorString += ` ${this.position}`;
      }
      errorString += '\n\n';
    }

    if (this.solutions.length > 0 && this.solutions[0] !== '') {
      errorString += `* Try the following: \n${this.solutions.map(str => `  > ${str}`).join('\n')}\n`;
    }

    return errorString;
  }
}