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
 * This test case is used to verify the correctness of the properties inside the initialRender function within the @ReusableV2 decorated custom component.
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
struct V2Reusable {
  @Local num:number = 100;
  @Param num2:number = 150;
  @Provider('a') pro:number = 500;
  num3:number = 10;
  color:string = "#282556";
  boo:boolean = true;
  ss:A = new A(300);
  bbb:B = new B(50,new A(20));
  static bbb2:B = new B(50,new A(20));
  static ss2:A = new A(300);
  getX() { return 10; }
  @Computed
  get Y() { return this.ss.n * 99; }
  @Builder getA() {
    Text()
      .width(200)
      .margin({top:30,bottom:V2Reusable.bbb2.a.n})
      .padding({top:20,bottom:this.ss.n})
      .fontStyle(FontStyle.Italic)
      .textOverflow({ overflow: TextOverflow.Ellipsis })
      .height(this.ss.n)
      .fontColor(this.color)
      .borderWidth(this.num)
      .borderRadius(this.num3)
      .fontWeight(this.bbb.b)
      .minFontSize(this.bbb.a.n)
      .privacySensitive(this.boo)
      .maxFontSize(V2Reusable.bbb2.b)
      .lineHeight(V2Reusable.ss2.n)
      .textAlign(TextAlign.Center)
      .maxLines(this.num2)
      .minFontScale(this.getX())
      .maxFontScale(this.Y)
      .onClick(() => {
        console.log('nihao')
      })
  }
  build() {
    Column() {
      Text()
        .width(200)
        .margin({top:30,bottom:V2Reusable.bbb2.a.n})
        .padding({top:20,bottom:this.ss.n})
        .fontStyle(FontStyle.Italic)
        .textOverflow({ overflow: TextOverflow.Ellipsis })
        .height(this.ss.n)
        .fontColor(this.color)
        .borderWidth(this.num)
        .borderRadius(this.num3)
        .fontWeight(this.bbb.b)
        .minFontSize(this.bbb.a.n)
        .privacySensitive(this.boo)
        .maxFontSize(V2Reusable.bbb2.b)
        .lineHeight(V2Reusable.ss2.n)
        .textAlign(TextAlign.Center)
        .maxLines(this.num2)
        .minFontScale(this.getX())
        .maxFontScale(this.Y)
        .onClick(() => {
          console.log('nihao')
        })
    }
  }
}