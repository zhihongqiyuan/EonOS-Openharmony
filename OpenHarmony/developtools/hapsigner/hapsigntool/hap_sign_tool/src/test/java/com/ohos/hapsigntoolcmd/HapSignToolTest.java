/*
 * Copyright (c) 2024-2024 Huawei Device Co., Ltd.
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

package com.ohos.hapsigntoolcmd;

import com.ohos.entity.InForm;
import com.ohos.entity.Mode;
import com.ohos.entity.ProFileSigned;
import com.ohos.entity.SignAppParameters;
import com.ohos.entity.VerifyAppParameters;
import com.ohos.hapsigntool.HapSignTool;
import com.ohos.hapsigntool.error.ERROR;
import com.ohos.hapsigntool.utils.FileUtils;
import org.junit.jupiter.api.AfterAll;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

/**
 * Method Test
 *
 * @since 2024/04/12
 */
public class HapSignToolTest {
    private static final String TMP_SIGNED_FILE = "signed-";

    private static final String TMP_HAP_SUFFIX = ".hap";

    private static final File TMP_DIR = new File("concurrentTest");

    private static final List<Cleanable> tmpSource = new ArrayList<>();

    @BeforeAll
    public static void before() {
        TMP_DIR.mkdirs();
    }

    @AfterAll
    public static void after() {
        for (Cleanable c : tmpSource) {
            c.clean();
        }
    }

    /**
     * test sign params default value
     *
     * @throws IOException IOException
     */
    @Test
    public void testDefaultValue() throws IOException {
        File outputFile = File.createTempFile(TMP_SIGNED_FILE, TMP_HAP_SUFFIX, TMP_DIR);
        tmpSource.add(new Cleanable(outputFile));

        SignAppParameters signAppParameters = new SignAppParameters();
        signAppParameters.setMode(Mode.LOCAL_SIGN);
        signAppParameters.setKeyAlias("oh-app1-key-v1");
        signAppParameters.setKeyPwd("123456".toCharArray());
        signAppParameters.setAppCertFile("../../tools/app1.pem");
        signAppParameters.setProfileFile("../../tools/app1-profile.p7b");
        signAppParameters.setInFile("../../tools/test/app1-unsigned.hap");
        signAppParameters.setSignAlg("SHA256withECDSA");
        signAppParameters.setKeyStoreFile("../../tools/ohtest_pass.jks");
        signAppParameters.setKeystorePwd("123456".toCharArray());
        signAppParameters.setOutFile(outputFile.getCanonicalPath());
        Assertions.assertEquals(HapSignTool.signApp(signAppParameters).getErrCode(), ERROR.SUCCESS_CODE);

        VerifyAppParameters verifyAppParameters = new VerifyAppParameters();
        verifyAppParameters.setInFile(outputFile.getCanonicalPath());
        verifyAppParameters.setOutCertChain("out.cer");
        verifyAppParameters.setOutProfile("out.p7b");
        Assertions.assertEquals(HapSignTool.verifyApp(verifyAppParameters).getErrCode(), ERROR.SUCCESS_CODE);
    }

    /**
     * test sign elf
     *
     * @throws IOException IOException
     */
    @Test
    public void testSignElf() throws IOException {
        File outputFile = File.createTempFile(TMP_SIGNED_FILE, TMP_HAP_SUFFIX, TMP_DIR);
        tmpSource.add(new Cleanable(outputFile));

        SignAppParameters signAppParameters = new SignAppParameters();
        signAppParameters.setMode(Mode.LOCAL_SIGN);
        signAppParameters.setKeyAlias("oh-app1-key-v1");
        signAppParameters.setKeyPwd("123456".toCharArray());
        signAppParameters.setAppCertFile("../../tools/app1.pem");
        signAppParameters.setProfileFile("../../tools/app1-profile.p7b");
        signAppParameters.setInFile("../../tools/test/elf_unittest.elf");
        signAppParameters.setSignAlg("SHA256withECDSA");
        signAppParameters.setKeyStoreFile("../../tools/ohtest_pass.jks");
        signAppParameters.setKeystorePwd("123456".toCharArray());
        signAppParameters.setOutFile(outputFile.getCanonicalPath());
        signAppParameters.setInForm(InForm.ELF);
        Assertions.assertEquals(HapSignTool.signApp(signAppParameters).getErrCode(), ERROR.SUCCESS_CODE);

        VerifyAppParameters verifyAppParameters = new VerifyAppParameters();
        verifyAppParameters.setInFile(outputFile.getCanonicalPath());
        verifyAppParameters.setOutCertChain("out.cer");
        verifyAppParameters.setOutProfile("out.p7b");
        verifyAppParameters.setInForm(InForm.ELF);
        Assertions.assertEquals(HapSignTool.verifyApp(verifyAppParameters).getErrCode(), ERROR.SUCCESS_CODE);
    }

