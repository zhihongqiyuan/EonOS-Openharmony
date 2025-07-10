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

package com.ohos.hapsigntool.utils;

import java.io.ByteArrayInputStream;
import java.io.UnsupportedEncodingException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.security.cert.CRL;
import java.security.cert.CRLException;
import java.security.cert.Certificate;
import java.security.cert.CertificateException;
import java.security.cert.CertificateFactory;
import java.security.cert.X509CRL;
import java.security.cert.X509Certificate;
import java.util.Base64;

/**
 * Digest util class
 *
 * @since 2021-12-13
 */
public class DigestUtils {
    private static final LogUtils LOGGER = new LogUtils(DigestUtils.class);

    /**
     * Constructor of Method
     */
    private DigestUtils() {
    }

    /**
     * digest the inputContent with SHA-256
     *
     * @param inputContentArray input Content Array
     * @return the result of digest with SHA-256
     */
    public static byte[] sha256Digest(byte[] inputContentArray) {
        byte[] sha2Array = null;
        try {
            MessageDigest md = MessageDigest.getInstance("SHA-256");
            md.update(inputContentArray);
            sha2Array = md.digest();
        } catch (NoSuchAlgorithmException e) {
            LOGGER.error("don't has SHA-256 Algorithm");
        }
        return sha2Array;
    }

    /**
     * digest the inputContent with SHA-384
     *
     * @param inputContentArray input Content Array
     * @return the result of digest with SHA-384
     */
    public static byte[] sha384Digest(byte[] inputContentArray) {
        byte[] sha2Array = null;
        try {
            MessageDigest md = MessageDigest.getInstance("SHA-384");
            md.update(inputContentArray);
            sha2Array = md.digest();
        } catch (NoSuchAlgorithmException e) {
            LOGGER.error("don't has SHA-384 Algorithm");
        }
        return sha2Array;
    }

    /**
     * digest the inputContent with SHA-512
     *
     * @param inputContentArray input Content Array
     * @return the result of digest with SHA-512
     */
    public static byte[] sha512Digest(byte[] inputContentArray) {
        byte[] sha2Array = null;
        try {
            MessageDigest md = MessageDigest.getInstance("SHA-512");
            md.update(inputContentArray);
            sha2Array = md.digest();
        } catch (NoSuchAlgorithmException e) {
            LOGGER.error("don't has SHA-512 Algorithm");
        }
        return sha2Array;
    }

    /**
     * Transform a string of certificate with base64 encoded to an object of X509Certificate.
     *
     * @param encodeString string of certificate with base64 encoded
     * @return object of X509Certificate
     */
    public static X509Certificate decodeBase64ToX509Certifate(String encodeString) {
        String header = "-----BEGIN CERTIFICATE-----\n";
        String tail = "-----END CERTIFICATE-----\n";
        byte[] certificateDatas = null;
        X509Certificate x509Certificate = null;
        try {
            CertificateFactory cf = CertificateFactory.getInstance("X.509");
            if (encodeString.startsWith(header) && encodeString.endsWith(tail)) {
                certificateDatas = encodeString.getBytes("UTF-8");
            } else {
                certificateDatas = Base64.getUrlDecoder().decode(encodeString);
            }

            Certificate obj = cf.generateCertificate(new ByteArrayInputStream(certificateDatas));
            if (!(obj instanceof X509Certificate)) {
                LOGGER.error("generateCertificate is not x509");
                return x509Certificate;
            }
            x509Certificate = (X509Certificate) obj;
        } catch (UnsupportedEncodingException | CertificateException e) {
            LOGGER.error("Decode Base64 certificate failed!", e);
        }
        return x509Certificate;
    }

    /**
     * Get an object of x509CRL from a string of certificate with base64 encoded
     *
     * @param encodeString string of certificate with base64 encoded
     * @return an object of x509CRL
     */
    public static X509CRL decodeBase64ToX509CRL(String encodeString) {
        byte[] certificateDatas = Base64.getUrlDecoder().decode(encodeString);

        X509CRL x509CRL = null;
        try {
            CertificateFactory cf = CertificateFactory.getInstance("X.509");
            CRL obj = cf.generateCRL(new ByteArrayInputStream(certificateDatas));
            if (!(obj instanceof X509CRL)) {
                LOGGER.error("generateCRL is not x509");
                return x509CRL;
            }
            x509CRL = (X509CRL) obj;
        } catch (CertificateException | CRLException e) {
            LOGGER.error("Decode Base64 crl failed!");
        }
        return x509CRL;
    }
}
