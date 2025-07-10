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
 * table name
 */
export enum TableName {
  FolderTable = "folder_table",
  NoteTable = "note_table",
  AttachmentTable = "attachment_table",
  FormTable = "form_table"
}

/**
 * folder table column
 */
export enum FolderTableColumn {
  Id = "id", // 主键
  Name = "name", // 名称
  Uuid = "uuid", // 唯一标识
  Color = "color", // 图标颜色
  FolderType = "folder_type", // 类型
  IsDeleted = "is_deleted", // 是否被删除
  CreatedTime = "created_time", // 创建时间
  ModifiedTime = "modified_time" // 修改时间
}

/**
 * note table column
 */
export enum NoteTableColumn {
  Id = "id", // 主键
  Title = "title", // 标题
  Uuid = "uuid", // 唯一标识
  FolderUuid = "folder_uuid", // 文件夹uuid
  ContentText = "content_text", // 文字内容
  ContentImg = "content_img", // 图片路径
  NoteType = "note_type", // 类型
  IsTop = "is_top", // 是否置顶
  IsFavorite = "is_favorite", // 是否被收藏
  IsDeleted = "is_deleted", // 是否被删除
  CreatedTime = "created_time", // 创建时间
  ModifiedTime = "modified_time", // 修改时间
  DeletedTime = "deleted_time", // 删除时间
  SliderValue = "slider_value"
}

/**
 * attachment table column
 */
export enum AttachmentTableColumn {
  Id = "id", // 主键
  Uuid = "uuid", // 唯一标识
  NoteUuid = "note_uuid", // 笔记uuid
  Path = "path", // 路径
  AttachmentType = "attachment_type", // 类型
  CreatedTime = "created_time", // 创建时间
  ModifiedTime = "modified_time" // 修改时间
}

/**
 * form table column
 */
export enum FormTableColumn {
  Id = "id", // 卡片id
  Name = "name", // 卡片名称
  Dimension = "dimension", // 卡片尺寸
}

/**
 * Database table init sql
 */
export enum TableSql {
  FolderTableSQL = "CREATE TABLE IF NOT EXISTS " + TableName.FolderTable + " ("
  + FolderTableColumn.Id + " INTEGER PRIMARY KEY AUTOINCREMENT,"
  + FolderTableColumn.Name + " TEXT NOT NULL,"
  + FolderTableColumn.Uuid + " TEXT NOT NULL,"
  + FolderTableColumn.Color + " TEXT DEFAULT '#ffffffff',"
  + FolderTableColumn.FolderType + " INTEGER DEFAULT 1,"
  + FolderTableColumn.IsDeleted + " INTEGER DEFAULT 0,"
  + FolderTableColumn.CreatedTime + " INTEGER,"
  + FolderTableColumn.ModifiedTime + " INTEGER"
  + ")",

  NoteTableSQL = "CREATE TABLE IF NOT EXISTS " + TableName.NoteTable + " ("
  + NoteTableColumn.Id + " INTEGER PRIMARY KEY AUTOINCREMENT,"
  + NoteTableColumn.Title + " TEXT NOT NULL,"
  + NoteTableColumn.Uuid + " TEXT NOT NULL,"
  + NoteTableColumn.FolderUuid + " TEXT,"
  + NoteTableColumn.ContentText + " TEXT,"
  + NoteTableColumn.ContentImg + " TEXT,"
  + NoteTableColumn.NoteType + " INTEGER DEFAULT 1,"
  + NoteTableColumn.IsTop + " INTEGER DEFAULT 0,"
  + NoteTableColumn.IsFavorite + " INTEGER DEFAULT 0,"
  + NoteTableColumn.IsDeleted + " INTEGER DEFAULT 0,"
  + NoteTableColumn.CreatedTime + " INTEGER,"
  + NoteTableColumn.ModifiedTime + " INTEGER,"
  + NoteTableColumn.DeletedTime + " INTEGER,"
  + NoteTableColumn.SliderValue + " INTEGER "
  + ")",

  AttachmentTableSQL = "CREATE TABLE IF NOT EXISTS " + TableName.AttachmentTable + " ("
  + AttachmentTableColumn.Id + " INTEGER PRIMARY KEY AUTOINCREMENT,"
  + AttachmentTableColumn.Uuid + " TEXT NOT NULL,"
  + AttachmentTableColumn.NoteUuid + " TEXT NOT NULL,"
  + AttachmentTableColumn.Path + " TEXT,"
  + AttachmentTableColumn.AttachmentType + " INTEGER DEFAULT 0,"
  + AttachmentTableColumn.CreatedTime + " INTEGER,"
  + AttachmentTableColumn.ModifiedTime + " INTEGER"
  + ")",

  FormTableSQL = "CREATE TABLE IF NOT EXISTS " + TableName.FormTable + " ("
  + FormTableColumn.Id + " TEXT NOT NULL,"
  + FormTableColumn.Name + " TEXT NOT NULL,"
  + FormTableColumn.Dimension + " TEXT NOT NULL"
  + ")"
}

/**
 * system Define folder
 */
export enum SysDefFolder {
  AllNotes = "sys_def_allNotes",
  UnClassified = "sys_def_unClassified",
  MyFavorites = "sys_def_myFavorites",
  RecentDeletes = "sys_def_recentDeletes",
  Personal = "sys_def_personal",
  Life = "sys_def_life",
  Work = "sys_def_work"
}

/**
 * system Define folder uuid
 */
export enum SysDefFolderUuid {
  AllNotes = "sys_def_allNotes_uuid",
  UnClassified = "sys_def_unClassified_uuid",
  MyFavorites = "sys_def_myFavorites_uuid",
  RecentDeletes = "sys_def_recentDeletes_uuid",
  Personal = "sys_def_personal_uuid",
  Life = "sys_def_life_uuid",
  Work = "sys_def_work_uuid"
}

/**
 * folder type
 */
export enum FolderType {
  SysDef, // System definition
  CusDef, // User defined
  FeatureDef // feature defined: MyFavorites & RecentDeletes
}

/**
 * note type
 */
export enum NoteType {
  SysDef,
  CusDef
}

/**
 * attachment type
 */
export enum AttachmentType {
  Image,
  Voice,
  HandWriting
}

/**
 * Note favorite status
 */
export enum Favorite {
  No,
  Yes
}

/**
 * Note delete status
 */
export enum Delete {
  No,
  Yes
}

/**
 * Note top status
 */
export enum Top {
  No,
  Yes
}

/**
 * delete file type
 */
export enum DeleteFileType {
  NoteData,
  FolderData
}