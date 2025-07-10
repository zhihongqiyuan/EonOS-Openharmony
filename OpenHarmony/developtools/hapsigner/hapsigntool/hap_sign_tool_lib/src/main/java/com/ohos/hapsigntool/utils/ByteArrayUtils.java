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

package com.ohos.hapsigntool.utils;

/**
 * utils functions about byte arrays
 *
 * @since 2021-12-13
 */
public class ByteArrayUtils {
    private static final int BIT_SIZE = 8;

    private static final int DOUBLE_BIT_SIZE = 16;

    private static final int TRIPLE_BIT_SIZE = 24;

    private static final int HALF_INTEGER_BYTES = 2;

    private ByteArrayUtils() {
    }

    /**
     * Insert int value to byte array
     *
     * @param desByte destination byte array
     * @param index position of inserting
     * @param num value is inserted
     * @return end of position of inserting, if successfully
     */
    public static int insertIntToByteArray(byte[] desByte, int index, int num) {
        if (desByte == null) {
            return -1;
        }
        if (index + Integer.BYTES > desByte.length) {
            return -1;
        }
        int pos = index;
        desByte[pos] = (byte) ((num >> TRIPLE_BIT_SIZE) & 0xff);
        pos++;
        desByte[pos] = (byte) ((num >> DOUBLE_BIT_SIZE) & 0xff);
        pos++;
        desByte[pos] = (byte) ((num >> BIT_SIZE) & 0xff);
        pos++;
        desByte[pos] = (byte) (num & 0xff);
        pos++;
        return pos;
    }

    /**
     * Insert short value to byte array
     *
     * @param desByte destination byte array
     * @param desByteLen length of destination byte array
     * @param index position of inserting
     * @param num value is inserted
     * @return end of position of inserting, if successfully
     */
    public static int insertShortToByteArray(byte[] desByte, int desByteLen, int index, short num) {
        if (desByte == null) {
            return -1;
        }
        if (index + HALF_INTEGER_BYTES > desByteLen) {
            return -1;
        }
        int pos = index;
        desByte[pos] = (byte) ((num >> BIT_SIZE) & 0xff);
        pos++;
        desByte[pos] = (byte) (num & 0xff);
        pos++;
        return pos;
    }

    /**
     * Insert byte array to byte array
     *
     * @param des destination byte array
     * @param start position of inserting
     * @param src byte array is inserted
     * @param srcLen length of byte array is inserted
     * @return end of position of inserting, if successfully
     */
    public static int insertByteToByteArray(byte[] des, int start, byte[] src, int srcLen) {
        if (src.length < srcLen) {
            return -1;
        }
        System.arraycopy(src, 0, des, start, srcLen);
        return start + srcLen;
    }

    /**
     * Insert char array to byte array
     *
     * @param des destination byte array
     * @param start position of inserting
     * @param src char array is inserted
     * @return end of position of inserting, if successfully
     */
    public static int insertCharToByteArray(byte[] des, int start, char[] src) {
        if (des == null) {
            return -1;
        }
        if (start > des.length - src.length) {
            return -1;
        }
        for (int i = 0; i < src.length; i++) {
            des[i + start] = (byte) src[i];
        }
        return start + src.length;
    }
}