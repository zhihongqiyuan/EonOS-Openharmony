/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

declare enum UserRole {
  __Admin = 0,
  __Moderator = 1,
  __User = 2
}
interface Person {
  __name: string;
  __age: number;
  __greet(param: string): void;
}
type StringChecker = {
  __typeProp1: string;
  __typeProp2: number;
};
declare class Animal {
  __speak(): string;
}
declare const config: {
  __appName: string;
  __version: string;
  __logDetails(): void;
};