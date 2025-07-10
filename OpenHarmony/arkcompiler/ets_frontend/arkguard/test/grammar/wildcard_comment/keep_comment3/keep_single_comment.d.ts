/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * A generic stack class.
 * @template T - The type of elements in the stack.
 */
declare class A<T> {
    /**
     * Constructor for Stack.
     */
    constructor();

    /**
     * Pushes an element onto the stack.
     * @param {T} element - The element to push.
     */
    B(element: T): void;

    /**
     * Pops an element from the stack.
     * @returns {T} The popped element.
     */
    pop(): T;

    /**
     * Gets the size of the stack.
     * @returns {number} The size of the stack.
     */
    size(): number;
}

/**
 * An interface representing a geometric shape.
 * @interface
 */
declare interface C {
  /**
   * Calculates the area of the shape.
   * @returns {number} The area of the shape.
   */
  calculateArea(): number;
}

/**
 * A function to capitalize the first letter of a string.
 * @param {string} str - The input string.
 * @returns {string} The capitalized string.
 */
declare function D(str: string): string;