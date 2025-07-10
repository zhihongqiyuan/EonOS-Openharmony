/*
 * Copyright (C) 2024 Huawei Device Co., Ltd.
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

#ifndef CONTACTS_PERMISSION_H
#define CONTACTS_PERMISSION_H

#include <string>

namespace OHOS {
namespace ContactsFfi {
namespace Permission {
static constexpr const char *READ_CONTACTS = "ohos.permission.READ_CONTACTS";
static constexpr const char *WRITE_CONTACTS = "ohos.permission.WRITE_CONTACTS";
static constexpr const char *WRITE_CALL_LOG = "ohos.permission.WRITE_CALL_LOG";
static constexpr const char *READ_CALL_LOG = "ohos.permission.READ_CALL_LOG";
static constexpr const char *OHOS_PERMISSION_MANAGE_VOICEMAIL = "ohos.permission.MANAGE_VOICEMAIL";
} // namespace Permission

class ContactsTelephonyPermission {
public:
    ContactsTelephonyPermission();
    ~ContactsTelephonyPermission();
    static bool CheckPermission(const std::string &permissionName);
};
} // namespace Telephony
} // namespace OHOS
#endif // PERMISSION_H