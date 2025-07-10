/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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


// case 1
let accessor:number = 1;
print(accessor);

// case 2
class Test {
    accessor accessor:number = 2;
}
let t = new Test();
print(t.accessor);
t.accessor = 3;
print(t.accessor);

// case 3
class Test2 {
    accessor() {
        print("accessor method");
    }
}
let t2 = new Test2();
t2.accessor();

// case 4
class Test3 {
    static accessor() {
        print("static accessor method");
    }
}
Test3.accessor();

// case 5
class Test4 {
    public accessor() {
        print("public accessor method");
    }
}

let t4 = new Test4();
t4.accessor()

// case 6
class Test5 {
    private accessor:string = "private accessor property";
    test() {
        print(this.accessor);
    }
}
let t5 = new Test5();
t5.test()

// case 7
class Test6 {
    #accessor:string = "#accessor";
    test() {
        print(this.#accessor);
    }
}
let t6 = new Test6();
t6.test()

// case 8
{
    class accessor {
        static test:string= "class name is accessor";
    }
    print(accessor.test);

}
// case 9
{
    function accessor() {
        print("accessor function");
    }
    accessor();
}

