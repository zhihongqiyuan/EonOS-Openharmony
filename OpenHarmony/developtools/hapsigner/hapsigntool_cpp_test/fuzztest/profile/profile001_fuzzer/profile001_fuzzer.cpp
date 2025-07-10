/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#include <cstddef>
#include <cstdint>
#include <memory>
#include "securec.h"
#include "../profile_resources.h"

namespace OHOS {
namespace SignatureTools {
bool SignProfileTest001(const uint8_t* data, size_t size)
{
    std::string content(data, data + size);
    Options options;
    std::string mode = SIGN_PROFILE_MODE;
    std::string keyAlias = SIGN_PROFILE_KEY_ALIAS;
    std::string profileCertFile = SIGN_PROFILE_PROFILE_CERT_FILE;
    std::string signAlg = SIGN_PROFILE_SIGN_ALG;
    std::string keystoreFile = SIGN_PROFILE_KEY_STORE_FILE;
    std::string outFile = SIGN_PROFILE_OUT_FILE;
    std::string inFile = SIGN_PROFILE_IN_FILE;
    char keyStorePwd[] = "123456";
    char keypwd[] = "123456";
    options[Options::KEY_ALIAS] = keyAlias;
    options[Options::MODE] = mode;
    options[Options::PROFILE_CERT_FILE] = profileCertFile;
    options[Options::SIGN_ALG] = signAlg;
    options[Options::KEY_STORE_FILE] = keystoreFile;
    options[Options::OUT_FILE] = outFile;
    options[Options::IN_FILE] = inFile;
    options[Options::KEY_RIGHTS] = keypwd;
    options[Options::KEY_STORE_RIGHTS] = keyStorePwd;

    LocalizationAdapter adapter(&options);
    SignerFactory factory;
    std::shared_ptr<Signer> signer = factory.GetSigner(adapter);
    PKCS7Data p7;
    std::string p7b;
    int result = p7.Sign(content, signer, signAlg, p7b);
    return result == 0;
}

bool SignProfileTest002(const uint8_t* data, size_t size)
{
    Options options;
    std::string mode = SIGN_PROFILE_MODE;
    std::string keyAlias = SIGN_PROFILE_KEY_ALIAS;
    std::string profileCertFile = SIGN_PROFILE_PROFILE_CERT_FILE;
    std::string signAlg = SIGN_PROFILE_SIGN_ALG;
    std::string keystoreFile = SIGN_PROFILE_KEY_STORE_FILE;
    std::string outFile = SIGN_PROFILE_OUT_FILE;
    std::string inFile = SIGN_PROFILE_IN_FILE;
    char keyStorePwd[] = "123456";
    char keypwd[] = "123456";
    options[Options::KEY_ALIAS] = keyAlias;
    options[Options::MODE] = mode;
    options[Options::PROFILE_CERT_FILE] = profileCertFile;
    options[Options::SIGN_ALG] = signAlg;
    options[Options::KEY_STORE_FILE] = keystoreFile;
    options[Options::OUT_FILE] = outFile;
    options[Options::IN_FILE] = inFile;
    options[Options::KEY_RIGHTS] = keypwd;
    options[Options::KEY_STORE_RIGHTS] = keyStorePwd;

    LocalizationAdapter adapter(&options);
    SignerFactory factory;
    std::shared_ptr<Signer> signer = factory.GetSigner(adapter);
    STACK_OF(X509)* certs = signer->GetCertificates();
    PKCS7Data::PrintCertChainSub(certs);
    return true;
}

bool SignProfileTest003(const uint8_t* data, size_t size)
{
    ASN1_TIME* time_ = ASN1_TIME_new();
    time_t now;
    if (time(&now) < 0) {
        return false;
    }
    ASN1_TIME_set(time_, now);
    std::string result = PKCS7Data::GetASN1Time(time_);
    ASN1_TIME_free(time_);
    return result.empty() == false;
}

bool SignProfileTest004(const uint8_t* data, size_t size)
{
    ProfileInfo info;
    info.type = ProvisionType::RELEASE;
    ProfileInfo info2(info);
    return true;
}

bool SignProfileTest005(const uint8_t* data, size_t size)
{
    std::string  provision = "{\"app-distribution-type\": \"app_gallery\",\"b"
        "undle-info\":{\"app-feature\":\"hos_system_app\",\"bundle-name\":\"com.Op"
        "enHarmony.app.test\",\"developer-id\":\"OpenHarmony\",\"distribution-certif"
        "icate\":\"-----BEGIN CERTIFICATE-----\\n"
        "MIICMzCCAbegAwIBAgIEaOC/zDAMBggqhkjOPQQDAwUAMGMxCzAJBgNVBAYTAkNO\\n"
        "MRQwEgYDVQQKEwtPcGVuSGFybW9ueTEZMBcGA1UECxMQT3Blbkhhcm1vbnkgVGVh\\n"
        "bTEjMCEGA1UEAxMaT3Blbkhhcm1vbnkgQXBwbGljYXRpb24gQ0EwHhcNMjEwMjAy\\n"
        "MTIxOTMxWhcNNDkxMjMxMTIxOTMxWjBoMQswCQYDVQQGEwJDTjEUMBIGA1UEChML\\n"
        "T3Blbkhhcm1vbnkxGTAXBgNVBAsTEE9wZW5IYXJtb255IFRlYW0xKDAmBgNVBAMT\\n"
        "H09wZW5IYXJtb255IEFwcGxpY2F0aW9uIFJlbGVhc2UwWTATBgcqhkjOPQIBBggq\\n"
        "hkjOPQMBBwNCAATbYOCQQpW5fdkYHN45v0X3AHax12jPBdEDosFRIZ1eXmxOYzSG\\n"
        "JwMfsHhUU90E8lI0TXYZnNmgM1sovubeQqATo1IwUDAfBgNVHSMEGDAWgBTbhrci\\n"
        "FtULoUu33SV7ufEFfaItRzAOBgNVHQ8BAf8EBAMCB4AwHQYDVR0OBBYEFPtxruhl\\n"
        "cRBQsJdwcZqLu9oNUVgaMAwGCCqGSM49BAMDBQADaAAwZQIxAJta0PQ2p4DIu/ps\\n"
        "LMdLCDgQ5UH1l0B4PGhBlMgdi2zf8nk9spazEQI/0XNwpft8QAIwHSuA2WelVi/o\\n"
        "zAlF08DnbJrOOtOnQq5wHOPlDYB4OtUzOYJk9scotrEnJxJzGsh/\\n"
        "-----END CERTIFICATE-----\\n"
        "\"},\"debug-info\":{\"device-id-type\":\"udid\",\"device-ids\":[\"69C75"
        "05BE341BDA5948C3C0CB44ABCD530296054159EFE0BD16A16CD0129CC42\",\"7EED0650"
        "6FCE6325EB2E2FAA019458B856AB10493A6718C7679A73F958732865\"]},\"iss"
        "uer\":\"pki_internal\",\"permissions\":{\"restricted-permissions\":"
        "[\"\"]},\"type\":\"release\",\"uuid\":\"fe686e1b-3770-4824-a938-961b140"
        "a7c98\",\"validity\":{\"not-after\":1705127532,\"not-before\":16105195"
        "32},\"version-code\":1,\"version-name\":\"1.0.0\"}";
    ProfileInfo info;
    AppProvisionVerifyResult result = ParseAndVerify(provision, info);
    return result == AppProvisionVerifyResult::PROVISION_OK;
}

bool DoSomethingInterestingWithMyAPI(const uint8_t* data, size_t size)
{
    SignProfileTest001(data, size);
    SignProfileTest002(data, size);
    SignProfileTest003(data, size);
    SignProfileTest004(data, size);
    SignProfileTest005(data, size);
    return true;
}
}
}
/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    /* Run your code on data */
    OHOS::SignatureTools::DoSomethingInterestingWithMyAPI(data, size);
    return 0;
}