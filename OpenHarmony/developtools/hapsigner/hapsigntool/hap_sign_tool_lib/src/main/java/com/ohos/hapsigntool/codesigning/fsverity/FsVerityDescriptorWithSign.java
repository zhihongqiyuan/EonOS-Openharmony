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

import com.ohos.hapsigntool.codesigning.exception.FsVerityDigestException;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

/**
 * fsverity descriptor and signature
 * 1) u32 type: 0x1 fsverity descriptor
 * 2) u32 length: fsverity descriptor size
 * 3) FsVerityDescriptor: fsVerityDescriptor
 * 4) u8[] signature: signature after signing the data in byte array representation
 *
 * @since 2023/09/08
 */
public class FsVerityDescriptorWithSign {
    private int type = FsVerityDescriptor.FS_VERITY_DESCRIPTOR_TYPE;

    private int length;

    private FsVerityDescriptor fsVerityDescriptor;

    private byte[] signature = new byte[0];

    /**
     * Constructor of FsVerityDescriptorWithSign
     *
     * @param fsVerityDescriptor fs-verity descriptor
     * @param signature          signature
     */
    public FsVerityDescriptorWithSign(FsVerityDescriptor fsVerityDescriptor, byte[] signature) {
        this.fsVerityDescriptor = fsVerityDescriptor;
        if (signature != null) {
            this.signature = signature;
        }
        length = FsVerityDescriptor.DESCRIPTOR_SIZE + this.signature.length;
    }

    /**
     * Constructor of FsVerityDescriptorWithSign
     *
     * @param type               fs-verity descriptor type
     * @param length             fs-verity descriptor with signature size
     * @param fsVerityDescriptor fs-verity descriptor
     * @param signature signature
     */
    public FsVerityDescriptorWithSign(int type, int length, FsVerityDescriptor fsVerityDescriptor, byte[] signature) {
        this.type = type;
        this.length = length;
        this.fsVerityDescriptor = fsVerityDescriptor;
        this.signature = signature;
    }

    /**
     * Returns byte size of FsVerityDescriptorWithSign
     *
     * @return byte size of FsVerityDescriptorWithSign
     */
    public int size() {
        return Integer.BYTES * 2 + FsVerityDescriptor.DESCRIPTOR_SIZE + signature.length;
    }

    /**
     * Converts FsVerityDescriptorWithSign to a newly created byte array
     *
     * @return Byte array representation of FsVerityDescriptorWithSign
     * @throws FsVerityDigestException if error
     */
    public byte[] toByteArray() throws FsVerityDigestException {
        ByteBuffer buffer = ByteBuffer.allocate(size()).order(ByteOrder.LITTLE_ENDIAN);
        buffer.putInt(type);
        buffer.putInt(length);
        buffer.put(fsVerityDescriptor.toByteArray());
        buffer.put(signature);
        return buffer.array();
    }

    public FsVerityDescriptor getFsVerityDescriptor() {
        return fsVerityDescriptor;
    }

    public byte[] getSignature() {
        return signature;
    }
}
