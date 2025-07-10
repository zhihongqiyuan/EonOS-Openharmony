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

package com.ohos.hapsigntool.codesigning.sign;

import com.ohos.hapsigntool.codesigning.datastructure.CodeSignBlock;
import com.ohos.hapsigntool.codesigning.datastructure.CodeSignBlockHeader;
import com.ohos.hapsigntool.codesigning.datastructure.ElfSignBlock;
import com.ohos.hapsigntool.codesigning.datastructure.Extension;
import com.ohos.hapsigntool.codesigning.datastructure.FsVerityInfoSegment;
import com.ohos.hapsigntool.codesigning.datastructure.HapInfoSegment;
import com.ohos.hapsigntool.codesigning.datastructure.MerkleTreeExtension;
import com.ohos.hapsigntool.codesigning.datastructure.NativeLibInfoSegment;
import com.ohos.hapsigntool.codesigning.datastructure.SegmentHeader;
import com.ohos.hapsigntool.codesigning.datastructure.SignInfo;
import com.ohos.hapsigntool.codesigning.exception.FsVerityDigestException;
import com.ohos.hapsigntool.codesigning.exception.PageInfoException;
import com.ohos.hapsigntool.codesigning.exception.VerifyCodeSignException;
import com.ohos.hapsigntool.codesigning.fsverity.FsVerityGenerator;
import com.ohos.hapsigntool.codesigning.utils.CmsUtils;
import com.ohos.hapsigntool.codesigning.utils.HapUtils;
import com.ohos.hapsigntool.entity.Pair;
import com.ohos.hapsigntool.error.ProfileException;
import com.ohos.hapsigntool.utils.LogUtils;
import com.ohos.hapsigntool.utils.StringUtils;

import org.bouncycastle.asn1.ASN1ObjectIdentifier;
import org.bouncycastle.asn1.cms.Attribute;
import org.bouncycastle.asn1.cms.AttributeTable;
import org.bouncycastle.cms.CMSException;
import org.bouncycastle.cms.CMSSignedData;
import org.bouncycastle.cms.SignerInformation;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Arrays;
import java.util.Collection;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Locale;
import java.util.Map;
import java.util.Set;
import java.util.jar.JarEntry;
import java.util.jar.JarFile;
import java.util.zip.ZipInputStream;

/**
 * Verify code signature given a file with code sign block
 *
 * @since 2023/09/08
 */
public class VerifyCodeSignature {
    private static final LogUtils LOGGER = new LogUtils(VerifyCodeSignature.class);

    private static void checkOwnerID(byte[] signature, String profileOwnerID, String profileType)
        throws CMSException, VerifyCodeSignException {
        String ownerID = profileOwnerID;
        // if profileType is debug, check the app-id in signature, should be null or DEBUG_LIB_ID
        if ("debug".equals(profileType)) {
            ownerID = HapUtils.HAP_DEBUG_OWNER_ID;
        }
        checkSignatureOwnerID(ownerID, signature, profileType);
    }

    private static void checkHnpOwnerID(byte[] signature, String profileOwnerID, String profileType, String hnpType)
        throws CMSException, VerifyCodeSignException {
        String ownerID = profileOwnerID;
        // if profileType is debug, check the app-id in signature, should be null or DEBUG_LIB_ID
        if ("debug".equals(profileType)) {
            ownerID = HapUtils.HAP_DEBUG_OWNER_ID;
        } else if ("release".equals(profileType)) {
            if ("public".equals(hnpType)) {
                ownerID = HapUtils.HAP_SHARED_OWNER_ID;
            }
        }
        checkSignatureOwnerID(ownerID, signature, profileType);
    }

