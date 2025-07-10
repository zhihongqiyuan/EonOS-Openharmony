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
 * ZipDataOutput interface
 *
 * @since 2021/12/20
 */
public interface ZipDataOutput {
    /**
     * Write data into this ZipDataOutput
     *
     * @param buffer data bytes
     * @param offset offset index of data bytes
     * @param length the number of bytes to use, starting at offset index
     * @throws IOException when IO error occurred
     */
    void write(byte[] buffer, int offset, int length) throws IOException;

    /**
     * Write all remaining data in the ByteBuffer into this ZipDataOutput
     *
     * @param buffer data bytes
     * @throws IOException when IO error occurred
     */
    void write(ByteBuffer buffer) throws IOException;
}
