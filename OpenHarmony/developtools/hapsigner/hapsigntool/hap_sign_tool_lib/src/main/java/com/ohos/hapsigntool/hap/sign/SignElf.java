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

package com.ohos.hapsigntool.hap.sign;

import com.ohos.hapsigntool.codesigning.exception.CodeSignException;
import com.ohos.hapsigntool.codesigning.exception.FsVerityDigestException;
import com.ohos.hapsigntool.codesigning.sign.CodeSigning;
import com.ohos.hapsigntool.hap.config.SignerConfig;
import com.ohos.hapsigntool.hap.entity.BlockHead;
import com.ohos.hapsigntool.hap.entity.SignHead;
import com.ohos.hapsigntool.hap.entity.SignBlockData;
import com.ohos.hapsigntool.hap.entity.SignatureBlockTags;
import com.ohos.hapsigntool.hap.entity.SignatureBlockTypes;
import com.ohos.hapsigntool.error.HapFormatException;
import com.ohos.hapsigntool.error.ProfileException;
import com.ohos.hapsigntool.utils.FileUtils;
import com.ohos.hapsigntool.entity.ParamConstants;
import com.ohos.hapsigntool.utils.LogUtils;
import com.ohos.hapsigntool.utils.ParamProcessUtil;
import com.ohos.hapsigntool.utils.StringUtils;

import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.Map;

/**
 * elf file Signature signer.
 *
 * @since 2023/11/21
 */
public class SignElf {
    /**
     * codesign sign block type
     */
    public static final char CODESIGN_BLOCK_TYPE = 3;

    private static final LogUtils LOGGER = new LogUtils(SignElf.class);

    private static final String CODESIGN_OFF = "0";

    private static int blockNum = 0;

    private static final int PAGE_SIZE = 4096;

    private static final int FILE_BUFFER_BLOCK = 16384;

    /**
     * Constructor of Method
     */
    private SignElf() {
    }

    /**
     * Sign the elf file.
     *
     * @param signerConfig Config of the elf file to be signed.
     * @param signParams The input parameters of sign elf.
     * @return true if sign successfully; false otherwise.
     */
    public static boolean sign(SignerConfig signerConfig, Map<String, String> signParams) {
        boolean isSuccess = false;
        /* 1. Make block head, write to output file. */
        String inputFile = signParams.get(ParamConstants.PARAM_BASIC_INPUT_FILE);
        String tmpFile = alignFileBy4kBytes(inputFile);
        if (tmpFile == null) {
            LOGGER.error("copy input File failed");
            return isSuccess;
        }
        String outputFile = signParams.get(ParamConstants.PARAM_BASIC_OUTPUT_FILE);
        String profileSigned = signParams.get(ParamConstants.PARAM_BASIC_PROFILE_SIGNED);
        if (!writeBlockDataToFile(signerConfig, tmpFile, outputFile, profileSigned, signParams)) {
            LOGGER.error("The block head data made failed.`");
            ParamProcessUtil.delDir(new File(outputFile));
            return isSuccess;
        }
        LOGGER.info("The block head data made success.");

        /* 2. Make sign data, and write to output file */
        if (!writeSignHeadDataToOutputFile(tmpFile, outputFile, blockNum)) {
            LOGGER.error("The sign head data made failed.");
            ParamProcessUtil.delDir(new File(outputFile));
        } else {
            isSuccess = true;
        }
        return isSuccess;
    }

    private static String alignFileBy4kBytes(String inputFile) {
        String tmp = "tmpFile" + new Date().getTime();
        File tmpFile = new File(tmp);
        try {
            tmpFile.createNewFile();
        } catch (IOException e) {
            LOGGER.error("create tmp file Failed");
            return null;
        }
        try (FileOutputStream output = new FileOutputStream(tmpFile);
             FileInputStream input = new FileInputStream(inputFile)) {
            byte[] buffer = new byte[FILE_BUFFER_BLOCK];
            int read;
            while ((read = input.read(buffer)) != FileUtils.FILE_END) {
                output.write(buffer, 0, read);
            }

            long addLength = PAGE_SIZE - (tmpFile.length() % PAGE_SIZE);
            if (isLongOverflowInteger(addLength)) {
                LOGGER.error("File alignment error");
                return null;
            }
            byte[] bytes = new byte[(int) addLength];
            java.util.Arrays.fill(bytes, (byte) 0);
            FileUtils.writeByteToOutFile(bytes, tmp);
        } catch (IOException e) {
            LOGGER.error("copy inFile Failed");
            return null;
        }
        return tmp;
    }

    private static boolean writeBlockDataToFile(SignerConfig signerConfig,
        String inputFile, String outputFile, String profileSigned, Map<String, String> signParams) {
        try {
            String profileFile = signParams.get(ParamConstants.PARAM_BASIC_PROFILE);

            List<SignBlockData> signDataList = new ArrayList<>();

            long binFileLen = FileUtils.getFileLen(inputFile);
            if (binFileLen == -1) {
                LOGGER.error("file length is invalid, elf file len: " + binFileLen);
                throw new IOException();
            }
            // 1. generate sign data
            if (!StringUtils.isEmpty(signParams.get(ParamConstants.PARAM_BASIC_PROFILE))) {
                signDataList.add(generateProfileSignByte(profileFile, profileSigned));
            }
            blockNum = signDataList.size() + 1; // other sign block num + codesign block 1
            SignBlockData codeSign = generateCodeSignByte(signerConfig, signParams, inputFile, blockNum, binFileLen);
            if (codeSign != null) {
                signDataList.add(0, codeSign);
            }
            blockNum = signDataList.size();
            // 2. use sign data generate offset and sign block head
            generateSignBlockHead(signDataList);

            return writeSignedElf(inputFile, signDataList, outputFile);
        } catch (IOException e) {
            LOGGER.error("writeBlockDataToFile failed.", e);
            return false;
        } catch (FsVerityDigestException | CodeSignException | HapFormatException | ProfileException e) {
            LOGGER.error("codesign failed.", e);
            return false;
        }
    }

