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
const regex = new RegExp('^(?<=\\D+)\\W(?!\\s)[\\u{1D49C}-\\u{1F9FF}]' +
    '([\\cA\\cB\\cC\\cD\\cE\\cF\\cG\\cH\\cI\\cJ\\cK\\cL\\cM\\cN\\cO\\cP\\cQ\\cR\\cS\\cT\\cU\\cV\\cW\\cX\\cY\\cZ])' +
    '(?<![^\\W])[^\\s](?=\\1*)', 'ugm');
const str = `
\t😀\r😀\r\n\t😈\x07😈\x07
\t😈\t😈\x07
😆\v🌎\n𝒜\n
😏\t😏\t
😒😒\f😒\f
`
const matches = str.replace(regex, "@");
print(JSON.stringify(matches)); 
