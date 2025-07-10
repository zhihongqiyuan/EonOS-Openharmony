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

import com.ohos.hapsigntool.error.CustomException;
import com.ohos.hapsigntool.error.ERROR;
import com.ohos.hapsigntool.error.SignToolErrMsg;
import com.ohos.hapsigntool.utils.LogUtils;

import java.security.InvalidAlgorithmParameterException;
import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;
import java.security.PrivateKey;
import java.security.Signature;
import java.security.SignatureException;
import java.security.cert.X509CRL;
import java.security.cert.X509Certificate;
import java.security.spec.AlgorithmParameterSpec;
import java.util.ArrayList;
import java.util.List;

/**
 * LocalSigner.
 *
 * @since 2021/12/28
 */
public class LocalSigner implements ISigner {
    /**
     * logger of LocalSigner
     */
    private static final LogUtils LOGGER = new LogUtils(LocalSigner.class);

    /**
     * PrivateKey.
     */
    private final PrivateKey privateKey;

    /**
     * X509Certificate.
     */
    private final List<X509Certificate> certificates;

    /**
     * Create local signer.
     *
     * @param privateKey   Private key to sign
     * @param certificates Cert chain to sign
     */
    public LocalSigner(PrivateKey privateKey, List<X509Certificate> certificates) {
        this.privateKey = privateKey;
        this.certificates = certificates;
    }

    /**
     * Signed by local singer.
     *
     * @param data          Data to sign
     * @param signAlg       Sign algorithm
     * @param parameterSpec Preset params
     * @return Data signed
     */
    @Override
    public byte[] getSignature(byte[] data, String signAlg, AlgorithmParameterSpec parameterSpec) {
        byte[] signData = null;
        try {
            Signature signature = Signature.getInstance(signAlg);
            signature.initSign(privateKey);
            if (parameterSpec != null) {
                signature.setParameter(parameterSpec);
            }
            signature.update(data);
            signData = signature.sign();
        } catch (NoSuchAlgorithmException | InvalidKeyException | InvalidAlgorithmParameterException
                | SignatureException exception) {
            LOGGER.debug(exception.getMessage(), exception);
            CustomException.throwException(ERROR.SIGN_ERROR, SignToolErrMsg.SIGNATURE_FAILED
                    .toString(exception.getMessage()));
        }
        return signData;
    }

    /**
     * getCrls.
     *
     * @return crls
     */
    @Override
    public List<X509CRL> getCrls() {
        return new ArrayList<>();
    }

    /**
     * getCertificates.
     *
     * @return Certificates
     */
    @Override
    public List<X509Certificate> getCertificates() {
        return certificates;
    }
}
