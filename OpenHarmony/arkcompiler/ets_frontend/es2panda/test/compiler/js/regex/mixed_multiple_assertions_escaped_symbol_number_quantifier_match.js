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
const regex = /(?<=\p{Nd}+)[\r\n\t\v\b\f]+(?=\p{N}+)\d+\s+\w+(?<=\p{Lu})[1-5]+(?=\p{Ll}+)/gmu; 
const str = `
1\r2\tA3b
2\b1234567890\nB5cdef
3\n\r\v\b\f123\rD6e2
0\f1 abC3d4
`;
const matches = str.match(regex);
print(JSON.stringify(matches)); 
