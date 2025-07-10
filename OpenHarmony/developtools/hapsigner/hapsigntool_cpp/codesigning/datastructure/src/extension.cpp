/*
 * Copyright (c) 2024-2024 Huawei Device Co., Ltd.
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
#include "extension.h"
#include "byte_buffer.h"

namespace OHOS {
namespace SignatureTools {

const int32_t Extension::EXTENSION_HEADER_SIZE = 8;

Extension::Extension()
{
    type = 0;
    size = 0;
}

Extension::Extension(int32_t type, int32_t size)
{
    this->type = type;
    this->size = size;
}

Extension::~Extension()
{
}

int32_t Extension::GetSize()
{
    return Extension::EXTENSION_HEADER_SIZE;
}

bool Extension::IsType(int32_t type)
{
    return this->type == type;
}

void Extension::ToByteArray(std::vector<int8_t>& ret)
{
    std::unique_ptr<ByteBuffer> bf = std::make_unique<ByteBuffer>
        (ByteBuffer(Extension::EXTENSION_HEADER_SIZE));
    bf->PutInt32(type);
    bf->PutInt32(size);
    bf->Flip();
    char dataArr[Extension::EXTENSION_HEADER_SIZE] = { 0 };
    bf->GetData(dataArr, Extension::EXTENSION_HEADER_SIZE);
    ret = std::vector<int8_t>(dataArr, dataArr + Extension::EXTENSION_HEADER_SIZE);
}

}
}