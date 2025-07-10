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

import type {Node, TransformerFactory} from 'typescript';
import {lstatSync, readdirSync} from 'fs';
import {join, resolve} from 'path';

import type {IOptions} from '../configs/IOptions';
import type {TransformPlugin} from './TransformPlugin';
import { Extension } from '../common/type';

export class TransformerManager {
  private static readonly sLoadPath: string = join(__dirname, '../', 'transformers');

  private readonly mTransformers: TransformerFactory<Node>[];

  public constructor(options: IOptions) {
    this.mTransformers = [];
    this.loadTransformers(options);
  }

  public getTransformers(): TransformerFactory<Node>[] {
    return this.mTransformers;
  }

  private loadTransformers(options: IOptions): void {
    let subFiles: string[] = readdirSync(TransformerManager.sLoadPath);
    let plugins: TransformPlugin[] = [];
    for (const subFile of subFiles) {
      let subPath: string = resolve(TransformerManager.sLoadPath + '/' + subFile);
      let isDir: boolean = lstatSync(subPath)?.isDirectory();
      if (!isDir) {
        continue;
      }

      let subDir: string[] = readdirSync(subPath);
      for (const file of subDir) {
        if (!file.endsWith(Extension.TS)) {
          continue;
        }
        const fileNameNoSuffix = file.lastIndexOf(Extension.DTS) > -1 ? file.slice(0, file.lastIndexOf(Extension.DTS)) :
          file.slice(0, file.lastIndexOf(Extension.TS));
        let path: string = join(subPath, fileNameNoSuffix);
        let module = require(path);
        let plugin: TransformPlugin = module?.transformerPlugin;
        if (plugin) {
          plugins.push(plugin as TransformPlugin);
        }
      }
    }

    plugins.sort((plugin1: TransformPlugin, plugin2: TransformPlugin) => {
      return plugin1.order - plugin2.order;
    });

    plugins.forEach((plugin: TransformPlugin) => {
      let transformerFactory: TransformerFactory<Node> = plugin?.createTransformerFactory(options);
      let name: string = plugin?.name;
      if (transformerFactory && name) {
        this.mTransformers.push(transformerFactory);
      }
    });
  }
}
