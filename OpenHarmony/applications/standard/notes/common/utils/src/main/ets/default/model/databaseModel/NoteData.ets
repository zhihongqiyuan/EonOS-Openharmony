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

import { NoteType, Favorite, Delete, Top } from './EnumData'
import relationalStore from '@ohos.data.relationalStore';

/**
 * 笔记类
 */
export default class NoteData {
  id: number // 主键
  title: string // 标题
  uuid: string // 唯一标识
  folder_uuid: string // 文件夹uuid
  content_text: string // 文字内容
  content_img: string // 图片路径
  note_type: NoteType // 类型
  is_top: Top // 是否置顶
  is_favorite: Favorite // 是否被收藏
  is_deleted: Delete // 是否被删除
  created_time: number // 创建时间
  modified_time: number // 修改时间
  deleted_time: number // 删除时间
  slider_value: number //字体进度条大小

  constructor(id: number, title: string, uuid: string, folder_uuid: string, content_text: string, content_img: string,
              note_type: NoteType, is_top: Top, is_favorite: Favorite, is_deleted: Delete, created_time: number,
              modified_time: number, deleted_time: number, slider_value: number) {
    this.id = id
    this.title = title
    this.uuid = uuid
    this.folder_uuid = folder_uuid
    this.content_text = content_text
    this.content_img = content_img
    this.note_type = note_type
    this.is_top = is_top
    this.is_favorite = is_favorite
    this.is_deleted = is_deleted
    this.created_time = created_time
    this.modified_time = modified_time
    this.deleted_time = deleted_time
    this.slider_value = slider_value
  }

  /**
   * 转化为note_table表的valueBucket
   */
  toNoteObject(): relationalStore.ValuesBucket {
    return {
      "title": this.title,
      "uuid": this.uuid,
      "folder_uuid": this.folder_uuid,
      "content_text": this.content_text,
      "content_img": this.content_img,
      "note_type": this.note_type,
      "is_top": this.is_top,
      "is_favorite": this.is_favorite,
      "is_deleted": this.is_deleted,
      "created_time": this.created_time,
      "modified_time": this.modified_time,
      "deleted_time": this.deleted_time,
      "slider_value":this.slider_value
    }
  }
}