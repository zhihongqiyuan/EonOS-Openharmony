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
export const dbName = 'gp.db';

export const dbVersion = 1;

export const sql_t_general_info =
  'CREATE TABLE IF NOT EXISTS ' +
  't_general_info' +
  '(' +
  'sessionId TEXT PRIMARY KEY, ' +
  'taskId TEXT NOT NULL,' +
  'appName TEXT,' +
  'appVersion TEXT,' +
  'packageName TEXT,' +
  'startTime TEXT,' +
  'endTime TEXT,' +
  'testDuration TEXT,' +
  'taskName TEXT,' +
  'testCase TEXT,' +
  'testType TEXT,' +
  'user TEXT,' +
  'userId TEXT,' +
  'projectId TEXT,' +
  'dataSource TEXT,' +
  'spVersion TEXT,' +
  'deviceTypeName TEXT,' +
  'brand TEXT,' +
  'deviceName TEXT,' +
  'board TEXT,' +
  'version TEXT,' +
  'plat TEXT,' +
  'cpuCluster TEXT,' +
  'sn TEXT,' +
  'resolution TEXT,' +
  'screenBrightness TEXT,' +
  'volume TEXT,' +
  'batteryVolume TEXT,' +
  'isOnline TEXT,' +
  'deviceNum TEXT,' +
  'roomNumber TEXT,' +
  'taskType TEXT' +
  ')';

export const sql_t_index_info =
  'CREATE TABLE IF NOT EXISTS ' +
  't_index_info' +
  '(' +
  'timestamp TEXT PRIMARY KEY, ' +
  'taskId TEXT NOT NULL, ' +
  'flag TEXT, ' +
  'shellBackTemp TEXT,' +
  'shellFrameTemp TEXT,' +
  'shellFrontTemp TEXT,' +
  'socThermalTemp TEXT,' +
  'systemHTemp TEXT,' +
  'gpuTemp TEXT,' +
  'ambientTemp TEXT,' +
  'batteryTemp TEXT,' +
  'ddrFrequency TEXT,' +
  'fps TEXT,' +
  'fpsJitters TEXT,' +
  'cpu0Frequency TEXT,' +
  'cpu0Load TEXT,' +
  'cpu1Frequency TEXT,' +
  'cpu1Load TEXT,' +
  'cpu2Frequency TEXT,' +
  'cpu2Load TEXT,' +
  'cpu3Frequency TEXT,' +
  'cpu3Load TEXT,' +
  'cpu4Frequency TEXT,' +
  'cpu4Load TEXT,' +
  'cpu5Frequency TEXT,' +
  'cpu5Load TEXT,' +
  'cpu6Frequency TEXT,' +
  'cpu6Load TEXT,' +
  'cpu7Frequency TEXT,' +
  'cpu7Load TEXT,' +
  'cpuLoad TEXT,' +
  'gpuFrequency TEXT,' +
  'gpuLoad TEXT,' +
  'currentNow TEXT,' +
  'capacity TEXT,' +
  'enableHiz TEXT,' +
  'status TEXT,' +
  'voltageNow TEXT,' +
  'pss TEXT,' +
  'networkUpSpeed TEXT,' +
  'networkDownSpeed TEXT' +
  ')';

export const task_powersensor_info =
  'CREATE TABLE IF NOT EXISTS ' +
  'task_powersensor_info' +
  '(' +
  'id TEXT PRIMARY KEY,' +
  'taskId TEXT NOT NULL,' +
  'sensor TEXT,' +
  'power TEXT,' +
  'current TEXT,' +
  'percent TEXT' +
  ')';

export const task_powerapp_info =
  'CREATE TABLE IF NOT EXISTS ' +
  'task_powerapp_info' +
  '(' +
  'id TEXT PRIMARY KEY,' +
  'taskId TEXT NOT NULL,' +
  'process TEXT,' +
  'energy TEXT,' +
  'percent TEXT' +
  ')';
