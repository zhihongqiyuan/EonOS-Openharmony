/*
 * Copyright (c) 2021-2023 Huawei Device Co., Ltd.
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

package com.ohos.hapsigntool.hap.verify;

import com.ohos.hapsigntool.entity.Options;
import com.ohos.hapsigntool.codesigning.exception.FsVerityDigestException;
import com.ohos.hapsigntool.codesigning.exception.VerifyCodeSignException;
import com.ohos.hapsigntool.codesigning.sign.VerifyCodeSignature;
import com.ohos.hapsigntool.error.SignToolErrMsg;
import com.ohos.hapsigntool.hap.entity.ElfBlockData;
import com.ohos.hapsigntool.hap.entity.BlockHead;
import com.ohos.hapsigntool.hap.entity.SignHead;
import com.ohos.hapsigntool.hap.entity.SignatureBlockTypes;
import com.ohos.hapsigntool.hap.entity.SigningBlock;
import com.ohos.hapsigntool.error.ProfileException;
import com.ohos.hapsigntool.hap.sign.SignElf;
import com.ohos.hapsigntool.utils.FileUtils;
import com.ohos.hapsigntool.entity.ParamConstants;
import com.ohos.hapsigntool.utils.LogUtils;
import com.ohos.hapsigntool.utils.StringUtils;

import org.bouncycastle.cms.CMSException;
import org.bouncycastle.cms.CMSSignedData;
import org.bouncycastle.jce.provider.BouncyCastleProvider;
import org.bouncycastle.openssl.jcajce.JcaPEMWriter;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.OutputStream;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.security.Security;
import java.security.cert.X509Certificate;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Class of verify ELF.
 *
 * @since 2023/11/23
 */
public class VerifyElf {
    private static final LogUtils LOGGER = new LogUtils(VerifyElf.class);

    static {
        Security.addProvider(new BouncyCastleProvider());
    }

    private static String getProfileContent(byte[] profile) throws ProfileException {
        try {
            CMSSignedData cmsSignedData = new CMSSignedData(profile);
            if (!VerifyUtils.verifyCmsSignedData(cmsSignedData)) {
                throw new ProfileException(SignToolErrMsg.VERIFY_PROFILE_INVALID.toString());
            }
            Object contentObj = cmsSignedData.getSignedContent().getContent();
            if (!(contentObj instanceof byte[])) {
                throw new ProfileException(SignToolErrMsg.VERIFY_PROFILE_FAILED
                        .toString("Check profile failed, signed profile content is not byte array!"));
            }
            return new String((byte[]) contentObj, StandardCharsets.UTF_8);
        } catch (CMSException e) {
            return new String(profile, StandardCharsets.UTF_8);
        }
    }


    /**
     * Check whether parameters are valid
     *
     * @param options input parameters used to verify ELF.
     * @return true, if all parameters are valid.
     */
    public boolean checkParams(Options options) {
        if (!options.containsKey(ParamConstants.PARAM_VERIFY_CERTCHAIN_FILE)) {
            LOGGER.error("Missing parameter: {}", ParamConstants.PARAM_VERIFY_CERTCHAIN_FILE);
            return false;
        }
        if (!options.containsKey(ParamConstants.PARAM_VERIFY_PROFILE_FILE)) {
            LOGGER.error("Missing parameter: {}", ParamConstants.PARAM_VERIFY_PROFILE_FILE);
            return false;
        }
        if (!options.containsKey(ParamConstants.PARAM_VERIFY_PROOF_FILE)) {
            LOGGER.warn("Missing parameter: {}", ParamConstants.PARAM_VERIFY_PROOF_FILE);
        }
        return true;
    }

