/*
 * Copyright (c) 2021-2023 Huawei Device Co., Ltd.
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

package com.ohos.hapsigntool.hap.utils;

import com.ohos.hapsigntool.entity.Pair;
import com.ohos.hapsigntool.hap.entity.SigningBlock;
import com.ohos.hapsigntool.error.SignatureNotFoundException;
import com.ohos.hapsigntool.entity.ContentDigestAlgorithm;
import com.ohos.hapsigntool.utils.LogUtils;
import com.ohos.hapsigntool.zip.MessageDigestZipDataOutput;
import com.ohos.hapsigntool.zip.ZipDataInput;
import com.ohos.hapsigntool.zip.ZipDataOutput;
import com.ohos.hapsigntool.zip.ZipFileInfo;

import org.bouncycastle.util.Arrays;

import java.io.ByteArrayOutputStream;
import java.io.FileInputStream;
import java.io.IOException;
import java.nio.BufferUnderflowException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.security.DigestException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

/**
 * Hap util, parse hap, find signature block.
 *
 * @since 2021/12/20
 */
public class HapUtils {
    private static final LogUtils LOGGER = new LogUtils(HapUtils.class);

    /**
     * ID of hap signature blocks of version 1
     */
    public static final int HAP_SIGNATURE_SCHEME_V1_BLOCK_ID = 0x20000000;

    /**
     * ID of hap proof of rotation block
     */
    public static final int HAP_PROOF_OF_ROTATION_BLOCK_ID = 0x20000001;

    /**
     * ID of profile block
     */
    public static final int HAP_PROFILE_BLOCK_ID = 0x20000002;

    /**
     * ID of property block
     */
    public static final int HAP_PROPERTY_BLOCK_ID = 0x20000003;

    /**
     * ID of property block
     */
    public static final int HAP_CODE_SIGN_BLOCK_ID = 0x30000001;

    /**
     * The size of data block used to get digest
     */

    public static final int CONTENT_DIGESTED_CHUNK_MAX_SIZE_BYTES = 1024 * 1024;

    /**
     * Content version
     */
    public static final int CONTENT_VERSION = 2;

    /**
     * bit size
     */
    public static final int BIT_SIZE = 8;

    /**
     * half bit size
     */
    public static final int HALF_BIT_SIZE = 4;

    /**
     * int size
     */
    public static final int INT_SIZE = 4;

    /**
     * block number
     */
    public static final int BLOCK_NUMBER = 1;

    /**
     * hap sign schema v2 signature block version
     */
    public static final int HAP_SIGN_SCHEME_V2_BLOCK_VERSION = 2;

    /**
     * hap sign schema v3 signature block version
     */
    public static final int HAP_SIGN_SCHEME_V3_BLOCK_VERSION = 3;

    /**
     * The value of lower 8-bytes of old magic word
     */
    public static final long HAP_SIG_BLOCK_MAGIC_LO_V2 = 0x2067695320504148L;

    /**
     * The value of higher 8-bytes of old magic word
     */
    public static final long HAP_SIG_BLOCK_MAGIC_HI_V2 = 0x3234206b636f6c42L;

    /**
     * The value of lower 8 bytes of magic word
     */
    public static final long HAP_SIG_BLOCK_MAGIC_LO_V3 = 0x676973207061683cL;

    /**
     * The value of higher 8 bytes of magic word
     */
    public static final long HAP_SIG_BLOCK_MAGIC_HI_V3 = 0x3e6b636f6c62206eL;

    /**
     * Size of hap signature block header
     */
    public static final int HAP_SIG_BLOCK_HEADER_SIZE = 32;

    /**
     * The min size of hap signature block
     */
    public static final int HAP_SIG_BLOCK_MIN_SIZE = HAP_SIG_BLOCK_HEADER_SIZE;

    /**
     * hap block size
     */
    public static final int BLOCK_SIZE = 8;

