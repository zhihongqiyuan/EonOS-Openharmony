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

package com.ohos.hapsigntool.codesigning.exception;

import com.ohos.hapsigntool.error.ErrorMsg;

/**
 * CodeSignErrMsg
 *
 * @since 2025/01/06
 */
public class CodeSignErrMsg {
    /**
     * FILE_FORMAT_UNSUPPORTED_ERROR
     */
    public static final ErrorMsg FILE_FORMAT_UNSUPPORTED_ERROR = ErrorMsg.getCodeSignErrBuilder()
        .addTypeCode("06")
        .addErrCode("001")
        .addDescription("Invalid File Format")
        .addCause("Unsupported file to sign")
        .addSolution("Support file format: %s")
        .build();

    /**
     * READ_INPUT_STREAM_ERROR
     */
    public static final ErrorMsg READ_INPUT_STREAM_ERROR = ErrorMsg.getCodeSignErrBuilder()
        .addTypeCode("10")
        .addErrCode("001")
        .addDescription("Input Stream Read Error")
        .addCause("Read buffer from input error")
        .build();

    /**
     * CERTIFICATES_CONFIGURE_ERROR
     */
    public static final ErrorMsg CERTIFICATES_CONFIGURE_ERROR = ErrorMsg.getCodeSignErrBuilder()
        .addTypeCode("11")
        .addErrCode("001")
        .addDescription("Certificates Error")
        .addCause("%s")
        .addSolution("Please check whether the certificate is correct")
        .build();

    /**
     * SIGNATURE_VERIFY_FAILED_ERROR
     */
    public static final ErrorMsg SIGNATURE_VERIFY_FAILED_ERROR = ErrorMsg.getCodeSignErrBuilder()
        .addTypeCode("11")
        .addErrCode("002")
        .addDescription("Certificates Error")
        .addCause("Signing failed")
        .addSolution("Please check whether the keyAlias is correct")
        .build();

    /**
     * PROFILE_TYPE_UNSUPPORTED_ERROR
     */
    public static final ErrorMsg PROFILE_TYPE_UNSUPPORTED_ERROR = ErrorMsg.getCodeSignErrBuilder()
        .addTypeCode("12")
        .addErrCode("001")
        .addDescription("Profile Content Error")
        .addCause("Unsupported profile type")
        .addSolution("Value of 'type' in profile file should be debug or release")
        .build();

    /**
     * PROFILE_TYPE_NOT_EXISTED_ERROR
     */
    public static final ErrorMsg PROFILE_TYPE_NOT_EXISTED_ERROR = ErrorMsg.getCodeSignErrBuilder()
        .addTypeCode("12")
        .addErrCode("001")
        .addDescription("Profile Content Error")
        .addCause("'type' not found in profile file")
        .addSolution("Add 'type' to the profile file")
        .build();

    /**
     * PROFILE_BUNDLE_INFO_NOT_EXISTED_ERROR
     */
    public static final ErrorMsg PROFILE_BUNDLE_INFO_NOT_EXISTED_ERROR = ErrorMsg.getCodeSignErrBuilder()
        .addTypeCode("12")
        .addErrCode("001")
        .addDescription("Profile Content Error")
        .addCause("'bundle-info' not found in profile file")
        .addSolution("Add 'bundle-info' to the profile file")
        .build();

    /**
     * PROFILE_APPID_VALUE_TYPE_ERROR
     */
    public static final ErrorMsg PROFILE_APPID_VALUE_TYPE_ERROR = ErrorMsg.getCodeSignErrBuilder()
        .addTypeCode("12")
        .addErrCode("001")
        .addDescription("Profile Content Error")
        .addCause("Value type of app-identifier is not string")
        .addSolution("Value type of app-identifier should be string")
        .build();

