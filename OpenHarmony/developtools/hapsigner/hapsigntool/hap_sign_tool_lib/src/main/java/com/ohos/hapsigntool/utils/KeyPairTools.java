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

import com.ohos.hapsigntool.error.CustomException;
import com.ohos.hapsigntool.error.ERROR;

import com.ohos.hapsigntool.error.SignToolErrMsg;
import org.bouncycastle.util.encoders.Base64;

import java.security.Key;
import java.security.KeyFactory;
import java.security.KeyPair;
import java.security.KeyPairGenerator;
import java.security.PublicKey;
import java.security.PrivateKey;
import java.security.NoSuchAlgorithmException;
import java.security.spec.InvalidKeySpecException;
import java.security.spec.PKCS8EncodedKeySpec;
import java.security.spec.X509EncodedKeySpec;

/**
 * Key pair relation Class, to create new key pairs.
 *
 * @since 2021/12/28
 */
public final class KeyPairTools {
    /**
     * Field RSA.
     */
    public static final String RSA = "RSA";

    /**
     * Field EC.
     */
    public static final String ECC = "EC";

    /**
     * Field ECC.
     */
    public static final String ECC_INPUT = "ECC";

    /**
     * Field RSA_2048.
     */
    public static final int RSA_2048 = 2048;

    /**
     * Field RSA_3072.
     */
    public static final int RSA_3072 = 3072;

    /**
     * Field RSA_4096.
     */
    public static final int RSA_4096 = 4096;

    /**
     * Field NIST_P_256.
     */
    public static final int NIST_P_256 = 256;

    /**
     * Field NIST_P_384.
     */
    public static final int NIST_P_384 = 384;

    /**
     * Logger.
     */
    private static final LogUtils LOGGER = new LogUtils(KeyPairTools.class);

    private KeyPairTools() {}

    /**
     * generateKeyPair
     *
     * @param algorithm RSA/ECC
     * @param keySize   RSA_2048/3072/4096 NIST_P_256/384
     * @return Generated keypair
     */
    public static KeyPair generateKeyPair(String algorithm, int keySize) {
        if (algorithm == null) {
            CustomException.throwException(ERROR.NOT_SUPPORT_ERROR, SignToolErrMsg.ALGORITHM_NOT_SUPPORT
                    .toString("Not support algorithm: null"));
        }
        String alg = algorithm;
        if (ECC_INPUT.equalsIgnoreCase(alg)) {
            alg = ECC;
        }
        if (RSA.equalsIgnoreCase(alg)) {
            ValidateUtils.throwIfNotMatches((keySize == RSA_2048 || keySize == RSA_3072 || keySize == RSA_4096),
                    ERROR.NOT_SUPPORT_ERROR, SignToolErrMsg.ALGORITHM_NOT_SUPPORT
                            .toString("Algorithm 'RSA' not support size: " + keySize));
        } else if (ECC.equalsIgnoreCase(alg)) {
            ValidateUtils.throwIfNotMatches((keySize == NIST_P_256 || keySize == NIST_P_384),
                    ERROR.NOT_SUPPORT_ERROR, SignToolErrMsg.ALGORITHM_NOT_SUPPORT
                            .toString("Algorithm 'ECC' not support size: " + keySize));
        } else {
            CustomException.throwException(ERROR.NOT_SUPPORT_ERROR, SignToolErrMsg.ALGORITHM_NOT_SUPPORT
                    .toString("Not support algorithm: " + alg));
        }

        try {
            KeyPairGenerator keyPairGenerator = KeyPairGenerator.getInstance(alg);
            keyPairGenerator.initialize(keySize);
            return keyPairGenerator.generateKeyPair();
        } catch (NoSuchAlgorithmException e) {
            LOGGER.debug(e.getMessage(), e);
            CustomException.throwException(ERROR.NOT_SUPPORT_ERROR, SignToolErrMsg.NO_SUCH_SIGNATURE
                    .toString(e.getMessage()));
            return null;
        }
    }

    /**
     * Convert key to String
     *
     * @param key input parameter and key can not be null.
     * @return return key.getEncoded() in Base64 format.
     */
    public static String key2String(Key key) {
        return Base64.toBase64String(key.getEncoded());
    }

    /**
     * Convert string back to key
     *
     * @param algorithm input parameter and algorithm can not be null.
     * @param keyString input parameter and keyString can not be null.
     * @return return PublicKey.
     */
    public static PublicKey stringToPublicKey(String algorithm, String keyString) {
        X509EncodedKeySpec spec = new X509EncodedKeySpec(Base64.decode(keyString));
        PublicKey result = null;
        try {
            result = KeyFactory.getInstance(algorithm).generatePublic(spec);
        } catch (InvalidKeySpecException | NoSuchAlgorithmException exception) {
            LOGGER.debug(exception.getMessage(), exception);
            CustomException.throwException(ERROR.ACCESS_ERROR, SignToolErrMsg.NO_SUCH_SIGNATURE
                    .toString(exception.getMessage()));
        }
        return result;
    }

    /**
     * Convert string back to key
     *
     * @param algorithm input parameter and algorithm can not be null.
     * @param keyString input parameter and keyString can not be null.
     * @return return PrivateKey.
     */
    public static PrivateKey stringToPrivateKey(String algorithm, String keyString) {
        PKCS8EncodedKeySpec spec = new PKCS8EncodedKeySpec(Base64.decode(keyString));
        PrivateKey result = null;
        try {
            result = KeyFactory.getInstance(algorithm).generatePrivate(spec);
        } catch (InvalidKeySpecException | NoSuchAlgorithmException exception) {
            LOGGER.debug(exception.getMessage(), exception);
            CustomException.throwException(ERROR.ACCESS_ERROR, SignToolErrMsg.NO_SUCH_SIGNATURE
                    .toString(exception.getMessage()));
        }
        return result;
    }
}
