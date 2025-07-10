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
import org.bouncycastle.asn1.x500.X500NameBuilder;
import org.bouncycastle.asn1.x500.style.BCStyle;
import org.bouncycastle.cert.X509v3CertificateBuilder;
import org.bouncycastle.cert.jcajce.JcaX509CertificateConverter;
import org.bouncycastle.cert.jcajce.JcaX509v3CertificateBuilder;
import org.bouncycastle.operator.ContentSigner;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.security.Key;
import java.security.KeyPair;
import java.security.KeyStore;
import java.security.KeyStoreException;
import java.security.NoSuchAlgorithmException;
import java.security.PrivateKey;
import java.security.UnrecoverableKeyException;
import java.security.cert.Certificate;
import java.security.cert.CertificateException;
import java.security.cert.CertificateExpiredException;
import java.security.cert.CertificateNotYetValidException;
import java.security.cert.X509Certificate;
import java.time.LocalDateTime;
import java.time.ZoneId;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

/**
 * Read and save Keypair and certificate.
 *
 * @since 2021/12/28
 */
public class KeyStoreHelper {
    /**
     * Field KEYSTORE_TYPE_PKCS12.
     */
    private static final String KEYSTORE_TYPE_PKCS12 = "pkcs12";

    /**
     * Field KEYSTORE_TYPE_JKS.
     */
    private static final String KEYSTORE_TYPE_JKS = "jks";

    /**
     * Field FILE_TYPE_JKS.
     */
    private static final String FILE_TYPE_JKS = "jks";

    /**
     * Field FILE_TYPE_PKCS12.
     */
    private static final String FILE_TYPE_PKCS12 = "p12";

    /**
     * Field number 100.
     */
    private static final int NUM_ONE_HUNDRED = 100;

    /**
     * Use LogManager to show log instead of use "System.out.print" to show log.
     */
    private static final LogUtils LOGGER = new LogUtils(KeyStoreHelper.class);

    /**
     * Field keyStorePath.
     */
    private final String keyStorePath;

    /**
     * Field keyStorePwd.
     */
    private final char[] keyStorePwd;

    /**
     * Field keyStore.
     */
    private final KeyStore keyStore;

    /**
     * Helper to load and save pair.
     *
     * @param keyStorePath File path
     * @param storePwd passwd of key store
     */
    public KeyStoreHelper(String keyStorePath, char[] storePwd) {
        char[] pwd = storePwd;
        ValidateUtils.throwIfMatches(StringUtils.isEmpty(keyStorePath),
                ERROR.COMMAND_ERROR, SignToolErrMsg.MISSING_PARAM.toString("keyStorePath"));
        if (pwd == null) {
            pwd = new char[0];
        }
        this.keyStorePwd = pwd;
        this.keyStorePath = keyStorePath;
        this.keyStore = createKeyStoreAccordingFileType(keyStorePath);
        FileInputStream fis = null;
        try {
            if (FileUtils.isFileExist(keyStorePath)) {
                LOGGER.info("{} is exist. Try to load it with given passwd", keyStorePath);
                fis = new FileInputStream(keyStorePath);
                keyStore.load(fis, pwd);
            } else {
                keyStore.load(null, null);
            }
        } catch (IOException | NoSuchAlgorithmException | CertificateException exception) {
            LOGGER.debug(exception.getMessage(), exception);
             CustomException.throwException(ERROR.ACCESS_ERROR, SignToolErrMsg.INIT_KEYSTORE_FAILED
                    .toString(exception.getMessage()));
        } finally {
            FileUtils.close(fis);
        }
    }

    public String getKeyStorePath() {
        return keyStorePath;
    }

    /**
     * Throw exception if alias exist in keystore.
     *
     * @param alias alias of key
     */
    public void errorOnExist(String alias) {
        ValidateUtils.throwIfMatches(this.hasAlias(alias),
                ERROR.ACCESS_ERROR, SignToolErrMsg.KEY_ALIAS_EXIST.toString(alias, this.keyStorePath));
    }

