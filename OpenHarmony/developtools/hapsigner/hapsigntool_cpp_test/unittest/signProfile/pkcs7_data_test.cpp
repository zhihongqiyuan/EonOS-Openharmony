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
#include "verify_hap_openssl_utils.h"

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
static const std::string VERIFY_PROFILE_IN_FILE = "./signProfile/signed-profile.p7b";
static const std::string VERIFY_PROFILE_OUT_FILE = "./signProfile/VerifyResult.json";

class Pkcs7DataTest : public testing::Test {
public:
    static void SetUpTestCase(void);
    static void TearDownTestCase(void);
    void SetUp()override;
    void TearDown()override;
};
void Pkcs7DataTest::SetUpTestCase(void)
{
}

void Pkcs7DataTest::TearDownTestCase(void)
{
}

void Pkcs7DataTest::SetUp()
{
}

void Pkcs7DataTest::TearDown()
{
}

/**
 * @tc.name: pkcs7_test001
 * @tc.desc: Test function of VerifyCertOpensslUtils::CertVerify（） failed for invalid param.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(Pkcs7DataTest, pkcs7_test001, testing::ext::TestSize.Level1)
{
    X509* cert = X509_new();
    X509* issuerCert = X509_new();
    bool result = VerifyCertOpensslUtils::CertVerify(NULL, issuerCert);
    X509_free(cert);
    X509_free(issuerCert);
    EXPECT_FALSE(result);
}

/**
 * @tc.name: pkcs7_test002
 * @tc.desc: Test function of VerifyCertOpensslUtils::CertVerify() failed for invalid param.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(Pkcs7DataTest, pkcs7_test002, testing::ext::TestSize.Level1)
{
    X509* cert = X509_new();
    X509* issuerCert = X509_new();
    bool result = VerifyCertOpensslUtils::CertVerify(cert, NULL);
    X509_free(cert);
    X509_free(issuerCert);
    EXPECT_FALSE(result);
}

/**
 * @tc.name: pkcs7_test003
 * @tc.desc: Test function of VerifyCertOpensslUtils::CertVerify failed for invalid cert.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(Pkcs7DataTest, pkcs7_test003, testing::ext::TestSize.Level1)
{
    X509* cert = X509_new();
    X509* issuerCert = X509_new();
    bool result = VerifyCertOpensslUtils::CertVerify(cert, issuerCert);
    X509_free(cert);
    X509_free(issuerCert);
    EXPECT_FALSE(result);
}

/**
 * @tc.name: pkcs7_test004
 * @tc.desc: Test function of VerifyCertOpensslUtils::CertVerify() failed for invalid cert.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(Pkcs7DataTest, pkcs7_test004, testing::ext::TestSize.Level1)
{
    X509* cert = X509_new();
    X509* issuerCert = X509_new();
    bool result = VerifyCertOpensslUtils::CertVerify(cert, issuerCert);
    X509_free(cert);
    X509_free(issuerCert);
    EXPECT_FALSE(result);
}

static void ReverseX509Stack(STACK_OF(X509)* certs)
{
    if (certs == NULL)
        return;
    std::vector<X509*> certChain;
    for (int i = 0; i < sk_X509_num(certs); i++) {
        certChain.push_back(sk_X509_value(certs, i));
    }
    std::reverse(certChain.begin(), certChain.end());
    while (sk_X509_num(certs))sk_X509_pop(certs);
    for (int i = 0; i < static_cast<int>(certChain.size()); i++)
        sk_X509_push(certs, certChain[i]);
}

\
/**
 * @tc.name: pkcs7_test005
 * @tc.desc: Test function of VerifyCertOpensslUtils::CertVerify() interface cert signature verify failed.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(Pkcs7DataTest, pkcs7_test005, testing::ext::TestSize.Level1)
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
    STACK_OF(X509)* certs = signer->GetCertificates();
    ReverseX509Stack(certs);
    bool result = VerifyCertOpensslUtils::CertVerify(sk_X509_value(certs, 0), sk_X509_value(certs, 1));
    PKCS7Data::PrintCertChainSub(certs);
    EXPECT_FALSE(result);
}


/**
 * @tc.name: pkcs7_test007
 * @tc.desc: Test function of PKCS7Data::GetASN1Time() failed for too large time.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(Pkcs7DataTest, pkcs7_test007, testing::ext::TestSize.Level1)
{
    ASN1_TIME* time = ASN1_TIME_new();
    ASN1_TIME_set(time, -1);
    std::string result = PKCS7Data::GetASN1Time(time);
    ASN1_TIME_free(time);
    EXPECT_TRUE(result.empty());
}

/**
 * @tc.name: pkcs7_test008
 * @tc.desc: Test function of VerifyCertOpensslUtils::GetTextFromX509Name() failed for invalid param.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(Pkcs7DataTest, pkcs7_test008, testing::ext::TestSize.Level1)
{
    std::string text;
    VerifyCertOpensslUtils::GetTextFromX509Name(NULL, 45, text);
    EXPECT_TRUE(text.empty());
}

/**
 * @tc.name: pkcs7_test009
 * @tc.desc: Test function of VerifyCertOpensslUtils::GetDnToString() failed for invalid param.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(Pkcs7DataTest, pkcs7_test009, testing::ext::TestSize.Level1)
{
    std::string result = VerifyCertOpensslUtils::GetDnToString(NULL);
    EXPECT_TRUE(result.empty());
}

/**
 * @tc.name: pkcs7_test010
 * @tc.desc: Test function of VerifyCertOpensslUtils::GetSubjectFromX509() failed for invalid param.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(Pkcs7DataTest, pkcs7_test010, testing::ext::TestSize.Level1)
{
    std::string subject;
    bool result = VerifyCertOpensslUtils::GetSubjectFromX509(NULL, subject);
    EXPECT_FALSE(result);
}

/**
 * @tc.name: pkcs7_test011
 * @tc.desc: Test function of PKCS7Data::X509NameCompare() failed for invalid param.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(Pkcs7DataTest, pkcs7_test011, testing::ext::TestSize.Level1)
{
    int result = PKCS7Data::X509NameCompare(NULL, NULL);
    EXPECT_EQ(result, false);
}

/**
 * @tc.name: pkcs7_test012
 * @tc.desc: Test function of PKCS7Data::X509NameCompare() failed for invalid param.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(Pkcs7DataTest, pkcs7_test012, testing::ext::TestSize.Level1)
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
    STACK_OF(X509)* certs = signer->GetCertificates();
    int result = PKCS7Data::X509NameCompare(sk_X509_value(certs, 0), NULL);
    EXPECT_EQ(result, false);
}

/**
 * @tc.name: pkcs7_test013
 * @tc.desc: Test function of PKCS7Data::X509NameCompare() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(Pkcs7DataTest, pkcs7_test013, testing::ext::TestSize.Level1)
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
    STACK_OF(X509)* certs = signer->GetCertificates();
    int result = PKCS7Data::X509NameCompare(sk_X509_value(certs, 0), sk_X509_value(certs, 1));
    EXPECT_EQ(result, true);
}

/**
 * @tc.name: pkcs7_test014
 * @tc.desc: Test function of Pkcs7Data::Parse() failed for invalid pkcs7.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(Pkcs7DataTest, pkcs7_test014, testing::ext::TestSize.Level1)
{
    PKCS7Data p7Data;
    unsigned char buf[15] = "hello";
    const unsigned char* p = buf;
    int len = 5;
    std::string p7b(p, p + len);
    int result = p7Data.Parse(p7b);
    EXPECT_TRUE(result < 0);
}

/**
 * @tc.name: pkcs7_test015
 * @tc.desc: Test function of PKCS7Data::CheckSignTimeInValidPeriod() failed for invalid asn1_time.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(Pkcs7DataTest, pkcs7_test015, testing::ext::TestSize.Level1)
{
    ASN1_TYPE* signTime = NULL;
    ASN1_TIME* notBefore = NULL;
    ASN1_TIME* notAfter = NULL;
    PKCS7Data p7Data;
    int result = PKCS7Data::CheckSignTimeInValidPeriod(signTime, notBefore, notAfter);
    EXPECT_TRUE(result < 0);
}

/**
 * @tc.name: pkcs7_test016
 * @tc.desc: Test function of PKCS7Data::CheckSignTimeInValidPeriod() failed for invalid asn1_time.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(Pkcs7DataTest, pkcs7_test016, testing::ext::TestSize.Level1)
{
    ASN1_TYPE* signTime = NULL;
    ASN1_TIME notBefore{ 0 };
    notBefore.data = NULL;
    ASN1_TIME* notAfter = NULL;
    PKCS7Data p7Data;
    int result = PKCS7Data::CheckSignTimeInValidPeriod(signTime, &notBefore, notAfter);
    EXPECT_TRUE(result < 0);
}

/**
 * @tc.name: pkcs7_test017
 * @tc.desc: Test function of PKCS7Data::CheckSignTimeInValidPeriod() failed for invalid asn1_time.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(Pkcs7DataTest, pkcs7_test017, testing::ext::TestSize.Level1)
{
    unsigned char data[5] = "hell";
    ASN1_TYPE* signTime = NULL;
    ASN1_TIME notBefore{ 0 };
    notBefore.data = data;
    ASN1_TIME* notAfter = NULL;
    PKCS7Data p7Data;
    int result = PKCS7Data::CheckSignTimeInValidPeriod(signTime, &notBefore, notAfter);
    EXPECT_TRUE(result < 0);
}

/**
 * @tc.name: pkcs7_test018
 * @tc.desc: Test function of PKCS7Data::CheckSignTimeInValidPeriod() failed for invalid asn1_time.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(Pkcs7DataTest, pkcs7_test018, testing::ext::TestSize.Level1)
{
    unsigned char data[5] = "hell";
    ASN1_TYPE* signTime = NULL;
    ASN1_TIME notBefore{ 0 };
    notBefore.data = data;
    ASN1_TIME notAfter;
    notAfter.data = NULL;
    PKCS7Data p7Data;
    int result = PKCS7Data::CheckSignTimeInValidPeriod(signTime, &notBefore, &notAfter);
    EXPECT_TRUE(result < 0);
}

/**
 * @tc.name: pkcs7_test019
 * @tc.desc: Test function of PKCS7Data::CheckSignTimeInValidPeriod() failed for invalid asn1_time.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(Pkcs7DataTest, pkcs7_test019, testing::ext::TestSize.Level1)
{
    unsigned char data[5] = "hell";
    ASN1_TYPE* signTime = NULL;
    ASN1_TIME notBefore{ 0 };
    notBefore.data = data;
    ASN1_TIME notAfter;
    notAfter.data = data;
    PKCS7Data p7Data;
    int result = PKCS7Data::CheckSignTimeInValidPeriod(signTime, &notBefore, &notAfter);
    EXPECT_TRUE(result < 0);
}

/**
 * @tc.name: pkcs7_test020
 * @tc.desc: Test function of PKCS7Data::CheckSignTimeInValidPeriod() failed for invalid asn1_time.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(Pkcs7DataTest, pkcs7_test020, testing::ext::TestSize.Level1)
{
    unsigned char data[5] = "hell";
    ASN1_TYPE signTime;
    signTime.value.asn1_string = NULL;
    ASN1_TIME notBefore{ 0 };
    notBefore.data = data;
    ASN1_TIME notAfter;
    notAfter.data = data;
    PKCS7Data p7Data;
    int result = PKCS7Data::CheckSignTimeInValidPeriod(&signTime, &notBefore, &notAfter);
    EXPECT_TRUE(result < 0);
}

/**
 * @tc.name: pkcs7_test021
 * @tc.desc: Test function of PKCS7Data::CheckSignTimeInValidPeriod() failed for invalid asn1_time.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(Pkcs7DataTest, pkcs7_test021, testing::ext::TestSize.Level1)
{
    ASN1_STRING* tmp = ASN1_STRING_new();
    unsigned char data[5] = "hell";
    ASN1_TYPE signTime;
    signTime.value.asn1_string = tmp;
    signTime.value.asn1_string->data = NULL;
    ASN1_TIME notBefore{ 0 };
    notBefore.data = data;
    ASN1_TIME notAfter;
    notAfter.data = data;
    PKCS7Data p7Data;
    int result = PKCS7Data::CheckSignTimeInValidPeriod(&signTime, &notBefore, &notAfter);
    ASN1_STRING_free(tmp);
    EXPECT_TRUE(result < 0);
}

/**
 * @tc.name: pkcs7_test022
 * @tc.desc: Test function of PKCS7Data::CheckSignTimeInValidPeriod() failed for invalid asn1_time.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(Pkcs7DataTest, pkcs7_test022, testing::ext::TestSize.Level1)
{
    ASN1_TIME* tmp = NULL;
    ASN1_TYPE* signTime = NULL;
    ASN1_TIME* notBefore = NULL;
    ASN1_TIME* notAfter = NULL;
    time_t t1 = 365 * 24 * 3600;

    signTime = ASN1_TYPE_new();
    tmp = ASN1_TIME_new();
    notBefore = ASN1_TIME_new();
    notAfter = ASN1_TIME_new();
    time_t timeNow = time(NULL);
    ASN1_TIME_set(tmp, timeNow - t1);
    ASN1_TYPE_set(signTime, V_ASN1_UTCTIME, tmp);
    ASN1_TIME_set(notBefore, timeNow);
    ASN1_TIME_set(notAfter, timeNow + t1);
    int result = PKCS7Data::CheckSignTimeInValidPeriod(signTime, notBefore, notAfter);
    ASN1_TYPE_free(signTime);
    ASN1_TIME_free(notBefore);
    ASN1_TIME_free(notAfter);
    EXPECT_TRUE(result < 0);
}

/**
 * @tc.name: pkcs7_test023
 * @tc.desc: Test function of PKCS7Data::CheckSignTimeInValidPeriod() failed for invalid asn1_time.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(Pkcs7DataTest, pkcs7_test023, testing::ext::TestSize.Level1)
{
    ASN1_TIME* tmp = NULL;
    ASN1_TYPE* signTime = NULL;
    ASN1_TIME* notBefore = NULL;
    ASN1_TIME* notAfter = NULL;
    time_t t1 = 365 * 24 * 3600;

    signTime = ASN1_TYPE_new();
    tmp = ASN1_TIME_new();
    notBefore = ASN1_TIME_new();
    notAfter = ASN1_TIME_new();
    time_t timeNow = time(NULL);
    ASN1_TIME_set(tmp, timeNow + t1);
    ASN1_TYPE_set(signTime, V_ASN1_UTCTIME, tmp);
    ASN1_TIME_set(notBefore, timeNow - t1);
    ASN1_TIME_set(notAfter, timeNow);
    int result = PKCS7Data::CheckSignTimeInValidPeriod(signTime, notBefore, notAfter);
    ASN1_TYPE_free(signTime);
    ASN1_TIME_free(notBefore);
    ASN1_TIME_free(notAfter);
    EXPECT_TRUE(result < 0);
}

/**
 * @tc.name: pkcs7_test024
 * @tc.desc: Test function of HapSignTool::RunSignProfile() failed for  no requred mode.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(Pkcs7DataTest, pkcs7_test024, testing::ext::TestSize.Level1)
{
    Options options;
    char keyStorePwd[] = "123456";
    char keypwd[] = "123456";
    options[Options::KEY_ALIAS] = SIGN_PROFILE_KEY_ALIAS;
    options[Options::PROFILE_CERT_FILE] = SIGN_PROFILE_PROFILE_CERT_FILE;
    options[Options::SIGN_ALG] = SIGN_PROFILE_SIGN_ALG;
    options[Options::KEY_STORE_FILE] = SIGN_PROFILE_KEY_STORE_FILE;
    options[Options::OUT_FILE] = SIGN_PROFILE_OUT_FILE;
    options[Options::IN_FILE] = SIGN_PROFILE_IN_FILE;
    options[Options::KEY_RIGHTS] = keypwd;
    options[Options::KEY_STORE_RIGHTS] = keyStorePwd;

    LocalizationAdapter adapter(&options);
    SignToolServiceImpl api;
    bool result = ParamsRunTool::RunSignProfile(&options, api);
    EXPECT_EQ(result, false);
}

/**
 * @tc.name: pkcs7_test025
 * @tc.desc: Test function of HapSignTool::RunSignProfile() failed for no required sigAlg.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(Pkcs7DataTest, pkcs7_test025, testing::ext::TestSize.Level1)
{
    Options options;
    char keyStorePwd[] = "123456";
    char keypwd[] = "123456";
    options[Options::KEY_ALIAS] = SIGN_PROFILE_KEY_ALIAS;
    options[Options::MODE] = SIGN_PROFILE_MODE;
    options[Options::PROFILE_CERT_FILE] = SIGN_PROFILE_PROFILE_CERT_FILE;
    options[Options::KEY_STORE_FILE] = SIGN_PROFILE_KEY_STORE_FILE;
    options[Options::OUT_FILE] = SIGN_PROFILE_OUT_FILE;
    options[Options::IN_FILE] = SIGN_PROFILE_IN_FILE;
    options[Options::KEY_RIGHTS] = keypwd;
    options[Options::KEY_STORE_RIGHTS] = keyStorePwd;

    LocalizationAdapter adapter(&options);
    SignToolServiceImpl api;
    bool result = ParamsRunTool::RunSignProfile(&options, api);
    EXPECT_EQ(result, false);
}

/**
 * @tc.name: pkcs7_test026
 * @tc.desc: Test function of HapSignTool::RunSignProfile() failed for no output File.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(Pkcs7DataTest, pkcs7_test026, testing::ext::TestSize.Level1)
{
    Options options;
    char keyStorePwd[] = "123456";
    char keypwd[] = "123456";
    options[Options::KEY_ALIAS] = SIGN_PROFILE_KEY_ALIAS;
    options[Options::MODE] = SIGN_PROFILE_MODE;
    options[Options::PROFILE_CERT_FILE] = SIGN_PROFILE_PROFILE_CERT_FILE;
    options[Options::SIGN_ALG] = SIGN_PROFILE_SIGN_ALG;
    options[Options::KEY_STORE_FILE] = SIGN_PROFILE_KEY_STORE_FILE;
    options[Options::IN_FILE] = SIGN_PROFILE_IN_FILE;
    options[Options::KEY_RIGHTS] = keypwd;
    options[Options::KEY_STORE_RIGHTS] = keyStorePwd;

    LocalizationAdapter adapter(&options);
    SignToolServiceImpl api;
    bool result = ParamsRunTool::RunSignProfile(&options, api);
    EXPECT_EQ(result, false);
}

/**
 * @tc.name: pkcs7_test027
 * @tc.desc: Test function of HapSignTool::RunSignProfile() failed for no inFile.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(Pkcs7DataTest, pkcs7_test027, testing::ext::TestSize.Level1)
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
    options[Options::KEY_RIGHTS] = keypwd;
    options[Options::KEY_STORE_RIGHTS] = keyStorePwd;

    LocalizationAdapter adapter(&options);
    SignToolServiceImpl api;
    bool result = ParamsRunTool::RunSignProfile(&options, api);
    EXPECT_EQ(result, false);
}

/**
 * @tc.name: pkcs7_test028
 * @tc.desc: Test function of HapSignTool::RunSignProfile() failed for invalid mode.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(Pkcs7DataTest, pkcs7_test028, testing::ext::TestSize.Level1)
{
    Options options;
    char keyStorePwd[] = "123456";
    char keypwd[] = "123456";
    options[Options::KEY_ALIAS] = SIGN_PROFILE_KEY_ALIAS;
    options[Options::MODE] = "LocalSign";
    options[Options::PROFILE_CERT_FILE] = SIGN_PROFILE_PROFILE_CERT_FILE;
    options[Options::SIGN_ALG] = SIGN_PROFILE_SIGN_ALG;
    options[Options::KEY_STORE_FILE] = SIGN_PROFILE_KEY_STORE_FILE;
    options[Options::OUT_FILE] = SIGN_PROFILE_OUT_FILE;
    options[Options::IN_FILE] = SIGN_PROFILE_IN_FILE;
    options[Options::KEY_RIGHTS] = keypwd;
    options[Options::KEY_STORE_RIGHTS] = keyStorePwd;

    LocalizationAdapter adapter(&options);
    SignToolServiceImpl api;
    bool result = ParamsRunTool::RunSignProfile(&options, api);
    EXPECT_EQ(result, false);
}

/**
 * @tc.name: pkcs7_test029
 * @tc.desc: Test function of HapSignTool::RunSignProfile() failed for no mode param.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(Pkcs7DataTest, pkcs7_test029, testing::ext::TestSize.Level1)
{
    Options options;
    char keyStorePwd[] = "123456";
    char keypwd[] = "123456";
    options[Options::MODE] = SIGN_PROFILE_MODE;
    options[Options::PROFILE_CERT_FILE] = SIGN_PROFILE_PROFILE_CERT_FILE;
    options[Options::SIGN_ALG] = SIGN_PROFILE_SIGN_ALG;
    options[Options::KEY_STORE_FILE] = SIGN_PROFILE_KEY_STORE_FILE;
    options[Options::OUT_FILE] = SIGN_PROFILE_OUT_FILE;
    options[Options::IN_FILE] = SIGN_PROFILE_IN_FILE;
    options[Options::KEY_RIGHTS] = keypwd;
    options[Options::KEY_STORE_RIGHTS] = keyStorePwd;

    LocalizationAdapter adapter(&options);
    SignToolServiceImpl api;
    bool result = ParamsRunTool::RunSignProfile(&options, api);
    EXPECT_EQ(result, false);
}

/**
 * @tc.name: pkcs7_test030
 * @tc.desc: Test function of HapSignTool::RunSignProfile() failed for no profile cert file.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(Pkcs7DataTest, pkcs7_test030, testing::ext::TestSize.Level1)
{
    Options options;
    char keyStorePwd[] = "123456";
    char keypwd[] = "123456";
    options[Options::KEY_ALIAS] = SIGN_PROFILE_KEY_ALIAS;
    options[Options::MODE] = SIGN_PROFILE_MODE;
    options[Options::SIGN_ALG] = SIGN_PROFILE_SIGN_ALG;
    options[Options::KEY_STORE_FILE] = SIGN_PROFILE_KEY_STORE_FILE;
    options[Options::OUT_FILE] = SIGN_PROFILE_OUT_FILE;
    options[Options::IN_FILE] = SIGN_PROFILE_IN_FILE;
    options[Options::KEY_RIGHTS] = keypwd;
    options[Options::KEY_STORE_RIGHTS] = keyStorePwd;

    LocalizationAdapter adapter(&options);
    SignToolServiceImpl api;
    bool result = ParamsRunTool::RunSignProfile(&options, api);
    EXPECT_EQ(result, false);
}

/**
 * @tc.name: pkcs7_test031
 * @tc.desc: Test function of HapSignTool::RunSignProfile() failed for no keystore file.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(Pkcs7DataTest, pkcs7_test031, testing::ext::TestSize.Level1)
{
    Options options;
    char keyStorePwd[] = "123456";
    char keypwd[] = "123456";
    options[Options::KEY_ALIAS] = SIGN_PROFILE_KEY_ALIAS;
    options[Options::MODE] = SIGN_PROFILE_MODE;
    options[Options::PROFILE_CERT_FILE] = SIGN_PROFILE_PROFILE_CERT_FILE;
    options[Options::SIGN_ALG] = SIGN_PROFILE_SIGN_ALG;
    options[Options::OUT_FILE] = SIGN_PROFILE_OUT_FILE;
    options[Options::IN_FILE] = SIGN_PROFILE_IN_FILE;
    options[Options::KEY_RIGHTS] = keypwd;
    options[Options::KEY_STORE_RIGHTS] = keyStorePwd;

    LocalizationAdapter adapter(&options);
    SignToolServiceImpl api;
    bool result = ParamsRunTool::RunSignProfile(&options, api);
    EXPECT_EQ(result, false);
}

/**
 * @tc.name: pkcs7_test032
 * @tc.desc: Test function of SignToolServiceImpl::GetProvisionContent() failed for invalid json.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(Pkcs7DataTest, pkcs7_test032, testing::ext::TestSize.Level1)
{
    std::string ret;
    const std::string input = "invalid.txt";
    SignToolServiceImpl::GetProvisionContent(input, ret);
    EXPECT_TRUE(ret.empty());
}

/**
 * @tc.name: pkcs7_test033
 * @tc.desc: Test function of VerifyHapOpensslUtils::ParsePkcs7Package() failed for invalid packageLen .
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(Pkcs7DataTest, pkcs7_test033, testing::ext::TestSize.Level1)
{
    unsigned char packageData[] = "hello,world";
    uint32_t packageLen = 0;
    Pkcs7Context ctx;
    bool result = VerifyHapOpensslUtils::ParsePkcs7Package(packageData, packageLen, ctx);
    EXPECT_EQ(result, false);
}

/**
 * @tc.name: pkcs7_test034
 * @tc.desc: Test function of SignToolServiceImpl::SignProfile() failed for empty json.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(Pkcs7DataTest, pkcs7_test034, testing::ext::TestSize.Level1)
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
    options[Options::IN_FILE] = std::string("test.bin");
    options[Options::KEY_RIGHTS] = keypwd;
    options[Options::KEY_STORE_RIGHTS] = keyStorePwd;

    LocalizationAdapter adapter(&options);
    SignToolServiceImpl api;
    bool result = api.SignProfile(&options);
    EXPECT_EQ(result, false);
}

/**
 * @tc.name: pkcs7_test035
 * @tc.desc: Test function of SignToolServiceImpl::SignProfile() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(Pkcs7DataTest, pkcs7_test035, testing::ext::TestSize.Level1)
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
    SignToolServiceImpl api;
    bool result = api.SignProfile(&options);
    EXPECT_EQ(result, true);
}

/**
 * @tc.name: pkcs7_test036
 * @tc.desc: Test function of SignToolServiceImpl::SignProfile() failed for invalid certchain.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(Pkcs7DataTest, pkcs7_test036, testing::ext::TestSize.Level1)
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
    SignToolServiceImpl api;
    bool result = api.SignProfile(&options);
    EXPECT_EQ(result, false);
}

/**
* @tc.name: pkcs7_test037
* @tc.desc: Test function of SignToolServiceImpl::VerifyProfile() failed for invalid pkcs7.
* @tc.size: MEDIUM
* @tc.type: FUNC
* @tc.level Level 1
* @tc.require: SR000H63TL
*/
HWTEST_F(Pkcs7DataTest, pkcs7_test037, testing::ext::TestSize.Level1)
{
    Options options;
    options[Options::IN_FILE] = SIGN_PROFILE_CERT_PEM;
    options[Options::OUT_FILE] = VERIFY_PROFILE_OUT_FILE;
    bool result = false;
    SignToolServiceImpl api;
    result = api.VerifyProfile(&options);
    EXPECT_EQ(result, false);
}

