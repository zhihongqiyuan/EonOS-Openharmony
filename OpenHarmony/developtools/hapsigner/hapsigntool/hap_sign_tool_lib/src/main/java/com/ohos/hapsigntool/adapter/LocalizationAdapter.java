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

package com.ohos.hapsigntool.adapter;

import com.ohos.hapsigntool.entity.Options;
import com.ohos.hapsigntool.error.CustomException;
import com.ohos.hapsigntool.error.ERROR;
import com.ohos.hapsigntool.error.SignToolErrMsg;
import com.ohos.hapsigntool.error.VerifyCertificateChainException;
import com.ohos.hapsigntool.utils.KeyPairTools;
import com.ohos.hapsigntool.utils.KeyStoreHelper;
import com.ohos.hapsigntool.utils.CertUtils;
import com.ohos.hapsigntool.utils.FileUtils;
import com.ohos.hapsigntool.utils.LogUtils;
import com.ohos.hapsigntool.utils.StringUtils;
import com.ohos.hapsigntool.utils.ValidateUtils;
import org.bouncycastle.asn1.x500.X500Name;
import org.bouncycastle.asn1.x509.KeyPurposeId;
import org.bouncycastle.asn1.x509.KeyUsage;

import java.io.File;
import java.io.IOException;
import java.security.KeyPair;
import java.security.cert.CertificateException;
import java.security.cert.X509Certificate;
import java.util.Arrays;
import java.util.List;

/**
 * Localization adapter.
 *
 * @since 2021/12/28
 */
public class LocalizationAdapter {
    /**
     * Check cert chain size
     */
    private static final int MIN_CERT_CHAIN_SIZE = 2;
    private static final int MAX_CERT_CHAIN_SIZE = 3;

    /**
     * Logger
     */
    private static final LogUtils LOGGER = new LogUtils(LocalizationAdapter.class);

    /**
     * Params
     */
    private final Options options;

    /**
     * Operation of keystore
     */
    private KeyStoreHelper keyStoreHelper;

    /**
     * Judge whether IssuerKeyStoreFile exists
     */
    private boolean isIssuerKeyStoreFile = false;

    /**
     * Constructor of LocalizationAdapter.
     *
     * @param options options
     */
    public LocalizationAdapter(Options options) {
        this.options = options;
    }

    /**
     * Set keyStoreHelper
     *
     * @param keyStoreHelper keyStoreHelper
     */
    public void setKeyStoreHelper(KeyStoreHelper keyStoreHelper) {
        this.keyStoreHelper = keyStoreHelper;
    }

    /**
     * Set issuerKeyStoreFile
     *
     * @param issuerKeyStoreFile issuerKeyStoreFile
     */
    public void setIssuerKeyStoreFile(boolean issuerKeyStoreFile) {
        this.isIssuerKeyStoreFile = issuerKeyStoreFile;
    }

    /**
     * Get options.
     *
     * @return options
     */
    public Options getOptions() {
        return options;
    }

    private void initKeyStore() {
        // Avoid duplicated initialization
        if (keyStoreHelper != null) {
            return;
        }

        String keyStore ;
        if (this.isIssuerKeyStoreFile) {
            keyStore = options.getString(Options.ISSUER_KEY_STORE_FILE, "");
            keyStoreHelper = new KeyStoreHelper(keyStore, options.getChars(Options.ISSUER_KEY_STORE_RIGHTS));
        } else {
            keyStore = options.getString(Options.KEY_STORE_FILE, "");
            keyStoreHelper = new KeyStoreHelper(keyStore, options.getChars(Options.KEY_STORE_RIGHTS));
        }
        this.setIssuerKeyStoreFile(false);
    }

    /**
     * Get KeyPair through key alias and password.
     *
     * @param autoCreate autoCreate
     * @return keyPair
     */
    public KeyPair getAliasKey(boolean autoCreate) {
        return getKeyPair(options.getString(Options.KEY_ALIAS),
                options.getChars(Options.KEY_RIGHTS), autoCreate);
    }

    /**
     * Get the alias of issuer key.
     *
     * @return param of issuer alias key .
     */
    public KeyPair getIssuerAliasKey() {
        return getKeyPair(options.getString(Options.ISSUER_KEY_ALIAS),
                options.getChars(Options.ISSUER_KEY_RIGHTS), false);
    }

