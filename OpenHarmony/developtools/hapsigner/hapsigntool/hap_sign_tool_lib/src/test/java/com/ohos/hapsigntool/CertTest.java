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

package com.ohos.hapsigntool;

import com.ohos.hapsigntool.adapter.LocalizationAdapter;
import com.ohos.hapsigntool.entity.Options;
import com.ohos.hapsigntool.api.CertTools;
import com.ohos.hapsigntool.error.CustomException;
import com.ohos.hapsigntool.utils.KeyPairTools;
import com.ohos.hapsigntool.utils.CertUtils;
import org.bouncycastle.asn1.x500.X500Name;
import org.bouncycastle.jce.provider.BouncyCastleProvider;
import org.junit.jupiter.api.MethodOrderer;
import org.junit.jupiter.api.Order;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.TestMethodOrder;
import org.junit.platform.commons.logging.Logger;
import org.junit.platform.commons.logging.LoggerFactory;

import java.security.InvalidKeyException;
import java.security.KeyPair;
import java.security.NoSuchAlgorithmException;
import java.security.NoSuchProviderException;
import java.security.Security;
import java.security.SignatureException;
import java.security.cert.CertificateException;
import java.security.cert.CertificateExpiredException;
import java.security.cert.CertificateNotYetValidException;
import java.security.cert.X509Certificate;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertTrue;

/**
 * Test various certificate generation status.
 *
 * @since 2021/12/28
 */
@TestMethodOrder(MethodOrderer.OrderAnnotation.class)
public class CertTest {
    /**
     * The certificate is valid for 365 days.
     */
    public static final int VALIDITY_365 = 365;

    /**
     * Params SHA384withRSA.
     */
    public static final String SHA_384_WITH_RSA = "SHA384withRSA";

    /**
     * Params CN=Application Signature Service CA.
     */
    public static final String APP_CA = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,"
            + "CN=Application Signature Service CA";

    /**
     * Params CN=App1 Release.
     */
    public static final String APP1_RELEASE = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";

    /**
     * App signing Capabilty Bytes.
     */
    private static final byte[] APP_SIGNING_CAPABILITY = {0x30, 0x06, 0x02, 0x01, 0x01, 0x0A, 0x01, 0x00};

    /**
     * Add log info.
     */
    private static final Logger logger = LoggerFactory.getLogger(CertTest.class);

    static {
        Security.addProvider(new BouncyCastleProvider());
    }

    /**
     * Generate keystore file.
     */
    private final KeyPair keyPair = KeyPairTools.generateKeyPair(KeyPairTools.RSA, KeyPairTools.RSA_2048);

    /**
     * test RootCaCert
     */
    @Order(1)
    @Test
    public void testRootCaCert() {
        String subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN= Root CA";
        X500Name subName = CertUtils.buildDN(subject);
        byte[] csr = generateCsrParameters(subName);

        Options options = new Options();
        options.put(Options.ISSUER, subject);
        options.put(Options.SIGN_ALG, SHA_384_WITH_RSA);
        options.put(Options.VALIDITY, VALIDITY_365);
        options.put(Options.BASIC_CONSTRAINTS_PATH_LEN, 0);
        X509Certificate rootCaCert = CertTools.generateRootCaCert(keyPair, csr, new LocalizationAdapter(options));

        try {
            rootCaCert.verify(keyPair.getPublic());
        } catch (CertificateException | NoSuchAlgorithmException | InvalidKeyException
                | NoSuchProviderException | SignatureException exception) {
            logger.error(exception, () -> exception.getMessage());
        }
        assertNotNull(rootCaCert);
        try {
            rootCaCert.checkValidity();
        } catch (CertificateExpiredException | CertificateNotYetValidException exception) {
            logger.error(exception, () -> exception.getMessage());
        }
        assertEquals(subName.toString(), rootCaCert.getSubjectDN().toString());
        assertTrue(SHA_384_WITH_RSA.equalsIgnoreCase(rootCaCert.getSigAlgName()));

        try {
            options.put(Options.ISSUER, null);
            options.put(Options.SIGN_ALG, "");
            rootCaCert = CertTools.generateRootCaCert(keyPair, csr, new LocalizationAdapter(options));
            assertNull(rootCaCert);
        } catch (CustomException exception) {
            logger.error(exception, () -> exception.getMessage());
        }
    }

