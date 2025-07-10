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

package com.ohos.hapsigntool.cert;

import com.ohos.hapsigntool.api.ServiceApi;
import com.ohos.hapsigntool.error.CustomException;
import com.ohos.hapsigntool.error.ERROR;
import com.ohos.hapsigntool.error.SignToolErrMsg;
import com.ohos.hapsigntool.utils.CertUtils;

import com.ohos.hapsigntool.utils.LogUtils;
import org.bouncycastle.asn1.ASN1ObjectIdentifier;
import org.bouncycastle.asn1.x500.X500Name;
import org.bouncycastle.asn1.x509.BasicConstraints;
import org.bouncycastle.asn1.x509.ExtendedKeyUsage;
import org.bouncycastle.asn1.x509.Extension;
import org.bouncycastle.asn1.x509.KeyPurposeId;
import org.bouncycastle.asn1.x509.KeyUsage;
import org.bouncycastle.asn1.x509.SubjectPublicKeyInfo;
import org.bouncycastle.cert.CertIOException;
import org.bouncycastle.cert.X509v3CertificateBuilder;
import org.bouncycastle.cert.jcajce.JcaX509CertificateConverter;
import org.bouncycastle.cert.jcajce.JcaX509ExtensionUtils;
import org.bouncycastle.jce.provider.BouncyCastleProvider;
import org.bouncycastle.operator.ContentSigner;
import org.bouncycastle.pkcs.PKCS10CertificationRequest;

import java.io.IOException;
import java.security.InvalidKeyException;
import java.security.KeyPair;
import java.security.NoSuchAlgorithmException;
import java.security.NoSuchProviderException;
import java.security.SignatureException;
import java.security.cert.CertificateException;
import java.security.cert.X509Certificate;
import java.time.LocalDateTime;
import java.time.ZoneId;
import java.util.Date;

/**
 * Builder pattern to build certification.
 *
 * @since 2021/12/28
 */
public class CertBuilder {
    /**
     * Logger.
     */
    private static final LogUtils LOGGER = new LogUtils(ServiceApi.class);

    /**
     * issuer keyPair.
     */
    private final KeyPair keyPair;

    /**
     * CertificateBuilder.
     */
    private final X509v3CertificateBuilder x509v3CertificateBuilder;

    /**
     * CertBuilder.
     *
     * @param keyPair    keyPair
     * @param issuer     issuer
     * @param csr        csr
     * @param certExpire certExpire
     */
    public CertBuilder(KeyPair keyPair, X500Name issuer, byte[] csr, long certExpire) {
        this.keyPair = keyPair;
        LocalDateTime notBefore = LocalDateTime.now();
        LocalDateTime notAfter = notBefore.plusDays(certExpire);

        PKCS10CertificationRequest request = null;
        try {
            request = new PKCS10CertificationRequest(csr);
        } catch (IOException exception) {
            LOGGER.debug(exception.getMessage(), exception);
            CustomException.throwException(ERROR.IO_CSR_ERROR, SignToolErrMsg.IO_CSR_ERROR
                    .toString(exception.getMessage()));
        }
        x509v3CertificateBuilder = new X509v3CertificateBuilder(
                issuer, CertUtils.randomSerial(), Date.from(notBefore.atZone(ZoneId.systemDefault()).toInstant()),
                Date.from(notAfter.atZone(ZoneId.systemDefault()).toInstant()),
                request.getSubject(), request.getSubjectPublicKeyInfo());
        try {
            JcaX509ExtensionUtils extUtils = new JcaX509ExtensionUtils();
            x509v3CertificateBuilder.addExtension(Extension.subjectKeyIdentifier, false,
                    extUtils.createSubjectKeyIdentifier(request.getSubjectPublicKeyInfo()));
        } catch (NoSuchAlgorithmException exception) {
            LOGGER.debug(exception.getMessage(), exception);
            CustomException.throwException(ERROR.NOT_SUPPORT_ERROR, SignToolErrMsg.NO_SUCH_SIGNATURE
                    .toString(exception.getMessage()));
        } catch (CertIOException exception) {
            LOGGER.debug(exception.getMessage(), exception);
            CustomException.throwException(ERROR.IO_CERT_ERROR, SignToolErrMsg.CERT_IO_FAILED
                    .toString(exception.getMessage()));
        }
    }

    /**
     * Add authorityKeyIdentifier for certificate builder.
     *
     * @param certLevel certLevel
     * @return CertBuilder
     */
    public CertBuilder withAuthorityKeyIdentifier(CertLevel certLevel) {
        try {
            JcaX509ExtensionUtils extUtils = new JcaX509ExtensionUtils();
            if (certLevel == CertLevel.SUB_CA) {
                x509v3CertificateBuilder.addExtension(Extension.authorityKeyIdentifier, false,
                        extUtils.createAuthorityKeyIdentifier(SubjectPublicKeyInfo
                                .getInstance(keyPair.getPublic().getEncoded())));
            }
        } catch (NoSuchAlgorithmException exception) {
            LOGGER.debug(exception.getMessage(), exception);
            CustomException.throwException(ERROR.NOT_SUPPORT_ERROR, SignToolErrMsg.NO_SUCH_SIGNATURE
                    .toString(exception.getMessage()));
        } catch (CertIOException exception) {
            LOGGER.debug(exception.getMessage(), exception);
            CustomException.throwException(ERROR.IO_CERT_ERROR, SignToolErrMsg.CERT_IO_FAILED
                    .toString(exception.getMessage()));
        }
        return this;
    }

