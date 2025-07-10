/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file ePara1cept in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either ePara1press or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

type function1 = (Para1: number, Para2?: string) => void;

type function2 = (...args: number[]) => number;

type function3 = (Para1: number, Para2: number) => number;

type function4 = () => number | null;

type function5 = {
  (Para1: number): string;
  (Para1: string): number;
};

type function6 = (Para1: number, Para2: string) => string;

type function7 = () => void;

type function8<T> = (Para1: T) => T;

type function9 = (Para1: number) => Promise<number>;

type function10 = (Para1: number) => (Para2: string) => void;

function function11(Para1 ?:(Para1: number) => void): void {}

function function12(Para1: number): (Para1: string) => number { return () => 0; }

interface MyInterface2 {
  Para1?: (Para1: number) => string;
}

function function13(Para1?: (Para1: (Para1: string) => void) => void): void {}

class User2 {
  constructor(Para1?: (Para1: string) => void) {}
}

function function14(): (Para1: number) => (str: string) => boolean {
  return () => (str) => true;
}

type function15 = (Para1: (Para1: number) => boolean) => string;

function function16(): (Para1: string) => (Para1: number) => void {
  return (Para1) => (Para1) => {};
}

function function17(Para3?: (Para3: number) => (Para3: string) => boolean): void {}

type type4 = string;
type type5 = (Para1: string) => type4;