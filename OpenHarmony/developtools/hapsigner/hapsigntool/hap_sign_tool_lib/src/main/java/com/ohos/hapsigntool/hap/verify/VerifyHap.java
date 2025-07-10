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
import com.ohos.hapsigntool.entity.Pair;
import com.ohos.hapsigntool.error.SignToolErrMsg;
import com.ohos.hapsigntool.hap.entity.SigningBlock;
import com.ohos.hapsigntool.error.HapFormatException;
import com.ohos.hapsigntool.error.ProfileException;
import com.ohos.hapsigntool.error.SignatureNotFoundException;
import com.ohos.hapsigntool.utils.FileUtils;
import com.ohos.hapsigntool.hap.utils.HapUtils;
import com.ohos.hapsigntool.entity.ParamConstants;
import com.ohos.hapsigntool.utils.LogUtils;
import com.ohos.hapsigntool.utils.StringUtils;
import com.ohos.hapsigntool.zip.ByteBufferZipDataInput;
import com.ohos.hapsigntool.zip.RandomAccessFileZipDataInput;
import com.ohos.hapsigntool.zip.UnsignedDecimalUtil;
import com.ohos.hapsigntool.zip.ZipDataInput;
import com.ohos.hapsigntool.zip.ZipFileInfo;
import com.ohos.hapsigntool.zip.ZipUtils;

import org.bouncycastle.cms.CMSException;
import org.bouncycastle.cms.CMSSignedData;
import org.bouncycastle.jce.provider.BouncyCastleProvider;
import org.bouncycastle.openssl.jcajce.JcaPEMWriter;
import org.bouncycastle.util.Arrays;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.OutputStream;
import java.io.RandomAccessFile;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.security.Security;
import java.security.cert.X509Certificate;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

/**
 * Class of verify hap.
 *
 * @since 2021/12/23
 */
public class VerifyHap {
    private static final LogUtils LOGGER = new LogUtils(VerifyHap.class);
    private static final int ZIP_HEAD_OF_SIGNING_BLOCK_LENGTH = 32;
    private static final int ZIP_HEAD_OF_SIGNING_BLOCK_COUNT_OFFSET_REVERSE = 28;
    private static final int ZIP_HEAD_OF_SUBSIGNING_BLOCK_LENGTH = 12;

    static {
        Security.addProvider(new BouncyCastleProvider());
    }

    private final boolean isPrintCert;

    public VerifyHap() {
        this(true);
    }

