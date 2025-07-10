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

package com.ohos.hapsigntool.codesigning.utils;

import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

/**
 * Digest util class
 *
 * @since 2023/06/05
 */
public class DigestUtils {
    /**
     * digest the inputContent with specific algorithm
     *
     * @param inputContentArray input Content Array
     * @param algorithm         hash algorithm
     * @return the result of digest, is a byte array
     * @throws NoSuchAlgorithmException if error
     */
    public static byte[] computeDigest(byte[] inputContentArray, String algorithm) throws NoSuchAlgorithmException {
        MessageDigest md = MessageDigest.getInstance(algorithm);
        md.update(inputContentArray);
        return md.digest();
    }
}
