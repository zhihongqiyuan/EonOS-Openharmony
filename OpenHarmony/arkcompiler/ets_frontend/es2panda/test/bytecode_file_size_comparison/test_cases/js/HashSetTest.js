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

const set = new Set();
set.add('Apple');
set.add('Banana');
set.add('Cherry');

// Test if the set contains a certain element
if (set.has('Banana')) {
    console.log('The set contains the element \'Banana\'.');
}

// Test removing an element from the set
set.delete('Banana');
if (!set.has('Banana')) {
    console.log('The element \'Banana\' has been removed from the set.');
}