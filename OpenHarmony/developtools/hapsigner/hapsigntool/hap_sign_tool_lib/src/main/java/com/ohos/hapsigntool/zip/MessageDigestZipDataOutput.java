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
import java.security.MessageDigest;

/**
 * MessageDigest implementation of ZipDataOutput
 *
 * @since 2021/12/20
 */
public class MessageDigestZipDataOutput implements ZipDataOutput {
    private final MessageDigest[] messageDigests;

    public MessageDigestZipDataOutput(MessageDigest[] digests) {
        messageDigests = digests;
    }

    @Override
    public void write(byte[] buffer, int offset, int length) throws IOException {
        for (MessageDigest messageDigest : messageDigests) {
            messageDigest.update(buffer, offset, length);
        }
    }

    @Override
    public void write(ByteBuffer buffer) throws IOException {
        int position = buffer.position();
        for (MessageDigest messageDigest : messageDigests) {
            buffer.position(position);
            messageDigest.update(buffer);
        }
    }
}
