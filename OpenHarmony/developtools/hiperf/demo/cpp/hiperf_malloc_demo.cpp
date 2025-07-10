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
using namespace HiperfMallocDemo;

int main(const int argc, const char *argv[])
{
    static const int WART_TIME = 100;
    static const int COUNT_TRY = 1000;
    const int bufSize = 1048576;

    printf("demo start\n");
    sleep(WART_TIME);
    void *temp = nullptr;
    // try for each thread times
    for (int i = 0; i < COUNT_TRY; i++) {
        temp = malloc(bufSize);
        printf("malloc %d \n", i);
        sleep(WART_TIME);
        if (temp != nullptr) {
            free(temp);
            temp = nullptr;
        }
    }
    printf("demo stop \n");
    if (temp != nullptr) {
        free(temp);
        temp = nullptr;
    }
    return 0;
};