    /**
     * Add basicConstraints for certificate builder.
     *
     * @param certLevel                certLevel
     * @param basicConstraintsCritical basicConstraintsCritical
     * @param basicConstraintsCa       basicConstraintsCa
     * @param basicConstraintsPathLen  basicConstraintsPathLen
     * @return CertBuilder
     * @throws CertIOException CertIOException
     */
    public CertBuilder withBasicConstraints(CertLevel certLevel, boolean basicConstraintsCritical,
                                            boolean basicConstraintsCa, Integer basicConstraintsPathLen)
            throws CertIOException {
        BasicConstraints basicConstraints;
        if (certLevel == CertLevel.END_ENTITY) {
            basicConstraints = new BasicConstraints(basicConstraintsCritical);
        } else {
            if (basicConstraintsPathLen == null) {
                basicConstraints = new BasicConstraints(basicConstraintsCa);
            } else {
                basicConstraints = new BasicConstraints(basicConstraintsPathLen);
            }
        }
        x509v3CertificateBuilder.addExtension(Extension.basicConstraints, basicConstraintsCritical, basicConstraints);
        return this;
    }

    /**
     * Add keyUsages for certificate builder.
     *
     * @param keyUsage         keyUsage
     * @param keyUsageCritical keyUsageCritical
     * @return CertBuilder
     * @throws CertIOException CertIOException
     */
    public CertBuilder withKeyUsages(KeyUsage keyUsage, boolean keyUsageCritical) throws CertIOException {
        x509v3CertificateBuilder.addExtension(Extension.keyUsage, keyUsageCritical, keyUsage);
        return this;
    }

    /**
     * Add extKeyUsages for certificate builder.
     *
     * @param extKeyUsages        extKeyUsages
     * @param extKeyUsageCritical extKeyUsageCritical
     * @return CertBuilder
     * @throws CertIOException CertIOException
     */
    public CertBuilder withExtKeyUsages(KeyPurposeId[] extKeyUsages, boolean extKeyUsageCritical)
            throws CertIOException {
        if (extKeyUsages != null) {
            ExtendedKeyUsage extendedKeyUsage = new ExtendedKeyUsage(extKeyUsages);
            x509v3CertificateBuilder.addExtension(Extension.extendedKeyUsage, extKeyUsageCritical, extendedKeyUsage);
        }
        return this;
    }

    /**
     * Add signingCapabilty for certificate builder.
     *
     * @param signingCapabiltyBytes signingCapabiltyBytes
     * @return CertBuilder
     * @throws CertIOException CertIOException
     */
    public CertBuilder withSigningCapabilty(byte[] signingCapabiltyBytes) throws CertIOException {
        ASN1ObjectIdentifier signingCapabiltyIdentifier = (new ASN1ObjectIdentifier("1.3.6.1.4.1.2011.2.376.1.3"))
                .intern();
        x509v3CertificateBuilder.addExtension(signingCapabiltyIdentifier, false, signingCapabiltyBytes);
        return this;
    }

    /**
     * build X509Certificate.
     *
     * @param signAlgorithm signAlgorithm to sign
     * @return X509Certificate
     */
    public X509Certificate build(String signAlgorithm) {
        ContentSigner contentSigner = CertUtils.createFixedContentSigner(keyPair.getPrivate(), signAlgorithm);
        X509Certificate cert = null;
        try {
            cert = new JcaX509CertificateConverter().setProvider(BouncyCastleProvider.PROVIDER_NAME)
                    .getCertificate(x509v3CertificateBuilder.build(contentSigner));
            cert.verify(keyPair.getPublic());
        } catch (InvalidKeyException exception) {
            LOGGER.debug(exception.getMessage(), exception);
            CustomException.throwException(ERROR.KEY_ERROR, SignToolErrMsg.INVALID_KEY
                    .toString(exception.getMessage()));
        } catch (SignatureException exception) {
            LOGGER.debug(exception.getMessage(), exception);
            CustomException.throwException(ERROR.SIGN_ERROR, SignToolErrMsg.SIGNATURE_FAILED
                    .toString(exception.getMessage()));
        } catch (CertificateException exception) {
            LOGGER.debug(exception.getMessage(), exception);
            CustomException.throwException(ERROR.CERTIFICATE_ERROR, SignToolErrMsg.CERT_IO_FAILED
                    .toString(exception.getMessage()));
        } catch (NoSuchAlgorithmException | NoSuchProviderException exception) {
            LOGGER.debug(exception.getMessage(), exception);
            CustomException.throwException(ERROR.NOT_SUPPORT_ERROR, SignToolErrMsg.NO_SUCH_SIGNATURE
                    .toString(exception.getMessage()));
        }
        return cert;
    }
}
