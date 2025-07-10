/*
 * Copyright (c) 2021-2023 Huawei Device Co., Ltd.
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
#include <memory>
#include <unistd.h>

#include "hiview_platform.h"
#include "hiview_service.h"

#include "defines.h"
#include "hiview_logger.h"
#include "memory_util.h"
#include "dump_manager_cpu_service.h"

DEFINE_LOG_TAG("HiView-Main");
using namespace OHOS::HiviewDFX;

int main(int argc __UNUSED, char* argv[] __UNUSED)
{
    if (MemoryUtil::DisableThreadCache() != 0 || MemoryUtil::DisableDelayFree() != 0) {
        HIVIEW_LOGW("Failed to optimize memory for current thread");
    }

    auto& hiview = HiviewPlatform::GetInstance();
    // process cmdline
    hiview.ProcessArgsRequest(argc, argv);

    // parse configs and load plugin
    if (!hiview.InitEnvironment()) {
        HIVIEW_LOGW("Fail to init plugin environment. exit");
        _exit(-1); // user _exit to avoid double-free of HiviewPlatform
    }

    auto hidumperCpuService = std::make_unique<DumpManagerCpuService>();
    hidumperCpuService->StartService();

    // start service
    auto hiviewService = std::make_unique<HiviewService>();
    hiviewService->StartService();

    return 0;
}
