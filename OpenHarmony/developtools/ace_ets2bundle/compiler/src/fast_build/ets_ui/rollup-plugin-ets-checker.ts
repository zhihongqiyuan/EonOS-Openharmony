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
import { EventEmitter } from 'events';
import * as ts from 'typescript';

import {
  projectConfig,
  globalProgram
} from '../../../main';
import {
  serviceChecker,
  languageService,
  printDiagnostic,
  fastBuildLogger,
  emitBuildInfo,
  runArkTSLinter,
  targetESVersionChanged,
  collectFileToIgnoreDiagnostics,
  TSC_SYSTEM_CODE,
  traverseProgramSourceFiles
} from '../../ets_checker';
import { TS_WATCH_END_MSG } from '../../pre_define';
import {
  setChecker
} from '../../utils';
import {
  configureSyscapInfo,
  configurePermission
} from '../system_api/api_check_utils';
import { MemoryMonitor } from '../meomry_monitor/rollup-plugin-memory-monitor';
import { MemoryDefine } from '../meomry_monitor/memory_define';
import {
  CompileEvent,
  createAndStartEvent,
  stopEvent,
  getHookEventFactory,
  processExternalEvents,
  ExternalEventType
} from '../../performance';
import { LINTER_SUBSYSTEM_CODE } from '../../hvigor_error_code/hvigor_error_info';
import { ErrorCodeModule } from '../../hvigor_error_code/const/error_code_module';

export let tsWatchEmitter: EventEmitter | undefined = undefined;
export let tsWatchEndPromise: Promise<void>;

export function etsChecker() {
  let executedOnce: boolean = false;
  return {
    name: 'etsChecker',
    buildStart() {
      const recordInfo = MemoryMonitor.recordStage(MemoryDefine.ROLLUP_PLUGIN_BUILD_START);
      const hookEventFactory: CompileEvent = getHookEventFactory(this.share, 'etsChecker', 'buildStart');
      const eventServiceChecker = createAndStartEvent(hookEventFactory, 'serviceChecker');
      if (process.env.watchMode === 'true' && process.env.triggerTsWatch === 'true') {
        tsWatchEmitter = new EventEmitter();
        tsWatchEndPromise = new Promise<void>(resolve => {
          tsWatchEmitter.on(TS_WATCH_END_MSG, () => {
            resolve();
          });
        });
      }
      if (this.share.projectConfig.deviceTypes) {
        configureSyscapInfo(this.share.projectConfig);
      }
      if (this.share.projectConfig.permission) {
        configurePermission(this.share.projectConfig);
      }
      if (this.share.projectConfig.integratedHsp) {
        projectConfig.integratedHsp = this.share.projectConfig.integratedHsp;
        projectConfig.resetBundleName = this.share.projectConfig.integratedHsp;
      }
      Object.assign(projectConfig, this.share.projectConfig);
      Object.assign(this.share.projectConfig, {
        compileHar: projectConfig.compileHar,
        compileShared: projectConfig.compileShared,
        moduleRootPath: projectConfig.moduleRootPath,
        buildPath: projectConfig.buildPath,
        isCrossplatform: projectConfig.isCrossplatform,
        requestPermissions: projectConfig.requestPermissions,
        definePermissions: projectConfig.definePermissions,
        syscapIntersectionSet: projectConfig.syscapIntersectionSet,
        syscapUnionSet: projectConfig.syscapUnionSet,
        deviceTypesMessage: projectConfig.deviceTypesMessage,
        compileSdkVersion: projectConfig.compileSdkVersion,
        compatibleSdkVersion: projectConfig.compatibleSdkVersion,
        runtimeOS: projectConfig.runtimeOS,
        ignoreCrossplatformCheck: projectConfig.ignoreCrossplatformCheck
      });
      const logger = this.share.getLogger('etsChecker');
      const rootFileNames: string[] = [];
      const resolveModulePaths: string[] = [];
      rootFileNamesCollect(rootFileNames);
      if (this.share && this.share.projectConfig && this.share.projectConfig.resolveModulePaths &&
        Array.isArray(this.share.projectConfig.resolveModulePaths)) {
        resolveModulePaths.push(...this.share.projectConfig.resolveModulePaths);
      }
      if (process.env.watchMode === 'true') {
        !executedOnce && serviceChecker(rootFileNames, logger, resolveModulePaths, eventServiceChecker, this.share);
        ts.PerformanceDotting.startAdvanced('diagnostic');
        if (executedOnce) {
          const timePrinterInstance = ts.ArkTSLinterTimePrinter.getInstance();
          timePrinterInstance.setArkTSTimePrintSwitch(false);
          const buildProgramRecordInfo = MemoryMonitor.recordStage(MemoryDefine.BUILDER_PROGRAM);
          timePrinterInstance.appendTime(ts.TimePhase.START);
          globalProgram.builderProgram = languageService.getBuilderProgram(/*withLinterProgram*/ true);
          globalProgram.program = globalProgram.builderProgram.getProgram();
          traverseProgramSourceFiles(languageService.getProps());
          timePrinterInstance.appendTime(ts.TimePhase.GET_PROGRAM);
          MemoryMonitor.stopRecordStage(buildProgramRecordInfo);
          const collectFileToIgnore = MemoryMonitor.recordStage(MemoryDefine.COLLECT_FILE_TOIGNORE_RUN_TSLINTER);
          collectFileToIgnoreDiagnostics(rootFileNames);
          runArkTSLinter(getErrorCodeLogger(LINTER_SUBSYSTEM_CODE, this.share));
          MemoryMonitor.stopRecordStage(collectFileToIgnore);
        }
        executedOnce = true;
        const allDiagnostics: ts.Diagnostic[] = globalProgram.builderProgram
          .getSyntacticDiagnostics()
          .concat(globalProgram.builderProgram.getSemanticDiagnostics());
        ts.PerformanceDotting.stopAdvanced('diagnostic');
        emitBuildInfo();
        let errorCodeLogger: Object | undefined = this.share?.getHvigorConsoleLogger ?
          this.share?.getHvigorConsoleLogger(TSC_SYSTEM_CODE) : undefined;

        allDiagnostics.forEach((diagnostic: ts.Diagnostic) => {
          printDiagnostic(diagnostic, ErrorCodeModule.TSC, errorCodeLogger);
        });
        fastBuildLogger.debug(TS_WATCH_END_MSG);
        tsWatchEmitter.emit(TS_WATCH_END_MSG);
      } else {
        serviceChecker(rootFileNames, logger, resolveModulePaths, eventServiceChecker, this.share);
      }
      setChecker();
      MemoryMonitor.stopRecordStage(recordInfo);
      processExternalEvents(projectConfig, ExternalEventType.TSC, {parentEvent: eventServiceChecker, filePath: ''});
      stopEvent(eventServiceChecker);
    },
    shouldInvalidCache(): boolean {
      // The generated js file might be different in some cases when we change the targetESVersion,
      // so we need to regenerate them all when targetESVersion is changed.
      return targetESVersionChanged;
    }
  };
}

function getErrorCodeLogger(code: string, share: Object): Object | undefined {
  return !!share?.getHvigorConsoleLogger ? share?.getHvigorConsoleLogger(code) : undefined;
}

function rootFileNamesCollect(rootFileNames: string[]): void {
  const entryFiles: string[] = projectConfig.widgetCompile ? Object.values(projectConfig.cardEntryObj) : Object.values(projectConfig.entryObj);
  entryFiles.forEach((fileName: string) => {
    rootFileNames.push(path.resolve(fileName));
  });
}