    /**
     * Check whether the keystore has alias or not.
     *
     * @param alias alias of key
     * @return true or false
     */
    public boolean hasAlias(String alias) {
        if (keyStoreHelper == null) {
            initKeyStore();
        }
        return keyStoreHelper.hasAlias(alias);
    }

    /**
     * Error if not exist.
     *
     * @param alias alias
     */
    public void errorIfNotExist(String alias) {
        if (keyStoreHelper == null) {
            initKeyStore();
        }
        keyStoreHelper.errorIfNotExist(alias);
    }

    /**
     * Error on exist.
     *
     * @param alias alias
     */
    public void errorOnExist(String alias) {
        if (keyStoreHelper == null) {
            initKeyStore();
        }
        keyStoreHelper.errorOnExist(alias);
    }

    private KeyPair getKeyPair(String alias, char[] keyPwd, boolean autoCreate) {
        if (keyStoreHelper == null) {
            initKeyStore();
        }
        ValidateUtils.throwIfNotMatches(!StringUtils.isEmpty(alias), ERROR.ACCESS_ERROR,
                SignToolErrMsg.PARAM_VALUE_EMPTY.toString("KeyAlias"));
        KeyPair keyPair = null;
        if (keyStoreHelper.hasAlias(alias)) {
            keyPair = keyStoreHelper.loadKeyPair(alias, keyPwd);
        } else {
            if (autoCreate) {
                options.required(Options.KEY_ALG, Options.KEY_SIZE);
                keyPair = KeyPairTools.generateKeyPair(options.getString(Options.KEY_ALG),
                        options.getInt(Options.KEY_SIZE));
                keyStoreHelper.store(alias, keyPwd, keyPair, null);
            }
        }
        ValidateUtils.throwIfNotMatches(keyPair != null, ERROR.PARAM_NOT_EXIST_ERROR,
                SignToolErrMsg.KEY_ALIAS_NOT_FOUND.toString(alias, keyStoreHelper.getKeyStorePath()));
        return keyPair;
    }

    /**
     * Get profile cert.
     *
     * @return profile cert.
     */
    public List<X509Certificate> getSignCertChain() {
        String certPath = options.getString(Options.PROFILE_CERT_FILE);
        if (StringUtils.isEmpty(certPath)) {
            certPath = options.getString(Options.APP_CERT_FILE);
        }
        List<X509Certificate> certificates = getCertsFromFile(certPath, Options.PROFILE_CERT_FILE);

        ValidateUtils.throwIfNotMatches(
                certificates.size() >= MIN_CERT_CHAIN_SIZE && certificates.size() <= MAX_CERT_CHAIN_SIZE,
                ERROR.NOT_SUPPORT_ERROR, SignToolErrMsg.CERT_CHAIN_FORMAT_FAILED.toString(certPath));
        return certificates;
    }

    /**
     * Get the cert file of sub ca.
     *
     * @return the cert file of sub ca.
     */
    public X509Certificate getSubCaCertFile() {
        String certPath = options.getString(Options.SUB_CA_CERT_FILE);
        return getCertsFromFile(certPath, Options.SUB_CA_CERT_FILE).get(0);
    }

    /**
     * Get the cert file of root ca.
     *
     * @return the cert file of root ca.
     */
    public X509Certificate getCaCertFile() {
        String certPath = options.getString(Options.CA_CERT_FILE);
        return getCertsFromFile(certPath, Options.CA_CERT_FILE).get(0);
    }

    /**
     * Check whether the form is cert chain.
     *
     * @return result indicating whether the form is cert chain.
     */
    public boolean isOutFormChain() {
        String outForm = options.getString(Options.OUT_FORM, "certChain");
        return "certChain".equals(outForm);
    }

    /**
     * Get certificates from file.
     *
     * @param certPath the path of cert
     * @param logTitle log title
     * @return certificates
     */
    public List<X509Certificate> getCertsFromFile(String certPath, String logTitle) {
        ValidateUtils.throwIfNotMatches(!StringUtils.isEmpty(certPath), ERROR.PARAM_NOT_EXIST_ERROR,
                SignToolErrMsg.FILE_NOT_EXIST.toString(logTitle));

        File certFile = new File(certPath);
        ValidateUtils.throwIfNotMatches(certFile.exists(), ERROR.FILE_NOT_FOUND,
                SignToolErrMsg.FILE_NOT_EXIST.toString(certPath));
        List<X509Certificate> certificates = null;
        try {
            certificates = CertUtils.generateCertificates(FileUtils.readFile(certFile));
        } catch (IOException | CertificateException | VerifyCertificateChainException exception) {
            LOGGER.debug(exception.getMessage(), exception);
            CustomException.throwException(ERROR.ACCESS_ERROR, SignToolErrMsg.CERT_FORMAT_FAILED
                    .toString(exception.getMessage()));
        }
        ValidateUtils.throwIfNotMatches(certificates != null && !certificates.isEmpty(), ERROR.READ_FILE_ERROR,
                SignToolErrMsg.CERT_FORMAT_FAILED.toString("can not found certificates in file " + certPath));
        return certificates;
    }

