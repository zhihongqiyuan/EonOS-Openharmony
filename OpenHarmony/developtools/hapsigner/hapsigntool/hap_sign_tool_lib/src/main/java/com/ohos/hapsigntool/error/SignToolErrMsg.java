/*
 * Copyright (c) 2025-2025 Huawei Device Co., Ltd.
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

package com.ohos.hapsigntool.error;

/**
 * SignToolErrMsg
 *
 * @since 2025/01/06
 */
public class SignToolErrMsg {
    // unknown error
    /**
     * UNKNOWN_ERROR
     */
    public static final ErrorMsg UNKNOWN_ERROR = ErrorMsg.getSignToolErrBuilder()
            .addTypeCode("10")
            .addErrCode("001")
            .addDescription("Unknown error")
            .addCause("%s")
            .build();

    // command error
    /**
     * UNSUPPORTED_METHOD
     */
    public static final ErrorMsg UNSUPPORTED_METHOD = ErrorMsg.getSignToolErrBuilder()
            .addTypeCode("11")
            .addErrCode("001")
            .addDescription("Unsupported command method")
            .addCause("Can not find method {%s}")
            .addSolution("Please run java -jar hap-sign-tool.jar -h to see more help message.")
            .build();

    /**
     * PARAM_CHECK_FAILED
     */
    public static final ErrorMsg PARAM_CHECK_FAILED = ErrorMsg.getSignToolErrBuilder()
            .addTypeCode("11")
            .addErrCode("002")
            .addDescription("{%s} param is incorrect")
            .addCause("%s")
            .build();

    /**
     * PARAM_NUM_ERROR
     */
    public static final ErrorMsg PARAM_NUM_ERROR = ErrorMsg.getSignToolErrBuilder()
            .addTypeCode("11")
            .addErrCode("003")
            .addDescription("Check param num failed")
            .addCause("Please run java -jar hap-sign-tool.jar -h to see more help message.")
            .build();

    /**
     * PARAM_VALUE_EMPTY
     */
    public static final ErrorMsg PARAM_VALUE_EMPTY = ErrorMsg.getSignToolErrBuilder()
            .addTypeCode("11")
            .addErrCode("004")
            .addDescription("Check param failed")
            .addCause("Param {%s} value could not be empty")
            .build();

    /**
     * PARAM_NOT_TRUSTED
     */
    public static final ErrorMsg PARAM_NOT_TRUSTED = ErrorMsg.getSignToolErrBuilder()
            .addTypeCode("11")
            .addErrCode("005")
            .addDescription("Param is not trusted")
            .addCause("Please run java -jar hap-sign-tool.jar -h to see more help message.")
            .build();

    /**
     * PARAM_NOT_IN_PAIRS
     */
    public static final ErrorMsg PARAM_NOT_IN_PAIRS = ErrorMsg.getSignToolErrBuilder()
            .addTypeCode("11")
            .addErrCode("006")
            .addDescription("Param {-key value} must in pairs")
            .addCause("Please run java -jar hap-sign-tool.jar -h to see more help message.")
            .build();

    /**
     * PARAM_DUPLICATE
     */
    public static final ErrorMsg PARAM_DUPLICATE = ErrorMsg.getSignToolErrBuilder()
            .addTypeCode("11")
            .addErrCode("007")
            .addDescription("Check param num failed")
            .addCause("Param {%s} is duplicated")
            .build();

    /**
     * PARAM_REQUIRED
     */
    public static final ErrorMsg PARAM_REQUIRED = ErrorMsg.getSignToolErrBuilder()
            .addTypeCode("11")
            .addErrCode("008")
            .addDescription("Check param failed")
            .addCause("Param {%s} is required, but can not be found")
            .addSolution("Please input required param")
            .build();

    /**
     * MISSING_PARAM
     */
    public static final ErrorMsg MISSING_PARAM = ErrorMsg.getSignToolErrBuilder()
            .addTypeCode("11")
            .addErrCode("008")
            .addDescription("Check param failed")
            .addCause("Missed param {%s}")
            .build();

