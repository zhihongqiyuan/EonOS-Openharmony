/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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

package com.ohos.hapsigntool.hap.sign;

import com.ohos.hapsigntool.hap.config.SignerConfig;
import com.ohos.hapsigntool.error.SignatureException;

/**
 * interface of PKCS#7 signed data generator
 *
 * @since 2021/12/21
 */
@FunctionalInterface
public interface Pkcs7Generator {
    /**
     * PKCS#7 Generator
     */
    Pkcs7Generator BC = new BcPkcs7Generator();

    /**
     * Generate PKCS#7 signed data with the specific content and signer config.
     *
     * @param content PKCS7 data, content of unsigned file digest.
     * @param signerConfig configurations of signer.
     * @return PKCS7 signed data.
     * @throws SignatureException if an error occurs when generating PKCS#7 block.
     */
    byte[] generateSignedData(byte[] content, SignerConfig signerConfig) throws SignatureException;
}
