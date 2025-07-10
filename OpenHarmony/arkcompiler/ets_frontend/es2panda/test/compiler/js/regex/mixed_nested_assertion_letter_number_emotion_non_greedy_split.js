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
const regex = /(?<=(?<=[5-90]+?)\S{1,}?(?!\u{1F600}-\u{1F64F}+?)\d{2}?)[\u{1F600}-\u{1F64F}]{2,3}(?=\w{3,4}?(?<![\u{1F600}-\u{1F64F}]{2}?)\D{3,1000000000000}(?=\d??))/gmu;
const str = `
8😀22😎😎3333😏😏😏
41😀23😆😎3wd😒😒😒4$
1𝒜𝓑78😑123😏😏%
1     𝓑78😕abvd😖😏😊𝒜𝒜𝓑𝓑
0w😛😛𝒜𝓑78😛😕___\f\r\n\t\v\b𝒜𝒜𝓑𝓑111
\r 2😆32😒😒😒4
`;
const matches = str.split(regex);
print(JSON.stringify(matches)); 
