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

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.HashMap;
import java.util.Map;

/**
 * The utils function used to get hash value.
 *
 * @since 2021/12/21
 */
public class HashUtils {
    private static final LogUtils LOGGER = new LogUtils(HashUtils.class);
    private static final int HASH_LEN = 4096;
    private static final Map<String, Integer> ALG_METHOD = new HashMap<>();

    static {
        ALG_METHOD.put("SHA-224", HashAlgs.USE_SHA224);
        ALG_METHOD.put("SHA-256", HashAlgs.USE_SHA256);
        ALG_METHOD.put("SHA-384", HashAlgs.USE_SHA384);
        ALG_METHOD.put("SHA-512", HashAlgs.USE_SHA512);
    }

    /**
     * Constructor of Method
     */
    private HashUtils() {
    }

    /**
     * Get algorithm id of algorithm name.
     *
     * @param algMethod algorithm name
     * @return algorithm ID
     */
    public static int getHashAlgsId(String algMethod) {
        if (ALG_METHOD.containsKey(algMethod)) {
            return ALG_METHOD.get(algMethod);
        }
        return HashAlgs.USE_NONE;
    }

    private static MessageDigest getMessageDigest(String algMethod) {
        MessageDigest md = null;
        try {
            md = MessageDigest.getInstance(algMethod);
        } catch (NoSuchAlgorithmException e) {
            LOGGER.error("no such algorithm", e);
        }
        return md;
    }

    private static byte[] getByteDigest(byte[] str, int count, String algMethod) {
        MessageDigest md = getMessageDigest(algMethod);
        md.update(str, 0, count);
        return md.digest();
    }

    /**
     * get digest used data in file.
     *
     * @param filePath input file path.
     * @param algName algorithm name.
     * @return byte array of digest.
     */
    public static byte[] getFileDigest(String filePath, String algName) {
        byte[] digest = null;
        MessageDigest md = getMessageDigest(algName);
        try (InputStream input = new FileInputStream(new File(filePath))) {
            if (md == null) {
                throw new NoSuchAlgorithmException();
            }
            byte[] fileDate = new byte[HASH_LEN];
            int num = 0;
            int byteCount;
            HashMap<Integer, byte[]> hashList = new HashMap<Integer, byte[]>();
            while ((byteCount = input.read(fileDate)) > 0) {
                byte[] dig = getByteDigest(fileDate, byteCount, algName);
                hashList.put(num, dig);
                num++;
            }
            if (hashList.isEmpty()) {
                LOGGER.error("hashList is empty");
                return digest;
            }
            for (int i = 0; i < hashList.size(); i++) {
                md.update(hashList.get(i));
            }
            return md.digest();
        } catch (FileNotFoundException e) {
            LOGGER.error("getFileDigest File Not Found failed.");
        } catch (IOException e) {
            LOGGER.error("getFileDigest IOException failed.", e);
        } catch (NoSuchAlgorithmException e) {
            LOGGER.error("MessageDigest is null", e);
        }
        return digest;
    }

    /**
     * Algorithm of Hash.
     *
     * @since 2021/12/21
     */
    static class HashAlgs {
        /**
         * None
         */
        public static final int USE_NONE = 0;

        /**
         * The MD2 message digest.
         */
        public static final int USE_MD2 = 1;

        /**
         * The MD4 message digest.
         */
        public static final int USE_MD4 = 2;

        /**
         * The MD5 message digest.
         */
        public static final int USE_MD5 = 3;

        /**
         * The SSH-1 message digest.
         */
        public static final int USE_SHA1 = 4;

        /**
         * The SSH-224 message digest.
         */
        public static final int USE_SHA224 = 5;

        /**
         * The SSH-256 message digest.
         */
        public static final int USE_SHA256 = 6;

        /**
         * The SSH-384 message digest.
         */
        public static final int USE_SHA384 = 7;

        /**
         * The SSH-512 message digest.
         */
        public static final int USE_SHA512 = 8;

        /**
         * The RIPEMD-160 message digest.
         */
        public static final int USE_RIPEMD160 = 9;

        /**
         * Constructor of Method
         */
        private HashAlgs() {
        }

    }
}