    // file error
    /**
     * LOAD_REMOTE_PLUGIN_FAILED
     */
    public static final ErrorMsg LOAD_REMOTE_PLUGIN_FAILED = ErrorMsg.getSignToolErrBuilder()
            .addTypeCode("12")
            .addErrCode("001")
            .addDescription("Load remote sign plugin failed")
            .addCause("%s")
            .build();

    /**
     * FILE_NOT_EXIST
     */
    public static final ErrorMsg FILE_NOT_EXIST = ErrorMsg.getSignToolErrBuilder()
            .addTypeCode("12")
            .addErrCode("002")
            .addDescription("File not exist")
            .addCause("Param {%s} is not exist")
            .build();

    /**
     * FILE_WRITE_FAILED
     */
    public static final ErrorMsg FILE_WRITE_FAILED = ErrorMsg.getSignToolErrBuilder()
            .addTypeCode("12")
            .addErrCode("003")
            .addDescription("Write file failed")
            .addCause("%s")
            .build();

    /**
     * FILE_READ_FAILED
     */
    public static final ErrorMsg FILE_READ_FAILED = ErrorMsg.getSignToolErrBuilder()
            .addTypeCode("12")
            .addErrCode("004")
            .addDescription("Read file failed")
            .addCause("Read file {%s} failed")
            .build();

    /**
     * NOT_SUPPORT_FILE
     */
    public static final ErrorMsg NOT_SUPPORT_FILE = ErrorMsg.getSignToolErrBuilder()
            .addTypeCode("12")
            .addErrCode("005")
            .addDescription("Not support file")
            .addCause("Not support file: %s")
            .build();

    /**
     * FILE_IO_FAILED
     */
    public static final ErrorMsg FILE_IO_FAILED = ErrorMsg.getSignToolErrBuilder()
            .addTypeCode("12")
            .addErrCode("006")
            .addDescription("File IO failed")
            .addCause("%s")
            .build();

    // cert error
    /**
     * CERT_DN_FORMAT_FAILED
     */
    public static final ErrorMsg CERT_DN_FORMAT_FAILED = ErrorMsg.getSignToolErrBuilder()
            .addTypeCode("13")
            .addErrCode("001")
            .addDescription("Check DN format failed")
            .addCause("Format error, must be \"X=xx,XX=xxx,...\"")
            .addSolution("Please check param {%s}")
            .build();

    /**
     * CERT_FORMAT_FAILED
     */
    public static final ErrorMsg CERT_FORMAT_FAILED = ErrorMsg.getSignToolErrBuilder()
            .addTypeCode("13")
            .addErrCode("002")
            .addDescription("Certificate format is in correct, please check your appCertFile parameter.")
            .addCause("%s")
            .addSolution("{-appCertFile} should input a cert chain file，This file is usually suffixed with .cer.")
            .build();

    /**
     * GENERATE_CA_FAILED
     */
    public static final ErrorMsg GENERATE_CA_FAILED = ErrorMsg.getSignToolErrBuilder()
            .addTypeCode("13")
            .addErrCode("003")
            .addDescription("Generate CA failed")
            .addCause("Parameter '%s' and parameter '%s' are inconsistent")
            .build();

    /**
     * CERT_CHAIN_FORMAT_FAILED
     */
    public static final ErrorMsg CERT_CHAIN_FORMAT_FAILED = ErrorMsg.getSignToolErrBuilder()
            .addTypeCode("13")
            .addErrCode("004")
            .addDescription("Profile cert must a cert chain")
            .addCause("cause in cert file: %s")
            .build();

    /**
     * NO_SUCH_SIGNATURE
     */
    public static final ErrorMsg NO_SUCH_SIGNATURE = ErrorMsg.getSignToolErrBuilder()
            .addTypeCode("13")
            .addErrCode("005")
            .addDescription("No such algorithm")
            .addCause("%s")
            .build();

    /**
     * CERT_IO_FAILED
     */
    public static final ErrorMsg CERT_IO_FAILED = ErrorMsg.getSignToolErrBuilder()
            .addTypeCode("13")
            .addErrCode("006")
            .addDescription("Certificate IO failed")
            .addCause("%s")
            .build();

