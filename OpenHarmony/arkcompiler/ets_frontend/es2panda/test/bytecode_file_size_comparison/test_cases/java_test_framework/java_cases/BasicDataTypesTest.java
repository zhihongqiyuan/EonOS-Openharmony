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
 * Basic Data Types Test
 * 
 * @since 2024/5/8
 */
public class BasicDataTypesTest {
    public static void main(String[] args) {
        // Integer type
        byte byteValue = 127;
        short shortValue = 32767;
        int intValue = 123456789;
        long longValue = 1234567890123456789L;

        // Floating-point types
        float floatValue = 123.456f;
        double doubleValue = 123.456789d;

        // Boolean type
        boolean booleanValue = true;

        // Character type
        char charValue = 'A';

        // Print the values of basic data types
        System.out.println("Byte value: " + byteValue);
        System.out.println("Short value: " + shortValue);
        System.out.println("Int value: " + intValue);
        System.out.println("Long value: " + longValue);
        System.out.println("Float value: " + floatValue);
        System.out.println("Double value: " + doubleValue);
        System.out.println("Boolean value: " + booleanValue);
        System.out.println("Char value: " + charValue);
    }
}