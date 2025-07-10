/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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

import AbilityManager from '../../../../../../../../common/src/main/ets/default/abilitymanager/abilityManager';
import Log from '../../../../../../../../common/src/main/ets/default/Log';
import ResourceUtil from '../../../../../../../../common/src/main/ets/default/ResourceUtil';
import NoDisturbingModel from '../model/noDisturbingModel';
import ConfigData, {DoNotDisturbType} from '../common/constants';

const TAG = 'ManagementComponent-NoDisturbComponentViewModel';

export default class NoDisturbComponentViewModel {
  defaultStartTime: Date= new Date();
  defaultEndTime: Date= new Date();
  startTime = '';
  endTime = '';
  repeatMode = 0;
  repeatName: Resource;
  prompt = '';
  isEffective = true;
  nextDayLabel = '';

  viewModelInit(): void{
    Log.showInfo(TAG, 'ViewModelInit');
    void this.getNextDayLabel();
    this.getNoDisturbingDate.bind(this)();
  }

  getNoDisturbingDate(): void {
    Log.showInfo(TAG, 'getNoDisturbingDate');
    NoDisturbingModel.getNoDisturbingDate((data) => {
      Log.showDebug(TAG, 'getNoDisturbingDate data:' + JSON.stringify(data));
      this.repeatMode = data.type??0;
      this.startTime = data.begin??'';
      this.endTime = data.end??'';
      Log.showDebug(TAG, `getNoDisturbingDate repeatMode : ${this.repeatMode} startTime : ${this.startTime} endTime : ${this.endTime}`);
      this.repeatName = this.refreshRepeatName(this.repeatMode);
      this.setClues.bind(this)();
    });
  }

  setNoDisturbingDate(): void {
    if (!this.isEffective) {
      this.repeatMode = DoNotDisturbType.TYPE_NONE;
    }
    Log.showDebug(TAG, `this.repeatMode is : ${this.repeatMode}`);
    let noDisturbingTime = {
      type: this.repeatMode, begin: this.defaultStartTime, end: this.defaultEndTime
    };
    NoDisturbingModel.setNoDisturbingDate(noDisturbingTime, () => {
      Log.showInfo(TAG, 'setNoDisturbingDate is success');
    });
  }

  setClues(): void {
    Log.showInfo(TAG, 'setClues');
    if (this.repeatMode == DoNotDisturbType.TYPE_DAILY ||
    this.repeatMode == DoNotDisturbType.TYPE_ONCE ||
    this.repeatMode == DoNotDisturbType.TYPE_CLEARLY) {
      this.isEffective = true;
    } else {
      this.isEffective = false;
    }

    this.setCluesWithoutSetEffect();
    Log.showDebug(TAG, `this.prompt : ${this.prompt}`);
  }
  setCluesWithoutSetEffect(): void {
    Log.showInfo(TAG, 'setCluesWithoutSetEffect start');
    if (this.repeatMode == DoNotDisturbType.TYPE_CLEARLY) {
      this.prompt = this.getDateTimeLabel(this.startTime) + ' - ' + this.getDateTimeLabel(this.endTime);
      this.defaultStartTime = this.getDateByDateTime(this.startTime);
      this.defaultEndTime = this.getDateByDateTime(this.endTime);
    } else if (this.repeatMode == DoNotDisturbType.TYPE_DAILY || this.repeatMode == DoNotDisturbType.TYPE_ONCE) {
      if (this.startTime >= this.endTime) {
        this.prompt = this.startTime + ' - ' + this.nextDayLabel + this.endTime;
      } else {
        this.prompt = this.startTime + ' - ' + this.endTime;
      }
      this.defaultStartTime = this.getDateByHHMI(this.startTime);
      this.defaultEndTime = this.getDateByHHMI(this.endTime);
    } else {
      this.prompt = '';
      this.defaultStartTime = this.getDateByHHMI(this.startTime);
      this.defaultEndTime = this.getDateByHHMI(this.endTime);
    }
    Log.showInfo(TAG, 'setCluesWithoutSetEffect end');
  }
  refreshDate(repeatMode: number, inputStartTime: Date, inputEndTime: Date): void {
    this.defaultStartTime = inputStartTime;
    this.defaultEndTime = inputEndTime;
    if (repeatMode == DoNotDisturbType.TYPE_CLEARLY) {
      this.startTime = NoDisturbingModel.formatDateTime(this.defaultStartTime) ;
      this.endTime = NoDisturbingModel.formatDateTime(this.defaultEndTime) ;
    } else {
      this.startTime = NoDisturbingModel.formatTime(this.defaultStartTime) ;
      this.endTime = NoDisturbingModel.formatTime(this.defaultEndTime) ;
    }
  }

