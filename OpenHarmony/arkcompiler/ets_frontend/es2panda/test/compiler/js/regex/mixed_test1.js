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
const str = `
Abcd # 😊 𝒜𝓑, 🎉🎊ef
gHIjKL @# 🥳🧑‍💻 𝕏🌟, ♥️❤️ghi
opQrstu % 🚀🚁 𝒟💻, 😄😁jkl
xXzabCde !&* 📝💡 𝓢🎉, 🐱‍👓👨‍💻rstuv
vW  $ ✨👋 𝓑🚀, 🥳🥸vwxyz
`;
const regex = new RegExp(`([a-zA-Z]{2,6})\\s*([#$@%&*!]*?)\\s*([\\s\\S]{2,8})[\\u0020]*` + 
    `([\\u2500-\\u28FF\\p{Z}\\p{P}]{2,8})\\s*([\\s\\S]{2,8})[\\u0020]*([\\W|\\d{2,8}])\\s*([a-zA-Z]{2,8})`, 'gu');
const matches = str.match(regex);
print(JSON.stringify(matches, null, 2));