    /**
     * PROFILE_APPID_VALUE_LENGTH_ERROR
     */
    public static final ErrorMsg PROFILE_APPID_VALUE_LENGTH_ERROR = ErrorMsg.getCodeSignErrBuilder()
        .addTypeCode("12")
        .addErrCode("001")
        .addDescription("Profile Content Error")
        .addCause("Value length of app-identifier is invalid")
        .addSolution("Modify to a valid app-identifier in profile file")
        .build();

    /**
     * PROFILE_JSON_PARSE_ERROR
     */
    public static final ErrorMsg PROFILE_JSON_PARSE_ERROR = ErrorMsg.getCodeSignErrBuilder()
        .addTypeCode("12")
        .addErrCode("001")
        .addDescription("Profile Content Error")
        .addCause("Profile json content is invalid")
        .addSolution("Please check whether the profile file is correct")
        .build();

    /**
     * MODULE_JSON_PARSE_ERROR
     */
    public static final ErrorMsg MODULE_JSON_PARSE_ERROR = ErrorMsg.getCodeSignErrBuilder()
        .addTypeCode("12")
        .addErrCode("002")
        .addDescription("module.json Content Error")
        .addCause("module.json content is invalid")
        .addSolution("Please check whether the module.json is correct")
        .build();

    /**
     * HNP_FILE_DESCRIPTION_ERROR
     */
    public static final ErrorMsg HNP_FILE_DESCRIPTION_ERROR = ErrorMsg.getCodeSignErrBuilder()
        .addTypeCode("12")
        .addErrCode("002")
        .addDescription("module.json Content Error")
        .addCause("Hnp {%s} is not described in module.json")
        .addSolution("Hnp file should be described in module.json")
        .build();

    /**
     * ELF_FILE_HEADER_ERROR
     */
    public static final ErrorMsg ELF_FILE_HEADER_ERROR = ErrorMsg.getCodeSignErrBuilder()
        .addTypeCode("12")
        .addErrCode("003")
        .addDescription("Elf File Error")
        .addCause("ELF {%s} is incorrect")
        .addSolution("Failed to read the elf file, please check whether the file header information is correct")
        .build();

    /**
     * EXTRACT_HNP_FILE_ERROR
     */
    public static final ErrorMsg EXTRACT_HNP_FILE_ERROR = ErrorMsg.getCodeSignErrBuilder()
        .addTypeCode("12")
        .addErrCode("004")
        .addDescription("File Format Error")
        .addCause("Extract hnp file {%s} error")
        .addSolution("Check whether the hnp file is packaged correctly")
        .build();

    /**
     * DIGEST_ALGORITHM_ERROR
     */
    public static final ErrorMsg ALGORITHM_NOT_SUPPORT_ERROR = ErrorMsg.getCodeSignErrBuilder()
        .addTypeCode("13")
        .addErrCode("001")
        .addDescription("Digest Algorithm Error")
        .addCause("Invalid algorithm {%s}")
        .addSolution("Support SHA-256 and SHA-512, if an error occurs, please check the JDK version")
        .build();

    /**
     * CODE_SIGN_INTERNAL_ERROR
     */
    public static final ErrorMsg CODE_SIGN_INTERNAL_ERROR = ErrorMsg.getCodeSignErrBuilder()
        .addTypeCode("14")
        .addErrCode("001")
        .addDescription("Code Sign Internal Error")
        .addCause("%s")
        .build();

    /**
     * FILE_4K_ALIGNMENT_ERROR
     */
    public static final ErrorMsg FILE_4K_ALIGNMENT_ERROR = ErrorMsg.getCodeSignErrBuilder()
        .addTypeCode("14")
        .addErrCode("001")
        .addDescription("Code Sign Internal Error")
        .addCause("Invalid data size {%d}, not a multiple of 4096")
        .build();

    /**
     * PAGE_INFO_UNIT_SIZE_ERROR
     */
    public static final ErrorMsg PAGE_INFO_ERROR = ErrorMsg.getCodeSignErrBuilder()
        .addTypeCode("14")
        .addErrCode("001")
        .addDescription("Code Sign Internal Error")
        .addCause("Page Info Error : %s")
        .build();
}