    private static void checkSignatureOwnerID(String ownerID, byte[] signature, String profileType)
        throws CMSException, VerifyCodeSignException {
        CMSSignedData cmsSignedData = new CMSSignedData(signature);
        Collection<SignerInformation> signers = cmsSignedData.getSignerInfos().getSigners();
        for (SignerInformation signer : signers) {
            AttributeTable attrTable = signer.getSignedAttributes();
            Attribute attr = attrTable.get(new ASN1ObjectIdentifier(BcSignedDataGenerator.SIGNER_OID));
            // if app-id is null, if profileType is debug, it's ok.
            if (attr == null) {
                if ("debug".equals(profileType)) {
                    continue;
                }
                if (ownerID == null) {
                    continue;
                } else {
                    throw new VerifyCodeSignException("app-identifier is not in the signature");
                }
            }
            if (ownerID == null) {
                throw new VerifyCodeSignException("app-identifier in profile is null, but is not null in signature");
            }
            // if app-id in signature exists, it should be equal to the app-id in profile.
            String resultOwnerID = attr.getAttrValues().getObjectAt(0).toString();
            if (!ownerID.equals(resultOwnerID)) {
                throw new VerifyCodeSignException("app-identifier in signature is invalid");
            }
        }
    }

    /**
     * Verify a signed elf's signature
     *
     * @param file       signed elf file
     * @param offset     start position of code sign block based on the start of the elf file
     * @param length     byte size of code sign block
     * @param fileFormat elf
     * @param profileContent profile json string
     * @return true if signature verify succeed and false otherwise
     * @throws IOException             If an input or output exception occurred
     * @throws VerifyCodeSignException parsing result invalid
     * @throws FsVerityDigestException if fs-verity digest generation failed
     * @throws CMSException            if signature verify failed
     * @throws ProfileException        if verify profile failed
     */
    public static boolean verifyElf(File file, long offset, long length, String fileFormat, String profileContent)
            throws IOException, VerifyCodeSignException, FsVerityDigestException, CMSException, ProfileException {
        if (!CodeSigning.SUPPORT_BIN_FILE_FORM.equalsIgnoreCase(fileFormat)) {
            LOGGER.info("Not elf file, skip code signing verify");
            return true;
        }
        // 1) parse sign block to ElfCodeSignBlock object
        ElfSignBlock elfSignBlock;
        try (FileInputStream signedElf = new FileInputStream(file)) {
            byte[] codeSignBlockBytes = new byte[(int) length];
            signedElf.skip(offset);
            signedElf.read(codeSignBlockBytes);
            elfSignBlock = ElfSignBlock.fromByteArray(codeSignBlockBytes);
        }
        // 2) verify file data
        try (FileInputStream signedElf = new FileInputStream(file)) {
            int paddingSize = ElfSignBlock.computeMerkleTreePaddingLength(offset);
            byte[] merkleTreeWithPadding = elfSignBlock.getMerkleTreeWithPadding();
            byte[] merkleTree = Arrays.copyOfRange(merkleTreeWithPadding, paddingSize, merkleTreeWithPadding.length);
            verifySingleFile(signedElf, elfSignBlock.getDataSize(), elfSignBlock.getSignature(),
                elfSignBlock.getTreeOffset(), merkleTree);
        }
        if (profileContent != null) {
            Pair<String, String> pairResult = HapUtils.parseAppIdentifier(profileContent);
            checkOwnerID(elfSignBlock.getSignature(), pairResult.getFirst(), pairResult.getSecond());
        }
        return true;
    }

