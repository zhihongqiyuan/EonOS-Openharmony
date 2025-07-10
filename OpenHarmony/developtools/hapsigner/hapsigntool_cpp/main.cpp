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
#include "params_run_tool.h"
using namespace OHOS::SignatureTools;
int main(int argc, char** argv)
{
    // prepare modes vector by macro DEFINE_MODE which subscribe UPDATER_MAIN_PRE_EVENT event
    bool isSuccess = ParamsRunTool::ProcessCmd(argv, argc);
    if (isSuccess) {
        return 0;
    }
    return 1;
}