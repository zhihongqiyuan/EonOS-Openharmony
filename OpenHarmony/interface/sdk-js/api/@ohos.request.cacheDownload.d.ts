/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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
 * @file
 * @kit BasicServicesKit
 */

import { BusinessError } from './@ohos.base';

/**
 * Cache download capability provider.
 *
 * @namespace cacheDownload
 * @syscap SystemCapability.Request.FileTransferAgent
 * @since 18
 */
declare namespace cacheDownload {
    /**
     * Options of the cache download task.
     *
     * @typedef CacheDownloadOptions
     * @syscap SystemCapability.Request.FileTransferAgent
     * @since 18
     */
    interface CacheDownloadOptions {
        /**
         * HTTP headers added to the cache download request.
         *
         * @type { ?Record<string, string> }
         * @syscap SystemCapability.Request.FileTransferAgent
         * @since 18
         */
        headers?: Record<string, string>;
    }

    /**
     * Downloads resources at the specified URL. Resources will be stored in memory cache or files cache.
     * The maximum size of the specified URL is 8192 bytes.
     * The maximum size of a single resource after decompression is 20,971,520 bytes(20 MB).
     * If the decompressed size of the downloaded resource exceeds the limit, it will not be recorded in the cache.
     *
     * @permission ohos.permission.INTERNET
     * @param { string } url - URL of the cache download target.
     * @param { CacheDownloadOptions } options - Options of the cache download task.
     * @throws { BusinessError } 201 - permission denied.
     * @throws { BusinessError } 401 - parameter error. Possible causes: 1. Missing mandatory parameters.
     * <br>2. Incorrect parameter type. 3. Parameter verification failed.
     * @syscap SystemCapability.Request.FileTransferAgent
     * @since 18
     */
    function download(url: string, options: CacheDownloadOptions);

    /**
     * Cancels an ongoing cache download task based on the target URL.
     * The maximum size of the specified URL is 8192 bytes.
     *
     * @param { string } url - URL of the cache download target.
     * @throws { BusinessError } 401 - parameter error. Possible causes: 1. Missing mandatory parameters.
     * <br>2. Incorrect parameter type. 3. Parameter verification failed.
     * @syscap SystemCapability.Request.FileTransferAgent
     * @since 18
     */
    function cancel(url: string);

    /**
     * Sets the size of the memory cache used to store downloaded content.
     * The default size is 0 bytes.
     * The maximum size is 1,073,741,824 bytes(1 GB).
     *
     * @param { number } bytes - The maximum amount of data cached in memory, in bytes.
     * @throws { BusinessError } 401 - parameter error. Possible causes: 1. Missing mandatory parameters.
     * <br>2. Incorrect parameter type. 3. Parameter verification failed.
     * @syscap SystemCapability.Request.FileTransferAgent
     * @since 18
     */
    function setMemoryCacheSize(bytes: number);

    /**
     * Sets the size of the file cache used to store downloaded content.
     * The default size is 104,857,600 bytes(100 MB).
     * The maximum size is 4,294,967,296 bytes(4 GB).
     *
     * @param { number } bytes - The maximum amount of data cached in files, in bytes.
     * @throws { BusinessError } 401 - parameter error. Possible causes: 1. Missing mandatory parameters.
     * <br>2. Incorrect parameter type. 3. Parameter verification failed.
     * @syscap SystemCapability.Request.FileTransferAgent
     * @since 18
     */
    function setFileCacheSize(bytes: number);
}

export default cacheDownload;