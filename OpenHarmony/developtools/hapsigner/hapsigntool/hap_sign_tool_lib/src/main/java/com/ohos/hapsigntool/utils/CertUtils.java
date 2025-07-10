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

import com.ohos.hapsigntool.error.CustomException;
import com.ohos.hapsigntool.error.ERROR;
import com.ohos.hapsigntool.error.SignToolErrMsg;
import com.ohos.hapsigntool.error.VerifyCertificateChainException;

import org.bouncycastle.asn1.x500.X500Name;
import org.bouncycastle.asn1.x509.KeyPurposeId;
import org.bouncycastle.asn1.x509.KeyUsage;
import org.bouncycastle.jce.provider.BouncyCastleProvider;
import org.bouncycastle.operator.ContentSigner;
import org.bouncycastle.operator.OperatorCreationException;
import org.bouncycastle.operator.jcajce.JcaContentSignerBuilder;

import javax.security.auth.x500.X500Principal;
import java.io.ByteArrayInputStream;
import java.math.BigInteger;
import java.nio.charset.StandardCharsets;
import java.security.NoSuchAlgorithmException;
import java.security.PrivateKey;
import java.security.SecureRandom;
import java.security.cert.CertificateEncodingException;
import java.security.cert.CertificateException;
import java.security.cert.CertificateFactory;
import java.security.cert.X509Certificate;
import java.security.interfaces.ECPrivateKey;
import java.security.interfaces.RSAPrivateKey;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Cert Usage Util.
 *
 * @since 2021/12/28
 */
public final class CertUtils {
    /**
     * Logger.
     */
    private static final LogUtils LOGGER = new LogUtils(CertUtils.class);

    /**
     * Max length to print certificate string.
     */
    private static final int MAX_LINE_LENGTH = 65;

    /**
     * Length of serial security random number.
     */
    private static final int RANDOM_SERIAL_LENGTH = 32;

    /**
     * number constant.
     */
    private static final int SECOND_INDEX = 2;

    /**
     * ECC.
     */
    private static final String ECC = "ECDSA";

    /**
     * Compile String.
     */
    private static final Pattern SIGN_ALGORITHM_PATTERN = Pattern.compile("^SHA([0-9]{3})with([A-Z]{1,5})$");

    private CertUtils() {}

    /**
     * Parse string to key usage.
     *
     * @param keyUsageStr Key usage string
     * @return Key usage
     */
    public static int parseKeyUsage(String keyUsageStr) {
        int keyUsage = 0;
        if (keyUsageStr.contains("digitalSignature")) {
            keyUsage |= KeyUsage.digitalSignature;
        }
        if (keyUsageStr.contains("nonRepudiation")) {
            keyUsage |= KeyUsage.nonRepudiation;
        }
        if (keyUsageStr.contains("keyEncipherment")) {
            keyUsage |= KeyUsage.keyEncipherment;
        }
        if (keyUsageStr.contains("dataEncipherment")) {
            keyUsage |= KeyUsage.dataEncipherment;
        }
        if (keyUsageStr.contains("keyAgreement")) {
            keyUsage |= KeyUsage.keyAgreement;
        }
        if (keyUsageStr.contains("certificateSignature")) {
            keyUsage |= KeyUsage.keyCertSign;
        }
        if (keyUsageStr.contains("crlSignature")) {
            keyUsage |= KeyUsage.cRLSign;
        }
        if (keyUsageStr.contains("encipherOnly")) {
            keyUsage |= KeyUsage.encipherOnly;
        }
        if (keyUsageStr.contains("decipherOnly")) {
            keyUsage |= KeyUsage.decipherOnly;
        }
        return keyUsage;
    }

    /**
     * Parse string to KeyPurposeId[]
     *
     * @param extKeyUsageStr ext key usage string
     * @return KeyPurposeId[]
     */
    public static KeyPurposeId[] parseExtKeyUsage(String extKeyUsageStr) {
        ArrayList<KeyPurposeId> ids = new ArrayList<>();
        if (extKeyUsageStr.contains("clientAuthentication")) {
            ids.add(KeyPurposeId.id_kp_clientAuth);
        }
        if (extKeyUsageStr.contains("serverAuthentication")) {
            ids.add(KeyPurposeId.id_kp_serverAuth);
        }
        if (extKeyUsageStr.contains("codeSignature")) {
            ids.add(KeyPurposeId.id_kp_codeSigning);
        }
        if (extKeyUsageStr.contains("emailProtection")) {
            ids.add(KeyPurposeId.id_kp_emailProtection);
        }
        if (extKeyUsageStr.contains("smartCardLogin")) {
            ids.add(KeyPurposeId.id_kp_smartcardlogon);
        }
        if (extKeyUsageStr.contains("timestamp")) {
            ids.add(KeyPurposeId.id_kp_timeStamping);
        }
        if (extKeyUsageStr.contains("ocspSignature")) {
            ids.add(KeyPurposeId.id_kp_OCSPSigning);
        }
        return ids.toArray(new KeyPurposeId[]{});
    }

    /**
     * buildDN
     *
     * @param nameString nameString
     * @return X500Name
     */
    public static X500Name buildDN(String nameString) {
        checkDN(nameString);
        X500Name dn = null;
        try {
            dn = new X500Name(nameString);
        } catch (IllegalArgumentException | IndexOutOfBoundsException exception) {
            LOGGER.debug(exception.getMessage(), exception);
            CustomException.throwException(ERROR.COMMAND_ERROR,
                    String.format("Error params near: %s. Reason: %s", nameString, exception.getMessage()));
        }
        return dn;
    }

