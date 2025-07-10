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
const str = `
opQ\brstu % 🚀🚁 𝒟💻, 😄😁jkl.
a1 \t B!🎉😄123  \n\t Hello.
a ,,\f K 🥳 789 \n\tA42 Codex.
123 !!!\r K 🥳🚀 789 \n\tA42 Codex.
a!,,\t1🥳🥳1 ab.
abde ,,\f K 😁🌍😊 3435  \n\tA42 Codex.
a!,, \n\t😊🎊 123Abc\rabc.
A **,\r 123  😁😄  456 a Codex.
B??  #  &&    abc    *🎉🎊   90    1234!  ",
C    ,,, #   \t   798  😊  👨‍💻   76  * 100.50   ",
`
const regex = new RegExp('^\\w+(?<=\\w+)([^\\S]?)\\W{1,2}([!@#;:,.]{0,2})' +
    '[\\f\\v\\r\\n\\t\\b]{1,2}\\1(\\s*\\w*\\s*)*(?<![^\\D])' +
    '[\\uD83C-\\uDBFF\\uDC00-\\uDFFF]{2,}([^\S]?\\d+\\s*)\\w*.\\b\\w+\\.$', 'gsmiu');
const matches = str.match(regex);
print(JSON.stringify(matches));
