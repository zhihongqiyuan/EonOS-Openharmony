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
import { CPU } from '../item/CPU';
import { GPU } from '../item/GPU';
import { FPS } from '../item/FPS';
import { Power } from '../item/Power';
import { RAM } from '../item/RAM';
import { Thermal } from '../item/Thermal';
import { DDR } from '../item/DDR';
import { NetWork } from '../item/NetWork';
import { CollectorType } from './ProfilerConstant';
import { BaseProfiler } from './BaseProfiler';
import SPLogger from '../../../common/utils/SPLogger';

export class ProfilerFactory {
  static getProfilerByConfig(moduleName: string): BaseProfiler {
    if (moduleName == CollectorType.TYPE_FPS) {
      return FPS.getInstance();
    } else if (moduleName == CollectorType.TYPE_CPU) {
      return CPU.getInstance();
    } else if (moduleName == CollectorType.TYPE_GPU) {
      return GPU.getInstance();
    } else if (moduleName == CollectorType.TYPE_POWER) {
      return Power.getInstance();
    } else if (moduleName == CollectorType.TYPE_RAM) {
      return RAM.getInstance();
    } else if (moduleName == CollectorType.TYPE_TEMPERATURE) {
      return Thermal.getInstance();
    } else if (moduleName == CollectorType.TYPE_DDR) {
      return DDR.getInstance();
    } else if (moduleName == CollectorType.TYPE_NET) {
      return NetWork.getInstance();
    }
    return null;
  }
}
