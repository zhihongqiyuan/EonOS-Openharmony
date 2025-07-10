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

//trace 状态
export enum CatchTraceStatus {
  catch_trace_start = 1,
  // trace采集结束
  catch_trace_finish = 2,
  // trace采集第一次运行中
  catch_trace_first_running = 3,
  // trace采集运行中
  catch_trace_running = 4,
  // trace 采集 停止后间隔60次
  catch_trace_times = 60,
}

//采集状态
export enum TaskStatus {
  //采集初始化
  task_init = 1,
  //采集运行中
  task_running = 2,
  //采集暂停
  task_pause = 3,
  //采集结束
  task_stop = 4,
}

//采集项
export enum CollectorType {
  TYPE_CPU = 'CPU',
  TYPE_GPU = 'GPU',
  TYPE_DDR = 'DDR',
  TYPE_FPS = 'FPS',
  TYPE_HW_COUNTER = 'HW_COUNTER',
  TYPE_POWER = 'POWER',
  TYPE_TEMPERATURE = 'TEMP',
  TYPE_RAM = 'RAM',
  TYPE_NET = 'NETWORK',
  TYPE_UNKNOWN = 'UNKNOWN',
}
