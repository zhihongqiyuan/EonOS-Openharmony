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
#ifndef SIGNATRUETOOLS_BYTEBUFFER_H
#define SIGNATRUETOOLS_BYTEBUFFER_H

#include <memory>
#include <string>

#include "export_define.h"

namespace OHOS {
namespace SignatureTools {

enum ReadFileErrorCode {
    DEST_BUFFER_IS_NULL = -1,
    FILE_IS_CLOSE = -2,
    MMAP_COPY_FAILED = -3,
    READ_OFFSET_OUT_OF_RANGE = -4,
    MMAP_FAILED = -5,
    MMAP_PARAM_INVALID = -6,
};
/*
* The underlying storage structure of ByteBuffer is an array, and all operations are based on that array.
* ByteBuffer is in its initial state and also in a write state after creation
*
* 1. Several position related member variables in the ByteBuffer class
* Mark: Read the starting position of the data to facilitate subsequent rollback to that position
* Position: Where to start reading or writing
* Limit: When reading the status, it indicates how much data is actually stored;
* Equal to capacity when writing status
* Capacity: represents the capacity of ByteBuffer, which is the maximum number of bytes that can be stored
*
* Bool hasRemaining(); Is there any data between position and limit
* Uint32_t remaining() const; Return the number of bytes between position and limit
*
* 2. Set ByteBuffer from writer state to read state: limit is set to position value, position is set to zero
* Void flip();
*
* 3. If you want to write or overwrite again, you can call clear() or compact()
* Void clear()// Reset to initial state: mark=-1, position=0, limit=capacity
* Void compact()// Copy the data between position and limit to the starting position at index 0,
* then move the position to the next position of this data segment and set the limit value to capacity
*/
class ByteBuffer {
public:
    DLL_EXPORT ByteBuffer();
    DLL_EXPORT explicit ByteBuffer(int32_t bufferCapacity);
    DLL_EXPORT ByteBuffer(const char* arr, int32_t length);
    DLL_EXPORT ByteBuffer(const ByteBuffer& other);
    DLL_EXPORT ~ByteBuffer();
    DLL_EXPORT ByteBuffer& operator=(const ByteBuffer& other);
    DLL_EXPORT bool GetInt64(int64_t& value);
    DLL_EXPORT bool GetInt64(int32_t index, int64_t& value);
    DLL_EXPORT bool GetUInt32(uint32_t& value);
    DLL_EXPORT bool GetUInt32(int32_t index, uint32_t& value);
    DLL_EXPORT bool GetInt32(int32_t& value);
    DLL_EXPORT bool GetInt32(int32_t index, int32_t& value);
    DLL_EXPORT bool GetUInt16(uint16_t& value);
    DLL_EXPORT bool GetUInt16(int32_t index, uint16_t& value);
    DLL_EXPORT bool GetInt16(int16_t& value);
    DLL_EXPORT bool GetInt16(int32_t index, int16_t& value);
    DLL_EXPORT bool GetUInt8(uint8_t& value);
    DLL_EXPORT bool GetUInt8(int32_t index, uint8_t& value);
    DLL_EXPORT bool GetInt8(int8_t& value);
    DLL_EXPORT bool GetInt8(int32_t index, int8_t& value);
    DLL_EXPORT void PutUInt8(uint8_t value);
    DLL_EXPORT void PutUInt16(uint16_t value);
    DLL_EXPORT void PutUInt32(uint32_t value);
    DLL_EXPORT void PutInt16(int16_t value);
    DLL_EXPORT void PutInt16(int32_t offset, int16_t value);
    DLL_EXPORT void PutInt32(int32_t value);
    DLL_EXPORT void PutInt32(int32_t offset, int32_t value);
    DLL_EXPORT void PutInt64(int64_t value);
    DLL_EXPORT void PutByte(int32_t offset, char value);
    DLL_EXPORT void PutData(const char data[], int32_t len);
    DLL_EXPORT void PutData(int8_t data[], int32_t len);
    DLL_EXPORT void PutData(int32_t offset, const char data[], int32_t len);
    DLL_EXPORT void PutData(int32_t offset, const int8_t data[], int32_t len);
    DLL_EXPORT void PutData(int32_t offset, const char data[], int32_t len, int32_t type);
    DLL_EXPORT void PutByte(char value);
    DLL_EXPORT void Put(const ByteBuffer& byteBuffer);
    DLL_EXPORT void GetData(char data[], uint32_t len);
    DLL_EXPORT void GetData(int32_t offset, int8_t data[], uint32_t len);
    DLL_EXPORT void GetByte(int8_t data[], int32_t len);
    DLL_EXPORT void ClearData();
    DLL_EXPORT int32_t GetCapacity() const;
    DLL_EXPORT int32_t GetPosition() const;
    DLL_EXPORT int32_t GetLimit() const;
    DLL_EXPORT const char* GetBufferPtr() const;
    DLL_EXPORT void SetPosition(int32_t pos);
    DLL_EXPORT void SetLimit(int32_t lim);
    DLL_EXPORT void SetCapacity(int32_t cap);
    DLL_EXPORT ByteBuffer& Slice();
    DLL_EXPORT ByteBuffer& slice_for_codesigning();
    DLL_EXPORT ByteBuffer* Duplicate();
    DLL_EXPORT int32_t Remaining() const;
    DLL_EXPORT bool HasRemaining() const;
    DLL_EXPORT void Clear();
    DLL_EXPORT ByteBuffer& Flip();
    // to the beginning of the cache area; Switch to write state
    DLL_EXPORT bool IsEqual(const ByteBuffer& other);
    DLL_EXPORT bool IsEqual(const std::string& other);
    DLL_EXPORT void Rewind();
    DLL_EXPORT ByteBuffer& RewindHap();
    DLL_EXPORT std::string ToString();

private:
    void Init(int32_t bufferCapacity);
    bool CheckInputForGettingData(int32_t index, int32_t dataLen);

private:
    static const int32_t MAX_PRINT_LENGTH;
    static const int32_t HEX_PRINT_LENGTH;
    std::shared_ptr<char> buffer;
    int32_t position = 0;
    int32_t limit = 0;
    int32_t capacity = 0;
};
} // namespace SignatureTools
} // namespace OHOS
#endif // SIGNATRUETOOLS_BYTEBUFFER_H
