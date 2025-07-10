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
USAGE: <generate-keypair|generate-csr|generate-cert|generate-ca|generate-app-cert|generate-profile-cert>[options]
USAGE: <sign-profile|verify-profile|sign-app|verify-app>[options]
)";

const std::string KEYPAIR_HELP_TXT = R"(
    generate-keypair[options]:
        -keyAlias : key alias, required fields;
        -keyPwd : key password, optional fields;
        -keyAlg : key algorithm, required fields, including ECC;
        -keySize : key size, required fields, and the size of the ECC algorithm is NIST-P-256/NIST-P-384;
        -keystoreFile : keystore file, required fields, JKS or P12 format;
        -keystorePwd : keystore password, optional fields;
        -extCfgFile : Extend Profile, optional fields;

    EXAMPLE:
        generate-keypair -keyAlias "oh-app1-key-v1" -keyPwd ****** -keyAlg ECC -keySize NIST-P-256
-keystoreFile "/home/app-keypair.jks" -keystorePwd ******
        generate-keypair -keyAlias "oh-profile-key-v1" -keyPwd ****** -keyAlg ECC -keySize NIST-P-256
-keystoreFile "/home/profile-keypair.jks" -keystorePwd ******
)";

const std::string CSR_HELP_TXT = R"(
    generate-csr[options]:
        -keyAlias : key alias, required fields;
        -keyPwd : key password, optional fields;
        -subject : certificate subject, required fields;
        -signAlg : signature algorithm, required fields, including SHA256withECDSA/SHA384withECDSA;
        -keystoreFile : keystore file, required fields, JKS or P12 format;
        -keystorePwd : keystore password, optional fields;
        -outFile : output file, optional fields, if not filled, it will be directly output to the console;
        -extCfgFile : Extend Profile, optional fields;

    EXAMPLE:
        generate-csr -keyAlias "oh-app1-key-v1" -keyPwd ****** -signAlg SHA256withECDSA -keystorePwd ******
-subject "C=CN, O=OpenHarmony, OU=OpenHarmony Community, CN=App1 Release" -keystoreFile "/home/app-keypair.jks"
-outFile "/home/oh-app1-key-v1.csr"
)";

const std::string CERT_HELP_TXT = R"(
    generate-cert[options]:
        -keyAlias : key alias, required fields;
        -keyPwd : key password, optional fields;
        -issuer : issuer subject, required fields;
        -issuerKeyAlias : issuer key alias, required fields;
        -issuerKeyPwd : issuer key password, optional fields;
        -subject : certificate subject, required fields;
        -validity : certificate validity, optional fields, the default is 1095 days;
        -keyUsage : key usage, required fields, including digitalSignature, nonRepudiation, keyEncipherment,
    dataEncipherment, keyAgreement, certificateSignature, crlSignature, encipherOnly and decipherOnly, if the
    certificate includes multiple key usages, separate them with commas;
        -keyUsageCritical : whether keyUsage is a key item, optional fields, the default is true;
        -extKeyUsage : extended key usage, optional fields, including clientAuthentication, serverAuthentication,
    codeSignature, emailProtection, smartCardLogin, timestamp, ocspSignature;
        -extKeyUsageCritical : whether extKeyUsage is a key item, optional fields, the default is false;
        -signAlg : signature algorithm, required fields, includingSHA256withECDSA/SHA384withECDSA；
        -basicConstraints : whether to include basicConstraints, optional fields, the default is false;
        -basicConstraintsCritical : whether basicConstraints is a key item, optional fields, the default is false;
        -basicConstraintsCa : whether it is CA, optional fields, the default is false;
        -basicConstraintsPathLen : basicConstraints path length, optional fields, the default is 0;
        -keystoreFile : keystore file, required fields, JKS or P12 format;
        -keystorePwd : keystore password, optional fields;
        -outFile : output file, optional fields, if not filled, it will be directly output to the console;
        -extCfgFile : Extend Profile, optional fields;
        -issuerKeystoreFile : issuer keystore file, optional fields, JKS or P12 format;
        -issuerKeystorePwd : issuer keystore password, optional fields;

    EXAMPLE:
        generate-cert -keyAlias "oh-app1-key-v1" -keyPwd ****** -issuerKeyAlias "oh-app-sign-srv-ca-key-v1"