    private static boolean writeSignedElf(String inputFile, List<SignBlockData> signBlockList, String outputFile) {
        try (FileOutputStream fileOutputStream = new FileOutputStream(outputFile);
             DataOutputStream dataOutputStream = new DataOutputStream(fileOutputStream)) {
            // 1. write the input file to the output file.
            if (!FileUtils.writeFileToDos(inputFile, dataOutputStream)) {
                LOGGER.error("Failed to write information of input file: " + inputFile
                        + " to outputFile: " + outputFile);
                throw new IOException();
            }

            // 2. write block head to the output file.
            for (SignBlockData signBlockData : signBlockList) {
                if (!FileUtils.writeByteToDos(signBlockData.getBlockHead(), dataOutputStream)) {
                    LOGGER.error("Failed to write Block Head to output file: " + outputFile);
                    throw new IOException();
                }
            }

            // 3. write block data to the output file.
            for (SignBlockData signBlockData : signBlockList) {
                boolean isSuccess;
                if (signBlockData.isByte()) {
                    isSuccess = FileUtils.writeByteToDos(signBlockData.getSignData(), dataOutputStream);
                } else {
                    isSuccess = FileUtils.writeFileToDos(signBlockData.getSignFile(), dataOutputStream);
                }

                if (!isSuccess) {
                    LOGGER.error("Failed to write Block Data to output file: " + outputFile);
                    throw new IOException();
                }
            }
        } catch (IOException e) {
            LOGGER.error("writeSignedElf failed.", e);
            return false;
        }
        return true;
    }

    private static void generateSignBlockHead(List<SignBlockData> signDataList)
            throws IOException {
        long offset = (long) BlockHead.getElfBlockLen() * signDataList.size();

        for (int i = 0; i < signDataList.size(); i++) {
            SignBlockData signBlockData = signDataList.get(i);

            signBlockData.setBlockHead(BlockHead.getBlockHeadLittleEndian(signBlockData.getType(),
                    SignatureBlockTags.DEFAULT, (int) signBlockData.getLen(), (int) offset));
            offset += signBlockData.getLen();
            if (isLongOverflowInteger(offset)) {
                LOGGER.error("The sign block " + i + "offset is overflow integer, offset: " + offset);
                throw new IOException();
            }
        }
    }

    private static SignBlockData generateProfileSignByte(String profileFile, String profileSigned) throws IOException {
        long profileDataLen = FileUtils.getFileLen(profileFile);

        if (profileDataLen == -1 || isLongOverflowShort(profileDataLen)) {
            LOGGER.error("file length is invalid, profileDataLen: " + profileDataLen);
            throw new IOException();
        }

        char isSigned = SignatureBlockTypes.getProfileBlockTypes(profileSigned);
        return new SignBlockData(profileFile, isSigned);
    }

    private static SignBlockData generateCodeSignByte(SignerConfig signerConfig, Map<String, String> signParams,
        String inputFile, int blockNum, long binFileLen) throws IOException,
            FsVerityDigestException, CodeSignException, HapFormatException, ProfileException {
        if (CODESIGN_OFF.equals(signParams.get(ParamConstants.PARAM_SIGN_CODE))) {
            return null;
        }
        CodeSigning codeSigning = new CodeSigning(signerConfig);
        long offset = binFileLen + (long) BlockHead.getElfBlockLen() * blockNum;
        String profileContent = signParams.get(ParamConstants.PARAM_PROFILE_JSON_CONTENT);
        byte[] codesignData = codeSigning.getElfCodeSignBlock(new File(inputFile), offset,
                signParams.get(ParamConstants.PARAM_IN_FORM), profileContent);
        return new SignBlockData(codesignData, CODESIGN_BLOCK_TYPE);
    }

    private static boolean writeSignHeadDataToOutputFile(String inputFile, String outputFile, int blockNum) {
        long size = FileUtils.getFileLen(outputFile) - FileUtils.getFileLen(inputFile);
        if (isLongOverflowInteger(size)) {
            LOGGER.error("File size is overflow integer range.");
            return false;
        }
        SignHead signHeadData = new SignHead();
        byte[] signHeadByte = signHeadData.getSignHeadLittleEndian((int) size, blockNum);
        if (signHeadByte == null) {
            LOGGER.error("Failed to get sign head data.");
            return false;
        }
        return FileUtils.writeByteToOutFile(signHeadByte, outputFile);
    }

    private static boolean isLongOverflowInteger(long num) {
        return (num - (num & 0xffffffffL)) != 0;
    }

    private static boolean isLongOverflowShort(long num) {
        return (num - (num & 0xffffL)) != 0;
    }
}
