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

package com.ohos.hapsigntool.codesigning.datastructure;

import com.ohos.hapsigntool.codesigning.exception.CodeSignErrMsg;
import com.ohos.hapsigntool.codesigning.exception.PageInfoException;
import com.ohos.hapsigntool.codesigning.exception.VerifyCodeSignException;
import com.ohos.hapsigntool.codesigning.utils.NumberUtils;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.Locale;

/**
 * Pages info extension is a type of Extension to store bitmap file's information, i.e. size and offset, ect.
 * <p>
 * structure
 * <p>
 * 1) u32 type 0x2
 * <p>
 * 2) u64 mapOffset: offset of the bitmap by the start of the file.
 * <p>
 * 3) u64 mapSize: the bit size of bitmap.
 * <p>
 * 4) u8  unitSize: unit size corresponding to each page, default 4 .
 * <p>
 * 5) u8[3] reserved:
 * <p>
 * 6) u32 signSize: signature size
 * <p>
 * 7) u8[] signature: signature of the data
 *
 * @since 2024/07/01
 */
public class PageInfoExtension extends Extension {
    /**
     * Type of PageInfoExtension
     */
    public static final int PAGE_INFO_INLINED = 0x2;

    /**
     * Byte size of PageInfoExtension
     */
    public static final int PAGE_INFO_EXTENSION_DATA_SIZE_WITHOUT_SIGN = 24;

    /**
     * default unit size
     */
    public static final int DEFAULT_UNIT_SIZE = 4;

    private static final int RESERVED_SIZE = 3;

    private static final int SIGNATURE_ALIGNMENT = 4;

    private long mapOffset;

    private long mapSize;

    private byte unitSize;

    private byte[] reserved = new byte[RESERVED_SIZE];

    private int signSize;

    private byte[] signature = new byte[0];

    private byte[] zeroPadding = new byte[0];

    /**
     * Constructor for PageInfoExtension
     *
     * @param mapOffset bitmap offset
     * @param mapSize   bit size
     */
    public PageInfoExtension(long mapOffset, long mapSize) {
        super(PAGE_INFO_INLINED, PAGE_INFO_EXTENSION_DATA_SIZE_WITHOUT_SIGN);
        this.mapOffset = mapOffset;
        this.mapSize = mapSize;
        unitSize = DEFAULT_UNIT_SIZE;
    }

    public void setSignature(byte[] signature) {
        if (signature != null) {
            this.signSize = signature.length;
            this.signature = signature;
            this.zeroPadding = new byte[(SIGNATURE_ALIGNMENT - (signSize % SIGNATURE_ALIGNMENT)) % SIGNATURE_ALIGNMENT];
        }
        super.setSize(size() - Extension.EXTENSION_HEADER_SIZE);
    }

    public long getMapOffset() {
        return mapOffset;
    }

    public long getMapSize() {
        return mapSize;
    }

    public byte getUnitSize() {
        return unitSize;
    }

    /**
     * Byte size of PageInfoExtension
     *
     * @return Byte size of PageInfoExtension
     */
    @Override
    public int size() {
        return Extension.EXTENSION_HEADER_SIZE + PAGE_INFO_EXTENSION_DATA_SIZE_WITHOUT_SIGN + signSize
            + zeroPadding.length;
    }

    /**
     * Converts PageInfoExtension to a newly created byte array
     *
     * @return Byte array representation of PageInfoExtension
     */
    @Override
    public byte[] toByteArray() {
        ByteBuffer bf = ByteBuffer.allocate(size()).order(ByteOrder.LITTLE_ENDIAN);
        bf.put(super.toByteArray());
        bf.putLong(this.mapOffset);
        bf.putLong(this.mapSize);
        bf.put(this.unitSize);
        bf.put(this.reserved);
        bf.putInt(this.signSize);
        bf.put(this.signature);
        bf.put(this.zeroPadding);
        return bf.array();
    }

    /**
     * Init the PageInfoExtension by a byte array
     *
     * @param bytes Byte array representation of a PageInfoExtension object
     * @return a newly created PageInfoExtension object
     * @throws VerifyCodeSignException parse result invalid
     */
    public static PageInfoExtension fromByteArray(byte[] bytes) throws VerifyCodeSignException {
        ByteBuffer bf = ByteBuffer.allocate(bytes.length).order(ByteOrder.LITTLE_ENDIAN);
        bf.put(bytes);
        bf.rewind();
        long inMapOffset = bf.getLong();
        if (!NumberUtils.isMultiple4K(inMapOffset)) {
            throw new VerifyCodeSignException("mapOffset is not a multiple of 4096");
        }
        long inMapSize = bf.getLong();
        byte inUnitSize = bf.get();
        if (inMapSize % inUnitSize != 0) {
            throw new VerifyCodeSignException("mapSize is not a multiple of unitSize");
        }
        bf.get(new byte[RESERVED_SIZE]);
        int inSignSize = bf.getInt();
        byte[] inSignature = new byte[inSignSize];
        bf.get(inSignature);
        PageInfoExtension extension = new PageInfoExtension(inMapOffset, inMapSize);
        extension.unitSize = inUnitSize;
        extension.setSignature(inSignature);
        return extension;
    }

    /**
     * validate PageInfoExtension with dataSize
     *
     * @param pgExtension  PageInfoExtension
     * @param dataSize     signed data size
     * @return true while PageInfoExtension filed value is valid
     * @throws PageInfoException error while filed value is invalid
     */
    public static boolean valid(PageInfoExtension pgExtension, long dataSize) throws PageInfoException {
        if (!NumberUtils.isMultiple4K(pgExtension.getMapOffset())) {
            throw new PageInfoException(CodeSignErrMsg.PAGE_INFO_ERROR.toString(
                "Invalid bitmapOff { " + pgExtension.getMapOffset() + " }, not a multiple of 4096"));
        }
        if (pgExtension.getUnitSize() != PageInfoExtension.DEFAULT_UNIT_SIZE) {
            throw new PageInfoException(
                CodeSignErrMsg.PAGE_INFO_ERROR.toString("Invalid page info unitSize : " + pgExtension.getUnitSize()));
        }
        if (pgExtension.getMapOffset() < 0 || pgExtension.getMapSize() < 0) {
            throw new PageInfoException(
                CodeSignErrMsg.PAGE_INFO_ERROR.toString("Page info offset or size is negative number"));
        }
        if (pgExtension.getMapSize() % pgExtension.getUnitSize() != 0) {
            throw new PageInfoException(
                CodeSignErrMsg.PAGE_INFO_ERROR.toString("Page info size is not multiple of unit"));
        }

        if (pgExtension.getMapOffset() > dataSize - pgExtension.getMapSize() / Byte.SIZE) {
            throw new PageInfoException(CodeSignErrMsg.PAGE_INFO_ERROR.toString("Page info is out of dataSize"));
        }
        if (pgExtension.getMapSize() / pgExtension.getUnitSize() >= dataSize / CodeSignBlock.PAGE_SIZE_4K) {
            throw new PageInfoException(
                CodeSignErrMsg.PAGE_INFO_ERROR.toString("Page info size is not less than data page "));
        }
        return true;
    }

    @Override
    public String toString() {
        return String.format(Locale.ROOT,
            "PageInfoExtension: size[%d], mapOffset[%d], mapSize[%d], unitSize[%d], signSize[%d]", size(),
            this.mapOffset, this.mapSize, this.unitSize, this.signSize);
    }
}