  refreshRepeatName(inputRepeatMode: number):  Resource {
    let result: Resource ;
    if (inputRepeatMode == DoNotDisturbType.TYPE_ONCE) {
      result = $r('app.string.noDisturb_once');
    } else if (inputRepeatMode == DoNotDisturbType.TYPE_DAILY) {
      result = $r('app.string.noDisturb_daily');
    } else if (inputRepeatMode == DoNotDisturbType.TYPE_CLEARLY) {
      result = $r('app.string.noDisturb_clearly');
    } else {
      result = $r('app.string.noDisturb_none');
    }
    return result;
  }

  getDateByHHMI(selectDate: string): Date{
    Log.showInfo(TAG, 'getDateByHHMI selectDate：' + selectDate);
    let tempDate: Date = new Date();
    let hhmiArr = selectDate.split(':');
    let hour = parseInt(hhmiArr[0]);
    let minute = parseInt(hhmiArr[1]);
    tempDate = new Date(tempDate.getFullYear(), tempDate.getMonth(), tempDate.getDate(),
      hour, minute);
    return tempDate;
  }
  getDateByDateTime(selectDate: string): Date{
    Log.showInfo(TAG, 'getDateByDateTime selectDate：' + selectDate);
    let arrayValue = this.getValuesFromDate(selectDate);
    let tempDate: Date = new Date();
    tempDate = new Date(arrayValue[ConfigData.DATE_YEAR_IDX], arrayValue[ConfigData.DATE_MONTH_IDX] - 1,
      arrayValue[ConfigData.DATE_DAY_IDX], arrayValue[ConfigData.DATE_HOUR_IDX],
      arrayValue[ConfigData.DATE_MINUTE_IDX]);
    return tempDate;
  }
  getValuesFromDate(selectDate: string): number[] {
    let arrayValue = [];
    let idxStart = 0;
    let chrFlg = false;
    if (selectDate) {
      for (let idx = 0;idx < selectDate.length + 1;idx++) {
        if (isNaN(parseInt(selectDate.substring(idx, idx + 1)))) {
          arrayValue.push(parseInt(selectDate.substring(idxStart, idx)));
          chrFlg = true;
        } else if (chrFlg) {
          idxStart = idx;
          chrFlg = false;
        }
      }
    }
    return arrayValue;
  }
  getDateLabel(selectDate: string): string{
    let result = '';
    if (selectDate) {
      let arrayValue = this.getValuesFromDate(selectDate);
      let cutDict = ['年' , '月', '日'];
      for (let idx2 = 0;idx2 < cutDict.length;idx2++) {
        result += arrayValue[idx2];
        result += cutDict[idx2];
      }
    }
    return result;
  }
  getDateTimeLabel(selectDate: string): string{
    let result = '';
    if (selectDate) {
      result += this.getDateLabel(selectDate);
      result += ' ' + NoDisturbingModel.formatTime(this.getDateByDateTime(selectDate));
    }
    return result;
  }
  async getNextDayLabel(): Promise<void> {
    await ResourceUtil.initResourceManager(AbilityManager.ABILITY_NAME_NOTIFICATION_MANAGEMENT);
    this.nextDayLabel = await ResourceUtil.getString($r('app.string.noDisturb_nextDay'));
  }
}

