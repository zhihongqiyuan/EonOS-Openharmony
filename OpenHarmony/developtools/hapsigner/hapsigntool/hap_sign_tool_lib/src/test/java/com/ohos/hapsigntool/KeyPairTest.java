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

package com.ohos.hapsigntool;

import com.ohos.hapsigntool.error.CustomException;
import com.ohos.hapsigntool.utils.KeyPairTools;
import org.junit.jupiter.api.Test;
import org.junit.platform.commons.logging.Logger;
import org.junit.platform.commons.logging.LoggerFactory;

import java.security.KeyPair;
import java.security.interfaces.ECPrivateKey;
import java.security.interfaces.ECPublicKey;
import java.security.interfaces.RSAPrivateKey;
import java.security.interfaces.RSAPublicKey;

import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertTrue;

/**
 * KeyPairTest.
 *
 * @since 2021/12/28
 */
public class KeyPairTest {
    /**
     * Add log info.
     */
    private static final Logger logger = LoggerFactory.getLogger(KeyPairTest.class);

    /**
     * test keyPair
     */
    @Test
    public void testKeyPair() {
        KeyPair kRsa2048 = KeyPairTools.generateKeyPair(KeyPairTools.RSA, KeyPairTools.RSA_2048);
        assertNotNull(kRsa2048);
        assertTrue(kRsa2048.getPrivate() instanceof RSAPrivateKey);
        assertTrue(kRsa2048.getPublic() instanceof RSAPublicKey);

        KeyPair kRsa3072 = KeyPairTools.generateKeyPair(KeyPairTools.RSA, KeyPairTools.RSA_3072);
        assertNotNull(kRsa3072);
        assertTrue(kRsa3072.getPrivate() instanceof RSAPrivateKey);
        assertTrue(kRsa3072.getPublic() instanceof RSAPublicKey);

        KeyPair kRsa4096 = KeyPairTools.generateKeyPair(KeyPairTools.RSA, KeyPairTools.RSA_4096);
        assertNotNull(kRsa4096);
        assertTrue(kRsa4096.getPrivate() instanceof RSAPrivateKey);
        assertTrue(kRsa4096.getPublic() instanceof RSAPublicKey);

        KeyPair kEcc256 = KeyPairTools.generateKeyPair(KeyPairTools.ECC, KeyPairTools.NIST_P_256);
        assertNotNull(kEcc256);
        assertTrue(kEcc256.getPrivate() instanceof ECPrivateKey);
        assertTrue(kEcc256.getPublic() instanceof ECPublicKey);

        KeyPair kEcc384 = KeyPairTools.generateKeyPair(KeyPairTools.ECC, KeyPairTools.NIST_P_384);
        assertNotNull(kEcc384);
        assertTrue(kEcc384.getPrivate() instanceof ECPrivateKey);
        assertTrue(kEcc384.getPublic() instanceof ECPublicKey);

        try {
            KeyPair keyPairRsa = KeyPairTools.generateKeyPair(KeyPairTools.RSA, KeyPairTools.NIST_P_256);
            assertNull(keyPairRsa);
        } catch (CustomException exception) {
            logger.info(exception, () -> exception.getMessage());
        }
        try {
            KeyPair keyPairEcc = KeyPairTools.generateKeyPair(KeyPairTools.ECC, KeyPairTools.RSA_3072);
            assertNull(keyPairEcc);
        } catch (CustomException exception) {
            logger.info(exception, () -> exception.getMessage());
        }
    }
}
