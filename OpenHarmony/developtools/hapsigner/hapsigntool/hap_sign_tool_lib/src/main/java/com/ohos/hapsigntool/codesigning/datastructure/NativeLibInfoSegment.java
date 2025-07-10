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
import com.ohos.hapsigntool.entity.Pair;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Locale;

/**
 * SoInfoSegment consists of a header part:
 * <p>
 * u32 magic: magic number
 * <p>
 * u32 length: byte size of SoInfoSegment
 * <p>
 * u32 section num: the amount of file being signed
 * <p>
 * Followed by an area containing the offset and size of each file being signed with its signed info:
 * <p>
 * u32 file name offset: position of file name based on the start of SoInfoSegment
 * <p>
 * u32 file name size : byte size of file name string
 * <p>
 * u32 sign info offset : position of signed info based on the start of SoInfoSegment
 * <p>
 * u32 sign size: byte size of signed info
 * <p>
 * Ends with the file name and signed info content:
 * <p>
 * file name List : file name of each signed file
 * <p>
 * sign info List : signed info of each file
 * <p>
 *
 * @since 2023/09/08
 */
public class NativeLibInfoSegment {
    private static final int MAGIC_LENGTH_SECNUM_BYTES = 12;

    private static final int SIGNED_FILE_POS_SIZE = 16;

    // lower 4 bytes of the MD5 result of string "native lib info segment" (0ED2 E720)
    private static final int MAGIC_NUM = (0x0ED2 << 16) + 0xE720;

    private static final int ALIGNMENT_FOR_SIGNINFO = 4;

    private int magic;

    private int segmentSize;

    private int sectionNum;

    private List<Pair<String, SignInfo>> soInfoList = new ArrayList<>();

    private List<SignedFilePos> signedFilePosList;

    private List<String> fileNameList;

    private List<SignInfo> signInfoList;

    private byte[] zeroPadding;

    private int fileNameListBlockSize;

    private int signInfoListBlockSize;

    /**
     * Constructor for SoInfoSegment
     *
     * @param builder Builder
     */
    private NativeLibInfoSegment(Builder builder) {
        this.magic = builder.magic;
        this.segmentSize = builder.segmentSize;
        this.sectionNum = builder.sectionNum;
        this.signedFilePosList = builder.signedFilePosList;
        this.fileNameList = builder.fileNameList;
        this.signInfoList = builder.signInfoList;
        this.zeroPadding = builder.zeroPadding;
    }

    /**
     * set soInfoList, generate fileNameList and soInfoList
     *
     * @param soInfoList list of file and its signed info
     */
    public void setSoInfoList(List<Pair<String, SignInfo>> soInfoList) {
        this.soInfoList = soInfoList;
        // Once map is set, update length, sectionNum as well
        this.sectionNum = soInfoList.size();
        // generate file name list and sign info list
        generateList();
    }

    public int getSectionNum() {
        return sectionNum;
    }

    public List<String> getFileNameList() {
        return fileNameList;
    }

    public List<SignInfo> getSignInfoList() {
        return signInfoList;
    }

