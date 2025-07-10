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

#include "sign_tool_service_impl.h"
#include  <ctime>

#include "pkcs7_data.h"
#include "profile_sign_tool.h"
#include "nlohmann/json.hpp"
#include "profile_info.h"
#include "profile_verify.h"
#include "signature_tools_errno.h"
#include "local_sign_provider.h"
#include "signature_tools_log.h"
#include "param_constants.h"
#include "verify_hap.h"
#include "constant.h"
#include "remote_sign_provider.h"
#include "verify_elf.h"
#include "verify_bin.h"

namespace OHOS {
namespace SignatureTools {

bool SignToolServiceImpl::GenerateCA(Options* options)
{
    bool flag = true;
    std::unique_ptr<LocalizationAdapter> adapter = std::make_unique<LocalizationAdapter>(options);
    bool isEmpty = FileUtils::IsEmpty(options->GetString(Options::ISSUER_KEY_ALIAS));
    EVP_PKEY* subKey = adapter->GetAliasKey(true);
    if (!subKey) {
        SIGNATURE_TOOLS_LOGE("failed to get subKey!");
        return false;
    }
    if (isEmpty) {
        if (HandleIssuerKeyAliasEmpty(options) == RET_FAILED) {
            EVP_PKEY_free(subKey);
            return false;
        }
        flag = GenerateRootCertToFile(options, subKey);
        EVP_PKEY_free(subKey);
    } else {
        EVP_PKEY* rootKey = nullptr;
        if (HandleIsserKeyAliasNotEmpty(options) == RET_FAILED) {
            EVP_PKEY_free(subKey);
            return false;
        }
        adapter->SetIssuerKeyStoreFile(true);
        rootKey = adapter->GetAliasKey(false);
        flag = GenerateSubCertToFile(options, rootKey);
        EVP_PKEY_free(rootKey);
        EVP_PKEY_free(subKey);
    }
    adapter->ResetPwd();
    return flag;
}

bool SignToolServiceImpl::GenerateRootCertToFile(Options* options, EVP_PKEY* rootKey)
{
    if (rootKey == nullptr) {
        SIGNATURE_TOOLS_LOGE("generate root cert failed because rootKey is nullptr!");
        return false;
    }
    std::string signAlg = options->GetString(Options::SIGN_ALG);
    std::string subject = options->GetString(Options::SUBJECT);
    X509* certPtr = nullptr;
    X509_REQ* csr = nullptr;
    bool result = false;
    do {
        csr = CertTools::GenerateCsr(rootKey, signAlg, subject);
        if (csr == nullptr) {
            SIGNATURE_TOOLS_LOGE("generate root cert failed because csr is nullptr!");
            break;
        }
        certPtr = CertTools::GenerateRootCertificate(rootKey, csr, options);
        if (certPtr == nullptr) {
            SIGNATURE_TOOLS_LOGE("generate root cert failed because certPtr is nullptr!");
            break;
        }
        if (!X509CertVerify(certPtr, rootKey)) {
            SIGNATURE_TOOLS_LOGE("generate root cert failed because verify failed!");
            break;
        }
        if (!OutputModeOfCert(certPtr, options)) {
            SIGNATURE_TOOLS_LOGE("generate root cert failed because output failed!");
            break;
        }
        result = true;
    } while (0);

    X509_free(certPtr);
    X509_REQ_free(csr);
    return result;
}

bool SignToolServiceImpl::GenerateSubCertToFile(Options* options, EVP_PKEY* rootKey)
{
    if (rootKey == nullptr) {
        SIGNATURE_TOOLS_LOGE("generate sub cert failed because rootKey is nullptr!");
        return false;
    }
    std::string signAlg = options->GetString(Options::SIGN_ALG);
    std::string issuer = options->GetString(Options::ISSUER);
    X509* cert = nullptr;
    X509_REQ* csr = nullptr;
    bool result = false;
    do {
        csr = CertTools::GenerateCsr(rootKey, signAlg, issuer);
        if (csr == nullptr) {
            SIGNATURE_TOOLS_LOGE("generate sub cert failed because csr is nullptr!");
            break;
        }
        cert = CertTools::GenerateSubCert(rootKey, csr, options);
        if (cert == nullptr) {
            SIGNATURE_TOOLS_LOGE("generate sub cert failed because cert is nullptr!");
            break;
        }
        if (!X509CertVerify(cert, rootKey)) {
            SIGNATURE_TOOLS_LOGE("generate sub cert failed because verify failed!");
            break;
        }
        if (!OutputModeOfCert(cert, options)) {
            SIGNATURE_TOOLS_LOGE("generate sub cert failed because output failed!");
            break;
        }
        result = true;
    } while (0);

    X509_free(cert);
    X509_REQ_free(csr);
    return result;
}

int SignToolServiceImpl::HandleIssuerKeyAliasEmpty(Options* options)
{
    std::string iksFile = options->GetString(Options::ISSUER_KEY_STORE_FILE);
    if (!FileUtils::IsEmpty(iksFile) && !options->Equals(Options::KEY_STORE_FILE, Options::ISSUER_KEY_STORE_FILE)) {
        PrintErrorNumberMsg("COMMAND_PARAM_ERROR", COMMAND_PARAM_ERROR,
                            "Parameter '" + iksFile + "' and parameter '" +
                            options->GetString(Options::KEY_STORE_FILE) + "' are inconsistent");
        return RET_FAILED;
    }
    char* keyStoreRights = options->GetChars(Options::KEY_STORE_RIGHTS);
    char* issuerKeyStoreRights = options->GetChars(Options::ISSUER_KEY_STORE_RIGHTS);
    if (!FileUtils::IsEmpty(iksFile)) {
        if ((keyStoreRights == nullptr && issuerKeyStoreRights != nullptr) ||
            (keyStoreRights != nullptr && issuerKeyStoreRights == nullptr)) {
            goto err;
        } else if (keyStoreRights == nullptr && issuerKeyStoreRights == nullptr) {
            return RET_OK;
        } else {
            if (std::strcmp(keyStoreRights, issuerKeyStoreRights) != 0) {
                goto err;
            }
        }
    }
    return RET_OK;
err:
    PrintErrorNumberMsg("COMMAND_PARAM_ERROR", COMMAND_PARAM_ERROR,
                        "Parameter 'keystorePwd' and parameter 'issuerKeystorePwd' are inconsistent");
    return RET_FAILED;
}

int SignToolServiceImpl::HandleIsserKeyAliasNotEmpty(Options* options)
{
    std::string iksFile = options->GetString(Options::ISSUER_KEY_STORE_FILE);
    if (!FileUtils::IsEmpty(iksFile)) {
        if (!FileUtils::ValidFileType(iksFile, {"p12", "jks"})) {
            SIGNATURE_TOOLS_LOGE("issuer keystore file type is inconsistent!");
            return RET_FAILED;
        }
    }
    return RET_OK;
}

bool SignToolServiceImpl::OutputModeOfCert(X509* cert, Options* options)
{
    std::string outFile = options->GetString(Options::OUT_FILE);
    if (!outFile.empty()) {
        if (!CertTools::SaveCertTofile(outFile, cert)) {
            PrintErrorNumberMsg("WRITE_FILE_ERROR", IO_ERROR, "failed to save cert to file");
            return false;
        }
    } else {
        if (!PrintX509CertFromMemory(cert)) {
            return false;
        }
    }
    return true;
}

bool SignToolServiceImpl::GenerateCert(Options* options)
{
    std::unique_ptr<LocalizationAdapter> adapter = std::make_unique<LocalizationAdapter>(options);
    std::string signAlg = options->GetString(Options::SIGN_ALG);
    std::string subject = options->GetString(Options::SUBJECT);
    EVP_PKEY* subjectkeyPair = nullptr;
    EVP_PKEY* rootKeyPair = nullptr;
    X509_REQ* csr = nullptr;
    X509* cert = nullptr;
    bool result = false;
    subjectkeyPair = adapter->GetAliasKey(false);
    if (!subjectkeyPair) {
        goto err;
    }
    adapter->SetIssuerKeyStoreFile(true);
    rootKeyPair = adapter->GetIssuerKeyByAlias();
    if (!rootKeyPair) {
        goto err;
    }
    adapter->ResetPwd();
    csr = CertTools::GenerateCsr(subjectkeyPair, signAlg, subject);
    if (!csr) {
        goto err;
    }
    cert = CertTools::GenerateCert(rootKeyPair, csr, options);
    if (!cert) {
        goto err;
    }
    if (!X509CertVerify(cert, rootKeyPair)) {
        goto err;
    }
    if (!OutputModeOfCert(cert, options)) {
        goto err;
    }
    result = true;
err:
    if (result == false)
        SIGNATURE_TOOLS_LOGE("generate cert failed!");
    adapter->ResetPwd();
    X509_free(cert);
    X509_REQ_free(csr);
    EVP_PKEY_free(rootKeyPair);
    EVP_PKEY_free(subjectkeyPair);
    return result;
}

bool SignToolServiceImpl::GenerateKeyStore(Options* options)
{
    std::unique_ptr<LocalizationAdapter> adaptePtr = std::make_unique<LocalizationAdapter>(options);
    std::string keyAlias = adaptePtr->options->GetString(Options::KEY_ALIAS);

    int status = adaptePtr->IsAliasExist(keyAlias);
    if (status == RET_OK) {
        adaptePtr->ResetPwd();
        PrintErrorNumberMsg("KEY_ALIAS_ERROR", KEY_ALIAS_ERROR, "'" + keyAlias
                            + "' key alias already exists and cannot be generated repeatedly");
        return false;
    }

    if (!adaptePtr->keyStoreHelper->GetPassWordStatus()) {
        adaptePtr->ResetPwd();
        return false;
    }

    EVP_PKEY* keyPair = nullptr;
    keyPair = adaptePtr->GetAliasKey(true);
    adaptePtr->ResetPwd();
    if (keyPair == nullptr) {
        SIGNATURE_TOOLS_LOGE("failed to get keypair!");
        return false;
    }
    EVP_PKEY_free(keyPair);
    return true;
}

bool SignToolServiceImpl::GenerateCsr(Options* options)
{
    std::unique_ptr<LocalizationAdapter> adapter = std::make_unique<LocalizationAdapter>(options);
    EVP_PKEY* keyPair = adapter->GetAliasKey(false);
    if (!keyPair) {
        SIGNATURE_TOOLS_LOGE("failed to get keypair!");
        adapter->ResetPwd();
        return false;
    }
    adapter->ResetPwd();
    X509_REQ* csr = nullptr;
    std::string signAlg = options->GetString(Options::SIGN_ALG);
    std::string subject = options->GetString(Options::SUBJECT);
    if (signAlg.empty()) {
        PrintErrorNumberMsg("INVALIDPARAM_ERROR",
                            INVALIDPARAM_ERROR,
                            "Please check if signalg has been specified which is required.");
        EVP_PKEY_free(keyPair);
        return false;
    }
    if (subject.empty()) {
        PrintErrorNumberMsg("INVALIDPARAM_ERROR",
                            INVALIDPARAM_ERROR,
                            "Please check if subject has been specified which is required.");
        EVP_PKEY_free(keyPair);
        return false;
    }
    csr = CertTools::GenerateCsr(keyPair, signAlg, subject);
    if (!csr) {
        SIGNATURE_TOOLS_LOGE("failed to generate csr request!");
        EVP_PKEY_free(keyPair);
        return false;
    }
    std::string csrStr = CertTools::CsrToString(csr);
    EVP_PKEY_free(keyPair);
    X509_REQ_free(csr);
    if (csrStr.size() == 0) {
        PrintErrorNumberMsg("PARSE_ERROR", PARSE_ERROR,
                            "failed to transform csr to string!");
        return false;
    }
    std::string outFile = options->GetString(Options::OUT_FILE);
    return OutputString(csrStr, outFile);
}

bool SignToolServiceImpl::OutputString(std::string content, std::string file)
{
    if (file.size() == 0) {
        puts(content.c_str());
        return true;
    }
    std::ofstream outFile(file.c_str());
    if (!outFile.is_open()) {
        PrintErrorNumberMsg("IO_ERROR", IO_ERROR,
                            "failed to open the outFile " + file + " !");
        return false;
    }
    outFile << content;
    outFile.close();
    return true;
}

bool SignToolServiceImpl::X509CertVerify(X509* cert, EVP_PKEY* privateKey)
{
    if (!X509_verify(cert, privateKey)) {
        PrintErrorNumberMsg("VERIFY_ERROR", VERIFY_ERROR, "Verify certificate failed!");
        return false;
    }
    return true;
}

X509_REQ* SignToolServiceImpl::GetCsr(EVP_PKEY* keyPair, std::string signAlg, std::string subject)
{
    if (signAlg.empty() || subject.empty()) {
        SIGNATURE_TOOLS_LOGE("failed to get signalg or subject!");
        return nullptr;
    }
    X509_REQ* csr = CertTools::GenerateCsr(keyPair, signAlg, subject);
    if (!csr) {
        SIGNATURE_TOOLS_LOGE("failed to generate csr!");
        return nullptr;
    }
    return csr;
}

bool SignToolServiceImpl::GenerateAppCert(Options* options)
{
    std::unique_ptr<LocalizationAdapter> adapter = std::make_unique<LocalizationAdapter>(options);
    EVP_PKEY* keyPairPtr = nullptr;
    EVP_PKEY* issuerKeyPairPtr = nullptr;
    X509_REQ* csrPtr = nullptr;
    X509* x509CertificatePtr = nullptr;
    std::string signAlg = adapter->options->GetString(Options::SIGN_ALG);
    std::string subject = adapter->options->GetString(Options::SUBJECT);

    if (!(keyPairPtr = adapter->GetAliasKey(false))) { // get keypair
        goto err;
    }
    adapter->SetIssuerKeyStoreFile(true);
    if (!(issuerKeyPairPtr = adapter->GetIssuerKeyByAlias())) { // get issuer keypair
        goto err;
    }
    adapter->ResetPwd(); // clean pwd for safety
    csrPtr = GetCsr(keyPairPtr, signAlg, subject);
    if (!csrPtr) { // get CSR request
        goto err;
    }
    x509CertificatePtr = CertTools::GenerateEndCert(csrPtr, issuerKeyPairPtr, *adapter,
                                                    APP_SIGNING_CAPABILITY,
                                                    sizeof(APP_SIGNING_CAPABILITY)); // get app x509 cert
    if (!x509CertificatePtr) {
        PrintErrorNumberMsg("CERTIFICATE_ERROR", CERTIFICATE_ERROR, "generate app cert failed");
        goto err;
    }
    if (!X509CertVerify(x509CertificatePtr, issuerKeyPairPtr)) {
        goto err;
    }
    if (!GetAndOutPutCert(*adapter, x509CertificatePtr)) {
        goto err;
    }
    // realse heap memory
    adapter->AppAndProfileAssetsRealse({issuerKeyPairPtr, keyPairPtr}, {csrPtr}, {x509CertificatePtr});
    return true;
    
err:
    adapter->AppAndProfileAssetsRealse({issuerKeyPairPtr, keyPairPtr}, {csrPtr}, {x509CertificatePtr});
    adapter->ResetPwd();
    return false;
}

bool SignToolServiceImpl::GenerateProfileCert(Options* options)
{
    std::unique_ptr<LocalizationAdapter> adapter = std::make_unique<LocalizationAdapter>(options);
    EVP_PKEY* keyPair = nullptr;
    EVP_PKEY* issuerKeyPair = nullptr;
    X509_REQ* csr = nullptr;
    X509* x509Certificate = nullptr;
    std::string signAlg = adapter->options->GetString(Options::SIGN_ALG);
    std::string subject = adapter->options->GetString(Options::SUBJECT);
    
    if (!(keyPair = adapter->GetAliasKey(false))) { // get keypair
        goto err;
    }
    adapter->SetIssuerKeyStoreFile(true);
    if (!(issuerKeyPair = adapter->GetIssuerKeyByAlias())) { // get issuer keypair
        goto err;
    }
    adapter->ResetPwd(); // clean pwd for safety
    csr = GetCsr(keyPair, signAlg, subject);
    if (!csr) { // get CSR request
        goto err;
    }
    x509Certificate = CertTools::GenerateEndCert(csr, issuerKeyPair, *adapter,
                                                 PROFILE_SIGNING_CAPABILITY,
                                                 sizeof(PROFILE_SIGNING_CAPABILITY)); // get profile x509 cert
    if (!x509Certificate) {
        PrintErrorNumberMsg("CERTIFICATE_ERROR", CERTIFICATE_ERROR, "generate profile cert failed");
        goto err;
    }
    if (!X509CertVerify(x509Certificate, issuerKeyPair)) {
        goto err;
    }
    if (!GetAndOutPutCert(*adapter, x509Certificate)) { // output cert to file
        goto err;
    }

    adapter->AppAndProfileAssetsRealse({issuerKeyPair, keyPair}, {csr}, {x509Certificate}); // realse heap memory
    return true;
    
err:
    adapter->AppAndProfileAssetsRealse({issuerKeyPair, keyPair}, {csr}, {x509Certificate});
    adapter->ResetPwd();
    return false;
}

bool SignToolServiceImpl::GetAndOutPutCert(LocalizationAdapter& adapter, X509* cert)
{
    std::string outFile = adapter.options->GetString(Options::OUT_FILE);
    bool successflag = false;
    X509* subCaCert = nullptr;
    X509* rootCaCert = nullptr;
    if (adapter.IsOutFormChain()) {
        std::vector<X509*> certificates;
        certificates.emplace_back(cert); // add entity cert
        successflag = (!(subCaCert = adapter.GetSubCaCertFile()) ||
                       !(rootCaCert = adapter.GetCaCertFile()));
        if (successflag) {
            return false;
        }
        certificates.emplace_back(subCaCert); // add sub ca cert
        certificates.emplace_back(rootCaCert); // add root ca cert

        if (outFile.empty()) {
            successflag = PrintX509CertChainFromMemory(certificates); // print certchain to cmd
            adapter.AppAndProfileAssetsRealse({}, {}, {certificates[1], certificates[2]});
            return successflag;
        }
        successflag = OutPutCertChain(certificates, adapter.GetOutFile()); // out put certchain to file
        adapter.AppAndProfileAssetsRealse({}, {}, {certificates[1], certificates[2]});
        return successflag;
    }

    if (outFile.empty()) {
        successflag = PrintX509CertFromMemory(cert); // print cert to cmd
        return successflag;
    }
    successflag = OutPutCert(cert, adapter.GetOutFile()); // out put cert to file
    return successflag;
}

bool SignToolServiceImpl::SignProfile(Options* options)
{
    LocalizationAdapter adapter(options);
    const std::string inFile = adapter.GetInFile();
    const std::string outFile = adapter.GetOutFile();
    std::string provisionContent;
    std::string p7b;
    if (SignToolServiceImpl::GetProvisionContent(inFile, provisionContent) < 0) {
        SIGNATURE_TOOLS_LOGE("getProvisionContent failed");
        return false;
    }
    if (ProfileSignTool::GenerateP7b(adapter, provisionContent, p7b) < 0) {
        SIGNATURE_TOOLS_LOGE("generate P7b data failed");
        return false;
    }
    if (FileUtils::Write(p7b, outFile) < 0) {
        SIGNATURE_TOOLS_LOGE("write p7b data failed");
        return false;
    }
    return true;
}

bool SignToolServiceImpl::SignHap(Options* options)
{
    std::string mode = options->GetString(Options::MODE);
    std::shared_ptr<SignProvider> signProvider;
    if (LOCAL_SIGN == mode) {
        signProvider = std::make_shared<LocalSignProvider>();
    } else if (REMOTE_SIGN == mode) {
        signProvider = std::make_shared<RemoteSignProvider>();
    } else {
        SIGNATURE_TOOLS_LOGE("Resign mode. But not implemented yet");
        return false;
    }
    std::string inForm = options->GetString(Options::INFORM);
    if (ZIP == inForm) {
        return signProvider->Sign(options);
    } else if (ELF == inForm) {
        return signProvider->SignElf(options);
    } else {
        return signProvider->SignBin(options);
    }
    return true;
}

bool SignToolServiceImpl::VerifyProfile(Options* options)
{
    LocalizationAdapter adapter(options);
    std::string p7b;
    if (FileUtils::ReadFile(adapter.GetInFile(), p7b) < 0) {
        SIGNATURE_TOOLS_LOGE("read p7b data error");
        return false;
    }
    PKCS7Data p7Data;
    if (p7Data.Parse(p7b) < 0) {
        SIGNATURE_TOOLS_LOGE("verify profile failed");
        return false;
    }
    if (p7Data.Verify() < 0) {
        SIGNATURE_TOOLS_LOGE("verify profile failed");
        return false;
    }
    const std::string outFile = adapter.GetOutFile();
    std::string originalData;
    if (p7Data.GetContent(originalData) < 0) {
        SIGNATURE_TOOLS_LOGE("get content failed");
        return false;
    }
    if (outFile.empty()) {
        PrintMsg(originalData);
    } else {
        std::ofstream out(outFile, std::ios::binary);
        out.write(originalData.data(), originalData.size());
    }
    return true;
}

bool SignToolServiceImpl::OutPutCertChain(std::vector<X509*>& certs, const std::string& outPutPath)
{
    SIGNATURE_TOOLS_LOGD("outPutPath = %s", outPutPath.c_str());
    BIO* bio = nullptr;
    if (!(bio = BIO_new_file(outPutPath.c_str(), "wb"))) {
        SIGNATURE_TOOLS_LOGE("failed to open file %s", outPutPath.c_str());
        goto err;
    }
    for (auto cert : certs) {
        if (PEM_write_bio_X509(bio, cert) < 0) {
            SIGNATURE_TOOLS_LOGE("failed to write certChain to file!");
            goto err;
        }
    }
    BIO_free(bio);
    return true;
err:
    VerifyHapOpensslUtils::GetOpensslErrorMessage();
    BIO_free(bio);
    return false;
}

bool SignToolServiceImpl::OutPutCert(X509* cert, const std::string& outPutPath)
{
    BIO* bio = BIO_new_file(outPutPath.c_str(), "wb");
    if (!bio) {
        SIGNATURE_TOOLS_LOGE("failed to open file: %s", outPutPath.c_str());
        goto err;
    }
    if (!PEM_write_bio_X509(bio, cert)) {
        SIGNATURE_TOOLS_LOGE("failed to write cert to file!");
        goto err;
    }
    BIO_free(bio);
    return true;
err:
    VerifyHapOpensslUtils::GetOpensslErrorMessage();
    BIO_free(bio);
    return false;
}

int SignToolServiceImpl::GetProvisionContent(const std::string& input, std::string& ret)
{
    std::string bytes;
    if (FileUtils::ReadFile(input, bytes) < 0) {
        SIGNATURE_TOOLS_LOGE("provision read faild!");
        return IO_ERROR;
    }
    nlohmann::json obj = nlohmann::json::parse(bytes);
    if (obj.is_discarded() || (!obj.is_structured())) {
        PrintErrorNumberMsg("PARSE ERROR", PARSE_ERROR, "Parsing appProvision failed!");
        return PARSE_ERROR;
    }
    ret = obj.dump();
    ProfileInfo provision;
    AppProvisionVerifyResult result = ParseProvision(ret, provision);
    if (result != PROVISION_OK) {
        SIGNATURE_TOOLS_LOGE("invalid provision");
        return INVALIDPARAM_ERROR;
    }
    return 0;
}

bool SignToolServiceImpl::PrintX509CertFromMemory(X509* cert)
{
    BIO* bio = BIO_new(BIO_s_mem());
    if (!bio) {
        VerifyHapOpensslUtils::GetOpensslErrorMessage();
        return false;
    }
    if (PEM_write_bio_X509(bio, cert) == 1) {
        BUF_MEM* bptr;
        BIO_get_mem_ptr(bio, &bptr);
        PrintMsg(std::string(bptr->data, bptr->length));
    } else {
        VerifyHapOpensslUtils::GetOpensslErrorMessage();
        PrintErrorNumberMsg("IO_ERROR", IO_ERROR, "print x509 cert falied");
        BIO_free(bio);
        return false;
    }
    BIO_free(bio);
    return true;
}

bool SignToolServiceImpl::PrintX509CertChainFromMemory(std::vector<X509*> certs)
{
    for (auto cert : certs) {
        BIO* bio = BIO_new(BIO_s_mem());
        BUF_MEM* bptr = nullptr;
        if (!bio) {
            VerifyHapOpensslUtils::GetOpensslErrorMessage();
            return false;
        }
        if (PEM_write_bio_X509(bio, cert) == 1) {
            BIO_get_mem_ptr(bio, &bptr);
            PrintMsg(std::string(bptr->data, bptr->length));
        } else {
            VerifyHapOpensslUtils::GetOpensslErrorMessage();
            PrintErrorNumberMsg("IO_ERROR", IO_ERROR, "print x509 cert chain falied");
            BIO_free(bio);
            return false;
        }
        BIO_free(bio);
    }
    return true;
}

bool SignToolServiceImpl::VerifyHapSigner(Options* option)
{
    std::string inForm = option->GetString(ParamConstants::PARAM_IN_FORM);
    if (inForm == ZIP) {
        VerifyHap hapVerify;
        int32_t ret = hapVerify.Verify(option->GetString(Options::IN_FILE), option);
        if (ret == RET_OK) {
            PrintMsg("hap verify successed!");
            return true;
        }
        PrintMsg("hap verify failed !");
        return false;
    } else if (inForm == ELF) {
        VerifyElf verifyElf;
        if (!verifyElf.Verify(option)) {
            PrintMsg("elf verify failed!");
            return false;
        }
        PrintMsg("elf verify successed!");
        return true;
    } else if (inForm == BIN) {
        VerifyBin verifyBin;
        if (!verifyBin.Verify(option)) {
            PrintMsg("bin verify failed!");
            return false;
        }
        PrintMsg("bin verify successed!");
        return true;
    } else {
        PrintErrorNumberMsg("NOT_SUPPORT_ERROR", NOT_SUPPORT_ERROR, "Unsupported inForm!");
        return false;
    }
}

} // namespace SignatureTools
} // namespace OHOS
