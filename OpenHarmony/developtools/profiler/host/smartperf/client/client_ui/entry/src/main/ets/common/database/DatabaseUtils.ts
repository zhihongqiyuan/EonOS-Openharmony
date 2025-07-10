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

import relationalStore from '@ohos.data.relationalStore';
import {
  GPData,
  TIndexInfo,
  TGeneralInfo,
  TPowerSensorInfo,
  TPowerAppInfo,
} from '../entity/DatabaseEntity';
import {
  sql_t_index_info,
  dbVersion,
  dbName,
  task_powersensor_info,
  task_powerapp_info,
} from '../constant/ConstantSQL';
import SPLogger from '../utils/SPLogger';

const TAG = 'DatabaseUtils';

export default {
  //创建表(T_INDEX_INFO T_GENERAL_INFO)
  async createTable(pathSuffix: number): Promise<void> {
    try {
      const STORE_CONFIG: relationalStore.StoreConfig = {
        name: pathSuffix + '.db',
        securityLevel: relationalStore.SecurityLevel.S1,
      };
      relationalStore
        .getRdbStore(globalThis.abilityContext, STORE_CONFIG)
        .then((rdbStore) => {
          return rdbStore;
        });
    } catch (err) {
      SPLogger.ERROR(TAG, 'createTable ERR:' + err);
    }
  },

  //插入表( T_GENERAL_INFO)
  insertGeneraData(tableName: string, tGeneralInfo: TGeneralInfo): void {
    let strMap = new Map();
    for (let k of Object.keys(tGeneralInfo)) {
      strMap.set(k, tGeneralInfo[k]);
    }
    const valueInsert = {
      sessionId: strMap.get('sessionId'),
      taskId: strMap.get('taskId'),
      appName: strMap.get('appName'),
      appVersion: strMap.get('appVersion'),
      packageName: strMap.get('packageName'),
      startTime: strMap.get('startTime'),
      endTime: strMap.get('endTime'),
      testDuration: strMap.get('testDuration'),
      taskName: strMap.get('taskName'),
    };
    try {
      const STORE_CONFIG: relationalStore.StoreConfig = {
        name: dbName,
        securityLevel: relationalStore.SecurityLevel.S1,
      };
      relationalStore.getRdbStore(
        globalThis.abilityContext,
        STORE_CONFIG,
        (err, rdbStore) => {
          SPLogger.DEBUG(
            TAG,
            '--> insert into  insertGeneraData :tableName:' +
              tableName +
              '| valueInsert:' +
            JSON.stringify(valueInsert)
          );
          rdbStore.insert(tableName, valueInsert);
        }
      );
    } catch (err) {
      SPLogger.ERROR(TAG, 'insertGeneraData ERR:' + err);
    }
  },

  //插入表(T_INDEX_INFO)
  insertData(tableName: string, pathSuffix: number, tIndexInfo: TIndexInfo): void {
    let strMap = new Map();
    for (let k of Object.keys(tIndexInfo)) {
      strMap.set(k, tIndexInfo[k]);
    }

    const valueInsert = {
      timestamp: strMap.get('timestamp'),
      taskId: strMap.get('taskId'),
      ddrFrequency: strMap.get('ddrFrequency'),
      cpu0Frequency: strMap.get('cpu0Frequency'),
      cpu1Frequency: strMap.get('cpu1Frequency'),
      cpu2Frequency: strMap.get('cpu2Frequency'),
      cpu3Frequency: strMap.get('cpu3Frequency'),
      cpu4Frequency: strMap.get('cpu4Frequency'),
      cpu5Frequency: strMap.get('cpu5Frequency'),
      cpu6Frequency: strMap.get('cpu6Frequency'),
      cpu7Frequency: strMap.get('cpu7Frequency'),
      cpu0Load: strMap.get('cpu0Load'),
      cpu1Load: strMap.get('cpu1Load'),
      cpu2Load: strMap.get('cpu2Load'),
      cpu3Load: strMap.get('cpu3Load'),
      cpu4Load: strMap.get('cpu4Load'),
      cpu5Load: strMap.get('cpu5Load'),
      cpu6Load: strMap.get('cpu6Load'),
      cpu7Load: strMap.get('cpu7Load'),
      gpuFrequency: strMap.get('gpuFrequency'),
      gpuLoad: strMap.get('gpuLoad'),
      currentNow: strMap.get('currentNow'),
      voltageNow: strMap.get('voltageNow'),
      shellFrameTemp: strMap.get('shellFrameTemp'),
      shellFrontTemp: strMap.get('shellFrontTemp'),
      shellBackTemp: strMap.get('shellBackTemp'),
      socThermalTemp: strMap.get('socThermalTemp'),
      systemHTemp: strMap.get('systemHTemp'),
      gpuTemp: strMap.get('gpuTemp'),
      ambientTemp: strMap.get('ambientTemp'),
      batteryTemp: strMap.get('batteryTemp'),
      pss: strMap.get('pss'),
      fps: strMap.get('fps'),
      fpsJitters: strMap.get('fpsJitters'),
      networkUpSpeed: strMap.get('networkUpSpeed'),
      networkDownSpeed: strMap.get('networkDownSpeed'),
    };
    try {
      const STORE_CONFIG: relationalStore.StoreConfig = {
        name: pathSuffix + '.db',
        securityLevel: relationalStore.SecurityLevel.S1,
      };
      relationalStore.getRdbStore(
        globalThis.abilityContext,
        STORE_CONFIG,
        (err, rdbStore) => {
          rdbStore.insert(tableName, valueInsert);
        }
      );
    } catch (err) {
      SPLogger.ERROR(TAG, 'insertGeneraData ERR:' + err);
    }
  },

  //查询表( T_GENERAL_INFO)
  async queryGeneralData(): Promise<Array<TGeneralInfo>> {
    const STORE_CONFIG: relationalStore.StoreConfig = {
      name: dbName,
      securityLevel: relationalStore.SecurityLevel.S1,
    };
    let results = Array<TGeneralInfo>();
    try {
      return await relationalStore
        .getRdbStore(globalThis.abilityContext, STORE_CONFIG)
        .then((rdbStore) => {
          let predicates = new relationalStore.RdbPredicates('t_general_info');
          predicates.orderByDesc('startTime');
          return rdbStore.query(predicates, [
            'sessionId',
            'startTime',
            'appName',
            'appVersion',
            'packageName',
            'endTime',
            'testDuration',
            'taskName',
            'board',
            'deviceTypeName',
            'brand',
            'version',
            'sn',
            'taskId',
          ]);
        })
        .then((resultSet) => {
          while (resultSet.goToNextRow()) {
            let sessionId = resultSet.getString(
              resultSet.getColumnIndex('sessionId')
            );
            let taskId = resultSet.getString(
              resultSet.getColumnIndex('taskId')
            );
            let appName = resultSet.getString(
              resultSet.getColumnIndex('appName')
            );
            let appVersion = resultSet.getString(
              resultSet.getColumnIndex('appVersion')
            );
            let packageName = resultSet.getString(
              resultSet.getColumnIndex('packageName')
            );
            let startTime = resultSet.getString(
              resultSet.getColumnIndex('startTime')
            );
            let endTime = resultSet.getString(
              resultSet.getColumnIndex('endTime')
            );
            let testDuration = resultSet.getLong(
              resultSet.getColumnIndex('testDuration')
            );
            let taskName = resultSet.getString(
              resultSet.getColumnIndex('taskName')
            );
            let board = resultSet.getString(resultSet.getColumnIndex('board'));
            let deviceTypeName = resultSet.getString(
              resultSet.getColumnIndex('deviceTypeName')
            );
            let brand = resultSet.getString(resultSet.getColumnIndex('brand'));
            let version = resultSet.getString(
              resultSet.getColumnIndex('version')
            );
            let sn = resultSet.getString(resultSet.getColumnIndex('sn'));

            let tGeneralInfo = new TGeneralInfo(
              sessionId,
              taskId,
              appName,
              appVersion,
              packageName,
              Number(startTime).valueOf(),
              Number(endTime).valueOf(),
              testDuration,
              taskName
            );
            tGeneralInfo.board = board;
            tGeneralInfo.deviceTypeName = deviceTypeName;
            tGeneralInfo.brand = brand;
            tGeneralInfo.version = version;
            tGeneralInfo.sn = sn;
            results.push(tGeneralInfo);
          }
          return results;
        });
    } catch (err) {
      SPLogger.ERROR(TAG, 'resultSet queryGeneralData:err' + err);
      return results;
    }
  },
  //查询表( T_INDEX_INFO) 2022-02-23 改为传时间戳
  async queryData(dbPath: string): Promise<Array<TIndexInfo>> {
    const STORE_CONFIG: relationalStore.StoreConfig = {
      name: dbPath,
      securityLevel: relationalStore.SecurityLevel.S1,
    };
    let results: TIndexInfo[] = [];
    try {
      return await relationalStore
        .getRdbStore(globalThis.abilityContext, STORE_CONFIG)
        .then((rdbStore) => {
          let predicates = new relationalStore.RdbPredicates('t_index_info');
          predicates.orderByDesc('timestamp');
          return rdbStore.query(predicates, [
            'timestamp',
            'taskId',
            'shellBackTemp',
            'shellFrameTemp',
            'shellFrontTemp',
            'socThermalTemp',
            'systemHTemp',
            'gpuTemp',
            'ambientTemp',
            'batteryTemp',
            'ddrFrequency',
            'cpu0Frequency',
            'cpu1Frequency',
            'cpu2Frequency',
            'cpu3Frequency',
            'cpu4Frequency',
            'cpu5Frequency',
            'cpu6Frequency',
            'cpu7Frequency',
            'cpu0Load',
            'cpu1Load',
            'cpu2Load',
            'cpu3Load',
            'cpu4Load',
            'cpu5Load',
            'cpu6Load',
            'cpu7Load',
            'gpuLoad',
            'gpuFrequency',
            'currentNow',
            'voltageNow',
            'pss',
            'fps',
            'fpsJitters',
            'networkUpSpeed',
            'networkDownSpeed',
          ]);
        })
        .then((resultSet) => {
          while (resultSet.goToNextRow()) {
            let timestamp = resultSet.getString(
              resultSet.getColumnIndex('timestamp')
            );
            let shellBackTemp = resultSet.getString(
              resultSet.getColumnIndex('shellBackTemp')
            );
            let shellFrameTemp = resultSet.getString(
              resultSet.getColumnIndex('shellFrameTemp')
            );
            let shellFrontTemp = resultSet.getString(
              resultSet.getColumnIndex('shellFrontTemp')
            );
            let socThermalTemp = resultSet.getString(
              resultSet.getColumnIndex('socThermalTemp')
            );
            let systemHTemp = resultSet.getString(
              resultSet.getColumnIndex('systemHTemp')
            );
            let gpuTemp = resultSet.getString(
              resultSet.getColumnIndex('gpuTemp')
            );
            let ambientTemp = resultSet.getString(
              resultSet.getColumnIndex('ambientTemp')
            );
            let batteryTemp = resultSet.getString(
              resultSet.getColumnIndex('batteryTemp')
            );
            let ddrFrequency = resultSet.getString(
              resultSet.getColumnIndex('ddrFrequency')
            );
            let cpu0Frequency = resultSet.getString(
              resultSet.getColumnIndex('cpu0Frequency')
            );
            let cpu1Frequency = resultSet.getString(
              resultSet.getColumnIndex('cpu1Frequency')
            );
            let cpu2Frequency = resultSet.getString(
              resultSet.getColumnIndex('cpu2Frequency')
            );
            let cpu3Frequency = resultSet.getString(
              resultSet.getColumnIndex('cpu3Frequency')
            );
            let cpu4Frequency = resultSet.getString(
              resultSet.getColumnIndex('cpu4Frequency')
            );
            let cpu5Frequency = resultSet.getString(
              resultSet.getColumnIndex('cpu5Frequency')
            );
            let cpu6Frequency = resultSet.getString(
              resultSet.getColumnIndex('cpu6Frequency')
            );
            let cpu7Frequency = resultSet.getString(
              resultSet.getColumnIndex('cpu7Frequency')
            );

            let cpu0Load = resultSet.getString(
              resultSet.getColumnIndex('cpu0Load')
            );
            let cpu1Load = resultSet.getString(
              resultSet.getColumnIndex('cpu1Load')
            );
            let cpu2Load = resultSet.getString(
              resultSet.getColumnIndex('cpu2Load')
            );
            let cpu3Load = resultSet.getString(
              resultSet.getColumnIndex('cpu3Load')
            );
            let cpu4Load = resultSet.getString(
              resultSet.getColumnIndex('cpu4Load')
            );
            let cpu5Load = resultSet.getString(
              resultSet.getColumnIndex('cpu5Load')
            );
            let cpu6Load = resultSet.getString(
              resultSet.getColumnIndex('cpu6Load')
            );
            let cpu7Load = resultSet.getString(
              resultSet.getColumnIndex('cpu7Load')
            );

            let gpuLoad = resultSet.getString(
              resultSet.getColumnIndex('gpuLoad')
            );
            let gpuFrequency = resultSet.getString(
              resultSet.getColumnIndex('gpuFrequency')
            );
            let currentNow = resultSet.getString(
              resultSet.getColumnIndex('currentNow')
            );
            let voltageNow = resultSet.getString(
              resultSet.getColumnIndex('voltageNow')
            );

            let pss = resultSet.getString(resultSet.getColumnIndex('pss'));
            let fps = resultSet.getString(resultSet.getColumnIndex('fps'));
            let fpsJitters = resultSet.getString(
              resultSet.getColumnIndex('fpsJitters')
            );

            let networkUpSpeed = resultSet.getString(
              resultSet.getColumnIndex('networkUpSpeed')
            );
            let networkDownSpeed = resultSet.getString(
              resultSet.getColumnIndex('networkDownSpeed')
            );

            results.push(
              new TIndexInfo(
                timestamp,
                '18',
                cpu0Frequency,
                cpu1Frequency,
                cpu2Frequency,
                cpu3Frequency,
                cpu4Frequency,
                cpu5Frequency,
                cpu6Frequency,
                cpu7Frequency,
                cpu0Load,
                cpu1Load,
                cpu2Load,
                cpu3Load,
                cpu4Load,
                cpu5Load,
                cpu6Load,
                cpu7Load,
                gpuFrequency,
                gpuLoad,
                ddrFrequency,
                shellFrameTemp,
                shellFrontTemp,
                shellBackTemp,
                socThermalTemp,
                systemHTemp,
                gpuTemp,
                ambientTemp,
                batteryTemp,
                currentNow,
                voltageNow,
                pss,
                fps,
                fpsJitters,
                networkUpSpeed,
                networkDownSpeed
              )
            );
            console.log(
              'resultSet column names:results:index:' + resultSet.rowIndex
            );
          }
          console.log(
            'resultSet column names:results length:' + results.length
          );
          console.log(
            'resultSet column names:results:' + JSON.stringify(results)
          );
          return results;
        });
    } catch (err) {
      SPLogger.ERROR(TAG, 'resultSet queryIndexInfo Data:err' + err);
      return results;
    }
  },
  /**
   * Array<GPData> 封装为TIndexInfo
   * @param gpDatas
   */
  gpArray2Index(gpDatas: Array<GPData>): TIndexInfo {
    let tIndexInfo: TIndexInfo = new TIndexInfo();
    tIndexInfo.setTaskId('18');
    tIndexInfo.setTimeStamp(new Date().getTime().toString());
    if (gpDatas != null) {
      for (let index = 0; index < gpDatas.length; index++) {
        let curGPData: GPData = gpDatas[index];
        let map = curGPData.values;

        switch (curGPData.moduleType) {
          case 'CPU':
            tIndexInfo.setCPUData(
              map.get('cpu0Freq'),
              map.get('cpu1Freq'),
              map.get('cpu2Freq'),
              map.get('cpu3Freq'),
              map.get('cpu4Freq'),
              map.get('cpu5Freq'),
              map.get('cpu6Freq'),
              map.get('cpu7Freq')
            );
            break;
          case 'CPULoad':
            tIndexInfo.setCPULoadData(
              map.get('cpu0Load'),
              map.get('cpu1Load'),
              map.get('cpu2Load'),
              map.get('cpu3Load'),
              map.get('cpu4Load'),
              map.get('cpu5Load'),
              map.get('cpu6Load'),
              map.get('cpu7Load')
            );
            break;
          case 'GPU':
            tIndexInfo.setGPUData(map.get('gpuFreq'), map.get('gpuLoad'));
            break;
          case 'DDR':
            tIndexInfo.setDDRData(map.get('ddrFreq'));
            break;
          case 'Temp':
            tIndexInfo.setTempData(
              map.get('shell_frame'),
              map.get('shell_front'),
              map.get('shell_back'),
              map.get('soc-thermal'),
              map.get('system_h'),
              map.get('gpu-thermal'),
              map.get('ambient')
            );
            break;
          case 'Power':
            tIndexInfo.setPowerData(
              map.get('current_now'),
              map.get('voltage_now'),
              map.get('temp')
            );
            break;
          case 'RAM':
            tIndexInfo.setRamData(map.get('pss'));
            break;
          case 'FPS':
            tIndexInfo.setFpsData(map.get('fps'), map.get('fpsJitters'));
            break;
          case 'NetWork':
            tIndexInfo.setNetWorkData(
              map.get('netSpeedUp'),
              map.get('netSpeedDown')
            );
            break;
        }
      }
    }

    return tIndexInfo;
  },

  //插入表(task_powersensor_info)
  insertPowerSensor(
    tableName: string,
    pathSuffix: number,
    tPowerSenspor: TPowerSensorInfo
  ): void {
    SPLogger.INFO(
      'TAG',
      'resultSet query_applications_display-----tPowerSenspor' +
      JSON.stringify(tPowerSenspor)
    );
    let strMap = new Map();
    for (let k of Object.keys(tPowerSenspor)) {
      strMap.set(k, tPowerSenspor[k]);
    }
    const valueInsert = {
      timestamp: strMap.get('timestamp'),
      taskId: strMap.get('taskId'),
      sensor: strMap.get('sensor'),
      power: strMap.get('power'),
      current: strMap.get('current'),
      percent: strMap.get('percent'),
    };
    try {
      const STORE_CONFIG: relationalStore.StoreConfig = {
        name: pathSuffix + '.db',
        securityLevel: relationalStore.SecurityLevel.S1,
      };
      relationalStore.getRdbStore(
        globalThis.abilityContext,
        STORE_CONFIG,
        (err, rdbStore) => {
          SPLogger.INFO(
            'TAG',
            'resultSet query_applications_display-----tPowerSenspor' +
            JSON.stringify(pathSuffix)
          );
          rdbStore.insert(tableName, valueInsert);
        }
      );
    } catch (err) {
      SPLogger.ERROR(TAG, 'resultSet insertPowerSensor Data:err' + err);
    }
  },

  //插入表(task_powerapp_info)
  insertPowerAppInfo(
    tableName: string,
    pathSuffix: number,
    tPowerApp: TPowerAppInfo
  ): void {
    SPLogger.INFO(
      'TAG',
      'resultSet query_applications_display-----tPowerApp' +
      JSON.stringify(tPowerApp)
    );
    let strMap = new Map();
    for (let k of Object.keys(tPowerApp)) {
      strMap.set(k, tPowerApp[k]);
    }
    const valueInsert = {
      taskId: '',
      process: strMap.get('application'),
      energy: strMap.get('power'),
      percent: strMap.get('percent'),
    };
    try {
      const STORE_CONFIG: relationalStore.StoreConfig = {
        name: pathSuffix + '.db',
        securityLevel: relationalStore.SecurityLevel.S1,
      };
      relationalStore.getRdbStore(
        globalThis.abilityContext,
        STORE_CONFIG,
        (err, rdbStore) => {
          SPLogger.INFO(
            'TAG',
            'resultSet query_applications_display-----tPowerApp' +
            JSON.stringify(pathSuffix)
          );
          rdbStore.insert(tableName, valueInsert);
        }
      );
    } catch (err) {
      SPLogger.ERROR(TAG, 'resultSet insertPowerAppInfo Data:err' + err);
    }
  },

  //查询表（detailed_applications_display）
  async query_applications_display(
    start_time: String,
    end_time: String,
    pkg_name: String
  ): Promise<Array<TPowerSensorInfo>> {
    const STORE_CONFIG: relationalStore.StoreConfig = {
      name: 'dubai.db',
      securityLevel: relationalStore.SecurityLevel.S1,
    };
    let results: TPowerSensorInfo[] = [];
    try {
      return await relationalStore
        .getRdbStore(globalThis.abilityContext, STORE_CONFIG)
        .then(async (rdbStore) => {
          let strSQL: string =
            'select sum(energy)/3600.0 power,sum(energy)/((max(end_time)-min(start_time))/1000.0) current ' +
              'from detailed_applications_display ' +
              'where formatted_start_time >= ' +
            JSON.stringify(start_time) +
              ' and formatted_end_time <= ' +
            JSON.stringify(end_time) +
              ' and name = ' +
            JSON.stringify(pkg_name);
          SPLogger.INFO(
            'TAG',
            'resultSet query_applications_display-----display' +
            JSON.stringify(strSQL)
          );
          return rdbStore.querySql(strSQL);
        })
        .then((resultSet) => {
          while (resultSet.goToNextRow()) {
            let sensor = 'display';
            let power = resultSet.getString(resultSet.getColumnIndex('power'));
            let current = resultSet.getString(
              resultSet.getColumnIndex('current')
            );
            let tPowerSensorInfo = new TPowerSensorInfo(
              '',
              sensor,
              power,
              current,
              ''
            );
            results.push(tPowerSensorInfo);
          }
          SPLogger.INFO(
            'TAG',
            'resultSet query_applications_display-----display' +
            JSON.stringify(results)
          );
          return results;
        });
    } catch (err) {
      SPLogger.ERROR(
        TAG,
        'resultSet query_applications_display err22222:' + err
      );
      return results;
    }
  },
  //查询表（task_powersensor_info）
  async query_powersensor_info(
    pathSuffix: number
  ): Promise<Array<TPowerSensorInfo>> {
    const STORE_CONFIG: relationalStore.StoreConfig = {
      name: pathSuffix + '.db',
      securityLevel: relationalStore.SecurityLevel.S1,
    };
    let results: TPowerSensorInfo[] = [];
    try {
      return await relationalStore
        .getRdbStore(globalThis.abilityContext, STORE_CONFIG)
        .then(async (rdbStore) => {
          let strSQL: string =
            'select * ' + 'from task_powersensor_info order by power desc  ';

          return rdbStore.querySql(strSQL);
        })
        .then((resultSet) => {
          while (resultSet.goToNextRow()) {
            let sensor = resultSet.getString(
              resultSet.getColumnIndex('sensor')
            );
            let power = resultSet.getString(resultSet.getColumnIndex('power'));
            let current = resultSet.getString(
              resultSet.getColumnIndex('current')
            );
            let tPowerSensorInfo = new TPowerSensorInfo(
              '',
              sensor,
              power,
              current,
              ''
            );
            results.push(tPowerSensorInfo);
          }
          return results;
        });
    } catch (err) {
      SPLogger.ERROR(
        TAG,
        'resultSet query_applications_display err22222:' + err
      );
      return results;
    }
  },
  //查询表（detailed_applications_cpu）
  async query_applications_cpu(
    start_time: String,
    end_time: String,
    pkg_name: String
  ): Promise<Array<TPowerSensorInfo>> {
    const STORE_CONFIG: relationalStore.StoreConfig = {
      name: 'dubai.db',
      securityLevel: relationalStore.SecurityLevel.S1,
    };
    let results: TPowerSensorInfo[] = [];
    try {
      return await relationalStore
        .getRdbStore(globalThis.abilityContext, STORE_CONFIG)
        .then(async (rdbStore) => {
          let strSQL: string =
            'select (sum(foreground_energy)+sum(background_energy))/3600.0 power, (sum(foreground_energy)+sum(background_energy))/((max(end_time)-min(start_time))/1000.0) current ' +
              'from detailed_applications_cpu ' +
              'where formatted_start_time >= ' +
            JSON.stringify(start_time) +
              ' and formatted_end_time <= ' +
            JSON.stringify(end_time) +
              ' and name = ' +
            JSON.stringify(pkg_name);

          return rdbStore.querySql(strSQL);
        })
        .then((resultSet) => {
          while (resultSet.goToNextRow()) {
            let sensor = 'cpu';
            let power = resultSet.getString(resultSet.getColumnIndex('power'));
            let current = resultSet.getString(
              resultSet.getColumnIndex('current')
            );
            let tPowerSensorInfo = new TPowerSensorInfo('', sensor, power, current, '');
            results.push(tPowerSensorInfo);
          }
          return results;
        });
    } catch (err) {
      return results;
    }
  },
  //查询表（detailed_applications_gpu）
  async query_applications_gpu(
    start_time: String,
    end_time: String,
    pkg_name: String
  ): Promise<Array<TPowerSensorInfo>> {
    const STORE_CONFIG: relationalStore.StoreConfig = {
      name: 'dubai.db',
      securityLevel: relationalStore.SecurityLevel.S1,
    };
    let results: TPowerSensorInfo[] = [];
    try {
      return await relationalStore
        .getRdbStore(globalThis.abilityContext, STORE_CONFIG)
        .then(async (rdbStore) => {
          let strSQL: string =
            'select sum(energy)/3600.0 power,sum(energy)/((max(end_time)-min(start_time))/1000.0) current ' +
              'from detailed_applications_gpu ' +
              'where formatted_start_time >= ' +
            JSON.stringify(start_time) +
              ' and formatted_end_time <= ' +
            JSON.stringify(end_time) +
              ' and name = ' +
            JSON.stringify(pkg_name);

          return rdbStore.querySql(strSQL);
        })
        .then((resultSet) => {
          while (resultSet.goToNextRow()) {
            let sensor = 'gpu';
            let power = resultSet.getString(resultSet.getColumnIndex('power'));
            let current = resultSet.getString(
              resultSet.getColumnIndex('current')
            );
            let tPowerSensorInfo = new TPowerSensorInfo('', sensor, power, current, '');
            results.push(tPowerSensorInfo);
          }
          return results;
        });
    } catch (err) {
      return results;
    }
  },
  //查询表（detailed_applications_wifi_data）
  async query_applications_wifi_data(
    start_time: String,
    end_time: String,
    pkg_name: String
  ): Promise<Array<TPowerSensorInfo>> {
    const STORE_CONFIG: relationalStore.StoreConfig = {
      name: 'dubai.db',
      securityLevel: relationalStore.SecurityLevel.S1,
    };
    let results: TPowerSensorInfo[] = [];
    try {
      return await relationalStore
        .getRdbStore(globalThis.abilityContext, STORE_CONFIG)
        .then(async (rdbStore) => {
          let strSQL: string =
            'select (sum(foreground_energy)+sum(background_energy))/3600.0 power, (sum(foreground_energy)+sum(background_energy))/((max(end_time)-min(start_time))/1000.0) current ' +
              'from detailed_applications_wifi_data ' +
              'where formatted_start_time >= ' +
            JSON.stringify(start_time) +
              ' and formatted_end_time <= ' +
            JSON.stringify(end_time) +
              ' and name = ' +
            JSON.stringify(pkg_name);
          return rdbStore.querySql(strSQL);
        })
        .then((resultSet) => {
          while (resultSet.goToNextRow()) {
            let sensor = 'wifi_data';
            let power = resultSet.getString(resultSet.getColumnIndex('power'));
            let current = resultSet.getString(
              resultSet.getColumnIndex('current')
            );
            let tPowerSensorInfo = new TPowerSensorInfo('', sensor, power, current, ''
            );
            results.push(tPowerSensorInfo);
          }
          return results;
        });
    } catch (err) {
      return results;
    }
  },
  //查询表（detailed_applications_system_idle）
  async query_applications_system_idle(
    start_time: String,
    end_time: String,
    pkg_name: String
  ): Promise<Array<TPowerSensorInfo>> {
    const STORE_CONFIG: relationalStore.StoreConfig = {
      name: 'dubai.db',
      securityLevel: relationalStore.SecurityLevel.S1,
    };
    let results: TPowerSensorInfo[] = [];
    try {
      return await relationalStore
        .getRdbStore(globalThis.abilityContext, STORE_CONFIG)
        .then(async (rdbStore) => {
          let strSQL: string =
            'select sum(energy)/3600.0 power,sum(energy)/((max(end_time)-min(start_time))/1000.0) current ' +
              'from detailed_applications_system_idle ' +
              'where formatted_start_time >= ' +
            JSON.stringify(start_time) +
              ' and formatted_end_time <= ' +
            JSON.stringify(end_time) +
              ' and name = ' +
            JSON.stringify(pkg_name);
          return rdbStore.querySql(strSQL);
        })
        .then((resultSet) => {
          while (resultSet.goToNextRow()) {
            let sensor = 'system_idle';
            let power = resultSet.getString(resultSet.getColumnIndex('power'));
            let current = resultSet.getString(
              resultSet.getColumnIndex('current')
            );
            let tPowerSensorInfo = new TPowerSensorInfo('', sensor, power, current, '');
            results.push(tPowerSensorInfo);
          }
          return results;
        });
    } catch (err) {
      return results;
    }
  },
  //查询表（detailed_applications_audio）
  async query_applications_audio(
    start_time: String,
    end_time: String,
    pkg_name: String
  ): Promise<Array<TPowerSensorInfo>> {
    const STORE_CONFIG: relationalStore.StoreConfig = {
      name: 'dubai.db',
      securityLevel: relationalStore.SecurityLevel.S1,
    };
    let results: TPowerSensorInfo[] = [];
    try {
      return await relationalStore
        .getRdbStore(globalThis.abilityContext, STORE_CONFIG)
        .then(async (rdbStore) => {
          let strSQL: string =
            'select (sum(foreground_energy)+sum(background_energy))/3600.0 power, (sum(foreground_energy)+sum(background_energy))/((max(end_time)-min(start_time))/1000.0) current ' +
              'from detailed_applications_audio ' +
              'where formatted_start_time >= ' +
            JSON.stringify(start_time) +
              ' and formatted_end_time <= ' +
            JSON.stringify(end_time) +
              ' and name = ' +
            JSON.stringify(pkg_name);
          return rdbStore.querySql(strSQL);
        })
        .then((resultSet) => {
          while (resultSet.goToNextRow()) {
            let sensor = 'audio';
            let power = resultSet.getString(resultSet.getColumnIndex('power'));
            let current = resultSet.getString(
              resultSet.getColumnIndex('current')
            );
            let tPowerSensorInfo = new TPowerSensorInfo('', sensor, power, current, '');
            results.push(tPowerSensorInfo);
          }
          return results;
        });
    } catch (err) {
      return results;
    }
  },
  //查询表（detailed_applications_dss）
  async query_applications_dss(
    start_time: String,
    end_time: String,
    pkg_name: String
  ): Promise<Array<TPowerSensorInfo>> {
    const STORE_CONFIG: relationalStore.StoreConfig = {
      name: 'dubai.db',
      securityLevel: relationalStore.SecurityLevel.S1,
    };
    let results: TPowerSensorInfo[] = [];
    try {
      return await relationalStore
        .getRdbStore(globalThis.abilityContext, STORE_CONFIG)
        .then(async (rdbStore) => {
          let strSQL: string =
            'select sum(foreground_energy)+sum(background_energy))/3600.0 power, (sum(foreground_energy)+sum(background_energy))/((max(end_time)-min(start_time))/1000.0) current ' +
              'from detailed_applications_dss ' +
              'where formatted_start_time >= ' +
            JSON.stringify(start_time) +
              ' and formatted_end_time <= ' +
            JSON.stringify(end_time) +
              ' and name = ' +
            JSON.stringify(pkg_name);
          return rdbStore.querySql(strSQL);
        })
        .then((resultSet) => {
          while (resultSet.goToNextRow()) {
            let sensor = 'dss';
            let power = resultSet.getString(resultSet.getColumnIndex('power'));
            let current = resultSet.getString(
              resultSet.getColumnIndex('current')
            );
            let tPowerSensorInfo = new TPowerSensorInfo('', sensor, power, current, '');
            results.push(tPowerSensorInfo);
          }
          return results;
        });
    } catch (err) {
      return results;
    }
  },
  //查询表（detailed_applications_ddr）
  async query_applications_ddr(
    start_time: String,
    end_time: String,
    pkg_name: String
  ): Promise<Array<TPowerSensorInfo>> {
    const STORE_CONFIG: relationalStore.StoreConfig = {
      name: 'dubai.db',
      securityLevel: relationalStore.SecurityLevel.S1,
    };
    let results: TPowerSensorInfo[] = [];
    try {
      return await relationalStore
        .getRdbStore(globalThis.abilityContext, STORE_CONFIG)
        .then(async (rdbStore) => {
          let strSQL: string =
            'select (sum(foreground_energy)+sum(background_energy))/3600.0 power, (sum(foreground_energy)+sum(background_energy))/((max(end_time)-min(start_time))/1000.0) current ' +
              'from detailed_applications_ddr ' +
              'where formatted_start_time >= ' +
            JSON.stringify(start_time) +
              ' and formatted_end_time <= ' +
            JSON.stringify(end_time) +
              ' and name = ' +
            JSON.stringify(pkg_name);
          return rdbStore.querySql(strSQL);
        })
        .then((resultSet) => {
          while (resultSet.goToNextRow()) {
            let sensor = 'ddr';
            let power = resultSet.getString(resultSet.getColumnIndex('power'));
            let current = resultSet.getString(
              resultSet.getColumnIndex('current')
            );
            let tPowerSensorInfo = new TPowerSensorInfo('', sensor, power, current, '');
            results.push(tPowerSensorInfo);
          }
          return results;
        });
    } catch (err) {
      return results;
    }
  }, //查询表（detailed_applications_sensor）
  async query_applications_sensor(
    start_time: String,
    end_time: String,
    pkg_name: String
  ): Promise<Array<TPowerSensorInfo>> {
    const STORE_CONFIG: relationalStore.StoreConfig = {
      name: 'dubai.db',
      securityLevel: relationalStore.SecurityLevel.S1,
    };
    let results: TPowerSensorInfo[] = [];
    try {
      return await relationalStore
        .getRdbStore(globalThis.abilityContext, STORE_CONFIG)
        .then(async (rdbStore) => {
          let strSQL: string =
            'select (sum(foreground_energy)+sum(background_energy))/3600.0 power, (sum(foreground_energy)+sum(background_energy))/((max(end_time)-min(start_time))/1000.0) current ' +
              'from detailed_applications_sensor ' +
              'where formatted_start_time >= ' +
            JSON.stringify(start_time) +
              ' and formatted_end_time <= ' +
            JSON.stringify(end_time) +
              ' and name = ' +
            JSON.stringify(pkg_name);
          return rdbStore.querySql(strSQL);
        })
        .then((resultSet) => {
          while (resultSet.goToNextRow()) {
            let sensor = 'sensor';
            let power = resultSet.getString(resultSet.getColumnIndex('power'));
            let current = resultSet.getString(
              resultSet.getColumnIndex('current')
            );
            let tPowerSensorInfo = new TPowerSensorInfo('', sensor, power, current, ''
            );
            results.push(tPowerSensorInfo);
          }
          return results;
        });
    } catch (err) {
      return results;
    }
  },
  //查询表（detailed_applications_rom）
  async query_applications_rom(
    start_time: String,
    end_time: String,
    pkg_name: String
  ): Promise<Array<TPowerSensorInfo>> {
    const STORE_CONFIG: relationalStore.StoreConfig = {
      name: 'dubai.db',
      securityLevel: relationalStore.SecurityLevel.S1,
    };
    let results: TPowerSensorInfo[] = [];
    try {
      return await relationalStore
        .getRdbStore(globalThis.abilityContext, STORE_CONFIG)
        .then(async (rdbStore) => {
          let strSQL: string =
            'select (sum(foreground_energy)+sum(background_energy))/3600.0 power, (sum(foreground_energy)+sum(background_energy))/((max(end_time)-min(start_time))/1000.0) current ' +
              'from detailed_applications_rom ' +
              'where formatted_start_time >= ' +
            JSON.stringify(start_time) +
              ' and formatted_end_time <= ' +
            JSON.stringify(end_time) +
              ' and name = ' +
            JSON.stringify(pkg_name);
          return rdbStore.querySql(strSQL);
        })
        .then((resultSet) => {
          while (resultSet.goToNextRow()) {
            let sensor = 'rom';
            let power = resultSet.getString(resultSet.getColumnIndex('power'));
            let current = resultSet.getString(
              resultSet.getColumnIndex('current')
            );
            let tPowerSensorInfo = new TPowerSensorInfo('', sensor, power, current, '');
            results.push(tPowerSensorInfo);
          }
          return results;
        });
    } catch (err) {
      return results;
    }
  },
  //查询dubai 所有进程功耗 并插入表 t_power_appinfo
  async query_applications_power_info(
    start_time: string,
    end_time: string
  ): Promise<Array<TPowerAppInfo>> {
    const STORE_CONFIG: relationalStore.StoreConfig = {
      name: 'dubai.db',
      securityLevel: relationalStore.SecurityLevel.S1,
    };
    let results: TPowerAppInfo[] = [];
    let strSQL: String = '';
    try {
      return await relationalStore
        .getRdbStore(globalThis.abilityContext, STORE_CONFIG)
        .then(async (rdbStore) => {
          let sqlWhere =
            " where formatted_start_time >= '{startTime}'  and formatted_end_time <= '{endTime}' ";
          let sqlWhereReplace = sqlWhere
            .replace('{startTime}', start_time)
            .replace('{endTime}', end_time);
          SPLogger.ERROR(
            'TAG',
            'resultSet query_applications_display-----sqlWhereReplace:' +
              sqlWhereReplace
          );
          strSQL =
            'select formatted_start_time, formatted_end_time, name, round((foreground_energy+background_energy)/3600.0,5) as energy ' +
              'from detailed_applications_cpu ' +
              sqlWhereReplace +
              'union all ' +
              'select formatted_start_time, formatted_end_time, name,round(energy/3600.0,5) as energy ' +
              'from detailed_applications_display ' +
              sqlWhereReplace +
              'union all ' +
              'select formatted_start_time, formatted_end_time, name,round(energy/3600.0,5) as energy ' +
              'from detailed_applications_gpu ' +
              sqlWhereReplace +
              'union all ' +
              'select formatted_start_time, formatted_end_time, name,round(energy/3600.0,5) as energy ' +
              'from detailed_applications_system_idle ' +
              sqlWhereReplace +
              'union all ' +
              'select formatted_start_time, formatted_end_time, name, round((foreground_energy+background_energy)/3600.0,5) as energy ' +
              'from detailed_applications_wifi_data ' +
              sqlWhereReplace +
              'union all ' +
              'select formatted_start_time, formatted_end_time, name, round((foreground_energy+background_energy)/3600.0,5) as energy ' +
              'from detailed_applications_sensor ' +
              sqlWhereReplace +
              'union all ' +
              'select formatted_start_time, formatted_end_time, name, round((foreground_energy+background_energy)/3600.0,5) as energy ' +
              'from detailed_applications_audio ' +
              sqlWhereReplace;

          return rdbStore.querySql(strSQL.toString());
        })
        .then((resultSet) => {
          SPLogger.ERROR(
            'TAG',
            'resultSet query_applications_display-----result L:' +
            resultSet.rowCount
          );
          let tMap: Map<String, String> = new Map();
          let totalSumEnergy: number = 0.0;

          while (resultSet.goToNextRow()) {
            let sum_energy = resultSet.getString(
              resultSet.getColumnIndex('energy')
            );
            totalSumEnergy = totalSumEnergy + Number(sum_energy);
            let name = resultSet.getString(resultSet.getColumnIndex('name'));
            let existElement = tMap.get(name);
            if (existElement !== undefined) {
              //存在则修改
              let newP: Number = Number(existElement) + Number(sum_energy);
              tMap.set(name, newP.toString());
            } else {
              tMap.set(name, sum_energy);
            }
          }
          //遍历去重相加后的arr
          for (let [name, power] of tMap) {
            let percent = (Number(power) * 100) / totalSumEnergy;
            let tPowerAppInfo = new TPowerAppInfo(
              '',
              '',
              name,
              power,
              '0',
              percent.toFixed(5)
            );
            SPLogger.ERROR(
              'TAG',
              'resultSet query_applications_display-----result0:' +
              JSON.stringify(tPowerAppInfo)
            );
            results.push(tPowerAppInfo);
          }
          return results
            .sort(
              (a, b) =>
              parseFloat(b.power.toString()) - parseFloat(a.power.toString())
            )
            .slice(0, 20);
        })
        .then((results) => {
          SPLogger.ERROR(
            'TAG',
            'resultSet query_applications_display-----result after:' +
            results.length
          );
          return results;
        });
    } catch (err) {
      SPLogger.ERROR(
        TAG,
        'resultSet query_applications_display err22222:' + err
      );
      return results;
    }
  },

  //查询 t_powerapp_info
  async query_powerapp_info(pathSuffix: number): Promise<Array<TPowerAppInfo>> {
    const STORE_CONFIG: relationalStore.StoreConfig = {
      name: pathSuffix + '.db',
      securityLevel: relationalStore.SecurityLevel.S1,
    };
    let results: TPowerAppInfo[] = [];
    try {
      return await relationalStore
        .getRdbStore(globalThis.abilityContext, STORE_CONFIG)
        .then(async (rdbStore) => {
          let strSQL: string =
            'select * ' + 'from task_powerapp_info order by energy desc  ';

          return rdbStore.querySql(strSQL);
        })
        .then((resultSet) => {
          while (resultSet.goToNextRow()) {
            let process = resultSet.getString(
              resultSet.getColumnIndex('process')
            );
            let energy = resultSet.getString(
              resultSet.getColumnIndex('energy')
            );
            let percent = resultSet.getString(
              resultSet.getColumnIndex('percent')
            );

            let tPowerAppInfo = new TPowerAppInfo(
              '',
              '',
              process,
              energy,
              '0',
              percent
            );
            results.push(tPowerAppInfo);
          }
          return results;
        });
    } catch (err) {
      SPLogger.ERROR(
        TAG,
        'resultSet query_applications_display query_powerapp_info:' + err
      );
      return results;
    }
  },
};
