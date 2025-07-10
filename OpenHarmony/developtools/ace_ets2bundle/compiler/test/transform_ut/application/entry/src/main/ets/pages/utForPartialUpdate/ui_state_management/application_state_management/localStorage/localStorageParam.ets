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
let para: Record<string, number> = { 'PropA': 47 };
let storage: LocalStorage = new LocalStorage(para);
@Entry(storage)
@Component
struct localStorageParam {
  @LocalStorageLink('PropA') storageLink1: number = 1;

  build() {
    Column() {
      ComA({message: 'Hi'}, storage);
      ComB({}, storage);
      ComD(storage);
      ComE();
    }
  }
}

@Component
struct ComA {
  @Prop message: string;
  build() {

  }
}

@Component
struct ComB {
  build() {

  }
}

@Component
struct ComD {
  build() {

  }
}

@Component
struct ComE {
  build() {

  }
}