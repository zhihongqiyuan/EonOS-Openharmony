/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License"),
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
 * @file
 * @kit LocalizationKit
 */

/**
 * Provides the capability of install and uninstall font.
 *
 * @namespace fontManager
 * @syscap SystemCapability.Global.FontManager
 * @systemapi
 * @since 19
 */
declare namespace fontManager {
  /**
   * Installs the specified path font.
   *
   * @permission ohos.permission.UPDATE_FONT
   * @param { string } path - path indicates the font path.
   * @returns { Promise<number> } - Number indicates the font installation result.
   *        0 - Install successful.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system application.
   * @throws { BusinessError } 31100101 - Font does not exist.
   * @throws { BusinessError } 31100102 - Font is not supported.
   * @throws { BusinessError } 31100103 - Font file copy failed.
   * @throws { BusinessError } 31100104 - Font file installed.
   * @throws { BusinessError } 31100105 - Exceeded maximum number of installed files.
   * @throws { BusinessError } 31100106 - Other error.
   * @syscap SystemCapability.Global.FontManager
   * @systemapi
   * @since 19
   */
  function installFont(path: string): Promise<number>;

  /**
   * Uninstalls the specified path font.
   *
   * @permission ohos.permission.UPDATE_FONT
   * @param { string } fullName - fullName indicates the font name.
   * @returns { Promise<number> } - number indicates the font uninstallation result.
   *        0 - Uninstall successful.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system application.
   * @throws { BusinessError } 31100107 - Font file does not exist.
   * @throws { BusinessError } 31100108 - Font file delete error.
   * @throws { BusinessError } 31100109 - Other error.
   * @syscap SystemCapability.Global.FontManager
   * @systemapi
   * @since 19
   */
  function uninstallFont(fullName: string): Promise<number>;
}
export default fontManager;