-issuer "C=CN, O=OpenHarmony, OU=OpenHarmony Community, CN=Application Signature Service CA" -issuerKeyPwd ******
-subject "C=CN, O=OpenHarmony, OU=OpenHarmony Community, CN=App1 Release" -outFile "/home/app1.cer" -keystorePwd ******
-keyUsage digitalSignature -extKeyUsage codeSignature -signAlg SHA256withECDSA -keystoreFile "/home/app-keypair.jks"
)";

const std::string CA_CERT_HELP_TXT = R"(
    generate-ca[options]:
        -keyAlias : key alias, required fields;
        -keyPwd : key password, optional fields;
        -keyAlg : key algorithm, required fields, including ECC;
        -keySize : key size, required fields, and the size of the ECC algorithm is NIST-P-256/NIST-P-384;
        -issuer : issuer subject, optional fields, if it is empty, it means root CA;
        -issuerKeyAlias : issuer key alias, optional fields, if it is empty, it means root CA;
        -issuerKeyPwd : issuer key password, optional fields;
        -subject : certificate subject, required fields;
        -validity : certificate validity, optional fields, the default is 3650 days;
        -signAlg : signature algorithm, required fields, including SHA256withECDSA/SHA384withECDSA;
        -basicConstraintsPathLen : basicConstraints path length, optional fields, the default is 0;
        -keystoreFile : keystore file, required fields, JKS or P12 format;
        -keystorePwd : keystore password, optional fields;
        -outFile : output file, optional fields, if not filled, it will be directly output to the console;
        -extCfgFile : Extend Profile, optional fields;
        -issuerKeystoreFile : issuer keystore file, optional fields, JKS or P12 format;
        -issuerKeystorePwd : issuer keystore password, optional fields;

    EXAMPLE:
        generate-ca -keyAlias "oh-root-ca-key-v1" -validity 365 -signAlg SHA384withECDSA -keySize NIST-P-256
 -subject "C=CN, O=OpenHarmony, OU=OpenHarmony Community,CN=Root CA" -keystoreFile  "/home/app-keypair.jks"
 -keystorePwd ****** -outFile "/home/root-ca.cer" -keyAlg ECC
        generate-ca -keyAlias "oh-app1-key-v1" -keyAlg ECC -issuerKeyAlias "oh-sub-app-ca-key-v1" -issuerKeyPwd ******
 -issuer "C=CN, O=OpenHarmony, OU=OpenHarmony Community, CN=Root CA" -keySize NIST-P-256 -validity 365
 -subject "C=CN, O=OpenHarmony, OU=OpenHarmony Community, CN= Application Signature Service CA" -signAlg SHA384withECDSA
 -keystoreFile  "/home/app-keypair.jks" -keystorePwd ****** -outFile "/home/sub-app-sign-srv-ca.cer"
)";

const std::string APP_CERT_HELP_TXT = R"(
    generate-app-cert[options]:
        -keyAlias : key alias, required fields;
        -keyPwd : key password, optional fields;
        -issuer : issuer subject, required fields;
        -issuerKeyAlias : issuer key alias, required fields;
        -issuerKeyPwd : issuer key password, optional fields;
        -subject : certificate subject, required fields;
        -validity : certificate validity, optional fields, the default is 1095 days;
        -signAlg : signature algorithm, required fields, including SHA256withECDSA/SHA384withECDSA;
        -keystoreFile : keystore file, required fields, JKS or P12 format;
        -keystorePwd : keystore password, optional fields;
        -outForm : the format of the output certificate file, including cert/certChain, optional fields,
    the default is cert;
        -rootCaCertFile : root CA certificate file, required when outForm is certChain;
        -subCaCertFile : secondary sub -CA certificate file, required when outForm is certChain;
        -outFile : output certificate file(certificate or certificate chain), optional fields, if not filled,
    it will be directly output to the console;
        -extCfgFile : Extend Profile, optional fields;
        -issuerKeystoreFile : issuer keystore file, optional fields, JKS or P12 format;
        -issuerKeystorePwd : issuer keystore password, optional fields;

    EXAMPLE:
        generate-app-cert -keyAlias "oh-app1-key-v1" -issuerKeyAlias "oh-app-sign-debug-srv-ca-key-v1" -validity 365
