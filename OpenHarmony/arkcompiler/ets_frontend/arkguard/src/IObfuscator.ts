/*
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

export interface IObfuscator {
  /**
   * interface for obfuscating source code
   *
   * @param code string sequence of source code
   */
  obfuscate(code: string): string;

  /**
   * interface for obfuscating source file
   *
   * @param file path of single source file.
   */
  obfuscateFile(file: string): string;

  /**
   * interface for obfuscating source files;
   *
   * @param files a list of source files
   */
  obfuscateFiles(files: string[]): string[];
}
