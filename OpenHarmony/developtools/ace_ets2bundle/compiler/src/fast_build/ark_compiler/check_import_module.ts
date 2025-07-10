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

import { ModuleSourceFile } from './module/module_source_file';
import { isJsSourceFile } from './utils';
import { toUnixPath } from '../../utils';
import { compilerOptions } from '../../ets_checker';
import {
  EXTNAME_D_ETS,
  EXTNAME_ETS,
  GEN_ABC_PLUGIN_NAME,
  red,
  yellow
} from './common/ark_define';
import {
  CommonLogger,
  LogData,
  LogDataFactory
} from './logger';
import {
  ArkTSErrorDescription,
  ErrorCode
} from './error_code';

export function checkIfJsImportingArkts(rollupObject: Object, moduleSourceFile?: ModuleSourceFile): void {
  if (rollupObject.share.projectConfig.singleFileEmit && moduleSourceFile) {
    checkAndLogArkTsFileImports(rollupObject, moduleSourceFile);
  } else {  
    ModuleSourceFile.getSourceFiles().forEach((sourceFile: ModuleSourceFile) => {
      checkAndLogArkTsFileImports(rollupObject, sourceFile);
    });
  }
}

function checkAndLogArkTsFileImports(rollupObject: Object, sourceFile: ModuleSourceFile): void {
  const id: string = sourceFile.getModuleId();
  const unixId: string = toUnixPath(id);
  const logger: CommonLogger = CommonLogger.getInstance(rollupObject);
  if (isJsSourceFile(id) && unixId.indexOf('/oh_modules/') === -1) {
    const importMap = rollupObject.getModuleInfo(id).importedIdMaps;
    Object.values(importMap).forEach((requestFile: string) => {
      logIfFileEndsWithArkts(requestFile, id, logger);
    }
    );
  }
}

function logIfFileEndsWithArkts(requestFile: string, id: string, logger: CommonLogger): void {
  if (requestFile.endsWith(EXTNAME_ETS) || requestFile.endsWith(EXTNAME_D_ETS)) {
    if (compilerOptions.isCompatibleVersion) {
      const warnMsg: string = `ArkTS:WARN File: ${id}\n` +
        `Importing ArkTS files in JS and TS files is about to be forbidden.\n`;
      logger.warn(yellow + warnMsg);
    } else {
      const errInfo: LogData = LogDataFactory.newInstance(
        ErrorCode.ETS2BUNDLE_EXTERNAL_FORBIDDEN_IMPORT_ARKTS_FILE,
        ArkTSErrorDescription,
        'Importing ArkTS files in JS and TS files is forbidden.',
        '',
        [`Please remove the import statement of the ArkTS file in ${id}.`]
      );
      logger.printError(errInfo);
    }
  }
}