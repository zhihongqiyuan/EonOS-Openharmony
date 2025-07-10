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
const regex = /(?<!\s+)[\f\v\r\n\t]+(?=\W+)([\p{L}|\p{S}﹏๑]+\s+)+(?<=[^\S]+)[\u{1F300}-\u{1F9FF}]+(?!\D+)/gmu; 
const str = `
123\n   ๑ŏ﹏ŏ๑ \t  ノ◕ω◕ノ ＜(▰˘◡˘▰) 🌎123
abc\n๑ŏ﹏ŏ๑   \r  ●﹏●   \n   ▓▓つ──☆✿✿✿ \t  ノ◕ω◕ノ \v ღゝ◡╹ノ♡  🌎123
abc\n   ٩(๑❛ᴗ❛๑)۶ \t  ノ◕ω◕ノ \v ღゝ◡╹ノ♡  🌎123
`;
const matches = str.split(regex);
print(JSON.stringify(matches)); 
