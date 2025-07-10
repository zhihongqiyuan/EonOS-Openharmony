/*
 * Copyright (c) 2023-2023 Huawei Device Co., Ltd.
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

package com.ohos.hapsigntool.codesigning.sign;

import com.ohos.hapsigntool.codesigning.exception.CodeSignException;
import com.ohos.hapsigntool.hap.config.SignerConfig;

/**
 * Signed data generator interface
 *
 * @since 2023/06/05
 */
@FunctionalInterface
public interface SignedDataGenerator {
    /**
     * Create a BcSignedDataGenerator instance
     */
    SignedDataGenerator BC = new BcSignedDataGenerator();

    /**
     * Generate signature data with specific content and sign configuration.
     *
     * @param content      unsigned file digest content.
     * @param signerConfig sign configurations.
     * @return signed data.
     * @throws CodeSignException if error.
     */
    byte[] generateSignedData(byte[] content, SignerConfig signerConfig) throws CodeSignException;
}
