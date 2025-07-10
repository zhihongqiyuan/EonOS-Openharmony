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
#ifndef HELP_H
#define HELP_H

#include <string>
 /* this file use to create help.txt content and
  * it is divided into 12 strings according
  * to the functional module
  */
namespace OHOS {
namespace SignatureTools {

const std::string HELP_TXT_HEADER = R"(
USAGE: <sign|verify-app>[options]
)";

const std::string SIGN_APP_HELP_TXT = R"(
    sign[options]:
        -mode : signature mode, required fields, including localSign/remoteSign/remoteResign;
        -keyAlias : key alias, required fields;
        -keyPwd : key password, optional fields on localSign mode;
        -appCertFile : application signature certificate file, required fields on localSign mode, optional fields
    on remoteSign mode;
        -profileFile : signed Provision Profile file, p7b format, required fields;
        -profileSigned : indicates whether the profile file has a signature.The options are as follows
    : 1 : yes; 0:no; default value:1. optional fields;
        -inFile : input original application package file, .hap, .bin, and .elf format, required fields;
        -signAlg : signature algorithm, required fields, including SHA256withECDSA/SHA384withECDSA;
        -keystoreFile : keystore file, if signature mode is localSign, required fields on localSign mode,
    JKS or P12 format;
        -keystorePwd : keystore password, optional fields on localSign mode;
        -outFile : output the signed Provision Profile file, required fields;
    application package file format is hap;
        -signServer : remote signer plugin, required fields on remoteSign mode;
        -signerPlugin : remote sign service url, required fields on remoteSign mode;
        -onlineAuthMode : remote sign auth mode, required fields on remoteSign mode, including account;
        -username : user account for online auth, required fields on remoteSign mode with account auth mode;
        -userPwd : user password for online auth, required fields on remoteSign mode with account auth mode;
        -ext : extend parameters for remote signer plugin, optional fields;
        -signCode : Whether the HAP file is signed code, The value 1 means enable sign code, and value 0 means
    disable sign code.The default value is 1. It is optional.
        -moduleFile : module.json file.
        -adHoc : Whether the HAP file is ad hoc, The value 1 means enable ad hoc, and value 0 means disable ad hoc.
    The default value is 0. It is optional.

    EXAMPLE :
        sign -keyAlias "oh-app1-key-v1" -appCertFile "/home/app-release-cert.cer" -signCode "1"
-keystoreFile "/home/app-keypair.jks" -keystorePwd ****** -outFile "/home/app1-signed.hap
-profileFile "/home/signed-profile.p7b" -inFile "/home/app1-unsigned.hap" -signAlg SHA256withECDSA
)";

const std::string VERIFY_APP_HELP_TXT = R"(
    verify-app[options]:
        -inFile : signed application package file, hap or bin format, required fields;
        -outCertChain : signed certificate chain file, required fields;
        -outProfile : profile file in application package, required fields;

    EXAMPLE:
        verify-app-inFile "/home/app1-signed.hap" -outCertChain "outCertChain.cer" -outProfile "outprofile.p7b"
)";

const std::string HELP_END_TXT = R"(
COMMANDS :
    sign : application package signature
    verify-app : application package file verification
)";
/* help.txt all content */
const std::string HELP_TXT = HELP_TXT_HEADER + SIGN_APP_HELP_TXT + VERIFY_APP_HELP_TXT + HELP_END_TXT;
}
}
#endif