    /**
     * The set of IDs of optional blocks in hap signature block.
     */
    private static final Set<Integer> HAP_SIGNATURE_OPTIONAL_BLOCK_IDS ;

    /**
     * Minimum api version for hap sign schema v3.
     */
    private static final int MIN_COMPATIBLE_VERSION_FOR_SCHEMA_V3 = 8;

    /**
     * Magic word of hap signature block v2
     */
    private static final byte[] HAP_SIGNING_BLOCK_MAGIC_V2 =
            new byte[] {0x48, 0x41, 0x50, 0x20, 0x53, 0x69, 0x67, 0x20, 0x42, 0x6c, 0x6f, 0x63, 0x6b, 0x20, 0x34, 0x32};

    /**
     * Magic word of hap signature block
     */
    private static final byte[] HAP_SIGNING_BLOCK_MAGIC_V3 =
            new byte[] {0x3c, 0x68, 0x61, 0x70, 0x20, 0x73, 0x69, 0x67, 0x6e, 0x20, 0x62, 0x6c, 0x6f, 0x63, 0x6b, 0x3e};

    private static final byte ZIP_FIRST_LEVEL_CHUNK_PREFIX = 0x5a;
    private static final byte ZIP_SECOND_LEVEL_CHUNK_PREFIX = (byte) 0xa5;
    private static final int DIGEST_PRIFIX_LENGTH = 5;
    private static final int BUFFER_LENGTH = 4096;
    private static final char[] HEX_CHAR_ARRAY = "0123456789ABCDEF".toCharArray();

    /**
     * The set of IDs of optional blocks in hap signature block.
     */
    static {
        Set<Integer> blockIds = new HashSet<Integer>();
        blockIds.add(HAP_PROOF_OF_ROTATION_BLOCK_ID);
        blockIds.add(HAP_PROFILE_BLOCK_ID);
        blockIds.add(HAP_PROPERTY_BLOCK_ID);
        HAP_SIGNATURE_OPTIONAL_BLOCK_IDS = Collections.unmodifiableSet(blockIds);
    }

    private HapUtils() {
    }

    /**
     * Get HAP_SIGNATURE_OPTIONAL_BLOCK_IDS
     *
     * @return HAP_SIGNATURE_OPTIONAL_BLOCK_IDS
     */
    public static Set<Integer> getHapSignatureOptionalBlockIds() {
        return HAP_SIGNATURE_OPTIONAL_BLOCK_IDS;
    }

    /**
     * Get HAP_SIGNING_BLOCK_MAGIC
     *
     * @param compatibleVersion compatible api version
     * @return HAP_SIGNING_BLOCK_MAGIC
     */
    public static byte[] getHapSigningBlockMagic(int compatibleVersion) {
        if (compatibleVersion >= MIN_COMPATIBLE_VERSION_FOR_SCHEMA_V3) {
            return HAP_SIGNING_BLOCK_MAGIC_V3.clone();
        }
        return HAP_SIGNING_BLOCK_MAGIC_V2.clone();
    }

    /**
     * Get version number of hap signature block
     *
     * @param compatibleVersion compatible api version
     * @return magic to number
     */
    public static int getHapSigningBlockVersion(int compatibleVersion) {
        if (compatibleVersion >= MIN_COMPATIBLE_VERSION_FOR_SCHEMA_V3) {
            return HAP_SIGN_SCHEME_V3_BLOCK_VERSION;
        }
        return HAP_SIGN_SCHEME_V2_BLOCK_VERSION;
    }

    /**
     * Read data from hap file.
     *
     * @param file input file path.
     * @return true, if read successfully.
     * @throws IOException on error.
     */
    public static byte[] readFileToByte(String file) throws IOException {
        try (FileInputStream in = new FileInputStream(file);
             ByteArrayOutputStream out = new ByteArrayOutputStream(in.available());) {
            byte[] buf = new byte[BUFFER_LENGTH];
            int len = 0;
            while ((len = in.read(buf)) != -1) {
                out.write(buf, 0, len);
            }
            return out.toByteArray();
        }
    }

