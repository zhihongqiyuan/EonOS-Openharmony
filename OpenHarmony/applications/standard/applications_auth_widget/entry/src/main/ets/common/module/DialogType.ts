/**
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

export enum DialogType {
  // only PIN
  PIN = 1,
  // only FACE
  FACE = 2,
  // only FINGER
  FINGER = 3,
  // PIN and FACE
  PIN_FACE = 6,
  // PIN and FINGER
  PIN_FINGER = 7,
  // FACE and FINGER
  FACE_FINGER_NAVIGATION = 8,
  // ALL
  ALL = 10,
}
