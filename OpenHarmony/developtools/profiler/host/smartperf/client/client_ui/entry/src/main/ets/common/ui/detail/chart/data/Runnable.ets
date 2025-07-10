/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
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

import { Poolable } from '../utils/Poolable';
export default class Runnable extends Poolable {
  private _type: string;
  private callback: {};

  constructor(_type: string, callback: {}) {
    super();
    this._type = _type;
    this.callback = callback;
  }

  protected setType(_type: string) {
    this._type = _type;
  }

  protected setCallback(callback: {}) {
    this.callback = callback;
  }
  public instantiate(): Poolable {
    return null;
  }
}
