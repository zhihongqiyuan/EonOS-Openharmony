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
import deviceInfo from '@ohos.deviceInfo';
import CommonEvent from '@ohos.commonEvent';
import database from '../database/DatabaseUtils';
import { CPU } from './item/CPU';
import { Power } from './item/Power';
import { dateFormat } from '../utils/TimeUtils';
import { TPowerAppInfo } from '../entity/DatabaseEntity';
import { csvGeneralInfo, csvTIndexInfo } from '../utils/CSVUtils';
import { createFilePath } from '../utils/IOUtils';
import CheckEmptyUtils from '../utils/CheckEmptyUtils';
import { GPData, TIndexInfo, TGeneralInfo, TPowerSensorInfo } from '../entity/DatabaseEntity';
import { ProfilerFactory } from './base/ProfilerFactory';
import { CollectorType } from './base/ProfilerConstant';
import SPLogger from '../../common/utils/SPLogger';
import { initDbIndex } from '../../common/database/LocalRepository';

export class ProfilerTask {
  private collectItems: Array<string> = [];
  private static instance: ProfilerTask;

  public static getInstance(): ProfilerTask {
    if (this.instance == null) {
      this.instance = new ProfilerTask();
    }
    return ProfilerTask.instance;
  }

  initModule() {
    SPLogger.DEBUG(ProfilerTask.name, 'ProfilerTask initModule configs: ' + JSON.stringify(globalThis.collectConfigs));
    const keys: any[] = Object.keys(globalThis.collectConfigs);
    for (var key of keys) {
      if (globalThis.collectConfigs[key]) {
        let typeCollect = ProfilerFactory.getProfilerByConfig(key.toString());
        if (typeCollect == null) {
          continue;
        } else {
          this.collectItems.push(typeCollect.init());
        }
      }
    }
  }

  getSupports(keys: string[]) {
    let supports: Map<string, Boolean> = new Map<string, Boolean>();
    for (var key of keys) {
      let typeCollect = ProfilerFactory.getProfilerByConfig(key);
      let isSupport = typeCollect.isSupport();
      supports.set(key.toString(), isSupport);
    }
    return supports;
  }

  taskInit() {
    SPLogger.INFO(ProfilerTask.name, 'ProfilerUtils taskInit call');
    var now = new Date();
    //数据库时间戳标记
    globalThis.dbTime = now.getTime();
    //开始时间
    globalThis.startTime = now.getTime();
    //结束时间
    globalThis.endTime = -1;
    //时间计数器
    globalThis.collectIntervalNum = -1;
    //入库数据
    globalThis.tTndexs = [];
    //实时数据
    globalThis.tTndex = new TIndexInfo();
    // ram队列
    globalThis.ramArr = [];
    // fps队列
    globalThis.fpsArr = [];
    // fpsJitter队列
    globalThis.fpsJitterArr = [];

    // fps队列
    globalThis.fpsArr = [];
    // power 电流队列
    globalThis.powerCurArr = [];

    // power 电压队列
    globalThis.powerVoltArr = [];
    globalThis.taskId = -1;
    initDbIndex();
    //初始化数据库  2022-02-23 dbName改为时间戳
    database.createTable(globalThis.dbTime);
    SPLogger.INFO(ProfilerTask.name, 'ProfilerUtils taskInit called');
  }

  taskStart() {
    var gpDataArr: GPData[] = [];
    this.collectItems.forEach((moduleName) => {
      let typeCollect = ProfilerFactory.getProfilerByConfig(moduleName.toString());
      if (typeCollect != null) {
        let gpData: GPData = typeCollect.readData();
        if (typeCollect instanceof CPU) {
          gpDataArr.push(typeCollect.readCPULoad());
        }
        gpDataArr.push(gpData);
      }
    });

    //防止cpuLoad -1
    if (globalThis.collectIntervalNum > 0) {
      let tTndex = database.gpArray2Index(gpDataArr);
      globalThis.tTndexs.push(tTndex);
      globalThis.tTndex = tTndex;
      CommonEvent.publish('event', { code: 0, data: JSON.stringify(tTndex) }, (err) => {});
    }
    globalThis.collectIntervalNum++;
  }

  taskSingleItemStart(collectorType: CollectorType) {
    let typeCollect = ProfilerFactory.getProfilerByConfig(collectorType.toString());
    if (typeCollect instanceof Power) {
      typeCollect.readFourTimesData();
    }
  }

