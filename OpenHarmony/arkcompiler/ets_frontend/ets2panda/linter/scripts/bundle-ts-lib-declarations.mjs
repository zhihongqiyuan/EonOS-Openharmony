/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

import * as fs from 'node:fs'
import * as path from 'node:path'
import { exit } from 'node:process'

const STANDARD_LIBRARIES = [
    'lib.d.ts',
    'lib.dom.iterable.d.ts',
    'lib.dom.d.ts',
    'lib.es2015.symbol.wellknown.d.ts',
    'lib.es2015.symbol.d.ts',
    'lib.es2015.reflect.d.ts',
    'lib.es2015.proxy.d.ts',
    'lib.es2015.promise.d.ts',
    'lib.es2015.iterable.d.ts',
    'lib.es2015.generator.d.ts',
    'lib.es2015.d.ts',
    'lib.es2015.core.d.ts',
    'lib.es2015.collection.d.ts',
    'lib.es2016.full.d.ts',
    'lib.es2016.d.ts',
    'lib.es2016.array.include.d.ts',
    'lib.es2017.typedarrays.d.ts',
    'lib.es2017.string.d.ts',
    'lib.es2017.sharedmemory.d.ts',
    'lib.es2017.object.d.ts',
    'lib.es2017.intl.d.ts',
    'lib.es2017.full.d.ts',
    'lib.es2017.d.ts',
    'lib.es2018.regexp.d.ts',
    'lib.es2018.promise.d.ts',
    'lib.es2018.intl.d.ts',
    'lib.es2018.full.d.ts',
    'lib.es2018.d.ts',
    'lib.es2018.asynciterable.d.ts',
    'lib.es2018.asyncgenerator.d.ts',
    'lib.es2019.symbol.d.ts',
    'lib.es2019.string.d.ts',
    'lib.es2019.object.d.ts',
    'lib.es2019.intl.d.ts',
    'lib.es2019.full.d.ts',
    'lib.es2019.d.ts',
    'lib.es2019.array.d.ts',
    'lib.es2020.symbol.wellknown.d.ts',
    'lib.es2020.string.d.ts',
    'lib.es2020.sharedmemory.d.ts',
    'lib.es2020.promise.d.ts',
    'lib.es2020.number.d.ts',
    'lib.es2020.intl.d.ts',
    'lib.es2020.full.d.ts',
    'lib.es2020.date.d.ts',
    'lib.es2020.d.ts',
    'lib.es2020.bigint.d.ts',
    'lib.es2021.weakref.d.ts',
    'lib.es2021.string.d.ts',
    'lib.es2021.promise.d.ts',
    'lib.es2021.intl.d.ts',
    'lib.es2021.full.d.ts',
    'lib.es2021.d.ts',
    'lib.es2022.string.d.ts',
    'lib.es2022.sharedmemory.d.ts',
    'lib.es2022.object.d.ts',
    'lib.es2022.intl.d.ts',
    'lib.es2022.full.d.ts',
    'lib.es2022.error.d.ts',
    'lib.es2022.d.ts',
    'lib.es2022.array.d.ts',
    'lib.es6.d.ts',
    'lib.es5.d.ts',
    'lib.esnext.weakref.d.ts',
    'lib.esnext.string.d.ts',
    'lib.esnext.promise.d.ts',
    'lib.esnext.intl.d.ts',
    'lib.esnext.full.d.ts',
    'lib.esnext.d.ts',
    'lib.scripthost.d.ts',
    'lib.webworker.iterable.d.ts',
    'lib.webworker.importscripts.d.ts',
    'lib.webworker.d.ts'
];

function reportErrorAndExit(msg) {
    console.log(msg);
    exit(1);
}

function copyTypescriptLibDeclarationsToDist() {
    const typescript_lib = path.join('node_modules', 'typescript', 'lib');
    const dist = path.join('dist');

    if (!fs.existsSync(typescript_lib)) {
        reportErrorAndExit('Failed to locate ' + typescript_lib + ' directory');
    }
    if (!fs.existsSync(dist)) {
        reportErrorAndExit('Failed to locate ' + dist + ' directory');
    }

    const srcPath = path.resolve(typescript_lib);
    const distPath = path.resolve(dist);

    fs.cpSync(srcPath, distPath, {
        force: true,
        recursive: true,
        filter: (src, dest) => {
            if (!src?.length) {
                return false;
            }
            if (fs.statSync(src).isDirectory()) {

                /*
                 * Return true only for the input directory in order to copy its context. Ignore other sub-directories.
                 * NOTE: On windows, Node appends special prefix for long paths ("\\?\") to path values. In such case,
                 * compare the path using the 'endsWith' method.
                 */
                const normalizedSrc = path.resolve(src);
                return normalizedSrc === srcPath || normalizedSrc.endsWith(srcPath);
            }
            return STANDARD_LIBRARIES.includes(path.basename(src));
        }
    });
}

copyTypescriptLibDeclarationsToDist();