/*
 * Copyright (c) 2023-2023 Huawei Device Co., Ltd.
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

package com.ohos.hapsigntool.codesigning.fsverity;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.charset.StandardCharsets;

/**
 * Format of FsVerity digest
 * int8[8] magic              "FSVerity"
 * le16    digestAlgorithm    sha256 = 1, sha512 = 2
 * le16    digestSize
 * uint8[] digest
 *
 * @since 2023/06/05
 */
public class FsVerityDigest {
    private static final String FSVERITY_DIGEST_MAGIC = "FSVerity";

    private static final int DIGEST_HEADER_SIZE = 12;

    /**
     * Get formatted FsVerity digest
     *
     * @param algoID hash algorithm id
     * @param digest raw digest computed from input
     * @return formatted FsVerity digest bytes
     */
    public static byte[] getFsVerityDigest(byte algoID, byte[] digest) {
        int size = DIGEST_HEADER_SIZE + digest.length;
        ByteBuffer buffer = ByteBuffer.allocate(size).order(ByteOrder.LITTLE_ENDIAN);
        buffer.put(FSVERITY_DIGEST_MAGIC.getBytes(StandardCharsets.UTF_8));
        buffer.putShort(algoID);
        buffer.putShort((short) digest.length);
        buffer.put(digest);
        return buffer.array();
    }
}
