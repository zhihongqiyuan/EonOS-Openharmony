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

import path from 'path';

process.env.compileTool = 'rollup';

const projectConfig: Object = {};

projectConfig.aceModuleJsonPath = path.resolve(__dirname, '../../../test/ark_compiler_ut/config/module.json');
projectConfig.modulePathMap = {
  "entry": "/testProjectRootPath/entry",
  "library": "/testProjectRootPath/library"
};
projectConfig.projectRootPath = "/testProjectRootPath";
projectConfig.packageDir = 'oh_modules';
projectConfig.harNameOhmMap = {
  "@ohos/sharedLibrary": "@bundle:UtTestApplication/sharedLibrary/ets/index",
  "@ohos/library": "@bundle:UtTestApplication/library/ets/index"
};
projectConfig.hspNameOhmMap = {};
projectConfig.cachePath = '../../test/ark_compiler_ut/cache/';
projectConfig.pkgContextInfo = {
  "@ohos/library": {
    "packageName": "@ohos/library",
    "bundleName": "",
    "moduleName": "",
    "version": "1.0.0",
    "entryPath": "Index",
    "isSO": false,
    "dependencyAlias": ""
  }
};
projectConfig.dependencyAliasMap = new Map([
  ['library', '@bundle:UtTestApplication/sharedLibrary/ets/index']
]);

export default projectConfig;