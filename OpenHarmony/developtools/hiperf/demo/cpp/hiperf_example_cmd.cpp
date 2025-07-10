/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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
#include <algorithm>
#include <cctype>
#include <chrono>
#include <cstring>
#include <iostream>
#include <random>
#include <string>
#include <thread>
#include <vector>

#include <sys/mman.h>
#include <sys/syscall.h>
#include <unistd.h>

using namespace std::chrono;
using namespace std::chrono_literals;

namespace {
#define USED_FUNCTION __attribute__((__used__)) __attribute__((optnone))
constexpr milliseconds EACH_STACK_FUN_RUN_TIME = 100ms;
constexpr milliseconds MS_DUARTION = 1000ms;
const ssize_t ERRINFOLEN = 512;

extern USED_FUNCTION void LoopBranch0(std::default_random_engine &rnd, int level);
extern USED_FUNCTION void LoopBranch1(std::default_random_engine &rnd, int level);

struct Option {
    int numThreads {5};
    int second {36000};
    int stack {5};
    bool noWait = false;
    bool dynamicStack = false;
    bool mmap = false;
    bool iowait = false;
    bool branch = false;
    bool nonew = false;
    bool nofunc = false;
    int boundCpu {-1};
    int sleepms {0};
};

inline int GetTid()
{
    int res = static_cast<int>(syscall(SYS_gettid));
    return res;
}

USED_FUNCTION void LoopDummy(milliseconds anything)
{
    if (anything.count() > 0) {
        printf("");
    }
}

USED_FUNCTION void LoopBranch0(std::default_random_engine &rnd, int level)
{
    constexpr int two {2};
    if (level == 0) {
        printf("");
        return;
    }
    if (rnd() % two == 0) {
        LoopBranch0(rnd, --level);
    } else {
        LoopBranch1(rnd, --level);
    }
}

USED_FUNCTION void LoopBranch1(std::default_random_engine &rnd, int level)
{
    constexpr int two {2};
    if (level == 0) {
        printf("");
        return;
    }
    if (rnd() % two == 0) {
        LoopBranch0(rnd, --level);
    } else {
        LoopBranch1(rnd, --level);
    }
}

USED_FUNCTION void LoopBranch()
{
    constexpr int two {2};
    int branchLevel = 10;
    std::default_random_engine rnd;
    if (rnd() % two == 0) {
        LoopBranch0(rnd, branchLevel);
    } else {
        LoopBranch1(rnd, branchLevel);
    }
}

USED_FUNCTION void LoopIowait()
{
    std::default_random_engine rnd;
    FILE *fp = fopen("temp", "rw");
    if (fp == nullptr) {
        return;
    }
    
    std::unique_ptr<FILE, decltype(&fclose)> fd {fp, &fclose};
    if (fd != nullptr) {
        const std::string tempBuf = std::to_string(rnd());
        fwrite(tempBuf.c_str(), tempBuf.size(), 1, fd.get());
    }
}

USED_FUNCTION void LoopMmap()
{
    int *arr = static_cast<int *>(
        mmap(nullptr, getpagesize(), PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, 0, 0));

    int *ptr = arr;
    int someVal1 {10};
    int someVal2 {20};
    int someVal3 {30};
    *ptr = someVal1;
    ++ptr;
    *ptr = someVal2;
    ++ptr;
    *ptr = someVal3;
    munmap(arr, getpagesize());
    return;
}

USED_FUNCTION void LoopFunction(milliseconds timeOutMS, const Option &option)
{
    auto now = std::chrono::steady_clock::now();
    auto sleepTime = now + seconds(1);
    int count = 0;
    while (std::chrono::steady_clock::now() < (now + timeOutMS)) {
        if (option.sleepms > 0) {
            if (std::chrono::steady_clock::now() >= sleepTime) {
                sleepTime = std::chrono::steady_clock::now() + seconds(1);
                std::this_thread::sleep_for(std::chrono::milliseconds(option.sleepms));
            }
        }
        if (option.mmap) {
            LoopMmap();
        }
        if (option.iowait) {
            LoopIowait();
        }
        if (option.branch) {
            LoopBranch();
        }

        std::default_random_engine rnd;
        int a = rnd();
        int b = rnd();
        int c = rnd();
        a = (a++) * (b++) * (c++);

        if (a == 0) {
            continue;
        }

        if (!option.nonew) {
            auto p = new unsigned int;
            *p = count++;
            delete p;
            p = nullptr;
        }

        if (!option.nofunc) {
            LoopDummy(timeOutMS);
        }
    }
    return;
}

inline void Loop(milliseconds timeOutMS, const Option &option)
{
    printf("loop at %s\n", __func__);
    LoopFunction(timeOutMS, option);
    return;
}

USED_FUNCTION void CallStack10(int currentStack, const Option &option)
{
    if (option.stack > 0) {
        Loop(option.second * MS_DUARTION, option);
    }
}

USED_FUNCTION void CallStack9(int currentStack, const Option &option)
{
    if (option.stack > 0) {
        if (option.dynamicStack) {
            Loop(EACH_STACK_FUN_RUN_TIME, option); // loop 100 ms
        }
        CallStack10(currentStack - 1, option);
    } else {
        Loop(option.second * MS_DUARTION, option);
    }
}

USED_FUNCTION void CallStack8(int currentStack, const Option &option)
{
    if (option.stack > 0) {
        if (option.dynamicStack) {
            Loop(EACH_STACK_FUN_RUN_TIME, option); // loop 100 ms
        }
        CallStack9(currentStack - 1, option);
    } else {
        Loop(option.second * MS_DUARTION, option);
    }
}

USED_FUNCTION void CallStack7(int currentStack, const Option &option)
{
    if (option.stack > 0) {
        if (option.dynamicStack) {
            Loop(EACH_STACK_FUN_RUN_TIME, option); // loop 100 ms
        }
        CallStack8(currentStack - 1, option);
    } else {
        Loop(option.second * MS_DUARTION, option);
    }
}

USED_FUNCTION void CallStack6(int currentStack, const Option &option)
{
    if (option.stack > 0) {
        if (option.dynamicStack) {
            Loop(EACH_STACK_FUN_RUN_TIME, option); // loop 100 ms
        }
        CallStack7(currentStack - 1, option);
    } else {
        Loop(option.second * MS_DUARTION, option);
    }
}

USED_FUNCTION void CallStack5(int currentStack, const Option &option)
{
    if (option.stack > 0) {
        if (option.dynamicStack) {
            Loop(EACH_STACK_FUN_RUN_TIME, option); // loop 100 ms
        }
        CallStack6(currentStack - 1, option);
    } else {
        Loop(option.second * MS_DUARTION, option);
    }
}

USED_FUNCTION void CallStack4(int currentStack, const Option &option)
{
    if (option.stack > 0) {
        if (option.dynamicStack) {
            Loop(EACH_STACK_FUN_RUN_TIME, option); // loop 100 ms
        }
        CallStack5(currentStack - 1, option);
    } else {
        Loop(option.second * MS_DUARTION, option);
    }
}

USED_FUNCTION void CallStack3(int currentStack, const Option &option)
{
    if (option.stack > 0) {
        if (option.dynamicStack) {
            Loop(EACH_STACK_FUN_RUN_TIME, option); // loop 100 ms
        }
        CallStack4(currentStack - 1, option);
    } else {
        Loop(option.second * MS_DUARTION, option);
    }
}

USED_FUNCTION void CallStack2(int currentStack, const Option &option)
{
    if (option.stack > 0) {
        if (option.dynamicStack) {
            Loop(EACH_STACK_FUN_RUN_TIME, option); // loop 100 ms
        }
        CallStack3(currentStack - 1, option);
    } else {
        Loop(option.second * MS_DUARTION, option);
    }
}

USED_FUNCTION void CallStack1(int currentStack, const Option &option)
{
    if (option.stack > 0) {
        if (option.dynamicStack) {
            Loop(EACH_STACK_FUN_RUN_TIME, option); // loop 100 ms
        }
        CallStack2(currentStack - 1, option);
    } else {
        Loop(option.second * MS_DUARTION, option);
    }
}

USED_FUNCTION void CallStack0(int currentStack, const Option &option)
{
    if (option.stack > 0) {
        if (option.dynamicStack) {
            Loop(EACH_STACK_FUN_RUN_TIME, option); // loop 100 ms
        }
        CallStack1(currentStack - 1, option);
    } else {
        Loop(option.second * MS_DUARTION, option);
    }
}

USED_FUNCTION void ExampleThread(const Option &option)
{
    printf("thread %d ++ with %d %d \n", GetTid(), option.second, option.stack);
    CallStack0(option.stack, option);
    printf("thread %d --\n", GetTid());

    return;
}

USED_FUNCTION void RunSampleThread(const Option &option)
{
    printf("run %d threads for %d second with %d stack level\n", option.numThreads, option.second,
           option.stack);
    printf("main thread %d\n", GetTid());

    std::thread threads[option.numThreads];
    for (int count = 0; count < option.numThreads; ++count) {
        threads[count] = std::thread(ExampleThread, option);
    }
    for (int count = 0; count < option.numThreads; ++count) {
        threads[count].join();
    }
    printf("all thread exit\n");
}

void WaitStart()
{
    std::string startArg;
    std::cout << "Please input 'start' to begin the subthread: \n";
    while (true) {
        std::cin >> startArg;
        if (startArg.compare("start") == 0) {
            break;
        } else {
            continue;
        }
    }
}

void Help()
{
    printf("this is a demo test command\n");
    printf("  Use the following commands to simulate different scenarios\n");
    printf("  --help\n");
    printf("    this page\n");
    printf("  --thread <number>\n");
    printf("    setup the thread number, default is 5 second\n");
    printf("  --time <time>\n");
    printf("    setup run sec, default is 36000 second\n");
    printf("  --stack <level>\n");
    printf("    setup stack level, default is 5\n");
    printf("  --nowait\n");
    printf("    setup skip the start, default wait the start\n");
    printf("  --dynamic\n");
    printf("    will run some code in each stack level\n");
    printf("  --mmap\n");
    printf("    will run mmap code in the loop\n");
    printf("  --iowait\n");
    printf("    will run iowait code in the loop\n");
    printf("  --branch\n");
    printf("    will run branch code in the loop\n");
    printf("  --nonew\n");
    printf("    will not new memory in the loop\n");
    printf("  --nofunc\n");
    printf("    will not call dummy func in the loop\n");
    printf("  --boundcpu <cpu>\n");
    printf("    the process will bound to <cpu>\n");
    printf("  --sleep <milliseconds>\n");
    printf("    threads will sleep <milliseconds> per second, default is 0.\n");
}

bool GetIntFromArg(std::vector<std::string> &args, int &value)
{
    if (!args.empty()) {
        if (std::all_of(args.begin()->begin(), args.begin()->end(), ::isdigit)) {
            value = std::stoi(args[0]);
            args.erase(args.begin());
        } else {
            printf("unknown format '%s'\n", args[0].c_str());
            return false;
        }
    }
    return true;
}

bool MatchArgs(std::vector<std::string> &args, const std::string &option)
{
    if (args[0] == option) {
        args.erase(args.begin());
        return true;
    }
    return false;
}
bool GetBoolFromArg(std::vector<std::string> &args, const std::string &option, bool &value)
{
    if (MatchArgs(args, option)) {
        value = true;
        return true;
    } else {
        return false;
    }
}
} // namespace