    private static long getChunkCount(ZipDataInput[] contents) {
        long chunkCount = 0L;
        for (ZipDataInput content : contents) {
            chunkCount += ((content.size() + CONTENT_DIGESTED_CHUNK_MAX_SIZE_BYTES - 1)
                    / CONTENT_DIGESTED_CHUNK_MAX_SIZE_BYTES);
        }
        return chunkCount;
    }

    /**
     * compute digests of contents
     *
     * @param digestAlgorithms algorithm of digest
     * @param zipData content used to get digest
     * @param optionalBlocks list of optional blocks used to get digest
     * @return digests
     * @throws DigestException digest error
     * @throws IOException if an IO error occurs when compute hap file digest
     */
    public static Map<ContentDigestAlgorithm, byte[]> computeDigests(
            Set<ContentDigestAlgorithm> digestAlgorithms, ZipDataInput[] zipData, List<SigningBlock> optionalBlocks)
            throws DigestException, IOException {
        long chunkCountLong = getChunkCount(zipData);
        if (chunkCountLong > Integer.MAX_VALUE) {
            throw new DigestException("Input too long: " + chunkCountLong + " chunks");
        }
        int chunkCount = (int) chunkCountLong;
        ContentDigestAlgorithm[] contentDigestAlgorithms = digestAlgorithms.toArray(
                new ContentDigestAlgorithm[digestAlgorithms.size()]);
        MessageDigest[] messageDigests = new MessageDigest[contentDigestAlgorithms.length];
        int[] digestOutputSizes = new int[contentDigestAlgorithms.length];
        byte[][] digestOfChunks = new byte[contentDigestAlgorithms.length][];
        initComputeItem(chunkCount, contentDigestAlgorithms, messageDigests, digestOutputSizes, digestOfChunks);
        int chunkIndex = 0;
        byte[] chunkContentPrefix = new byte[DIGEST_PRIFIX_LENGTH];
        chunkContentPrefix[0] = ZIP_SECOND_LEVEL_CHUNK_PREFIX;
        byte[] buf = new byte[CONTENT_DIGESTED_CHUNK_MAX_SIZE_BYTES];
        ZipDataOutput digests = new MessageDigestZipDataOutput(messageDigests);
        for (ZipDataInput content : zipData) {
            long offset = 0L;
            long remaining = content.size();
            while (remaining > 0) {
                int chunkSize = (int) Math.min(buf.length, remaining);
                setUInt32ToByteArrayWithLittleEngian(chunkSize, chunkContentPrefix, 1);
                for (int i = 0; i < contentDigestAlgorithms.length; i++) {
                    messageDigests[i].update(chunkContentPrefix);
                }
                try {
                    content.copyTo(offset, chunkSize, digests);
                } catch (IOException e) {
                    throw new IOException("Failed to read chunk #" + chunkIndex, e);
                }

                getDigests(contentDigestAlgorithms, digestOutputSizes, messageDigests, digestOfChunks, chunkIndex);
                offset += chunkSize;
                remaining -= chunkSize;
                chunkIndex++;
            }
        }
        return getContentDigestAlgorithmMap(optionalBlocks, contentDigestAlgorithms, messageDigests, digestOfChunks);
    }

    private static void getDigests(ContentDigestAlgorithm[] contentDigestAlgorithms, int[] digestOutputSizes,
        MessageDigest[] messageDigests, byte[][] digestOfChunks, int chunkIndex) throws DigestException {
        for (int i = 0; i < contentDigestAlgorithms.length; i++) {
            int expectedDigestSizeBytes = digestOutputSizes[i];
            int actualDigestSizeBytes = messageDigests[i].digest(digestOfChunks[i],
                    chunkIndex * expectedDigestSizeBytes + DIGEST_PRIFIX_LENGTH, expectedDigestSizeBytes);
            if (actualDigestSizeBytes != expectedDigestSizeBytes) {
                throw new DigestException("Unexpected output size of " + messageDigests[i].getAlgorithm()
                        + " digest: " + actualDigestSizeBytes);
            }
        }
    }

