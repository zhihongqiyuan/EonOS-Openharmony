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

package com.ohos.hapsigntool.hap.entity;

import com.ohos.hapsigntool.utils.ByteArrayUtils;

import java.io.IOException;
import java.util.ArrayList;

/**
 * Define class of content used to sign
 *
 * @since 2021-12-13
 */
class SignContentHash {
    /**
     * Length of two chars, one short,and one int
     */
    private static final int CONTENT_HEAD_SIZE = 8;

    /**
     * the signature sub-block type
     */
    protected char type;

    /**
     * the signature sub-block tag
     */
    protected char tag;

    /**
     * the algorithm ID of digest
     */
    protected short algId;

    /**
     * the data length of hash value
     */
    protected int length;

    /**
     * the data of hash value
     */
    protected byte[] hash;

    /**
     * the length of content
     */
    protected int contentHashLen;

    SignContentHash(char type, char tag, short algId, int length, byte[] hash) {
        this.type = type;
        this.tag = tag;
        this.algId = algId;
        this.length = length;
        this.hash = hash;
        this.contentHashLen = CONTENT_HEAD_SIZE + this.hash.length;
    }
}

/**
 * Define class of ContentInfo of PKCS7
 *
 * @since 2021-12-13
 */
public class SignContentInfo {
    // content version is 1.0.0.0
    private char[] version = "1000".toCharArray();
    private short size = 8;
    private short numOfBlocks = 0;
    private ArrayList<SignContentHash> hashData = new ArrayList<SignContentHash>();

    /**
     * input data to contentInfo
     *
     * @param type the signature sub-block type
     * @param tag the signature sub-block tag
     * @param algId the algorithm ID of digest
     * @param length the data length of hash value
     * @param hash the data of hash value
     */
    public void addContentHashData(char type, char tag, short algId, int length, byte[] hash) {
        SignContentHash signInfo = new SignContentHash(type, tag, algId, length, hash);
        this.addHashData(signInfo);
    }

    private void addHashData(SignContentHash signInfo) {
        hashData.add(signInfo);
        numOfBlocks++;
        size += signInfo.contentHashLen;
    }

    /**
     * Serialization of contentInfo
     *
     * @return Byte array of contentInfo after Serialization
     */
    public byte[] getByteContent() {
        byte[] ret = new byte[this.size];
        byte[] errorOutput = null;
        int index = 0;
        try {
            index = ByteArrayUtils.insertCharToByteArray(ret, index, version);
            if (index < 0) {
                throw new IOException();
            }
            index = ByteArrayUtils.insertShortToByteArray(ret, ret.length, index, size);
            if (index < 0) {
                throw new IOException();
            }
            index = ByteArrayUtils.insertShortToByteArray(ret, ret.length, index, numOfBlocks);
            if (index < 0) {
                throw new IOException();
            }
            for (int i = 0; i < hashData.size(); i++) {
                SignContentHash tmp = hashData.get(i);
                ret[index] = (byte) tmp.type;
                index++;
                ret[index] = (byte) tmp.tag;
                index++;
                index = ByteArrayUtils.insertShortToByteArray(ret, ret.length, index, tmp.algId);
                index = ByteArrayUtils.insertIntToByteArray(ret, index, tmp.length);
                index = ByteArrayUtils.insertByteToByteArray(ret, index, tmp.hash, tmp.hash.length);
                if (index < 0) {
                    throw new IOException();
                }
            }
        } catch (IOException e) {
            return errorOutput;
        }
        return ret;
    }
}
