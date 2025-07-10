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

import * as fs from 'fs';
import * as path from 'path';
import { logger } from './compile_info';
const { projectConfig } = require('../main');

const red: string = '\u001b[31m';
const reset: string = '\u001b[39m';

const REG_OHM_URL: RegExp = /^@bundle:(\S+)\/(\S+)\/(ets|js)\/(\S+)$/;

export class OHMResolverPlugin {
  private source: any;
  private target: any;

  constructor(source = 'resolve', target = 'resolve') {
    this.source = source;
    this.target = target;
  }

  apply(resolver) {
    const target: any = resolver.ensureHook(this.target);
    resolver.getHook(this.source).tapAsync('OHMResolverPlugin', (request, resolveContext, callback) => {
      if (isOhmUrl(request.request)) {
        const resolvedSourceFile: string = resolveSourceFile(request.request);
        const obj = Object.assign({}, request, {
          request: resolvedSourceFile
        });
        return resolver.doResolve(target, obj, null, resolveContext, callback);
      }
      callback();
      return undefined;
    });
  }
}

export function isOhmUrl(moduleRequest: string): boolean {
  return !!/^@(\S+):/.test(moduleRequest);
}

function addExtension(file: string, srcPath: string): string {
  let extension: string = '.d.ts';
  if (fs.existsSync(file + '.ets') && fs.statSync(file + '.ets').isFile()) {
    extension = '.ets';
  }
  if (fs.existsSync(file + '.ts') && fs.statSync(file + '.ts').isFile()) {
    if (extension !== '.d.ts') {
      logger.error(red, `ArkTS:ERROR Failed to compile with files with same name ${srcPath} in the same directory`, reset);
    }
    extension = '.ts';
  }
  if (fs.existsSync(file + '.js') && fs.statSync(file + '.js').isFile()) {
    if (extension !== '.d.ts') {
      logger.error(red, `ArkTS:ERROR Failed to compile with files with same name ${srcPath} in the same directory`, reset);
    }
    extension = '.js';
  }
  return file + extension;
}

export function resolveSourceFile(ohmUrl: string): string {
  if (!projectConfig.aceBuildJson) {
    logger.error(red, `ArkTS:ERROR Failed to resolve OhmUrl because of aceBuildJson not existing `, reset);
    return ohmUrl;
  }

  const result: RegExpMatchArray = ohmUrl.match(REG_OHM_URL);
  const moduleName: string = result[2];
  const srcKind: string = result[3];

  const modulePath: string = projectConfig.modulePathMap[moduleName];
  const srcRoot: string = projectConfig.isOhosTest ? 'src/ohosTest' : 'src/main';
  let file: string = path.join(modulePath, srcRoot, srcKind, result[4]);
  file = addExtension(file, result[4]);

  if (!fs.existsSync(file) || !fs.statSync(file).isFile()) {
    if (projectConfig.isOhosTest) {
      file = path.join(modulePath, 'src/main', srcKind, result[4]);
      file = addExtension(file, result[4]);

      if (fs.existsSync(file) && fs.statSync(file).isFile()) {
        return file;
      }
    }

    logger.error(red, `ArkTS:ERROR Failed to resolve existed file by this ohm url ${ohmUrl} `, reset);
  }

  return file;
}
