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

package com.ohos.hapsigntool.hap.sign;

import com.ohos.hapsigntool.hap.config.SignerConfig;
import com.ohos.hapsigntool.hap.entity.BlockHead;
import com.ohos.hapsigntool.hap.entity.SignHead;
import com.ohos.hapsigntool.hap.entity.SignContentInfo;
import com.ohos.hapsigntool.hap.entity.SignatureBlockTags;
import com.ohos.hapsigntool.hap.entity.SignatureBlockTypes;
import com.ohos.hapsigntool.error.SignatureException;
import com.ohos.hapsigntool.utils.FileUtils;
import com.ohos.hapsigntool.utils.HashUtils;
import com.ohos.hapsigntool.entity.ParamConstants;
import com.ohos.hapsigntool.utils.LogUtils;
import com.ohos.hapsigntool.utils.ParamProcessUtil;

import java.io.DataOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Map;

/**
 * LiteOS bin file Signature signer.
 *
 * @since 2021/12/21
 */
public class SignBin {
    private static final LogUtils LOGGER = new LogUtils(SignBin.class);

    /**
     * Constructor of Method
     */
    private SignBin() {
    }

    /**
     * Sign the bin file.
     *
     * @param signerConfig Config of the bin file to be signed.
     * @param signParams The input parameters of sign bin.
     * @return true if sign successfully; false otherwise.
     */
    public static boolean sign(SignerConfig signerConfig, Map<String, String> signParams) {
        /* 1. Make block head, write to output file. */
        String codesign = signParams.get(ParamConstants.PARAM_SIGN_CODE);
        if (ParamConstants.ProfileSignFlag.ENABLE_SIGN_CODE.getSignFlag().equals(codesign)) {
            LOGGER.warn("can not sign bin with codesign");
        }
        String inputFile = signParams.get(ParamConstants.PARAM_BASIC_INPUT_FILE);
        String outputFile = signParams.get(ParamConstants.PARAM_BASIC_OUTPUT_FILE);
        String profileFile = signParams.get(ParamConstants.PARAM_BASIC_PROFILE);
        String profileSigned = signParams.get(ParamConstants.PARAM_BASIC_PROFILE_SIGNED);
        if (!writeBlockDataToFile(inputFile, outputFile, profileFile, profileSigned)) {
            LOGGER.error("The block head data made failed.");
            ParamProcessUtil.delDir(new File(outputFile));
            return false;
        }
        LOGGER.info("The block head data made success.");

        /* 2. Make sign data, and write to output file */
        String signAlg = signParams.get(ParamConstants.PARAM_BASIC_SIGANTURE_ALG);
        if (!writeSignDataToOutputFile(signerConfig, outputFile, signAlg)) {
            LOGGER.error("The sign data made failed.");
            ParamProcessUtil.delDir(new File(outputFile));
            return false;
        }
        LOGGER.info("The data signed success.");
        /* 3. Make sign data, and write to output file */
        if (writeSignHeadDataToOutputFile(inputFile, outputFile)) {
            return true;
        }
        LOGGER.error("The sign head data made failed.");
        ParamProcessUtil.delDir(new File(outputFile));
        return false;
    }

    private static boolean writeBlockDataToFile(
            String inputFile, String outputFile, String profileFile, String profileSigned) {
        try {
            long binFileLen = FileUtils.getFileLen(inputFile);
            long profileDataLen = FileUtils.getFileLen(profileFile);
            if (!checkBinAndProfileLengthIsValid(binFileLen, profileDataLen)) {
                LOGGER.error("file length is invalid, binFileLen: " + binFileLen
                    + " profileDataLen: " + profileDataLen);
                throw new IOException();
            }

            long offset = binFileLen + BlockHead.getBlockLen() + BlockHead.getBlockLen();
            if (isLongOverflowInteger(offset)) {
                LOGGER.error("The profile block head offset is overflow integer range, offset: " + offset);
                throw new IOException();
            }
            char isSigned = SignatureBlockTypes.getProfileBlockTypes(profileSigned);
            byte[] proBlockByte =
                BlockHead.getBlockHead(isSigned, SignatureBlockTags.DEFAULT, (short) profileDataLen, (int) offset);

            offset += profileDataLen;
            if (isLongOverflowInteger(offset)) {
                LOGGER.error("The sign block head offset is overflow integer range, offset: " + offset);
                throw new IOException();
            }
            byte[] signBlockByte = BlockHead.getBlockHead(
                    SignatureBlockTypes.SIGNATURE_BLOCK, SignatureBlockTags.DEFAULT, (short) 0, (int) offset);

            return writeSignedBin(inputFile, proBlockByte, signBlockByte, profileFile, outputFile);
        } catch (IOException e) {
            LOGGER.error("writeBlockDataToFile failed.", e);
            return false;
        }
    }

