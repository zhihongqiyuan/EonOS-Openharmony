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
const regex = /[\u{1F300}-\u{1F9FF}]{2,4}?(?=\s)(?<=\S)([\x0D\x0B\x0C\x09\p{Zl}\p{Zs}])??(?:.){1,2}(?!=\p{Po})(?<!\w)\p{S}+/gums;
const str = `
😊😄 \n±
🥳🌟 
﹢┈┉﹣·
😁🎉\u2028\n∝
🎈🌍\r\n\㏒
😊😄﹢﹣＋－
`;
const matches = str.split(regex);
print(JSON.stringify(matches));
