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

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>

#include "random_access_file.h"
#include "random_access_file_input.h"
#include "random_access_file_output.h"

namespace OHOS {
namespace SignatureTools {

const char* UNSIGNED_HAP_FILE_PATH = "./zip/unsigned.hap";
void RandomAccessFileReadFileFunc(const uint8_t* data, size_t size)
{
    auto outputHap = std::make_shared<RandomAccessFile>();
    if (!outputHap->Init(UNSIGNED_HAP_FILE_PATH)) {
        return;
    }
    std::string buf(1, 0);
    outputHap->ReadFileFullyFromOffset(buf.data(), 0, 1);
}

void RandomAccessFileInputConstructor(const uint8_t* data, size_t size)
{
    RandomAccessFile file;
    if (!file.Init(UNSIGNED_HAP_FILE_PATH)) {
        return;
    }
    int64_t fileLength = file.GetLength();
    RandomAccessFileInput fileInput(file, 0, fileLength);
    fileInput.Size();
}

void RandomAccessFileOutputConstructor(const uint8_t* data, size_t size)
{
    RandomAccessFile file;
    if (!file.Init(UNSIGNED_HAP_FILE_PATH)) {
        return;
    }
    RandomAccessFileOutput fileOutput(&file);
}

void DoSomethingInterestingWithMyAPI(const uint8_t* data, size_t size)
{
    if (data == nullptr || size == 0) {
        return;
    }

    RandomAccessFileReadFileFunc(data, size);
    RandomAccessFileInputConstructor(data, size);
    RandomAccessFileOutputConstructor(data, size);
}
} // namespace SignatureTools
} // namespace OHOS

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    /* Run your code on data */
    (void)rename("./zip/unsigned.txt", OHOS::SignatureTools::UNSIGNED_HAP_FILE_PATH);
    sync();
    OHOS::SignatureTools::DoSomethingInterestingWithMyAPI(data, size);
    return 0;
}