    /**
     * Verify a signed hap's signature
     *
     * @param file       signed hap file
     * @param offset     start position of code sign block based on the start of the hap file
     * @param length     byte size of code sign block
     * @param fileFormat hap or hqf or hsp, etc.
     * @param profileContent profile of the hap
     * @return true if signature verify succeed and false otherwise
     * @throws IOException             If an input or output exception occurred
     * @throws VerifyCodeSignException parsing result invalid
     * @throws FsVerityDigestException if fs-verity digest generation failed
     * @throws CMSException            if signature verify failed
     * @throws ProfileException        profile of the hap failed
     */
    public static boolean verifyHap(File file, long offset, long length, String fileFormat, String profileContent)
            throws IOException, VerifyCodeSignException, FsVerityDigestException, CMSException, ProfileException {
        if (!StringUtils.containsIgnoreCase(CodeSigning.SUPPORT_FILE_FORM, fileFormat)) {
            LOGGER.info("Not hap, hsp or hqf file, skip code signing verify");
            return true;
        }
        Pair<String, String> pairResult = HapUtils.parseAppIdentifier(profileContent);

        CodeSignBlock csb = generateCodeSignBlock(file, offset, length);
        // 2) verify hap
        try (FileInputStream hap = new FileInputStream(file)) {
            long dataSize = csb.getHapInfoSegment().getSignInfo().getDataSize();
            byte[] signature = csb.getHapInfoSegment().getSignInfo().getSignature();
            Extension extension = csb.getHapInfoSegment()
                .getSignInfo()
                .getExtensionByType(MerkleTreeExtension.MERKLE_TREE_INLINED);
            MerkleTreeExtension mte = new MerkleTreeExtension(0, 0, null);
            if (extension instanceof MerkleTreeExtension) {
                mte = (MerkleTreeExtension) extension;
            }
            // temporary: merkle tree offset set to zero, change to merkleTreeOffset
            verifySingleFile(hap, dataSize, signature, mte.getMerkleTreeOffset(),
                csb.getOneMerkleTreeByFileName(CodeSigning.HAP_SIGNATURE_ENTRY_NAME));
            checkOwnerID(signature, pairResult.getFirst(), pairResult.getSecond());
        }
        // 3) verify native libs
        verifyLibs(file, csb, pairResult);
        return true;
    }

    private static void verifyLibs(File file, CodeSignBlock csb, Pair<String, String> pairResult)
        throws IOException, FsVerityDigestException, VerifyCodeSignException, CMSException, ProfileException {
        try (JarFile inputJar = new JarFile(file, false)) {
            Map<String, SignInfo> hnpLibSignInfoMap = new HashMap<>();
            Set<String> hnpEntryNames = new HashSet<>();
            for (int i = 0; i < csb.getSoInfoSegment().getSectionNum(); i++) {
                String entryName = csb.getSoInfoSegment().getFileNameList().get(i);
                SignInfo signInfo = csb.getSoInfoSegment().getSignInfoList().get(i);
                if (entryName.contains("!/")) {
                    String[] filePath = entryName.split("!/");
                    hnpEntryNames.add(filePath[0]);
                    hnpLibSignInfoMap.put(entryName, signInfo);
                } else {
                    LOGGER.info("verify lib: {}", entryName);
                    verifyHapLib(inputJar, entryName, signInfo, pairResult);
                }
            }
            if (hnpEntryNames.isEmpty()) {
                // not exists hnp lib
                return;
            }
            // get module.json
            Map<String, String> hnpTypeMap = HapUtils.getHnpsFromJson(inputJar);
            for (String hnpEntryName : hnpEntryNames) {
                verifyHnpLib(inputJar, hnpEntryName, hnpLibSignInfoMap, hnpTypeMap, pairResult);
            }
        }
    }

    private static void verifyHapLib(JarFile inputJar, String entryName, SignInfo signInfo,
        Pair<String, String> pairResult)
        throws IOException, FsVerityDigestException, VerifyCodeSignException, CMSException {

        JarEntry entry = inputJar.getJarEntry(entryName);
        if (entry.getSize() != signInfo.getDataSize()) {
            throw new VerifyCodeSignException(
                String.format(Locale.ROOT, "Invalid dataSize of native lib %s", entryName));
        }
        byte[] entrySig = signInfo.getSignature();
        try (InputStream entryInputStream = inputJar.getInputStream(entry)) {
            // temporary merkleTreeOffset 0
            verifySingleFile(entryInputStream, entry.getSize(), entrySig, 0, null);
            checkOwnerID(entrySig, pairResult.getFirst(), pairResult.getSecond());
        }
    }

