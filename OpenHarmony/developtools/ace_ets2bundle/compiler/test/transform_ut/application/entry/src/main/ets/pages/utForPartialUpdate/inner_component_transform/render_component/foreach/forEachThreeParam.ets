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
let mKeyGenerator2: (datasource: string, index: number) => string = (item: string, index: number) => 'item' + index

@Entry
@Component
struct foreachThreeParam {
  @State arr: number[] = [10, 20, 30];
  private mKeyGenerator?: (datasource: string, index: number) => string;
  private mKeyGenerator1: (datasource: string, index: number) => string = (item: string, index: number) => 'item' + index;
  build() {
    Column() {
      ForEach(this.arr, ((item: number) => {
        Text(item)
      }), this.mKeyGenerator);
      ForEach(this.arr, ((item: number) => {
        Text(item)
      }), this.mKeyGenerator1);
      ForEach(this.arr, ((item: number) => {
        Text(item)
      }), mKeyGenerator2);
    }
  }
}