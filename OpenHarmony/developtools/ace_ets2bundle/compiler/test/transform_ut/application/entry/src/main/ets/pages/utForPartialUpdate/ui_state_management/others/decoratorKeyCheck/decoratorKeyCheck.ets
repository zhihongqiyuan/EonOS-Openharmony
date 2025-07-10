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
import { stringVariable, stringObj, stringFunction } from '../../../../../test/decoratorKeyCheck'

let para:Record<string,number> = { 'PropA': 47 };
let storage: LocalStorage = new LocalStorage(para);
@Entry(storage)
@Component
struct Index {
  @LocalStorageLink(stringVariable) LocalStorageLink: string = 'LocalStorageLink';
  @LocalStorageLink(stringObj.stringKey) LocalStorageLink1: string = 'LocalStorageLink1';
  @LocalStorageLink(stringFunction()) LocalStorageLink2: string = 'LocalStorageLink2';
  @LocalStorageLink('LocalStorageLink3') LocalStorageLink3: string = 'LocalStorageLink3';
  
  @LocalStorageProp(stringVariable) LocalStorageProp: string = 'LocalStorageProp';
  @LocalStorageProp(stringObj.stringKey) LocalStorageProp1: string = 'LocalStorageProp1';
  @LocalStorageProp(stringFunction()) LocalStorageProp2: string = 'LocalStorageProp2';
  @LocalStorageProp('LocalStorageProp3') LocalStorageProp3: string = 'LocalStorageProp3';

  @StorageProp(stringVariable) StorageProp: string = 'StorageProp';
  @StorageProp(stringObj.stringKey) StorageProp1: string = 'StorageProp1';
  @StorageProp(stringFunction()) StorageProp2: string = 'StorageProp2';
  @StorageProp('StorageProp3') StorageProp3: string = 'StorageProp3';

  @StorageLink(stringVariable) StorageLink: string = 'StorageLink';
  @StorageLink(stringObj.stringKey) StorageLink1: string = 'StorageLink1';
  @StorageLink(stringFunction()) StorageLink2: string = 'StorageLink2';
  @StorageLink('StorageLink3') StorageLink3: string = 'StorageLink3';
 
  @Provide(stringVariable) Provide: string = 'Provide';
  @Provide(stringObj.stringKey) Provide1: string = 'Provide1';
  @Provide(stringFunction()) Provide2: string = 'Provide2';
  @Provide('Provide32') Provide3: string = 'Provide3';
  @Provide Provide4: string = 'Provide4';
  @Provide({allowOverride: stringVariable}) Provide5: string = 'Provide5';
  @Provide({allowOverride: stringObj}) Provide6: string = 'Provide6';
  @Provide({allowOverride: stringFunction}) Provide7: string = 'Provide7';

  @Consume(stringVariable) Consume: string;
  @Consume(stringObj.stringKey) Consume1: string;
  @Consume(stringFunction()) Consume2: string;
  @Consume('Consume3') Consume3: string;
  @Consume Consume4: string;

  build() {
    Row() {

    }
  }
}