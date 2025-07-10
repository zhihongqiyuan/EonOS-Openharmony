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

import java.util.HashMap;
/**
 * Hash Map Test
 * 
 * @since 2024/5/8
 */
public class HashMapTest {
    public static void main(String[] args) {
        HashMap<String, Integer> hashMap = new HashMap<>();

        hashMap.put("Apple", 1);
        hashMap.put("Banana", 2);
        hashMap.put("Cherry", 3);

        // Access a value by key
        System.out.println("Banana's value is: " + hashMap.get("Banana"));

        // Check if the map contains a certain key
        System.out.println("Does the map contain key 'Orange'? " + hashMap.containsKey("Orange"));

        // Remove a key-value pair from the map
        hashMap.remove("Cherry");

        // Print the size of the map
        System.out.println("Size of the map: " + hashMap.size());
    }
}