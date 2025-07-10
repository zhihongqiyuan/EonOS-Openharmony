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

import worker from '@ohos.worker';
import {MainPage} from "bytecode_har"
import {sourceHarTest} from "source_har"
import {bycodeHar1Test} from "bytecode_har1"
import {sourceHarDispatchbcHarTest} from "source_har1"
import promptAction from '@ohos.promptAction'
import {test} from "entry/ets/pages/test"
test()

console.log("hap dispatch test")
sourceHarTest();
import ("source_har").then((obj :ESObject)=>{
  console.log("dynamic import source_har")
  obj.sourceHarTest();
})

bycodeHar1Test();
import ("bytecode_har1").then((obj :ESObject)=>{
  console.log("dynamic import bytecodehar2")
  obj.bycodeHar1Test();
})
@Entry
@Component
struct Index {
  @State message: string = 'Hello World';
  pageStack : NavPathStack = new NavPathStack();
  workerInstance?: worker.ThreadWorker;
  workerInstance1?: worker.ThreadWorker;
  createWorkerThread() {
    this.workerInstance = new worker.ThreadWorker("@bytecode_har/ets/workers/Worker.ets");
    this.workerInstance.onmessage = (e) => {
      console.log("receive bytecodehar thread message success", e.data);
      promptAction.showToast({
        message: "bytecodehar" + e.data,
        duration: 2000
      });
    }
    this.workerInstance1 = new worker.ThreadWorker("@bytecode_har1/ets/workers/Workers.ets");
    this.workerInstance1.onmessage = (e) => {
      console.log("receive bytecodehar thread message success", e.data);
      promptAction.showToast({
        message: "bytecodehar2" + e.data,
        duration: 2000
      });
    }
  }

  aboutToAppear(): void {
    this.createWorkerThread();
  }

  build() {
    Navigation(this.pageStack) {
      Button("push Second Page1111")
        .fontWeight(FontWeight.Bold)
        .fontSize(25)
        .alignRules({
          center: { anchor: 'container', align: VerticalAlign.Center },
          middle: { anchor: 'container', align: HorizontalAlign.Center }
        })
        .onClick(()=>{
          this.pageStack.pushPathByName("SecondPage", null, false);
        })

      Button("dispatch bchar worker")
        .fontWeight(FontWeight.Bold)
        .fontSize(25)
        .alignRules({
          center: { anchor: '__container__', align: VerticalAlign.Center },
          middle: { anchor: '__container__', align: HorizontalAlign.Center }
        })
        .onClick(()=>{
          this.workerInstance?.postMessage(" hap to bytecodehar thread begin");
        })
      Button("dispatch bchar2 worker")
        .fontWeight(FontWeight.Bold)
        .fontSize(25)
        .alignRules({
          center: { anchor: '__container__', align: VerticalAlign.Center },
          middle: { anchor: '__container__', align: HorizontalAlign.Center }
        })
        .onClick(()=>{
          this.workerInstance1?.postMessage(" hap to bytecodehar2 thread begin");
        })
      Button("dispatch sourcehar1 test")
        .fontWeight(FontWeight.Bold)
        .fontSize(25)
        .alignRules({
          center: { anchor: '__container__', align: VerticalAlign.Center },
          middle: { anchor: '__container__', align: HorizontalAlign.Center }
        })

        .onClick(()=>{
          sourceHarDispatchbcHarTest()
        })
      MainPage()
    }
    .title("Entry Page")
    .height('100%')
    .width('100%')
  }
}