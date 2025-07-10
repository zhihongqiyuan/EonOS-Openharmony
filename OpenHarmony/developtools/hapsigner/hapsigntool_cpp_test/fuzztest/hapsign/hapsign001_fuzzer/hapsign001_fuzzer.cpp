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
#include <cstdio>
#include <memory>
#include <unistd.h>

#include "sign_tool_service_impl.h"
#include "options.h"
#include "sign_provider.h"
#include "local_sign_provider.h"
#include "hap_signer_block_utils.h"
#include "remote_sign_provider.h"
#include "verify_hap.h"

namespace OHOS {
namespace SignatureTools {
bool HapSignTest001(const uint8_t* data, size_t size)
{
    if (!data || !size) {
        return true;
    }
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string keyAlias = "oh-app1-key-v1";
    std::string signAlg = "SHA256w";
    std::string signCode = "0";
    std::string appCertFile = "./hapSign/app-release1.pem";
    std::string profileFile = "./hapSign/signed-profile.p7b";
    std::string inFile = "./hapSign/unsigned_with_cd_and_eocd.hap";
    std::string keystoreFile = "./hapSign/ohtest.p12";
    std::string outFile = "./hapSign/phone-default-signed.hap";
    char keyPwd[] = "123456";
    char keystorePwd[] = "123456";

    (*params)["mode"] = mode;
    (*params)["keyAlias"] = keyAlias;
    (*params)["signAlg"] = signAlg;
    (*params)["signCode"] = signCode;
    (*params)["appCertFile"] = appCertFile;
    (*params)["profileFile"] = profileFile;
    (*params)["inFile"] = inFile;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["outFile"] = outFile;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keystorePwd"] = keystorePwd;

    SignerConfig signerConfig;
    signerConfig.SetOptions(params.get());
    signerConfig.GetOptions();

    signerConfig.SetCertificates(nullptr);
    signerConfig.GetCertificates();

    STACK_OF(X509_CRL)* crls = sk_X509_CRL_new_null();
    signerConfig.SetX509CRLs(crls);
    signerConfig.GetX509CRLs();

    signerConfig.GetSignParamMap();
    return true;
}

bool HapSignTest002(const uint8_t* data, size_t size)
{
    if (!data || !size) {
        return true;
    }

    SignatureAlgorithmHelper::FindById(SignatureAlgorithmId::ECDSA_WITH_SHA256);
    SignatureAlgorithmHelper::FindById(SignatureAlgorithmId::ECDSA_WITH_SHA384);
    SignatureAlgorithmHelper::FindById(SignatureAlgorithmId::DSA_WITH_SHA256);

    ContentDigestAlgorithm alg_tmp;
    alg_tmp.GetDigestOutputByteSize();
    DigestParameter digestParam;
    DigestParameter digestParam1;
    DigestParameter digestParam2(digestParam);
    digestParam = digestParam1;
    digestParam2 = digestParam;
    return true;
}

bool HapSignTest003(const uint8_t* data, size_t size)
{
    if (!data || !size) {
        return true;
    }
    SIGNATURE_TOOLS_LOGI("hello world !!!");
    std::unique_ptr<RemoteSignProvider> signProvider = std::make_unique<RemoteSignProvider>();

    X509* cert = X509_new();
    X509* cert1 = X509_new();
    X509_REQ* issuerReq = X509_REQ_new();
    std::string issuername = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    X509_NAME* issName = BuildDN(issuername, issuerReq);
    CertTools::SetCertIssuerName(cert, issName);
    CertTools::SetCertIssuerName(cert1, issName);

    std::string subjectname = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    X509_NAME* subName = BuildDN(subjectname, issuerReq);
    X509_set_subject_name(cert, subName);
    X509_set_subject_name(cert1, subName);
    KeyStoreHelper stroe;
    EVP_PKEY* key1 = stroe.GenerateKeyPair("ECC", 256);
    EVP_PKEY* key2 = stroe.GenerateKeyPair("ECC", 256);

    X509_set_pubkey(cert, key1);
    X509_set_pubkey(cert1, key2);

    signProvider->CheckInputCertMatchWithProfile(cert1, cert);
    X509_free(cert);
    X509_free(cert1);
    X509_REQ_free(issuerReq);
    EVP_PKEY_free(key1);
    EVP_PKEY_free(key2);
    return true;
}
bool HapSignTest004(const uint8_t* data, size_t size)
{
    if (!data || !size) {
        return true;
    }

    std::unique_ptr<SignProvider> signProvider = std::make_unique<LocalSignProvider>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string keyAlias = "oh-app1-key-v1";
    std::string signAlg = "SHA256withECDSA";
    std::string signCode = "1";
    std::string appCertFile = "./hapSign/app-release1.pem";
    std::string profileFile = "./hapSign/signed-profile.p7b";
    std::string profileSigned = "1";
    std::string inFile = "./hapSign/phone-default-unsigned";
    std::string keystoreFile = "./hapSign/ohtest.p12";
    std::string outFile = "./hapSign/phone-default-signed.hap";
    char keyPwd[] = "123456";
    char keystorePwd[] = "123456";

    (*params)["mode"] = mode;
    (*params)["keyAlias"] = keyAlias;
    (*params)["signAlg"] = signAlg;
    (*params)["signCode"] = signCode;
    (*params)["appCertFile"] = appCertFile;
    (*params)["profileFile"] = profileFile;
    (*params)["profileSigned"] = profileSigned;
    (*params)["inFile"] = inFile;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["outFile"] = outFile;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keystorePwd"] = keystorePwd;
    signProvider->CheckInputCertMatchWithProfile(nullptr, nullptr);
    signProvider->GetCrl();
    bool ret = signProvider->Sign(params.get());
    return ret;
}

bool HapSignTest005(const uint8_t* data, size_t size)
{
    if (!data || !size) {
        return true;
    }
    int capacity = 120;
    std::vector<OptionalBlock> option;
    OptionalBlock testblock;
    testblock.optionalType = PROPERTY_BLOB;
    testblock.optionalBlockValue.SetCapacity(capacity);
    option.push_back(testblock);
    return true;
}

bool HapSignTest006(const uint8_t* data, size_t size)
{
    if (!data || !size) {
        return true;
    }
    std::unique_ptr<RemoteSignProvider> signProvider = std::make_unique<RemoteSignProvider>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "remoteSign";
    std::string keyAlias = "oh-app1-key-v1";
    std::string signAlg = "SHA256withECDSA";
    std::string signCode = "1";
    std::string profileFile = "./hapSign/signed-profile.p7b";
    std::string inFile = "./hapSign/phone-default-unsigned.hap";
    std::string outFile = "./hapSign/phone-default-signed.hap";
    std::string onlineAuthMode = "/mnt/d/testcpuls/OpenHarmony.p12";
    std::string compatibleVersion = "8";
    std::string signerPlugin = "/mnt/d/libRemoteSigner.so";
    std::string signServer = "/mnt/d/testcpuls/app-release1.pem";
    std::string username = "test";
    char userPwd[] = "123456";

    (*params)["mode"] = mode;
    (*params)["keyAlias"] = keyAlias;
    (*params)["signAlg"] = signAlg;
    (*params)["signCode"] = signCode;
    (*params)["profileFile"] = profileFile;
    (*params)["inFile"] = inFile;
    (*params)["outFile"] = outFile;
    (*params)["onlineAuthMode"] = onlineAuthMode;
    (*params)["compatibleVersion"] = compatibleVersion;
    (*params)["signerPlugin"] = signerPlugin;
    (*params)["signServer"] = signServer;
    (*params)["username"] = username;
    (*params)["userPwd"] = userPwd;

    signProvider->CheckParams(params.get());
    return true;
}

bool HapSignTest007(const uint8_t* data, size_t size)
{
    if (!data || !size) {
        return true;
    }

    std::unique_ptr<SignProvider> signProvider = std::make_unique<SignProvider>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string keyAlias = "oh-app1-key-v1";
    std::string signAlg = "SHA256withECDSA";
    std::string signCode = "1";
    std::string appCertFile = "./hapSign/app-release1.pem";
    std::string profileFile = "./hapSign/signed-profile.p7b";
    std::string profileSigned = "1";
    std::string inFile = "./hapSign/phone-default-unsigned";
    std::string keystoreFile = "./hapSign/ohtest.p12";
    std::string outFile = "./hapSign/phone-default-signed.hap";
    char keyPwd[] = "123456";
    char keystorePwd[] = "123456";

    (*params)["mode"] = mode;
    (*params)["keyAlias"] = keyAlias;
    (*params)["signAlg"] = signAlg;
    (*params)["signCode"] = signCode;
    (*params)["appCertFile"] = appCertFile;
    (*params)["profileFile"] = profileFile;
    (*params)["profileSigned"] = profileSigned;
    (*params)["inFile"] = inFile;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["outFile"] = outFile;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keystorePwd"] = keystorePwd;
    signProvider->GetCrl();
    return true;
}

void GenUnvaildFuzzHap(const std::string& path)
{
    std::ofstream outfile(path);
    if (!outfile) {
        SIGNATURE_TOOLS_LOGE("Unable to open file: %s", path.c_str());
        return;
    }
    outfile << "Hello, this is a Unvaild fuzz Hap.\n";
    outfile.flush();
    outfile.close();
    return;
}
}
}

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    OHOS::SignatureTools::GenUnvaildFuzzHap("./hapSign/unsigned_with_cd_and_eocd.hap");
    OHOS::SignatureTools::GenUnvaildFuzzHap("./hapSign/phone-default-unsigned");
    OHOS::SignatureTools::GenUnvaildFuzzHap("./hapSign/phone-default-unsigned.hap");
    sync();
    /* Run your code on data */
    OHOS::SignatureTools::HapSignTest001(data, size);
    OHOS::SignatureTools::HapSignTest002(data, size);
    OHOS::SignatureTools::HapSignTest003(data, size);
    OHOS::SignatureTools::HapSignTest004(data, size);
    OHOS::SignatureTools::HapSignTest005(data, size);
    OHOS::SignatureTools::HapSignTest006(data, size);
    OHOS::SignatureTools::HapSignTest007(data, size);
    return 0;
}