    // generate List based on current so
    private void generateList() {
        // empty all before generate list
        this.fileNameList.clear();
        this.signInfoList.clear();
        this.signedFilePosList.clear();
        int fileNameOffset = 0;
        int signInfoOffset = 0;
        for (Pair<String, SignInfo> soInfo : soInfoList) {
            String fileName = soInfo.getFirst();
            SignInfo signInfo = soInfo.getSecond();
            int fileNameSizeInBytes = fileName.getBytes(StandardCharsets.UTF_8).length;
            int signInfoSizeInBytes = signInfo.toByteArray().length;
            this.fileNameList.add(fileName);
            this.signInfoList.add(signInfo);
            this.signedFilePosList.add(
                new SignedFilePos(fileNameOffset, fileNameSizeInBytes, signInfoOffset, signInfoSizeInBytes));
            // increase fileNameOffset and signInfoOffset
            fileNameOffset += fileNameSizeInBytes;
            signInfoOffset += signInfoSizeInBytes;
        }
        this.fileNameListBlockSize = fileNameOffset;
        this.signInfoListBlockSize = signInfoOffset;
        // alignment for signInfo
        this.zeroPadding = new byte[(ALIGNMENT_FOR_SIGNINFO - this.fileNameListBlockSize % ALIGNMENT_FOR_SIGNINFO)
            % ALIGNMENT_FOR_SIGNINFO];
        // after fileNameList and signInfoList is generated, update segment size
        this.segmentSize = this.size();
        // adjust file name and sign info offset base on segment start
        int fileNameOffsetBase = MAGIC_LENGTH_SECNUM_BYTES + signedFilePosList.size() * SIGNED_FILE_POS_SIZE;
        int signInfoOffsetBase = fileNameOffsetBase + this.fileNameListBlockSize;
        for (SignedFilePos pos : this.signedFilePosList) {
            pos.increaseFileNameOffset(fileNameOffsetBase);
            pos.increaseSignInfoOffset(signInfoOffsetBase + this.zeroPadding.length);
        }
    }

    /**
     * Returns byte size of SoInfoSegment
     *
     * @return byte size of SoInfoSegment
     */
    public int size() {
        int blockSize = MAGIC_LENGTH_SECNUM_BYTES;
        blockSize += signedFilePosList.size() * SIGNED_FILE_POS_SIZE;
        blockSize += this.fileNameListBlockSize + this.zeroPadding.length + this.signInfoListBlockSize;
        return blockSize;
    }

    /**
     * Converts SoInfoSegment to a newly created byte array
     *
     * @return Byte array representation of SoInfoSegment
     */
    public byte[] toByteArray() {
        ByteBuffer bf = ByteBuffer.allocate(this.size()).order(ByteOrder.LITTLE_ENDIAN);
        bf.putInt(magic);
        bf.putInt(segmentSize);
        bf.putInt(sectionNum);
        for (SignedFilePos offsetAndSize : this.signedFilePosList) {
            bf.putInt(offsetAndSize.getFileNameOffset());
            bf.putInt(offsetAndSize.getFileNameSize());
            bf.putInt(offsetAndSize.getSignInfoOffset());
            bf.putInt(offsetAndSize.getSignInfoSize());
        }
        for (String fileName : fileNameList) {
            bf.put(fileName.getBytes(StandardCharsets.UTF_8));
        }
        bf.put(this.zeroPadding);
        for (SignInfo signInfo : signInfoList) {
            bf.put(signInfo.toByteArray());
        }
        return bf.array();
    }

