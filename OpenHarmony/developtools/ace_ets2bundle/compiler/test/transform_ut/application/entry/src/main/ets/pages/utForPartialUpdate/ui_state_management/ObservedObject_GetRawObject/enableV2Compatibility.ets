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
 // enableV2Compatibility test
import { UIUtils } from '@ohos.arkui.StateManagement';
@Observed
class Decorated_Observed_Compatible_Info {
  name: string = "Decorated_Observed_Compatible_Info";
  age: number = 25;
}

@Entry
@Component
struct Decorated {
  @State decorated_observed_compatible_info: Decorated_Observed_Compatible_Info = new Decorated_Observed_Compatible_Info();
  build() {
    Row() {
      Decorated_Child_Full({
        decorated_observed_compatible_info: UIUtils.enableV2Compatibility(this.decorated_observed_compatible_info)
      })
    }
  }
}

@ComponentV2
struct Decorated_Child_Full {
  @Require @Param decorated_observed_compatible_info: Decorated_Observed_Compatible_Info;
  build() {

  }
}