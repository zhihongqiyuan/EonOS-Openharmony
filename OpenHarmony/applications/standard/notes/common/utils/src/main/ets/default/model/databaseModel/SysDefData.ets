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

import { SysDefFolder, SysDefFolderUuid, FolderType, NoteType, Favorite, Top, Delete } from './EnumData';
import relationalStore from '@ohos.data.relationalStore';

interface DbInfoType {
  db_name: DbNameType;
  db_verison: number;
}

interface DbNameType {
  name: string;
  securityLevel: relationalStore.SecurityLevel;
}

class SysDefData {
  /**
   * db info
   */
  public dbInfo: DbInfoType = {
    db_name: {
      name: "note.db", // 数据库名称
      securityLevel: relationalStore.SecurityLevel.S1
    },
    db_verison: 1 // 数据库版本
  };

  /**
   * system defined folder
   */
  public sys_def_allNotes: relationalStore.ValuesBucket = {
      "id": 1,
      "name": SysDefFolder.AllNotes,
      "uuid": SysDefFolderUuid.AllNotes,
      "color": "#ffffff",
      "folder_type": FolderType.SysDef,
      "is_deleted": Delete.No,
      "created_time": 1641863695000,
      "modified_time": 1641863695000
    };

  public sys_def_unClassified: relationalStore.ValuesBucket = {
      "id": 2,
      "name": SysDefFolder.UnClassified,
      "uuid": SysDefFolderUuid.UnClassified,
      "color": "#ffffff",
      "folder_type": FolderType.SysDef,
      "is_deleted": Delete.No,
      "created_time": 1641860095000,
      "modified_time": 1641860095000
    };

  public sys_def_myFavorites: relationalStore.ValuesBucket = {
      "id": 3,
      "name": SysDefFolder.MyFavorites,
      "uuid": SysDefFolderUuid.MyFavorites,
      "color": "#ffffff",
      "folder_type": FolderType.FeatureDef,
      "is_deleted": Delete.No,
      "created_time": 1641856088000,
      "modified_time": 1641856088000
    };

  public sys_def_recentDeletes: relationalStore.ValuesBucket = {
      "id": 4,
      "name": SysDefFolder.RecentDeletes,
      "uuid": SysDefFolderUuid.RecentDeletes,
      "color": "#ffffff",
      "folder_type": FolderType.FeatureDef,
      "is_deleted": Delete.No,
      "created_time": 1641848898000,
      "modified_time": 1641848898000
    };

  public sys_def_personal: relationalStore.ValuesBucket = {
      "id": 5,
      "name": "个人",
      "uuid": SysDefFolderUuid.Personal,
      "color": "#41ba41",
      "folder_type": FolderType.CusDef,
      "is_deleted": Delete.No,
      "created_time": 1641677288000,
      "modified_time": 1641677288000
    };

  public sys_def_life: relationalStore.ValuesBucket = {
      "id": 6,
      "name": "生活",
      "uuid": SysDefFolderUuid.Life,
      "color": "#00aaee",
      "folder_type": FolderType.CusDef,
      "is_deleted": Delete.No,
      "created_time": 1641676108000,
      "modified_time": 1641676108000
    };

  public sys_def_work: relationalStore.ValuesBucket = {
      "id": 7,
      "name": "工作",
      "uuid": SysDefFolderUuid.Work,
      "color": "#f86d05",
      "folder_type": FolderType.CusDef,
      "is_deleted": Delete.No,
      "created_time": 1641636508000,
      "modified_time": 1641636508000
    };

  /**
   * system defined note
   */
  public sys_def_note1: relationalStore.ValuesBucket = {
      "id": 1,
      "title": "数学公式",
      "uuid": "note1_uuid",
      "folder_uuid": SysDefFolderUuid.UnClassified,
      "content_text": '<text>好好学习，天天向上</text>',
      "content_img": "",
      "note_type": NoteType.SysDef,
      "is_top": Top.Yes,
      "is_favorite": Favorite.Yes,
      "is_deleted": Delete.No,
      "created_time": 1641869573000,
      "modified_time": 1641869573000,
      "deleted_time": 0,
      "slider_value": 0
    };

  public sys_def_note2: relationalStore.ValuesBucket = {
      "id": 2,
      "title": "UX会议纪要",
      "uuid": "note2_uuid",
      "folder_uuid": SysDefFolderUuid.Work,
      "content_text": "",
      "content_img": "",
      "note_type": NoteType.SysDef,
      "is_top": Top.Yes,
      "is_favorite": Favorite.No,
      "is_deleted": Delete.No,
      "created_time": 1640999288000,
      "modified_time": 1640999288000,
      "deleted_time": 0,
      "slider_value": 0
    };

  public sys_def_note3: relationalStore.ValuesBucket = {
      "id": 3,
      "title": "旅游心得",
      "uuid": "note3_uuid",
      "folder_uuid": SysDefFolderUuid.Work,
      "content_text": '<div style="line-height:1.4;"><text>要想整个旅程无遗憾，关键是心态,首先要对即将到的目的地有一个正确的认识和恰当的期望。</text></div>',
      "content_img": "",
      "note_type": NoteType.SysDef,
      "is_top": Top.No,
      "is_favorite": Favorite.No,
      "is_deleted": Delete.No,
      "created_time": 1641385708000,
      "modified_time": 1641385708000,
      "deleted_time": 0,
      "slider_value": 0
    };

  public sys_def_note4: relationalStore.ValuesBucket = {
    "id": 4,
    "title": "蛋糕做法",
    "uuid": "note4_uuid",
    "folder_uuid": SysDefFolderUuid.Life,
    "content_text": '<span>&nbsp;</span>' +
      '<input name="checkbox" type="checkbox" onclick="onCheckChange(this)" class="note-checkbox" checked="checked">' +
      '<span style="line-height:22.4px;" class="note-checkbox-txt">糖60g</span><br>' +
      '<span>&nbsp;</span>' +
      '<input name="checkbox" type="checkbox" onclick="onCheckChange(this)" class="note-checkbox">' +
      '<span style="line-height:22.4px;" class="note-checkbox-txt">玉米油40g</span><br><div>' +
      '<div><text style="line-height:22.4px;">&nbsp;1.&nbsp;&nbsp;&nbsp;添加鸡蛋、盐、糖、蛋糕粉<br>' +
      '&nbsp;2.&nbsp;&nbsp;&nbsp;加入玉米油、牛奶，烤箱预热15分钟<br>' +
      '&nbsp;3.&nbsp;&nbsp;&nbsp;加入葡萄干、核桃仁，烤箱定时40分钟<br><br></text></div></div></div>',
    "content_img": "",
    "note_type": NoteType.CusDef,
    "is_top": Top.No,
    "is_favorite": Favorite.Yes,
    "is_deleted": Delete.No,
    "created_time": 1640909288000,
    "modified_time": 1640909288000,
    "deleted_time": 0,
    "slider_value": 0
  };
}

export default new SysDefData();