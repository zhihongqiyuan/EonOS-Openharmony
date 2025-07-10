/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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
#ifndef HIPERF_COMMAND_H
#define HIPERF_COMMAND_H

#include <string>
#include <vector>
namespace OHOS {
namespace Developtools {
namespace HiPerf {
class Command {
public:
    static std::string fullArgument;
    static bool DispatchCommands(std::vector<std::string> arguments);
    static bool DispatchCommand(std::string argument);
};
} // namespace HiPerf
} // namespace Developtools
} // namespace OHOS
#endif // HIPERF_COMMAND_H
