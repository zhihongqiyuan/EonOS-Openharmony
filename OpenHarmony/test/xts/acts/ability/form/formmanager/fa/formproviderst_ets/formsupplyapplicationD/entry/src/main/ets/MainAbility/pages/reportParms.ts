/*
 * Copyright (C) 2024 Huawei Device Co., Ltd.
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

export class formParmData {
    public data: { [key: string]: string | number } = {};

    getValue(key: string): string | number | undefined {
        return this.data[key];
    }

    setValue(key: string, value: string | number): void {
        this.data[key] = value;
    }
}

export class dataObjType {
  temperature: string
  time: string
  test: number
  test3: boolean
}

export class commonEventPublishDataType {
    data: string
    parameters: {
        formId : string
    }
}