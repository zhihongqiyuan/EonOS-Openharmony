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
a1  B!🎉😄123  \n\t Hello&
abc  D😊🌟🎉  456\f\n XYZ#
xy9   Z  .🎈 789\rA1b2#
mno123   R 🥳🌍  01\f\twd24C3%
abcde  K, 😁❤️😊  345  \n\t142 Codex!
z7  🎉🎉 ;;;  89\rA23Cs@1
abcd   M  :  🎊\n\tq4!
1pqr     J  !  😊🥳  6789 \t\t11x12&
xyz12   B  *  🎉  3\f!ww A3bC%
`;
const regex = new RegExp(`(?<=(?<![\\w])(?:[\\uD83C-\\uDBFF\\uDC00-\\uDFFF]{1,2})(?![\\w\\d]))\\s+|
    \\s+(?=(?<=[a-zA-Z0-9]{1,})(?:[\\f\\r\\n\\t\\v]+)(?=\\w{3,}\\w+))`, 'gimu');
const matches = str.match(regex);
print(JSON.stringify(matches));
