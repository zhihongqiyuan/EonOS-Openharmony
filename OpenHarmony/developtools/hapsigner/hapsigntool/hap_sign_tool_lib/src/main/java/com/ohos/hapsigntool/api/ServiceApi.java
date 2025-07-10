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

package com.ohos.hapsigntool.api;

import com.ohos.hapsigntool.entity.Options;

/**
 * Public api of core lib to deal with signature.
 *
 * @since 2021/12/28
 */
public interface ServiceApi {
    /**
     * Generate keystore.
     *
     * @param options options
     * @return Result indicating whether the keystore is generated.
     */
    boolean generateKeyStore(Options options);

    /**
     * Generate csr.
     *
     * @param options options
     * @return Result indicating whether the csr is generated.
     */
    boolean generateCsr(Options options);

    /**
     * Generate cert.
     *
     * @param options options
     * @return Result indicating whether the cert is generated.
     */
    boolean generateCert(Options options);

    /**
     * Generate CA.
     *
     * @param options options
     * @return Result indicating whether the CA is generated.
     */
    boolean generateCA(Options options);

    /**
     * Generate app cert.
     *
     * @param options options
     * @return Result indicating whether the app cert is generated.
     */
    boolean generateAppCert(Options options);

    /**
     * Generate profile cert.
     *
     * @param options options
     * @return Result indicating whether the profile cert is generated.
     */
    boolean generateProfileCert(Options options);

    /**
     * Sign for profile.
     *
     * @param options options
     * @return Result indicating whether the profile is signed.
     */
    boolean signProfile(Options options);

    /**
     * Verify profile.
     *
     * @param options options
     * @return Result indicating whether the profile is correct.
     */
    boolean verifyProfile(Options options);

    /**
     * Sign for hap.
     *
     * @param options options
     * @return Result indicating whether the HAP is signed.
     */
    boolean signHap(Options options);

    /**
     * Verify hap.
     *
     * @param options options
     * @return Result indicating whether the HAP is correct.
     */
    boolean verifyHap(Options options);

}
