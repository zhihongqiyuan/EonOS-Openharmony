/*
 * Copyright (c) 2022-2024 Huawei Device Co., Ltd.
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

/*
 * This test case is used to verify the compilation conversion of member variables within the @ReusableV2 decorated custom component.
 */
@ObservedV2
class A {
  @Trace n:number;
  constructor(n:number) {
    this.n = n;
  }
}

class B {
  b:number;
  a:A;
  constructor(b:number,a:A) {
    this.b = b;
    this.a= a;
  }
}

@Entry
@ReusableV2
@ComponentV2
struct V2ReusableMember {
  @Local num:number = 100;
  @Param num2:number = 150;
  @Require @Param num4:number;
  @Provider('a') pro:number = 500;
  @Consumer('b') con:string = '5000';
  @Consumer('c') con2?:boolean;
  @Event w1:(paramName:string)=>void = (paramName:string)=>{};
  @Event w2:(paramName:string)=>void = ()=>{};
  @Event w3:(paramName:string)=>number;
  @Event w4:(paramName:string)=>number = (paramName:string)=>{ return 15};
  num3:number = 10;
  color:string = "#282556";
  boo:boolean = true;
  bbb:B = new B(50,new A(20));
  static bbb2:B = new B(50,new A(20));
  getX() { return 10; }
  @Computed
  get Y() { return this.num3 * 99; }
  @Monitor('num')
  watchNum() { console.log('num has changed.') }
  @Monitor('num')
  watchNum2() { console.log('num has changed.') }
  @Builder m() {}
  @BuilderParam closer:()=>void = this.m;
  build() {
    Column() {
      Text()
        .width(200)
        .margin({top:30,bottom:V2Reusable.bbb2.a.n})
        .fontStyle(FontStyle.Italic)
        .fontColor(this.color)
        .borderWidth(this.num)
        .borderRadius(this.Y)
        .onClick(() => {
          console.log('nihao')
        })
    }
  }
}