    /**
     * Throw exception if alias no exist in keystore.
     *
     * @param alias alias of key
     */
    public void errorIfNotExist(String alias) {
        ValidateUtils.throwIfNotMatches(this.hasAlias(alias),
                ERROR.FILE_NOT_FOUND, SignToolErrMsg.KEY_ALIAS_NOT_FOUND.toString(alias, this.keyStorePath));
    }

    /**
     * Check if keystore contain the alias.
     *
     * @param alias key alias
     * @return result
     */
    public boolean hasAlias(String alias) {
        try {
            return keyStore.containsAlias(alias);
        } catch (KeyStoreException exception) {
            LOGGER.debug(exception.getMessage(), exception);
            CustomException.throwException(ERROR.ACCESS_ERROR, SignToolErrMsg.KEYSTORE_ERROR
                    .toString(exception.getMessage()));
            return false;
        }
    }

    /**
     * Load keypair form keystore.
     *
     * @param alias alias
     * @param certPwd key pwd
     * @return Keypair
     */
    public KeyPair loadKeyPair(String alias, char[] certPwd) {
        List<X509Certificate> certificates = loadCertificates(alias);
        PrivateKey privateKey = loadPrivateKey(alias, certPwd);
        return new KeyPair(certificates.get(0).getPublicKey(), privateKey);
    }

    /**
     * Get private key from give key store
     *
     * @param alias   Cert alias
     * @param certPwd Cert pwd
     * @return private key
     */
    public PrivateKey loadPrivateKey(String alias, char[] certPwd) {
        char[] pwd = certPwd;
        if (pwd == null) {
            pwd = new char[0];
        }
        try {
            Key key = keyStore.getKey(alias, pwd);
            if (key instanceof PrivateKey) {
                return (PrivateKey) key;
            }
        } catch (KeyStoreException | NoSuchAlgorithmException exception) {
            LOGGER.debug(exception.getMessage(), exception);
            CustomException.throwException(ERROR.ACCESS_ERROR, SignToolErrMsg.NO_SUCH_SIGNATURE
                    .toString(exception.getMessage()));
        } catch (UnrecoverableKeyException exception) {
            LOGGER.debug(exception.getMessage(), exception);
            CustomException.throwException(ERROR.ACCESS_ERROR, SignToolErrMsg.KEY_PASSWORD_ERROR
                    .toString(alias, exception));
        }
        return null;
    }

    /**
     * Validate the cert and save into cert list.
     *
     * @param certificates Result list to save
     * @param certificate  Cert to validate
     */
    private void putValidCert(List<X509Certificate> certificates, Certificate certificate) {
        if (!(certificate instanceof X509Certificate)) {
            return;
        }
        X509Certificate cert = (X509Certificate) certificate;
        try {
            cert.checkValidity();
        } catch (CertificateExpiredException | CertificateNotYetValidException exception) {
            LOGGER.info("p12's certificates is not valid");
        }
        certificates.add(cert);
    }

    /**
     * Get certificates from keystore.
     *
     * @param alias cert alias
     * @return certificates of alias
     */
    public List<X509Certificate> loadCertificates(String alias) {
        ValidateUtils.throwIfNotMatches(this.hasAlias(alias),
                ERROR.FILE_NOT_FOUND, SignToolErrMsg.KEY_ALIAS_NOT_FOUND.toString(alias, this.keyStorePath));

        List<X509Certificate> certificates = new ArrayList<>();
        try {
            if (keyStore.isKeyEntry(alias)) {
                Certificate[] certs = keyStore.getCertificateChain(alias);
                for (Certificate certificate : certs) {
                    putValidCert(certificates, certificate);
                }
            } else {
                if (keyStore.isCertificateEntry(alias)) {
                    Certificate cert = keyStore.getCertificate(alias);
                    putValidCert(certificates, cert);
                }
            }
        } catch (KeyStoreException exception) {
            LOGGER.debug(exception.getMessage(), exception);
            CustomException.throwException(ERROR.KEYSTORE_OPERATION_ERROR, SignToolErrMsg.KEYSTORE_ERROR
                    .toString(exception.getMessage()));
        }

        ValidateUtils.throwIfNotMatches(!certificates.isEmpty(), ERROR.ACCESS_ERROR, SignToolErrMsg.NO_USABLE_CERT
                .toString(this.keyStorePath));
        return certificates;
    }

