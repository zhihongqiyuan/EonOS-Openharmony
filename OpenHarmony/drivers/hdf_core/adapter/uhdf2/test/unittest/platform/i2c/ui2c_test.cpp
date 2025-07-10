/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#include <ctime>
#include <datetime_ex.h>
#include <gtest/gtest.h>
#include <iostream>
#include <thread>

#include "i2c_if.h"

#define HDF_LOG_TAG dmabuf_test

using namespace testing::ext;

namespace OHOS {
class I2cTest : public testing::Test {
public:
    static void SetUpTestCase() {}
    static void TearDownTestCase() {}
    void SetUp() {};
    void TearDown() {};
};

static constexpr int16_t DUMMY_I2C_NUM = 15;
static constexpr uint16_t I2C_TRANS_SIZE = 8;
static constexpr size_t MB_SIZE = 1024 * 1024; // 1MB

HWTEST_F(I2cTest, DummyEchoTest001, TestSize.Level1)
{
    DevHandle handle = I2cOpen(DUMMY_I2C_NUM);
    ASSERT_NE(handle, nullptr);
    I2cClose(handle);
}

HWTEST_F(I2cTest, DummyEchoTest002, TestSize.Level1)
{
    DevHandle handle = I2cOpen(DUMMY_I2C_NUM);
    ASSERT_NE(handle, nullptr);

    uint8_t buf[I2C_TRANS_SIZE] = {0};
    struct I2cMsg i2cMsg = {
        .addr = 0x0,
        .buf = buf,
        .len = I2C_TRANS_SIZE,
        .flags = I2C_FLAG_READ,
    };

    struct timespec startTime;
    clock_gettime(CLOCK_MONOTONIC, &startTime);

    for (size_t i = 0; i < MB_SIZE / I2C_TRANS_SIZE; i++) {
        int32_t ret = I2cTransfer(handle, &i2cMsg, 1);
        ASSERT_EQ(ret, 1);
    }

    struct timespec endTime;
    clock_gettime(CLOCK_MONOTONIC, &endTime);

    uint64_t nanoSecUsed = SecToNanosec(endTime.tv_sec - startTime.tv_sec) + endTime.tv_nsec - startTime.tv_nsec;
    std::cout << "i2c send 1MB used nano sec: " << nanoSecUsed << std::endl;
    I2cClose(handle);
}
} // namespace OHOS