    private static void initComputeItem(int chunkCount, ContentDigestAlgorithm[] contentDigestAlgorithms,
                                        MessageDigest[] messageDigests, int[] digestOutputSizes,
                                        byte[][] digestOfChunks) throws DigestException {
        try {
            for (int i = 0; i < contentDigestAlgorithms.length; i++) {
                int digestOutputSizeBytes = contentDigestAlgorithms[i].getDigestOutputByteSize();
                byte[] concatenationOfChunkCountAndChunkDigests =
                        new byte[DIGEST_PRIFIX_LENGTH + chunkCount * digestOutputSizeBytes];
                concatenationOfChunkCountAndChunkDigests[0] = ZIP_FIRST_LEVEL_CHUNK_PREFIX;
                setUInt32ToByteArrayWithLittleEngian(chunkCount, concatenationOfChunkCountAndChunkDigests, 1);
                digestOfChunks[i] = concatenationOfChunkCountAndChunkDigests;
                messageDigests[i] = MessageDigest.getInstance(contentDigestAlgorithms[i].getDigestAlgorithm());
                digestOutputSizes[i] = contentDigestAlgorithms[i].getDigestOutputByteSize();
            }
        } catch (NoSuchAlgorithmException e) {
            throw new DigestException("Digest algorithm not supported", e);
        }
    }

    private static Map<ContentDigestAlgorithm, byte[]> getContentDigestAlgorithmMap(List<SigningBlock> optionalBlocks,
        ContentDigestAlgorithm[] contentDigestAlgorithms, MessageDigest[] messageDigests, byte[][] digestOfChunks) {
        Map<ContentDigestAlgorithm, byte[]> result = new HashMap<>(contentDigestAlgorithms.length);
        for (int i = 0; i < contentDigestAlgorithms.length; i++) {
            messageDigests[i].update(digestOfChunks[i]);
            for (SigningBlock signingBlock : optionalBlocks) {
                messageDigests[i].update(signingBlock.getValue());
            }
            result.put(contentDigestAlgorithms[i], messageDigests[i].digest());
        }
        return result;
    }

    private static void setUInt32ToByteArrayWithLittleEngian(int value, byte[] result, int offset) {
        for (int i = 0; i < INT_SIZE; i++) {
            result[offset + i] = (byte) ((value >> (BIT_SIZE * i)) & 0xff);
        }
    }

    /**
     * Slice buffer to target size.
     *
     * @param source input data buffer
     * @param targetSize target buffer's size
     * @return target buffer of target size
     */
    public static ByteBuffer sliceBuffer(ByteBuffer source, int targetSize) {
        int limit = source.limit();
        int position = source.position();
        int targetLimit = position + targetSize;
        if ((targetLimit < position) || (targetLimit > limit)) {
            LOGGER.error("targetSize: " + targetSize);
            throw new BufferUnderflowException();
        }
        try {
            source.limit(targetLimit);
            ByteBuffer target = source.slice();
            target.order(source.order());
            return target;
        } finally {
            source.position(targetLimit);
            source.limit(limit);
        }
    }

    private static ByteBuffer sliceBuffer(ByteBuffer source, int startPos, int endPos) {
        int capacity = source.capacity();
        if (startPos < 0 || endPos < startPos || endPos > capacity) {
            throw new IllegalArgumentException(
                    "startPos: " + startPos + ", endPos: " + endPos + ", capacity: " + capacity);
        }
        int limit = source.limit();
        int position = source.position();
        try {
            source.position(0);
            source.limit(endPos);
            source.position(startPos);
            ByteBuffer target = source.slice();
            target.order(source.order());
            return target;
        } finally {
            source.limit(limit);
            source.position(position);
        }
    }