    /**
     * To verify the format of subject or issuer.
     * Refer to X500NameStyle.fromString().
     *
     * @param nameString subject or issuer
     */
    private static void checkDN(String nameString) {
        String errorMsg = SignToolErrMsg.CERT_DN_FORMAT_FAILED.toString(nameString);
        ValidateUtils.throwIfNotMatches(!StringUtils.isEmpty(nameString), ERROR.COMMAND_ERROR, errorMsg);
        String[] pairs = nameString.split(",");
        for (String pair : pairs) {
            ValidateUtils.throwIfNotMatches(!StringUtils.isEmpty(nameString.trim()), ERROR.COMMAND_ERROR, errorMsg);
            String[] kvPair = pair.split("=");
            ValidateUtils.throwIfNotMatches(kvPair.length == SECOND_INDEX, ERROR.COMMAND_ERROR, errorMsg);
            // Key will be checked in X500NameStyle.attrNameToOID
            ValidateUtils.throwIfNotMatches(!StringUtils.isEmpty(kvPair[1].trim()), ERROR.COMMAND_ERROR, errorMsg);
        }
    }

    /**
     * Convert byte to CSR String.
     *
     * @param csr bytes of CSR
     * @return String
     */
    public static String toCsrTemplate(byte[] csr) {
        return "-----BEGIN NEW CERTIFICATE REQUEST-----\n"
                + java.util.Base64.getMimeEncoder(MAX_LINE_LENGTH, "\n".getBytes(StandardCharsets.UTF_8))
                .encodeToString(csr)
                + "\n-----END NEW CERTIFICATE REQUEST-----\n";
    }

    /**
     * Encoding cert to String.
     *
     * @param certificate Cert to convert to string
     * @return Cert templated string
     * @throws CertificateEncodingException Failed encoding
     */
    public static String generateCertificateInCer(X509Certificate certificate)
            throws CertificateEncodingException {
        return "-----BEGIN CERTIFICATE-----\n"
                + java.util.Base64.getMimeEncoder(MAX_LINE_LENGTH, "\n".getBytes(StandardCharsets.UTF_8))
                .encodeToString(certificate.getEncoded())
                + "\n" + "-----END CERTIFICATE-----" + "\n";
    }

    /**
     * Random serial.
     *
     * @return Random big integer
     */
    public static BigInteger randomSerial() {
        try {
            return new BigInteger(RANDOM_SERIAL_LENGTH, SecureRandom.getInstanceStrong());
        } catch (NoSuchAlgorithmException e) {
            return new BigInteger(RANDOM_SERIAL_LENGTH, new SecureRandom());
        }
    }

    /**
     * Convert byte to cert.
     *
     * @param cert Byte from cert file
     * @return Certs
     * @throws CertificateException Convert failed
     * @throws VerifyCertificateChainException certificates in file are not certificate chain
     */
    @SuppressWarnings("unchecked")
    public static List<X509Certificate> generateCertificates(byte[] cert) throws CertificateException,
            VerifyCertificateChainException {
        CertificateFactory factory = CertificateFactory.getInstance("X.509");
        List<X509Certificate> certificates =
                (List<X509Certificate>) factory.generateCertificates(new ByteArrayInputStream(cert));
        sortCertificateChain(certificates);
        CertificateUtils.verifyCertChain(certificates);
        return certificates;
    }

    /**
     * Sort cert chain to sign cert, sub cert, root cert
     *
     * @param certificates cert chain
     */
    public static void sortCertificateChain(List<X509Certificate> certificates) {
        if (certificates != null && certificates.size() > 1) {
            int size = certificates.size();
            X500Principal lastSubjectX500Principal = (certificates.get(size - 1)).getSubjectX500Principal();
            X500Principal beforeIssuerX500Principal = (certificates.get(size - SECOND_INDEX)).getIssuerX500Principal();
            if (!lastSubjectX500Principal.equals(beforeIssuerX500Principal)) {
                Collections.reverse(certificates);
            }
        }
    }

    /**
     * Auto fix algorithm according key type and create content signer.
     *
     * @param privateKey    Sign key
     * @param signAlgorithm Sign algorithm
     * @return ContentSigner
     */
    public static ContentSigner createFixedContentSigner(PrivateKey privateKey, String signAlgorithm) {
        Matcher matcher = SIGN_ALGORITHM_PATTERN.matcher(signAlgorithm);
        ValidateUtils.throwIfNotMatches(matcher.matches(), ERROR.NOT_SUPPORT_ERROR, SignToolErrMsg.ALGORITHM_NOT_SUPPORT
                .toString("Not Support " + signAlgorithm));
        String signAlg = signAlgorithm;
        // Auto fix signAlgorithm error
        if (privateKey instanceof ECPrivateKey && signAlgorithm.contains("RSA")) {
            signAlg = signAlgorithm.replace("RSA", ECC);
        } else {
            if (privateKey instanceof RSAPrivateKey && signAlgorithm.contains(ECC)) {
                signAlg = signAlgorithm.replace(ECC, "RSA");
            }
        }

        JcaContentSignerBuilder jcaContentSignerBuilder = new JcaContentSignerBuilder(signAlg);
        jcaContentSignerBuilder.setProvider(BouncyCastleProvider.PROVIDER_NAME);
        try {
            return jcaContentSignerBuilder.build(privateKey);
        } catch (OperatorCreationException exception) {
            LOGGER.debug(exception.getMessage(), exception);
            CustomException.throwException(ERROR.OPERATOR_CREATION_ERROR, SignToolErrMsg.CERT_IO_FAILED
                    .toString(exception.getMessage()));
        }
        return null;
    }
}
