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

import ts from 'typescript';
import path from 'path';

import { projectConfig } from '../main';
import {
  resourceFileName,
  isDynamic
} from './process_ui_syntax';
import {
  PAGE_PATH,
  INTEGRATED_HSP,
  TRUE,
  FALSE,
  RESOURCE_NAME_BUNDLE,
  RESOURCE_NAME_MODULE
} from './pre_define';
import constantDefine from './constant_define';

export function routerOrNavPathWrite(context: ts.TransformationContext, keyName: string, projectPath: string,
  projectRootPath: string = ''): ts.PropertyAssignment {
  return context.factory.createPropertyAssignment(
    context.factory.createIdentifier(keyName),
    context.factory.createStringLiteral(
      projectConfig.compileHar ? keyName === PAGE_PATH ? byteCodeHarPagePath(projectRootPath) : '' :
        pathMessage(projectPath))
  );
}

function byteCodeHarPagePath(projectRootPath: string): string {
  return projectConfig.byteCodeHar ? pathMessage(projectRootPath) : constantDefine.HAR_DEFAULT_PAGE_PATH;
}

function pathMessage(projectPathName: string): string {
  return path.relative(projectPathName || '', resourceFileName).replace(/\\/g, '/').replace(/\.ets$/, '');
}

export function integratedHspType(): string {
  return projectConfig.integratedHsp ? TRUE : projectConfig.compileHar ? constantDefine.HAR_DEFAULT_INTEGRATED_HSP_TYPE : FALSE;
}

export function routerModuleType(context: ts.TransformationContext): ts.PropertyAssignment {
  return context.factory.createPropertyAssignment(
    context.factory.createIdentifier(constantDefine.MODULE_TYPE),
    context.factory.createStringLiteral(moduleType())
  );
}

function moduleType(): string {
  if (projectConfig.compileHar) {
    return projectConfig.byteCodeHar ? constantDefine.BYTE_CODE_HAR : constantDefine.CLOSED_SOURCE_HAR;
  } else if (projectConfig.compileShared) {
    return projectConfig.integratedHsp ? INTEGRATED_HSP : constantDefine.SHARED_HSP;
  }
  return constantDefine.FOLLOW_WITH_HAP;
}

export function routerBundleOrModule(context: ts.TransformationContext, type: string): ts.PropertyAssignment {
  const typeKey: string = type === RESOURCE_NAME_BUNDLE ? RESOURCE_NAME_BUNDLE : RESOURCE_NAME_MODULE;
  if (isDynamic()) {
    return context.factory.createPropertyAssignment(
      context.factory.createIdentifier(typeKey),
      context.factory.createIdentifier(type === RESOURCE_NAME_BUNDLE ? (projectConfig.allowEmptyBundleName ? '' : '__BUNDLE_NAME__') : '__MODULE_NAME__')
    );
  }
  return context.factory.createPropertyAssignment(
    context.factory.createIdentifier(typeKey),
    context.factory.createStringLiteral(type === RESOURCE_NAME_BUNDLE ? (projectConfig.allowEmptyBundleName ? '' : (projectConfig.bundleName || '')) :
      (projectConfig.moduleName || ''))
  );
}
