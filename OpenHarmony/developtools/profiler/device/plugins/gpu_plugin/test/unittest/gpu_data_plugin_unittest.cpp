/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
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

#include <gtest/gtest.h>
#include <dlfcn.h>
#include <fstream>
#include "gpu_data_plugin.h"
#include "plugin_module_api.h"

using namespace testing::ext;

namespace {
const std::string DEFAULT_TEST_PATH = "/data/local/tmp/resource";
const std::string SO_PATH = "libgpudataplugin.z.so";
const std::string DEFAULT_BIN_PATH("/data/local/tmp/gpudataplugintest");

std::string g_path;
std::string g_testPath;

#if defined(__LP64__)
const unsigned long long EXPECT_VAL = 12;
const unsigned long long SLEEP_TIME = 2;
constexpr uint32_t BUF_SIZE = 4 * 1024 * 1024;
#endif

struct TestVmstat {
    int64_t pgpgin;
    int64_t pgpgout;
};

class GpuDataPluginTest : public ::testing::Test {
public:
    static void SetUpTestCase() {}

    static void TearDownTestCase()
    {
        if (access(g_testPath.c_str(), F_OK) == 0) {
            std::string str = "rm -rf " + g_testPath;
            system(str.c_str());
        }
    }
};

string Getexepath()
{
    char buf[PATH_MAX] = "";
    std::string path = "/proc/self/exe";
    size_t rslt = readlink(path.c_str(), buf, sizeof(buf));
    if (rslt < 0 || (rslt >= sizeof(buf))) {
        return "";
    }
    buf[rslt] = '\0';
    for (int i = rslt; i >= 0; i--) {
        if (buf[i] == '/') {
            buf[i + 1] = '\0';
            break;
        }
    }
    return buf;
}

std::string GetFullPath(std::string path)
{
    if (path.size() > 0 && path[0] != '/') {
        return Getexepath() + path;
    }
    return path;
}

long WriteFunc(WriterStruct* writer, const void* data, size_t size)
{
    if (writer == nullptr || data == nullptr || size <= 0) {
        return -1;
    }

    return 0;
}

bool FlushFunc(WriterStruct* writer)
{
    if (writer == nullptr) {
        return false;
    }
    return true;
}

RandomWriteCtx* StartReportFunc(WriterStruct* writer)
{
    return nullptr;
}

void FinishReportFunc(WriterStruct* writer, int32_t size)
{
    return;
}

#if defined(__LP64__)
bool PluginGpuInfoStub(GpuDataPlugin& gpuPlugin, WriterStruct *writer)
{
    GpuConfig protoConfig;
    std::vector<uint8_t> configData(protoConfig.ByteSizeLong());
    int ret = protoConfig.SerializeToArray(configData.data(), configData.size());
    if (ret < 0) {
        return false;
    }
    gpuPlugin.SetWriter(writer);
    // start
    ret = gpuPlugin.Start(configData.data(), configData.size());
    if (ret < 0) {
        return false;
    }
    return true;
}
#endif

/**
 * @tc.name: gpu plugin
 * @tc.desc: Test whether the path exists.
 * @tc.type: FUNC
 */
HWTEST_F(GpuDataPluginTest, TestPath, TestSize.Level1)
{
    g_path = GetFullPath(DEFAULT_TEST_PATH);
    g_testPath = g_path;
    EXPECT_NE("", g_path);
    g_path += "/gpustat1.txt";
}

/**
 * @tc.name: gpu plugin
 * @tc.desc: gpu information test for specific path.
 * @tc.type: FUNC
 */
HWTEST_F(GpuDataPluginTest, TestPlugin, TestSize.Level1)
{
#if defined(__LP64__)
    GpuDataPlugin gpuPlugin;
    WriterStruct writer = {WriteFunc, FlushFunc, StartReportFunc, FinishReportFunc};
    EXPECT_TRUE(PluginGpuInfoStub(gpuPlugin, &writer));
    sleep(SLEEP_TIME);
    EXPECT_EQ(gpuPlugin.Stop(), 0);
    std::vector<std::pair<uint64_t, int>> gpuDataVect = {{1000, 32}, {1001, 43}, {1002, 50}};
    gpuPlugin.resultWriter_->isProtobufSerialize = true;
    gpuPlugin.FlushGpuData(gpuDataVect);
    EXPECT_GT(gpuPlugin.buffer_.size(), 0);
    gpuPlugin.resultWriter_->isProtobufSerialize = false;
    gpuPlugin.FlushGpuData(gpuDataVect);
    EXPECT_GT(gpuPlugin.GetBootTime(), 0);
#endif
}

/**
 * @tc.name: gpu plugin
 * @tc.desc: gpu information test read gpu data
 * @tc.type: FUNC
 */
HWTEST_F(GpuDataPluginTest, TestReadGpuData, TestSize.Level1)
{
#if defined(__LP64__)
    GpuDataPlugin gpuPlugin;
    WriterStruct writer = {WriteFunc, FlushFunc};
    gpuPlugin.SetWriter(&writer);
    gpuPlugin.running_ = true;
    gpuPlugin.resultWriter_->isProtobufSerialize = true;
    gpuPlugin.file_.open(g_path);
    EXPECT_EQ(gpuPlugin.ReadFile(), EXPECT_VAL);
    gpuPlugin.file_.close();
    EXPECT_TRUE(PluginGpuInfoStub(gpuPlugin, &writer));
    sleep(SLEEP_TIME);
    EXPECT_GT(gpuPlugin.buffer_.size(), 0);
    EXPECT_EQ(gpuPlugin.Stop(), 0);
#endif
}

/**
 * @tc.name: gpu plugin
 * @tc.desc: gpu plugin registration test.
 * @tc.type: FUNC
 */
HWTEST_F(GpuDataPluginTest, TestPluginRegister, TestSize.Level1)
{
#if defined(__LP64__)
    void* handle = dlopen(SO_PATH.c_str(), RTLD_LAZY);
    ASSERT_NE(handle, nullptr);
    PluginModuleStruct* gpuPlugin = (PluginModuleStruct*)dlsym(handle, "g_pluginModule");
    ASSERT_NE(gpuPlugin, nullptr);
    EXPECT_STREQ(gpuPlugin->name, "gpu-plugin");
    EXPECT_EQ(gpuPlugin->resultBufferSizeHint, BUF_SIZE);

    // Serialize config
    GpuConfig protoConfig;
    int configLength = protoConfig.ByteSizeLong();
    ASSERT_EQ(configLength, 0);
    std::vector<uint8_t> configBuffer(configLength);
    EXPECT_TRUE(protoConfig.SerializeToArray(configBuffer.data(), configLength));
    // run plugin
    WriterStruct writer = {WriteFunc, FlushFunc};
    std::vector<uint8_t> dataBuffer(gpuPlugin->resultBufferSizeHint);
    EXPECT_EQ(gpuPlugin->callbacks->onRegisterWriterStruct(&writer), 0);
    EXPECT_EQ(gpuPlugin->callbacks->onPluginSessionStart(configBuffer.data(), configLength), RET_SUCC);
    EXPECT_EQ(gpuPlugin->callbacks->onPluginSessionStop(), RET_SUCC);

    // 反序列化失败导致的start失败
    configLength++;
    std::vector<uint8_t> configBuffer2(configLength);
    EXPECT_TRUE(protoConfig.SerializeToArray(configBuffer2.data(), configLength));
    EXPECT_EQ(gpuPlugin->callbacks->onPluginSessionStart(configBuffer2.data(), configLength+1), RET_FAIL);
#endif
}

/**
 * @tc.name: gpu plugin
 * @tc.desc: test gpu plugin read array data
 * @tc.type: FUNC
 */
HWTEST_F(GpuDataPluginTest, TestPluginReadArrayData, TestSize.Level1)
{
#if defined(__LP64__)
    void* handle = dlopen(SO_PATH.c_str(), RTLD_LAZY);
    ASSERT_NE(handle, nullptr);
    PluginModuleStruct* gpuPlugin = (PluginModuleStruct*)dlsym(handle, "g_pluginModule");
    ASSERT_NE(gpuPlugin, nullptr);
    EXPECT_STREQ(gpuPlugin->name, "gpu-plugin");
    EXPECT_EQ(gpuPlugin->resultBufferSizeHint, BUF_SIZE);

    // Serialize config
    GpuConfig protoConfig;
    protoConfig.set_pid("11001");
    protoConfig.set_report_gpu_data_array(true);
    int configLength = protoConfig.ByteSizeLong();
    ASSERT_NE(configLength, 0);
    std::vector<uint8_t> configBuffer(configLength);
    EXPECT_TRUE(protoConfig.SerializeToArray(configBuffer.data(), configLength));
    // run plugin
    WriterStruct writer = {WriteFunc, FlushFunc};
    std::vector<uint8_t> dataBuffer(gpuPlugin->resultBufferSizeHint);
    EXPECT_EQ(gpuPlugin->callbacks->onRegisterWriterStruct(&writer), 0);
    EXPECT_EQ(gpuPlugin->callbacks->onPluginSessionStart(configBuffer.data(), configLength), RET_SUCC);
    EXPECT_EQ(gpuPlugin->callbacks->onPluginSessionStop(), RET_SUCC);
#endif
}

} // namespace