    /**
     * verify elf file.
     *
     * @param options input parameters used to verify elf.
     * @return true, if verify successfully.
     */
    public boolean verify(Options options) {
        VerifyResult verifyResult;
        try {
            if (!checkParams(options)) {
                LOGGER.error("Check params failed!");
                throw new IOException();
            }
            String filePath = options.getString(ParamConstants.PARAM_BASIC_INPUT_FILE);
            if (StringUtils.isEmpty(filePath)) {
                LOGGER.error("Not found verify file path!");
                throw new IOException();
            }
            File signedFile = new File(filePath);
            if (!checkSignFile(signedFile)) {
                LOGGER.error("Check input signature ELF false!");
                throw new IOException();
            }
            verifyResult = verifyElf(filePath);
            if (!verifyResult.isVerified()) {
                LOGGER.error("verify: {}", verifyResult.getMessage());
                throw new IOException();
            }
            String outputCertPath = options.getString(ParamConstants.PARAM_VERIFY_CERTCHAIN_FILE);
            if (verifyResult.getCertificates() != null) {
                writeCertificate(outputCertPath, verifyResult.getCertificates());
            }
        } catch (IOException e) {
            LOGGER.error("Write certificate chain error", e);
            return false;
        }

        String outputProfileFile = options.getString(ParamConstants.PARAM_VERIFY_PROFILE_FILE);
        try {
            outputOptionalBlocks(outputProfileFile, verifyResult);
        } catch (IOException e) {
            LOGGER.error("Output optional blocks error", e);
            return false;
        }

        LOGGER.info("verify: {}", verifyResult.getMessage());
        return true;
    }

    private void writeCertificate(String destFile, List<X509Certificate> certificates) throws IOException {
        try (JcaPEMWriter writer = new JcaPEMWriter(new FileWriter(destFile))) {
            for (final X509Certificate cert : certificates) {
                writer.write(cert.getSubjectDN().toString() + System.lineSeparator());
                writer.writeObject(cert);
            }
            LOGGER.info("Write certificate chain success!");
        }
    }

    private void outputOptionalBlocks(String outputProfileFile, VerifyResult verifyResult) throws IOException {
        byte[] profile = verifyResult.getProfile();
        if (profile != null) {
            writeOptionalBytesToFile(profile, outputProfileFile);
        }
    }

    private void writeOptionalBytesToFile(byte[] data, String outputFile) throws IOException {
        if (outputFile == null || outputFile.isEmpty()) {
            return;
        }
        try (OutputStream out = Files.newOutputStream(Paths.get(outputFile))) {
            out.write(data);
            out.flush();
        }
    }

    private boolean checkSignFile(File signedFile) {
        try {
            FileUtils.isValidFile(signedFile);
        } catch (IOException e) {
            LOGGER.error("signedFile is invalid.", e);
            return false;
        }
        return true;
    }

    /**
     * Verify elf file.
     *
     * @param binFile path of elf file.
     * @return true, if verify successfully.
     */
    public VerifyResult verifyElf(String binFile) {
        VerifyResult result = new VerifyResult(true, VerifyResult.RET_SUCCESS, "verify signature success");
        File bin = new File(binFile);
        try {
            byte[] bytes = FileUtils.readFile(bin);
            ElfBlockData elfSignBlockData = getElfSignBlockData(bytes);
            String profileJson;
            byte[] profileByte;
            Map<Character, SigningBlock> signBlock = getSignBlock(bytes, elfSignBlockData);
            if (signBlock.containsKey(SignatureBlockTypes.PROFILE_NOSIGNED_BLOCK)) {
                profileByte = signBlock.get(SignatureBlockTypes.PROFILE_NOSIGNED_BLOCK).getValue();
                profileJson = new String(profileByte, StandardCharsets.UTF_8);
                result.setProfile(profileByte);
                LOGGER.warn("profile is not signed");
            } else if (signBlock.containsKey(SignatureBlockTypes.PROFILE_SIGNED_BLOCK)) {
                // verify signed profile
                SigningBlock profileSign = signBlock.get(SignatureBlockTypes.PROFILE_SIGNED_BLOCK);
                profileByte = profileSign.getValue();
                profileJson = getProfileContent(profileByte);
                result = new HapVerify().verifyElfProfile(profileSign.getValue());
                result.setProfile(profileByte);
                LOGGER.info("verify profile success");
            } else {
                LOGGER.warn("can not found profile sign block");
                profileJson = null;
            }

            if (signBlock.containsKey(SignElf.CODESIGN_BLOCK_TYPE)) {
                // verify codesign
                SigningBlock codesign = signBlock.get(SignElf.CODESIGN_BLOCK_TYPE);
                if (!VerifyCodeSignature.verifyElf(bin, codesign.getOffset(), codesign.getLength(),
                        "elf", profileJson)) {
                    String errMsg = "Verify codesign error!";
                    result = new VerifyResult(false, VerifyResult.RET_IO_ERROR, errMsg);
                }
                LOGGER.info("verify codesign success");
            } else {
                LOGGER.warn("can not found code sign block");
            }
        } catch (IOException e) {
            LOGGER.error("Verify file has IO error!", e);
            result = new VerifyResult(false, VerifyResult.RET_IO_ERROR, e.getMessage());
        } catch (FsVerityDigestException | VerifyCodeSignException e) {
            LOGGER.error("Verify codesign error!", e);
            result = new VerifyResult(false, VerifyResult.RET_IO_ERROR, e.getMessage());
        } catch (CMSException | ProfileException e) {
            LOGGER.error("Verify profile error!", e);
            result = new VerifyResult(false, VerifyResult.RET_IO_ERROR, e.getMessage());
        }
        return result;
    }

