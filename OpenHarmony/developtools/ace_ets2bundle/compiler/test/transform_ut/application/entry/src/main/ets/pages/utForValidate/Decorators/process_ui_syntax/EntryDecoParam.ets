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
let storage = LocalStorage.getShared();

class EntryDecoParamClassA {
  public id: number = 1;
  public type: number = 2;
  public a: string = "aaa";
  constructor(a: string){
    this.a = a;
  }
}

@Entry
@Component
struct LocalStorageComponent {
  @LocalStorageLink("storageSimpleProp") simpleVarName: number = 0;
  @LocalStorageProp("storageObjectProp") objectName: EntryDecoParamClassA = new EntryDecoParamClassA("x");
  build() {
    Column() {
      Text(this.objectName.a)
        .onClick(()=>{
          this.simpleVarName +=1;
          this.objectName.a = this.objectName.a === 'x' ? 'yex' : 'no';
        })
    }
    .height(500)
  }
}