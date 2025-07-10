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


import assert from 'assert'

let x = 1;
assert(x === 1);

function foo_01() {
    return x;
}
foo_01();
assert(foo_01() === x);


function foo_02()
{
    function foo_02() {
        function bar_02() {
            return x;
        }
        bar_02();
        assert(bar_02() === x);
        return bar_02;
    }
    function bar_02() {
        return 2;
    }
    assert(bar_02() === x+1);
    foo_02()();
    assert(foo_02()() === x);
    return foo_02;
}
foo_02();
assert(foo_02()()() === x);


function foo_03(...paras) {
    paras[0];
    paras[1];
    paras[2]()()();
    return paras[0] + paras[1] + paras[2]()()();
}
foo_03(1,2,foo_02);
assert(foo_03(1,2,foo_02) === 1+2+x);

function foo_04({}) {
    return x;
}
foo_04(foo_03);
assert(foo_04(foo_03) === x);


function foo_05({...a}) {
    a.a;
    a.b(foo_03);
    return a.a+a.b(foo_03);
}
foo_05({a:1, b: foo_04});
assert(foo_05({a:1, b: foo_04}) === 1+x);


function foo_06({a}) {
    a;
    return a;
}
foo_06({a:1});
assert(foo_06({a:1}) === 1);


function foo_07({a, ...b}) {
    a;
    b.c;
    b.d({a:1});
    return b.c + b.d({a:a});
}
foo_07({a: "1", c: foo_06({a:1}), d: foo_06});
assert(foo_07({a: "1", c: foo_06({a:1}), d: foo_06}) === "11");


function foo_08({a,"c":d,...b}) {
    a;
    d.c;
    b.d({a:1});
    return d.c + b.d({a:a});
}
foo_08({a: "1", c: {c: foo_06({a:1})}, d: foo_06});
assert(foo_08({a: "1", c: {c: foo_06({a:1})}, d: foo_06}) === "11");


function foo_09({a=1, b=a}) {
    a;
    b;
    return b;
}
foo_09({a:1});
assert(foo_09({a:1}) === 1);


function foo_10(para = 1) {
    para;
    return para;
}
foo_10();
assert(foo_10() === 1);

function foo_11(a?:string, b?:number, ...c:number[]) {
    a;
    b;
    c[0];
    return a!+b!+c[0]+c[1];
}
foo_11("a",1,2,3,4);
assert(foo_11("a",1,2,3,4) === "a123");


function foo_12(a1:number, a2:string) {
    return a1 + a2;
}
foo_12(1, "a");
assert(foo_12(1, "a") === "1a");

let val = 1;
function foo_13(a1:number, a2:string) {
    return a1 + a2 + val;
}
assert(foo_13(1, "1") === "111");