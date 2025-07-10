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
#ifndef HIPERF_RING_BUFFER_H
#define HIPERF_RING_BUFFER_H
#include <memory>

namespace OHOS {
namespace Developtools {
namespace HiPerf {
class RingBuffer {
public:
    // little endian, perf_event_header.type is less than 0xff, so set it
    static constexpr uint8_t MARGIN_BYTE = 0xFF;

    explicit RingBuffer(size_t size);
    ~RingBuffer();
    // get size of the writable space
    size_t GetFreeSize() const;

    // before writing data to rbuff, alloc space first
    uint8_t *AllocForWrite(size_t writeSize);
    // after writing data, move head pointer
    void EndWrite();
    // get data from buff, return nullptr if no readable data
    uint8_t *GetReadData();
    // after reading, move tail pointer
    void EndRead();

private:
    std::unique_ptr<uint8_t[]> buf_ = nullptr;
    const size_t size_;
    std::atomic_size_t head_ = 0; // write after this, always increase
    std::atomic_size_t tail_ = 0; // read from this, always increase
    size_t writeSize_ = 0;
    size_t readSize_ = 0;
};
} // namespace HiPerf
} // namespace Developtools
} // namespace OHOS
#endif // HIPERF_RING_BUFFER_H
