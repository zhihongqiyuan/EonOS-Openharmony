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

package com.ohos.hapsigntool.zip;

import java.io.IOException;
import java.nio.ByteBuffer;

/**
 * ZipDataInput interface
 *
 * @since 2021/12/20
 */
public interface ZipDataInput {
    /**
     * Get how many bytes are contained in this data input.
     *
     * @return this data input size
     */
    long size();

    /**
     * Copy the specified data block into the destination ZipDataOutput
     *
     * @param offset offset index at the ZipDataInput
     * @param size size of the data block
     * @param output the destination ZipDataOutput
     * @throws IOException when IO error occurred
     */
    void copyTo(long offset, long size, ZipDataOutput output) throws IOException;

    /**
     * Copy the specified data block into the destination ZipDataOutput
     *
     * @param offset offset index at the ZipDataInput
     * @param size size of the data block
     * @param buffer the destination ZipDataOutput
     * @throws IOException when IO error occurred
     */
    void copyTo(long offset, int size, ByteBuffer buffer) throws IOException;

    /**
     * Create a ByteBuffer which contain the specified data block from this ZipDataInput
     *
     * @param offset offset index at the ZipDataInput
     * @param size size of the data block
     * @return a ByteBuffer
     * @throws IOException when IO error occurred
     */
    ByteBuffer createByteBuffer(long offset, int size) throws IOException;

    /**
     * Create a new ZipDataInput whose content is shared by this ZipDataInput
     *
     * @param offset offset index at the ZipDataInput
     * @param size size of the data block
     * @return new ZipDataInput
     */
    ZipDataInput slice(long offset, long size);
}