    public VerifyHap(boolean isPrintCert) {
        this.isPrintCert = isPrintCert;
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
     * @param options input parameters used to verify hap.
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
     * verify hap file.
     *
     * @param options input parameters used to verify hap.
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
                LOGGER.error("Check input signature hap false!");
                throw new IOException();
            }
            verifyResult = verifyHap(filePath);
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
        String outputProofFile = options.getString(ParamConstants.PARAM_VERIFY_PROOF_FILE);
        String outputPropertyFile = options.getString(ParamConstants.PARAM_VERIFY_PROPERTY_FILE);
        try {
            outputOptionalBlocks(outputProfileFile, outputProofFile, outputPropertyFile, verifyResult);
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

    private void outputOptionalBlocks(String outputProfileFile, String outputProofFile, String outputPropertyFile,
                                      VerifyResult verifyResult) throws IOException {
        List<SigningBlock> optionalBlocks = verifyResult.getOptionalBlocks();
        if (optionalBlocks != null && optionalBlocks.size() > 0) {
            for (SigningBlock optionalBlock : optionalBlocks) {
                int type = optionalBlock.getType();
                switch (type) {
                    case HapUtils.HAP_PROFILE_BLOCK_ID:
                        writeOptionalBytesToFile(optionalBlock.getValue(), outputProfileFile);
                        break;
                    case HapUtils.HAP_PROOF_OF_ROTATION_BLOCK_ID:
                        writeOptionalBytesToFile(optionalBlock.getValue(), outputProofFile);
                        break;
                    case HapUtils.HAP_PROPERTY_BLOCK_ID:
                        writeOptionalBytesToFile(optionalBlock.getValue(), outputPropertyFile);
                        break;
                    default:
                        throw new IOException("Unsupported Block Id: 0x" + Long.toHexString(type));
                }
            }
        }
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
     * Verify hap file.
     *
     * @param hapFilePath path of hap file.
     * @return true, if verify successfully.
     */
    public VerifyResult verifyHap(String hapFilePath) {
        VerifyResult result;
        try (RandomAccessFile fle = new RandomAccessFile(hapFilePath, "r")) {
            ZipDataInput hapFile = new RandomAccessFileZipDataInput(fle);
            ZipFileInfo zipInfo = ZipUtils.findZipInfo(hapFile);
            long eocdOffset = zipInfo.getEocdOffset();
            if (ZipUtils.checkZip64EoCDLocatorIsPresent(hapFile, eocdOffset)) {
                String errorMsg = "ZIP64 format not supported!";
                LOGGER.error(errorMsg);
                return new VerifyResult(false, VerifyResult.RET_UNSUPPORTED_FORMAT_ERROR, errorMsg);
            }
            HapUtils.HapSignBlockInfo hapSigningBlockAndOffsetInFile = HapUtils.findHapSigningBlock(hapFile, zipInfo);
            ByteBuffer signingBlock = hapSigningBlockAndOffsetInFile.getContent();
            signingBlock.order(ByteOrder.LITTLE_ENDIAN);
            Pair<ByteBuffer, List<SigningBlock>> blockPair = getHapSignatureSchemeBlockAndOptionalBlocks(signingBlock);
            ByteBuffer signatureSchemeBlock = blockPair.getFirst();
            List<SigningBlock> optionalBlocks = blockPair.getSecond();
            Collections.reverse(optionalBlocks);
            if (!checkCodeSign(hapFilePath, optionalBlocks)) {
                String errMsg = "code sign verify failed";
                return new VerifyResult(false, VerifyResult.RET_CODESIGN_DATA_ERROR, errMsg);
            }
            HapVerify verifyEngine = getHapVerify(hapFile, zipInfo, hapSigningBlockAndOffsetInFile,
                    signatureSchemeBlock, optionalBlocks);
            result = verifyEngine.verify();
            result.setSignBlockVersion(hapSigningBlockAndOffsetInFile.getVersion());
        } catch (IOException e) {
            LOGGER.error("Verify Hap has IO error!", e);
            result = new VerifyResult(false, VerifyResult.RET_IO_ERROR, e.getMessage());
        } catch (SignatureNotFoundException e) {
            LOGGER.error("Verify Hap failed, signature not found.", e);
            result = new VerifyResult(false, VerifyResult.RET_SIGNATURE_NOT_FOUND_ERROR, e.getMessage());
        } catch (HapFormatException e) {
            LOGGER.error("Verify Hap failed, unsupported format hap.", e);
            result = new VerifyResult(false, VerifyResult.RET_UNSUPPORTED_FORMAT_ERROR, e.getMessage());
        } catch (FsVerityDigestException e) {
            LOGGER.error("Verify Hap failed, fs-verity digest generate failed.", e);
            result = new VerifyResult(false, VerifyResult.RET_DIGEST_ERROR, e.getMessage());
        } catch (VerifyCodeSignException e) {
            LOGGER.error("Verify Hap failed, code sign block verify failed.", e);
            result = new VerifyResult(false, VerifyResult.RET_CODE_SIGN_BLOCK_ERROR, e.getMessage());
        } catch (CMSException e) {
            LOGGER.error("Verify Hap failed, code signature verify failed.", e);
            result = new VerifyResult(false, VerifyResult.RET_SIGNATURE_ERROR, e.getMessage());
        } catch (ProfileException e) {
            LOGGER.error("Verify Hap failed, parse app-identifier from profile failed, profile is invalid", e);
            return new VerifyResult(false, VerifyResult.RET_CODE_SIGN_BLOCK_ERROR, e.getMessage());
        }
        return result;
    }

    private HapVerify getHapVerify(ZipDataInput hapFile, ZipFileInfo zipInfo,
                                   HapUtils.HapSignBlockInfo hapSigningBlockAndOffsetInFile,
                                   ByteBuffer signatureSchemeBlock, List<SigningBlock> optionalBlocks) {
        long signingBlockOffset = hapSigningBlockAndOffsetInFile.getOffset();
        ZipDataInput beforeHapSigningBlock = hapFile.slice(0, signingBlockOffset);
        ZipDataInput centralDirectoryBlock = hapFile.slice(zipInfo.getCentralDirectoryOffset(),
                zipInfo.getCentralDirectorySize());
        ByteBuffer eocdBbyteBuffer = zipInfo.getEocd();
        ZipUtils.setCentralDirectoryOffset(eocdBbyteBuffer, signingBlockOffset);
        ZipDataInput eocdBlock = new ByteBufferZipDataInput(eocdBbyteBuffer);
        HapVerify verifyEngine = new HapVerify(beforeHapSigningBlock, signatureSchemeBlock,
                centralDirectoryBlock, eocdBlock, optionalBlocks);
        verifyEngine.setIsPrintCert(isPrintCert);
        return verifyEngine;
    }

    /**
     * code sign check
     *
     * @param hapFilePath hap file path
     * @param optionalBlocks optional blocks
     * @return true or false
     * @throws FsVerityDigestException FsVerity digest on error
     * @throws IOException IO error
     * @throws VerifyCodeSignException verify code sign on error
     * @throws CMSException cms on error
     * @throws ProfileException profile of the hap error
     */
    private boolean checkCodeSign(String hapFilePath, List<SigningBlock> optionalBlocks)
            throws FsVerityDigestException, IOException, VerifyCodeSignException, CMSException, ProfileException {
        Map<Integer, byte[]> map = optionalBlocks.stream()
                .collect(Collectors.toMap(SigningBlock::getType, SigningBlock::getValue));
        byte[] propertyBlockArray = map.get(HapUtils.HAP_PROPERTY_BLOCK_ID);
        if (propertyBlockArray != null && propertyBlockArray.length > 0) {
            LOGGER.info("trying verify codesign block");
            String[] fileNameArray = hapFilePath.split("\\.");
            if (fileNameArray.length < ParamConstants.FILE_NAME_MIN_LENGTH) {
                LOGGER.error("ZIP64 format not supported");
                return false;
            }
            ByteBuffer byteBuffer = ByteBuffer.wrap(propertyBlockArray);
            ByteBuffer header = HapUtils.reverseSliceBuffer(byteBuffer, 0, ZIP_HEAD_OF_SUBSIGNING_BLOCK_LENGTH);
            long blockOffset = UnsignedDecimalUtil.getUnsignedInt(header);
            int blockLength = header.getInt();
            int blockType = header.getInt();
            if (blockType != HapUtils.HAP_CODE_SIGN_BLOCK_ID) {
                LOGGER.error("Verify Hap has no code sign data error!");
                return false;
            }
            File outputFile = new File(hapFilePath);
            byte[] profileArray = map.get(HapUtils.HAP_PROFILE_BLOCK_ID);
            String profileContent = getProfileContent(profileArray);
            String suffix = fileNameArray[fileNameArray.length - 1];
            boolean isCodeSign = VerifyCodeSignature.verifyHap(outputFile, blockOffset, blockLength,
                    suffix, profileContent);
            if (!isCodeSign) {
                LOGGER.error("Verify Hap has no code sign data error!");
                return false;
            }
            LOGGER.info("verify codesign success");
            return true;
        }
        LOGGER.info("can not find codesign block");
        return true;
    }

    private Pair<ByteBuffer, List<SigningBlock>> getHapSignatureSchemeBlockAndOptionalBlocks(ByteBuffer hapSigningBlock)
            throws SignatureNotFoundException {
        try {
            ByteBuffer header = HapUtils.reverseSliceBuffer(
                    hapSigningBlock,
                    hapSigningBlock.capacity() - ZIP_HEAD_OF_SIGNING_BLOCK_LENGTH,
                    hapSigningBlock.capacity());
            ByteBuffer value = HapUtils.reverseSliceBuffer(hapSigningBlock, 0,
                    hapSigningBlock.capacity() - ZIP_HEAD_OF_SIGNING_BLOCK_LENGTH);

            byte[] signatureValueBytes = new byte[value.capacity()];
            value.get(signatureValueBytes, 0, signatureValueBytes.length);
            signatureValueBytes = Arrays.reverse(signatureValueBytes);
            header.position(ZIP_HEAD_OF_SIGNING_BLOCK_COUNT_OFFSET_REVERSE); // position to the block count offset
            int blockCount = header.getInt();
            int current = value.capacity() - ZIP_HEAD_OF_SUBSIGNING_BLOCK_LENGTH * blockCount;
            value.position(current);

            int blockType = -1;
            int blockLength = -1;
            int blockOffset = -1;
            ByteBuffer hapSigningPkcs7Block = null;
            List<SigningBlock> optionalBlocks = new ArrayList<SigningBlock>();
            for (int i = 0; i < blockCount; i++) {
                blockOffset = value.getInt();
                blockLength = value.getInt();
                blockType = value.getInt();
                if (blockOffset + blockLength > signatureValueBytes.length) {
                    throw new SignatureNotFoundException("block end pos: " + (blockOffset + blockLength)
                            + " is larger than block len: " + signatureValueBytes.length);
                }
                if (HapUtils.getHapSignatureOptionalBlockIds().contains(blockType)) {
                    byte[] blockValue = Arrays.copyOfRange(signatureValueBytes, blockOffset, blockOffset + blockLength);
                    optionalBlocks.add(new SigningBlock(blockType, blockValue));
                }
                if (blockType == HapUtils.HAP_SIGNATURE_SCHEME_V1_BLOCK_ID) {
                    byte[] result = Arrays.copyOfRange(signatureValueBytes, blockOffset, blockOffset + blockLength);
                    hapSigningPkcs7Block = ByteBuffer.wrap(result);
                }
            }
            return Pair.create(hapSigningPkcs7Block, optionalBlocks);
        } finally {
            hapSigningBlock.clear();
        }
    }
}