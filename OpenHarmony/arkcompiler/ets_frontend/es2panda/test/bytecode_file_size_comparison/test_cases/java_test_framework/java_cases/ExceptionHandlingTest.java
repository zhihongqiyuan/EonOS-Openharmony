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
 * Exception Handling Test
 * 
 * @since 2024/5/8
 */
public class ExceptionHandlingTest {
    /**
     * Define a method that may throw an exception
     * 
     * @param a int a
     * @param b int b
     * @return int a / b
     * @throws ArithmeticException throw ArithmeticException
     */
    public static int divide(int a, int b) throws ArithmeticException {
        if (b == 0) {
            throw new ArithmeticException("Division by zero is not allowed.");
        }
        return a / b;
    }

    public static void main(String[] args) {
        try {
            int result = divide(10, 0); // This will throw an Exception
            System.out.println("Result: " + result);
        } catch (ArithmeticException e) {
            System.out.println("Caught an Exception: " + e.getMessage());
        } finally {
            System.out.println("This is always executed.");
        }
    }
}