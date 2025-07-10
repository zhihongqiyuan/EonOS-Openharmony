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
import com.ohos.entity.SignCode;
import com.ohos.entity.SignProfileParameters;
import com.ohos.entity.VerifyAppParameters;
import com.ohos.entity.VerifyProfileParameters;
import com.ohos.hapsigntool.HapSignTool;
import com.ohos.hapsigntool.error.ERROR;
import com.ohos.hapsigntool.utils.FileUtils;

import com.ohos.hapsigntool.utils.LogUtils;
import org.junit.jupiter.api.AfterAll;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.RepeatedTest;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.Callable;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.Future;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;

/**
 * Concurrency Test
 *
 * @since 2024/03/18
 */
public class ConcurrencyTest {
    private static final String TMP_UNSIGNED_FILE = "unsigned-";

    private static final String TMP_SIGNED_FILE = "signed-";

    private static final String TMP_HAP_SUFFIX = ".hap";

    private static final String TMP_PROFILE_SUFFIX = ".p7b";

    private static final int CONCURRENT_TASK_COUNT = 100;

    private static final int LOOP_COUNT = 5;

    private static final int KEEP_ALIVE_TIMES = 30;

    private static final int MAX_ENTRY_SIZE = 10 * 1024 * 1024;

    private static final int MIN_ENTRY_SIZE = 1024 * 1024;

    private static final File TMP_DIR = new File("concurrentTest");

    private static final List<Cleanable> tmpSource = new ArrayList<>();
    private static final LogUtils LOG = new LogUtils(ConcurrencyTest.class);

    /**
     * before test
     */
    @BeforeAll
    public static void before() {
        TMP_DIR.mkdirs();
    }

    /**
     * after test
     */
    @AfterAll
    public static void after() {
        for (Cleanable c : tmpSource) {
            c.clean();
        }
    }

    /**
     * test Sign Hap
     *
     * @throws IOException IOException
     * @throws ExecutionException ExecutionException
     * @throws InterruptedException InterruptedException
     */
    @RepeatedTest(2)
    public void testSignHapConcurrent() throws IOException, ExecutionException, InterruptedException {
        executeConcurrentTask();
    }

    private Callable<Boolean> generateSignHapTask(CountDownLatch countDownLatch) throws IOException {
        File inputFile = File.createTempFile(TMP_UNSIGNED_FILE, TMP_HAP_SUFFIX, TMP_DIR);
        File outputFile = File.createTempFile(TMP_SIGNED_FILE, TMP_HAP_SUFFIX, TMP_DIR);
        File profile = File.createTempFile(TMP_SIGNED_FILE, TMP_PROFILE_SUFFIX, TMP_DIR);
        tmpSource.add(new Cleanable(inputFile));
        tmpSource.add(new Cleanable(outputFile));
        tmpSource.add(new Cleanable(profile));
        generateHap(inputFile);
        return new MultiSignHapTask(inputFile, outputFile, profile, countDownLatch);
    }

    private void generateHap(File file) throws IOException {
        try (ZipOutputStream out = new ZipOutputStream(new FileOutputStream(file))) {
            ZipEntry zipEntry = new ZipEntry("test.so");
            zipEntry.setMethod(ZipEntry.DEFLATED);
            byte[] bytes = generateChunkBytes();
            out.putNextEntry(zipEntry);
            out.write(bytes);
            out.closeEntry();
        }
    }

    private byte[] generateChunkBytes() {
        Random random = new Random();
        int size = Math.max(MIN_ENTRY_SIZE, random.nextInt(MAX_ENTRY_SIZE));
        byte[] bytes = new byte[size];
        random.nextBytes(bytes);
        return bytes;
    }

