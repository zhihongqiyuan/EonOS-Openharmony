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

import com.ohos.hapsigntool.error.CustomException;
import com.ohos.hapsigntool.error.ERROR;
import com.ohos.hapsigntool.error.SignToolErrMsg;
import com.ohos.hapsigntool.utils.LogUtils;

import java.util.HashMap;

/**
 * Options parameter class.
 *
 * @since 2021/12/28
 */
public class Options extends HashMap<String, Object> {
    /**
     * App cert file parameter name.
     */
    public static final String APP_CERT_FILE = "appCertFile";

    /**
     * BasicConstraints parameter name.
     */
    public static final String BASIC_CONSTRAINTS = "basicConstraints";

    /**
     * BasicConstraintsCa parameter name.
     */
    public static final String BASIC_CONSTRAINTS_CA = "basicConstraintsCa";

    /**
     * BasicConstraintsCritical parameter name.
     */
    public static final String BASIC_CONSTRAINTS_CRITICAL = "basicConstraintsCritical";

    /**
     * BasicConstraintsPathLen parameter name.
     */
    public static final String BASIC_CONSTRAINTS_PATH_LEN = "basicConstraintsPathLen";

    /**
     * End file type of cert. values in: cert / certChain
     */
    public static final String OUT_FORM = "outForm";

    /**
     * Ca cert file parameter name.
     */
    public static final String CA_CERT_FILE = "rootCaCertFile";

    /**
     * Sub cert for sign
     */
    public static final String SUB_CA_CERT_FILE = "subCaCertFile";

    /**
     * Ext cfg file parameter name.
     */
    public static final String EXT_CFG_FILE = "extCfgFile";

    /**
     * Ext key usage parameter name.
     */
    public static final String EXT_KEY_USAGE = "extKeyUsage";

    /**
     * Ext key usage critical parameter name.
     */
    public static final String EXT_KEY_USAGE_CRITICAL = "extKeyUsageCritical";

    /**
     * In file parameter name.
     */
    public static final String IN_FILE = "inFile";

    /**
     * In file parameter form.
     */
    public static final String IN_FORM = "inForm";

    /**
     * Issuer parameter name.
     */
    public static final String ISSUER = "issuer";

    /**
     * Issuer key alias parameter name.
     */
    public static final String ISSUER_KEY_ALIAS = "issuerKeyAlias";

    /**
     * Issuer key right parameter name.
     */
    public static final String ISSUER_KEY_RIGHTS = "issuerKeyPwd";

    /**
     * Issuer Key Store File parameter name.
     */
    public static final String ISSUER_KEY_STORE_FILE = "issuerKeystoreFile";

    /**
     * Issuer Key Store Pwd parameter name.
     */
    public static final String ISSUER_KEY_STORE_RIGHTS = "issuerKeystorePwd";

    /**
     * Key alg parameter name.
     */
    public static final String KEY_ALG = "keyAlg";

    /**
     * Key alias parameter name.
     */
    public static final String KEY_ALIAS = "keyAlias";

    /**
     * Key right parameter name.
     */
    public static final String KEY_RIGHTS = "keyPwd";

    /**
     * Key size parameter name.
     */
    public static final String KEY_SIZE = "keySize";

    /**
     * Keystore file parameter name.
     */
    public static final String KEY_STORE_FILE = "keystoreFile";

    /**
     * Keystore right parameter name.
     */
    public static final String KEY_STORE_RIGHTS = "keystorePwd";

    /**
     * Key usage parameter name.
     */
    public static final String KEY_USAGE = "keyUsage";

    /**
     * Key usage critical parameter name.
     */
    public static final String KEY_USAGE_CRITICAL = "keyUsageCritical";

    /**
     * Mode parameter name.
     */
    public static final String MODE = "mode";

    /**
     * Out file parameter name.
     */
    public static final String OUT_FILE = "outFile";

    /**
     * Out file parameter name.
     */
    public static final String OUT_CERT_CHAIN = "outCertChain";

    /**
     * Out file parameter name.
     */
    public static final String OUT_PROFILE = "outProfile";

    /**
     * Profile cert file parameter name.
     */
    public static final String PROFILE_CERT_FILE = "profileCertFile";