    /**
     * Slice buffer from startPos to endPos, and then reverse it.
     *
     * @param hapSigningBlock input buffer used to slice.
     * @param startPos start position of slice buffer.
     * @param endPos end position of slice buffer.
     * @return new buffer.
     */
    public static ByteBuffer reverseSliceBuffer(ByteBuffer hapSigningBlock, int startPos, int endPos) {
        ByteBuffer header = HapUtils.sliceBuffer(hapSigningBlock, startPos, endPos);
        byte[] signatureBlockBytes = new byte[header.capacity()];
        header.get(signatureBlockBytes, 0, signatureBlockBytes.length);
        return ByteBuffer.wrap(Arrays.reverse(signatureBlockBytes));
    }

    /**
     * Check whether buffer is little endian.
     *
     * @param buffer ByteBuffer used to check
     */
    public static void checkBufferLittleEndian(ByteBuffer buffer) {
        if (buffer.order() == ByteOrder.LITTLE_ENDIAN) {
            return;
        }
        throw new IllegalArgumentException("ByteBuffer is not little endian");
    }

    /**
     * TLV encode list of pairs
     *
     * @param pairList input list of pairs
     * @return byte array after encoding
     */
    public static byte[] encodeListOfPairsToByteArray(List<Pair<Integer, byte[]>> pairList) {
        int encodeSize = 0;
        encodeSize += INT_SIZE + INT_SIZE;
        for (Pair<Integer, byte[]> pair : pairList) {
            encodeSize += INT_SIZE + INT_SIZE + INT_SIZE + pair.getSecond().length;
        }
        ByteBuffer encodeBytes = ByteBuffer.allocate(encodeSize);
        encodeBytes.order(ByteOrder.LITTLE_ENDIAN);
        encodeBytes.putInt(CONTENT_VERSION); // version
        encodeBytes.putInt(BLOCK_NUMBER); // block number
        for (Pair<Integer, byte[]> pair : pairList) {
            byte[] second = pair.getSecond();
            encodeBytes.putInt(INT_SIZE + INT_SIZE + second.length);
            encodeBytes.putInt(pair.getFirst());
            encodeBytes.putInt(second.length);
            encodeBytes.put(second);
        }
        return encodeBytes.array();
    }

    /**
     * Translate value to Hex string.
     *
     * @param value input byte array.
     * @param separator symbol insert between two bytes.
     * @return a hex-values string.
     */
    public static String toHex(byte[] value, String separator) {
        StringBuilder sb = new StringBuilder(value.length + value.length);
        String useSeparator = separator == null ? "" : separator;
        int len = value.length;
        for (int i = 0; i < len; i++) {
            int hi = (value[i] & 0xff) >>> HALF_BIT_SIZE;
            int lo = value[i] & 0x0f;
            sb.append(HEX_CHAR_ARRAY[hi]).append(HEX_CHAR_ARRAY[lo]);
            if (i != len - 1) {
                sb.append(useSeparator);
            }
        }
        return sb.toString();
    }

