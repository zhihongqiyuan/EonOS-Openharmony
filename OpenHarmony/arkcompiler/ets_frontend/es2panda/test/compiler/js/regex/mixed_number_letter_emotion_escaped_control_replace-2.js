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
const regex = new RegExp('^(\\d?)?(\\w+)+([\\u{1F300}-\\u{1F9FF}]*)*(\\s?)?' +
    '([\\cA\\cB\\cC\\cD\\cE\\cF\\cG\\cH\\cI\\cJ\\cK\\cL\\cM\\cN\\cO\\cP\\cQ\\cR\\cS\\cT\\cU\\cV\\cW\\cX\\cY\\cZ]+)+' +
    '(\\p{N}*)*(\\p{L}?)?(\\W+)+(\\p{Nd}*)*(\\p{Lu}?)?\\S+\\D*\\10??\\9??\\8??\\7+?\\6+?\\5+?\\4+?\\3*?\\2*?\\1*?$' ,'gmu');
const str = `
①C😎\v\x0F①①①①L\\2A^^A2😎L①\u000f\r\v\f\t😏a1
1a😏\r\x0F①L\\2A\{\}AAA2\\L①\u000f\f\tr😏a1
2bbb😑\t\v\r\u000c3T😑😑4B😑 B4😑T3\u000c\t😑b2
c\x14 &\x14
d😑\xFF &\x14😑
`;
const matches = str.replace(regex, '$1$2$3$4$5$6$7$8$9$10');
print(JSON.stringify(matches));
