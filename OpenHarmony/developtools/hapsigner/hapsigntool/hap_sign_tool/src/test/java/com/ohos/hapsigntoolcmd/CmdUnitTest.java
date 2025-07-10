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

package com.ohos.hapsigntoolcmd;

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

import com.ohos.hapsigntool.HapSignTool;
import com.ohos.hapsigntool.codesigning.utils.HapUtils;
import com.ohos.hapsigntool.entity.ParamConstants;
import com.ohos.hapsigntool.error.CustomException;
import com.ohos.hapsigntool.error.ProfileException;
import com.ohos.hapsigntool.utils.KeyPairTools;
import com.ohos.hapsigntool.utils.FileUtils;
import com.ohos.hapsigntool.zip.Zip;

import org.junit.jupiter.api.AfterAll;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.MethodOrderer;
import org.junit.jupiter.api.Order;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.TestMethodOrder;
import org.junit.platform.commons.logging.Logger;
import org.junit.platform.commons.logging.LoggerFactory;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.math.BigInteger;
import java.net.URL;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardCopyOption;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Random;
import java.util.jar.JarFile;
import java.util.zip.CRC32;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;
import java.util.zip.ZipOutputStream;

/**
 * CmdUnitTest.
 *
 * @since 2021/12/28
 */
@TestMethodOrder(MethodOrderer.OrderAnnotation.class)
public class CmdUnitTest {
    /**
     * Command line parameter signCode.
     */
    public static final String CMD_SIGN_CODE = "-signCode";

    /**
     * Command line parameter appCaCertFile.
     */
    public static final String CMD_SUB_CA_CERT_FILE = "-subCaCertFile";

    /**
     * Command line parameter outFile.
     */
    public static final String CMD_OUT_FILE = "-outFile";

    /**
     * Command line parameter basicConstraints.
     */
    public static final String CMD_BASIC_CONSTRAINTS = "-basicConstraints";

    /**
     * Command line parameter basicConstraintsCa.
     */
    public static final String CMD_BASIC_CONSTRAINTS_CA = "-basicConstraintsCa";

    /**
     * Command line parameter basicConstraintsCritical.
     */
    public static final String CMD_BASIC_CONSTRAINTS_CRITICAL = "-basicConstraintsCritical";

    /**
     * Command line parameter basicConstraintsPathLen.
     */
    public static final String CMD_BASIC_CONSTRAINTS_PATH_LEN = "-basicConstraintsPathLen";

    /**
     * Command line parameter caCertFile.
     */
    public static final String CMD_ROOT_CA_CERT_FILE = "-rootCaCertFile";

    /**
     * Command line parameter outForm.
     */
    public static final String CMD_OUT_FORM = "-outForm";

    /**
     * Command line parameter cert.
     */
    public static final String CMD_CERT_CHAIN = "certChain";

    /**
     * Command line parameter digitalSignature.
     */
    public static final String CMD_DIGITAL_SIGNATURE = "digitalSignature";

    /**
     * Command line parameter codeSignature and emailProtection.
     */
    public static final String CMD_CODE_AND_EMAIL = "codeSignature,emailProtection";

    /**
     * Command line parameter mode.
     */
    public static final String CMD_MODE = "-mode";

    /**
     * Command line parameter keystoreFile.
     */
    public static final String CMD_KEY_STORE_FILE = "-keystoreFile";

    /**
     * Command line parameter keystorePwd.
     */
    public static final String CMD_KEY_STORE_RIGHTS = "-keystorePwd";

    /**
     * Command line parameter keyAlg.
     */
    public static final String CMD_KEY_ALG = "-keyAlg";

    /**
     * Command line parameter keyAlias.
     */
    public static final String CMD_KEY_ALIAS = "-keyAlias";

    /**
     * Command line parameter keyPwd.
     */
    public static final String CMD_KEY_RIGHTS = "-keyPwd";

    /**
     * Command line parameter keySize.
     */
    public static final String CMD_KEY_SIZE = "-keySize";

    /**
     * Command line parameter keyUsage.
     */
    public static final String CMD_KEY_USAGE = "-keyUsage";

    /**
     * Command line parameter keyUsageCritical.
     */
    public static final String CMD_KEY_USAGE_CRITICAL = "-keyUsageCritical";

    /**
     * Command line parameter extKeyUsage.
     */
    public static final String CMD_EXT_KEY_USAGE = "-extKeyUsage";

    /**
     * Command line parameter extKeyUsageCritical.
     */
    public static final String CMD_EXT_KEY_USAGE_CRITICAL = "-extKeyUsageCritical";

    /**
     * Command line parameter profileCertFile.
     */
    public static final String CMD_PROFILE_CERT_FILE = "-profileCertFile";

    /**
     * Command line parameter subject.
     */
    public static final String CMD_SUBJECT = "-subject";

    /**
     * Command line parameter signAlg.
     */
    public static final String CMD_SIGN_ALG = "-signAlg";

    /**
     * Command line parameter inFile.
     */
    public static final String CMD_IN_FILE = "-inFile";

    /**
     * Command line parameter issuer.
     */
    public static final String CMD_ISSUER = "-issuer";

    /**
     * Command line parameter issuerKeyAlias.
     */
    public static final String CMD_ISSUER_KEY_ALIAS = "-issuerKeyAlias";

    /**
     * Command line parameter issuerKeyPwd.
     */
    public static final String CMD_ISSUER_KEY_RIGHTS = "-issuerKeyPwd";

    /**
     * Command line parameter validity.
     */
    public static final String CMD_VALIDITY = "-validity";

    /**
     * Command line parameter appCertFile.
     */
    public static final String CMD_APP_CERT_FILE = "-appCertFile";

    /**
     * Command line parameter appCertFile.
     */
    public static final String CMD_PROFILE_FILE = "-profileFile";

    /**
     * Command line parameter appCertFile.
     */
    public static final String CMD_OUT_CERT_CHAIN = "-outCertChain";

    /**
     * Command line parameter appCertFile.
     */
    public static final String CMD_OUT_PROFILE = "-outProfile";

    /**
     * Command line parameter false.
     */
    public static final String CMD_FALSE = "false";

    /**
     * Command line parameter true.
     */
    public static final String CMD_TRUE = "true";

    /**
     * Command line parameter basicConstraintsPathLen is 0.
     */
    public static final String CMD_BC_PATH_LEN_0 = "0";

    /**
     * Command line parameter password is 123456.
     */
    public static final String CMD_RIGHTS_123456 = "123456";

