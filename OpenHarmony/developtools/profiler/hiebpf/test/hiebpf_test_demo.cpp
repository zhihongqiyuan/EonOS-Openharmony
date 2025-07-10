/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
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
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <dlfcn.h>
#include <new>
#include <pthread.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <thread>
#include <sys/prctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <vector>

#pragma clang optimize off

namespace {
constexpr int ARGC_NUM_MUST = 3;
constexpr int ARGC_WRITE_READ_TIMES = 2;
constexpr int BUF_SIZE = 512;
constexpr int FILE_MODE = 0644;
std::string DEFAULT_PATH("./");

class Timer {
public:
    using Clock = std::chrono::steady_clock;
    using TimePoint = Clock::time_point;

    Timer() : startTime_(Now()) {}

    ~Timer() {}

    long ElapsedUs()
    {
        auto currentTime = Now();
        return std::chrono::duration_cast<std::chrono::microseconds>(currentTime - startTime_).count();
    }

    void Reset()
    {
        startTime_ = Now();
    }

protected:
    static TimePoint Now()
    {
        return Clock::now();
    }

private:
    TimePoint startTime_;
};

static int g_sleepUs = 0;
void CallStack1(const char * filename)
{
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, FILE_MODE);
    if (fd < 0) {
        printf("NOTE Open file err: %s!\n", filename);
        return ;
    }
    if (g_sleepUs) {
        usleep(g_sleepUs);
    }

    std::vector<char> buf(BUF_SIZE * 2, 'a'); // 2: double
    write(fd, buf.data(), buf.size());
    fsync(fd);
    if (g_sleepUs) {
        usleep(g_sleepUs);
    }

    lseek(fd, 0, SEEK_SET);
    read(fd, buf.data(), BUF_SIZE);
    read(fd, buf.data(), BUF_SIZE);
    if (g_sleepUs) {
        usleep(g_sleepUs);
    }

    close(fd);
    if (g_sleepUs) {
        usleep(g_sleepUs);
    }
}
void CallStack0(const char * filename)
{
    CallStack1(filename);
}

void* ThreadFuncCpp(void* param)
{
    unsigned int idx = 0;
    int threadNo = static_cast<int*>(param)[0];
    int time = static_cast<int*>(param)[1];
    long tid = syscall(SYS_gettid);

    printf("start threadNo: %d , threadId: %ld\n", threadNo, tid);

    std::string name = "thread";
    name = name + std::to_string(threadNo);
    prctl(PR_SET_NAME, name.c_str());
    name = DEFAULT_PATH + name;
    const auto startTime = std::chrono::steady_clock::now();
    std::chrono::seconds timeOut{time};
    const auto endTime = startTime + timeOut;

    while (true) {
        const auto thisTime = std::chrono::steady_clock::now();
        if (thisTime >= endTime) {
            break;
        }

        CallStack0(name.c_str());
        idx++;
        printf("%2d: Thread %ld file(%s) open,write,read,close %u times\n", threadNo, tid, name.c_str(), idx);
    }

    return nullptr;
}

long ThreadTimeCost(int threadNum, int writeReadTime)
{
    Timer timer = {};
    int idx;
    int args[threadNum][2];

    if (threadNum <= 0) {
        printf("threadNum less than or equal to 0.\n");
        return 1;
    }

    pthread_t* thrArray = new (std::nothrow) pthread_t[threadNum];
    if (!thrArray) {
        printf("new thread array failed.\n");
        return 1;
    }

    for (idx = 0; idx < threadNum; ++idx) {
        args[idx][0] = idx;
        args[idx][1] = writeReadTime;
        if (pthread_create(thrArray + idx, nullptr, ThreadFuncCpp, static_cast<void*>(args[idx])) != 0) {
            printf("Creating thread failed.\n");
        }
    }

    for (idx = 0; idx < threadNum; ++idx) {
        pthread_join(thrArray[idx], nullptr);
    }
    delete []thrArray;

    return timer.ElapsedUs();
}
} // namespace

int main(int argc, char *argv[])
{
    int threadNum = 1;
    int writeReadTime = 0;
    if  (argc >= ARGC_NUM_MUST) {
        if (atoi(argv[1]) > 0) {
            threadNum = atoi(argv[1]);
        }
        writeReadTime = atoi(argv[ARGC_WRITE_READ_TIMES]);
    } else {
        printf("Command error,Please input:thread Num, run seconds, sleep us\n");
        return 0;
    }
    if  (argc > ARGC_NUM_MUST) {
        g_sleepUs = atoi(argv[3]); // 3: 4th argument
    }

    printf("Test start %d thread, read or write %d times\n", threadNum, writeReadTime);
    printf("First isn't bpf hook.");

    auto timeCost = ThreadTimeCost(threadNum, writeReadTime);
    if (timeCost > 0) {
        printf("Time cost %ld us.\n", timeCost);
    } else {
        printf("Test failure end!\n");
        return 0;
    }

    return 0;
}

#pragma clang optimize on
