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
#ifndef SIGNATURETOOLS_HAP_SIGN_TEST_H
#define SIGNATURETOOLS_HAP_SIGN_TEST_H
#include <gtest/gtest.h>

#include "localization_adapter.h"
#include "openssl/err.h"
#include "openssl/pem.h"
#include "openssl/ssl.h"
#include "options.h"
#include "sign_tool_service_impl.h"
#include "signature_tools_log.h"

namespace OHOS {
namespace SignatureTools {
class HapSignTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp() {};
    void TearDown() {};
};
} // namespace SignatureTools
} // namespace OHOS

#endif // SIGNATURETOOLS_HAP_SIGN_TEST_H