    /**
     * Command line parameter RSA is 2048.
     */
    public static final String CMD_RSA_2048 = "2048";

    /**
     * Command line parameter ECC is 256.
     */
    public static final String CMD_ECC_256 = "NIST-P-256";

    /**
     * Command line parameter validity is 365.
     */
    public static final String CMD_VALIDITY_365 = "365";

    /**
     * Command line parameter json file is UnsgnedDebugProfileTemplate.
     */
    public static final String CMD_JSON_FILE = "UnsgnedDebugProfileTemplate.json";

    /**
     * Command line parameter json file is UnsgnedReleaseProfileTemplate.
     */
    public static final String CMD_RELEASE_JSON_FILE = "UnsgnedReleaseProfileTemplate.json";

    /**
     * Command line parameter localSign.
     */
    public static final String CMD_LOCAL_SIGN = "localSign";

    /**
     * Command line parameter SHA256withECDSA.
     */
    public static final String CMD_SHA_256_WITH_ECDSA = "SHA256withECDSA";

    /**
     * Command line parameter cer file is test_app-debug-cert.
     */
    public static final String CMD_APP_DEBUG_CERT_PATH = "test_app-debug-cert.pem";

    /**
     * Command line parameter cer file is test_app-release-cert.
     */
    public static final String CMD_APP_RELEASE_CERT_PATH = "test_app-release-cert.pem";

    /**
     * Command line parameter cer file is test_cert.
     */
    public static final String CMD_CERT_PATH = "test_cert.cer";

    /**
     * Command line parameter csr file is test_csr.
     */
    public static final String CMD_CSR_PATH = "test_csr.csr";

    /**
     * Command line parameter jks file is test_app_csr.
     */
    public static final String CMD_KEY_APP_STORE_PATH = "test_app_keypair.jks";

    /**
     * Command line parameter jks file is test_profile_csr.
     */
    public static final String CMD_KEY_PROFILE_STORE_PATH = "test_profile_keypair.jks";

    /**
     * Command line parameter cer file is test_root_app_ca.
     */
    public static final String CMD_ROOT_APP_CA_PATH = "test_root_app_ca.cer";

    /**
     * Command line parameter cer file is test_root_profile_ca.
     */
    public static final String CMD_ROOT_PROFILE_CA_PATH = "test_root_profile_ca.cer";

    /**
     * Command line parameter cer file is test_sub_app_ca.
     */
    public static final String CMD_SUB_APP_CA_PATH = "test_sub_app_ca.cer";

    /**
     * Command line parameter cer file is test_sub_profile_ca.
     */
    public static final String CMD_SUB_PROFILE_CA_PATH = "test_sub_profile_ca.cer";

    /**
     * Command line parameter p7b file is test_sign_profile.
     */
    public static final String CMD_SIGN_PROFILE_PATH = "test_sign_profile.p7b";

    /**
     * Command line parameter p7b file is test_sign_profile.
     */
    public static final String CMD_SIGN_RELEASE_PROFILE_PATH = "test_sign_release_profile.p7b";

    /**
     * Command line parameter cer file is test_profile-debug-cert.
     */
    public static final String CMD_PROFILE_DEBUG_CERT_PATH = "test_profile-debug-cert.pem";

    /**
     * Command line parameter cer file is test_profile-release-cert.
     */
    public static final String CMD_PROFILE_RELEASE_CERT_PATH = "test_profile-release-cert.pem";

    /**
     * Command line parameter cer file is test_verify_profile.
     */
    public static final String CMD_VERIFY_PROFILE_RESULT_PATH = "test_verify_profile_result.json";

    /**
     * Command line parameter cer file is test_verify_profile.
     */
    public static final String CMD_TEST_HAP_FILE_PATH = "entry-default-unsigned.hap";

    /**
     * Command line parameter oh-profile-key-v1.
     */
    public static final String CMD_OH_PROFILE_KEY_V1 = "oh-profile-key-v1";

    /**
     * Command line parameter oh-app1-key-v1.
     */
    public static final String CMD_OH_APP1_KEY_V1 = "oh-app1-key-v1";

    /**
     * Command line parameter oh-root-ca-key-v1.
     */
    public static final String CMD_OH_ROOT_CA_KEY_V1 = "oh-root-ca-key-v1";

    /**
     * Command line parameter oh-sub-app-ca-key-v1.
     */
    public static final String CMD_OH_SUB_APP_CA_KEY_V1 = "oh-sub-app-ca-key-v1";

    /**
     * Command line parameter oh-sub-profile-ca-key-v1.
     */
    public static final String CMD_OH_SUB_PROFILE_CA_KEY_V1 = "oh-sub-profile-ca-key-v1";

    /**
     * Command line parameter CN=ROOT CA.
     */
    public static final String CMD_ROOT_CA = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=ROOT CA";

    /**
     * Command line parameter CN=App1 Release.
     */
    public static final String CMD_APP1_RELEASE = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";

    /**
     * Command line parameter CN=Provision Profile Release.
     */
    public static final String CMD_PROFILE_RELEASE = "C=CN,O=OpenHarmony,"
            + "OU=OpenHarmony Community,CN=Provision Profile Release";

    /**
     * Command line parameter CN=Provision Profile Signature Service CA.
     */
    public static final String CMD_PROFILE_CA = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,"
            + "CN=Provision Profile Signature Service CA";

    /**
     * Command line parameter CN=Application Signature Service CA.
     */
    public static final String CMD_APP_CA = "C=CN,"
            + "O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";

    /**
     * Add log info.
     */
    private static final Logger LOGGER = LoggerFactory.getLogger(CmdUnitTest.class);

    /**
     * create test dir
     */
    @BeforeAll
    public static void mkTestDir() {
        File dir = new File("test");
        dir.mkdir();
    }

    /**
     * delete test dir
     */
    @AfterAll
    public static void delTestDir() {
        File dir = new File("test");
        if (dir.exists()) {
            for (File file : dir.listFiles()) {
                file.delete();
            }
            dir.delete();
        }
    }

