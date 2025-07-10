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
const regex = /(?<=(?<=[\u{1F600}-\u{1F64F}]{1,1000000})\s{1}(?!=\s+)[\u{1F300}-\u{1F9FF}]*)[\r\n\f\v\t\b]{2,4}(?=[\u{1D49C}-\u{1D4A9}]{1,5}(?!<\s{2,3})\s{1}(?=[^\r\n\t\v\b\f]*))/gmu;
const str = `
1😀22😎333😏😏😏%
1😀2😆3😒😒😒4$
1𝒜𝓑78😑123😏😏%
1     𝓑78😕abvd😖😏😊𝒜𝒜𝓑𝓑
0w😛😛𝒜𝓑78😛😕___\f\r\n\t\v\b𝒜𝒜𝓑𝓑111
\r 2😆32😒😒😒4
😁\f😎\r\n\u{1D49D}
😈😐😑😒😓😔😕😖😗😘😙😚😛😜😝😞😟\t\r\n\b\f𝒞
😁\f🌎\r\nabc
`;
const matches = str.split(regex);
print(JSON.stringify(matches)); 
