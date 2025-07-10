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
const regex = /(?<!\w+)\p{Nd}+(?=[^0-9]+)([\u{1F300}-\u{1F9FF}]+)\p{N}+(?<=\d+)\1(?![^\D]+)/gmu;
const str = `
😀1😀2😀3😀😏😏😏
1😀2😆3😒😒😒4$
1🌎22😁333😁n
2😆45😆678
3😒90😒12
4🌎11🌎34
`
const matches = str.match(regex);
print(JSON.stringify(matches));