    /**
     * test cmdKeypair
     *
     * @throws IOException io error
     */
    @Order(1)
    @Test
    public void testCmdKeypair() throws IOException {
        try {
            deleteFile(CMD_KEY_APP_STORE_PATH);
            deleteFile(CMD_KEY_PROFILE_STORE_PATH);
            boolean result = HapSignTool.processCmd(new String[]{CmdUtil.Method.GENERATE_KEYPAIR});
            assertFalse(result);
            assertFalse(FileUtils.isFileExist(CMD_KEY_APP_STORE_PATH));
            assertFalse(FileUtils.isFileExist(CMD_KEY_PROFILE_STORE_PATH));
        } catch (CustomException exception) {
            LOGGER.info(exception, () -> exception.getMessage());
        }

        deleteFile(CMD_KEY_APP_STORE_PATH);
        boolean result = HapSignTool.processCmd(new String[]{
            CmdUtil.Method.GENERATE_KEYPAIR,
            CMD_KEY_ALIAS, CMD_OH_APP1_KEY_V1,
            CMD_KEY_RIGHTS, CMD_RIGHTS_123456,
            CMD_KEY_ALG, KeyPairTools.ECC_INPUT,
            CMD_KEY_SIZE, CMD_ECC_256,
            CMD_KEY_STORE_FILE, CMD_KEY_APP_STORE_PATH,
            CMD_KEY_STORE_RIGHTS, CMD_RIGHTS_123456});
        assertTrue(result);
        assertTrue(FileUtils.isFileExist(CMD_KEY_APP_STORE_PATH));
        deleteFile(CMD_KEY_PROFILE_STORE_PATH);
        result = HapSignTool.processCmd(new String[]{
            CmdUtil.Method.GENERATE_KEYPAIR,
            CMD_KEY_ALIAS, CMD_OH_PROFILE_KEY_V1,
            CMD_KEY_RIGHTS, CMD_RIGHTS_123456,
            CMD_KEY_ALG, KeyPairTools.ECC_INPUT,
            CMD_KEY_SIZE, CMD_ECC_256,
            CMD_KEY_STORE_FILE, CMD_KEY_PROFILE_STORE_PATH,
            CMD_KEY_STORE_RIGHTS, CMD_RIGHTS_123456});
        assertTrue(result);
        assertTrue(FileUtils.isFileExist(CMD_KEY_PROFILE_STORE_PATH));
    }

    /**
     * Csr test case.
     *
     * @throws IOException Error
     */
    @Order(2)
    @Test
    public void testCmdCsr() throws IOException {
        try {
            deleteFile(CMD_CSR_PATH);
            boolean result = HapSignTool.processCmd(new String[]{CmdUtil.Method.GENERATE_CSR});
            assertFalse(result);
            assertFalse(FileUtils.isFileExist(CMD_CSR_PATH));
        } catch (CustomException exception) {
            LOGGER.info(exception, () -> exception.getMessage());
        }

        deleteFile(CMD_CSR_PATH);
        boolean result = HapSignTool.processCmd(new String[]{
            CmdUtil.Method.GENERATE_CSR,
            CMD_KEY_ALIAS, CMD_OH_APP1_KEY_V1,
            CMD_KEY_RIGHTS, CMD_RIGHTS_123456,
            CMD_SUBJECT, CMD_APP1_RELEASE,
            CMD_SIGN_ALG, CMD_SHA_256_WITH_ECDSA,
            CMD_KEY_STORE_FILE, CMD_KEY_APP_STORE_PATH,
            CMD_KEY_STORE_RIGHTS, CMD_RIGHTS_123456,
            CMD_OUT_FILE, CMD_CSR_PATH});
        assertTrue(result);
        assertTrue(FileUtils.isFileExist(CMD_CSR_PATH));
    }

    /**
     * Cert test case
     *
     * @throws IOException Error
     */
    @Order(3)
    @Test
    public void testCmdCert() throws IOException {
        try {
            deleteFile(CMD_CERT_PATH);
            boolean result = HapSignTool.processCmd(new String[]{CmdUtil.Method.GENERATE_CERT});
            assertFalse(result);
            assertFalse(FileUtils.isFileExist(CMD_CERT_PATH));
        } catch (CustomException exception) {
            LOGGER.info(exception, () -> exception.getMessage());
        }

        deleteFile(CMD_CERT_PATH);
        boolean result = HapSignTool.processCmd(new String[]{
            CmdUtil.Method.GENERATE_CERT,
            CMD_KEY_ALIAS, CMD_OH_APP1_KEY_V1,
            CMD_KEY_RIGHTS, CMD_RIGHTS_123456,
            CMD_ISSUER, CMD_APP_CA,
            CMD_SIGN_ALG, CMD_SHA_256_WITH_ECDSA,
            CMD_KEY_STORE_FILE, CMD_KEY_APP_STORE_PATH,
            CMD_KEY_STORE_RIGHTS, CMD_RIGHTS_123456,
            CMD_OUT_FILE, CMD_CERT_PATH,
            CMD_ISSUER_KEY_ALIAS, CMD_OH_APP1_KEY_V1,
            CMD_ISSUER_KEY_RIGHTS, CMD_RIGHTS_123456,
            CMD_SUBJECT, CMD_APP1_RELEASE,
            CMD_VALIDITY, CMD_VALIDITY_365,
            CMD_KEY_USAGE, CMD_DIGITAL_SIGNATURE,
            CMD_KEY_USAGE_CRITICAL, CMD_FALSE,
            CMD_EXT_KEY_USAGE, CMD_CODE_AND_EMAIL,
            CMD_EXT_KEY_USAGE_CRITICAL, CMD_TRUE,
            CMD_BASIC_CONSTRAINTS, CMD_FALSE,
            CMD_BASIC_CONSTRAINTS_CRITICAL, CMD_TRUE,
            CMD_BASIC_CONSTRAINTS_CA, CMD_FALSE,
            CMD_BASIC_CONSTRAINTS_PATH_LEN, CMD_BC_PATH_LEN_0});
        assertTrue(result);
        assertTrue(FileUtils.isFileExist(CMD_CERT_PATH));
    }

