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
#include "signer_factory.h"
#include "dynamic_lib_handle.h"

namespace OHOS {
namespace SignatureTools {

std::shared_ptr<Signer> SignerFactory::GetSigner(LocalizationAdapter& adapter)const
{
    if (adapter.IsRemoteSigner()) {
        return LoadRemoteSigner(adapter);
    }

    EVP_PKEY* keyPair = adapter.GetAliasKey(false);
    if (keyPair == NULL) {
        SIGNATURE_TOOLS_LOGE("key is NULL, get signer failed");
        adapter.ResetPwd();
        return NULL;
    }
    adapter.ResetPwd();
    STACK_OF(X509)*certs = adapter.GetSignCertChain();
    std::shared_ptr<Signer> signer = std::make_shared<LocalSigner>(keyPair, certs);
    return signer;
}

std::shared_ptr<Signer> SignerFactory::LoadRemoteSigner(LocalizationAdapter& adapter) const
{
    std::string keyAlias = adapter.GetOptions()->GetString(ParamConstants::PARAM_BASIC_PRIVATE_KEY);
    std::string signServer = adapter.GetOptions()->GetString(ParamConstants::PARAM_REMOTE_SERVER);
    std::string signerPlugin = adapter.GetOptions()->GetString(ParamConstants::PARAM_REMOTE_SIGNERPLUGIN);
    std::string onlineAuthMode = adapter.GetOptions()->GetString(ParamConstants::PARAM_REMOTE_ONLINEAUTHMODE);
    std::string username = adapter.GetOptions()->GetString(ParamConstants::PARAM_REMOTE_USERNAME);
    char* userPwd = adapter.GetOptions()->GetChars(ParamConstants::PARAM_REMOTE_USERPWD);

    // open so
    DynamicLibHandle::handle = dlopen(signerPlugin.c_str(), RTLD_NOW | RTLD_GLOBAL);
    if (!DynamicLibHandle::handle) {
        PrintErrorNumberMsg("LoadRemoteSigner", RET_FAILED, dlerror());
        return nullptr;
    }

    // clear previous error
    dlerror();

    // get "Create" function
    RemoteSignerCreator remoteSignerCreator = (RemoteSignerCreator)dlsym(DynamicLibHandle::handle, "Create");
    char* error = nullptr;
    if ((error = dlerror()) != NULL) {
        SIGNATURE_TOOLS_LOGE("%s", error);
        return nullptr;
    }

    RemoteSignerParamType keyAliasType{keyAlias.c_str(), keyAlias.size()};
    RemoteSignerParamType signServerType{signServer.c_str(), signServer.size()};
    RemoteSignerParamType onlineAuthModeType{onlineAuthMode.c_str(), onlineAuthMode.size()};
    RemoteSignerParamType usernameType{username.c_str(), username.size()};
    RemoteSignerParamType userPwdType{userPwd, strlen(userPwd)};

    Signer* signer = remoteSignerCreator(keyAliasType, signServerType, onlineAuthModeType, usernameType, userPwdType);

    for (size_t i = 0; i < strlen(userPwd); i++) {
        userPwd[i] = 0;
    }

    std::shared_ptr<Signer> remoteSigner(signer);
    return remoteSigner;
}
} // namespace SignatureTools
} // namespace OHOS