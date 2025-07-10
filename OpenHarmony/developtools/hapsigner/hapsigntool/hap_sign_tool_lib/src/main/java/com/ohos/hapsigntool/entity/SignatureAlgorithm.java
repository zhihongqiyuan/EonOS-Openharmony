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

package com.ohos.hapsigntool.entity;

import java.security.spec.AlgorithmParameterSpec;
import java.security.spec.MGF1ParameterSpec;
import java.security.spec.PSSParameterSpec;

/**
 * Signature algorithm
 *
 * @since 2021-12-13
 */
public enum SignatureAlgorithm {
    RSA_PSS_WITH_SHA256(
            0x101,
            "RSA",
            ContentDigestAlgorithm.SHA256,
            Pair.create(
                    "SHA256withRSAANDMGF1",
                    new PSSParameterSpec("SHA-256", "MGF1", MGF1ParameterSpec.SHA256, 256 / 8, 1))),
    RSA_PSS_WITH_SHA384(
            0x102,
            "RSA",
            ContentDigestAlgorithm.SHA384,
            Pair.create(
                    "SHA384withRSAANDMGF1",
                    new PSSParameterSpec("SHA-384", "MGF1", MGF1ParameterSpec.SHA384, 384 / 8, 1))),
    RSA_PSS_WITH_SHA512(
            0x103,
            "RSA",
            ContentDigestAlgorithm.SHA512,
            Pair.create(
                    "SHA512withRSAANDMGF1",
                    new PSSParameterSpec("SHA-512", "MGF1", MGF1ParameterSpec.SHA512, 512 / 8, 1))),
    RSA_PKCS1_V1_5_WITH_SHA256(0x104, "RSA", ContentDigestAlgorithm.SHA256, Pair.create("SHA256withRSA", null)),
    RSA_PKCS1_V1_5_WITH_SHA384(0x105, "RSA", ContentDigestAlgorithm.SHA256, Pair.create("SHA384withRSA", null)),
    RSA_PKCS1_V1_5_WITH_SHA512(0x106, "RSA", ContentDigestAlgorithm.SHA512, Pair.create("SHA512withRSA", null)),
    ECDSA_WITH_SHA256(0x201, "EC", ContentDigestAlgorithm.SHA256, Pair.create("SHA256withECDSA", null)),
    ECDSA_WITH_SHA384(0x202, "EC", ContentDigestAlgorithm.SHA384, Pair.create("SHA384withECDSA", null)),
    ECDSA_WITH_SHA512(0x203, "EC", ContentDigestAlgorithm.SHA512, Pair.create("SHA512withECDSA", null)),
    DSA_WITH_SHA256(0x301, "DSA", ContentDigestAlgorithm.SHA256, Pair.create("SHA256withDSA", null)),
    DSA_WITH_SHA384(0x302, "DSA", ContentDigestAlgorithm.SHA384, Pair.create("SHA384withDSA", null)),
    DSA_WITH_SHA512(0x303, "DSA", ContentDigestAlgorithm.SHA512, Pair.create("SHA512withDSA", null));

    private int id;

    private String keyAlgorithm;

    private ContentDigestAlgorithm contentDigestAlgorithm;

    private Pair<String, ? extends AlgorithmParameterSpec> signatureAlgAndParams;

    SignatureAlgorithm(
            int id,
            String keyAlgorithm,
            ContentDigestAlgorithm contentDigestAlgorithm,
            Pair<String, ? extends AlgorithmParameterSpec> signatureAlgAndParams) {
        this.id = id;
        this.keyAlgorithm = keyAlgorithm;
        this.contentDigestAlgorithm = contentDigestAlgorithm;
        this.signatureAlgAndParams = signatureAlgAndParams;
    }

    public int getId() {
        return id;
    }

    public String getKeyAlgorithm() {
        return keyAlgorithm;
    }

    public ContentDigestAlgorithm getContentDigestAlgorithm() {
        return contentDigestAlgorithm;
    }

    public Pair<String, ? extends AlgorithmParameterSpec> getSignatureAlgAndParams() {
        return signatureAlgAndParams;
    }

    /**
     * Find value of SignatureAlgorithm according to ID
     *
     * @param id ID of SignatureAlgorithm object
     * @return SignatureAlgorithm object
     */
    public static SignatureAlgorithm findById(int id) {
        SignatureAlgorithm ret = null;
        for (SignatureAlgorithm alg : SignatureAlgorithm.values()) {
            if (id == alg.getId()) {
                return alg;
            }
        }
        return ret;
    }
}
