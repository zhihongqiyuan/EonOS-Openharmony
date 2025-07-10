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

import fs from 'fs';
import { createFilter } from '@rollup/pluginutils';
import { 
  findVisualFile, 
  visualTransform as processVisual 
} from '../../process_visual';
import MagicString from 'magic-string';
import { PluginContext } from 'rollup';
import { projectConfig } from '../../../main';

const filter: any = createFilter(/(?<!\.d)\.ets$/);

export function visualTransform() {
  return {
    name: 'visualTransform',
    transform(code: string, id: string) {
if (!filter(id)) {
        return null;
      }
      if (process.env.watchMode !== 'true' && 'esmodule' === projectConfig.compileMode) {
        return null;
      }
      const logger = this.share.getLogger('visualTransform');
      code = processVisual(code, id, logger);
      const magicString = new MagicString(code);
      return {
        code,
        map: magicString.generateMap({ hires: true })
      };
    },
    shouldInvalidCache(this: PluginContext, options: any): boolean {
      const moduleId: string = options.id;
      if (!filter(moduleId) || !moduleId) {
        return false;
      }
      const visualId: string = findVisualFile(moduleId);
      if (!visualId || !fs.existsSync(visualId)) {
        if (this.cache.has(visualId)) {
          this.cache.delete(visualId);
        }
        return false;
      }
      const stat: fs.Stats = fs.statSync(visualId);
      const currentTimestamp: number = stat.mtime.getTime();
      if (!this.cache.has(visualId)) {
        this.cache.set(visualId, currentTimestamp);
        return true;
      }
      const lastTimestamp: number = this.cache.get(visualId);
      this.cache.set(visualId, currentTimestamp);
      if (currentTimestamp === lastTimestamp) {
        return false;
      }
      return true;
    }
  }
}