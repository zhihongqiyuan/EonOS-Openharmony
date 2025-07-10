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

import com.ohos.hapsigntool.utils.KeyPairTools;
import com.ohos.hapsigntool.utils.KeyStoreHelper;
import com.ohos.hapsigntool.utils.FileUtils;
import org.bouncycastle.jce.provider.BouncyCastleProvider;
import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.security.KeyPair;
import java.security.Security;

import static org.junit.jupiter.api.Assertions.assertEquals;

/**
 * KeyStoreTest.
 *
 * @since 2021/12/28
 */
public class KeyStoreTest {
    static {
        Security.addProvider(new BouncyCastleProvider());
    }

    /**
     * test keystore
     *
     * @throws IOException io error.
     */
    @Test
    public void testKeyStore() throws IOException {
        String keyStorePath = "test_keypair.jks";
        String pwd = "123456";
        String keyAlias = "oh-app1-key-v1";
        if (FileUtils.isFileExist(keyStorePath)) {
            Path path = Paths.get(keyStorePath);
            Files.delete(path);
        }
        KeyPair keyPair = KeyPairTools.generateKeyPair(KeyPairTools.RSA, KeyPairTools.RSA_2048);
        KeyStoreHelper keyStoreHelper = new KeyStoreHelper(keyStorePath, pwd.toCharArray());
        keyStoreHelper.store(keyAlias, pwd.toCharArray(), keyPair, null);

        KeyStoreHelper keyStore = new KeyStoreHelper(keyStorePath, pwd.toCharArray());
        KeyPair keyPairLoad = keyStore.loadKeyPair(keyAlias, pwd.toCharArray());

        assertEquals(keyPair.getPrivate(), keyPairLoad.getPrivate());
        assertEquals(keyPair.getPublic(), keyPairLoad.getPublic());
    }
}
