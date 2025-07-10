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
#ifndef SINATURETOOLS_SIGNER_FACTORY_H
#define SINATURETOOLS_SIGNER_FACTORY_H

#include <dlfcn.h>

#include "local_signer.h"
#include "localization_adapter.h"
#include "param_constants.h"

namespace OHOS {
namespace SignatureTools {

typedef struct RemoteSignerParamTypeSt {
    const char* data;
    size_t len;
} RemoteSignerParamType;

typedef Signer* (*RemoteSignerCreator)(RemoteSignerParamType,
                                       RemoteSignerParamType,
                                       RemoteSignerParamType,
                                       RemoteSignerParamType,
                                       RemoteSignerParamType);

class SignerFactory {
public:
    SignerFactory() = default;
    ~SignerFactory() = default;

    std::shared_ptr<Signer> GetSigner(LocalizationAdapter& adapter) const;

private:
    std::shared_ptr<Signer> LoadRemoteSigner(LocalizationAdapter& adapter) const;
};
} // namespace SignatureTools
} // namespace OHOS
#endif