    /**
     * Profile file parameter name.
     */
    public static final String PROFILE_FILE = "profileFile";

    /**
     * Profile file is signed or not.
     */
    public static final String PROFILE_SIGNED = "profileSigned";

    /**
     * Sign alg parameter name.
     */
    public static final String SIGN_ALG = "signAlg";

    /**
     * Subject parameter name.
     */
    public static final String SUBJECT = "subject";

    /**
     * Trusted app source file parameter name.
     */
    public static final String TRUSTED_APP_SOURCE_FILE = "trustedAppSourceFile";

    /**
     * Trusted root ca file parameter name.
     */
    public static final String TRUSTED_ROOT_CA_FILE = "trustedRootCaFile";

    /**
     * Validity parameter name.
     */
    public static final String VALIDITY = "validity";

    /**
     * All usages included in the extended key usage.
     */
    public static final String EXT_KEY_USAGE_SCOPE = "clientAuthentication,serverAuthentication,codeSignature,"
            + "emailProtection,smartCardLogin,timestamp,ocspSignature";

    /**
     * Key usage includes all usages.
     */
    public static final String KEY_USAGE_SCOPE = "digitalSignature,nonRepudiation,keyEncipherment,dataEncipherment,"
            + "keyAgreement,certificateSignature,crlSignature,encipherOnly,decipherOnly";

    /**
     * Out form includes all forms.
     */
    public static final String OUT_FORM_SCOPE = "cert,certChain";

    /**
     * Serial version UID.
     */
    private static final long serialVersionUID = 1L;

    /**
     * Empty char.
     */
    private static final char[] NO_CHAR = {};

    /**
     * Logger.
     */
    private static final LogUtils LOGGER = new LogUtils(Options.class);


    /**
     * Check required key, throw exception
     *
     * @param keys keys to check
     */
    public void required(String... keys) {
        for (String key : keys) {
            if (!isEmpty(key) && !this.containsKey(key)) {
                CustomException.throwException(ERROR.COMMAND_ERROR, SignToolErrMsg.PARAM_REQUIRED.toString(key));
            }
        }
    }

    private boolean isEmpty(final CharSequence cs) {
        return cs == null || cs.length() == 0;
    }

    /**
     * Get char value of key.
     *
     * @param key key
     * @return value of key
     */
    public char[] getChars(String key) {
        Object value = this.get(key);
        if (value instanceof char[]) {
            return (char[]) value;
        }
        return NO_CHAR;
    }

    /**
     * Get string value of key.
     *
     * @param key      key
     * @param defValue defValue
     * @return string value of key
     */
    public String getString(String key, String defValue) {
        Object value = this.get(key);
        if (!(value instanceof String)) {
            return defValue;
        }
        return (String) value;
    }

    /**
     * Get string value of key or def value.
     *
     * @param key key
     * @return string value of key
     */
    public String getString(String key) {
        return getString(key, "");
    }

    /**
     * Get boolean value of key.
     *
     * @param key      key
     * @param defValue defValue
     * @return boolean value of key
     */
    public boolean getBoolean(String key, boolean defValue) {
        Object value = this.get(key);
        boolean result = defValue;
        if (value instanceof Boolean) {
            result = (boolean) value;
        }
        if (value instanceof String) {
            if ("true".equalsIgnoreCase((String) value)) {
                result = true;
            }
            if ("false".equalsIgnoreCase((String) value)) {
                result = false;
            }
        }
        return result;
    }

    /**
     * Get boolean value of key or def value.
     *
     * @param key key
     * @return boolean value of key
     */
    public boolean getBoolean(String key) {
        return getBoolean(key, false);
    }

    /**
     * Get int value of key.
     *
     * @param key      key
     * @param defValue defValue
     * @return Value of key
     */
    public int getInt(String key, int defValue) {
        Object value = this.get(key);
        if (value instanceof Integer) {
            return (int) value;
        }
        if (value instanceof String) {
            try {
                return Integer.parseInt((String) value);
            } catch (NumberFormatException exception) {
                LOGGER.debug(exception.getMessage(), exception);
            }
        }
        return defValue;
    }

    /**
     * Get int value of key or def value.
     *
     * @param key key
     * @return Value of key
     */
    public int getInt(String key) {
        return getInt(key, 0);
    }
}

