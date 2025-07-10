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
const regex = /(?<=(?<!\u{1F600}-\u{1F64F}+)\W{1,}(?=[1-9]*)\d{2})[\u{1F600}-\u{1F64F}]{1}(?=[^u{1F600}-\u{1F64F}]{2,4}(?<=\d?)\D{3}(?!=\u{1F600}-\u{1F64F}*))/gmu
const str = `
1😀22😎333😏😏😏%
1😀2😆3😒😒😒4$
1𝒜𝓑78😑123😏😏😏%
1     𝓑78😕abvd😖😏😊𝒜𝒜𝓑𝓑
\r𝒜𝓑78😛\f\r\n\t\v\b𝒜𝒜𝓑𝓑
\r 2😆32😒😒😒4
`;
const matches = str.replace(regex, "@");
print(JSON.stringify(matches)); 
