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

import com.ohos.hapsigntool.cert.CertBuilder;
import com.ohos.hapsigntool.cert.CertLevel;
import com.ohos.hapsigntool.adapter.LocalizationAdapter;
import com.ohos.hapsigntool.entity.Options;
import com.ohos.hapsigntool.error.CustomException;
import com.ohos.hapsigntool.error.ERROR;
import com.ohos.hapsigntool.error.SignToolErrMsg;
import com.ohos.hapsigntool.utils.CertUtils;

import com.ohos.hapsigntool.utils.LogUtils;
import org.bouncycastle.asn1.x500.X500Name;
import org.bouncycastle.asn1.x509.KeyPurposeId;
import org.bouncycastle.asn1.x509.KeyUsage;
import org.bouncycastle.pkcs.PKCS10CertificationRequest;
import org.bouncycastle.pkcs.jcajce.JcaPKCS10CertificationRequestBuilder;

import java.io.IOException;
import java.security.KeyPair;
import java.security.cert.X509Certificate;

/**
 * CertTools.
 *
 * @since 2021/12/28
 */
public final class CertTools {
    /**
     * Ten years, in days.
     */
    private static final int TEN_YEAR_DAY = 3650;

    /**
     * Three years, in dats.
     */
    private static final int THREE_YEAR_DAY = 1095;

    /**
     * Empty csr array.
     */
    private static final byte[] NO_CSR = {};

    /**
     * Logger.
     */
    private static final LogUtils LOGGER = new LogUtils(ServiceApi.class);

    private CertTools() {
    }

    /**
     * Generate root ca certificate.
     *
     * @param keyPair keyPair
     * @param csr     csr
     * @param adapter adapter
     * @return X509Certificate
     */
    public static X509Certificate generateRootCaCert(KeyPair keyPair, byte[] csr, LocalizationAdapter adapter) {
        try {
            return new CertBuilder(keyPair, adapter.getIssuer(), csr,
                    adapter.getOptions().getInt(Options.VALIDITY, TEN_YEAR_DAY))
                    .withAuthorityKeyIdentifier(CertLevel.ROOT_CA)
                    .withBasicConstraints(CertLevel.ROOT_CA, true, true,
                            adapter.getBasicConstraintsPathLen())
                    .withKeyUsages(new KeyUsage(KeyUsage.keyCertSign | KeyUsage.cRLSign), true)
                    .withExtKeyUsages(null, false)
                    .build(adapter.getSignAlg());
        } catch (IOException exception) {
            LOGGER.debug(exception.getMessage(), exception);
            CustomException.throwException(ERROR.IO_CERT_ERROR, SignToolErrMsg.CERT_IO_FAILED
                    .toString(exception.getMessage()));
        }
        return null;
    }

    /**
     * Generate sub ca certificate.
     *
     * @param keyPair keyPair
     * @param csr     csr
     * @param adapter parameter
     * @return X509Certificate
     */
    public static X509Certificate generateSubCert(KeyPair keyPair, byte[] csr, LocalizationAdapter adapter) {
        try {
            return new CertBuilder(keyPair, adapter.getIssuer(), csr,
                    adapter.getOptions().getInt(Options.VALIDITY, TEN_YEAR_DAY))
                    .withAuthorityKeyIdentifier(CertLevel.SUB_CA)
                    .withBasicConstraints(CertLevel.SUB_CA, true, true,
                            adapter.getBasicConstraintsPathLen())
                    .withKeyUsages(new KeyUsage(KeyUsage.keyCertSign | KeyUsage.cRLSign), true)
                    .build(adapter.getSignAlg());
        } catch (IOException exception) {
            LOGGER.debug(exception.getMessage(), exception);
            CustomException.throwException(ERROR.IO_CERT_ERROR, SignToolErrMsg.CERT_IO_FAILED
                    .toString(exception.getMessage()));
        }
        return null;
    }

    /**
     * Generate certificate.
     *
     * @param keyPair keyPair
     * @param csr     csr
     * @param adapter parameter
     * @return X509Certificate
     */
    public static X509Certificate generateCert(KeyPair keyPair, byte[] csr, LocalizationAdapter adapter) {
        try {
            return new CertBuilder(keyPair, adapter.getIssuer(), csr,
                    adapter.getOptions().getInt(Options.VALIDITY, THREE_YEAR_DAY))
                    // Need CertLevel
                    .withAuthorityKeyIdentifier(CertLevel.ROOT_CA)
                    .withBasicConstraints(CertLevel.ROOT_CA,
                            adapter.isBasicConstraintsCritical(),
                            adapter.isBasicConstraintsCa(),
                            adapter.getBasicConstraintsPathLen())
                    .withKeyUsages(adapter.getKeyUsage(), adapter.isKeyUsageCritical())
                    .withExtKeyUsages(adapter.getExtKeyUsage(), adapter.isExtKeyUsageCritical())
                    .build(adapter.getSignAlg());
        } catch (IOException exception) {
            LOGGER.debug(exception.getMessage(), exception);
            CustomException.throwException(ERROR.IO_CERT_ERROR, SignToolErrMsg.CERT_IO_FAILED
                    .toString(exception.getMessage()));
        }
        return null;
    }

    /**
     * Generate app certificate.
     *
     * @param keyPair keyPair
     * @param csr     csr
     * @param adapter adapter
     * @param signingCapabiltyBytes signingCapabiltyBytes
     * @return X509Certificate
     */
    public static X509Certificate generateEndCert(KeyPair keyPair, byte[] csr, LocalizationAdapter adapter,
                                                  byte[] signingCapabiltyBytes) {
        try {
            return new CertBuilder(keyPair, adapter.getIssuer(), csr,
                    adapter.getOptions().getInt(Options.VALIDITY, THREE_YEAR_DAY))
                    .withBasicConstraints(CertLevel.END_ENTITY, false, false,
                            null)
                    .withKeyUsages(new KeyUsage(KeyUsage.digitalSignature), true)
                    .withExtKeyUsages(new KeyPurposeId[]{KeyPurposeId.id_kp_codeSigning}, false)
                    .withSigningCapabilty(signingCapabiltyBytes)
                    .build(adapter.getSignAlg());
        } catch (IOException exception) {
            LOGGER.debug(exception.getMessage(), exception);
            CustomException.throwException(ERROR.IO_CERT_ERROR, SignToolErrMsg.CERT_IO_FAILED
                    .toString(exception.getMessage()));
        }
        return null;
    }

    /**
     * generateCsr.
     *
     * @param keyPair       Applier keypair
     * @param signAlgorithm sign algorithm
     * @param subject       Applier subject
     * @return csr bytes
     */
    public static byte[] generateCsr(KeyPair keyPair, String signAlgorithm, X500Name subject) {
        JcaPKCS10CertificationRequestBuilder csrBuilder = new JcaPKCS10CertificationRequestBuilder(subject,
                keyPair.getPublic());
        PKCS10CertificationRequest csr = csrBuilder.build(CertUtils.createFixedContentSigner(keyPair.getPrivate(),
                signAlgorithm));
        try {
            return csr.getEncoded();
        } catch (IOException exception) {
            LOGGER.debug(exception.getMessage(), exception);
            CustomException.throwException(ERROR.IO_CSR_ERROR, SignToolErrMsg.IO_CSR_ERROR
                    .toString("Not support " + subject));
            return NO_CSR;
        }
    }



}
