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

class Rectangle {
    constructor(private length: number, private width: number) {
        this.length = length;
        this.width = width;
    }

    public getArea(): number {
        return this.length * this.width;
    }
}

// Create an instance of Rectangle and test its methods
const rect = new Rectangle(5.0, 3.0);

const area: number = rect.getArea();
console.log('The area of the rectangle is: ' + area);