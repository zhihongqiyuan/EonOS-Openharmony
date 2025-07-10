/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
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

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>

#define TOSTRING(x) #x

struct ClockTime {
    std::string name;
    clockid_t clockId;
    struct timespec ts;
};

int main()
{
    std::vector<ClockTime> times = {
        { TOSTRING(CLOCK_REALTIME), CLOCK_REALTIME, { 0 } },
        { TOSTRING(CLOCK_REALTIME_ALARM), CLOCK_REALTIME_ALARM, { 0 } },
        { TOSTRING(CLOCK_REALTIME_COARSE), CLOCK_REALTIME_COARSE, { 0 } },
        { TOSTRING(CLOCK_MONOTONIC), CLOCK_MONOTONIC, { 0 } },
        { TOSTRING(CLOCK_MONOTONIC_COARSE), CLOCK_MONOTONIC_COARSE, { 0 } },
        { TOSTRING(CLOCK_MONOTONIC_RAW), CLOCK_MONOTONIC_RAW, { 0 } },
        { TOSTRING(CLOCK_BOOTTIME), CLOCK_BOOTTIME, { 0 } },
        { TOSTRING(CLOCK_BOOTTIME_ALARM), CLOCK_BOOTTIME_ALARM, { 0 } },
    };

    for (auto& time : times) {
        clock_gettime(time.clockId, &time.ts);
    }

    for (auto& time : times) {
        printf("%-25s: %10lld.%09ld\n", time.name.c_str(),
            static_cast<long long>(time.ts.tv_sec), time.ts.tv_nsec);
    }
    fflush(stdout);
    return 0;
}