    private ElfBlockData getElfSignBlockData(byte[] bytes) throws IOException {
        int offset = bytes.length - SignHead.SIGN_HEAD_LEN;
        byte[] magicByte = readByteArrayOffset(bytes, offset, SignHead.ELF_MAGIC.length);
        offset += SignHead.ELF_MAGIC.length;
        byte[] versionByte = readByteArrayOffset(bytes, offset, SignHead.VERSION.length);
        offset += SignHead.VERSION.length;
        for (int i = 0; i < SignHead.ELF_MAGIC.length; i++) {
            if (SignHead.ELF_MAGIC[i] != magicByte[i]) {
                throw new IOException("elf magic verify failed");
            }
        }
        for (int i = 0; i < SignHead.VERSION.length; i++) {
            if (SignHead.VERSION[i] != versionByte[i]) {
                throw new IOException("elf sign version verify failed");
            }
        }
        int intByteLength = 4;
        byte[] blockSizeByte = readByteArrayOffset(bytes, offset, intByteLength);
        offset += intByteLength;
        byte[] blockNumByte = readByteArrayOffset(bytes, offset, intByteLength);
        ByteBuffer blockNumBf = ByteBuffer.wrap(blockNumByte).order(ByteOrder.LITTLE_ENDIAN);
        int blockNum = blockNumBf.getInt();

        ByteBuffer blockSizeBf = ByteBuffer.wrap(blockSizeByte).order(ByteOrder.LITTLE_ENDIAN);
        int blockSize = blockSizeBf.getInt();

        int blockStart = bytes.length - SignHead.SIGN_HEAD_LEN - blockSize;
        return new ElfBlockData(blockNum, blockStart);
    }

    private Map<Character, SigningBlock> getSignBlock(byte[] bytes, ElfBlockData elfBlockData) throws ProfileException {
        int offset = elfBlockData.getBlockStart();

        Map<Character, SigningBlock> blockMap = new HashMap<>();
        for (int i = 0; i < elfBlockData.getBlockNum(); i++) {
            byte[] blockByte = readByteArrayOffset(bytes, offset, BlockHead.ELF_BLOCK_LEN);
            ByteBuffer blockBuffer = ByteBuffer.wrap(blockByte).order(ByteOrder.LITTLE_ENDIAN);
            char type = blockBuffer.getChar();
            char tag = blockBuffer.getChar();
            int length = blockBuffer.getInt();
            int blockOffset = blockBuffer.getInt();
            byte[] value = readByteArrayOffset(bytes, elfBlockData.getBlockStart() + blockOffset, length);
            blockMap.put(type, new SigningBlock(type, value, elfBlockData.getBlockStart() + blockOffset));
            offset += BlockHead.ELF_BLOCK_LEN;
        }
        return blockMap;
    }

    private byte[] readByteArrayOffset(byte[] bytes, int offset, int length) {
        byte[] output = new byte[length];
        System.arraycopy(bytes, offset, output, 0, length);
        return output;
    }
}