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
const regex = /(?<=(?<=\w+)\d{1,}(?!=\D{2,})\w+)\d{3}(?=\D+(?<!\d+)\d{2,}(?=[a-z]*))/gm;
const str = `
_a1de456g789
cba__1fed654ghi98
abc12_123abc456
x987@@@678h34
abc12_123456
`;
const matches = str.matchAll(regex);
print(JSON.stringify([...matches])); 
