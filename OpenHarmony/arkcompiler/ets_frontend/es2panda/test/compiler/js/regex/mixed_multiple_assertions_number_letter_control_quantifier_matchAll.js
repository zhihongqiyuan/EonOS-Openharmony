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
const regex = new RegExp('(?<=\\d)[^\\w]+(?=\\p{L})\\w?' +
    '[\\cA\\cB\\cC\\cD\\cE\\cF\\cG\\cH\\cI\\cJ\\cK\\cL\\cM\\cN\\cO\\cP\\cQ\\cR\\cS\\cT\\cU\\cV\\cW\\cX\\cY\\cZ]{2,5}' +
    '(?<![A-Z])[1-7098b-yaz_]+(?![^\\W])', 'gmu');
const str = `
1\x01B\x02c\x03d\x04
2\x05E\x06\x07f\x07g\x08
3\x09h\x0A\x0BI\x0Bi\x0Ci
4\x0Dj\x0E\x0Fo\x0F\x10
5\x0F\x0E\x0Fw\x0F\x10
`
const matches = str.matchAll(regex);
print(JSON.stringify([...matches])); 
