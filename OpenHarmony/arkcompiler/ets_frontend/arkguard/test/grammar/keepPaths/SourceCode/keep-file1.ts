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

import { DependencyClass } from './depency-file1';
class Animal {
    constructor(public name: string) { }

    makeSound(): void {
        console.log(`${this.name} is making a sound.`);
    }
}

class Cat extends Animal {
    makeSound(): void {
        console.log(`${this.name} says: Meow!`);
    }
}

class Dog extends Animal {
    makeSound(): void {
        console.log(`${this.name} says: Woof!`);
    }
}

const cat = new Cat('Kitty');
const dog = new Dog('Buddy');

cat.makeSound();
dog.makeSound();
let depenIns = new DependencyClass();
depenIns.depencyMethod1();
depenIns.depencyProp1;
depenIns.depencyProp2;
