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

/**
 * Loop Structures Test
 * 
 * @since 2024/5/8
 */
public class LoopStructuresTest {
    public static void main(String[] args) {
        // Test for loop
        System.out.println("For loop:");
        for (int i = 0; i < 5; i++) {
            System.out.print(i + " ");
        }
        System.out.println();

        // Test while loop
        System.out.println("While loop:");
        int count = 0;
        while (count < 5) {
            System.out.print(++count + " ");
        }
        System.out.println();

        // Test do-while loop
        System.out.println("Do-while loop:");
        int loopCount = 0;
        do {
            System.out.print(++loopCount + " ");
        } while (loopCount < 5);
        System.out.println();

        // Test nested loops
        System.out.println("Nested loops:");
        for (int i = 1; i <= 3; i++) {
            for (int j = 1; j <= 3; j++) {
                System.out.print("(" + i + ", " + j + ") ");
            }
            System.out.println();
        }
    }
}