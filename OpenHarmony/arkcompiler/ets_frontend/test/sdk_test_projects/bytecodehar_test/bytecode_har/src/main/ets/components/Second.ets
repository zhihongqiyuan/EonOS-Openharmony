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

@Builder
export function SecondPageBuilder() {
  Second()
}

@Component
export struct Second {
  pageStack: NavPathStack = new NavPathStack();
  @State message: string = 'SecondPage';
  @State bytecideMessage: string = 'Initial state';

  build() {
    NavDestination() {
      Column() {
        Text(this.message)
          .fontSize(25)
          .fontWeight(FontWeight.Bold)
          .fontColor(Color.Red)
        Text("push to bytecode har1")
          .fontSize(25)
          .fontWeight(FontWeight.Bold)
          .fontColor(Color.Red)
          .onClick(()=>{
            this.pageStack.pushPathByName("BytecodeHar1Page", null, false);
          })
      }
    }
    .title('SecondPage')
    .onReady((context: NavDestinationContext)=>{
      this.pageStack = context.pathStack;
    });
  }
}