    private static void verifyHnpLib(JarFile inputJar, String hnpEntryName, Map<String, SignInfo> hnpLibSignInfoMap,
        Map<String, String> hnpTypeMap, Pair<String, String> pairResult)
        throws IOException, FsVerityDigestException, VerifyCodeSignException, CMSException {
        JarEntry hnpEntry = inputJar.getJarEntry(hnpEntryName);
        try (InputStream inputStream = inputJar.getInputStream(hnpEntry);
            ZipInputStream hnpInputStream = new ZipInputStream(inputStream)) {
            String hnpFileName = HapUtils.parseHnpPath(hnpEntryName);
            if (!hnpTypeMap.containsKey(hnpFileName)) {
                throw new VerifyCodeSignException("hnp should be described in module.json");
            }
            String hnpType = hnpTypeMap.get(hnpFileName);
            java.util.zip.ZipEntry libEntry = null;
            while ((libEntry = hnpInputStream.getNextEntry()) != null) {
                String libPath = hnpEntry.getName() + "!/" + libEntry.getName();
                if (!hnpLibSignInfoMap.containsKey(libPath)) {
                    continue;
                }
                LOGGER.info("verify lib: {}", libPath);
                SignInfo signInfo = hnpLibSignInfoMap.get(libPath);
                byte[] entrySig = signInfo.getSignature();
                long dataSize = signInfo.getDataSize();
                verifySingleFile(hnpInputStream, dataSize, entrySig, 0, null);
                checkHnpOwnerID(entrySig, pairResult.getFirst(), pairResult.getSecond(), hnpType);
                hnpInputStream.closeEntry();
            }
        }
    }

    private static CodeSignBlock generateCodeSignBlock(File file, long offset, long length)
        throws IOException, VerifyCodeSignException {
        CodeSignBlock csb = new CodeSignBlock();
        // 1) parse sign block to CodeSignBlock object
        try (FileInputStream signedHap = new FileInputStream(file)) {
            int fileReadOffset = 0;
            // 0) skip data part, but fileReadOffset remains at start(0)
            signedHap.skip(offset);
            // 1) parse codeSignBlockHeader
            byte[] codeSignBlockHeaderByteArray = new byte[CodeSignBlockHeader.size()];
            fileReadOffset += signedHap.read(codeSignBlockHeaderByteArray);
            csb.setCodeSignBlockHeader(CodeSignBlockHeader.fromByteArray(codeSignBlockHeaderByteArray));
            if (csb.getCodeSignBlockHeader().getBlockSize() != length) {
                throw new VerifyCodeSignException("Invalid code Sign block size of setCodeSignBlockHeader");
            }
            // 2) parse segment headers
            for (int i = 0; i < csb.getCodeSignBlockHeader().getSegmentNum(); i++) {
                byte[] segmentHeaderByteArray = new byte[SegmentHeader.SEGMENT_HEADER_LENGTH];
                fileReadOffset += signedHap.read(segmentHeaderByteArray);
                csb.addToSegmentList(SegmentHeader.fromByteArray(segmentHeaderByteArray));
            }
            // compute merkle tree offset by alignment, based on file start
            long computedTreeOffset = getAlignmentAddr(CodeSignBlock.PAGE_SIZE_4K, fileReadOffset + offset);
            // skip zero padding before merkle tree, adds zero padding length to fileReadOffset
            fileReadOffset += signedHap.skip(computedTreeOffset - offset - fileReadOffset);
            parseMerkleTree(csb, fileReadOffset, signedHap, computedTreeOffset);
        }
        return csb;
    }