    /**
     * Ca test case.
     *
     * @throws IOException Error
     */
    @Order(4)
    @Test
    public void testCmdCa() throws IOException {
        try {
            deleteFile(CMD_ROOT_APP_CA_PATH);
            deleteFile(CMD_ROOT_PROFILE_CA_PATH);
            deleteFile(CMD_SUB_APP_CA_PATH);
            deleteFile(CMD_SUB_PROFILE_CA_PATH);
            boolean result = HapSignTool.processCmd(new String[]{CmdUtil.Method.GENERATE_CA});
            assertFalse(result);
            assertFalse(FileUtils.isFileExist(CMD_ROOT_APP_CA_PATH));
            assertFalse(FileUtils.isFileExist(CMD_ROOT_PROFILE_CA_PATH));
            assertFalse(FileUtils.isFileExist(CMD_SUB_APP_CA_PATH));
            assertFalse(FileUtils.isFileExist(CMD_SUB_PROFILE_CA_PATH));
        } catch (CustomException exception) {
            LOGGER.info(exception, () -> exception.getMessage());
        }
        deleteFile(CMD_ROOT_APP_CA_PATH);
        boolean result = generateAppRootCa();
        assertTrue(result);
        assertTrue(FileUtils.isFileExist(CMD_ROOT_APP_CA_PATH));
        deleteFile(CMD_ROOT_PROFILE_CA_PATH);
        result = generateProfileRootCa();
        assertTrue(result);
        assertTrue(FileUtils.isFileExist(CMD_ROOT_PROFILE_CA_PATH));
        deleteFile(CMD_SUB_APP_CA_PATH);
        result = generateAppSubCa();
        assertTrue(result);
        assertTrue(FileUtils.isFileExist(CMD_SUB_APP_CA_PATH));
        deleteFile(CMD_SUB_PROFILE_CA_PATH);
        result = generateProfileSubCa();
        assertTrue(result);
        assertTrue(FileUtils.isFileExist(CMD_SUB_PROFILE_CA_PATH));
    }

    /**
     * App cert test case.
     *
     * @throws IOException Error
     */
    @Order(5)
    @Test
    public void testCmdAppCert() throws IOException {
        try {
            deleteFile(CMD_APP_DEBUG_CERT_PATH);
            deleteFile(CMD_APP_RELEASE_CERT_PATH);
            boolean result = HapSignTool.processCmd(new String[]{CmdUtil.Method.GENERATE_APP_CERT});
            assertFalse(result);
            assertFalse(FileUtils.isFileExist(CMD_APP_DEBUG_CERT_PATH));
            assertFalse(FileUtils.isFileExist(CMD_APP_RELEASE_CERT_PATH));
        } catch (CustomException exception) {
            LOGGER.info(exception, () -> exception.getMessage());
        }
        deleteFile(CMD_APP_DEBUG_CERT_PATH);
        boolean result = HapSignTool.processCmd(new String[]{
            CmdUtil.Method.GENERATE_APP_CERT,
            CMD_KEY_ALIAS, CMD_OH_APP1_KEY_V1,
            CMD_KEY_RIGHTS, CMD_RIGHTS_123456,
            CMD_ISSUER, CMD_APP_CA,
            CMD_KEY_STORE_FILE, CMD_KEY_APP_STORE_PATH,
            CMD_KEY_STORE_RIGHTS, CMD_RIGHTS_123456,
            CMD_OUT_FILE, CMD_APP_DEBUG_CERT_PATH,
            CMD_ISSUER_KEY_ALIAS, CMD_OH_SUB_APP_CA_KEY_V1,
            CMD_ISSUER_KEY_RIGHTS, CMD_RIGHTS_123456,
            CMD_SUBJECT, CMD_APP1_RELEASE,
            CMD_VALIDITY, CMD_VALIDITY_365,
            CMD_OUT_FORM, CMD_CERT_CHAIN,
            CMD_ROOT_CA_CERT_FILE, CMD_ROOT_APP_CA_PATH,
            CMD_SUB_CA_CERT_FILE, CMD_SUB_APP_CA_PATH,
            CMD_SIGN_ALG, CMD_SHA_256_WITH_ECDSA});
        assertTrue(result);
        assertTrue(FileUtils.isFileExist(CMD_APP_DEBUG_CERT_PATH));
        deleteFile(CMD_APP_RELEASE_CERT_PATH);
        result = HapSignTool.processCmd(new String[]{
            CmdUtil.Method.GENERATE_APP_CERT,
            CMD_KEY_ALIAS, CMD_OH_APP1_KEY_V1,
            CMD_KEY_RIGHTS, CMD_RIGHTS_123456,
            CMD_ISSUER, CMD_APP_CA,
            CMD_KEY_STORE_FILE, CMD_KEY_APP_STORE_PATH,
            CMD_KEY_STORE_RIGHTS, CMD_RIGHTS_123456,
            CMD_OUT_FILE, CMD_APP_RELEASE_CERT_PATH,
            CMD_ISSUER_KEY_ALIAS, CMD_OH_SUB_APP_CA_KEY_V1,
            CMD_ISSUER_KEY_RIGHTS, CMD_RIGHTS_123456,
            CMD_SUBJECT, CMD_APP1_RELEASE,
            CMD_VALIDITY, CMD_VALIDITY_365,
            CMD_OUT_FORM, CMD_CERT_CHAIN,
            CMD_ROOT_CA_CERT_FILE, CMD_ROOT_APP_CA_PATH,
            CMD_SUB_CA_CERT_FILE, CMD_SUB_APP_CA_PATH,
            CMD_SIGN_ALG, CMD_SHA_256_WITH_ECDSA});
        assertTrue(result);
        assertTrue(FileUtils.isFileExist(CMD_APP_RELEASE_CERT_PATH));
    }

