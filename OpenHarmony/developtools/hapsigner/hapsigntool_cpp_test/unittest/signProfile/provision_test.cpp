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
#include <chrono>
#include <thread>
#include <string>
#include <filesystem>

#include "gtest/gtest.h"
#include "options.h"
#include "sign_tool_service_impl.h"
#include "nlohmann/json.hpp"
#include "signer_factory.h"
#include "profile_sign_tool.h"
#include "params_run_tool.h"
#include "pkcs7_data.h"
#include "signer_config.h"
#include "local_signer.h"
#include "bc_pkcs7_generator.h"
#include "bc_signeddata_generator.h"
#include "profile_verify.h"
#include "constant.h"
#include "cms_utils.h"

using  nlohmann::json;

namespace OHOS {
namespace SignatureTools {

// sign profile使用的全局参数
static const std::string SIGN_PROFILE_MODE = "localSign";
static const std::string SIGN_PROFILE_KEY_ALIAS = "oh-profile1-key-v1";
static const std::string SIGN_PROFILE_PROFILE_CERT_FILE = "./signProfile/profile-release1.pem";
static const std::string SIGN_PROFILE_SIGN_ALG = "SHA384withECDSA";
static const std::string SIGN_PROFILE_KEY_STORE_FILE = "./signProfile/ohtest.p12";
static const std::string SIGN_PROFILE_OUT_FILE = "./signProfile/signed-profile.p7b";
static const std::string SIGN_PROFILE_IN_FILE = "./signProfile/profile.json";

static const std::string SIGN_PROFILE_CERT_PEM = "./signProfile/profile-release1-cert.pem";
static const std::string SIGN_PROFILE_REVERSE_PEM = "./signProfile/profile-release1-reverse.pem";
static const std::string SIGN_PROFILE_DOUBLE_CERT_PEM = "./signProfile/profile-release1-invalid_cert_chain.pem";

// verify profile 使用的全局参数
static const std::string VERIFY_PROFILE_IN_FILE = "./signProfile/app1-profile1.p7b";
static const std::string VERIFY_PROFILE_OUT_FILE = "./signProfile/VerifyResult.json";
// sign app 使用全局参数
static const std::string SIGN_APP_MODE = "localSign";
static const std::string SIGN_APP_KEY_ALIAS = "oh-app1-key-v1";
static const std::string SIGN_APP_APP_CERT_FILE = "./signProfile/app-release1.pem";
static const std::string SIGN_APP_PROFILE_FILE = "./signProfile/app1-profile1.p7b";
static const std::string SIGN_APP_IN_FILE = "./signProfile/unsigned.hap";
static const std::string SIGN_APP_IN_FILE_TXT = "./signProfile/unsigned.txt";
static const std::string SIGN_APP_SIGN_ALG = "SHA256withECDSA";
static const std::string SIGN_APP_KEY_STORE_FILE = "./signProfile/ohtest.p12";
static const std::string SIGN_APP_OUT_FILE = "./signProfile/signed.hap";

class ProvisionTest : public testing::Test {
public:
    static void SetUpTestCase(void);
    static void TearDownTestCase(void);
    void SetUp()override;
    void TearDown()override;
};
void ProvisionTest::SetUpTestCase(void)
{
    std::filesystem::copy(SIGN_APP_IN_FILE_TXT, SIGN_APP_IN_FILE);
}

void ProvisionTest::TearDownTestCase(void)
{
    std::filesystem::remove(SIGN_APP_IN_FILE);
}

void ProvisionTest::SetUp()
{
}

void ProvisionTest::TearDown()
{
}

/**
 * @tc.name: provision_test001
 * @tc.desc: Test function of LocalSigner::GetCrls() interface get NULL crls.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(ProvisionTest, provision_test001, testing::ext::TestSize.Level1)
{
    EVP_PKEY* pkey = NULL;
    STACK_OF(X509)* certs = NULL;
    std::shared_ptr<LocalSigner> signer = std::make_shared<LocalSigner>(pkey, certs);
    STACK_OF(X509_CRL)* crls = signer->GetCrls();
    EXPECT_TRUE(crls == NULL);
}

/**
 * @tc.name: provision_test002
 * @tc.desc: Test function of LocalSigner::GetSignature() interface diffrent sign algorithm
 * for success only support SHA256/384withECDSA.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(ProvisionTest, provision_test002, testing::ext::TestSize.Level1)
{
    std::string content = "signed content data";
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
    std::string signature1 = signer->GetSignature(content, "SHA384withECDSA");
    EXPECT_TRUE(signature1.size());
    std::string signature2 = signer->GetSignature(content, "SHA256withECDSA");
    EXPECT_TRUE(signature2.size());
    std::string signature3 = signer->GetSignature(content, "SHA999withECDSA");
    EXPECT_TRUE(signature3.empty());
    std::string signature4 = signer->GetSignature("", "SHA384withECDSA");
    EXPECT_FALSE(signature4.empty());
}

/**
 * @tc.name: provision_test003
 * @tc.desc: Test function of ParseProvision() failed for not debug/release provision.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(ProvisionTest, provision_test003, testing::ext::TestSize.Level1)
{
    std::string provision = "{\"acls\":{\"allowed-acls\":[\"ac1\",\"ac2\"]},\"bundle-info\":{\"app-feature\":"
        "\"hos_system_app\",\"bundle-name\":\"com.example.nativetemplatedemo\",\"developer-id\":\"OpenHarmony\","
        "\"development-certificate\":\"-----BEGIN CERTIFICATE-----\\n"
        "MIICXjCCAeOgAwIBAgIBATAKBggqhkjOPQQDAzBuMQswCQYDVQQGEwJDTjEUMBIG\\n"
        "A1UECgwLT3Blbkhhcm1vbnkxHjAcBgNVBAsMFU9wZW5IYXJtb255IENvbW11bml0\\n"
        "eTEpMCcGA1UEAwwgQXBwbGljYXRpb24gU2lnbmF0dXJlIFNlcnZpY2UgQ0EwHhcN\\n"
        "MjQwNDE1MDUzOTUyWhcNMjUwNDE1MDUzOTUyWjBaMQswCQYDVQQGEwJDTjEUMBIG\\n"
        "A1UECgwLT3Blbkhhcm1vbnkxHjAcBgNVBAsMFU9wZW5IYXJtb255IENvbW11bml0\\n"
        "eTEVMBMGA1UEAwwMQXBwMSBSZWxlYXNlMHYwEAYHKoZIzj0CAQYFK4EEACIDYgAE\\n"
        "odSztdiucyVAo7VQnzHzBJsS9vQYa1vU1cP92F6fiJLazWtvEljNP1XoJldSZaN9\\n"
        "UYGdAVHh2yrHzaJFEqHCSB3uQhlJgSbl9sT0lJ4hro1YvVx921/knMRlunz4eAGX\\n"
        "o2kwZzAMBgNVHRMBAf8EAjAAMAsGA1UdDwQEAwIHgDATBgNVHSUEDDAKBggrBgEF\\n"
        "BQcDAzAbBgNVHQ4EFEpzK6IntvQxLaKGX6xZQSiISBx+MBgGDCsGAQQBj1sCgngB\\n"
        "AwQIMAYCAQEKAQAwCgYIKoZIzj0EAwMDaQAwZgIxAPboDdi9EhOiwAhO3N6vTRcK\\n"
        "QT1K1TQq2vjvpC2231Dq4tLPeSzLz6ROq+Zv6IgBYgIxAJ9sZZUBoR2lgPHBzt01\\n"
        "4uxt5nLfJj2XKa6Leb/JWDoosXjoVXoB47y699PtGetcFw==\\n"
        "-----END CERTIFICATE-----\\n"
        "\"},\"debug-info\":{\"device-id-type\":\"udid\",\"device-ids\":[\"69C7505BE341BDA5948C3C0CB44ABCD53029605"
        "4159EFE0BD16A16CD0129CC42\",\"7EED06506FCE6325EB2E2FAA019458B856AB10493A6718C7679A73F958732865\"]},"
        "\"issuer\":\"pki_internal\",\"permissions\":{\"restricted-permissions\":[\"\"]},\"type\":\"invalid de"
        "bug\",\"uuid\""
        ":\"fe686e1b-3770-4824-a938-961b140a7c98\",\"validity\":{\"not-after\":1705127532,\"not-before\":1610519532}"
        ",\"version-code\":1,\"version-name\":\"1.0.0\",\"baseapp-info\":{\"package-name\":\"package_name\",\"packag"
        "e-cert\":\"package_cert\"}}";
    ProfileInfo info;
    AppProvisionVerifyResult result = ParseProvision(provision, info);
    EXPECT_FALSE(result == PROVISION_OK);
}

/**
 * @tc.name: provision_test004
 * @tc.desc: Test function of ParseProvision() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(ProvisionTest, provision_test004, testing::ext::TestSize.Level1)
{
    std::string provision = "{\"acls\":{\"allowed-acls\":[\"ac1\",\"ac2\"]},\"bundle-info\":{\"app-feature\":"
        "\"hos_system_"
        "app\",\"bundle-name\":\".*\",\"developer-id\":\"OpenHarmony\",\"development-certificate\":"
        "\"-----BEGIN CERTIFICATE-----\\n"
        "MIICXjCCAeOgAwIBAgIBATAKBggqhkjOPQQDAzBuMQswCQYDVQQGEwJDTjEUMBIG\\n"
        "A1UECgwLT3Blbkhhcm1vbnkxHjAcBgNVBAsMFU9wZW5IYXJtb255IENvbW11bml0\\n"
        "eTEpMCcGA1UEAwwgQXBwbGljYXRpb24gU2lnbmF0dXJlIFNlcnZpY2UgQ0EwHhcN\\n"
        "MjQwNDE1MDUzOTUyWhcNMjUwNDE1MDUzOTUyWjBaMQswCQYDVQQGEwJDTjEUMBIG\\n"
        "A1UECgwLT3Blbkhhcm1vbnkxHjAcBgNVBAsMFU9wZW5IYXJtb255IENvbW11bml0\\n"
        "eTEVMBMGA1UEAwwMQXBwMSBSZWxlYXNlMHYwEAYHKoZIzj0CAQYFK4EEACIDYgAE\\n"
        "odSztdiucyVAo7VQnzHzBJsS9vQYa1vU1cP92F6fiJLazWtvEljNP1XoJldSZaN9\\n"
        "UYGdAVHh2yrHzaJFEqHCSB3uQhlJgSbl9sT0lJ4hro1YvVx921/knMRlunz4eAGX\\n"
        "o2kwZzAMBgNVHRMBAf8EAjAAMAsGA1UdDwQEAwIHgDATBgNVHSUEDDAKBggrBgEF\\n"
        "BQcDAzAbBgNVHQ4EFEpzK6IntvQxLaKGX6xZQSiISBx+MBgGDCsGAQQBj1sCgngB\\n"
        "AwQIMAYCAQEKAQAwCgYIKoZIzj0EAwMDaQAwZgIxAPboDdi9EhOiwAhO3N6vTRcK\\n"
        "QT1K1TQq2vjvpC2231Dq4tLPeSzLz6ROq+Zv6IgBYgIxAJ9sZZUBoR2lgPHBzt01\\n"
        "4uxt5nLfJj2XKa6Leb/JWDoosXjoVXoB47y699PtGetcFw==\\n"
        "-----END CERTIFICATE-----\\n"
        "\"},\"debug-info\":{\"device-id-type\":\"udid\",\"device-ids\":[\"69C7505BE341BDA5948C3C0CB44ABCD530296"
        "054159EFE0BD16A16CD0129CC42\",\"7EED06506FCE6325EB2E2FAA019458B856AB10493A6718C7679A73F958732865\"]},\""
        "issuer\":\"pki_internal\",\"permissions\":{\"restricted-permissions\":[\"\"]},\"type\":\"debug\",\"uuid"
        "\":\"fe686e1b-3770-4824-a938-961b140a7c98\",\"validity\":{\"not-after\":1705127532,\"not-before\":161051"
        "9532},\"version-code\":1,\"version-name\":\"1.0.0\",\"baseapp-info\":{\"package-name\":\"package_name\","
        "\"package-cert\":\"package_cert\"}}";
    ProfileInfo info2;
    AppProvisionVerifyResult result = ParseProvision(provision, info2);
    EXPECT_TRUE(result == PROVISION_OK);
}
/**
 * @tc.name: provision_test005
 * @tc.desc: Test function of ParseProvision() failed for not struct json.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(ProvisionTest, provision_test005, testing::ext::TestSize.Level1)
{
    ProfileInfo info;
    std::string provision = "55.2";
    AppProvisionVerifyResult result = ParseProvision(provision, info);
    EXPECT_FALSE(result == PROVISION_OK);
}

/**
 * @tc.name: provision_test006
 * @tc.desc: Test function of ParseProvision() failed for discarded json.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(ProvisionTest, provision_test006, testing::ext::TestSize.Level1)
{
    ProfileInfo info;
    std::string provision = "{\"name\": \"feixing\",\"age\": 18}+";
    AppProvisionVerifyResult result = ParseProvision(provision, info);
    EXPECT_FALSE(result == PROVISION_OK);
}

/**
n * @tc.name: provision_test009
 * @tc.desc: Test function of ParseProfile() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(ProvisionTest, provision_test009, testing::ext::TestSize.Level1)
{
    ProfileInfo info;
    std::string provision = "[88,99,42,11,22]";
    AppProvisionVerifyResult result = ParseProfile(provision, info);
    EXPECT_TRUE(result == PROVISION_OK);
}

/**
 * @tc.name: provision_test010
 * @tc.desc: Test function of ParseProvision() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(ProvisionTest, provision_test010, testing::ext::TestSize.Level1)
{
    ProfileInfo info;
    std::string provision = "{\"app-distribution-type\": \"app_gallery\",\"bundle-info\":{\"app-feature\""
        ":\"hos_system_app\",\"bundle-name\":\"com.example.nativetemplatedemo\",\"developer-id\":\"OpenHarm"
        "ony\",\"development-certificate\":\"-----BEGIN CERTIFICATE-----\\n"
        "MIICXjCCAeOgAwIBAgIBATAKBggqhkjOPQQDAzBuMQswCQYDVQQGEwJDTjEUMBIG\\n"
        "A1UECgwLT3Blbkhhcm1vbnkxHjAcBgNVBAsMFU9wZW5IYXJtb255IENvbW11bml0\\n"
        "eTEpMCcGA1UEAwwgQXBwbGljYXRpb24gU2lnbmF0dXJlIFNlcnZpY2UgQ0EwHhcN\\n"
        "MjQwNDE1MDUzOTUyWhcNMjUwNDE1MDUzOTUyWjBaMQswCQYDVQQGEwJDTjEUMBIG\\n"
        "A1UECgwLT3Blbkhhcm1vbnkxHjAcBgNVBAsMFU9wZW5IYXJtb255IENvbW11bml0\\n"
        "eTEVMBMGA1UEAwwMQXBwMSBSZWxlYXNlMHYwEAYHKoZIzj0CAQYFK4EEACIDYgAE\\n"
        "odSztdiucyVAo7VQnzHzBJsS9vQYa1vU1cP92F6fiJLazWtvEljNP1XoJldSZaN9\\n"
        "UYGdAVHh2yrHzaJFEqHCSB3uQhlJgSbl9sT0lJ4hro1YvVx921/knMRlunz4eAGX\\n"
        "o2kwZzAMBgNVHRMBAf8EAjAAMAsGA1UdDwQEAwIHgDATBgNVHSUEDDAKBggrBgEF\\n"
        "BQcDAzAbBgNVHQ4EFEpzK6IntvQxLaKGX6xZQSiISBx+MBgGDCsGAQQBj1sCgngB\\n"
        "AwQIMAYCAQEKAQAwCgYIKoZIzj0EAwMDaQAwZgIxAPboDdi9EhOiwAhO3N6vTRcK\\n"
        "QT1K1TQq2vjvpC2231Dq4tLPeSzLz6ROq+Zv6IgBYgIxAJ9sZZUBoR2lgPHBzt01\\n"
        "4uxt5nLfJj2XKa6Leb/JWDoosXjoVXoB47y699PtGetcFw==\\n"
        "-----END CERTIFICATE-----\\n"
        "\"},\"debug-info\":{\"device-id-type\":\"udid\",\"device-ids\":[\"69C7505BE341BDA5948C3C0CB44ABCD"
        "530296054159EFE0BD16A16CD0129CC42\",\"7EED06506FCE6325EB2E2FAA019458B856AB10493A6718C7679A73F95873"
        "2865\"]},\"issuer\":\"pki_internal\",\"permissions\":{\"restricted-permissions\":[\"\"]},\"type\":"
        "\"debug\",\"uuid\":\"fe686e1b-3770-4824-a938-961b140a7c98\",\"validity\":{\"not-after\":1705127532"
        ",\"not-before\":1610519532},\"version-code\":1,\"version-name\":\"1.0.0\"}";
    AppProvisionVerifyResult result = ParseProvision(provision, info);
    EXPECT_TRUE(result == PROVISION_OK);
}

/**
 * @tc.name: provision_test014
 * @tc.desc: Test function of ParseProvision() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(ProvisionTest, provision_test014, testing::ext::TestSize.Level1)
{
    ProfileInfo info;
    std::string provision = "{\"app-privilege-capabilities\": [99,\"88\"],\"bundle-info\":{\"app"
        "-feature\":\"hos_system_app\",\"bundle-name\":\"com.example.nativetemplatedemo\",\"developer"
        "-id\":\"OpenHarmony\",\"development-certificate\":\"-----BEGIN CERTIFICATE-----\\n"
        "MIICXjCCAeOgAwIBAgIBATAKBggqhkjOPQQDAzBuMQswCQYDVQQGEwJDTjEUMBIG\\n"
        "A1UECgwLT3Blbkhhcm1vbnkxHjAcBgNVBAsMFU9wZW5IYXJtb255IENvbW11bml0\\n"
        "eTEpMCcGA1UEAwwgQXBwbGljYXRpb24gU2lnbmF0dXJlIFNlcnZpY2UgQ0EwHhcN\\n"
        "MjQwNDE1MDUzOTUyWhcNMjUwNDE1MDUzOTUyWjBaMQswCQYDVQQGEwJDTjEUMBIG\\n"
        "A1UECgwLT3Blbkhhcm1vbnkxHjAcBgNVBAsMFU9wZW5IYXJtb255IENvbW11bml0\\n"
        "eTEVMBMGA1UEAwwMQXBwMSBSZWxlYXNlMHYwEAYHKoZIzj0CAQYFK4EEACIDYgAE\\n"
        "odSztdiucyVAo7VQnzHzBJsS9vQYa1vU1cP92F6fiJLazWtvEljNP1XoJldSZaN9\\n"
        "UYGdAVHh2yrHzaJFEqHCSB3uQhlJgSbl9sT0lJ4hro1YvVx921/knMRlunz4eAGX\\n"
        "o2kwZzAMBgNVHRMBAf8EAjAAMAsGA1UdDwQEAwIHgDATBgNVHSUEDDAKBggrBgEF\\n"
        "BQcDAzAbBgNVHQ4EFEpzK6IntvQxLaKGX6xZQSiISBx+MBgGDCsGAQQBj1sCgngB\\n"
        "AwQIMAYCAQEKAQAwCgYIKoZIzj0EAwMDaQAwZgIxAPboDdi9EhOiwAhO3N6vTRcK\\n"
        "QT1K1TQq2vjvpC2231Dq4tLPeSzLz6ROq+Zv6IgBYgIxAJ9sZZUBoR2lgPHBzt01\\n"
        "4uxt5nLfJj2XKa6Leb/JWDoosXjoVXoB47y699PtGetcFw==\\n"
        "-----END CERTIFICATE-----\\n"
        "\"},\"debug-info\":{\"device-id-type\":\"udid\",\"device-ids\":[\"69C7505BE341BDA5948C3C0CB"
        "44ABCD530296054159EFE0BD16A16CD0129CC42\",\"7EED06506FCE6325EB2E2FAA019458B856AB10493A6718C7"
        "679A73F958732865\"]},\"issuer\":\"pki_internal\",\"permissions\":{\"restricted-permissions\""
        ":[\"\"]},\"type\":\"debug\",\"uuid\":\"fe686e1b-3770-4824-a938-961b140a7c98\",\"validity\":"
        "{\"not-after\":1705127532,\"not-before\":1610519532},\"version-code\":1,\"version-name\":\"1.0.0\"}";
    AppProvisionVerifyResult result = ParseProvision(provision, info);
    EXPECT_TRUE(result == PROVISION_OK);
}
/**
 * @tc.name: provision_test019
 * @tc.desc: Test function of ParseProvision() failed for development-certificate is empty.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(ProvisionTest, provision_test019, testing::ext::TestSize.Level1)
{
    ProfileInfo info;
    std::string provision = "{\"bundle-info\":{\"app-feature\":\"hos_system_app\",\"bundle-name\":\"com"
        ".example.nativetemplatedemo\",\"developer-id\":\"OpenHarmony\",\"development-certificate\":\"\"},\""
        "d""ebug-info\":{\"device-id-type\":\"udid\",\"device-ids\":[\"69C7505BE341BDA5948C3C0CB44ABCD53029"
        "6054159EFE0BD16A16CD0129CC42\",\"7EED06506FCE6325EB2E2FAA019458B856AB10493A6718C7679A73F95873286"
        "5\"]},\"issuer\":\"pki_internal\",\"permissions\":{\"restricted-permissions\":[\"\"]},\"type\":\"de"
        "bug\",\"uuid\":\"fe686e1b-3770-4824-a938-961b140a7c98\",\"validity\":{\"not-after\":1705127532,\"no"
        "t-before\":1610519532},\"version-code\":1,\"version-name\":\"1.0.0\"}";
    AppProvisionVerifyResult result = ParseProvision(provision, info);
    EXPECT_FALSE(result == PROVISION_OK);
}

/**
 * @tc.name: provision_test021
 * @tc.desc: Test function of Provision operator==() interface stability.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(ProvisionTest, provision_test021, testing::ext::TestSize.Level1)
{
    ProfileInfo info;
    ProfileInfo info2;
    info2.profileBlockLength = 0;
    info = info2;
    EXPECT_EQ(info.profileBlockLength, info2.profileBlockLength);
}

/**
 * @tc.name: provision_test023
 * @tc.desc: Test function of Provision operator==() interface stability.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(ProvisionTest, provision_test023, testing::ext::TestSize.Level1)
{
    ProfileInfo info;
    ProfileInfo info2;
    info2.profileBlockLength = 5;
    info2.profileBlock = NULL;
    info = info2;
    EXPECT_EQ(info.profileBlockLength, info2.profileBlockLength);
}

/**
 * @tc.name: provision_test025
 * @tc.desc: Test function of ProfileSignTool::GenerateP7b() failed for invalid keyStorePassword.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(ProvisionTest, provision_test025, testing::ext::TestSize.Level1)
{
    Options options;
    char keyStorePwd[] = "123456";
    char invalidKeyPwd[] = "12345";
    options[Options::KEY_ALIAS] = SIGN_PROFILE_KEY_ALIAS;
    options[Options::MODE] = SIGN_PROFILE_MODE;
    options[Options::PROFILE_CERT_FILE] = SIGN_PROFILE_PROFILE_CERT_FILE;
    options[Options::SIGN_ALG] = SIGN_PROFILE_SIGN_ALG;
    options[Options::KEY_STORE_FILE] = SIGN_PROFILE_KEY_STORE_FILE;
    options[Options::OUT_FILE] = SIGN_PROFILE_OUT_FILE;
    options[Options::IN_FILE] = SIGN_PROFILE_IN_FILE;
    options[Options::KEY_RIGHTS] = invalidKeyPwd;
    options[Options::KEY_STORE_RIGHTS] = keyStorePwd;

    LocalizationAdapter adapter(&options);
    std::string content = "json content";
    std::string ret;
    int result = ProfileSignTool::GenerateP7b(adapter, content, ret);
    EXPECT_FALSE(result == 0);
}

/**
 * @tc.name: provision_test026
 * @tc.desc: Test function of ProfileSignTool::GenerateP7b() failed for invalid sign algorithm.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(ProvisionTest, provision_test026, testing::ext::TestSize.Level1)
{
    Options options;
    char keyStorePwd[] = "123456";
    char keypwd[] = "123456";
    std::string invalid_sigAlg = "SHA385withECDSA";
    options[Options::KEY_ALIAS] = SIGN_PROFILE_KEY_ALIAS;
    options[Options::MODE] = SIGN_PROFILE_MODE;
    options[Options::PROFILE_CERT_FILE] = SIGN_PROFILE_PROFILE_CERT_FILE;
    options[Options::SIGN_ALG] = invalid_sigAlg;
    options[Options::KEY_STORE_FILE] = SIGN_PROFILE_KEY_STORE_FILE;
    options[Options::OUT_FILE] = SIGN_PROFILE_OUT_FILE;
    options[Options::IN_FILE] = SIGN_PROFILE_IN_FILE;
    options[Options::KEY_RIGHTS] = keypwd;
    options[Options::KEY_STORE_RIGHTS] = keyStorePwd;

    LocalizationAdapter adapter(&options);
    std::string content = "json content";
    std::string ret;
    int result = ProfileSignTool::GenerateP7b(adapter, content, ret);
    EXPECT_FALSE(result == 0);
}

/**
 * @tc.name: provision_test027
 * @tc.desc: Test function of ProfileSignTool::GenerateP7b() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(ProvisionTest, provision_test027, testing::ext::TestSize.Level1)
{
    Options options;
    char keyStorePwd[] = "123456";
    char keypwd[] = "123456";
    options[Options::KEY_ALIAS] = SIGN_PROFILE_KEY_ALIAS;
    options[Options::MODE] = SIGN_PROFILE_MODE;
    options[Options::PROFILE_CERT_FILE] = SIGN_PROFILE_PROFILE_CERT_FILE;
    options[Options::SIGN_ALG] = SIGN_PROFILE_SIGN_ALG;
    options[Options::KEY_STORE_FILE] = SIGN_PROFILE_KEY_STORE_FILE;
    options[Options::OUT_FILE] = SIGN_PROFILE_OUT_FILE;
    options[Options::IN_FILE] = SIGN_PROFILE_IN_FILE;
    options[Options::KEY_RIGHTS] = keypwd;
    options[Options::KEY_STORE_RIGHTS] = keyStorePwd;

    LocalizationAdapter adapter(&options);
    std::string content = "json content";
    std::string ret;
    int result = ProfileSignTool::GenerateP7b(adapter, content, ret);
    EXPECT_TRUE(result == 0);
}

/**
 * @tc.name: provision_test028
 * @tc.desc: Test function of ProfileSignTool::SignProfile() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(ProvisionTest, provision_test028, testing::ext::TestSize.Level1)
{
    Options options;
    char keyStorePwd[] = "123456";
    char keypwd[] = "123456";
    options[Options::KEY_ALIAS] = SIGN_PROFILE_KEY_ALIAS;
    options[Options::MODE] = SIGN_PROFILE_MODE;
    options[Options::PROFILE_CERT_FILE] = SIGN_PROFILE_PROFILE_CERT_FILE;
    options[Options::SIGN_ALG] = SIGN_PROFILE_SIGN_ALG;
    options[Options::KEY_STORE_FILE] = SIGN_PROFILE_KEY_STORE_FILE;
    options[Options::OUT_FILE] = SIGN_PROFILE_OUT_FILE;
    options[Options::IN_FILE] = SIGN_PROFILE_IN_FILE;
    options[Options::KEY_RIGHTS] = keypwd;
    options[Options::KEY_STORE_RIGHTS] = keyStorePwd;

    LocalizationAdapter adapter(&options);
    SignerFactory factory;
    std::shared_ptr<Signer> signer = factory.GetSigner(adapter);
    std::string content = "json content";
    std::string ret;
    int result = ProfileSignTool::SignProfile(content, signer, SIGN_PROFILE_SIGN_ALG, ret);
    EXPECT_TRUE(result == 0);
}

/**
 * @tc.name: provision_test029
 * @tc.desc: Test function of ProfileSignTool::GenerateP7b() failed for invalid certchain.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(ProvisionTest, provision_test029, testing::ext::TestSize.Level1)
{
    Options options;
    char keyStorePwd[] = "123456";
    char keypwd[] = "123456";
    options[Options::KEY_ALIAS] = SIGN_PROFILE_KEY_ALIAS;
    options[Options::MODE] = SIGN_PROFILE_MODE;
    options[Options::PROFILE_CERT_FILE] = SIGN_PROFILE_CERT_PEM;
    options[Options::SIGN_ALG] = SIGN_PROFILE_SIGN_ALG;
    options[Options::KEY_STORE_FILE] = SIGN_PROFILE_KEY_STORE_FILE;
    options[Options::OUT_FILE] = SIGN_PROFILE_OUT_FILE;
    options[Options::IN_FILE] = SIGN_PROFILE_IN_FILE;
    options[Options::KEY_RIGHTS] = keypwd;
    options[Options::KEY_STORE_RIGHTS] = keyStorePwd;

    LocalizationAdapter adapter(&options);
    std::string content = "json content";
    std::string ret;
    int result = ProfileSignTool::GenerateP7b(adapter, content, ret);
    EXPECT_FALSE(result == 0);
}

/**
 * @tc.name: provision_test030
 * @tc.desc: Test function of ProfileSignTool::GenerateP7b() failed for invalid certichain.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(ProvisionTest, provision_test030, testing::ext::TestSize.Level1)
{
    Options options;
    char keyStorePwd[] = "123456";
    char keypwd[] = "123456";
    options[Options::KEY_ALIAS] = SIGN_PROFILE_KEY_ALIAS;
    options[Options::MODE] = SIGN_PROFILE_MODE;
    options[Options::PROFILE_CERT_FILE] = SIGN_PROFILE_DOUBLE_CERT_PEM;
    options[Options::SIGN_ALG] = SIGN_PROFILE_SIGN_ALG;
    options[Options::KEY_STORE_FILE] = SIGN_PROFILE_KEY_STORE_FILE;
    options[Options::OUT_FILE] = SIGN_PROFILE_OUT_FILE;
    options[Options::IN_FILE] = SIGN_PROFILE_IN_FILE;
    options[Options::KEY_RIGHTS] = keypwd;
    options[Options::KEY_STORE_RIGHTS] = keyStorePwd;

    LocalizationAdapter adapter(&options);
    std::string content = "json content";
    std::string ret;
    int result = ProfileSignTool::GenerateP7b(adapter, content, ret);
    EXPECT_FALSE(result == 0);
}

/**
 * @tc.name: provision_test031
 * @tc.desc: Test function of LocalSigner::GetCrls() interface get NULL crls.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(ProvisionTest, provision_test031, testing::ext::TestSize.Level1)
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
    STACK_OF(X509_CRL)* crls = signer->GetCrls();
    EXPECT_TRUE(crls == NULL);
}

/**
 * @tc.name: provision_test032
 * @tc.desc: Test function of LocalSigner() constructor.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(ProvisionTest, provision_test032, testing::ext::TestSize.Level1)
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

    EVP_PKEY* pkey = NULL;
    STACK_OF(X509)* certs = NULL;
    std::shared_ptr<Signer> signer = NULL;
    signer = std::make_shared<LocalSigner>(pkey, certs);
    EXPECT_NE(signer, nullptr);
}

/**
 * @tc.name: provision_test033
 * @tc.desc: Test function of LocalSigner() interface constructor.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(ProvisionTest, provision_test033, testing::ext::TestSize.Level1)
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

    EVP_PKEY* pkey = NULL;
    STACK_OF(X509)* certs = NULL;
    LocalizationAdapter adapter(&options);
    pkey = adapter.GetAliasKey(false);
    std::shared_ptr<Signer> signer = std::make_shared<LocalSigner>(pkey, certs);
    EXPECT_NE(signer, nullptr);
}

/**
 * @tc.name: provision_test034
 * @tc.desc: Test function of LocalSigner() interface constructor.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(ProvisionTest, provision_test034, testing::ext::TestSize.Level1)
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

    EVP_PKEY* pkey = NULL;
    STACK_OF(X509)* certs = sk_X509_new(NULL);
    LocalizationAdapter adapter(&options);
    pkey = adapter.GetAliasKey(false);
    std::shared_ptr<Signer> signer = std::make_shared<LocalSigner>(pkey, certs);
    EXPECT_NE(signer, nullptr);
}

/**
 * @tc.name: provision_test035
 * @tc.desc: Test function of GetSignature::GetSignature() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(ProvisionTest, provision_test035, testing::ext::TestSize.Level1)
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

    EVP_PKEY* pkey = NULL;
    LocalizationAdapter adapter(&options);
    SignerFactory factory;
    std::shared_ptr<Signer> signer = factory.GetSigner(adapter);
    STACK_OF(X509)* certs = signer->GetCertificates();
    STACK_OF(X509)* certsDup = sk_X509_new(NULL);
    X509* cert = sk_X509_value(certs, 0);
    X509_up_ref(cert);
    sk_X509_push(certsDup, cert);
    std::shared_ptr<Signer> signer2 = std::make_shared<LocalSigner>(pkey, certsDup);
    std::string signature = signer2->GetSignature("content", "SHA384withECDSA");
    EXPECT_NE(signer, nullptr);
}

/**
 * @tc.name: provision_test036
 * @tc.desc: Test function of ParseProvision() failed for distribution-certificate is empty.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(ProvisionTest, provision_test036, testing::ext::TestSize.Level1)
{
    ProfileInfo info;
    std::string provision = "{\"app-distribution-type\": \"app_gallery\",\"app-distribution-type"
        "\": \"app_gallery\",\"bundle-info\":{\"app-feature\":\"hos_system_app\",\"bundle-name\":\"co"
        "m.example.nativetemplatedemo\",\"developer-id\":\"OpenHarmony\",\"development-certifica"
        "te\":\"-----BEGIN CERTIFICATE-----\\n"
        "MIICXjCCAeOgAwIBAgIBATAKBggqhkjOPQQDAzBuMQswCQYDVQQGEwJDTjEUMBIG\\n"
        "A1UECgwLT3Blbkhhcm1vbnkxHjAcBgNVBAsMFU9wZW5IYXJtb255IENvbW11bml0\\n"
        "eTEpMCcGA1UEAwwgQXBwbGljYXRpb24gU2lnbmF0dXJlIFNlcnZpY2UgQ0EwHhcN\\n"
        "MjQwNDE1MDUzOTUyWhcNMjUwNDE1MDUzOTUyWjBaMQswCQYDVQQGEwJDTjEUMBIG\\n"
        "A1UECgwLT3Blbkhhcm1vbnkxHjAcBgNVBAsMFU9wZW5IYXJtb255IENvbW11bml0\\n"
        "eTEVMBMGA1UEAwwMQXBwMSBSZWxlYXNlMHYwEAYHKoZIzj0CAQYFK4EEACIDYgAE\\n"
        "odSztdiucyVAo7VQnzHzBJsS9vQYa1vU1cP92F6fiJLazWtvEljNP1XoJldSZaN9\\n"
        "UYGdAVHh2yrHzaJFEqHCSB3uQhlJgSbl9sT0lJ4hro1YvVx921/knMRlunz4eAGX\\n"
        "o2kwZzAMBgNVHRMBAf8EAjAAMAsGA1UdDwQEAwIHgDATBgNVHSUEDDAKBggrBgEF\\n"
        "BQcDAzAbBgNVHQ4EFEpzK6IntvQxLaKGX6xZQSiISBx+MBgGDCsGAQQBj1sCgngB\\n"
        "AwQIMAYCAQEKAQAwCgYIKoZIzj0EAwMDaQAwZgIxAPboDdi9EhOiwAhO3N6vTRcK\\n"
        "QT1K1TQq2vjvpC2231Dq4tLPeSzLz6ROq+Zv6IgBYgIxAJ9sZZUBoR2lgPHBzt01\\n"
        "4uxt5nLfJj2XKa6Leb/JWDoosXjoVXoB47y699PtGetcFw==\\n"
        "-----END CERTIFICATE-----\\n"
        "\"},\"debug-info\":{\"device-id-type\":\"udid\",\"device-ids\":[\"69C7505BE341BDA5948C3C0CB44ABCD530"
        "296054159EFE0BD16A16CD0129CC42\",\"7EED06506FCE6325EB2E2FAA019458B856AB10493A6718C7679A73F958732865"
        "\"]},\"issuer\":\"pki_internal\",\"permissions\":{\"restricted-permissions\":[\"\"]},\"type\":\"re"
        "lease\",\"uuid\":\"fe686e1b-3770-4824-a938-961b140a7c98\",\"validity\":{\"not-after\":1705127532,\"n"
        "ot-before\":1610519532},\"version-code\":1,\"version-name\":\"1.0.0\"}";
    AppProvisionVerifyResult result = ParseProvision(provision, info);
    EXPECT_FALSE(result == PROVISION_OK);
}

/**
 * @tc.name: provision_test037
 * @tc.desc: Test function of ParseProvision() failed for distribution-certificate is empty.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(ProvisionTest, provision_test037, testing::ext::TestSize.Level1)
{
    ProfileInfo info;
    std::string provision = "{\"app-distribution-type\": \"app_gallery\",\"bundle-info\":{\"app-featur"
        "e\":\"hos_system_app\",\"bundle-name\":\"com.example.nativetemplatedemo\",\"developer-id\":\"Ope"
        "nHarmony\",\"development-certificate\":\"-----BEGIN CERTIFICATE-----\\n"
        "MIICXjCCAeOgAwIBAgIBATAKBggqhkjOPQQDAzBuMQswCQYDVQQGEwJDTjEUMBIG\\n"
        "A1UECgwLT3Blbkhhcm1vbnkxHjAcBgNVBAsMFU9wZW5IYXJtb255IENvbW11bml0\\n"
        "eTEpMCcGA1UEAwwgQXBwbGljYXRpb24gU2lnbmF0dXJlIFNlcnZpY2UgQ0EwHhcN\\n"
        "MjQwNDE1MDUzOTUyWhcNMjUwNDE1MDUzOTUyWjBaMQswCQYDVQQGEwJDTjEUMBIG\\n"
        "A1UECgwLT3Blbkhhcm1vbnkxHjAcBgNVBAsMFU9wZW5IYXJtb255IENvbW11bml0\\n"
        "eTEVMBMGA1UEAwwMQXBwMSBSZWxlYXNlMHYwEAYHKoZIzj0CAQYFK4EEACIDYgAE\\n"
        "odSztdiucyVAo7VQnzHzBJsS9vQYa1vU1cP92F6fiJLazWtvEljNP1XoJldSZaN9\\n"
        "UYGdAVHh2yrHzaJFEqHCSB3uQhlJgSbl9sT0lJ4hro1YvVx921/knMRlunz4eAGX\\n"
        "o2kwZzAMBgNVHRMBAf8EAjAAMAsGA1UdDwQEAwIHgDATBgNVHSUEDDAKBggrBgEF\\n"
        "BQcDAzAbBgNVHQ4EFEpzK6IntvQxLaKGX6xZQSiISBx+MBgGDCsGAQQBj1sCgngB\\n"
        "AwQIMAYCAQEKAQAwCgYIKoZIzj0EAwMDaQAwZgIxAPboDdi9EhOiwAhO3N6vTRcK\\n"
        "QT1K1TQq2vjvpC2231Dq4tLPeSzLz6ROq+Zv6IgBYgIxAJ9sZZUBoR2lgPHBzt01\\n"
        "4uxt5nLfJj2XKa6Leb/JWDoosXjoVXoB47y699PtGetcFw==\\n"
        "-----END CERTIFICATE-----\\n"
        "\"},\"debug-info\":{\"device-id-type\":\"udid\",\"device-ids\":[\"69C7505BE341BDA5948C3C0CB44A"
        "BCD530296054159EFE0BD16A16CD0129CC42\",\"7EED06506FCE6325EB2E2FAA019458B856AB10493A6718C7679A73"
        "F958732865\"]},\"issuer\":\"pki_internal\",\"permissions\":{\"restricted-permissions\":[\"\"]"
        "},\"type\":\"release\",\"uuid\":\"fe686e1b-3770-4824-a938-961b140a7c98\",\"validity\":{\"not-a"
        "fter\":1705127532,\"not-before\":1610519532},\"version-code\":0,\"version-name\":\"1.0.0\"}";
    AppProvisionVerifyResult result = ParseProvision(provision, info);
    EXPECT_FALSE(result == PROVISION_OK);
}

/**
 * @tc.name: provision_test038
 * @tc.desc: Test function of ParseProvision() failed for distribution-certificate is empty.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(ProvisionTest, provision_test038, testing::ext::TestSize.Level1)
{
    ProfileInfo info;
    std::string provision = "{\"app-distribution-type\": \"app_gallery\",\"bundle-info\":{\"app-fea"
        "ture\":\"hos_system_app\",\"bundle-name\":\"com.example.nativetemplatedemo\",\"developer-id\":\""
        "OpenHarmony\",\"development-certificate\":\"-----BEGIN CERTIFICATE-----\\n"
        "MIICXjCCAeOgAwIBAgIBATAKBggqhkjOPQQDAzBuMQswCQYDVQQGEwJDTjEUMBIG\\n"
        "A1UECgwLT3Blbkhhcm1vbnkxHjAcBgNVBAsMFU9wZW5IYXJtb255IENvbW11bml0\\n"
        "eTEpMCcGA1UEAwwgQXBwbGljYXRpb24gU2lnbmF0dXJlIFNlcnZpY2UgQ0EwHhcN\\n"
        "MjQwNDE1MDUzOTUyWhcNMjUwNDE1MDUzOTUyWjBaMQswCQYDVQQGEwJDTjEUMBIG\\n"
        "A1UECgwLT3Blbkhhcm1vbnkxHjAcBgNVBAsMFU9wZW5IYXJtb255IENvbW11bml0\\n"
        "eTEVMBMGA1UEAwwMQXBwMSBSZWxlYXNlMHYwEAYHKoZIzj0CAQYFK4EEACIDYgAE\\n"
        "odSztdiucyVAo7VQnzHzBJsS9vQYa1vU1cP92F6fiJLazWtvEljNP1XoJldSZaN9\\n"
        "UYGdAVHh2yrHzaJFEqHCSB3uQhlJgSbl9sT0lJ4hro1YvVx921/knMRlunz4eAGX\\n"
        "o2kwZzAMBgNVHRMBAf8EAjAAMAsGA1UdDwQEAwIHgDATBgNVHSUEDDAKBggrBgEF\\n"
        "BQcDAzAbBgNVHQ4EFEpzK6IntvQxLaKGX6xZQSiISBx+MBgGDCsGAQQBj1sCgngB\\n"
        "AwQIMAYCAQEKAQAwCgYIKoZIzj0EAwMDaQAwZgIxAPboDdi9EhOiwAhO3N6vTRcK\\n"
        "QT1K1TQq2vjvpC2231Dq4tLPeSzLz6ROq+Zv6IgBYgIxAJ9sZZUBoR2lgPHBzt01\\n"
        "4uxt5nLfJj2XKa6Leb/JWDoosXjoVXoB47y699PtGetcFw==\\n"
        "-----END CERTIFICATE-----\\n"
        "\"},\"debug-info\":{\"device-id-type\":\"udid\",\"device-ids\":[\"69C7505BE341BDA5948C3C0C"
        "B44ABCD530296054159EFE0BD16A16CD0129CC42\",\"7EED06506FCE6325EB2E2FAA019458B856AB10493A6718"
        "C7679A73F958732865\"]},\"issuer\":\"pki_internal\",\"permissions\":{\"restricted-permission"
        "s\":[\"\"]},\"type\":\"release\",\"uuid\":\"fe686e1b-3770-4824-a938-961b140a7c98\",\"valid"
        "ity\":{\"not-after\":1705127532,\"not-before\":1610519532},\"version-code_no\":0,\"ver"
        "sion-name\":\"1.0.0\"}";
    AppProvisionVerifyResult result = ParseProvision(provision, info);
    EXPECT_FALSE(result == PROVISION_OK);
}

/**
 * @tc.name: provision_test039
 * @tc.desc: Test function of ParseProvision() failed for distribution-certificate is empty.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(ProvisionTest, provision_test039, testing::ext::TestSize.Level1)
{
    ProfileInfo info;
    std::string provision = "{\"app-distribution-type\": \"app_gallery\",\"bundle-info\":{\"app-f"
        "eature\":\"hos_system_app\",\"bundle-name\":\"com.example.nativetemplatedemo\",\"developer-"
        "id\":\"OpenHarmony\",\"development-certificate\":\"-----BEGIN CERTIFICATE-----\\n"
        "MIICXjCCAeOgAwIBAgIBATAKBggqhkjOPQQDAzBuMQswCQYDVQQGEwJDTjEUMBIG\\n"
        "A1UECgwLT3Blbkhhcm1vbnkxHjAcBgNVBAsMFU9wZW5IYXJtb255IENvbW11bml0\\n"
        "eTEpMCcGA1UEAwwgQXBwbGljYXRpb24gU2lnbmF0dXJlIFNlcnZpY2UgQ0EwHhcN\\n"
        "MjQwNDE1MDUzOTUyWhcNMjUwNDE1MDUzOTUyWjBaMQswCQYDVQQGEwJDTjEUMBIG\\n"
        "A1UECgwLT3Blbkhhcm1vbnkxHjAcBgNVBAsMFU9wZW5IYXJtb255IENvbW11bml0\\n"
        "eTEVMBMGA1UEAwwMQXBwMSBSZWxlYXNlMHYwEAYHKoZIzj0CAQYFK4EEACIDYgAE\\n"
        "odSztdiucyVAo7VQnzHzBJsS9vQYa1vU1cP92F6fiJLazWtvEljNP1XoJldSZaN9\\n"
        "UYGdAVHh2yrHzaJFEqHCSB3uQhlJgSbl9sT0lJ4hro1YvVx921/knMRlunz4eAGX\\n"
        "o2kwZzAMBgNVHRMBAf8EAjAAMAsGA1UdDwQEAwIHgDATBgNVHSUEDDAKBggrBgEF\\n"
        "BQcDAzAbBgNVHQ4EFEpzK6IntvQxLaKGX6xZQSiISBx+MBgGDCsGAQQBj1sCgngB\\n"
        "AwQIMAYCAQEKAQAwCgYIKoZIzj0EAwMDaQAwZgIxAPboDdi9EhOiwAhO3N6vTRcK\\n"
        "QT1K1TQq2vjvpC2231Dq4tLPeSzLz6ROq+Zv6IgBYgIxAJ9sZZUBoR2lgPHBzt01\\n"
        "4uxt5nLfJj2XKa6Leb/JWDoosXjoVXoB47y699PtGetcFw==\\n"
        "-----END CERTIFICATE-----\\n"
        "\"},\"debug-info\":{\"device-id-type\":\"udid\",\"device-ids\":[\"69"
        "C7505BE341BDA5948C3C0CB44ABCD530296054159EFE0BD16A16CD0129CC42\",\"7E"
        "ED06506FCE6325EB2E2FAA019458B856AB10493A6718C7679A73F958732865\"]},\"is"
        "suer\":\"pki_internal\",\"permissions\":{\"restricted-permissions\":[\"\"]},\"t"
        "ype\":\"release\",\"uuid\":\"fe686e1b-3770-4824-a938-961b140a7c98\",\"validity\":{\"no"
        "t-after\":1705127532,\"not-before\":1610519532},\"version-code\":\"0\",\"version-name\":\"1.0.0\"}";
    AppProvisionVerifyResult result = ParseProvision(provision, info);
    EXPECT_FALSE(result == PROVISION_OK);
}

/**
 * @tc.name: provision_test040
 * @tc.desc: Test function of ParseProvision() failed for distribution-certificate is empty.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(ProvisionTest, provision_test040, testing::ext::TestSize.Level1)
{
    ProfileInfo info;
    std::string provision = "{\"app-distribution-type\": \"app_gallery\",\"app-privilege-cap"
        "abilities\": [99,\"88\"],\"bundle-info\":{\"app-feature\":\"hos_system_app\",\"bundle-"
        "name\":\"com.example.nativetemplatedemo\",\"developer-id\":\"OpenHarmony\",\"develop"
        "ment-certificate\":\"-----BEGIN CERTIFICATE-----\\n"
        "MIICXjCCAeOgAwIBAgIBATAKBggqhkjOPQQDAzBuMQswCQYDVQQGEwJDTjEUMBIG\\n"
        "A1UECgwLT3Blbkhhcm1vbnkxHjAcBgNVBAsMFU9wZW5IYXJtb255IENvbW11bml0\\n"
        "eTEpMCcGA1UEAwwgQXBwbGljYXRpb24gU2lnbmF0dXJlIFNlcnZpY2UgQ0EwHhcN\\n"
        "MjQwNDE1MDUzOTUyWhcNMjUwNDE1MDUzOTUyWjBaMQswCQYDVQQGEwJDTjEUMBIG\\n"
        "A1UECgwLT3Blbkhhcm1vbnkxHjAcBgNVBAsMFU9wZW5IYXJtb255IENvbW11bml0\\n"
        "eTEVMBMGA1UEAwwMQXBwMSBSZWxlYXNlMHYwEAYHKoZIzj0CAQYFK4EEACIDYgAE\\n"
        "odSztdiucyVAo7VQnzHzBJsS9vQYa1vU1cP92F6fiJLazWtvEljNP1XoJldSZaN9\\n"
        "UYGdAVHh2yrHzaJFEqHCSB3uQhlJgSbl9sT0lJ4hro1YvVx921/knMRlunz4eAGX\\n"
        "o2kwZzAMBgNVHRMBAf8EAjAAMAsGA1UdDwQEAwIHgDATBgNVHSUEDDAKBggrBgEF\\n"
        "BQcDAzAbBgNVHQ4EFEpzK6IntvQxLaKGX6xZQSiISBx+MBgGDCsGAQQBj1sCgngB\\n"
        "AwQIMAYCAQEKAQAwCgYIKoZIzj0EAwMDaQAwZgIxAPboDdi9EhOiwAhO3N6vTRcK\\n"
        "QT1K1TQq2vjvpC2231Dq4tLPeSzLz6ROq+Zv6IgBYgIxAJ9sZZUBoR2lgPHBzt01\\n"
        "4uxt5nLfJj2XKa6Leb/JWDoosXjoVXoB47y699PtGetcFw==\\n"
        "-----END CERTIFICATE-----\\n"
        "\"},\"debug-info\":{\"device-id-type\":\"udid\",\"device-ids\":[\"69C7505BE341B"
        "DA5948C3C0CB44ABCD530296054159EFE0BD16A16CD0129CC42\",\"7EED06506FCE6325EB2E2F"
        "AA019458B856AB10493A6718C7679A73F958732865\"]},\"issuer\":\"pki_internal\",\"perm"
        "issions\":{\"restricted-permissions\":[\"\"]},\"type\":\"release\",\"uuid\":\"fe6"
        "86e1b-3770-4824-a938-961b140a7c98\",\"validity\":{\"not-after\":1705127532,\"not-bef"
        "ore\":1610519532},\"version-code\":1,\"version-name\":\"1.0.0\"}";
    AppProvisionVerifyResult result = ParseProvision(provision, info);
    EXPECT_FALSE(result == PROVISION_OK);
}

/**
 * @tc.name: provision_test041
 * @tc.desc: Test function of ParseProvision() failed for distribution-certificate is empty.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(ProvisionTest, provision_test041, testing::ext::TestSize.Level1)
{
    ProfileInfo info;
    std::string provision = "{\"app-distribution-type\": \"app_gallery\",\"bundle-in"
        "fo\":{\"app-feature\":\"hos_system_app\",\"bundle-name\":\"\",\"developer-id\":\"Op"
        "enHarmony\",\"development-certificate\":\"-----BEGIN CERTIFICATE-----\\n"
        "MIICXjCCAeOgAwIBAgIBATAKBggqhkjOPQQDAzBuMQswCQYDVQQGEwJDTjEUMBIG\\n"
        "A1UECgwLT3Blbkhhcm1vbnkxHjAcBgNVBAsMFU9wZW5IYXJtb255IENvbW11bml0\\n"
        "eTEpMCcGA1UEAwwgQXBwbGljYXRpb24gU2lnbmF0dXJlIFNlcnZpY2UgQ0EwHhcN\\n"
        "MjQwNDE1MDUzOTUyWhcNMjUwNDE1MDUzOTUyWjBaMQswCQYDVQQGEwJDTjEUMBIG\\n"
        "A1UECgwLT3Blbkhhcm1vbnkxHjAcBgNVBAsMFU9wZW5IYXJtb255IENvbW11bml0\\n"
        "eTEVMBMGA1UEAwwMQXBwMSBSZWxlYXNlMHYwEAYHKoZIzj0CAQYFK4EEACIDYgAE\\n"
        "odSztdiucyVAo7VQnzHzBJsS9vQYa1vU1cP92F6fiJLazWtvEljNP1XoJldSZaN9\\n"
        "UYGdAVHh2yrHzaJFEqHCSB3uQhlJgSbl9sT0lJ4hro1YvVx921/knMRlunz4eAGX\\n"
        "o2kwZzAMBgNVHRMBAf8EAjAAMAsGA1UdDwQEAwIHgDATBgNVHSUEDDAKBggrBgEF\\n"
        "BQcDAzAbBgNVHQ4EFEpzK6IntvQxLaKGX6xZQSiISBx+MBgGDCsGAQQBj1sCgngB\\n"
        "AwQIMAYCAQEKAQAwCgYIKoZIzj0EAwMDaQAwZgIxAPboDdi9EhOiwAhO3N6vTRcK\\n"
        "QT1K1TQq2vjvpC2231Dq4tLPeSzLz6ROq+Zv6IgBYgIxAJ9sZZUBoR2lgPHBzt01\\n"
        "4uxt5nLfJj2XKa6Leb/JWDoosXjoVXoB47y699PtGetcFw==\\n"
        "-----END CERTIFICATE-----\\n"
        "\"},\"debug-info\":{\"device-id-type\":\"udid\",\"device-ids\":[\"69C7505BE341BDA"
        "5948C3C0CB44ABCD530296054159EFE0BD16A16CD0129CC42\",\"7EED06506FCE6325EB2E2FAA0194"
        "58B856AB10493A6718C7679A73F958732865\"]},\"issuer\":\"pki_internal\",\"permissions"
        "\":{\"restricted-permissions\":[\"\"]},\"type\":\"release\",\"uuid\":\"fe686e1b-37"
        "70-4824-a938-961b140a7c98\",\"validity\":{\"not-after\":1705127532,\"not-before\":1610"
        "519532},\"version-code\":1,\"version-name\":\"1.0.0\"}";
    AppProvisionVerifyResult result = ParseProvision(provision, info);
    EXPECT_FALSE(result == PROVISION_OK);
}

/**
 * @tc.name: provision_test042
 * @tc.desc: Test function of ParseProvision() failed for distribution-certificate is empty.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(ProvisionTest, provision_test042, testing::ext::TestSize.Level1)
{
    ProfileInfo info;
    std::string provision = "{\"app-distribution-type\": \"app_gallery\",\"bundle-info\":{\"ap"
        "p-feature\":\"hos_system_app\",\"bundle-name\":\"com.example.nativetemplatedemo\",\"devel"
        "oper-id\":\"OpenHarmony\",\"development-certificate\":\"-----BEGIN CERTIFICATE-----\\n"
        "MIICXjCCAeOgAwIBAgIBATAKBggqhkjOPQQDAzBuMQswCQYDVQQGEwJDTjEUMBIG\\n"
        "A1UECgwLT3Blbkhhcm1vbnkxHjAcBgNVBAsMFU9wZW5IYXJtb255IENvbW11bml0\\n"
        "eTEpMCcGA1UEAwwgQXBwbGljYXRpb24gU2lnbmF0dXJlIFNlcnZpY2UgQ0EwHhcN\\n"
        "MjQwNDE1MDUzOTUyWhcNMjUwNDE1MDUzOTUyWjBaMQswCQYDVQQGEwJDTjEUMBIG\\n"
        "A1UECgwLT3Blbkhhcm1vbnkxHjAcBgNVBAsMFU9wZW5IYXJtb255IENvbW11bml0\\n"
        "eTEVMBMGA1UEAwwMQXBwMSBSZWxlYXNlMHYwEAYHKoZIzj0CAQYFK4EEACIDYgAE\\n"
        "odSztdiucyVAo7VQnzHzBJsS9vQYa1vU1cP92F6fiJLazWtvEljNP1XoJldSZaN9\\n"
        "UYGdAVHh2yrHzaJFEqHCSB3uQhlJgSbl9sT0lJ4hro1YvVx921/knMRlunz4eAGX\\n"
        "o2kwZzAMBgNVHRMBAf8EAjAAMAsGA1UdDwQEAwIHgDATBgNVHSUEDDAKBggrBgEF\\n"
        "BQcDAzAbBgNVHQ4EFEpzK6IntvQxLaKGX6xZQSiISBx+MBgGDCsGAQQBj1sCgngB\\n"
        "AwQIMAYCAQEKAQAwCgYIKoZIzj0EAwMDaQAwZgIxAPboDdi9EhOiwAhO3N6vTRcK\\n"
        "QT1K1TQq2vjvpC2231Dq4tLPeSzLz6ROq+Zv6IgBYgIxAJ9sZZUBoR2lgPHBzt01\\n"
        "4uxt5nLfJj2XKa6Leb/JWDoosXjoVXoB47y699PtGetcFw==\\n"
        "-----END CERTIFICATE-----\\n"
        "\"},\"debug-info\":{\"device-id-type\":\"udid\",\"device-ids\":[\"69C7505BE341BDA5948C3C"
        "0CB44ABCD530296054159EFE0BD16A16CD0129CC42\",\"7EED06506FCE6325EB2E2FAA019458B856AB10493"
        "A6718C7679A73F958732865\"]},\"issuer\":\"pki_internal\",\"permissions\":{\"restricted-pe"
        "rmissions\":[\"\"]},\"type\":\"release\",\"uuid\":\"fe686e1b-3770-4824-a938-961b140a7c9"
        "8\",\"validity\":{\"not-after\":1705127532,\"not-before\":1610519532},\"version-code\":"
        "1,\"version-name\":\"\"}";
    AppProvisionVerifyResult result = ParseProvision(provision, info);
    EXPECT_FALSE(result == PROVISION_OK);
}

/**
 * @tc.name: provision_test043
 * @tc.desc: Test function of ParseProvision() failed for distribution-certificate is empty.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(ProvisionTest, provision_test043, testing::ext::TestSize.Level1)
{
    ProfileInfo info;
    std::string provision = "{\"app-distribution-type\": \"app_gallery\",\"bundle-info\":{\"a"
        "pp-feature\":\"hos_system_app\",\"bundle-name\":\"com.example.nativetemplatedemo\",\"d"
        "eveloper-id\":\"OpenHarmony\",\"development-certificate\":\"-----BEGIN CERTIFICATE-----\\n"
        "MIICXjCCAeOgAwIBAgIBATAKBggqhkjOPQQDAzBuMQswCQYDVQQGEwJDTjEUMBIG\\n"
        "A1UECgwLT3Blbkhhcm1vbnkxHjAcBgNVBAsMFU9wZW5IYXJtb255IENvbW11bml0\\n"
        "eTEpMCcGA1UEAwwgQXBwbGljYXRpb24gU2lnbmF0dXJlIFNlcnZpY2UgQ0EwHhcN\\n"
        "MjQwNDE1MDUzOTUyWhcNMjUwNDE1MDUzOTUyWjBaMQswCQYDVQQGEwJDTjEUMBIG\\n"
        "A1UECgwLT3Blbkhhcm1vbnkxHjAcBgNVBAsMFU9wZW5IYXJtb255IENvbW11bml0\\n"
        "eTEVMBMGA1UEAwwMQXBwMSBSZWxlYXNlMHYwEAYHKoZIzj0CAQYFK4EEACIDYgAE\\n"
        "odSztdiucyVAo7VQnzHzBJsS9vQYa1vU1cP92F6fiJLazWtvEljNP1XoJldSZaN9\\n"
        "UYGdAVHh2yrHzaJFEqHCSB3uQhlJgSbl9sT0lJ4hro1YvVx921/knMRlunz4eAGX\\n"
        "o2kwZzAMBgNVHRMBAf8EAjAAMAsGA1UdDwQEAwIHgDATBgNVHSUEDDAKBggrBgEF\\n"
        "BQcDAzAbBgNVHQ4EFEpzK6IntvQxLaKGX6xZQSiISBx+MBgGDCsGAQQBj1sCgngB\\n"
        "AwQIMAYCAQEKAQAwCgYIKoZIzj0EAwMDaQAwZgIxAPboDdi9EhOiwAhO3N6vTRcK\\n"
        "QT1K1TQq2vjvpC2231Dq4tLPeSzLz6ROq+Zv6IgBYgIxAJ9sZZUBoR2lgPHBzt01\\n"
        "4uxt5nLfJj2XKa6Leb/JWDoosXjoVXoB47y699PtGetcFw==\\n"
        "-----END CERTIFICATE-----\\n"
        "\"},\"debug-info\":{\"device-id-type\":\"udid\",\"device-ids\":[\"69C7505BE341BDA59"
        "48C3C0CB44ABCD530296054159EFE0BD16A16CD0129CC42\",\"7EED06506FCE6325EB2E2FAA019458B"
        "856AB10493A6718C7679A73F958732865\"]},\"issuer\":\"pki_internal\",\"permissions\":{\"r"
        "estricted-permissions\":[\"\"]},\"type\":\"release\",\"uuid\":\"\",\"validity\":{\"no"
        "t-after\":1705127532,\"not-before\":1610519532},\"version-code\":1,\"version-name\":\"\"}";
    AppProvisionVerifyResult result = ParseProvision(provision, info);
    EXPECT_FALSE(result == PROVISION_OK);
}

/**
 * @tc.name: provision_test044
 * @tc.desc: Test function of ParseProvision() failed for distribution-certificate is empty.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(ProvisionTest, provision_test044, testing::ext::TestSize.Level1)
{
    ProfileInfo info;
    std::string provision = "{\"app-distribution-type\": \"app_gallery\",\"bundle-info\":{\"app"
        "-feature\":\"hos_system_app\",\"bundle-name\":\"com.example.nativetemplatedemo\",\"develo"
        "per-id\":\"\",\"development-certificate\":\"-----BEGIN CERTIFICATE-----\\n"
        "MIICXjCCAeOgAwIBAgIBATAKBggqhkjOPQQDAzBuMQswCQYDVQQGEwJDTjEUMBIG\\n"
        "A1UECgwLT3Blbkhhcm1vbnkxHjAcBgNVBAsMFU9wZW5IYXJtb255IENvbW11bml0\\n"
        "eTEpMCcGA1UEAwwgQXBwbGljYXRpb24gU2lnbmF0dXJlIFNlcnZpY2UgQ0EwHhcN\\n"
        "MjQwNDE1MDUzOTUyWhcNMjUwNDE1MDUzOTUyWjBaMQswCQYDVQQGEwJDTjEUMBIG\\n"
        "A1UECgwLT3Blbkhhcm1vbnkxHjAcBgNVBAsMFU9wZW5IYXJtb255IENvbW11bml0\\n"
        "eTEVMBMGA1UEAwwMQXBwMSBSZWxlYXNlMHYwEAYHKoZIzj0CAQYFK4EEACIDYgAE\\n"
        "odSztdiucyVAo7VQnzHzBJsS9vQYa1vU1cP92F6fiJLazWtvEljNP1XoJldSZaN9\\n"
        "UYGdAVHh2yrHzaJFEqHCSB3uQhlJgSbl9sT0lJ4hro1YvVx921/knMRlunz4eAGX\\n"
        "o2kwZzAMBgNVHRMBAf8EAjAAMAsGA1UdDwQEAwIHgDATBgNVHSUEDDAKBggrBgEF\\n"
        "BQcDAzAbBgNVHQ4EFEpzK6IntvQxLaKGX6xZQSiISBx+MBgGDCsGAQQBj1sCgngB\\n"
        "AwQIMAYCAQEKAQAwCgYIKoZIzj0EAwMDaQAwZgIxAPboDdi9EhOiwAhO3N6vTRcK\\n"
        "QT1K1TQq2vjvpC2231Dq4tLPeSzLz6ROq+Zv6IgBYgIxAJ9sZZUBoR2lgPHBzt01\\n"
        "4uxt5nLfJj2XKa6Leb/JWDoosXjoVXoB47y699PtGetcFw==\\n"
        "-----END CERTIFICATE-----\\n"
        "\"},\"debug-info\":{\"device-id-type\":\"udid\",\"device-ids\":[\"69C7505BE341BDA5"
        "948C3C0CB44ABCD530296054159EFE0BD16A16CD0129CC42\",\"7EED06506FCE6325EB2E2FAA019458B"
        "856AB10493A6718C7679A73F958732865\"]},\"issuer\":\"pki_internal\",\"permissions\":{\"re"
        "stricted-permissions\":[\"\"]},\"type\":\"release\",\"uuid\":\"fe686e1b-3770-4824-a938"
        "-961b140a7c98\",\"validity\":{\"not-after\":1705127532,\"not-before\":1610519532},\"ver"
        "sion-code\":1,\"version-name\":\"1.0.0\"}";
    AppProvisionVerifyResult result = ParseProvision(provision, info);
    EXPECT_FALSE(result == PROVISION_OK);
}

/**
 * @tc.name: provision_test045
 * @tc.desc: Test function of ParseProvision() failed for development-certificate is empty..
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(ProvisionTest, provision_test045, testing::ext::TestSize.Level1)
{
    ProfileInfo info;
    std::string provision = "{\"app-distribution-type\": \"app_gallery\",\"bundle-info\":{\"a"
        "pp-feature\":\"hos_system_app\",\"bundle-name\":\"com.example.nativetemplatedemo\",\"dev"
        "eloper-id\":\"OpenHarmony\",\"development-certificate\":\"\"},\"debug-info\":{\"device-i"
        "d-type\":\"udid\",\"device-ids\":[\"69C7505BE341BDA5948C3C0CB44ABCD530296054159EFE0BD16"
        "A16CD0129CC42\",\"7EED06506FCE6325EB2E2FAA019458B856AB10493A6718C7679A73F958732865\"]},\"i"
        "ssuer\":\"pki_internal\",\"permissions\":{\"restricted-permissions\":[\"\"]},\"type\":\"de"
        "bug\",\"uuid\":\"fe686e1b-3770-4824-a938-961b140a7c98\",\"validity\":{\"not-after\":17051275"
        "32,\"not-before\":1610519532},\"version-code\":1,\"version-name\":\"1.0.0\"}";
    AppProvisionVerifyResult result = ParseProvision(provision, info);
    EXPECT_FALSE(result == PROVISION_OK);
}

/**
 * @tc.name: provision_test046
 * @tc.desc: Test function of ParseProvision() distribution-certificate is empty..
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(ProvisionTest, provision_test046, testing::ext::TestSize.Level1)
{
    ProfileInfo info;
    std::string provision = "{\"app-distribution-type\": \"app_gallery\",\"bundle-in"
        "fo\":{\"app-feature\":\"\",\"bundle-name\":\"com.example.nativetemplatedemo\",\"de"
        "veloper-id\":\"OpenHarmony\",\"development-certificate\":\"-----BEGIN CERTIFICATE-----\\n"
        "MIICXjCCAeOgAwIBAgIBATAKBggqhkjOPQQDAzBuMQswCQYDVQQGEwJDTjEUMBIG\\n"
        "A1UECgwLT3Blbkhhcm1vbnkxHjAcBgNVBAsMFU9wZW5IYXJtb255IENvbW11bml0\\n"
        "eTEpMCcGA1UEAwwgQXBwbGljYXRpb24gU2lnbmF0dXJlIFNlcnZpY2UgQ0EwHhcN\\n"
        "MjQwNDE1MDUzOTUyWhcNMjUwNDE1MDUzOTUyWjBaMQswCQYDVQQGEwJDTjEUMBIG\\n"
        "A1UECgwLT3Blbkhhcm1vbnkxHjAcBgNVBAsMFU9wZW5IYXJtb255IENvbW11bml0\\n"
        "eTEVMBMGA1UEAwwMQXBwMSBSZWxlYXNlMHYwEAYHKoZIzj0CAQYFK4EEACIDYgAE\\n"
        "odSztdiucyVAo7VQnzHzBJsS9vQYa1vU1cP92F6fiJLazWtvEljNP1XoJldSZaN9\\n"
        "UYGdAVHh2yrHzaJFEqHCSB3uQhlJgSbl9sT0lJ4hro1YvVx921/knMRlunz4eAGX\\n"
        "o2kwZzAMBgNVHRMBAf8EAjAAMAsGA1UdDwQEAwIHgDATBgNVHSUEDDAKBggrBgEF\\n"
        "BQcDAzAbBgNVHQ4EFEpzK6IntvQxLaKGX6xZQSiISBx+MBgGDCsGAQQBj1sCgngB\\n"
        "AwQIMAYCAQEKAQAwCgYIKoZIzj0EAwMDaQAwZgIxAPboDdi9EhOiwAhO3N6vTRcK\\n"
        "QT1K1TQq2vjvpC2231Dq4tLPeSzLz6ROq+Zv6IgBYgIxAJ9sZZUBoR2lgPHBzt01\\n"
        "4uxt5nLfJj2XKa6Leb/JWDoosXjoVXoB47y699PtGetcFw==\\n"
        "-----END CERTIFICATE-----\\n"
        "\"},\"debug-info\":{\"device-id-type\":\"udid\",\"device-ids\":[\"69C7505BE341BDA59"
        "48C3C0CB44ABCD530296054159EFE0BD16A16CD0129CC42\",\"7EED06506FCE6325EB2E2FAA019458B8"
        "56AB10493A6718C7679A73F958732865\"]},\"issuer\":\"pki_internal\",\"permissions\":{\"re"
        "stricted-permissions\":[\"\"]},\"type\":\"release\",\"uuid\":\"fe686e1b-3770-4824-a938-"
        "961b140a7c98\",\"validity\":{\"not-after\":1705127532,\"not-before\":1610519532},\"ver"
        "sion-code\":1,\"version-name\":\"1.0.0\"}";
    AppProvisionVerifyResult result = ParseProvision(provision, info);
    EXPECT_FALSE(result == PROVISION_OK);
}

/**
 * @tc.name: provision_test047
 * @tc.desc: Test function of ParseProvision() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(ProvisionTest, provision_test047, testing::ext::TestSize.Level1)
{
    ProfileInfo info;
    std::string provision = "{\"app-distribution-type\": \"app_gallery\",\"bundle-info\":{\"a"
        "pl\":\"apl_\",\"app-feature\":\"hos_system_app\",\"bundle-name\":\"com.example.nativetemp"
        "latedemo\",\"developer-id\":\"OpenHarmony\",\"development-certificate\":\"-----BEGIN CERTIFICATE-----\\n"
        "MIICXjCCAeOgAwIBAgIBATAKBggqhkjOPQQDAzBuMQswCQYDVQQGEwJDTjEUMBIG\\n"
        "A1UECgwLT3Blbkhhcm1vbnkxHjAcBgNVBAsMFU9wZW5IYXJtb255IENvbW11bml0\\n"
        "eTEpMCcGA1UEAwwgQXBwbGljYXRpb24gU2lnbmF0dXJlIFNlcnZpY2UgQ0EwHhcN\\n"
        "MjQwNDE1MDUzOTUyWhcNMjUwNDE1MDUzOTUyWjBaMQswCQYDVQQGEwJDTjEUMBIG\\n"
        "A1UECgwLT3Blbkhhcm1vbnkxHjAcBgNVBAsMFU9wZW5IYXJtb255IENvbW11bml0\\n"
        "eTEVMBMGA1UEAwwMQXBwMSBSZWxlYXNlMHYwEAYHKoZIzj0CAQYFK4EEACIDYgAE\\n"
        "odSztdiucyVAo7VQnzHzBJsS9vQYa1vU1cP92F6fiJLazWtvEljNP1XoJldSZaN9\\n"
        "UYGdAVHh2yrHzaJFEqHCSB3uQhlJgSbl9sT0lJ4hro1YvVx921/knMRlunz4eAGX\\n"
        "o2kwZzAMBgNVHRMBAf8EAjAAMAsGA1UdDwQEAwIHgDATBgNVHSUEDDAKBggrBgEF\\n"
        "BQcDAzAbBgNVHQ4EFEpzK6IntvQxLaKGX6xZQSiISBx+MBgGDCsGAQQBj1sCgngB\\n"
        "AwQIMAYCAQEKAQAwCgYIKoZIzj0EAwMDaQAwZgIxAPboDdi9EhOiwAhO3N6vTRcK\\n"
        "QT1K1TQq2vjvpC2231Dq4tLPeSzLz6ROq+Zv6IgBYgIxAJ9sZZUBoR2lgPHBzt01\\n"
        "4uxt5nLfJj2XKa6Leb/JWDoosXjoVXoB47y699PtGetcFw==\\n"
        "-----END CERTIFICATE-----\\n"
        "\"},\"debug-info\":{\"device-id-type\":\"udid\",\"device-ids\":[\"69C7505BE341BDA594"
        "8C3C0CB44ABCD530296054159EFE0BD16A16CD0129CC42\",\"7EED06506FCE6325EB2E2FAA019458B856A"
        "B10493A6718C7679A73F958732865\"]},\"issuer\":\"pki_internal\",\"permissions\":{\"restr"
        "icted-permissions\":[\"\"]},\"type\":\"debug\",\"uuid\":\"fe686e1b-3770-4824-a938-961b"
        "140a7c98\",\"validity\":{\"not-after\":1705127532,\"not-before\":1610519532},\"versio"
        "n-code\":3,\"version-name\":\"1.0.0\"}";
    AppProvisionVerifyResult result = ParseProvision(provision, info);
    EXPECT_TRUE(result == PROVISION_OK);
}

}
}
