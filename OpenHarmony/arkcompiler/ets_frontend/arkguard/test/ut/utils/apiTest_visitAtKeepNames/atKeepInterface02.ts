/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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

// @KeepAsConsumer
interface MyInterface01 {
  interfaceProperty01: string;
  interfaceMethod01(): void;
}

// @KeepAsConsumer
export interface MyInterface02 {
  interfaceProperty02: string;
  interfaceMethod02(): void;
}


interface MyInterface03 {
  // @KeepAsConsumer
  interfaceProperty03: string;

  interfaceProperty03_01: string;

  // @KeepAsConsumer
  interfaceMethod03(): void;
}

export interface MyInterface04 {
  // @KeepAsConsumer
  interfaceProperty04: string;

  // @KeepAsConsumer
  interfaceMethod04(): void;
}