    /**
     * Profile cert test case
     *
     * @throws IOException Error
     */
    @Order(6)
    @Test
    public void testCmdProfileCert() throws IOException {
        try {
            deleteFile(CMD_PROFILE_DEBUG_CERT_PATH);
            deleteFile(CMD_PROFILE_RELEASE_CERT_PATH);
            boolean result = HapSignTool.processCmd(new String[]{CmdUtil.Method.GENERATE_PROFILE_CERT});
            assertFalse(result);
            assertFalse(FileUtils.isFileExist(CMD_PROFILE_DEBUG_CERT_PATH));
            assertFalse(FileUtils.isFileExist(CMD_PROFILE_RELEASE_CERT_PATH));
        } catch (CustomException exception) {
            LOGGER.info(exception, () -> exception.getMessage());
        }
        deleteFile(CMD_PROFILE_DEBUG_CERT_PATH);
        boolean result = HapSignTool.processCmd(new String[]{
            CmdUtil.Method.GENERATE_PROFILE_CERT,
            CMD_KEY_ALIAS, CMD_OH_PROFILE_KEY_V1,
            CMD_KEY_RIGHTS, CMD_RIGHTS_123456,
            CMD_ISSUER, CMD_PROFILE_CA,
            CMD_KEY_STORE_FILE, CMD_KEY_PROFILE_STORE_PATH,
            CMD_KEY_STORE_RIGHTS, CMD_RIGHTS_123456,
            CMD_OUT_FILE, CMD_PROFILE_DEBUG_CERT_PATH,
            CMD_ISSUER_KEY_ALIAS, CMD_OH_SUB_PROFILE_CA_KEY_V1,
            CMD_ISSUER_KEY_RIGHTS, CMD_RIGHTS_123456,
            CMD_SUBJECT, CMD_PROFILE_RELEASE,
            CMD_VALIDITY, CMD_VALIDITY_365,
            CMD_OUT_FORM, CMD_CERT_CHAIN,
            CMD_ROOT_CA_CERT_FILE, CMD_ROOT_PROFILE_CA_PATH,
            CMD_SUB_CA_CERT_FILE, CMD_SUB_PROFILE_CA_PATH,
            CMD_SIGN_ALG, CMD_SHA_256_WITH_ECDSA});
        assertTrue(result);
        assertTrue(FileUtils.isFileExist(CMD_PROFILE_DEBUG_CERT_PATH));
        deleteFile(CMD_PROFILE_RELEASE_CERT_PATH);
        result = HapSignTool.processCmd(new String[]{
            CmdUtil.Method.GENERATE_PROFILE_CERT,
            CMD_KEY_ALIAS, CMD_OH_PROFILE_KEY_V1,
            CMD_KEY_RIGHTS, CMD_RIGHTS_123456,
            CMD_ISSUER, CMD_PROFILE_CA,
            CMD_KEY_STORE_FILE, CMD_KEY_PROFILE_STORE_PATH,
            CMD_KEY_STORE_RIGHTS, CMD_RIGHTS_123456,
            CMD_OUT_FILE, CMD_PROFILE_RELEASE_CERT_PATH,
            CMD_ISSUER_KEY_ALIAS, CMD_OH_SUB_PROFILE_CA_KEY_V1,
            CMD_ISSUER_KEY_RIGHTS, CMD_RIGHTS_123456,
            CMD_SUBJECT, CMD_PROFILE_RELEASE,
            CMD_VALIDITY, CMD_VALIDITY_365,
            CMD_OUT_FORM, CMD_CERT_CHAIN,
            CMD_ROOT_CA_CERT_FILE, CMD_ROOT_PROFILE_CA_PATH,
            CMD_SUB_CA_CERT_FILE, CMD_SUB_PROFILE_CA_PATH,
            CMD_SIGN_ALG, CMD_SHA_256_WITH_ECDSA});
        assertTrue(result);
        assertTrue(FileUtils.isFileExist(CMD_PROFILE_RELEASE_CERT_PATH));
    }

    /**
     * Sign profile test case.
     *
     * @throws IOException error
     */
    @Order(7)
    @Test
    public void testCmdSignProfile() throws IOException {
        try {
            deleteFile(CMD_SIGN_PROFILE_PATH);
            boolean result = HapSignTool.processCmd(new String[]{CmdUtil.Method.SIGN_PROFILE});
            assertFalse(result);
            assertFalse(FileUtils.isFileExist(CMD_SIGN_PROFILE_PATH));
        } catch (CustomException exception) {
            LOGGER.info(exception, () -> exception.getMessage());
        }

        deleteFile(CMD_SIGN_PROFILE_PATH);
        loadFile(CMD_JSON_FILE);
        boolean result = HapSignTool.processCmd(new String[]{
            CmdUtil.Method.SIGN_PROFILE,
            CMD_MODE, CMD_LOCAL_SIGN,
            CMD_KEY_ALIAS, CMD_OH_PROFILE_KEY_V1,
            CMD_KEY_RIGHTS, CMD_RIGHTS_123456,
            CMD_PROFILE_CERT_FILE, CMD_PROFILE_DEBUG_CERT_PATH,
            CMD_IN_FILE, CMD_JSON_FILE,
            CMD_SIGN_ALG, CMD_SHA_256_WITH_ECDSA,
            CMD_KEY_STORE_FILE, CMD_KEY_PROFILE_STORE_PATH,
            CMD_KEY_STORE_RIGHTS, CMD_RIGHTS_123456,
            CMD_OUT_FILE, CMD_SIGN_PROFILE_PATH});
        assertTrue(result);
        assertTrue(FileUtils.isFileExist(CMD_SIGN_PROFILE_PATH));

        deleteFile(CMD_SIGN_RELEASE_PROFILE_PATH);
        loadFile(CMD_RELEASE_JSON_FILE);
        result = HapSignTool.processCmd(new String[]{
            CmdUtil.Method.SIGN_PROFILE,
            CMD_MODE, CMD_LOCAL_SIGN,
            CMD_KEY_ALIAS, CMD_OH_PROFILE_KEY_V1,
            CMD_KEY_RIGHTS, CMD_RIGHTS_123456,
            CMD_PROFILE_CERT_FILE, CMD_PROFILE_RELEASE_CERT_PATH,
            CMD_IN_FILE, CMD_RELEASE_JSON_FILE,
            CMD_SIGN_ALG, CMD_SHA_256_WITH_ECDSA,
            CMD_KEY_STORE_FILE, CMD_KEY_PROFILE_STORE_PATH,
            CMD_KEY_STORE_RIGHTS, CMD_RIGHTS_123456,
            CMD_OUT_FILE, CMD_SIGN_RELEASE_PROFILE_PATH});
        assertTrue(result);
        assertTrue(FileUtils.isFileExist(CMD_SIGN_RELEASE_PROFILE_PATH));
    }

    /**
     * Verify profile test case.
     */
    @Order(8)
    @Test
    public void testVerifyProfile() {
        try {
            boolean result = HapSignTool.processCmd(new String[]{CmdUtil.Method.VERIFY_PROFILE});
            assertFalse(result);
        } catch (CustomException exception) {
            LOGGER.info(exception, () -> exception.getMessage());
        }

        boolean result = HapSignTool.processCmd(new String[]{
            CmdUtil.Method.VERIFY_PROFILE,
            CMD_IN_FILE, CMD_SIGN_PROFILE_PATH,
            CMD_OUT_FILE, CMD_VERIFY_PROFILE_RESULT_PATH});
        assertTrue(result);
    }

    /**
     * Sign hap test case.
     */
    @Order(9)
    @Test
    public void testCmdSignApp() {
        try {
            boolean result = HapSignTool.processCmd(new String[]{CmdUtil.Method.SIGN_APP});
            assertFalse(result);
        } catch (CustomException exception) {
            LOGGER.info(exception, () -> exception.getMessage());
        }
    }

    /**
     * Verify signed app test case.
     */
    @Order(10)
    @Test
    public void testCmdVerifyApp() {
        try {
            boolean result = HapSignTool.processCmd(new String[]{CmdUtil.Method.VERIFY_APP});
            assertFalse(result);
        } catch (CustomException exception) {
            LOGGER.info(exception, () -> exception.getMessage());
        }
    }

