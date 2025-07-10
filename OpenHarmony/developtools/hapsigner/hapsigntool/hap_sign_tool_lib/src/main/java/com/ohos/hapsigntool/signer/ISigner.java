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

package com.ohos.hapsigntool.signer;

import java.security.cert.X509CRL;
import java.security.cert.X509Certificate;
import java.security.spec.AlgorithmParameterSpec;
import java.util.List;

/**
 * ISigner.
 *
 * @since 2021/12/28
 */
public interface ISigner {
    /**
     * Sign function, implement by local or remote.
     *
     * @param data          Data to sign
     * @param signAlg       Sign algorithm
     * @param parameterSpec Preset params
     * @return Data signed
     */
    byte[] getSignature(byte[] data, String signAlg, AlgorithmParameterSpec parameterSpec);

    /**
     * getCrls.
     *
     * @return crls
     */
    List<X509CRL> getCrls();

    /**
     * getCertificates.
     *
     * @return Certificates
     */
    List<X509Certificate> getCertificates();

}