    /**
     * CERTIFICATE_ERROR
     */
    public static final ErrorMsg CERTIFICATE_ERROR = ErrorMsg.getSignToolErrBuilder()
            .addTypeCode("13")
            .addErrCode("007")
            .addDescription("Certificate check failed")
            .addCause("%s")
            .build();

    /**
     * IO_CSR_ERROR
     */
    public static final ErrorMsg IO_CSR_ERROR = ErrorMsg.getSignToolErrBuilder()
            .addTypeCode("13")
            .addErrCode("008")
            .addDescription("generate csr failed")
            .addCause("%s")
            .build();

    // key store error
    /**
     * KEY_ALIAS_NOT_FOUND
     */
    public static final ErrorMsg KEY_ALIAS_NOT_FOUND = ErrorMsg.getSignToolErrBuilder()
            .addTypeCode("14")
            .addErrCode("001")
            .addDescription("key alias not found")
            .addCause("KeyAlias {%s} is not exist in {%s}")
            .addSolution("Please check keystore file and keyAlias, ensure keyAlias is exist")
            .addSolution("Use jdk tool [keytool] check keystore: [keytool -list -keystore xxx.p12]")
            .build();

    /**
     * KEY_ALIAS_EXIST
     */
    public static final ErrorMsg KEY_ALIAS_EXIST = ErrorMsg.getSignToolErrBuilder()
            .addTypeCode("14")
            .addErrCode("002")
            .addDescription("Key alias is exist")
            .addCause("KeyAlias {%s} is exist in {%s}, cloud not overwrite.")
            .addSolution("Please check keystore file and keyAlias, ensure keyAlias is not exist")
            .addSolution("Use jdk tool [keytool] check keystore: [keytool -list -keystore xxx.p12]")
            .build();

    /**
     * INIT_KEYSTORE_FAILED
     */
    public static final ErrorMsg INIT_KEYSTORE_FAILED = ErrorMsg.getSignToolErrBuilder()
            .addTypeCode("14")
            .addErrCode("003")
            .addDescription("Init keystore failed")
            .addCause("%s")
            .addSolution("The key store file does not exist, please check the key store file path.")
            .addSolution("Incorrect keystore password, please input the correct plaintext password.")
            .addSolution("The keystore was created by a newer JDK version, please use the same JDK version")
            .build();

    /**
     * INVALID_KEY
     */
    public static final ErrorMsg INVALID_KEY = ErrorMsg.getSignToolErrBuilder()
            .addTypeCode("14")
            .addErrCode("004")
            .addDescription("Invalid Key")
            .addCause("%s")
            .build();

    /**
     * ALGORITHM_NOT_SUPPORT
     */
    public static final ErrorMsg ALGORITHM_NOT_SUPPORT = ErrorMsg.getSignToolErrBuilder()
            .addTypeCode("14")
            .addErrCode("005")
            .addDescription("Not support algorithm")
            .addCause("%s")
            .build();

    /**
     * KEYSTORE_ERROR
     */
    public static final ErrorMsg KEYSTORE_ERROR = ErrorMsg.getSignToolErrBuilder()
            .addTypeCode("14")
            .addErrCode("006")
            .addDescription("Keystore exception")
            .addCause("%s")
            .build();

    /**
     * KEY_PASSWORD_ERROR
     */
    public static final ErrorMsg KEY_PASSWORD_ERROR = ErrorMsg.getSignToolErrBuilder()
            .addTypeCode("14")
            .addErrCode("007")
            .addDescription("Key alias {%s} password error")
            .addCause("%s")
            .build();

    /**
     * NO_USABLE_CERT
     */
    public static final ErrorMsg NO_USABLE_CERT = ErrorMsg.getSignToolErrBuilder()
            .addTypeCode("14")
            .addErrCode("008")
            .addDescription("No usable cert found in {%s}")
            .addCause("MayBe the certificate in keystore is invalid.")
            .build();

