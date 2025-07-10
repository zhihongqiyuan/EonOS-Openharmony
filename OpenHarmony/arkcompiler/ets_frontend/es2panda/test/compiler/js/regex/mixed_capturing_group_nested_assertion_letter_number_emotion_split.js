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
const regex = new RegExp('(\\w+(?=\\d+([\\u{1F300}-\\u{1F9FF}]*' +
    '(?:[\\cA\\cB\\cC\\cD\\cE\\cF\\cG\\cH\\cI\\cJ\\cK\\cL\\cM\\cN\\cO\\cP\\cQ\\cR\\cS\\cT\\cU\\cV\\cW\\cX\\cY\\cZ]+)' +
    '\\2))\\1(?:\\p{N}+(\\D+(\\s*(?!\\W)\\4)\\3))\\1)\\1+\\2?\\3*\\4{1,3}', 'gum');
const str = `
a1😊\x09a1😊\t😊\t😊\t😊\t😊\t\t
aa2😊\x0Aaa2🌍\x0A😊\n😊
bbb222😊\x0Abbb222😊\x0A😊\n😊
\n\n
c3\rc3\r\r\r\r
`;
const matches = str.split(regex);
print(JSON.stringify(matches));
