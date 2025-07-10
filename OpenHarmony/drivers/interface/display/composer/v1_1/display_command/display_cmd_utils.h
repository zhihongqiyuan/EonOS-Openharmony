/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#ifndef OHOS_HDI_DISPLAY_V1_1_DISPLAY_CMD_UTILS_H
#define OHOS_HDI_DISPLAY_V1_1_DISPLAY_CMD_UTILS_H

#include "v1_0/display_command/display_cmd_utils.h"
#include "v1_1/display_composer_type.h"

#undef LOG_TAG
#define LOG_TAG "DISP_CMD"
#undef LOG_DOMAIN
#define LOG_DOMAIN 0xD002515

namespace OHOS {
namespace HDI {
namespace Display {
namespace Composer {
namespace V1_1 {

class DisplayCmdUtils : public V1_0::DisplayCmdUtils {};
using CmdUtils = DisplayCmdUtils;
} // namespace V1_1
} // namespace Composer
} // namespace Display
} // namespace HDI
} // namespace OHOS
#endif // OHOS_HDI_DISPLAY_V1_1_DISPLAY_CMD_UTILS_H