    /**
     * test sign elf
     *
     * @throws IOException IOException
     */
    @Test
    public void testSignElfUnsignedProfile() throws IOException {
        File outputFile = File.createTempFile(TMP_SIGNED_FILE, TMP_HAP_SUFFIX, TMP_DIR);
        tmpSource.add(new Cleanable(outputFile));

        SignAppParameters signAppParameters = new SignAppParameters();
        signAppParameters.setMode(Mode.LOCAL_SIGN);
        signAppParameters.setKeyAlias("oh-app1-key-v1");
        signAppParameters.setKeyPwd("123456".toCharArray());
        signAppParameters.setAppCertFile("../../tools/app1.pem");
        signAppParameters.setProfileFile("../../tools/profile.json");
        signAppParameters.setInFile("../../tools/test/elf_unittest.elf");
        signAppParameters.setSignAlg("SHA256withECDSA");
        signAppParameters.setKeyStoreFile("../../tools/ohtest_pass.jks");
        signAppParameters.setKeystorePwd("123456".toCharArray());
        signAppParameters.setOutFile(outputFile.getCanonicalPath());
        signAppParameters.setProfileSigned(ProFileSigned.UNSIGNED);
        signAppParameters.setInForm(InForm.ELF);
        Assertions.assertNotEquals(HapSignTool.signApp(signAppParameters).getErrCode(), ERROR.SUCCESS_CODE);
    }

    /**
     * test Param Without KeyAlias
     *
     * @throws IOException IOException
     */
    @Test
    public void testParamWithoutKeyAlias() throws IOException {
        File outputFile = File.createTempFile(TMP_SIGNED_FILE, TMP_HAP_SUFFIX, TMP_DIR);
        tmpSource.add(new Cleanable(outputFile));

        SignAppParameters signAppParameters = new SignAppParameters();
        signAppParameters.setMode(Mode.LOCAL_SIGN);
        signAppParameters.setKeyPwd("123456".toCharArray());
        signAppParameters.setAppCertFile("../../tools/app1.pem");
        signAppParameters.setProfileFile("../../tools/app1-profile.p7b");
        signAppParameters.setInFile("../../tools/test/app1-unsigned.hap");
        signAppParameters.setSignAlg("SHA256withECDSA");
        signAppParameters.setKeyStoreFile("../../tools/ohtest_pass.jks");
        signAppParameters.setKeystorePwd("123456".toCharArray());
        signAppParameters.setOutFile(outputFile.getCanonicalPath());
        Assertions.assertNotEquals(HapSignTool.signApp(signAppParameters).getErrCode(), ERROR.SUCCESS_CODE);
    }

    /**
     * test Param Without SignAlg
     *
     * @throws IOException IOException
     */
    @Test
    public void testParamWithoutSignAlg() throws IOException {
        File outputFile = File.createTempFile(TMP_SIGNED_FILE, TMP_HAP_SUFFIX, TMP_DIR);
        tmpSource.add(new Cleanable(outputFile));

        SignAppParameters signAppParameters = new SignAppParameters();
        signAppParameters.setMode(Mode.LOCAL_SIGN);
        signAppParameters.setKeyAlias("oh-app1-key-v1");
        signAppParameters.setKeyPwd("123456".toCharArray());
        signAppParameters.setAppCertFile("../../tools/app1.pem");
        signAppParameters.setProfileFile("../../tools/app1-profile.p7b");
        signAppParameters.setInFile("../../tools/test/app1-unsigned.hap");
        signAppParameters.setKeyStoreFile("../../tools/ohtest_pass.jks");
        signAppParameters.setKeystorePwd("123456".toCharArray());
        signAppParameters.setOutFile(outputFile.getCanonicalPath());
        Assertions.assertNotEquals(HapSignTool.signApp(signAppParameters).getErrCode(), ERROR.SUCCESS_CODE);
    }