-issuer "C=CN, O=OpenHarmony, OU=OpenHarmony Community, CN=Application Debug Signature Service CA" -keyPwd ******
-signAlg SHA256withECDSA -keystoreFile  "/home/app-keypair.jks" -outFile "/home/app-debug-cert.cer" -outForm cert
-subject "C=CN, O=OpenHarmony, OU=OpenHarmony Community, CN=App1 Debug" -keystorePwd ******
        generate-app-cert -keyAlias "oh-app1-key-v1" -issuerKeyAlias "oh-app-sign-release-srv-ca-key-v1" -validity 365
-issuer "C=CN, O=OpenHarmony, OU=OpenHarmony Community, CN=Application Release Signature Service CA" -keyPwd ******
-signAlg SHA256withECDSA -rootCaCertFile "/home/root-ca.cer" -subCaCertFile "/home/sub-app-sign-srv-ca.cer"
-subject "C=CN, O=OpenHarmony, OU=OpenHarmony Community, CN=App1 Release" -keystorePwd ****** -outForm certChain
-keystoreFile "/home/app-keypair.jks" -outFile "/home/app-release-cert.cer"
)";

const std::string PROFILE_CERT_HELP_TXT = R"(
    generate-profile-cert[options]:
        -subject : certificate subject, required fields;
        -validity : certificate validity, optional fields, the default is 1095 days;
        -signAlg : signature algorithm, required fields, including SHA256withECDSA/SHA384withECDSA;
        -keystoreFile : keystore file, required fields, JKS or P12 format;
        -keystorePwd : keystore password, optional fields;
        -outForm : the format of the output certificate file, including cert/certChain, optional fields,
    the default is cert;
        -keyAlias : key alias, required fields;
        -keyPwd : key password, optional fields;
        -issuer : issuer subject, required fields;
        -issuerKeyAlias : issuer key alias, required fields;
        -issuerKeyPwd : issuer key password, optional fields;
        -rootCaCertFile : root CA certificate file, required when outForm is certChain;
        -subCaCertFile : secondary sub -CA certificate file, required when outForm is certChain;
        -outFile : output file, optional fields, if not filled, it will be directly output to the console;
        -extCfgFile : Extend Profile, optional fields;
        -issuerKeystoreFile : issuer keystore file, optional fields, JKS or P12 format;
        -issuerKeystorePwd : issuer keystore password, optional fields;

    EXAMPLE:
        generate-profile-cert -keyAlias "oh-profile-key-v1" -issuerKeyAlias "oh-profile-sign-debug-srv-ca-key-v1"
-keyPwd ****** -issuerKeyPwd ****** -keystoreFile "/home/profile-keypair.jks" -keystorePwd ****** -validity 365
-issuer "C=CN, O=OpenHarmony, OU=OpenHarmony Community, CN=Provision Profile Debug Signature Service CA"
-subject "C=CN, O=OpenHarmony, OU=OpenHarmony Community, CN=Provision Profile Debug" -signAlg SHA256withECDSA
-outFile "/home/provision-profile-debug.cer" -outForm cert
        generate-profile-cert -keyAlias "oh-profile-key-v1" -issuerKeyAlias "oh-profile-sign-release-srv-ca-key-v1"
-keyPwd ****** -issuerKeyPwd ****** -keystoreFile  "/home/profile-keypair.jks" -keystorePwd ****** -validity 365
-issuer "C=CN, O=OpenHarmony, OU=OpenHarmony Community, CN=Provision Profile Release Signature Service CA"
-subject "C=CN, O=OpenHarmony, OU=OpenHarmony Community, CN=Provision Profile Release" -signAlg SHA256withECDSA
-rootCaCertFile "/home/root-ca.cer" -subCaCertFile "/home/sub-profile-sign-srv-ca.cer"  -outForm certChain
-outFile "/home/provision-profile-release.cer"
)";


const std::string SIGN_PROFILE_HELP_TXT = R"(
    sign-profile[options]:
        -mode : signature mode, required fields, including localSign/remoteSign;
        -keyAlias : key alias, required fields;
        -keyPwd : key password, optional fields;
        -profileCertFile : profile signing certificate(certificate chain, the order is three -level -two -root),
    required fields;
        -inFile : input original Provision Profile file, required fields;
        -signAlg : signature algorithm, required fields, including SHA256withECDSA/SHA384withECDSA;
        -keystoreFile : keystore file, if signature mode is localSign, required fields, JKS or P12 format;
        -keystorePwd : keystore password, optional fields;
        -outFile : output the signed Provision Profile file, p7b format, required fields;
        -extCfgFile : Extend Profile, optional fields;
        -signServer : remote signer plugin, required fields on remoteSign mode;
        -signerPlugin : remote sign service url, required fields on remoteSign mode;
        -onlineAuthMode : remote sign auth mode, required fields on remoteSign mode, including account;
        -username : user account for online auth, required fields on remoteSign mode with account auth mode;
        -userPwd : user password for online auth, required fields on remoteSign mode with account auth mode;

    EXAMPLE:
        sign-profile -mode localSign -keyAlias "oh-profile-key-v1" -outFile "/home/signed-profile.p7b"
