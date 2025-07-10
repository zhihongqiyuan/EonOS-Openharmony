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

#include <gtest/gtest.h>

#include "../hapVerify/test_const.h"
#include "byte_buffer.h"

namespace OHOS {
namespace SignatureTools {
class ByteBufferTest : public testing::Test {
public:
    static void SetUpTestCase(void)
    {
    };
    static void TearDownTestCase(void)
    {
    };
    void SetUp()
    {
    };
    void TearDown()
    {
    };
};

/**
 * @tc.name: Test ByteBuffer Constructor and overload function.
 * @tc.desc: The static function will return an object of ByteBuffer;
 * @tc.type: FUNC
 */
HWTEST_F(ByteBufferTest, ByteBufferConstructorTest001, testing::ext::TestSize.Level1)
{
    ByteBuffer buffer1;
    ByteBuffer buffer2(buffer1);
    bool judge1 = (buffer2.GetCapacity() == buffer1.GetCapacity()) &&
        (buffer2.GetPosition() == buffer1.GetPosition()) &&
        (buffer2.GetLimit() == buffer1.GetLimit()) &&
        (buffer2.GetBufferPtr() == nullptr);
    ASSERT_TRUE(judge1);

    ByteBuffer buffer3(1);
    ByteBuffer buffer4(buffer3);
    bool judge2 = (buffer4.GetCapacity() == buffer3.GetCapacity()) &&
        (buffer4.GetPosition() == buffer3.GetPosition()) &&
        (buffer4.GetLimit() == buffer3.GetLimit()) &&
        (buffer4.GetBufferPtr() != nullptr);
    ASSERT_TRUE(judge2);
}

/**
 * @tc.name: Test ByteBuffer IsEqual.
 * @tc.desc: Test function of ByteBuffer::IsEqual() interface.
 * @tc.type: FUNC
 */
HWTEST_F(ByteBufferTest, ByteBufferIsEqualTest001, testing::ext::TestSize.Level1)
{
    ByteBuffer buffer1(2);
    buffer1.PutInt32(0, 10);
    ByteBuffer buffer2(1);
    buffer2.PutInt32(0, 1);
    buffer1 = buffer2;
    ASSERT_TRUE(buffer1.IsEqual(buffer2));

    ByteBuffer buffer3;
    ByteBuffer buffer4;
    buffer4 = buffer3;
    ASSERT_FALSE(buffer4.IsEqual(buffer3));

    ASSERT_FALSE(buffer1.IsEqual(buffer3));

    ASSERT_FALSE(buffer3.IsEqual(buffer1));

    ByteBuffer buffer5(1);
    ByteBuffer buffer6(3);
    ASSERT_FALSE(buffer5.IsEqual(buffer6));

    std::string str{ "abc" };
    ASSERT_FALSE(buffer5.IsEqual(str));

    buffer6.PutByte('a');
    buffer6.PutByte('b');
    buffer6.PutByte('c');

    ASSERT_TRUE(buffer6.IsEqual(str));
}

/**
 * @tc.name: Test ByteBuffer IsEqual.
 * @tc.desc: Test function of ByteBuffer's PutGetData interface.
 * @tc.type: FUNC
 */
HWTEST_F(ByteBufferTest, PutGetDataTest001, testing::ext::TestSize.Level1)
{
    ByteBuffer buffer(1);
    uint32_t v1;
    ASSERT_FALSE(buffer.GetUInt32(v1));

    uint16_t v2;
    ASSERT_FALSE(buffer.GetUInt16(v2));

    int16_t v3;
    ASSERT_FALSE(buffer.GetInt16(v3));

    uint8_t v4 = 1;
    buffer.PutUInt8(v4);
    uint8_t v5 = 0;
    buffer.PutUInt8(v5);
    ASSERT_FALSE(buffer.GetUInt8(v5));

    int8_t v6;
    ASSERT_FALSE(buffer.GetInt8(v6));


    int16_t v7 = 1;
    int32_t v8 = 1;
    int64_t v9 = 1;
    uint8_t v10 = 1;
    buffer.SetPosition(1);
    buffer.PutInt16(v7);
    buffer.PutInt32(v8);
    buffer.PutInt64(v9);
    buffer.PutUInt8(v10);
    buffer.PutData("a", 2);

    buffer.SetPosition(0);
    const char* dummy = nullptr;
    buffer.PutData(dummy, 1);
    buffer.PutData("a", -1);
    int8_t* data = nullptr;
    buffer.PutData(data, 1);
    buffer.PutInt16(1);
    uint8_t v11;
    buffer.GetUInt8(v11);
    buffer.GetUInt8(0, v11);
    buffer.ClearData();

    ByteBuffer empty;
    empty.PutByte(1);
    empty.PutInt16(1);
    empty.PutInt32(1);
    empty.PutInt64(1);
    empty.PutUInt8(1);
    empty.PutData("a", 2);
    empty.PutData(data, 1);
    empty.ClearData();
}

/**
 * @tc.name: Test a ByteBuffer object's operation of GetInt and Put
 * @tc.desc: The static function will return data from ByteBuffer's buffer
 * @tc.type: FUNC
 */
HWTEST_F(ByteBufferTest, GetIntAndPutOperation001, testing::ext::TestSize.Level1)
{
    /*
     * @tc.steps: step1. Create an empty buffer and get data from it.
     * @tc.expected: step1. The return result is false.
     */
    ByteBuffer emptyBuffer;
    int32_t dataInt32;
    ASSERT_FALSE(emptyBuffer.GetInt32(dataInt32));
    int64_t dataInt64;
    ASSERT_FALSE(emptyBuffer.GetInt64(dataInt64));
    unsigned short dataUInt16;
    ASSERT_FALSE(emptyBuffer.GetUInt16(0, dataUInt16));
    /*
     * @tc.steps: step2. Create a ByteBuffer with one byte's buffer and get data from second byte.
     * @tc.expected: step2. The return result is false.
     */
    ByteBuffer testBuffer(1);
    char testChar = TEST_HAPBYTEBUFFER_CHAR_DATA;
    testBuffer.PutData(0, &testChar, sizeof(testChar));
    uint32_t dataUInt32;
    ASSERT_FALSE(testBuffer.GetUInt32(1, dataUInt32));
    ASSERT_FALSE(testBuffer.GetInt32(1, dataInt32));
    ASSERT_FALSE(testBuffer.GetInt64(1, dataInt64));
    ASSERT_FALSE(testBuffer.GetUInt16(1, dataUInt16));
    /*
     * @tc.steps: step3. Get data from negative position.
     * @tc.expected: step3. The return result is false.
     */
    ASSERT_FALSE(testBuffer.GetInt64(TEST_HAPBYTEBUFFER_INVALID_INDEX, dataInt64));
    /*
     * @tc.steps: step4. Put data to buffer and get data from it.
     * @tc.expected: step4. The return data is same as which we put.
     */
    ByteBuffer testBuffer2(TEST_HAPBYTEBUFFER_LENGTH);
    testBuffer2.PutByte(0, testChar);
    /*
     * @tc.steps: step5. Put data to buffer with type and get data from it.
     * @tc.expected: step5. The return data is same as which we put.
     */
    ByteBuffer testBuffer3(TEST_HAPBYTEBUFFER_LENGTH);
    testBuffer3.PutData(0, &testChar, sizeof(testChar), 1);
    testBuffer3.PutData(1, nullptr, 0, 1);

    ByteBuffer testBuffer4;
    testBuffer4.PutData(-1, &testChar, sizeof(testChar), 1);
}

/**
 * @tc.name: Test ByteBuffer function of slice
 * @tc.desc: The static function will get an object after slice and detect it is right;
 * @tc.type: FUNC
 */
HWTEST_F(ByteBufferTest, Slice001, testing::ext::TestSize.Level1)
{
    /*
     * @tc.steps: step1. Set a fixed length buffer.
     * @tc.expected: step1. The return is same as value is set.
     */
    ByteBuffer buffer1(TEST_HAPBYTEBUFFER_LENGTH_2);
    buffer1.SetCapacity(TEST_HAPBYTEBUFFER_LENGTH);
    ASSERT_TRUE(buffer1.GetCapacity() == TEST_HAPBYTEBUFFER_LENGTH);
    /*
     * @tc.steps: step2. Slice buffer.
     * @tc.expected: step2. The return is the target length after slice.
     */
    buffer1.PutInt32(0, TEST_HAPBYTEBUFFER_INT32_DATA);
    buffer1.PutInt32(sizeof(int), TEST_HAPBYTEBUFFER_INT32_DATA_2);
    buffer1.SetPosition(sizeof(int));
    buffer1.SetLimit(sizeof(int) + sizeof(int));
    buffer1.Slice();
    ASSERT_TRUE(buffer1.Remaining() == sizeof(int));
    /*
     * @tc.steps: step3. Get int32 from buffer1.
     * @tc.expected: step3. The return result is equal to TEST_HAPBYTEBUFFER_INT32_DATA_2.
     */
    int32_t testDataInt32;
    ASSERT_TRUE(buffer1.GetInt32(testDataInt32));
    ASSERT_EQ(testDataInt32, TEST_HAPBYTEBUFFER_INT32_DATA_2);
    /*
     * @tc.steps: step4. Slice continue, reset position and limit, and calculate if buffer has remain.
     * @tc.expected: step4. The return result is true.
     */
    buffer1.Slice();
    buffer1.Clear();
    ASSERT_TRUE(buffer1.HasRemaining());
}

/**
 * @tc.name: Test ByteBuffer function of IsEqual001
 * @tc.desc: The static function will return two object whether is equal.
 * @tc.type: FUNC
 */
HWTEST_F(ByteBufferTest, IsEqual001, testing::ext::TestSize.Level1)
{
    /*
     * @tc.steps: step1. Create a buffer, and compare it with itself.
     * @tc.expected: step1. The return result is true.
     */
    char testChar[TEST_HAPBYTEBUFFER_LENGTH] = "Hello, world!!";
    ByteBuffer buffer1(TEST_HAPBYTEBUFFER_LENGTH);
    buffer1.PutData(0, testChar, TEST_HAPBYTEBUFFER_LENGTH);
    ASSERT_TRUE(buffer1.IsEqual(buffer1));
    /*
     * @tc.steps: step2. Create another buffer and compare it with buffer1.
     * @tc.expected: step2. The return result is false.
     */
    ByteBuffer buffer2;
    ASSERT_FALSE(buffer1.IsEqual(buffer2));
    /*
     * @tc.steps: step3. Set length of buffer2 same as buffer1.
     * @tc.expected: step3. The return result is false.
     */
    buffer2.SetCapacity(TEST_HAPBYTEBUFFER_LENGTH);
    ASSERT_FALSE(buffer1.IsEqual(buffer2));
    /*
     * @tc.steps: step4. Use copy constructor to create an buffer3, and compare it with buffer1.
     * @tc.expected: step4. The return result is true.
     */
    ByteBuffer buffer3(buffer1);
    ASSERT_TRUE(buffer1.IsEqual(buffer3));
}

/**
 * @tc.name: Test ByteBuffer function of IsEqual002
 * @tc.desc: The static function will return whether the value in buffer is equal to a string.
 * @tc.type: FUNC
 */
HWTEST_F(ByteBufferTest, IsEqual002, testing::ext::TestSize.Level1)
{
    /*
     * @tc.steps: step1. Create a buffer and string, and compare.
     * @tc.expected: step1. The return is false.
     */
    std::string testStr = "Hello, world!!!";
    ByteBuffer buffer1;
    const ByteBuffer& buffer2 = buffer1;
    buffer1 = buffer2;
    ASSERT_FALSE(buffer1.IsEqual(testStr));
    /*
     * @tc.steps: step2. Set length of buffer1 same as string.
     * @tc.expected: step2. The return is false.
     */
    buffer1.SetCapacity(static_cast<int>(testStr.size()));
    ASSERT_FALSE(buffer1.IsEqual(testStr));
    /*
     * @tc.steps: step3. Put string to buffer1 and compare.
     * @tc.expected: step3. The return is true.
     */
    for (int32_t i = 0; i < static_cast<int>(testStr.size()); i++) {
        buffer1.PutByte(i, testStr[i]);
    }
    ASSERT_TRUE(buffer1.IsEqual(testStr));
}

} // namespace SignatureTools
} // namespace OHOS
