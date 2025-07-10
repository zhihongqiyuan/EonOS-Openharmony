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

import com.ohos.hapsigntool.codesigning.exception.PageInfoException;
import com.ohos.hapsigntool.codesigning.exception.VerifyCodeSignException;
import com.ohos.hapsigntool.utils.LogUtils;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Locale;

/**
 * Sign info represents information after signing a file, including signature, merkle tree.
 * Structure:
 * <p>
 * 1) u32 saltSize: byte size of salt
 * <p>
 * 2) u32 sigSize: byte size of signature
 * <p>
 * 3) u32 flags: reserved flags
 * <p>
 * 4) u64 dataSize: byte size of data being signed
 * <p>
 * 5) u8[32] salt: salt used in signing
 * <p>
 * 6) u32 extensionNum: number of extension
 * <p>
 * 7) u32 extensionOffset
 * <p>
 * 8) u8[] signature: signature of the data
 * <p>
 * MerkleTree is represented as an extension of the sign info.
 * Its structure is defined in MerkleTreeExtension.java
 *
 * @since 2023/09/08
 */
public class SignInfo {
    /**
     * merkle tree extension is included in sign info
     */
    public static final int FLAG_MERKLE_TREE_INCLUDED = 0x1;

    /**
     * maximum of extension number
     */
    public static final int MAX_EXTENSION_NUM = 2;

    private static final LogUtils LOGGER = new LogUtils(SignInfo.class);

    /**
     * sign info structure without signature in bytes, refer to toByteArray() method
     */
    private static final int SIGN_INFO_SIZE_WITHOUT_SIGNATURE = 60;

    private static final int SALT_BUFFER_LENGTH = 32;

    private static final int SIGNATURE_ALIGNMENT = 4;

    private int saltSize;

    private int sigSize;

    private int flags;

    private long dataSize;

    private byte[] salt;

    private int extensionNum;

    private int extensionOffset;

    private byte[] signature;

    private byte[] zeroPadding;

    // temporary, use list instead
    private List<Extension> extensionList = new ArrayList<>();

    /**
     * Constructor for SignInfo
     *
     * @param saltSize byte size of salt
     * @param flags    reserved flags
     * @param dataSize byte size of data being signed
     * @param salt     salt in byte array representation
     * @param sig      signature after signing the data in byte array representation
     */
    public SignInfo(int saltSize, int flags, long dataSize, byte[] salt, byte[] sig) {
        this.saltSize = saltSize;
        this.flags = flags;
        this.dataSize = dataSize;
        if (salt == null) {
            this.salt = new byte[SALT_BUFFER_LENGTH];
        } else {
            this.salt = salt;
        }
        this.signature = sig;
        this.sigSize = sig == null ? 0 : sig.length;
        // align for extension after signature
        this.zeroPadding = new byte[(SIGNATURE_ALIGNMENT - (this.sigSize % SIGNATURE_ALIGNMENT)) % SIGNATURE_ALIGNMENT];
        this.extensionOffset = SIGN_INFO_SIZE_WITHOUT_SIGNATURE + sigSize + this.zeroPadding.length;
    }

    /**
     * Constructor by a SignInfoBuilder
     *
     * @param builder SignInfoBuilder
     */
    private SignInfo(SignInfoBuilder builder) {
        this.saltSize = builder.saltSize;
        this.sigSize = builder.sigSize;
        this.flags = builder.flags;
        this.dataSize = builder.dataSize;
        this.salt = builder.salt;
        this.extensionNum = builder.extensionNum;
        this.extensionOffset = builder.extensionOffset;
        this.signature = builder.signature;
        this.zeroPadding = builder.zeroPadding;
        this.extensionList = builder.extensionList;
    }

    /**
     * Add one Extension into SignInfo Object
     *
     * @param extension Extension object
     */
    public void addExtension(Extension extension) {
        this.extensionList.add(extension);
        this.extensionNum = this.extensionList.size();
    }

    /**
     * Get Extension from SignInfo based on extension type
     *
     * @param type extension type
     * @return Extension object
     */
    public Extension getExtensionByType(int type) {
        for (Extension ext : this.extensionList) {
            if (ext.isType(type)) {
                return ext;
            }
        }
        return null;
    }

    /**
     * Returns extensionNum
     *
     * @return extensionNum
     */
    public int getExtensionNum() {
        return extensionNum;
    }

    public byte[] getSignature() {
        return signature;
    }

    public long getDataSize() {
        return dataSize;
    }

    /**
     * Returns byte size of SignInfo object
     *
     * @return byte size of SignInfo object
     */
    public int size() {
        int blockSize = SIGN_INFO_SIZE_WITHOUT_SIGNATURE + this.signature.length + this.zeroPadding.length;
        for (Extension ext : this.extensionList) {
            blockSize += ext.size();
        }
        return blockSize;
    }

