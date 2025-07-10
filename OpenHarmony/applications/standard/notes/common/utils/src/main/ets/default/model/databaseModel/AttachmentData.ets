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

/**
 * 附件类
 */
export default class AttachmentData {
  id: number // 主键
  uuid: string // 唯一标识
  note_uuid: string // 笔记uuid
  path: string // 路径
  attachment_type: number // 类型
  created_time: number // 创建时间
  modified_time: number // 修改时间

  constructor(id: number, uuid: string, note_uuid: string, path: string, attachment_type: number, created_time: number, modified_time: number) {
    this.id = id;
    this.uuid = uuid;
    this.note_uuid = note_uuid;
    this.path = path;
    this.attachment_type = attachment_type;
    this.created_time = created_time;
    this.modified_time = modified_time;
  }
}