    private void executeConcurrentTask() throws IOException, ExecutionException, InterruptedException {
        CountDownLatch countDownLatch = new CountDownLatch(CONCURRENT_TASK_COUNT);
        ThreadPoolExecutor executor = new ThreadPoolExecutor(CONCURRENT_TASK_COUNT, CONCURRENT_TASK_COUNT,
                KEEP_ALIVE_TIMES, TimeUnit.SECONDS, new ArrayBlockingQueue<>(CONCURRENT_TASK_COUNT),
                new ThreadPoolExecutor.DiscardPolicy());
        List<Future<Boolean>> futures = new ArrayList<>(CONCURRENT_TASK_COUNT);
        for (int i = 0; i < CONCURRENT_TASK_COUNT; i++) {
            futures.add(executor.submit(generateSignHapTask(countDownLatch)));
        }
        executor.shutdown();
        boolean isFinished;
        try {
            isFinished = countDownLatch.await(KEEP_ALIVE_TIMES, TimeUnit.SECONDS);
        } catch (InterruptedException e) {
            isFinished = false;
            LOG.error("concurrency test interrupted", e);
        }
        if (!isFinished) {
            executor.shutdownNow();
        }
        Assertions.assertTrue(isFinished, "task time out : " + KEEP_ALIVE_TIMES + " seconds");
        for (Future<Boolean> f : futures) {
            Boolean isSuccess = f.get();
            Assertions.assertNotNull(isSuccess, "task not finished");
            Assertions.assertTrue(isSuccess, "task failed");
        }
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

    private static class MultiSignHapTask implements Callable<Boolean> {
        private final File inputFile;

        private final File outputFile;

        private final File profile;

        private final CountDownLatch countDownLatch;

        private MultiSignHapTask(File inputFile, File outputFile, File profile, CountDownLatch countDownLatch) {
            this.inputFile = inputFile;
            this.outputFile = outputFile;
            this.profile = profile;
            this.countDownLatch = countDownLatch;
        }

        @Override
        public Boolean call() throws IOException {
            try {
                for (int i = 0; i < LOOP_COUNT; i++) {
                    if (!signProfile()) {
                        return false;
                    }

                    if (!verifyProfile()) {
                        return false;
                    }

                    if (!signApp()) {
                        return false;
                    }
                    if (!verifyApp()) {
                        return false;
                    }
                }
                return true;
            } finally {
                countDownLatch.countDown();
            }
        }

        private boolean signProfile() throws IOException {
            SignProfileParameters signProfileParameters = new SignProfileParameters();
            signProfileParameters.setMode(Mode.LOCAL_SIGN);
            signProfileParameters.setKeyAlias("oh-app1-key-v1");
            signProfileParameters.setKeyPwd("123456".toCharArray());
            signProfileParameters.setProfileCertFile("../../tools/app1.pem");
            signProfileParameters.setInFile("../../tools/profile.json");
            signProfileParameters.setSignAlg("SHA256withECDSA");
            signProfileParameters.setKeyStoreFile("../../tools/ohtest_pass.jks");
            signProfileParameters.setKeystorePwd("123456".toCharArray());
            signProfileParameters.setOutFile(profile.getCanonicalPath());
            return HapSignTool.signProfile(signProfileParameters).getErrCode() == ERROR.SUCCESS_CODE;
        }

        private boolean verifyProfile() throws IOException {
            VerifyProfileParameters verifyProfileParameters = new VerifyProfileParameters();
            verifyProfileParameters.setInFile(profile.getCanonicalPath());
            verifyProfileParameters.setOutFile("out.json");
            return HapSignTool.verifyProfile(verifyProfileParameters).getErrCode() == ERROR.SUCCESS_CODE;
        }

        private boolean signApp() throws IOException {
            SignAppParameters signAppParameters = new SignAppParameters();
            signAppParameters.setMode(Mode.LOCAL_SIGN);
            signAppParameters.setKeyAlias("oh-app1-key-v1");
            signAppParameters.setKeyPwd("123456".toCharArray());
            signAppParameters.setAppCertFile("../../tools/app1.pem");
            signAppParameters.setProfileFile(profile.getCanonicalPath());
            signAppParameters.setInFile(inputFile.getCanonicalPath());
            signAppParameters.setSignAlg("SHA256withECDSA");
            signAppParameters.setKeyStoreFile("../../tools/ohtest_pass.jks");
            signAppParameters.setKeystorePwd("123456".toCharArray());
            signAppParameters.setOutFile(outputFile.getCanonicalPath());
            signAppParameters.setProfileSigned(ProFileSigned.SIGNED);
            signAppParameters.setInForm(InForm.ZIP);
            signAppParameters.setSignCode(SignCode.OPEN);
            return HapSignTool.signApp(signAppParameters).getErrCode() == ERROR.SUCCESS_CODE;
        }

        private boolean verifyApp() throws IOException {
            VerifyAppParameters verifyAppParameters = new VerifyAppParameters();
            verifyAppParameters.setInFile(outputFile.getCanonicalPath());
            verifyAppParameters.setOutCertChain("out.cer");
            verifyAppParameters.setOutProfile("out.p7b");
            return HapSignTool.verifyApp(verifyAppParameters).getErrCode() == ERROR.SUCCESS_CODE;
        }
    }
}