    /**
     * test sign and verify hap file include multi lib
     *
     * @throws IOException error
     */
    @Order(11)
    @Test
    public void testCmdMultiHap() throws IOException {
        multiBundleTest(".hap");
    }

    /**
     * test sign and verify hqf file include multi lib
     *
     * @throws IOException error
     */
    @Order(12)
    @Test
    public void testCmdMultiHqf() throws IOException {
        multiBundleTest(".hqf");
    }

    @Order(13)
    @Test
    public void testCmdMultiHnp() throws IOException {
        loadFile(CMD_TEST_HAP_FILE_PATH);
        File unsignedHap = new File(CMD_TEST_HAP_FILE_PATH);
        signAndVerifyHap(unsignedHap.getAbsolutePath(), ".hap");
    }

    @Order(14)
    @Test
    public void testNoCodeSignHap() throws IOException {
        File unsignedHap = generateHapFile(FileType.FILE_UNCOMPRESSED, FileType.FILE_UNCOMPRESSED,
            FileType.FILE_UNCOMPRESSED, FileType.FILE_UNCOMPRESSED, ".hap");
        String signedHap = File.createTempFile("signed-", ".hap", new File("test")).getAbsolutePath();

        boolean result = HapSignTool.processCmd(new String[] {
            CmdUtil.Method.SIGN_APP,
            CMD_MODE, CMD_LOCAL_SIGN,
            CMD_KEY_ALIAS, CMD_OH_APP1_KEY_V1,
            CMD_KEY_RIGHTS, CMD_RIGHTS_123456,
            CMD_APP_CERT_FILE, CMD_APP_DEBUG_CERT_PATH,
            CMD_PROFILE_FILE, CMD_SIGN_PROFILE_PATH,
            CMD_SIGN_ALG, CMD_SHA_256_WITH_ECDSA,
            CMD_KEY_STORE_FILE, CMD_KEY_APP_STORE_PATH,
            CMD_KEY_STORE_RIGHTS, CMD_RIGHTS_123456,
            CMD_IN_FILE, unsignedHap.getAbsolutePath(),
            CMD_OUT_FILE, signedHap,
            CMD_SIGN_CODE, ParamConstants.SignCodeFlag.DISABLE_SIGN_CODE.getSignCodeFlag()
        });
        assertTrue(result);
        result = HapSignTool.processCmd(new String[] {
            CmdUtil.Method.VERIFY_APP, CMD_IN_FILE, signedHap, CMD_OUT_CERT_CHAIN, "test" + File.separator + "1.cer",
            CMD_OUT_PROFILE, "test" + File.separator + "1.p7b"
        });
        assertTrue(result);
        assertFalse(existPagesInfoFile(signedHap));
    }

    @Order(15)
    @Test
    public void testNoRunnableFileHap() throws IOException {
        File unsignedHap = generateHapFile(FileType.FILE_NOT_EXISTED, FileType.FILE_NOT_EXISTED,
            FileType.FILE_UNCOMPRESSED, FileType.FILE_UNCOMPRESSED, ".hap");
        String signedHap = signAndVerifyHap(unsignedHap.getAbsolutePath(), ".hap");
        assertFalse(existPagesInfoFile(signedHap));
    }

    @Order(16)
    @Test
    public void testUncompressedRunnableFileHap() throws IOException {
        File unsignedHap = generateHapFile(FileType.FILE_UNCOMPRESSED, FileType.FILE_UNCOMPRESSED,
            FileType.FILE_UNCOMPRESSED, FileType.FILE_UNCOMPRESSED, ".hap");
        String signedHap = signAndVerifyHap(unsignedHap.getAbsolutePath(), ".hap");
        assertTrue(existPagesInfoFile(signedHap));
    }

    @Order(17)
    @Test
    public void testCompressedRunnableFileHap() throws IOException {
        File unsignedHap = generateHapFile(FileType.FILE_COMPRESSED, FileType.FILE_COMPRESSED,
            FileType.FILE_COMPRESSED, FileType.FILE_UNCOMPRESSED, ".hap");
        String signedHap = signAndVerifyHap(unsignedHap.getAbsolutePath(), ".hap");
        assertFalse(existPagesInfoFile(signedHap));
    }

    private void multiBundleTest(String bundleSuffix) throws IOException {
        for (FileType abcFile : FileType.values()) {
            for (FileType soFile : FileType.values()) {
                for (FileType anFile : FileType.values()) {
                    File unsignedHap = generateHapFile(abcFile, soFile, anFile, FileType.FILE_NOT_EXISTED,
                        bundleSuffix);
                    signAndVerifyHap(unsignedHap.getAbsolutePath(), bundleSuffix);

                    unsignedHap = generateHapFile(abcFile, soFile, anFile, FileType.FILE_UNCOMPRESSED, bundleSuffix);
                    signAndVerifyHap(unsignedHap.getAbsolutePath(), bundleSuffix);

                    unsignedHap = generateHapFile(abcFile, soFile, anFile, FileType.FILE_COMPRESSED, bundleSuffix);
                    signAndVerifyHap(unsignedHap.getAbsolutePath(), bundleSuffix);
                }
            }
        }
    }

    private File generateHapFile(FileType abc, FileType so, FileType an, FileType otherFile, String bundleSuffix)
        throws IOException {
        File unsignedHap = File.createTempFile("unsigned-", bundleSuffix, new File("test"));
        try (ZipOutputStream out = new ZipOutputStream(new FileOutputStream(unsignedHap))) {
            for (int i = 0; i < 10; i++) {
                fillZipEntryFile(abc, ".abc", out);
                if (".hqf".equals(bundleSuffix)) {
                    fillZipEntryFile(so, ".so.diff", out);
                }
                fillZipEntryFile(so, ".solib", out);
                fillZipEntryFile(so, ".so", out);
                fillZipEntryFile(so, ".so.111", out);
                fillZipEntryFile(so, ".so.111.111", out);
                fillZipEntryFile(so, ".so.111.111.111", out);
                fillZipEntryFile(an, ".an", out);
                fillZipEntryFile(otherFile, ".json", out);
            }
        }
        return unsignedHap;
    }

