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

const hashMap: Map<string, number> = new Map();

hashMap.set('Apple', 1);
hashMap.set('Banana', 2);
hashMap.set('Cherry', 3);

// Access a value by key
console.log('Banana\'s value is: ' + hashMap.get('Banana'));

// Check if the map contains a certain key
console.log('Does the map contain key \'Orange\'? ' + hashMap.has('Orange'));

// Remove a key-value pair from the map
hashMap.delete('Cherry');

// Print the size of the map
console.log('Size of the map: ' + hashMap.size);