USED_FUNCTION int main(int argc, char *argv[])
{
    std::vector<std::string> args;
    for (int i = 1; i < argc; i++) {
        args.push_back(argv[i]);
    }
    Option option;

    while (!args.empty()) {
        if (MatchArgs(args, "--help")) {
            Help();
            return 0;
        } else if (MatchArgs(args, "--boundcpu")) {
            if (!GetIntFromArg(args, option.boundCpu)) {
                return -1;
            }
        } else if (MatchArgs(args, "--sleep")) {
            if (!GetIntFromArg(args, option.sleepms)) {
                return -1;
            }
        } else if (MatchArgs(args, "--thread")) {
            if (!GetIntFromArg(args, option.numThreads)) {
                return -1;
            }
        } else if (MatchArgs(args, "--time")) {
            if (!GetIntFromArg(args, option.second)) {
                return -1;
            }
        } else if (MatchArgs(args, "--stack")) {
            if (!GetIntFromArg(args, option.stack)) {
                return -1;
            }
        } else if (GetBoolFromArg(args, "--dynamic", option.dynamicStack)) {
            continue;
        } else if (GetBoolFromArg(args, "--nowait", option.noWait)) {
            continue;
        } else if (GetBoolFromArg(args, "--mmap", option.mmap)) {
            continue;
        } else if (GetBoolFromArg(args, "--iowait", option.iowait)) {
            continue;
        } else if (GetBoolFromArg(args, "--branch", option.branch)) {
            continue;
        } else if (GetBoolFromArg(args, "--nonew", option.nonew)) {
            continue;
        } else if (GetBoolFromArg(args, "--nofunc", option.nofunc)) {
            continue;
        } else {
            printf("unknown format '%s'\n", args[0].c_str());
            return -1;
        }
    }

    if (option.boundCpu > -1) {
        cpu_set_t mask;
        CPU_ZERO(&mask);
        CPU_SET(option.boundCpu, &mask);
        if (sched_setaffinity(0, sizeof(cpu_set_t), &mask) == -1) {
            char errInfo[ERRINFOLEN] = { 0 };
            strerror_r(errno, errInfo, ERRINFOLEN);
            printf("Set CPU(%d) affinity failure, ERROR:%s\n", option.boundCpu, errInfo);
        }
    }
    if (!option.noWait) {
        WaitStart();
    }

    RunSampleThread(option);
    return 0;
}