    private static boolean writeSignedBin(String inputFile, byte[] proBlockByte, byte[] signBlockByte,
        String profileFile, String outputFile) {
        try (FileOutputStream fileOutputStream = new FileOutputStream(outputFile);
             DataOutputStream dataOutputStream = new DataOutputStream(fileOutputStream);) {
            // 1. write the input file to the output file.
            if (!FileUtils.writeFileToDos(inputFile, dataOutputStream)) {
                LOGGER.error("Failed to write information of input file: " + inputFile +
                        " to outputFile: " + outputFile);
                throw new IOException();
            }

            // 2. write profile block head to the output file.
            if (!FileUtils.writeByteToDos(proBlockByte, dataOutputStream)) {
                LOGGER.error("Failed to write proBlockByte to output file: " + outputFile);
                throw new IOException();
            }

            // 3. write sign block head to the output file.
            if (!FileUtils.writeByteToDos(signBlockByte, dataOutputStream)) {
                LOGGER.error("Failed to write binBlockByte to output file: " + outputFile);
                throw new IOException();
            }

            // 4. write profile src file to the output file.
            if (!FileUtils.writeFileToDos(profileFile, dataOutputStream)) {
                LOGGER.error("Failed to write profile file: " + profileFile);
                throw new IOException();
            }
        } catch (IOException e) {
            LOGGER.error("writeSignedBin failed.", e);
            return false;
        }
        return true;
    }

    private static boolean checkBinAndProfileLengthIsValid(long binFileLen, long profileDataLen) {
        return (binFileLen != -1) && (profileDataLen != -1) && (!isLongOverflowShort(profileDataLen));
    }

    private static boolean writeSignHeadDataToOutputFile(String inputFile, String outputFile) {
        long size = FileUtils.getFileLen(outputFile) - FileUtils.getFileLen(inputFile) + SignHead.SIGN_HEAD_LEN;
        if (isLongOverflowInteger(size)) {
            LOGGER.error("File size is overflow integer range.");
            return false;
        }
        SignHead signHeadData = new SignHead();
        byte[] signHeadByte = signHeadData.getSignHead((int) size);
        if (signHeadByte == null || signHeadByte.length == 0) {
            LOGGER.error("Failed to get sign head data.");
            return false;
        }
        return FileUtils.writeByteToOutFile(signHeadByte, outputFile);
    }

    private static boolean writeSignDataToOutputFile(SignerConfig signerConfig, String outputFile, String signAlg) {
        String alg = ParamProcessUtil.getSignatureAlgorithm(signAlg).getContentDigestAlgorithm().getDigestAlgorithm();
        byte[] data = HashUtils.getFileDigest(outputFile, alg);
        if (data == null) {
            LOGGER.error("getFileDigest failed.");
            return false;
        }
        byte[] outputChunk = null;
        SignContentInfo contentInfo = null;
        try {
            contentInfo = new SignContentInfo();
            contentInfo.addContentHashData(
                    (char) 0, SignatureBlockTags.HASH_ROOT_4K, (short) HashUtils.getHashAlgsId(alg), data.length, data);
            byte[] dig = contentInfo.getByteContent();
            outputChunk = Pkcs7Generator.BC.generateSignedData(dig, signerConfig);
            return FileUtils.writeByteToOutFile(outputChunk, outputFile);
        } catch (SignatureException e) {
            LOGGER.error("Sign hap Lite failed.", e);
        }
        return false;
    }

    private static boolean isLongOverflowInteger(long num) {
        return (num - (num & 0xffffffffL)) != 0;
    }

    private static boolean isLongOverflowShort(long num) {
        return (num - (num & 0xffffL)) != 0;
    }
}
