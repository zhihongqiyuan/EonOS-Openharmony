/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

import {FolderType, Delete} from './EnumData'
import relationalStore from '@ohos.data.relationalStore';

/**
 * 文件夹类
 */
export default class FolderData {
  id: number // 主键
  name: string // 名称
  uuid: string // 唯一标识
  color: string // 图标颜色
  folder_type: FolderType // 类型
  is_deleted: Delete // 是否被删除
  created_time: number // 创建时间
  modified_time: number // 修改时间

  constructor(id: number, name: string, uuid: string, color: string, folder_type: FolderType,
              is_deleted: Delete, created_time: number, modified_time: number) {
    this.id = id
    this.name = name
    this.uuid = uuid
    this.color = color
    this.folder_type = folder_type
    this.is_deleted = is_deleted
    this.created_time = created_time
    this.modified_time = modified_time
  }

  /**
   * 转化为folder_table表的valueBucket
   */
  toFolderObject(): relationalStore.ValuesBucket {
    return {
      "name": this.name,
      "uuid": this.uuid,
      "color": this.color,
      "folder_type": this.folder_type,
      "is_deleted": this.is_deleted,
      "created_time": this.created_time,
      "modified_time": this.modified_time
    }
  }
}