    /**
     * test Param Without KeyPwd
     *
     * @throws IOException IOException
     */
    @Test
    public void testParamWithoutKeyPwd() throws IOException {
        File outputFile = File.createTempFile(TMP_SIGNED_FILE, TMP_HAP_SUFFIX, TMP_DIR);
        tmpSource.add(new Cleanable(outputFile));

        SignAppParameters signAppParameters = new SignAppParameters();
        signAppParameters.setMode(Mode.LOCAL_SIGN);
        signAppParameters.setKeyAlias("oh-app1-key-v1");
        signAppParameters.setAppCertFile("../../tools/app1.pem");
        signAppParameters.setProfileFile("../../tools/app1-profile.p7b");
        signAppParameters.setInFile("../../tools/test/app1-unsigned.hap");
        signAppParameters.setSignAlg("SHA256withECDSA");
        signAppParameters.setKeyStoreFile("../../tools/ohtest_pass.jks");
        signAppParameters.setKeystorePwd("123456".toCharArray());
        signAppParameters.setOutFile(outputFile.getCanonicalPath());
        Assertions.assertNotEquals(HapSignTool.signApp(signAppParameters).getErrCode(), ERROR.SUCCESS_CODE);
    }

    /**
     * test Param Without AppCertFile
     *
     * @throws IOException IOException
     */
    @Test
    public void testParamWithoutAppCertFile() throws IOException {
        File outputFile = File.createTempFile(TMP_SIGNED_FILE, TMP_HAP_SUFFIX, TMP_DIR);
        tmpSource.add(new Cleanable(outputFile));

        SignAppParameters signAppParameters = new SignAppParameters();
        signAppParameters.setMode(Mode.LOCAL_SIGN);
        signAppParameters.setKeyAlias("oh-app1-key-v1");
        signAppParameters.setKeyPwd("123456".toCharArray());
        signAppParameters.setProfileFile("../../tools/app1-profile.p7b");
        signAppParameters.setInFile("../../tools/test/app1-unsigned.hap");
        signAppParameters.setSignAlg("SHA256withECDSA");
        signAppParameters.setKeyStoreFile("../../tools/ohtest_pass.jks");
        signAppParameters.setKeystorePwd("123456".toCharArray());
        signAppParameters.setOutFile(outputFile.getCanonicalPath());
        Assertions.assertNotEquals(HapSignTool.signApp(signAppParameters).getErrCode(), ERROR.SUCCESS_CODE);
    }

    /**
     * test Param Without KeyStoreFile
     *
     * @throws IOException IOException
     */
    @Test
    public void testParamWithoutKeyStoreFile() throws IOException {
        File outputFile = File.createTempFile(TMP_SIGNED_FILE, TMP_HAP_SUFFIX, TMP_DIR);
        tmpSource.add(new Cleanable(outputFile));

        SignAppParameters signAppParameters = new SignAppParameters();
        signAppParameters.setMode(Mode.LOCAL_SIGN);
        signAppParameters.setKeyAlias("oh-app1-key-v1");
        signAppParameters.setKeyPwd("123456".toCharArray());
        signAppParameters.setProfileFile("../../tools/app1-profile.p7b");
        signAppParameters.setInFile("../../tools/test/app1-unsigned.hap");
        signAppParameters.setSignAlg("SHA256withECDSA");
        signAppParameters.setAppCertFile("../../tools/app1.pem");
        signAppParameters.setKeystorePwd("123456".toCharArray());
        signAppParameters.setOutFile(outputFile.getCanonicalPath());
        Assertions.assertNotEquals(HapSignTool.signApp(signAppParameters).getErrCode(), ERROR.SUCCESS_CODE);
    }

    private static class Cleanable {
        private final File file;

        public Cleanable(File file) {
            this.file = file;
        }

        private void clean() {
            FileUtils.deleteFile(file);
        }
    }
}
