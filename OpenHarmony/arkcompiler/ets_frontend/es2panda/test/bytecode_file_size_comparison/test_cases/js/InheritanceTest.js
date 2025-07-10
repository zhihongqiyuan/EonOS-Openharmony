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

// Base class Vehicle
class Vehicle {
    constructor(type) {
        this.type = type;
    }

    move() {
        console.log('The vehicle is moving.');
    }

    startEngine() {
        console.log('Starting the engine of a generic vehicle.');
    }
}

// Subclass Car extends Vehicle
class Car extends Vehicle {
    constructor(type, model) {
        super(type);
        this.model = model;
    }

    startEngine() {
        console.log('Starting the ' + this.model + ' car engine.');
    }

    honk() {
        console.log('Beep beep! The car is honking.');
    }
}

// Test the inheritance
const vehicle = new Vehicle('Generic');
vehicle.move();
vehicle.startEngine();

const car = new Car('Sedan', 'Toyota Camry');
car.move();
car.startEngine();
car.honk();