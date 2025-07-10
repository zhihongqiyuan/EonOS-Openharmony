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

import { ESMODULE } from './common/ark_define';
import { ModuleBuildMode } from './module/module_build_mode';
import { ModuleColdreloadMode } from './module/module_coldreload_mode';
import { ModuleHotfixMode } from './module/module_hotfix_mode';
import { ModuleHotreloadMode } from './module/module_hotreload_mode';
import { ModulePreviewMode } from './module/module_preview_mode';
import { ModuleSourceFile } from './module/module_source_file';
import type { ModuleMode } from './module/module_mode';
import { SourceMapGenerator } from './generate_sourcemap';
import {
  CompileEvent,
  getHookEventFactory,
  createAndStartEvent,
  stopEvent
} from '../../performance';

let moduleMode: ModuleMode = null;

export async function generateModuleAbc(error) {
  const hookEventFactory: CompileEvent | undefined = getHookEventFactory(this.share, 'genAbc', 'buildEnd');
  if (error) {
    // When error thrown in previous plugins, rollup will catch and call buildEnd plugin.
    // Stop generate abc if error exists
    return;
  }
  if (this.share.projectConfig.compileMode === ESMODULE) {
    if (this.share.projectConfig.singleFileEmit) {
      if (ModuleSourceFile.needProcessMock) {
        ModuleSourceFile.generateMockConfigFile(this);
      }
    } else {
      await ModuleSourceFile.processModuleSourceFiles(this, hookEventFactory);
    }
    if (this.share.projectConfig.compileHar) {
      if (!this.share.projectConfig.byteCodeHar) {
        // compileHar: compile har of project, which convert .ets to .d.ts and js
        SourceMapGenerator.getInstance().buildModuleSourceMapInfo(hookEventFactory);
        return;
      }
    }
    generateAbc(this, hookEventFactory);
  }
}

function generateAbc(rollupObject: Object, parentEvent: CompileEvent | undefined): void {
  const eventGenerateAbc = createAndStartEvent(parentEvent, 'generate abc');
  if (rollupObject.share.projectConfig.watchMode !== 'true') {
    if (rollupObject.share.arkProjectConfig.coldReload) {
      const moduleColdreloadMode: ModuleColdreloadMode = new ModuleColdreloadMode(rollupObject);
      moduleColdreloadMode.generateAbc(rollupObject, eventGenerateAbc);
      moduleMode = moduleColdreloadMode;
    } else if (rollupObject.share.arkProjectConfig.hotReload) {
      // If file changes are monitored by the IDE, rollup is not started in watch mode, 
      // so rollupObject.share.projectConfig.watchMode is not true. Hotreload in this mode 
      // supports scenarios where entry depends on HAR and HSP.
      const moduleHotreloadMode: ModuleHotreloadMode = new ModuleHotreloadMode(rollupObject);
      moduleHotreloadMode.generateAbc(rollupObject, eventGenerateAbc);
      moduleMode = moduleHotreloadMode;
    } else {
      const moduleBuildMode: ModuleBuildMode = new ModuleBuildMode(rollupObject);
      moduleBuildMode.generateAbc(rollupObject, eventGenerateAbc);
      moduleMode = moduleBuildMode;
    }
  } else if (rollupObject.share.arkProjectConfig.hotReload) {
    // If file changes are monitored by rollup, rollup must be started in watch mode, 
    // so rollupObject.share.projectConfig.watchMode needs to be true. Hotreload in this mode
    // does not support scenarios where entry depends on HSP.
    const moduleHotreloadMode: ModuleHotreloadMode = new ModuleHotreloadMode(rollupObject);
    moduleHotreloadMode.generateAbc(rollupObject, eventGenerateAbc);
    moduleMode = moduleHotreloadMode;
  } else if (rollupObject.share.arkProjectConfig.hotFix) {
    const moduleHotfixMode: ModuleHotfixMode = new ModuleHotfixMode(rollupObject);
    moduleHotfixMode.generateAbc(rollupObject, eventGenerateAbc);
    moduleMode = moduleHotfixMode;
  } else {
    const modulePreviewMode: ModulePreviewMode = new ModulePreviewMode(rollupObject);
    modulePreviewMode.generateAbc(rollupObject, eventGenerateAbc);
    moduleMode = modulePreviewMode;
  }
  stopEvent(eventGenerateAbc);
}

export function cleanModuleMode(): void {
  if (moduleMode) {
    moduleMode.triggerAsync = null;
    moduleMode.triggerEndSignal = null;
    moduleMode = null;
  }
}
