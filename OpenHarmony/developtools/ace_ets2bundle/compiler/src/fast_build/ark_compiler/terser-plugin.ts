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
import terser from '@rollup/plugin-terser';

export function terserPlugin(): Object {
    return terser({
        compress: {
            defaults: false,
            dead_code: true,
            collapse_vars: true,
            unused: true,
            drop_debugger: true,
            if_return: true,
            reduce_vars: true,
            join_vars: false,
            sequences: 0
        },
        format: {
            semicolons: false,
            beautify: true,
            braces: true,
            indent_level: 2
        }
    });
}