    /**
     * Get signature algorithm.
     *
     * @return signature algorithm.
     */
    public String getSignAlg() {
        return options.getString(Options.SIGN_ALG);
    }

    /**
     * Check whether the key usage is critical.
     *
     * @return result indicating whether the key usage is critical.
     */
    public boolean isKeyUsageCritical() {
        return options.getBoolean(Options.KEY_USAGE_CRITICAL, true);
    }

    /**
     * Check whether the external key usage is critical.
     *
     * @return result indicating whether the external key usage is critical.
     */
    public boolean isExtKeyUsageCritical() {
        return options.getBoolean(Options.EXT_KEY_USAGE_CRITICAL, true);
    }

    /**
     * Check whether the basic constraints is ca.
     *
     * @return result indicating whether the basic constraints is ca.
     */
    public boolean isBasicConstraintsCa() {
        return options.getBoolean(Options.BASIC_CONSTRAINTS_CA, false);
    }

    /**
     * Check whether the basic constraints is critical.
     *
     * @return result indicating whether the basic constraints is critical.
     */
    public boolean isBasicConstraintsCritical() {
        return options.getBoolean(Options.BASIC_CONSTRAINTS_CRITICAL, false);
    }

    /**
     * Get the path length of basic constraints.
     *
     * @return the path length of basic constraints.
     */
    public int getBasicConstraintsPathLen() {
        return options.getInt(Options.BASIC_CONSTRAINTS_PATH_LEN);
    }

    /**
     * Get the external key usage.
     *
     * @return KeyPurposeId[] of ExtKeyUsage.
     */
    public KeyPurposeId[] getExtKeyUsage() {
        return CertUtils.parseExtKeyUsage(options.getString(Options.EXT_KEY_USAGE));
    }

    /**
     * Get the key usage.
     *
     * @return the key usage.
     */
    public KeyUsage getKeyUsage() {
        return new KeyUsage(CertUtils.parseKeyUsage(options.getString(Options.KEY_USAGE)));
    }

    /**
     * Get the subject of cert.
     *
     * @return the subject of cert.
     */
    public X500Name getSubject() {
        String subject = options.getString(Options.SUBJECT);
        return CertUtils.buildDN(subject);
    }

    /**
     * Get the subject of issuer.
     *
     * @return the subject of issuer.
     */
    public X500Name getIssuer() {
        String issuer = options.getString(Options.ISSUER, options.getString(Options.SUBJECT));
        return CertUtils.buildDN(issuer);
    }

    /**
     * Get the output file.
     *
     * @return the string of output file.
     */
    public String getOutFile() {
        return options.getString(Options.OUT_FILE);
    }

    /**
     * Get the input file.
     *
     * @return the string of input file.
     */
    public String getInFile() {
        String file = options.getString(Options.IN_FILE);
        ValidateUtils.throwIfNotMatches(new File(file).exists(),
                ERROR.FILE_NOT_FOUND, SignToolErrMsg.FILE_NOT_EXIST.toString(file));
        return file;
    }

    /**
     * Check if it is a remote signature.
     *
     * @return result indicating whether the signer is a remote signer.
     */
    public boolean isRemoteSigner() {
        String mode = options.getString(Options.MODE, "localSign");
        return "remoteSign".equalsIgnoreCase(mode);
    }

    /**
     * Reset the password to ensure security.
     */
    public void releasePwd() {
        resetChars(options.getChars(Options.KEY_STORE_RIGHTS));
        resetChars(options.getChars(Options.KEY_RIGHTS));
        resetChars(options.getChars(Options.ISSUER_KEY_RIGHTS));
        resetChars(options.getChars(Options.ISSUER_KEY_STORE_RIGHTS));
    }

    private void resetChars(char[] chars) {
        if (chars == null) {
            return;
        }
        Arrays.fill(chars, (char) 0);
    }
}
