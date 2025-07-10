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
#include <thread>
#include <unistd.h>
#include <vector>

#include "hiperf_client.h"

using namespace OHOS::Developtools::HiPerf;

#if defined(__aarch64__)
static const std::string TEST_PROCESSES = "com.ohos.sceneboard";
#else
static const std::string TEST_PROCESSES = "com.ohos.launcher";
#endif

namespace HiperfClientDemo {
void TestCodeThread(int id)
{
    constexpr uint32_t k = 1024;
    constexpr uint32_t k2 = 2 * k;
    constexpr uint32_t k10 = 10 * k;
    constexpr uint32_t two = 2;
    constexpr uint64_t thousand = 1000;

    std::vector<std::unique_ptr<char[]>> mems;
    printf("TestCodeThread %d:++\n", id);
    std::this_thread::sleep_for(std::chrono::milliseconds(thousand));

    for (uint32_t i = 0; i < k10; i++) {
        if (i % two == 0) {
            mems.push_back(std::make_unique<char[]>(k));
        } else {
            mems.push_back(std::make_unique<char[]>(k2));
        }
    }

    for (uint32_t i = 0; i < k10; i++) {
        mems.pop_back();
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(thousand));
    printf("TestCodeThread %d:--\n", id);
}
} // namespace HiperfClientDemo

using namespace HiperfClientDemo;

int main()
{
    const int waitTime = 2;
    const int countTry = 3;

    HiperfClient::Client myHiperf;
    printf("GetOutputDir:'%s'\n", myHiperf.GetOutputDir().c_str());
    printf("GetCommandPath:'%s'\n", myHiperf.GetCommandPath().c_str());
    myHiperf.SetDebugMode();
    printf("demo start\n");
    HiperfClient::RecordOption opt;
    const int timeout = 30;
    opt.SetAppPackage(TEST_PROCESSES);
    opt.SetTimeStopSec(timeout);
    if (myHiperf.Start(opt)) {
        printf("demo start successfully\n");
    }
    auto it = []() {
        TestCodeThread(0);
    };
    std::thread workload(it);
    sleep(waitTime);
    // try for each thread times
    for (int i = 0; i < countTry; i++) {
        printf("demo pause\n");
        if (!myHiperf.Pause()) {
            printf("demo pause failed\n");
        }
        sleep(1);
        printf("demo resume \n");
        if (!myHiperf.Resume()) {
            printf("demo resume failed\n");
        }
        sleep(1);
    }
    printf("demo stop \n");
    if (myHiperf.Stop()) {
        printf("demo stop successfully\n");
    }
    workload.join();
    return 0;
};
