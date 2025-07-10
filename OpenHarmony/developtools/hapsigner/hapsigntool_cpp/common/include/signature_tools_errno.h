/*
 * Copyright (c) 2024-2024 Huawei Device Co., Ltd.
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
#ifndef SIGNATURETOOLS_INDUSTRIAL_BUS_ERRNO_H
#define SIGNATURETOOLS_INDUSTRIAL_BUS_ERRNO_H

namespace OHOS {
namespace SignatureTools {

const int  RET_OK = 0;
const int  RET_FAILED = -1;

const int COMMAND_ERROR = -101;
/**
 * Enum constant FILE_NOT_FOUND.
 */
const int FILE_NOT_FOUND = -102;
/**
 * Enum constant IO_ERROR.
 */
const int IO_ERROR = -103;
/**
 * Enum constant NOT_SUPPORT_ERROR.
 */
const int NOT_SUPPORT_ERROR = -104;
/**
 * Enum constant SIGN_ERROR.
 */
const int SIGN_ERROR = -105;
/**
 * Enum constant VERIFY_ERROR.
 */
const int VERIFY_ERROR = -106;
/**
 * Enum constant COMMAND_PARAM_ERROR.
 */
const int COMMAND_PARAM_ERROR = -107;
/**
 * Enum constant CERTIFICATE_ERROR
*/
const int CERTIFICATE_ERROR = -108;
/**
 * Enum constant KEY_ALIAS_ERROR.
 */
const int KEY_ALIAS_ERROR = -109;
/**
 * Enum constant INVALID_ERROR.
 */
const int INVALIDPARAM_ERROR = -110;
/**
 * Enum constant ZIP_ERROR.
 */
const int ZIP_ERROR = -111;
/**
 * Enum constant FORMAT_ERROR.
 */
const int FORMAT_ERROR = -112;
/**
 * Enum constant PARSE_ERROR.
 */
const int PARSE_ERROR = -113;
/**
 * Enum constant KEY_PASSWORD_ERROR.
 */
const int KEY_PASSWORD_ERROR = -114;
/**
 * Enum constant KEYSTORE_PASSWORD_ERROR.
 */
const int KEYSTORE_PASSWORD_ERROR = -115;
/**
 * Enum constant KEYSTORE_STRUCTURE_ERROR.
 */
const int KEYSTORE_STRUCTURE_ERROR = -116;
/**
 * Enum constant PROVISION_INVALID_ERROR.
 */
const int PROVISION_INVALID_ERROR = -117;

} // namespace SignatureTools
} // namespace OHOS
#endif // SIGNATURETOOLS_INDUSTRIAL_BUS_ERRNO_H