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

const list: string[] = [];

list.push('Apple');
list.push('Banana');
list.push('Cherry');
list.push('Date');

// Get the array's iterator
const iterator = list.values();

// Use the iterator to iterate over the collection
let next = iterator.next();
while (!next.done) {
    const fruit = next.value;
    console.log(fruit);
    next = iterator.next();
}