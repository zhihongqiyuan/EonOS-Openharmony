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
function func1(){}

@Component
struct Child {
  @BuilderParam build1: ()=>void
  build() {
    Text('Child')
  }
}

@Entry
@Component
struct Parent {
  build() {
    Column() {
      ForEach([1], (item: number)=>{
        Column(){
          Child() {
            Text('example1')
          }
          Repeat([1])
            .each((repeatItem: RepeatItem<number>)=>{
              Child() {
                Text('example2')
              }
              Text('example6')
                .onClick(()=>{
                  func1()
                  {
                    let b = 1;
                  }
                })
            })
            .key((item: number)=>{
              func1()
              {
                let b = 1;
              }
              return item.toString()
            })
            .template('1', (repeatItem: RepeatItem<number>)=>{
              Child() {
                Text('example3')
              }
              Text('example4')
                .onClick(()=>{
                  func1()
                  {
                    let b = 1;
                  }
                })
            })
        }
      }, (item: number)=>{
        return item.toString()
      })
    }.onClick(()=>{
      func1()
      {
        let b = 1;
      }
    })
  }
}