    private static void parseMerkleTree(CodeSignBlock csb, int readOffset, FileInputStream signedHap,
        long computedTreeOffset) throws VerifyCodeSignException, IOException {
        // check segment offset and segment size
        byte[] merkleTreeBytes = new byte[0];
        int fileReadOffset = readOffset;
        for (SegmentHeader segmentHeader : csb.getSegmentHeaderList()) {
            if (fileReadOffset > segmentHeader.getSegmentOffset()) {
                throw new VerifyCodeSignException("Invaild offset of merkle tree and segment header");
            }
            // get merkle tree bytes
            if (fileReadOffset < segmentHeader.getSegmentOffset()) {
                merkleTreeBytes = new byte[segmentHeader.getSegmentOffset() - fileReadOffset];
                fileReadOffset += signedHap.read(merkleTreeBytes);
            }
            byte[] sh = new byte[segmentHeader.getSegmentSize()];
            fileReadOffset += signedHap.read(sh);
            if (segmentHeader.getType() == SegmentHeader.CSB_FSVERITY_INFO_SEG) {
                // 3) parse fs-verity info segment
                csb.setFsVerityInfoSegment(FsVerityInfoSegment.fromByteArray(sh));
            } else if (segmentHeader.getType() == SegmentHeader.CSB_HAP_META_SEG) {
                // 4) parse hap info segment
                csb.setHapInfoSegment(HapInfoSegment.fromByteArray(sh));
            } else if (segmentHeader.getType() == SegmentHeader.CSB_NATIVE_LIB_INFO_SEG) {
                // 5) parse so info segment
                csb.setSoInfoSegment(NativeLibInfoSegment.fromByteArray(sh));
            }
        }
        if (fileReadOffset != csb.getCodeSignBlockHeader().getBlockSize()) {
            throw new VerifyCodeSignException("Invalid blockSize of getCodeSignBlockHeader");
        }
        // parse merkle tree
        Extension extension = csb.getHapInfoSegment()
            .getSignInfo()
            .getExtensionByType(MerkleTreeExtension.MERKLE_TREE_INLINED);
        if (extension == null) {
            throw new VerifyCodeSignException("Missing merkleTreeExtension in verifycation");
        }
        if (extension instanceof MerkleTreeExtension) {
            MerkleTreeExtension mte = (MerkleTreeExtension) extension;
            if (computedTreeOffset != mte.getMerkleTreeOffset()) {
                throw new VerifyCodeSignException("Invalid merkle tree offset");
            }
            if (merkleTreeBytes.length != mte.getMerkleTreeSize()) {
                throw new VerifyCodeSignException("Invalid merkle tree size");
            }
            csb.addOneMerkleTree(CodeSigning.HAP_SIGNATURE_ENTRY_NAME, merkleTreeBytes);
        }
    }

    private static long getAlignmentAddr(long alignment, long input) {
        long residual = input % alignment;
        if (residual == 0) {
            return input;
        } else {
            return input + (alignment - residual);
        }
    }

    /**
     * Verifies the signature of a given file with its signature in pkcs#7 format
     *
     * @param input             file being verified in InputStream representation
     * @param length            size of signed data in the file
     * @param signature         byte array of signature in pkcs#7 format
     * @param merkleTreeOffset  merkle tree offset based on file start
     * @param inMerkleTreeBytes merkle tree raw bytes
     * @throws FsVerityDigestException if fs-verity digest generation failed
     * @throws CMSException            if signature verify failed
     * @throws VerifyCodeSignException parsing code sign block error
     */
    public static void verifySingleFile(InputStream input, long length, byte[] signature, long merkleTreeOffset,
        byte[] inMerkleTreeBytes) throws FsVerityDigestException, CMSException, VerifyCodeSignException {
        Pair<byte[], byte[]> pairResult = null;
        try {
            pairResult = generateFsVerityDigest(input, length, merkleTreeOffset);
        } catch (PageInfoException e) {
            throw new VerifyCodeSignException(e.getMessage());
        }
        byte[] generatedMerkleTreeBytes = pairResult.getSecond();
        if (generatedMerkleTreeBytes == null) {
            generatedMerkleTreeBytes = new byte[0];
        }
        // For native libs, inMerkleTreeBytes is null, skip check here
        if ((inMerkleTreeBytes != null) && !Arrays.equals(inMerkleTreeBytes, generatedMerkleTreeBytes)) {
            throw new VerifyCodeSignException("verify merkle tree bytes failed");
        }
        CmsUtils.verifySignDataWithUnsignedDataDigest(pairResult.getFirst(), signature);
    }

    private static Pair<byte[], byte[]> generateFsVerityDigest(InputStream inputStream, long size,
        long merkleTreeOffset) throws FsVerityDigestException, PageInfoException {
        FsVerityGenerator fsVerityGenerator = new FsVerityGenerator();
        fsVerityGenerator.generateFsVerityDigest(inputStream, size, merkleTreeOffset);
        return Pair.create(fsVerityGenerator.getFsVerityDigest(), fsVerityGenerator.getTreeBytes());
    }
}