    /**
     * Init the SoInfoSegment by a byte array
     *
     * @param bytes Byte array representation of a SoInfoSegment object
     * @return a newly created NativeLibInfoSegment object
     * @throws VerifyCodeSignException parsing result invalid
     */
    public static NativeLibInfoSegment fromByteArray(byte[] bytes) throws VerifyCodeSignException {
        ByteBuffer bf = ByteBuffer.allocate(bytes.length).order(ByteOrder.LITTLE_ENDIAN);
        bf.put(bytes);
        bf.rewind();
        int inMagic = bf.getInt();
        if (inMagic != MAGIC_NUM) {
            throw new VerifyCodeSignException("Invalid magic number of NativeLibInfoSegment");
        }
        int inSegmentSize = bf.getInt();
        if (inSegmentSize < 0) {
            throw new VerifyCodeSignException("Invalid segmentSize of NativeLibInfoSegment");
        }
        int inSectionNum = bf.getInt();
        if (inSectionNum < 0) {
            throw new VerifyCodeSignException("Invalid sectionNum of NativeLibInfoSegment");
        }
        List<SignedFilePos> inSignedFilePosList = new ArrayList<>();
        for (int i = 0; i < inSectionNum; i++) {
            byte[] entry = new byte[SIGNED_FILE_POS_SIZE];
            bf.get(entry);
            inSignedFilePosList.add(SignedFilePos.fromByteArray(entry));
        }
        // parse file name list
        List<String> inFileNameList = new ArrayList<>();
        int fileNameListSize = 0;
        for (SignedFilePos pos : inSignedFilePosList) {
            byte[] fileNameBuffer = new byte[pos.getFileNameSize()];
            fileNameListSize += pos.getFileNameSize();
            bf.position(pos.getFileNameOffset());
            bf.get(fileNameBuffer);
            inFileNameList.add(new String(fileNameBuffer, StandardCharsets.UTF_8));
        }
        // parse zeroPadding
        byte[] inZeroPadding = new byte[(ALIGNMENT_FOR_SIGNINFO - fileNameListSize % ALIGNMENT_FOR_SIGNINFO)
            % ALIGNMENT_FOR_SIGNINFO];
        bf.get(inZeroPadding);
        // parse sign info list
        List<SignInfo> inSignInfoList = new ArrayList<>();
        for (SignedFilePos pos : inSignedFilePosList) {
            if (pos.getSignInfoOffset() % ALIGNMENT_FOR_SIGNINFO != 0) {
                throw new VerifyCodeSignException("SignInfo not aligned in NativeLibInfoSegment");
            }
            byte[] signInfoBuffer = new byte[pos.getSignInfoSize()];
            bf.position(pos.getSignInfoOffset());
            bf.get(signInfoBuffer);
            inSignInfoList.add(SignInfo.fromByteArray(signInfoBuffer));
        }
        return new Builder().setMagic(inMagic).setSegmentSize(inSegmentSize).setSectionNum(inSectionNum)
            .setSignedFilePosList(inSignedFilePosList).setFileNameList(inFileNameList)
            .setSignInfoList(inSignInfoList).setZeroPadding(inZeroPadding).build();
    }

    /**
     * Return a string representation of the object
     *
     * @return string representation of the object
     */
    public String toString() {
        return String.format(Locale.ROOT,
            "SoInfoSegment: magic[%d], length[%d], secNum[%d], signedFileEntryList[%s], fileNameList[%s], "
                + "zeroPadding[%s], signInfoList[%s]", this.magic, this.segmentSize, this.sectionNum,
            Arrays.toString(this.signedFilePosList.toArray()), Arrays.toString(this.fileNameList.toArray()),
            Arrays.toString(this.zeroPadding), Arrays.toString(this.signInfoList.toArray()));
    }

    /**
     * Builder of NativeLibInfoSegment class
     */
    public static class Builder {
        private int magic = MAGIC_NUM;

        private int segmentSize;

        private int sectionNum;

        private List<SignedFilePos> signedFilePosList = new ArrayList<>();

        private List<String> fileNameList = new ArrayList<>();

        private List<SignInfo> signInfoList = new ArrayList<>();

        private byte[] zeroPadding = new byte[0];

        public Builder setMagic(int magic) {
            this.magic = magic;
            return this;
        }

        public Builder setSegmentSize(int segmentSize) {
            this.segmentSize = segmentSize;
            return this;
        }

        public Builder setSectionNum(int sectionNum) {
            this.sectionNum = sectionNum;
            return this;
        }

        public Builder setSignedFilePosList(List<SignedFilePos> signedFilePosList) {
            this.signedFilePosList = signedFilePosList;
            return this;
        }

        public Builder setFileNameList(List<String> fileNameList) {
            this.fileNameList = fileNameList;
            return this;
        }

        public Builder setSignInfoList(List<SignInfo> signInfoList) {
            this.signInfoList = signInfoList;
            return this;
        }

        public Builder setZeroPadding(byte[] zeroPadding) {
            this.zeroPadding = zeroPadding;
            return this;
        }

        /**
         * Create a NativeLibInfoSegment object
         *
         * @return a NativeLibInfoSegment object
         */
        public NativeLibInfoSegment build() {
            return new NativeLibInfoSegment(this);
        }
    }
}