    /**
     * Converts SignInfo to a newly created byte array
     *
     * @return Byte array representation of SignInfo
     */
    public byte[] toByteArray() {
        ByteBuffer bf = ByteBuffer.allocate(this.size()).order(ByteOrder.LITTLE_ENDIAN);
        bf.putInt(this.saltSize);
        bf.putInt(this.sigSize);
        bf.putInt(this.flags);
        bf.putLong(this.dataSize);
        bf.put(this.salt);
        bf.putInt(this.extensionNum);
        bf.putInt(this.extensionOffset);
        bf.put(this.signature);
        bf.put(this.zeroPadding);
        // put extension
        for (Extension ext : this.extensionList) {
            bf.put(ext.toByteArray());
        }
        return bf.array();
    }

    /**
     * Init the SignInfo by a byte array
     *
     * @param bytes Byte array representation of a SignInfo object
     * @return a newly created SignInfo object
     * @throws VerifyCodeSignException parsing result invalid
     */
    public static SignInfo fromByteArray(byte[] bytes) throws VerifyCodeSignException {
        ByteBuffer bf = ByteBuffer.allocate(bytes.length).order(ByteOrder.LITTLE_ENDIAN);
        bf.put(bytes);
        bf.rewind();
        int inSaltSize = bf.getInt();
        if (inSaltSize < 0) {
            throw new VerifyCodeSignException("Invalid saltSize of SignInfo");
        }
        int inSigSize = bf.getInt();
        if (inSigSize < 0) {
            throw new VerifyCodeSignException("Invalid sigSize of SignInfo");
        }
        int inFlags = bf.getInt();
        if (inFlags != 0 && inFlags != FLAG_MERKLE_TREE_INCLUDED) {
            throw new VerifyCodeSignException("Invalid flags of SignInfo");
        }
        long inDataSize = bf.getLong();
        if (inDataSize < 0) {
            throw new VerifyCodeSignException("Invalid dataSize of SignInfo");
        }
        byte[] inSalt = new byte[SALT_BUFFER_LENGTH];
        bf.get(inSalt);
        int inExtensionNum = bf.getInt();
        if (inExtensionNum < 0 || inExtensionNum > MAX_EXTENSION_NUM) {
            LOGGER.info("The signature information may be generated by an new tool, extensionNum {} of SignInfo",
                inExtensionNum);
        }
        int inExtensionOffset = bf.getInt();
        if (inExtensionOffset < 0 || inExtensionOffset % 4 != 0) {
            throw new VerifyCodeSignException("Invalid extensionOffset of SignInfo");
        }
        byte[] inSignature = new byte[inSigSize];
        bf.get(inSignature);
        byte[] inZeroPadding = new byte[(SIGNATURE_ALIGNMENT - (inSigSize % SIGNATURE_ALIGNMENT))
            % SIGNATURE_ALIGNMENT];
        bf.get(inZeroPadding);
        // parse merkle tree extension
        List<Extension> inExtensionList = parseExtensionList(bf, inExtensionNum, inDataSize);
        return new SignInfoBuilder().setSaltSize(inSaltSize)
            .setSigSize(inSigSize)
            .setFlags(inFlags)
            .setDataSize(inDataSize)
            .setSalt(inSalt)
            .setExtensionNum(inExtensionNum)
            .setExtensionOffset(inExtensionOffset)
            .setSignature(inSignature)
            .setZeroPadding(inZeroPadding)
            .setExtensionList(inExtensionList)
            .build();
    }

    private static List<Extension> parseExtensionList(ByteBuffer bf, int inExtensionNum, long inDataSize)
        throws VerifyCodeSignException {
        List<Extension> inExtensionList = new ArrayList<>();
        for (int i = 0; i < inExtensionNum; i++) {
            int extensionType = bf.getInt();
            if (extensionType == MerkleTreeExtension.MERKLE_TREE_INLINED) {
                // parse merkle tree extension
                int extensionSize = bf.getInt();
                if (extensionSize != (MerkleTreeExtension.MERKLE_TREE_EXTENSION_DATA_SIZE)) {
                    throw new VerifyCodeSignException("Invalid MerkleTree extensionSize of SignInfo");
                }
                byte[] merkleTreeExtension = new byte[extensionSize];
                bf.get(merkleTreeExtension);
                inExtensionList.add(MerkleTreeExtension.fromByteArray(merkleTreeExtension));
            } else if (extensionType == PageInfoExtension.PAGE_INFO_INLINED) {
                // parse page info extension
                int extensionSize = bf.getInt();
                if (extensionSize < (PageInfoExtension.PAGE_INFO_EXTENSION_DATA_SIZE_WITHOUT_SIGN)) {
                    throw new VerifyCodeSignException("Invalid PageInfo extensionSize of SignInfo");
                }
                byte[] extensionBytes = new byte[extensionSize];
                bf.get(extensionBytes);
                PageInfoExtension pageInfoExtension = PageInfoExtension.fromByteArray(extensionBytes);
                try {
                    PageInfoExtension.valid(pageInfoExtension, inDataSize);
                } catch (PageInfoException e) {
                    throw new VerifyCodeSignException(e.getMessage());
                }
                inExtensionList.add(pageInfoExtension);
            } else {
                LOGGER.info("Invalid extensionType {} of SignInfo", extensionType);
            }
        }
        return inExtensionList;
    }