    /**
     * Create simple keystore file.
     * Exist if file exist.
     *
     * @param alias   cert alias
     * @param keyPwd  password of String format
     * @param keyPair keypair to save
     * @param certs   will create one if null
     * @return Boolean value.
     */
    public boolean store(String alias, char[] keyPwd, KeyPair keyPair, X509Certificate[] certs) {
        errorOnExist(alias);
        char[] pwd = keyPwd;
        if (pwd == null) {
            pwd = new char[0];
        }
        X509Certificate[] certificates = certs;
        if (certificates == null) {
            X509Certificate certificate = createKeyOnly(keyPair, alias);
            certificates = new X509Certificate[]{certificate};
        }
        try (FileOutputStream fos = new FileOutputStream(keyStorePath)) {
            keyStore.setKeyEntry(alias, keyPair.getPrivate(), pwd, certificates);
            keyStore.store(fos, keyStorePwd);
            fos.flush();
        } catch (KeyStoreException | IOException | NoSuchAlgorithmException | CertificateException exception) {
            LOGGER.debug(exception.getMessage(), exception);
            CustomException.throwException(ERROR.WRITE_FILE_ERROR, SignToolErrMsg.FILE_WRITE_FAILED
                    .toString(exception.getMessage()));
            return false;
        }
        return true;
    }

    private X509Certificate createKeyOnly(KeyPair keyPair, String alias) {
        X500NameBuilder nameBuilder = new X500NameBuilder(BCStyle.INSTANCE);
        nameBuilder.addRDN(BCStyle.CN, alias);

        LocalDateTime notBefore = LocalDateTime.now();
        LocalDateTime notAfter = notBefore.plusYears(NUM_ONE_HUNDRED);

        X509v3CertificateBuilder certificateBuilder = new JcaX509v3CertificateBuilder(
                nameBuilder.build(),
                CertUtils.randomSerial(),
                Date.from(notBefore.atZone(ZoneId.systemDefault()).toInstant()),
                Date.from(notAfter.atZone(ZoneId.systemDefault()).toInstant()),
                nameBuilder.build(),
                keyPair.getPublic()
        );
        ContentSigner contentSigner = CertUtils.createFixedContentSigner(keyPair.getPrivate(), "SHA256withRSA");
        try {
            return new JcaX509CertificateConverter().setProvider("BC")
                    .getCertificate(certificateBuilder.build(contentSigner));
        } catch (CertificateException exception) {
            LOGGER.debug(exception.getMessage(), exception);
            CustomException.throwException(ERROR.IO_CERT_ERROR, SignToolErrMsg.CERT_IO_FAILED
                    .toString(exception.getMessage()));
            return null;
        }
    }

    /**
     * Auto create jks/pkcs12 keystore according file type
     *
     * @param keyFile keystore file path
     * @return keystore instance
     */
    private KeyStore createKeyStoreAccordingFileType(String keyFile) {
        KeyStore typeKeyStore = null;
        String type = FileUtils.getSuffix(keyFile);
        try {
            if (type.equalsIgnoreCase(FILE_TYPE_PKCS12)) {
                typeKeyStore = KeyStore.getInstance(KEYSTORE_TYPE_PKCS12);
            } else if (type.equalsIgnoreCase(FILE_TYPE_JKS)) {
                typeKeyStore = KeyStore.getInstance(KEYSTORE_TYPE_JKS);
            } else {
                typeKeyStore = KeyStore.getInstance(KeyStore.getDefaultType());
            }
        } catch (KeyStoreException exception) {
            LOGGER.debug(exception.getMessage(), exception);
            CustomException.throwException(ERROR.KEYSTORE_OPERATION_ERROR, SignToolErrMsg.KEYSTORE_ERROR
                    .toString(exception.getMessage()));
        }
        return typeKeyStore;
    }
}