    /**
     * test subCaCert
     */
    @Order(2)
    @Test
    public void testSubCaCert() {
        X500Name subName = CertUtils.buildDN(APP1_RELEASE);
        byte[] csr = generateCsrParameters(subName);
        Options options = new Options();
        options.put(Options.SUBJECT, APP1_RELEASE);
        options.put(Options.ISSUER, APP_CA);
        options.put(Options.SIGN_ALG, SHA_384_WITH_RSA);
        options.put(Options.VALIDITY, VALIDITY_365);
        options.put(Options.BASIC_CONSTRAINTS_PATH_LEN, 0);
        X509Certificate subCaCert = CertTools.generateSubCert(keyPair, csr, new LocalizationAdapter(options));
        try {
            subCaCert.verify(keyPair.getPublic());
        } catch (CertificateException | NoSuchAlgorithmException | InvalidKeyException
                | NoSuchProviderException | SignatureException exception) {
            logger.info(exception, () -> exception.getMessage());
        }
        assertNotNull(subCaCert);
        try {
            subCaCert.checkValidity();
        } catch (CertificateExpiredException | CertificateNotYetValidException exception) {
            logger.info(exception, () -> exception.getMessage());
        }
        assertEquals(subName.toString(), subCaCert.getSubjectDN().toString());
        assertTrue(SHA_384_WITH_RSA.equalsIgnoreCase(subCaCert.getSigAlgName()));

        try {
            options.put(Options.ISSUER, null);
            options.put(Options.SIGN_ALG, "");
            subCaCert = CertTools.generateSubCert(keyPair, csr, new LocalizationAdapter(options));
            assertNull(subCaCert);
        } catch (CustomException exception) {
            logger.info(exception, () -> exception.getMessage());
        }
    }

    /**
     * test appCert
     */
    @Order(3)
    @Test
    public void testAppCert() {
        X500Name subName = CertUtils.buildDN(APP1_RELEASE);
        byte[] csr = generateCsrParameters(subName);
        Options options = new Options();
        options.put(Options.SUBJECT, APP1_RELEASE);
        options.put(Options.ISSUER, APP_CA);
        options.put(Options.SIGN_ALG, SHA_384_WITH_RSA);
        options.put(Options.VALIDITY, VALIDITY_365);
        X509Certificate appCert = CertTools.generateEndCert(keyPair, csr, new LocalizationAdapter(options),
                APP_SIGNING_CAPABILITY);
        try {
            appCert.verify(keyPair.getPublic());
        } catch (CertificateException | NoSuchAlgorithmException | InvalidKeyException
                | NoSuchProviderException | SignatureException exception) {
            logger.info(exception, () -> exception.getMessage());
        }
        assertNotNull(appCert);
        try {
            appCert.checkValidity();
        } catch (CertificateExpiredException | CertificateNotYetValidException exception) {
            logger.info(exception, () -> exception.getMessage());
        }
        assertEquals(subName.toString(), appCert.getSubjectDN().toString());
        assertTrue(SHA_384_WITH_RSA.equalsIgnoreCase(appCert.getSigAlgName()));

        try {
            options.put(Options.ISSUER, null);
            options.put(Options.SIGN_ALG, "");
            appCert = CertTools.generateEndCert(keyPair, csr, new LocalizationAdapter(options), APP_SIGNING_CAPABILITY);
            assertNull(appCert);
        } catch (CustomException exception) {
            logger.info(exception, () -> exception.getMessage());
        }
    }

    /**
     * test csrTemplate
     */
    @Order(4)
    @Test
    public void testCsrTemplate() {
        X500Name name = new X500Name(APP1_RELEASE);
        byte[] csr = generateCsrParameters(name);
        assertNotNull(csr);
        assertTrue(csr.length > 0);
        String csrTemplate = CertUtils.toCsrTemplate(csr);
        assertNotNull(csrTemplate);
        assertTrue(csrTemplate.startsWith("-----BEGIN NEW CERTIFICATE REQUEST-----\n"));
        assertTrue(csrTemplate.endsWith("\n-----END NEW CERTIFICATE REQUEST-----\n"));
    }

    private byte[] generateCsrParameters(X500Name name) {
        return CertTools.generateCsr(keyPair, SHA_384_WITH_RSA, name);
    }
}
