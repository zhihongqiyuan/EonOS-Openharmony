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
import java.util.Map;

/**
 * RemoteSigner.
 *
 * @since 2021/12/28
 */
public class RemoteSigner implements ISigner {
    /**
     * UnsupportedOperationException message.
     */
    private static final String ERROR = "Not implement yet";

    /**
     * Empty remote signer.
     *
     * @param params Preset map configurations
     */
    public RemoteSigner(Map<String, Object> params) {}

    /**
     * Signed by remote signer.
     *
     * @param data          Data to sign
     * @param signAlg       Sign algorithm
     * @param parameterSpec Preset params
     * @return Data signed
     */
    @Override
    public byte[] getSignature(byte[] data, String signAlg, AlgorithmParameterSpec parameterSpec) {
        throw new UnsupportedOperationException(ERROR);
    }

    /**
     * getCrls.
     *
     * @return crls
     */
    @Override
    public List<X509CRL> getCrls() {
        throw new UnsupportedOperationException(ERROR);
    }

    /**
     * getCertificates.
     *
     * @return Certificates
     */
    @Override
    public List<X509Certificate> getCertificates() {
        throw new UnsupportedOperationException(ERROR);
    }
}
