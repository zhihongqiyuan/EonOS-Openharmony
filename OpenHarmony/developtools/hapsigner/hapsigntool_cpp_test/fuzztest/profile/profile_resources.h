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
#ifndef PROFILE_RESOURCE_H
#define PROFILE_RESOURCE_H

#include <chrono>
#include <thread>
#include <string>

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
const std::string SIGN_PROFILE_MODE = "localSign";
const std::string SIGN_PROFILE_KEY_ALIAS = "oh-profile1-key-v1";
const std::string SIGN_PROFILE_PROFILE_CERT_FILE = "./signProfile/profile-release1.pem";
const std::string SIGN_PROFILE_SIGN_ALG = "SHA384withECDSA";
const std::string SIGN_PROFILE_KEY_STORE_FILE = "./signProfile/ohtest.p12";
const std::string SIGN_PROFILE_OUT_FILE = "./signProfile/signed-profile.p7b";
const std::string SIGN_PROFILE_IN_FILE = "./signProfile/profile.json";

const std::string SIGN_PROFILE_CERT_PEM = "./signProfile/profile-release1-cert.pem";
const std::string SIGN_PROFILE_REVERSE_PEM = "./signProfile/profile-release1-reverse.pem";
const std::string SIGN_PROFILE_DOUBLE_CERT_PEM = "./signProfile/"
"profile-release1-invalid_cert_chain.pem";

// verify profile 使用的全局参数
const std::string VERIFY_PROFILE_IN_FILE = "./signProfile/app1-profile1.p7b";
const std::string VERIFY_PROFILE_OUT_FILE = "./signProfile/verify-result.json";
}
}
#endif

