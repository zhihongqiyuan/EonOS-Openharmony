/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "macros.h"
#include "panda_executable_path_getter.h"

namespace test::utils {

std::string PandaExecutablePathGetter::Get() const
{
#ifdef BUILD_FOLDER
    return BUILD_FOLDER + std::string("/bin/es2panda test");
#else
    ASSERT_PRINT(false, "BUILD FOLDER not set");
    return std::string {};
#endif
}

}  // namespace test::utils
