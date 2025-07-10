/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use rollupObject file except in compliance with the License.
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

import { ModuleSourceFile } from '../../../../lib/fast_build/ark_compiler/module/module_source_file';
import { GEN_ABC_PLUGIN_NAME } from '../../../../lib/fast_build/ark_compiler/common/ark_define';

class ModuleSourceFileMock extends ModuleSourceFile {
  projectConfig: object;
  logger: object;

  initPluginEnvMock(rollup: object) {
    this.projectConfig = Object.assign(rollup.share.arkProjectConfig, rollup.share.projectConfig);
    this.logger = rollup.share.getLogger(GEN_ABC_PLUGIN_NAME);
  }

  testProcessJsModuleRequest(rollup: object) {
    this.processJsModuleRequest(rollup);
  }

  testProcessTransformedJsModuleRequest(rollup: object) {
    this.processTransformedJsModuleRequest(rollup);
  }
}

export default ModuleSourceFileMock;