    /**
     * Return a string representation of the object
     *
     * @return string representation of the object
     */
    public String toString() {
        String str = String.format(Locale.ROOT, "SignInfo: saltSize[%d], sigSize[%d],"
                + "flags[%d], dataSize[%d], salt[%s], zeroPad[%s], extNum[%d], extOffset[%d]",
            this.saltSize, this.sigSize, this.flags, this.dataSize, Arrays.toString(this.salt),
            Arrays.toString(this.zeroPadding), this.extensionNum, this.extensionOffset);
        if (this.getExtensionByType(MerkleTreeExtension.MERKLE_TREE_INLINED) != null) {
            str += String.format(Locale.ROOT, "SignInfo.merkleTreeExtension[%s]",
                this.getExtensionByType(MerkleTreeExtension.MERKLE_TREE_INLINED).toString());
        }
        return str;
    }

    /**
     * Builder of SignInfo object
     */
    public static class SignInfoBuilder {
        private int saltSize;

        private int sigSize;

        private int flags;

        private long dataSize;

        private byte[] salt;

        private int extensionNum;

        private int extensionOffset;

        private byte[] signature;

        private byte[] zeroPadding;

        // temporary, use list instead
        private List<Extension> extensionList = new ArrayList<>();

        /**
         * set saltSize
         *
         * @param saltSize saltSize
         * @return SignInfoBuilder
         */
        public SignInfoBuilder setSaltSize(int saltSize) {
            this.saltSize = saltSize;
            return this;
        }

        /**
         * set sigSize
         *
         * @param sigSize sigSize
         * @return SignInfoBuilder
         */
        public SignInfoBuilder setSigSize(int sigSize) {
            this.sigSize = sigSize;
            return this;
        }

        /**
         * set flags
         *
         * @param flags flags
         * @return SignInfoBuilder
         */
        public SignInfoBuilder setFlags(int flags) {
            this.flags = flags;
            return this;
        }

        /**
         * set dataSize
         *
         * @param dataSize dataSize
         * @return SignInfoBuilder
         */
        public SignInfoBuilder setDataSize(long dataSize) {
            this.dataSize = dataSize;
            return this;
        }

        /**
         * set salt
         *
         * @param salt salt
         * @return SignInfoBuilder
         */
        public SignInfoBuilder setSalt(byte[] salt) {
            this.salt = salt;
            return this;
        }

        /**
         * set extensionNum
         *
         * @param extensionNum extensionNum
         * @return SignInfoBuilder
         */
        public SignInfoBuilder setExtensionNum(int extensionNum) {
            this.extensionNum = extensionNum;
            return this;
        }

        /**
         * set extensionOffset
         *
         * @param extensionOffset extensionOffset
         * @return SignInfoBuilder
         */
        public SignInfoBuilder setExtensionOffset(int extensionOffset) {
            this.extensionOffset = extensionOffset;
            return this;
        }

        /**
         * set signature
         *
         * @param signature signature
         * @return SignInfoBuilder
         */
        public SignInfoBuilder setSignature(byte[] signature) {
            this.signature = signature;
            return this;
        }

        /**
         * set zeroPadding
         *
         * @param zeroPadding zeroPadding
         * @return SignInfoBuilder
         */
        public SignInfoBuilder setZeroPadding(byte[] zeroPadding) {
            this.zeroPadding = zeroPadding;
            return this;
        }

        /**
         * set extensionList
         *
         * @param extensionList extensionList
         * @return SignInfoBuilder
         */
        public SignInfoBuilder setExtensionList(List<Extension> extensionList) {
            this.extensionList = extensionList;
            return this;
        }

        /**
         * return a SignInfo object
         *
         * @return SignInfo object
         */
        public SignInfo build() {
            return new SignInfo(this);
        }
    }
}
