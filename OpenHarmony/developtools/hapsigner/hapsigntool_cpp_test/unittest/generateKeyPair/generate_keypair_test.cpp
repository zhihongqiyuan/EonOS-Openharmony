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

#include <gtest/gtest.h>
#include "signature_tools_log.h"
#include "options.h"
#include "sign_tool_service_impl.h"
#include "localization_adapter.h"
#include "openssl/ssl.h"
#include "openssl/pem.h"
#include "openssl/err.h"
#include "p12_local.h"
#include "cmd_util.h"
#include "file_utils.h"
#include "params_run_tool.h"
#include "constant.h"
#include "params.h"
#include "params_trust_list.h"
#include "param_constants.h"

namespace OHOS {
namespace SignatureTools {

class GenerateKeyPairTest : public testing::Test {
public:
    static void SetUpTestCase()
    {
    };
    static void TearDownTestCase()
    {
    };
    void SetUp()
    {
    };
    void TearDown()
    {
    };
};

/*
 * @tc.name: generate_keypair_test_001
 * @tc.desc: Generate a key pair and load it into the keystore.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GenerateKeyPairTest, generate_keypair_test_001, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string keyAlias = "oh-app1-key-v1";
    char keyPwd[] = "123456";
    std::string keyAlg = "ECC";
    int keySize = 256;
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    char keystorePwd[] = "123456";

    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;

    bool ret = api->GenerateKeyStore(params.get());
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: generate_keypair_test_002
 * @tc.desc: If you search for a key pair in the keystore using an alias,
 * @the key pair pointer is returned on success, and NULL is returned on failure.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GenerateKeyPairTest, generate_keypair_test_002, testing::ext::TestSize.Level1)
{
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string keyAlias = "oh-app1-key-v1";
    char keyPwd[] = "123456";
    std::string keyAlg = "ECC";
    int keySize = 256;
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    char keystorePwd[] = "123456";

    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;

    std::unique_ptr<LocalizationAdapter> adaptePtr = std::make_unique<LocalizationAdapter>(params.get());

    int status = adaptePtr->IsAliasExist(adaptePtr->options->GetString(Options::KEY_ALIAS));
    EXPECT_EQ(status, 0);
}

/*
 * @tc.name: generate_keypair_test_003
 * @tc.desc: Generate a key pair and load it into the keystore.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GenerateKeyPairTest, generate_keypair_test_003, testing::ext::TestSize.Level1)
{
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "oh-app1-key-v1";
    char keyPwd[] = "123456";
    std::string keyAlg = "ECC";
    int keySize = 256;
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    char keystorePwd[] = "123456";
    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;
    std::unique_ptr<LocalizationAdapter> adaptePtr = std::make_unique<LocalizationAdapter>(params.get());
    EVP_PKEY* keyPair = nullptr;
    keyPair = adaptePtr->GetAliasKey(true);
    EXPECT_NE(keyPair, nullptr);
}

/*
 * @tc.name: generate_keypair_test_004
 * @tc.desc: Generate key pair.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GenerateKeyPairTest, generate_keypair_test_004, testing::ext::TestSize.Level1)
{
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string keyAlg = "ECC";
    int keySize = 256;

    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;

    std::unique_ptr<LocalizationAdapter> adaptePtr = std::make_unique<LocalizationAdapter>(params.get());

    EVP_PKEY* keyPair = nullptr;
    std::string keyStorePath = adaptePtr->options->GetString(Options::KEY_STORE_FILE);
    keyPair = adaptePtr->keyStoreHelper->GenerateKeyPair(adaptePtr->options->GetString(Options::KEY_ALG),
                                                         adaptePtr->options->GetInt(Options::KEY_SIZE));
    EXPECT_NE(keyPair, nullptr);
}

/*
 * @tc.name: generate_keypair_test_005
 * @tc.desc: Load the key pair into the file.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GenerateKeyPairTest, generate_keypair_test_005, testing::ext::TestSize.Level1)
{
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string keyAlias = "oh-app1-key-v1";
    char keyPwd[] = "123456";
    std::string keyAlg = "ECC";
    int keySize = 256;
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    char keystorePwd[] = "123456";

    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;

    std::unique_ptr<LocalizationAdapter> adaptePtr = std::make_unique<LocalizationAdapter>(params.get());

    EVP_PKEY* keyPair = nullptr;
    std::string keyStorePath = adaptePtr->options->GetString(Options::KEY_STORE_FILE);
    keyPair = adaptePtr->keyStoreHelper->GenerateKeyPair(adaptePtr->options->GetString(Options::KEY_ALG),
                                                         adaptePtr->options->GetInt(Options::KEY_SIZE));
    int ret = adaptePtr->keyStoreHelper->WriteKeyStore(keyPair, keyStorePath,
                                                       adaptePtr->options->GetChars(Options::KEY_STORE_RIGHTS),
                                                       adaptePtr->options->GetString(Options::KEY_ALIAS),
                                                       adaptePtr->options->GetChars(Options::KEY_RIGHTS));
    EXPECT_NE(ret, RET_FAILED);
}

/*
 * @tc.name: generate_keypair_test_006
 * @tc.desc: Read the key pair from the file by alias.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GenerateKeyPairTest, generate_keypair_test_006, testing::ext::TestSize.Level1)
{
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string keyAlias = "oh-app1-key-v1";
    char keyPwd[] = "123456";
    std::string keyAlg = "ECC";
    int keySize = 256;
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    char keystorePwd[] = "123456";

    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;

    std::unique_ptr<LocalizationAdapter> adaptePtr = std::make_unique<LocalizationAdapter>(params.get());

    EVP_PKEY* keyPair = nullptr;
    std::string keyStorePath = adaptePtr->options->GetString(Options::KEY_STORE_FILE);
    adaptePtr->keyStoreHelper->ReadKeyStore(keyStorePath, adaptePtr->options->GetChars(Options::KEY_STORE_RIGHTS),
                                            adaptePtr->options->GetString(Options::KEY_ALIAS),
                                            adaptePtr->options->GetChars(Options::KEY_RIGHTS), &keyPair);
    EXPECT_NE(keyPair, nullptr);
}

/*
 * @tc.name: generate_keypair_test_007
 * @tc.desc: reset passwords.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GenerateKeyPairTest, generate_keypair_test_007, testing::ext::TestSize.Level1)
{
    std::shared_ptr<Options> params = std::make_shared<Options>();

    char keyPwd[] = "123456";
    char keystorePwd[] = "123456";
    char issuerKeyPwd[] = "123456";
    char issuerkeystorePwd[] = "123456";

    (*params)["keyPwd"] = keyPwd;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["issuerKeyPwd"] = issuerKeyPwd;
    (*params)["issuerKeystorePwd"] = issuerkeystorePwd;

    std::unique_ptr<LocalizationAdapter> adaptePtr = std::make_unique<LocalizationAdapter>(params.get());

    std::string str = "";
    adaptePtr->ResetPwd();
    EXPECT_EQ(adaptePtr->options->GetChars(Options::KEY_RIGHTS), str);
    EXPECT_EQ(adaptePtr->options->GetChars(Options::KEY_STORE_RIGHTS), str);
    EXPECT_EQ(adaptePtr->options->GetChars(Options::ISSUER_KEY_RIGHTS), str);
    EXPECT_EQ(adaptePtr->options->GetChars(Options::ISSUER_KEY_STORE_RIGHTS), str);
}

/*
 * @tc.name: generate_keypair_test_008
 * @tc.desc: Generate a key pair and load it into the keystore.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GenerateKeyPairTest, generate_keypair_test_008, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string keyAlias = "aaa";
    char keyPwd[] = "123456";
    std::string keyAlg = "ECC";
    int keySize = 385;
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    char keystorePwd[] = "123456";

    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;

    bool ret = api->GenerateKeyStore(params.get());
    EXPECT_EQ(ret, false);
}

/*
* @tc.name: generate_keypair_test_009
* @tc.desc: Generate a key pair and load it into the keystore.
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(GenerateKeyPairTest, generate_keypair_test_009, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string keyAlias = "oh-app1-key-v1";
    char keyPwd[] = "123456909";
    std::string keyAlg = "ECC";
    int keySize = 385;
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    char keystorePwd[] = "123456";

    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;

    bool ret = api->GenerateKeyStore(params.get());
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: generate_keypair_test_010
 * @tc.desc: Write the keypair to the keystore file.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GenerateKeyPairTest, generate_keypair_test_010, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    KeyStoreHelper keystorehelper;

    std::string keyAlias = "oh-app1-key-v1";
    char keyPwd[] = "123456";
    std::string keyAlg = "ECC";
    int keySize = 256;
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    char keystorePwd[] = "1234563333";

    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;

    EVP_PKEY* keyPair = keystorehelper.GenerateKeyPair(keyAlg, keySize);

    int ret = keystorehelper.WriteKeyStore(keyPair, keystoreFile, keystorePwd, keyAlias, keyPwd);
    EXPECT_EQ(ret, RET_FAILED);
}

/*
 * @tc.name: generate_keypair_test_011
 * @tc.desc: The key pair is read from the P12 file by alias.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GenerateKeyPairTest, generate_keypair_test_011, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    KeyStoreHelper keystorehelper;

    std::string keyAlias = "oh-app1-key-v1";
    char keyPwd[] = "123456";
    std::string keyAlg = "ECC";
    int keySize = 256;
    std::string keystoreFile = "./generateKeyPair/OpenHarmony99999.p12";
    char keystorePwd[] = "123456";

    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;

    EVP_PKEY* keyPair = nullptr;

    int ret = keystorehelper.ReadKeyStore(keystoreFile, keystorePwd, keyAlias, keyPwd, &keyPair);
    EXPECT_EQ(ret, RET_FAILED);
}

/*
* @tc.name: generate_keypair_test_012
* @tc.desc: Generate a key pair and load it into the keystore.
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(GenerateKeyPairTest, generate_keypair_test_012, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string keyAlias = "oh-app1-key-v1";
    std::string keyAlg = "ECC";
    int keySize = 256;
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";

    (*params)["keyAlias"] = keyAlias;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["keystoreFile"] = keystoreFile;

    bool ret = api->GenerateKeyStore(params.get());
    EXPECT_EQ(ret, false);
}

/*
* @tc.name: generate_keypair_test_013
* @tc.desc: Generate a key pair and load it into the keystore.
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(GenerateKeyPairTest, generate_keypair_test_013, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string keyAlias = "oh-app1-key-v1";
    char keyPwd[] = "";
    std::string keyAlg = "ECC";
    int keySize = 256;
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    char keystorePwd[] = "";

    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;

    bool ret = api->GenerateKeyStore(params.get());
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: generate_keypair_test_014
 * @tc.desc: Generate a key pair and load it into the keystore.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GenerateKeyPairTest, generate_keypair_test_014, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string keyAlias = "oh-app1-key-v1";
    std::string keyAlg = "ECC";
    int keySize = 256;
    std::string keystoreFile = "./generateKeyPair/OpenHarmony1.p12";

    (*params)["keyAlias"] = keyAlias;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["keystoreFile"] = keystoreFile;

    bool ret = api->GenerateKeyStore(params.get());
    EXPECT_EQ(ret, true);
}

/*
 * @tc.name: generate_keypair_test_015
 * @tc.desc: Create a p12 keystore.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GenerateKeyPairTest, generate_keypair_test_015, testing::ext::TestSize.Level1)
{
    X509* cert = X509_new();
    PKCS12* p12 = nullptr;
    EVP_PKEY* keypair = nullptr;
    KeyStoreHelper keyhelper;

    std::string keyStorePath = "./generateKeyPair/OpenHarmony.p12";
    char storePwd[] = "123456";
    std::string alias = "oh-app1-key-v1";
    char keyPwd[] = "123456";

    keypair = keyhelper.GenerateKeyPair("ECC", 256);
    keyhelper.InitX509(*cert, *keypair);

    keyhelper.CreatePKCS12(&p12, keyStorePath.c_str(), storePwd, keyPwd, alias.c_str(), keypair, cert);
    int ret = keyhelper.FindKeyPair(p12, "", nullptr, nullptr, nullptr, "");
    EXPECT_EQ(ret, RET_FAILED);
}

/*
 * @tc.name: generate_keypair_test_016
 * @tc.desc: Generate a key pair and load it into the keystore.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GenerateKeyPairTest, generate_keypair_test_016, testing::ext::TestSize.Level1)
{
    X509* cert = X509_new();
    PKCS12* p12 = nullptr;
    EVP_PKEY* keypair = nullptr;
    KeyStoreHelper keyhelper;

    std::string keyStorePath = "./generateKeyPair/OpenHarmony11.p12";
    char storePwd[] = "123456";
    std::string alias = "oh-app1-key-v1";
    char keyPwd[] = "123456";

    keypair = keyhelper.GenerateKeyPair("ECC", 256);
    int ret = keyhelper.CreatePKCS12(&p12, keyStorePath.c_str(), storePwd, keyPwd, alias.c_str(), keypair, cert);
    EXPECT_EQ(ret, RET_FAILED);
}

/*
 * @tc.name: generate_keypair_test_017
 * @tc.desc: Keystore password parsing.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GenerateKeyPairTest, generate_keypair_test_017, testing::ext::TestSize.Level1)
{
    KeyStoreHelper keyhelper;
    int ret = keyhelper.Pkcs12PasswordParse(nullptr, nullptr, "");
    EXPECT_EQ(ret, RET_FAILED);
}

/*
 * @tc.name: generate_keypair_test_018
 * @tc.desc: Generate a key pair and load it into the keystore.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GenerateKeyPairTest, generate_keypair_test_018, testing::ext::TestSize.Level1)
{
    X509* cert = X509_new();
    PKCS12* p12 = nullptr;
    EVP_PKEY* keypair = nullptr;
    KeyStoreHelper keyhelper;

    std::string keyStorePath = "./generateKeyPair/OpenHarmonyDamage.p12";
    char storePwd[] = "123456";
    std::string alias = "oh-app1-key-v1";
    char keyPwd[] = "123456";

    keypair = keyhelper.GenerateKeyPair("ECC", 256);
    keyhelper.InitX509(*cert, *keypair);
    int ret = keyhelper.CreatePKCS12(&p12, keyStorePath.c_str(), storePwd, keyPwd, alias.c_str(), keypair, cert);
    EXPECT_EQ(ret, RET_FAILED);
}

/*
 * @tc.name: generate_keypair_test_019
 * @tc.desc: Keystore password parsing.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GenerateKeyPairTest, generate_keypair_test_019, testing::ext::TestSize.Level1)
{
    X509* cert = X509_new();
    PKCS12* p12 = nullptr;
    EVP_PKEY* keypair = nullptr;
    KeyStoreHelper keyhelper;

    std::string keyStorePath = "./generateKeyPair/OpenHarmonyNullpwd.p12";
    std::string alias = "oh-app1-key-v1";

    keypair = keyhelper.GenerateKeyPair("ECC", 256);
    keyhelper.InitX509(*cert, *keypair);
    keyhelper.CreatePKCS12(&p12, keyStorePath.c_str(), nullptr, nullptr, alias.c_str(), keypair, cert);
    int ret = keyhelper.Pkcs12PasswordParse(p12, nullptr, "");
    EXPECT_EQ(ret, RET_OK);
}

/*
 * @tc.name: generate_keypair_test_020
 * @tc.desc: Keystore password parsing.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GenerateKeyPairTest, generate_keypair_test_020, testing::ext::TestSize.Level1)
{
    X509* cert = X509_new();
    PKCS12* p12 = nullptr;
    EVP_PKEY* keypair = nullptr;
    KeyStoreHelper keyhelper;

    std::string keyStorePath = "./generateKeyPair/OpenHarmonyNullpwd.p12";
    char storePwd[] = "";
    std::string alias = "oh-app1-key-v1";
    char keyPwd[] = "";

    keypair = keyhelper.GenerateKeyPair("ECC", 256);
    keyhelper.InitX509(*cert, *keypair);
    keyhelper.CreatePKCS12(&p12, keyStorePath.c_str(), storePwd, keyPwd, alias.c_str(), keypair, cert);
    int ret = keyhelper.Pkcs12PasswordParse(p12, nullptr, "");
    EXPECT_EQ(ret, RET_OK);
}

/*
 * @tc.name: generate_keypair_test_021
 * @tc.desc: Generate a key pair.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GenerateKeyPairTest, generate_keypair_test_021, testing::ext::TestSize.Level1)
{
    KeyStoreHelper keyhelper;
    EVP_PKEY* keypair = keyhelper.GenerateKeyPair("", 256);
    EXPECT_EQ(keypair, nullptr);
}

/*
 * @tc.name: generate_keypair_test_022
 * @tc.desc: Create a pkcs12 file.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GenerateKeyPairTest, generate_keypair_test_022, testing::ext::TestSize.Level1)
{
    KeyStoreHelper keyhelper;
    PKCS12* p12 = nullptr;
    p12 = keyhelper.CreatePKCS12(nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, 0, 0, 0, nullptr);
    EXPECT_EQ(p12, nullptr);
}

/*
 * @tc.name: generate_keypair_test_023
 * @tc.desc: Generate a key pair.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GenerateKeyPairTest, generate_keypair_test_023, testing::ext::TestSize.Level1)
{
    KeyStoreHelper keyhelper;
    EVP_PKEY* keypair = keyhelper.GenerateKeyPair("ECC", 0);
    EXPECT_EQ(keypair, nullptr);
}

/*
 * @tc.name: generate_keypair_test_024
 * @tc.desc: Call the SetNidMac() function.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GenerateKeyPairTest, generate_keypair_test_024, testing::ext::TestSize.Level1)
{
    int nidKey = 1;
    int iter = 1;
    int maIter = 1;
    KeyStoreHelper keyhelper;
    keyhelper.SetNidMac(nidKey, iter, maIter);
    EXPECT_NE(nidKey, 1);
}

/*
 * @tc.name: generate_keypair_test_025
 * @tc.desc: Call the SetNidMac() function.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GenerateKeyPairTest, generate_keypair_test_025, testing::ext::TestSize.Level1)
{
    int nidKey = 0;
    int iter = 0;
    int macIter = 0;
    KeyStoreHelper keyhelper;
    keyhelper.SetNidMac(nidKey, iter, macIter);
    EXPECT_EQ(macIter, 146);
}

/*
 * @tc.name: generate_keypair_test_026
 * @tc.desc: Set the public key into the pkcs12 structure.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GenerateKeyPairTest, generate_keypair_test_026, testing::ext::TestSize.Level1)
{
    KeyStoreHelper keyhelper;
    int ret = keyhelper.SetCertPkcs12(nullptr, nullptr, nullptr, nullptr, 0, nullptr, nullptr, 0, 0, nullptr);
    EXPECT_EQ(ret, RET_OK);
}

/*
 * @tc.name: generate_keypair_test_027
 * @tc.desc: Set the public key into the pkcs12 structure.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GenerateKeyPairTest, generate_keypair_test_027, testing::ext::TestSize.Level1)
{
    KeyStoreHelper keyhelper;
    X509* cert = X509_new();
    int ret = keyhelper.SetCertPkcs12(cert, nullptr, nullptr, nullptr, -2, nullptr, nullptr, 0, 0, nullptr);
    EXPECT_EQ(ret, RET_FAILED);
}

/*
 * @tc.name: generate_keypair_test_028
 * @tc.desc: Set the private key into the pkcs12 structure.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GenerateKeyPairTest, generate_keypair_test_028, testing::ext::TestSize.Level1)
{
    KeyStoreHelper keyhelper;
    int ret = keyhelper.SetPkeyPkcs12(nullptr, nullptr, nullptr, nullptr, nullptr, 0, nullptr, 0, 0, nullptr, 0);
    EXPECT_EQ(ret, RET_OK);
}

/*
 * @tc.name: generate_keypair_test_029
 * @tc.desc: Create a p12 file.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GenerateKeyPairTest, generate_keypair_test_029, testing::ext::TestSize.Level1)
{
    X509* cert = X509_new();
    PKCS12* p12 = nullptr;
    EVP_PKEY* keypair = nullptr;
    KeyStoreHelper keyhelper;
    STACK_OF(PKCS7)* safes = nullptr;

    std::string keyStorePath = "./generateKeyPair/OpenHarmony.p12";
    char storePwd1[] = "99999";
    std::string alias = "oh-app1-key-v1";
    char keyPwd[] = "";

    keypair = keyhelper.GenerateKeyPair("ECC", 256);
    keyhelper.InitX509(*cert, *keypair);
    p12 = keyhelper.CreatePKCS12(storePwd1, keyPwd, nullptr, keypair, cert, 0, -2, -2, -1, 0, &safes);
    EXPECT_EQ(p12, nullptr);
}

/*
 * @tc.name: generate_keypair_test_030
 * @tc.desc: Create a p12 file.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GenerateKeyPairTest, generate_keypair_test_030, testing::ext::TestSize.Level1)
{
    X509* cert = X509_new();
    PKCS12* p12 = nullptr;
    EVP_PKEY* keypair = nullptr;
    KeyStoreHelper keyhelper;
    STACK_OF(PKCS7)* safes = nullptr;

    std::string keyStorePath = "./generateKeyPair/OpenHarmony.p12";
    char storePwd1[] = "99999";
    std::string alias = "oh-app1-key-v1";
    char keyPwd[] = "";

    keypair = keyhelper.GenerateKeyPair("ECC", 256);
    keyhelper.InitX509(*cert, *keypair);
    p12 = keyhelper.CreatePKCS12(storePwd1, keyPwd, nullptr, keypair, cert, -2, 0, 0, -1, 0, &safes);
    EXPECT_EQ(p12, nullptr);
}

/*
 * @tc.name: generate_keypair_test_031
 * @tc.desc: Find the key pair by alias.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GenerateKeyPairTest, generate_keypair_test_031, testing::ext::TestSize.Level1)
{
    X509* cert = X509_new();
    PKCS12* p12 = nullptr;
    EVP_PKEY* keypair = nullptr;
    KeyStoreHelper keyhelper;

    std::string keyStorePath = "./generateKeyPair/OpenHarmony.p12";
    char storePwd[] = "123456";
    std::string alias = "oh-app1-key-v1";
    char keyPwd[] = "123456";

    keypair = keyhelper.GenerateKeyPair("ECC", 256);
    keyhelper.InitX509(*cert, *keypair);

    keyhelper.CreatePKCS12(&p12, keyStorePath.c_str(), storePwd, keyPwd, alias.c_str(), keypair, cert);
    p12->authsafes = PKCS7_new();
    int ret = keyhelper.FindKeyPair(p12, "", nullptr, nullptr, nullptr, "");
    EXPECT_EQ(ret, RET_FAILED);
}

/*
 * @tc.name: generate_keypair_test_032
 * @tc.desc: Write the keypair to the keystore file.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GenerateKeyPairTest, generate_keypair_test_032, testing::ext::TestSize.Level1)
{
    KeyStoreHelper keyhelper;

    std::string keyStorePath = "./generateKeyPair/OpenHarmony.p12";
    char storePwd[] = "123456";
    std::string alias = "oh-app1-key-v1";
    char keyPwd[] = "123456";

    int ret = keyhelper.WriteKeyStore(nullptr, keyStorePath, storePwd, alias, keyPwd);
    EXPECT_EQ(ret, RET_FAILED);
}
} // namespace SignatureTools
} // namespace OHOS