/*
 * Copyright (c) 2021-2023 Huawei Device Co., Ltd.
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

/**
 * Define const parameters
 *
 * @since 2021-12-13
 */
public class ParamConstants {
    /**
     * error code of hap format error.
     */
    public static final int HAP_FORMAT_ERROR = 20001;

    /**
     * error code of hap parse error.
     */
    public static final int HAP_PARSE_ERROR = 20002;

    /**
     * error code of hap signatures error.
     */
    public static final int HAP_SIGNATURE_ERROR = 20003;

    /**
     * error code of hap signature block not found error.
     */
    public static final int HAP_SIGNATURE_NOT_FOUND_ERROR = 20004;

    /**
     * Algorithm name of sha-256.
     */
    public static final String HAP_SIG_SCHEME_V256_DIGEST_ALGORITHM = "SHA-256";

    /**
     * Algorithm name of sha-384.
     */
    public static final String HAP_SIG_SCHEME_V384_DIGEST_ALGORITHM = "SHA-384";

    /**
     * Algorithm name of sha-512.
     */
    public static final String HAP_SIG_SCHEME_V512_DIGEST_ALGORITHM = "SHA-512";

    /**
     * Signature algorithm name of SHA256withECDSA.
     */
    public static final String HAP_SIG_ALGORITHM_SHA256_ECDSA = "SHA256withECDSA";

    /**
     * Signature algorithm name of SHA384withECDSA.
     */
    public static final String HAP_SIG_ALGORITHM_SHA384_ECDSA = "SHA384withECDSA";

    /**
     * Signature algorithm name of SHA512withECDSA.
     */
    public static final String HAP_SIG_ALGORITHM_SHA512_ECDSA = "SHA512withECDSA";

    /**
     * Signature algorithm name of SHA256withRSA.
     */
    public static final String HAP_SIG_ALGORITHM_SHA256_RSA = "SHA256withRSA";

    /**
     * Signature algorithm name of SHA384withRSA.
     */
    public static final String HAP_SIG_ALGORITHM_SHA384_RSA = "SHA384withRSA";

    /**
     * Signature algorithm name of SHA256withRSA/PSS.
     */
    public static final String HAP_SIG_ALGORITHM_SHA256_RSA_PSS = "SHA256withRSA/PSS";

    /**
     * Signature algorithm name of SHA384withRSA/PSS.
     */
    public static final String HAP_SIG_ALGORITHM_SHA384_RSA_PSS = "SHA384withRSA/PSS";

    /**
     * Signature algorithm name of SHA512withRSA/PSS.
     */
    public static final String HAP_SIG_ALGORITHM_SHA512_RSA_PSS = "SHA512withRSA/PSS";

    /**
     * Signature algorithm name of SHA256withRSAANDMGF1.
     */
    public static final String HAP_SIG_ALGORITHM_SHA256_RSA_MGF1 = "SHA256withRSAANDMGF1";

    /**
     * Signature algorithm name of SHA384withRSAANDMGF1.
     */
    public static final String HAP_SIG_ALGORITHM_SHA384_RSA_MGF1 = "SHA384withRSAANDMGF1";

    /**
     * Signature algorithm name of SHA512withRSAANDMGF1.
     */
    public static final String HAP_SIG_ALGORITHM_SHA512_RSA_MGF1 = "SHA512withRSAANDMGF1";

    /**
     * Default value of zip-file align
     */
    public static final String ALIGNMENT = "4";

    /**
     * Signature mode
     */
    public static final String PARAM_SIGN_MODE = "mode";

    /**
     * Certificate revocation list
     */
    public static final String PARAM_BASIC_CRL = "crl";

    /**
     * Hap-file's property, stored developer info
     */
    public static final String PARAM_BASIC_PROPERTY = "property";

    /**
     * Hap-file's capability profile
     */
    public static final String PARAM_BASIC_PROFILE = "profileFile";

    /**
     * json type content of Hap-file's capability profile
     */
    public static final String PARAM_PROFILE_JSON_CONTENT = "profileContent";

    /**
     * Hap-file's proof-of-rotation
     */
    public static final String PARAM_BASIC_PROOF = "proof";

    /**
     * Alignment
     */
    public static final String PARAM_BASIC_ALIGNMENT = "a";

    /**
     * Private key used in signature
     */
    public static final String PARAM_BASIC_PRIVATE_KEY = "keyAlias";

    /**
     * Unsigned file
     */
    public static final String PARAM_BASIC_INPUT_FILE = "inFile";

    /**
     * Signed file
     */
    public static final String PARAM_BASIC_OUTPUT_FILE = "outFile";

    /**
     * Algorithm name of signature
     */
    public static final String PARAM_BASIC_SIGANTURE_ALG = "signAlg";

    /**
     * Flag indicates whether profile is signed
     */
    public static final String PARAM_BASIC_PROFILE_SIGNED = "profileSigned";

    /**
     * The minimum SDK version required for running the application
     */
    public static final String PARAM_BASIC_COMPATIBLE_VERSION = "compatibleVersion";

    /**
     * Url of signature server
     */
    public static final String PARAM_REMOTE_SERVER = "signServer";

    /**
     * username used in remote sign mode
     */
    public static final String PARAM_REMOTE_USERNAME = "username";

    /**
     * password used in remote sign mode
     */
    public static final String PARAM_REMOTE_CODE = "password";

    /**
     * Local keystore path
     */
    public static final String PARAM_LOCAL_JKS_KEYSTORE = "keystoreFile";

    /**
     * The password of keystore
     */
    public static final String PARAM_LOCAL_JKS_KEYSTORE_CODE = "keystorePwd";

    /**
     * The key alias password
     */
    public static final String PARAM_LOCAL_JKS_KEYALIAS_CODE = "keyPwd";

    /**
     * The certificate file path
     */
    public static final String PARAM_LOCAL_PUBLIC_CERT = "appCertFile";

    /**
     * The path used to output certificate-chain
     */
    public static final String PARAM_VERIFY_CERTCHAIN_FILE = "outCertChain";

    /**
     * The path used to output profile
     */
    public static final String PARAM_VERIFY_PROFILE_FILE = "outProfile";

    /**
     * The path used to output proof-rotation file
     */
    public static final String PARAM_VERIFY_PROOF_FILE = "outproof";

    /**
     * The path used to output property file
     */
    public static final String PARAM_VERIFY_PROPERTY_FILE = "outproperty";

    /**
     * The config params of resign hap
     */
    public static final String PARAM_RESIGN_CONFIG_FILE = "resignconfig";

    /**
     * sign file type bin or zip or elf
     */
    public static final String PARAM_IN_FORM = "inForm";

    /**
     * The code sign params of resign hap
     */
    public static final String PARAM_SIGN_CODE = "signCode";

    /**
     * file name split . of min length
     */
    public static final int FILE_NAME_MIN_LENGTH = 2;

    /**
     * Enumerated value of whether a profile is signed
     */
    public enum ProfileSignFlag {
        DISABLE_SIGN_CODE("0"),
        ENABLE_SIGN_CODE("1");

        private String signFlag;

        ProfileSignFlag(String signFlag) {
            this.signFlag = signFlag;
        }

        public String getSignFlag() {
            return signFlag;
        }
    }

    /**
     * Enumerated value of whether a code sign is signed.
     */
    public enum SignCodeFlag {
        DISABLE_SIGN_CODE("0"),
        ENABLE_SIGN_CODE("1");

        private String signCodeFlag;

        SignCodeFlag(String signCodeFlag) {
            this.signCodeFlag = signCodeFlag;
        }

        public String getSignCodeFlag() {
            return signCodeFlag;
        }
    }
}