    private void fillZipEntryFile(FileType ft, String suffix, ZipOutputStream out) throws IOException {
        String tempSuffix = suffix;
        if (FileType.FILE_NOT_EXISTED.equals(ft)) {
            tempSuffix = "";
        }
        String fileName = new BigInteger(Long.SIZE, new Random()).toString() + tempSuffix;
        if (tempSuffix.startsWith(".so")) {
            fileName = "libs/" + fileName;
        }
        if (tempSuffix.startsWith(".an")) {
            fileName = "an/" + fileName;
        }
        ZipEntry zipEntry = new ZipEntry(fileName);
        byte[] bytes = generateChunkBytes();
        if (FileType.FILE_UNCOMPRESSED.equals(ft)) {
            zipEntry.setMethod(ZipEntry.STORED);
            zipEntry.setSize(bytes.length);
            CRC32 crc32 = new CRC32();
            crc32.reset();
            crc32.update(bytes, 0, bytes.length);
            zipEntry.setCrc(crc32.getValue());
        } else {
            zipEntry.setMethod(ZipEntry.DEFLATED);
        }
        out.putNextEntry(zipEntry);
        out.write(bytes);
        out.closeEntry();
    }

    private byte[] generateChunkBytes() {
        Random random = new Random();
        int size = Math.max(4096, random.nextInt(1024 * 1024 * 2));
        byte[] bytes = new byte[size];
        random.nextBytes(bytes);
        return bytes;
    }

    private String signAndVerifyHap(String unsignedHap, String bundleSuffix) throws IOException {
        String signedHap = File.createTempFile("signed-", bundleSuffix, new File("test")).getAbsolutePath();
        // debug
        boolean result = HapSignTool.processCmd(new String[] {
            CmdUtil.Method.SIGN_APP,
            CMD_MODE, CMD_LOCAL_SIGN,
            CMD_KEY_ALIAS, CMD_OH_APP1_KEY_V1,
            CMD_KEY_RIGHTS, CMD_RIGHTS_123456,
            CMD_APP_CERT_FILE, CMD_APP_DEBUG_CERT_PATH,
            CMD_PROFILE_FILE, CMD_SIGN_PROFILE_PATH,
            CMD_SIGN_ALG, CMD_SHA_256_WITH_ECDSA,
            CMD_KEY_STORE_FILE, CMD_KEY_APP_STORE_PATH,
            CMD_KEY_STORE_RIGHTS, CMD_RIGHTS_123456,
            CMD_IN_FILE, unsignedHap,
            CMD_OUT_FILE, signedHap
        });
        assertTrue(result);

        result = HapSignTool.processCmd(new String[] {
            CmdUtil.Method.VERIFY_APP, CMD_IN_FILE, signedHap, CMD_OUT_CERT_CHAIN, "test" + File.separator + "1.cer",
            CMD_OUT_PROFILE, "test" + File.separator + "1.p7b"
        });
        assertTrue(result);
        // release
        result = HapSignTool.processCmd(new String[] {
            CmdUtil.Method.SIGN_APP,
            CMD_MODE, CMD_LOCAL_SIGN,
            CMD_KEY_ALIAS, CMD_OH_APP1_KEY_V1,
            CMD_KEY_RIGHTS, CMD_RIGHTS_123456,
            CMD_APP_CERT_FILE, CMD_APP_RELEASE_CERT_PATH,
            CMD_PROFILE_FILE, CMD_SIGN_RELEASE_PROFILE_PATH,
            CMD_SIGN_ALG, CMD_SHA_256_WITH_ECDSA,
            CMD_KEY_STORE_FILE, CMD_KEY_APP_STORE_PATH,
            CMD_KEY_STORE_RIGHTS, CMD_RIGHTS_123456,
            CMD_IN_FILE, signedHap,
            CMD_OUT_FILE, signedHap
        });
        assertTrue(result);

        result = HapSignTool.processCmd(new String[] {
            CmdUtil.Method.VERIFY_APP, CMD_IN_FILE, signedHap, CMD_OUT_CERT_CHAIN, "test" + File.separator + "1.cer",
            CMD_OUT_PROFILE, "test" + File.separator + "1.p7b"
        });
        assertTrue(result);
        return signedHap;
    }

    private boolean existPagesInfoFile(String signedHap) throws IOException {
        try (ZipFile inputHap = new ZipFile(signedHap)) {
            ZipEntry entry = inputHap.getEntry(FileUtils.BIT_MAP_FILENAME);
            return entry != null;
        }
    }

    /**
     * Test Method: isRunnableFile()
     */
    @Test
    public void testIsRunnableFile() {
        List<String> correctName = new ArrayList<>();
        correctName.add("libs/中文.so");
        correctName.add("srtjdwrtj.an");
        correctName.add("srtjdwrtj.abc");
        correctName.add("libs/srtjdwrtj.so");
        correctName.add("libs/srtjdwrtj.so.1");
        correctName.add("libs/srtjdwrtj.so.1.1");
        correctName.add("libs/srtjdwrtj.so.1.1.1");
        correctName.add("libs/srtjdwrtj.aaaa.111.111.1111");
        correctName.add("libs/srtjdwrtj.so.111.111.1111");
        correctName.add("libs/中文.so.111.111.1111");
        correctName.add("libs/srtjdwrtj.so.111.%%%.1111");
        for (String name : correctName) {
            assertTrue(FileUtils.isRunnableFile(name));
        }

        List<String> incorrectName = new ArrayList<>();
        incorrectName.add("srtjdwrtj.so");
        incorrectName.add("srtjdwrtj.so.111.111.1111.54645");
        incorrectName.add("srtjdwrtjso.111.111.11111");
        incorrectName.add("srtjdwrtj.so.abc.111.111.1111");
        incorrectName.add("srtjdwrtj.so.111.111.json");
        incorrectName.add("srtjdwrtj.abc.json");
        incorrectName.add("srtjdwrtj.an.json");
        incorrectName.add("中文.so.111.111.json");
        for (String name : incorrectName) {
            assertFalse(FileUtils.isRunnableFile(name));
        }
    }

    @Test
    public void testGetHnpsFromHap() throws IOException, ProfileException {
        loadFile(CMD_TEST_HAP_FILE_PATH);
        File file = new File(CMD_TEST_HAP_FILE_PATH);
        try (JarFile inputJar = new JarFile(file, false)) {
            Map<String, String> hnpMap = HapUtils.getHnpsFromJson(inputJar);
            assertTrue(hnpMap.size() == 4);
            assertTrue("private".equals(hnpMap.get("entry-default-unsigned.hnp")));
            assertTrue("public".equals(hnpMap.get("entry-default-unsigned1.hnp")));
            assertTrue("private".equals(hnpMap.get("dd/entry-default-unsigned.hnp")));
            assertTrue("public".equals(hnpMap.get("dd/entry-default-unsigned1.hnp")));
        }
    }

