/*
* Copyright (c) 2022 Shenzhen Kaihong Digital Industry Development Co., Ltd. 
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
import { AsyncCallback, Callback } from './../basic';

declare namespace napitest {
    class TestClass1 {
        fun1(v: Object): number; //Interface func para1        
        fun2(num: number, v: Object): number; //Interface func para2
        fun3(num: number, v: Object, Sam: string): number; //Interface func para3
        fun4(numcc: number): Object; //Interface func return
        fun9(): number;
    }

    interface TestClass2 {
        num1: number;
        obj: Object; //Interface var
    }
    function fun5(v1: Object): number; //namespace function para1    
    function fun6(num1: number, v2: Object): number; //namespace function para2    
    function fun7(num2: number, v3: Object, nike: string): number; //namespace function para3    
    function fun8(dd: number): Object; //namespace function return  
    /* 
    function fun7(v: number[], cb: Callback<{ name: string, age: number }>): void;
    function fun6(v: string, cb: AsyncCallback<{ psTech: string, csTech: number }>): void;
    function fun6(v: string): Promise<{ psTech: string, csTech: number }>;*/
    
}

export default napitest;
