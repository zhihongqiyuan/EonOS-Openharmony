/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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

import * as ts from 'typescript';
import fs from 'fs';
import path from 'path';

export const WHITELIST: string = 'whiteList';
export const extConfig: string = 'externalconfig';

export let WHITELISTNEW: Array<string> = [];

const COMPONENT_PATH = './build-tools/ets-loader/components';

export function readExternalComponents(): object {
    const EXT_COMPONENT_MAP: object = {};
    const componentPaths: string[] | undefined = getExternalComponentPaths();
    if (!componentPaths) {
        return EXT_COMPONENT_MAP;
    }
    for (const componentPath of componentPaths) {
        if(!fs.existsSync(componentPath)){
            continue;
        }
        const files: string[] = fs.readdirSync(componentPath);
        files.forEach(function (item) {
            const fPath: string = path.join(componentPath, item);
            const json: any = require(fPath);
            const stat: any = fs.statSync(fPath);
            if (stat.isFile()) {
                if (json.name) {
                    const compName: string = json.name;
                    delete json.name;
                    EXT_COMPONENT_MAP[compName] = json;
                } else {
                    WHITELISTNEW = Array.from(new Set(json.extWhiteList ? json.extWhiteList : []));
                }
            }
        });
    }
    return EXT_COMPONENT_MAP;
}

export function getExternalComponentPaths(): string[] | undefined {
    const externalComponentsPath: string[] = [];
    if (process.env.externalApiPaths) {
        const externalApiPaths = process.env.externalApiPaths.split(path.delimiter);
        externalApiPaths.forEach(sdkPath => {
            externalComponentsPath.push(path.resolve(sdkPath, COMPONENT_PATH));
        });
        return externalComponentsPath;
    }
    return undefined;
}

export function concatenateEtsOptions(outOptions: ts.CompilerOptions, extOptions: ts.CompilerOptions): void {
    const tempComponents: Array<string> = Array.from(new Set([...outOptions.ets.components,
    ...extOptions.ets.components]));
    const tempExtendComponents: Array<{ name: string; type: string; instance: string; }> =
        Array.from(new Set([...outOptions.ets.extend.components,
        ...extOptions.ets.extend.components]));
    outOptions.ets.components = tempComponents;
    outOptions.ets.extend.components = tempExtendComponents;
}