    /**
     * find signing block from hap file
     *
     * @param hap ZipDataInput object of zip file
     * @param zipInfo ZipFileInfo object of hap file
     * @return pair of offset of signing block and data of signing block
     * @throws SignatureNotFoundException No signing block is found
     * @throws IOException file operation error
     */
    public static HapSignBlockInfo findHapSigningBlock(ZipDataInput hap, ZipFileInfo zipInfo)
            throws SignatureNotFoundException, IOException {
        long centralDirectoryStartOffset = zipInfo.getCentralDirectoryOffset();
        long centralDirectorySize = zipInfo.getCentralDirectorySize();
        long eocdOffset = zipInfo.getEocdOffset();
        long centralDirectoryEndOffset = centralDirectoryStartOffset + centralDirectorySize;
        if (eocdOffset != centralDirectoryEndOffset) {
            throw new SignatureNotFoundException("ZIP Central Directory is not immediately followed by End of Central"
                    + "Directory. CD end: " + centralDirectoryEndOffset + ", EoCD start: " + eocdOffset);
        }
        if (centralDirectoryStartOffset < HAP_SIG_BLOCK_MIN_SIZE) {
            throw new SignatureNotFoundException("Hap too small for Hap Signing Block. ZIP Central Directory offset: "
                    + centralDirectoryStartOffset);
        }
        long hapSigningBlockHeaderOffset = centralDirectoryStartOffset - HAP_SIG_BLOCK_HEADER_SIZE;
        ByteBuffer hapSigningBlockHeader = hap.createByteBuffer(hapSigningBlockHeaderOffset, HAP_SIG_BLOCK_HEADER_SIZE);
        hapSigningBlockHeader.order(ByteOrder.LITTLE_ENDIAN);
        int blockCount = hapSigningBlockHeader.getInt();
        long hapSigBlockSize = hapSigningBlockHeader.getLong();
        long hapSignBlockMagicLo = hapSigningBlockHeader.getLong();
        long hapSignBlockMagicHi = hapSigningBlockHeader.getLong();
        int version = hapSigningBlockHeader.getInt();
        long hapSigningBlockOffset = verifySignBlock(hapSigBlockSize,
                hapSignBlockMagicLo, hapSignBlockMagicHi, version, centralDirectoryStartOffset);
        ByteBuffer hapSigningBlockByteBuffer = hap.createByteBuffer(hapSigningBlockOffset, (int) hapSigBlockSize)
                .order(ByteOrder.LITTLE_ENDIAN);
        LOGGER.info("Find Hap Signing Block success, version: {}, block count: {}", version, blockCount);
        return new HapSignBlockInfo(hapSigningBlockOffset, version, hapSigningBlockByteBuffer);
    }

    private static long verifySignBlock(long hapSigBlockSize, long hapSignBlockMagicLo,
        long hapSignBlockMagicHi, int version, long centralDirectoryStartOffset) throws SignatureNotFoundException {
        if (!isVersionAndMagicNumValid(version, hapSignBlockMagicLo, hapSignBlockMagicHi)) {
            throw new SignatureNotFoundException("No Hap Signing Block before ZIP Central Directory");
        }
        if ((hapSigBlockSize < HAP_SIG_BLOCK_HEADER_SIZE)
                || (hapSigBlockSize > Integer.MAX_VALUE - BLOCK_SIZE)) {
            throw new SignatureNotFoundException("Hap Signing Block size out of range: " + hapSigBlockSize);
        }
        int totalSize = (int) hapSigBlockSize;
        long hapSigningBlockOffset = centralDirectoryStartOffset - totalSize;
        if (hapSigningBlockOffset < 0) {
            throw new SignatureNotFoundException("Hap Signing Block offset out of range: " + hapSigningBlockOffset);
        }
        return hapSigningBlockOffset;
    }

    private static boolean isVersionAndMagicNumValid(int version, long hapSignBlockMagicLo, long hapSignBlockMagicHi) {
        if (version < HAP_SIGN_SCHEME_V3_BLOCK_VERSION) {
            return hapSignBlockMagicLo == HAP_SIG_BLOCK_MAGIC_LO_V2 && hapSignBlockMagicHi == HAP_SIG_BLOCK_MAGIC_HI_V2;
        }
        return hapSignBlockMagicLo == HAP_SIG_BLOCK_MAGIC_LO_V3 && hapSignBlockMagicHi == HAP_SIG_BLOCK_MAGIC_HI_V3;
    }

    /**
     * Hap sign block info
     */
    public static class HapSignBlockInfo {
        private final long offset;
        private final int version;
        private final ByteBuffer content;

        public HapSignBlockInfo(long offset, int version, ByteBuffer content) {
            this.offset = offset;
            this.version = version;
            this.content = content;
        }

        public int getVersion() {
            return version;
        }

        public ByteBuffer getContent() {
            return content;
        }

        public long getOffset() {
            return offset;
        }
    }
}