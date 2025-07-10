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
import fileio from '@ohos.fileio';
import SPLogger from '../utils/SPLogger';
/**
 * create file by path
 * @param path
 * @param data
 */
const TAG = 'IOUtils';
export function createFilePath(path: string, data: string): void {
  SPLogger.INFO(TAG, 'createFilePath called:' + path);
  let writer;
  try {
    fileio.mkdirSync(
      globalThis.abilityContext.getApplicationContext().filesDir +
        '/' +
        globalThis.dbTime
    );
  } catch (err) {
    SPLogger.INFO(TAG, 'createFilePath:mkdirSync' + err);
  }

  try {
    writer = fileio.openSync(path, 0o102, 0o666);
    fileio.writeSync(writer, data);
    SPLogger.INFO(TAG, 'createFilePath:WRITER SUCCESS');
  } catch (err) {
    SPLogger.INFO(TAG, 'createFilePath:err' + err);
  } finally {
    fileio.closeSync(writer);
  }
}
