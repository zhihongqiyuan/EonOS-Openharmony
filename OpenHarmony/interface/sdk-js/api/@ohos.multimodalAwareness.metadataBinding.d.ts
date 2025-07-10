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
 * @kit MultimodalAwarenessKit
 */

import type { image } from '@ohos.multimedia.image';
import type { Callback } from './@ohos.base';
 
/**
 * the moudle for metadataBinding
 * @namespace metadataBinding
 * @syscap SystemCapability.MultimodalAwareness.MetadataBinding
 * @atomicservice
 * @since 18
 */
declare namespace metadataBinding {
  /**
   * MetadataBinding provides coding capabilities
   * @param { image.PixelMap } srcImage - Original image to be encoded
   * @param { string } metadata - Coded metadata
   * @returns { Promise<image.PixelMap> } encoded image
   * @throws { BusinessError } 202 - Permission check failed. A non-system application uses the system API.
   * @throws { BusinessError } 32100001 - Internal handling failed. File creation failed
   * @throws { BusinessError } 32100002 - Encode process fail. Possible causes: 1. Image processing error; 2. Channel coding error
   * @syscap SystemCapability.MultimodalAwareness.MetadataBinding
   * @systemapi
   * @since 18
   */
  function encodeImage(srcImage: image.PixelMap, metadata: string): Promise<image.PixelMap>;

  /**
   * MetadataBinding provides decode capabilities
   * @param { image.PixelMap } encodedImage - Encoded image
   * @returns { Promise<string> } decode result
   * @throws { BusinessError } 202 - Permission check failed. A non-system application uses the system API.
   * @throws { BusinessError } 32100001 - Internal handling failed. File read failed.
   * @throws { BusinessError } 32100003 - Decode process fail. Possible causes: 1. Image is not an encoded Image; 2. Image destroyed, decoding failed
   * @syscap SystemCapability.MultimodalAwareness.MetadataBinding
   * @systemapi
   * @since 18
   */
   function decodeImage(encodedImage: image.PixelMap): Promise<string>;

  /**
   * Obtaining the metadata of a Third-Party App
   * @param { string } bundleName - Bundle name of a third-party application
   * @returns { Promise<string> } third-party app callback the meta data
   * @throws { BusinessError } 202 - Permission check failed. A non-system application uses the system API.
   * @throws { BusinessError } 32100001 - Internal handling failed. Obtain metadata failed.
   * @syscap SystemCapability.MultimodalAwareness.MetadataBinding
   * @systemapi
   * @since 18
   */
  function notifyMetadataBindingEvent(bundleName: string): Promise<string>;

  /**
   * set the Metadata to the screenshot app
   * @param { string } metadata - the Metadata of a Third-Party App
   * @throws { BusinessError } 32100001 - Internal handling failed. Set Meta data to screenshot app fail.
   * @syscap SystemCapability.MultimodalAwareness.MetadataBinding
   * @atomicservice
   * @since 18
   */
  function submitMetadata(metadata: string): void;

  /**
   * Third-party app registration screenshot event
   * @param { 'operationSubmitMetadata' } type - Event Type
   * @param { string } bundleName - Bundle name of a third-party application
   * @param { Callback<number> } callback - Call back the screenshot event
   * @throws { BusinessError } 32100001 - Internal handling failed. Service exception.
   * @throws { BusinessError } 32100004 - Subscribe Failed. Possible causes: 1. Abnormal system capability; 2. IPC communication abnormality;
   * <br>3. Algorithm loading exception.
   * @syscap SystemCapability.MultimodalAwareness.MetadataBinding
   * @atomicservice
   * @since 18
   */
  function on(type: 'operationSubmitMetadata', bundleName: string, callback: Callback<number>): void;
 
  /**
   * Third-party app unregistration screenshot event
   * @param { 'operationSubmitMetadata' } type - Event Type
   * @param { string } bundleName - Bundle name of a third-party application
   * @param { Callback<number> } callback - Call back the screenshot event
   * @throws { BusinessError } 32100001 - Internal handling failed. Service exception.
   * @throws { BusinessError } 32100005 - Unsubscribe Failed. Possible causes: 1. Abnormal system capability; 2. IPC communication abnormality
   * @syscap SystemCapability.MultimodalAwareness.MetadataBinding
   * @atomicservice
   * @since 18
   */
  function off(type: 'operationSubmitMetadata', bundleName: string, callback?: Callback<number>): void;
}

export default metadataBinding;
