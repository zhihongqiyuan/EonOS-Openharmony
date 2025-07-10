/**
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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

import {sourceHarTest} from  "source_har";
import {bytecodeHar1Test} from "bytecode_har1";
import {test} from "bytecode_har/ets/components/test";
test();

@Component
export struct MainPage {
  @State message: string = 'dispatch source Har';
  @State bytecideMessage: string = 'Initial state';

  build() {
    Row() {
      Column() {
        Button(this.message)
          .fontSize(25)
          .fontWeight(FontWeight.Bold)
          .onClick(()=>{
            sourceHarTest();
            import("source_har").then((obj: ESObject)=>{
              console.log("dynamic import source_har");
              obj.sourceHarTest();
            })
          })
        Button("bytecode har1 test")
          .fontSize(25)
          .fontWeight(FontWeight.Bold)
          .onClick(()=>{
            sourceHarTest();
            import("bytecode_har1").then((obj: ESObject)=>{
              console.log("dynamic import source_har");
              obj.bytecodeHar1Test();
              this.bytecideMessage = "byteciode har1 test executed"
            })
          })
        Text(this.bytecideMessage)
          .fontSize(25)
          .fontWeight(FontWeight.Bold)
      }
      .width('100%')
    }
    .height('100%')
  }
}
