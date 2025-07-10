/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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
 * @kit CoreFileKit
 */

/**
 * This module provides the capability to parse file or device information.
 *
 * @namespace fileExtensionInfo
 * @syscap SystemCapability.FileManagement.UserFileService
 * @systemapi
 * @since 9
 */
declare namespace fileExtensionInfo {
  /**
   * DeviceType Indicates the type of device connected to the fileaccess server.
   *
   * @enum { number }
   * @syscap SystemCapability.FileManagement.UserFileService
   * @systemapi
   * @stagemodelonly
   * @since 9
   */
  enum DeviceType {
    /**
     * Local c,d... disk.
     *
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @stagemodelonly
     * @since 9
     */
    DEVICE_LOCAL_DISK = 1,

    /**
     * Multi-user shared disk.
     *
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @stagemodelonly
     * @since 9
     */
    DEVICE_SHARED_DISK,

    /**
     * Distributed networking terminal device.
     *
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @stagemodelonly
     * @since 9
     */
    DEVICE_SHARED_TERMINAL,

    /**
     * Network neighbor device.
     *
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @stagemodelonly
     * @since 9
     */
    DEVICE_NETWORK_NEIGHBORHOODS,

    /**
     * MTP device.
     *
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @stagemodelonly
     * @since 9
     */
    DEVICE_EXTERNAL_MTP,

    /**
     * USB device.
     *
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @stagemodelonly
     * @since 9
     */
    DEVICE_EXTERNAL_USB,

    /**
     * Cloud disk device.
     *
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @stagemodelonly
     * @since 9
     */
    DEVICE_EXTERNAL_CLOUD
  }

  /**
   * Indicates the supported capabilities of the device.
   *
   * @namespace DeviceFlag
   * @syscap SystemCapability.FileManagement.UserFileService
   * @systemapi
   * @stagemodelonly
   * @since 9
   */
  namespace DeviceFlag {
    /**
     * supports read.
     *
     * @constant
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @stagemodelonly
     * @since 9
     */
    const SUPPORTS_READ = 0b1;

    /**
     * supports write.
     *
     * @constant
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @stagemodelonly
     * @since 9
     */
    const SUPPORTS_WRITE = 0b10;
  }

  /**
   * Indicate the supported capabilities of the file or directory.
   *
   * @namespace DocumentFlag
   * @syscap SystemCapability.FileManagement.UserFileService
   * @systemapi
   * @stagemodelonly
   * @since 9
   */
  namespace DocumentFlag {
    /**
     * represents file.
     *
     * @constant
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @stagemodelonly
     * @since 9
     */
    const REPRESENTS_FILE = 0b1;

    /**
     * represents directory.
     *
     * @constant
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @stagemodelonly
     * @since 9
     */
    const REPRESENTS_DIR = 0b10;

    /**
     * supports read.
     *
     * @constant
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @stagemodelonly
     * @since 9
     */
    const SUPPORTS_READ = 0b100;

    /**
     * supports write.
     *
     * @constant
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @stagemodelonly
     * @since 9
     */
    const SUPPORTS_WRITE = 0b1000;
  }
}

export default fileExtensionInfo;