    /**
     * Test Method: testByteToZip()
     *
     * @throws IOException read file exception
     */
    @Test
    public void testByteToZip() throws IOException {
        for (int i = 0; i < 10; i++) {
            File file = generateHapFile(FileType.FILE_UNCOMPRESSED, FileType.FILE_UNCOMPRESSED,
                FileType.FILE_UNCOMPRESSED, FileType.FILE_UNCOMPRESSED, ".hap");
            Zip zip = new Zip(file);
            String outFileName = "test" + File.separator + "testOut.hap";
            zip.toFile(outFileName);
            File outFile = new File(outFileName);
            byte[] bytes = FileUtils.readFile(file);
            byte[] outBytes = FileUtils.readFile(outFile);
            assertArrayEquals(outBytes, bytes);

            deleteFile(file.getCanonicalPath());
            deleteFile(outFileName);
        }
    }

    private boolean generateAppRootCa() {
        return HapSignTool.processCmd(new String[]{
            CmdUtil.Method.GENERATE_CA,
            CMD_KEY_ALIAS, CMD_OH_ROOT_CA_KEY_V1,
            CMD_KEY_RIGHTS, CMD_RIGHTS_123456,
            CMD_KEY_ALG, KeyPairTools.ECC_INPUT,
            CMD_KEY_SIZE, CMD_ECC_256,
            CMD_KEY_STORE_FILE, CMD_KEY_APP_STORE_PATH,
            CMD_KEY_STORE_RIGHTS, CMD_RIGHTS_123456,
            CMD_OUT_FILE, CMD_ROOT_APP_CA_PATH,
            CMD_SUBJECT, CMD_ROOT_CA,
            CMD_VALIDITY, CMD_VALIDITY_365,
            CMD_SIGN_ALG, CMD_SHA_256_WITH_ECDSA,
            CMD_BASIC_CONSTRAINTS_PATH_LEN, CMD_BC_PATH_LEN_0});
    }

    private boolean generateProfileRootCa() {
        return HapSignTool.processCmd(new String[]{
            CmdUtil.Method.GENERATE_CA,
            CMD_KEY_ALIAS, CMD_OH_ROOT_CA_KEY_V1,
            CMD_KEY_RIGHTS, CMD_RIGHTS_123456,
            CMD_KEY_ALG, KeyPairTools.ECC_INPUT,
            CMD_KEY_SIZE, CMD_ECC_256,
            CMD_KEY_STORE_FILE, CMD_KEY_PROFILE_STORE_PATH,
            CMD_KEY_STORE_RIGHTS, CMD_RIGHTS_123456,
            CMD_OUT_FILE, CMD_ROOT_PROFILE_CA_PATH,
            CMD_SUBJECT, CMD_ROOT_CA,
            CMD_VALIDITY, CMD_VALIDITY_365,
            CMD_SIGN_ALG, CMD_SHA_256_WITH_ECDSA,
            CMD_BASIC_CONSTRAINTS_PATH_LEN, CMD_BC_PATH_LEN_0});
    }

    private boolean generateAppSubCa() {
        return HapSignTool.processCmd(new String[]{
            CmdUtil.Method.GENERATE_CA,
            CMD_KEY_ALIAS, CMD_OH_SUB_APP_CA_KEY_V1,
            CMD_KEY_RIGHTS, CMD_RIGHTS_123456,
            CMD_ISSUER, CMD_ROOT_CA,
            CMD_KEY_ALG, KeyPairTools.ECC_INPUT,
            CMD_KEY_SIZE, CMD_ECC_256,
            CMD_KEY_STORE_FILE, CMD_KEY_APP_STORE_PATH,
            CMD_KEY_STORE_RIGHTS, CMD_RIGHTS_123456,
            CMD_OUT_FILE, CMD_SUB_APP_CA_PATH,
            CMD_ISSUER_KEY_ALIAS, CMD_OH_ROOT_CA_KEY_V1,
            CMD_ISSUER_KEY_RIGHTS, CMD_RIGHTS_123456,
            CMD_SUBJECT, CMD_APP_CA,
            CMD_VALIDITY, CMD_VALIDITY_365,
            CMD_SIGN_ALG, CMD_SHA_256_WITH_ECDSA,
            CMD_BASIC_CONSTRAINTS_PATH_LEN, CMD_BC_PATH_LEN_0});
    }

    private boolean generateProfileSubCa() {
        return HapSignTool.processCmd(new String[]{
            CmdUtil.Method.GENERATE_CA,
            CMD_KEY_ALIAS, CMD_OH_SUB_PROFILE_CA_KEY_V1,
            CMD_KEY_RIGHTS, CMD_RIGHTS_123456,
            CMD_ISSUER, CMD_ROOT_CA,
            CMD_KEY_ALG, KeyPairTools.ECC_INPUT,
            CMD_KEY_SIZE, CMD_ECC_256,
            CMD_KEY_STORE_FILE, CMD_KEY_PROFILE_STORE_PATH,
            CMD_KEY_STORE_RIGHTS, CMD_RIGHTS_123456,
            CMD_OUT_FILE, CMD_SUB_PROFILE_CA_PATH,
            CMD_ISSUER_KEY_ALIAS, CMD_OH_ROOT_CA_KEY_V1,
            CMD_ISSUER_KEY_RIGHTS, CMD_RIGHTS_123456,
            CMD_SUBJECT, CMD_PROFILE_CA,
            CMD_VALIDITY, CMD_VALIDITY_365,
            CMD_SIGN_ALG, CMD_SHA_256_WITH_ECDSA,
            CMD_BASIC_CONSTRAINTS_PATH_LEN, CMD_BC_PATH_LEN_0});
    }

    private void loadFile(String filePath) throws IOException {
        ClassLoader classLoader = CmdUnitTest.class.getClassLoader();
        URL resource = classLoader.getResource(filePath);
        assert resource != null;
        Files.copy(new File(resource.getPath()).toPath(), new File(filePath).toPath(),
            StandardCopyOption.REPLACE_EXISTING);
    }

    private void deleteFile(String filePath) throws IOException {
        if (FileUtils.isFileExist(filePath)) {
            Path path = Paths.get(filePath);
            Files.delete(path);
        }
    }

    /**
     * Enumerated value of file type in zip.
     */
    public enum FileType {
        FILE_NOT_EXISTED,
        FILE_UNCOMPRESSED,
        FILE_COMPRESSED;
    }
}
