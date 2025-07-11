/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#include "spdaemon_fuzzer.h"

#include "FileDescriptor.h"
#include "Threads.h"
using namespace OHOS::SmartPerf;

namespace OHOS {
bool FuzzGetThreads(const uint8_t* data, size_t size)
{
    Threads &ths = Threads::GetInstance();
    std::string dataArg(reinterpret_cast<const char *>(data), size);
    ths.SetPackageName(dataArg);
    ths.SetProcessIdForFuzzTest({dataArg});
    ths.ItemData();
    return true;
}
bool FuzzGetFds(const uint8_t* data, size_t size)
{
    FileDescriptor &fds = FileDescriptor::GetInstance();
    std::string dataArg(reinterpret_cast<const char *>(data), size);
    fds.SetPackageName(dataArg);
    fds.SetProcessIdForFuzzTest({dataArg});
    fds.ItemData();
    return true;
}
} // namespace OHOS

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    /* Run your code on data */
    OHOS::FuzzGetThreads(data, size);
    OHOS::FuzzGetFds(data, size);
    return 0;
}