/**
* @tc.name: pkcs7_test038
* @tc.desc: Test function of SignToolServiceImpl::VerifyProfile() interface for SUCCESS json写入OUT_FILE.
* @tc.size: MEDIUM
* @tc.type: FUNC
* @tc.level Level 1
* @tc.require: SR000H63TL
*/
HWTEST_F(Pkcs7DataTest, pkcs7_test038, testing::ext::TestSize.Level1)
{
    Options options;
    options[Options::IN_FILE] = VERIFY_PROFILE_IN_FILE;
    bool result = false;
    SignToolServiceImpl api;
    result = api.VerifyProfile(&options);
    EXPECT_EQ(result, true);
}

/**
* @tc.name: pkcs7_test039
* @tc.desc: Test function of SignToolServiceImpl::VerifyProfile() failed for invalid pkcs7.
* @tc.size: MEDIUM
* @tc.type: FUNC
* @tc.level Level 1
* @tc.require: SR000H63TL
*/
HWTEST_F(Pkcs7DataTest, pkcs7_test039, testing::ext::TestSize.Level1)
{
    Options options;
    options[Options::IN_FILE] = std::string("text.bin");
    bool result = false;
    SignToolServiceImpl api;
    result = api.VerifyProfile(&options);
    EXPECT_EQ(result, false);
}

}
}