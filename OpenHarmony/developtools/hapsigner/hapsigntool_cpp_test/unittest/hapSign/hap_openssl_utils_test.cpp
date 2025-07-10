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
#include "hap_openssl_utils_test.h"
#include "params_run_tool.h"
#include "sign_hap.h"
#include "sign_provider.h"
#include "sign_tool_service_impl.h"
namespace OHOS {
namespace SignatureTools {
/*
 * @tc.name: hap_openssl_utils_test_001
 * @tc.desc: This function tests failed for interface DigestInit due to parameter DigestParameter not init
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapOpensslUtilsTest, hap_openssl_utils_test_001, testing::ext::TestSize.Level1)
{
    SIGNATURE_TOOLS_LOGI("hello world !!!");
    DigestParameter parameter;

    bool ret = DigestCommon::DigestInit(parameter);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hap_openssl_utils_test_002
 * @tc.desc: This function tests failed for interface DigestInit due to parameter DigestParameter not init completed
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapOpensslUtilsTest, hap_openssl_utils_test_002, testing::ext::TestSize.Level1)
{
    SIGNATURE_TOOLS_LOGI("hello world !!!");
    DigestParameter parameter;
    parameter.md = EVP_sha256();
    bool ret = DigestCommon::DigestInit(parameter);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hap_openssl_utils_test_003
 * @tc.desc: This function tests failed for interface DigestUpdate due to parameter content and len mismatch
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapOpensslUtilsTest, hap_openssl_utils_test_003, testing::ext::TestSize.Level1)
{
    DigestParameter parameter;
    parameter.md = EVP_sha256();
    const unsigned char content[] = "123";
    int32_t len = 5;
    bool ret = DigestCommon::DigestUpdate(parameter, content, len);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hap_openssl_utils_test_004
 * @tc.desc: This function tests failed for interface DigestUpdate due to parameter content and len mismatch
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapOpensslUtilsTest, hap_openssl_utils_test_004, testing::ext::TestSize.Level1)
{
    DigestParameter parameter;
    parameter.md = EVP_sha256();
    int32_t len = 5;
    bool ret = DigestCommon::DigestUpdate(parameter, nullptr, len);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hap_openssl_utils_test_005
 * @tc.desc: This function tests success for interface GetDigest
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapOpensslUtilsTest, hap_openssl_utils_test_005, testing::ext::TestSize.Level1)
{
    DigestParameter parameter;
    unsigned char dig[EVP_MAX_MD_SIZE];
    int32_t ret = DigestCommon::GetDigest(parameter, dig);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.name: hap_openssl_utils_test_006
 * @tc.desc: This function tests success for interface GetDigest
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapOpensslUtilsTest, hap_openssl_utils_test_006, testing::ext::TestSize.Level1)
{
    DigestParameter parameter;
    parameter.md = EVP_sha256();
    ByteBuffer chunk;
    std::vector<OptionalBlock> optionalBlocks;
    unsigned char out[EVP_MAX_MD_SIZE];
    int32_t ret = DigestCommon::GetDigest(chunk, optionalBlocks, parameter, out);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.name: hap_openssl_utils_test_007
 * @tc.desc: This function tests success for interface GetDigest
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapOpensslUtilsTest, hap_openssl_utils_test_007, testing::ext::TestSize.Level1)
{
    DigestParameter parameter;
    DigestParameter parameter_test(parameter);
    parameter_test = parameter;
    DigestParameter* parameter_ptr = &parameter;
    *parameter_ptr = parameter;
    ByteBuffer chunk;
    std::vector<OptionalBlock> optionalBlocks;
    unsigned char out[EVP_MAX_MD_SIZE];
    int32_t ret = DigestCommon::GetDigest(chunk, optionalBlocks, parameter, out);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.name: hap_openssl_utils_test_008
 * @tc.desc: This function tests success for interface GetDigestAlgorithmId
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapOpensslUtilsTest, hap_openssl_utils_test_008, testing::ext::TestSize.Level1)
{
    int32_t digId = DigestCommon::GetDigestAlgorithmId(ALGORITHM_SHA384_WITH_ECDSA);
    EXPECT_EQ(digId, NID_sha384);
    digId = DigestCommon::GetDigestAlgorithmId(ALGORITHM_SHA512_WITH_ECDSA);
    EXPECT_EQ(digId, NID_sha512);
}

/*
 * @tc.name: hap_openssl_utils_test_009
 * @tc.desc: This function tests success for interface VerifyPkcs7 due to Pkcs7Context not init
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapOpensslUtilsTest, hap_openssl_utils_test_009, testing::ext::TestSize.Level1)
{
    Pkcs7Context pkcs7Context;
    bool ret = VerifyHapOpensslUtils::VerifyPkcs7(pkcs7Context);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hap_openssl_utils_test_010
 * @tc.desc: This function tests success for interface VerifyPkcs7 due to Pkcs7Context not init complete
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapOpensslUtilsTest, hap_openssl_utils_test_010, testing::ext::TestSize.Level1)
{
    Pkcs7Context pkcs7Context;
    pkcs7Context.p7 = PKCS7_new();
    PKCS7_set_type(pkcs7Context.p7, NID_pkcs7_signed);
    PKCS7_content_new(pkcs7Context.p7, NID_pkcs7_data);
    bool ret = VerifyHapOpensslUtils::VerifyPkcs7(pkcs7Context);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hap_openssl_utils_test_011
 * @tc.desc: This function tests success for interface VerifyPkcs7 due to Pkcs7Context not init complete
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapOpensslUtilsTest, hap_openssl_utils_test_011, testing::ext::TestSize.Level1)
{
    Pkcs7Context pkcs7Context;
    pkcs7Context.p7 = PKCS7_new();
    PKCS7_set_type(pkcs7Context.p7, NID_pkcs7_data);
    bool ret = VerifyHapOpensslUtils::VerifyPkcs7(pkcs7Context);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hap_openssl_utils_test_012
 * @tc.desc: This function tests success for interface VerifyPkcs7 due to Pkcs7Context not init complete
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapOpensslUtilsTest, hap_openssl_utils_test_012, testing::ext::TestSize.Level1)
{
    Pkcs7Context pkcs7Context;
    pkcs7Context.p7 = PKCS7_new();
    PKCS7_set_type(pkcs7Context.p7, NID_pkcs7_signed);
    bool ret = VerifyHapOpensslUtils::VerifyPkcs7(pkcs7Context);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hap_openssl_utils_test_013
 * @tc.desc: This function tests success for interface GetCertChains due to Pkcs7Context not init
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapOpensslUtilsTest, hap_openssl_utils_test_013, testing::ext::TestSize.Level1)
{
    PKCS7* p7 = nullptr;
    Pkcs7Context pkcs7Context;
    bool ret = VerifyHapOpensslUtils::GetCertChains(p7, pkcs7Context);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hap_openssl_utils_test_014
 * @tc.desc: This function tests success for interface GetCertChains due to Pkcs7Context not init complete
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapOpensslUtilsTest, hap_openssl_utils_test_014, testing::ext::TestSize.Level1)
{
    Pkcs7Context pkcs7Context;
    pkcs7Context.p7 = PKCS7_new();
    PKCS7_set_type(pkcs7Context.p7, NID_pkcs7_signed);
    PKCS7_content_new(pkcs7Context.p7, NID_pkcs7_data);
    PKCS7* pkcsBak = pkcs7Context.p7;
    bool ret = VerifyHapOpensslUtils::GetCertChains(pkcsBak, pkcs7Context);
    EXPECT_EQ(ret, false);
}
} // namespace SignatureTools
} // namespace OHOS