-keystoreFile "/home/profile-keypair.jks" -keystorePwd ****** -keyPwd ****** -signAlg SHA256withECDSA
-profileCertFile "/home/provision-profile-release.cer" -inFile "/home/app1-profile-release.json"
)";

const std::string VERIFY_PROFILE_HELP_TXT = R"(
    verify-profile[options]:
        -inFile : signed Provision Profile file, p7b format, required fields;
        -outFile : Verification result file(including verification result and profile content),
    json format, optional; if not filled, it will be directly output to the console;
        -extCfgFile : Extend Profile, optional fields;

    EXAMPLE:
        verify-profile -inFile "/home/signed-profile.p7b" -outFile "/home/VerifyResult.json"
)";

const std::string SIGN_APP_HELP_TXT = R"(
    sign-app[options]:
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
        -extCfgFile : Extend Profile, optional fields;
        -inForm : Enter the format of the original file.The supported file formats include.zip, .bin, and .elf.;
        -compatibleVersion : min compatible api version for running app, required fields while input original
    application package file format is hap;
        -signServer : remote signer plugin, required fields on remoteSign mode;
        -signerPlugin : remote sign service url, required fields on remoteSign mode;
        -onlineAuthMode : remote sign auth mode, required fields on remoteSign mode, including account;
        -username : user account for online auth, required fields on remoteSign mode with account auth mode;
        -userPwd : user password for online auth, required fields on remoteSign mode with account auth mode;
        -ext : extend parameters for remote signer plugin, optional fields;
        -signCode : Whether the HAP file is signed code, The value 1 means enable sign code, and value 0 means
    disable sign code.The default value is 1. It is optional.

    EXAMPLE :
        sign-app-mode localSign -keyAlias "oh-app1-key-v1" -appCertFile "/home/app-release-cert.cer" -signCode "1"
-keystoreFile "/home/app-keypair.jks" -keystorePwd ****** -outFile "/home/app1-signed.hap -compatibleVersion 8"
-profileFile "/home/signed-profile.p7b" -inFile "/home/app1-unsigned.hap" -signAlg SHA256withECDSA
)";

const std::string VERIFY_APP_HELP_TXT = R"(
    verify-app[options]:
        -inFile : signed application package file, hap or bin format, required fields;
        -outCertChain : signed certificate chain file, required fields;
        -outProfile : profile file in application package, required fields;
        -extCfgFile : Extend Profile, optional fields;
        -inForm : Enter the format of the original file.The supported file formats include.zip, .bin, and .elf.;

    EXAMPLE:
        verify-app-inFile "/home/app1-signed.hap" -outCertChain "outCertChain.cer" -outProfile "outprofile.p7b"
)";

const std::string HELP_END_TXT = R"(
COMMANDS :
    generate-keypair : generate key pair
    generate-csr : generate certificate signing request
    generate-cert : generate certificate in full, large and complete, any certificate can be generated
    generate-ca : generate root / subject CA certificate, if the key does not exist, generate the key together
    generate-app -cert : generate application debug / release certificate
    generate-profile -cert : generate application debug / release certificate
    sign-profile : Provision Profile file signature
    verify-profile : Provision Profile file verification
    sign-app : application package signature
    verify-app : application package file verification
)";
/* help.txt all content */
const std::string HELP_TXT = HELP_TXT_HEADER + KEYPAIR_HELP_TXT + CSR_HELP_TXT + CERT_HELP_TXT
+ CA_CERT_HELP_TXT + APP_CERT_HELP_TXT + PROFILE_CERT_HELP_TXT
+ SIGN_PROFILE_HELP_TXT + VERIFY_PROFILE_HELP_TXT + SIGN_APP_HELP_TXT
+ VERIFY_APP_HELP_TXT + HELP_END_TXT;
}
}
#endif