    // signature error
    /**
     * SIGNATURE_FAILED
     */
    public static final ErrorMsg SIGNATURE_FAILED = ErrorMsg.getSignToolErrBuilder()
            .addTypeCode("15")
            .addErrCode("001")
            .addDescription("Signature failed")
            .addCause("%s")
            .build();

    /**
     * SIGNATURE_NOT_MATCHED
     */
    public static final ErrorMsg SIGNATURE_NOT_MATCHED = ErrorMsg.getSignToolErrBuilder()
            .addTypeCode("15")
            .addErrCode("002")
            .addDescription("Signature not matched!")
            .addCause("Signature verify failed")
            .addSolution("Please check if the keyAlias private key matches the public key in the certificate")
            .addSolution("If the certificate is changed, the keyAlias should be replaced synchronously")
            .build();

    /**
     * VERIFY_FAILED
     */
    public static final ErrorMsg VERIFY_FAILED = ErrorMsg.getSignToolErrBuilder()
            .addTypeCode("15")
            .addErrCode("003")
            .addDescription("Verify signature failed")
            .addCause("%s")
            .build();

    /**
     * VERIFY_PROFILE_FAILED
     */
    public static final ErrorMsg VERIFY_PROFILE_FAILED = ErrorMsg.getSignToolErrBuilder()
            .addTypeCode("15")
            .addErrCode("004")
            .addDescription("Verify profile failed")
            .addCause("%s")
            .build();

    /**
     * VERIFY_PROFILE_INVALID
     */
    public static final ErrorMsg VERIFY_PROFILE_INVALID = ErrorMsg.getSignToolErrBuilder()
            .addTypeCode("15")
            .addErrCode("005")
            .addDescription("Verify profile failed")
            .addCause("Verify profile pkcs7 failed! Profile is invalid")
            .addSolution("Check if the profile is legal")
            .addSolution("Maybe Failed during transmission, please download profile again")
            .build();

    /**
     * VERIFY_PROFILE_FAILED
     */
    public static final ErrorMsg PROFILE_CERT_MATCH_FAILED = ErrorMsg.getSignToolErrBuilder()
            .addTypeCode("15")
            .addErrCode("005")
            .addDescription("Profile certificate match failed")
            .addCause("input certificates do not match with profile")
            .addSolution("Profile include signature certificate info, please ensure it same as your appCertFile")
            .addSolution("When applying for profile, need to select a certificate, please choose your appCertFile")
            .build();


    // zip error
    /**
     * READ_ZIP_FAILED
     */
    public static final ErrorMsg READ_ZIP_FAILED = ErrorMsg.getSignToolErrBuilder()
            .addTypeCode("17")
            .addErrCode("001")
            .addDescription("Read zip file failed")
            .addCause("%s")
            .addSolution("App (or hap/hsp/hnp) use zip format.")
            .addSolution("Zip file can support a maximum size of 4G and 65535 sub files.")
            .addSolution("If this value is exceeded, it will be automatically converted to zip64.")
            .addSolution("Please check if file is zip64 format, or zip formatted correctly.")
            .build();

    /**
     * WRITE_ZIP_FAILED
     */
    public static final ErrorMsg WRITE_ZIP_FAILED = ErrorMsg.getSignToolErrBuilder()
            .addTypeCode("17")
            .addErrCode("002")
            .addDescription("Write zip file failed")
            .addCause("%s")
            .build();

    /**
     * ALIGNMENT_ZIP_FAILED
     */
    public static final ErrorMsg ALIGNMENT_ZIP_FAILED = ErrorMsg.getSignToolErrBuilder()
            .addTypeCode("17")
            .addErrCode("003")
            .addDescription("Alignment zip file failed")
            .addCause("%s")
            .build();

    /**
     * ZIP_FORMAT_FAILED
     */
    public static final ErrorMsg ZIP_FORMAT_FAILED = ErrorMsg.getSignToolErrBuilder()
            .addTypeCode("17")
            .addErrCode("004")
            .addDescription("Zip format failed")
            .addCause("%s")
            .build();
}
