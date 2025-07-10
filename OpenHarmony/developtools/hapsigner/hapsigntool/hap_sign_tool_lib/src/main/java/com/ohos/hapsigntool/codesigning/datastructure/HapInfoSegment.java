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

package com.ohos.hapsigntool.codesigning.datastructure;

import com.ohos.hapsigntool.codesigning.exception.VerifyCodeSignException;
import com.ohos.hapsigntool.codesigning.utils.NumberUtils;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.Locale;

/**
 * Hap info segment
 * <p>
 * Structure
 * <p>
 * 1) u32 magic: magic number
 * <p>
 * 2) SignInfo hapSignInfo: data struct of sign info, refer to SignInfo.java
 *
 * @since 2023/09/08
 */
public class HapInfoSegment {
    private static final int MAGIC_NUM_BYTES = 4;

    /**
     * lower 4 bytes of the MD5 result of string "hap info segment" (C1B5 CC66)
     */
    private static final int MAGIC_NUM = (0xC1B5 << 16) + 0xCC66;

    private int magic = MAGIC_NUM;

    private SignInfo hapSignInfo;

    /**
     * Default constructor of HapInfoSegment
     */
    public HapInfoSegment() {
        this(MAGIC_NUM, new SignInfo(0, 0, 0, null, null));
    }

    /**
     * Default constructor of HapInfoSegment
     *
     * @param magic       magic number
     * @param hapSignInfo hap sign info
     */
    public HapInfoSegment(int magic, SignInfo hapSignInfo) {
        this.magic = magic;
        this.hapSignInfo = hapSignInfo;
    }

    public void setSignInfo(SignInfo signInfo) {
        this.hapSignInfo = signInfo;
    }

    public SignInfo getSignInfo() {
        return hapSignInfo;
    }

    /**
     * Returns byte size of HapInfoSegment
     *
     * @return byte size of HapInfoSegment
     */
    public int size() {
        return MAGIC_NUM_BYTES + hapSignInfo.size();
    }

    /**
     * Converts HapInfoSegment to a newly created byte array
     *
     * @return Byte array representation of HapInfoSegment
     */
    public byte[] toByteArray() {
        byte[] hapSignInfoByteArray = this.hapSignInfo.toByteArray();
        // For now, only hap info segment has a merkle tree extension. So info segment
        // has none extension.
        ByteBuffer bf = ByteBuffer.allocate(MAGIC_NUM_BYTES + hapSignInfoByteArray.length)
            .order(ByteOrder.LITTLE_ENDIAN);
        bf.putInt(magic);
        bf.put(hapSignInfoByteArray);
        return bf.array();
    }

    /**
     * Init the HapInfoSegment by a byte array
     *
     * @param bytes Byte array representation of a HapInfoSegment object
     * @return a newly created HapInfoSegment object
     * @throws VerifyCodeSignException parsing result invalid
     */
    public static HapInfoSegment fromByteArray(byte[] bytes) throws VerifyCodeSignException {
        ByteBuffer bf = ByteBuffer.allocate(bytes.length).order(ByteOrder.LITTLE_ENDIAN);
        bf.put(bytes);
        bf.rewind();
        int inMagic = bf.getInt();
        if (inMagic != MAGIC_NUM) {
            throw new VerifyCodeSignException("Invalid magic number of HapInfoSegment");
        }
        if (bytes.length <= MAGIC_NUM_BYTES) {
            throw new VerifyCodeSignException("Invalid bytes size of HapInfoSegment");
        }
        byte[] hapSignInfoByteArray = new byte[bytes.length - MAGIC_NUM_BYTES];
        bf.get(hapSignInfoByteArray);
        SignInfo inHapSignInfo = SignInfo.fromByteArray(hapSignInfoByteArray);
        if (!NumberUtils.isMultiple4K(inHapSignInfo.getDataSize())) {
            throw new VerifyCodeSignException(
                String.format(Locale.ROOT, "Invalid dataSize number of HapInfoSegment, not a multiple of 4096: %d",
                    inHapSignInfo.getDataSize()));
        }
        if (inHapSignInfo.getExtensionByType(MerkleTreeExtension.MERKLE_TREE_INLINED) == null) {
            throw new VerifyCodeSignException("No merkle tree extension is found in HapInfoSegment");
        }
        return new HapInfoSegment(inMagic, inHapSignInfo);
    }

    /**
     * Return a string representation of the object
     *
     * @return string representation of the object
     */
    public String toString() {
        return String.format(Locale.ROOT, "HapInfoSegment: magic[%d], signInfo[%s]", this.magic,
            this.hapSignInfo.toString());
    }
}
