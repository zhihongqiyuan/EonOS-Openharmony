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

#include "hap_cert_verify_openssl_utils_test.h"

#include <fstream>
#include <gtest/gtest.h>

#include "openssl/asn1.h"
#include "openssl/x509.h"
#include "verify_hap.h"
#include "verify_cert_openssl_utils.h"

using namespace testing::ext;
namespace OHOS {
namespace SignatureTools {
class VerifyCertOpensslUtilsTest : public testing::Test {
public:
    static void SetUpTestCase(void);

    static void TearDownTestCase(void);

    void SetUp();

    void TearDown();
};

void VerifyCertOpensslUtilsTest::SetUpTestCase(void)
{
}

void VerifyCertOpensslUtilsTest::TearDownTestCase(void)
{
}
void VerifyCertOpensslUtilsTest::SetUp()
{
}

void VerifyCertOpensslUtilsTest::TearDown()
{
}

/**
 * @tc.name: Test GetSubjectFromX509 and GetIssuerFromX509 functions.
 * @tc.desc: The static function will return false due to invalid input;
 * @tc.type: FUNC
 */
HWTEST_F(VerifyCertOpensslUtilsTest, GetIssuerAndSubjectTest001, TestSize.Level1)
{
    /*
        * @tc.steps: step1. Use nullptr as input to test GetSubjectFromX509.
        * @tc.expected: step1. The return is false.
        */
    std::string subject;
    ASSERT_FALSE(VerifyCertOpensslUtils::GetSubjectFromX509(nullptr, subject));
    /*
        * @tc.steps: step2. Use nullptr as input to test GetIssuerFromX509.
        * @tc.expected: step2. The return is false.
        */
    std::string issuer;
    ASSERT_FALSE(VerifyCertOpensslUtils::GetIssuerFromX509(nullptr, issuer));
}

/**
 * @tc.name: Test VerifyCertChainPeriodOfValidity function.
 * @tc.desc: Verify whether the VerifyCertChainPeriodOfValidity function can verify validity
 *           period of a certificate chain.
 * @tc.type: FUNC
 */
HWTEST_F(VerifyCertOpensslUtilsTest, VerifyCertChainPeriodOfValidityTest001, TestSize.Level1)
{
    /*
        * @tc.steps: step1. Input an empty certChain.
        * @tc.expected: step1. The return is false.
        */
    CertChain certsChain;
    ASSERT_FALSE(VerifyCertOpensslUtils::VerifyCertChainPeriodOfValidity(certsChain, nullptr));
    /*
        * @tc.steps: step3. Input a signTime which out of period of validity.
        * @tc.expected: step3. The return is false.
        */
    ASN1_OCTET_STRING* asnString = ASN1_OCTET_STRING_new();
    ASSERT_TRUE(asnString != nullptr);
    ASSERT_TRUE(ASN1_OCTET_STRING_set(asnString, reinterpret_cast<const unsigned char*>(TEST_ANS_TIME.c_str()),
                static_cast<int>(TEST_ANS_TIME.size())));
    ASN1_TYPE* time = ASN1_TYPE_new();
    ASSERT_TRUE(time != nullptr);
    ASN1_TYPE_set(time, V_ASN1_UTCTIME, asnString);
    ASSERT_FALSE(VerifyCertOpensslUtils::VerifyCertChainPeriodOfValidity(certsChain, time));
    /*
        * @tc.steps: step4. Input a certChain with two nullptr.
        * @tc.expected: step4. The return iis false.
        */
    certsChain.clear();
    certsChain.push_back(nullptr);
    certsChain.push_back(nullptr);
    ASSERT_FALSE(VerifyCertOpensslUtils::VerifyCertChainPeriodOfValidity(certsChain, time));
    ASN1_TYPE_free(time);
}

/**
 * @tc.name: Test VerifyCrl function.
 * @tc.desc: Verify whether the VerifyCrl function can verify a crl.
 * @tc.type: FUNC
 */
HWTEST_F(VerifyCertOpensslUtilsTest, VerifyCrlTest001, TestSize.Level1)
{
    /*
        * @tc.steps: step1. Input an empty certChain.
        * @tc.expected: step1. The return is false.
        */
    CertChain certsChain;
    Pkcs7Context pkcs7Context;
    VerifyCertOpensslUtils::VerifyCrl(certsChain, nullptr, pkcs7Context);
    /*
        * @tc.steps: step2. Input a certChain with two nullptr.
        * @tc.expected: step2. the return is false.
        */
    STACK_OF(X509_CRL)* crls = sk_X509_CRL_new_null();
    certsChain.push_back(nullptr);
    certsChain.push_back(nullptr);
    VerifyCertOpensslUtils::VerifyCrl(certsChain, crls, pkcs7Context);
    /*
        * @tc.steps: step5. Input right certChain and crls.
        * @tc.expected: step5. The return is true.
        */
    certsChain.clear();
    ASSERT_FALSE(VerifyCertOpensslUtils::VerifyCrl(certsChain, crls, pkcs7Context));
    sk_X509_CRL_pop_free(crls, X509_CRL_free);
}

/**
 * @tc.name: Test private function
 * @tc.desc: The static function will return result of invalid input;
 * @tc.type: FUNC
 */
HWTEST_F(VerifyCertOpensslUtilsTest, PrivateFuncInvalidInputTest001, TestSize.Level1)
{
    /*
        * @tc.steps: step1. Use invalid input.
        * @tc.expected: step1. The return is false.
        */
    CertSign certVisitSign;
    VerifyCertOpensslUtils::GenerateCertSignFromCertStack(nullptr, certVisitSign);
    ASSERT_TRUE(VerifyCertOpensslUtils::FindCertOfIssuer(nullptr, certVisitSign) == nullptr);
    std::string str = VerifyCertOpensslUtils::GetDnToString(nullptr);
    ASSERT_FALSE(str.size() > 0);
    ASSERT_TRUE(VerifyCertOpensslUtils::GetCrlBySignedCertIssuer(nullptr, nullptr) == nullptr);
    ASSERT_FALSE(VerifyCertOpensslUtils::X509NameCompare(nullptr, nullptr));
    ASSERT_FALSE(VerifyCertOpensslUtils::CheckSignTimeInValidPeriod(nullptr, nullptr, nullptr));
    ASSERT_FALSE(VerifyCertOpensslUtils::CheckAsn1TimeIsValid(nullptr));
    ASSERT_FALSE(VerifyCertOpensslUtils::CheckAsn1TypeIsValid(nullptr));
}
} // namespace SignatureTools
} // namespace OHOS