  taskStop() {
    SPLogger.DEBUG(ProfilerTask.name, 'ProfilerTask taskStop call');
    // t_index_info  入库
    if (globalThis.tTndexs.length > 2) {
      for (var index = 2; index < globalThis.tTndexs.length; index++) {
        const tTndex = globalThis.tTndexs[index];
        database.insertData('t_index_info', globalThis.dbTime, tTndex);
      }
    }
    SPLogger.DEBUG(ProfilerTask.name, 'ProfilerTask insertData success');
    createFilePath(
      globalThis.abilityContext.getApplicationContext().filesDir + '/' + globalThis.dbTime + '/t_index_info.csv',
    csvTIndexInfo(globalThis.tTndexs)
    );
    SPLogger.DEBUG(ProfilerTask.name, 'ProfilerTask createFilePath index_info success');
    // t_general_info  入库
    globalThis.endTime = new Date().getTime();
    let tGeneralInfo = new TGeneralInfo(
    globalThis.dbTime.toString(),
      'NA',
    globalThis.appName,
    globalThis.appVersion,
    globalThis.packageName,
    globalThis.startTime,
    globalThis.endTime,
    globalThis.collectIntervalNum,
    globalThis.testTaskName
    );
    tGeneralInfo.board = deviceInfo.brand;
    tGeneralInfo.deviceTypeName = deviceInfo.productModel;
    tGeneralInfo.brand = deviceInfo.brand;
    tGeneralInfo.version = deviceInfo.displayVersion;
    tGeneralInfo.sn = deviceInfo.serial;
    SPLogger.DEBUG(ProfilerTask.name, 'ProfilerTask insertGeneraData');
    database.insertGeneraData('t_general_info', tGeneralInfo);
    SPLogger.DEBUG(ProfilerTask.name, 'ProfilerTask insertGeneraData success');
    createFilePath(
      globalThis.abilityContext.getApplicationContext().filesDir + '/' + globalThis.dbTime + '/t_general_info.csv',
    csvGeneralInfo(tGeneralInfo)
    );

    SPLogger.DEBUG(ProfilerTask.name, 'ProfilerTask taskStop called');
  }
  taskGetDubai() {
    let tPowers: TPowerSensorInfo[] = [];
    SPLogger.INFO(
      'TAG',
      'resultSet query_applications_display-----startTime' + JSON.stringify(dateFormat(globalThis.startTime))
    );
    SPLogger.INFO(
      'TAG',
      'resultSet query_applications_display-----endTime' + JSON.stringify(dateFormat(globalThis.endTime))
    );
    SPLogger.INFO('TAG', 'resultSet query_applications_display-----dbTime' + JSON.stringify(globalThis.dbTime));
    database
      .query_applications_display(
      dateFormat(globalThis.startTime),
      dateFormat(globalThis.endTime),
      globalThis.packageName
      )
      .then((data) => {
        if (data.length != 0) {
          tPowers.push(data[0]);
        }
        database
          .query_applications_cpu(
          dateFormat(globalThis.startTime),
          dateFormat(globalThis.endTime),
          globalThis.packageName
          )
          .then((data) => {
            if (data.length != 0) {
              tPowers.push(data[0]);
            }
            database
              .query_applications_gpu(
              dateFormat(globalThis.startTime),
              dateFormat(globalThis.endTime),
              globalThis.packageName
              )
              .then((data) => {
                if (data.length != 0) {
                  tPowers.push(data[0]);
                }
                database
                  .query_applications_audio(
                  dateFormat(globalThis.startTime),
                  dateFormat(globalThis.endTime),
                  globalThis.packageName
                  )
                  .then((data) => {
                    if (data.length != 0) {
                      tPowers.push(data[0]);
                    }
                    database
                      .query_applications_ddr(
                      dateFormat(globalThis.startTime),
                      dateFormat(globalThis.endTime),
                      globalThis.packageName
                      )
                      .then((data) => {
                        if (data.length != 0) {
                          tPowers.push(data[0]);
                        }
                        database
                          .query_applications_dss(
                          dateFormat(globalThis.startTime),
                          dateFormat(globalThis.endTime),
                          globalThis.packageName
                          )
                          .then((data) => {
                            if (data.length != 0) {
                              tPowers.push(data[0]);
                            }
                            database
                              .query_applications_system_idle(
                              dateFormat(globalThis.startTime),
                              dateFormat(globalThis.endTime),
                              globalThis.packageName
                              )
                              .then((data) => {
                                if (data.length != 0) {
                                  tPowers.push(data[0]);
                                }
                                database
                                  .query_applications_wifi_data(
                                  dateFormat(globalThis.startTime),
                                  dateFormat(globalThis.endTime),
                                  globalThis.packageName
                                  )
                                  .then((data) => {
                                    if (data.length != 0) {
                                      tPowers.push(data[0]);
                                    }
                                    for (var i = 0; i < tPowers.length; i++) {
                                      database.insertPowerSensor(
                                        'task_powersensor_info',
                                      globalThis.dbTime,
                                      tPowers[i]
                                      );
                                    }
                                  })
                                  .then(() => {
                                    return database.query_applications_power_info(
                                    dateFormat(globalThis.startTime),
                                    dateFormat(globalThis.endTime)
                                    );
                                  })
                                  .then((tArr: Array<TPowerAppInfo>) => {
                                    tArr.forEach((t) => {
                                      database.insertPowerAppInfo('task_powerapp_info', globalThis.dbTime, t);
                                    });
                                  });
                              });
                          });
                      });
                  });
              });
          });
      });
  }
  taskDestroy() {
    //数据库时间戳标记
    globalThis.dbTime = -1;
    //开始时间
    globalThis.startTime = -1;
    //结束时间
    globalThis.endTime = -1;
    //入库数据
    globalThis.tTndexs = [];

    //采集配置恢复fileOpen
    globalThis.collectConfigs = -1;
    globalThis.collectPkg = -1;

    //采集定时器 配置
    globalThis.collectInterval = -1;
    globalThis.collectIntervalCollect = -1;
    globalThis.collectPowerCollect = -1;
    globalThis.collectIntervalNum = -1;

    globalThis.powerValue = 0;

    //socket采集队列 fps ram
    globalThis.fpsArr = [];

    // power 电流队列
    globalThis.powerCurArr = [];

    // power 电压队列
    globalThis.powerVoltArr = [];
    globalThis.fpsJitterArr = [];
    globalThis.ramArr = [];
  }
}
