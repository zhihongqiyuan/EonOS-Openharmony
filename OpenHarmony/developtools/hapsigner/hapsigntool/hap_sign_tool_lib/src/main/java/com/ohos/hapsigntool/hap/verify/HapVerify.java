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

import com.ohos.hapsigntool.hap.entity.SigningBlock;
import com.ohos.hapsigntool.entity.ContentDigestAlgorithm;
import com.ohos.hapsigntool.entity.SignatureAlgorithm;
import com.ohos.hapsigntool.utils.DigestUtils;
import com.ohos.hapsigntool.hap.utils.HapUtils;
import com.ohos.hapsigntool.utils.LogUtils;
import com.ohos.hapsigntool.zip.ZipDataInput;

import org.bouncycastle.cert.X509CRLHolder;
import org.bouncycastle.cert.X509CertificateHolder;
import org.bouncycastle.cert.jcajce.JcaX509CRLConverter;
import org.bouncycastle.cert.jcajce.JcaX509CertificateConverter;
import org.bouncycastle.cms.CMSException;
import org.bouncycastle.cms.CMSSignedData;
import org.bouncycastle.cms.SignerInformation;
import org.bouncycastle.cms.SignerInformationStore;
import org.bouncycastle.util.Store;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.security.DigestException;
import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;
import java.security.NoSuchProviderException;
import java.security.PublicKey;
import java.security.SignatureException;
import java.security.cert.CRLException;
import java.security.cert.CertificateEncodingException;
import java.security.cert.CertificateException;
import java.security.cert.X509CRL;
import java.security.cert.X509CRLEntry;
import java.security.cert.X509Certificate;
import java.security.interfaces.DSAKey;
import java.security.interfaces.DSAParams;
import java.security.interfaces.ECKey;
import java.security.interfaces.RSAKey;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Collections;
import java.util.Date;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;

/**
 * Class used to verify hap-file with signature
 *
 * @since 2021/12/22
 */
public class HapVerify {
    private static final LogUtils LOGGER = new LogUtils(HapVerify.class);

    private static final DateFormat FORMAT = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");

    private ZipDataInput beforeApkSigningBlock;

    private ByteBuffer signatureSchemeBlock;

    private ZipDataInput centralDirectoryBlock;

    private ZipDataInput eocd;

    private List<SigningBlock> optionalBlocks;

    private Map<ContentDigestAlgorithm, byte[]> digestMap = new HashMap<ContentDigestAlgorithm, byte[]>();

    private JcaX509CertificateConverter certificateConverter = new JcaX509CertificateConverter();

    private JcaX509CRLConverter crlConverter = new JcaX509CRLConverter();

    private boolean isPrintCert;

    /**
     * Init Zip HapVerify
     *
     * @param beforeApkSigningBlock beforeApkSigningBlock
     * @param signatureSchemeBlock signatureSchemeBlock
     * @param centralDirectoryBlock centralDirectoryBlock
     * @param eocd eocd
     * @param optionalBlocks optionalBlocks
     */
    public HapVerify(
            ZipDataInput beforeApkSigningBlock,
            ByteBuffer signatureSchemeBlock,
            ZipDataInput centralDirectoryBlock,
            ZipDataInput eocd,
            List<SigningBlock> optionalBlocks) {
        this.beforeApkSigningBlock = beforeApkSigningBlock;
        this.signatureSchemeBlock = signatureSchemeBlock;
        this.centralDirectoryBlock = centralDirectoryBlock;
        this.eocd = eocd;
        this.optionalBlocks = optionalBlocks;
    }

    /**
     * init HapVerify
     */
    public HapVerify() {
    }

    /**
     * Verify hap signature.
     *
     * @return verify result.
     */
    public VerifyResult verify() {
        return parserSigner(signatureSchemeBlock);
    }

    /**
     * Verify elf signature.
     *
     * @param profile profile byte
     * @return verify result.
     */
    public VerifyResult verifyElfProfile(byte[] profile) {
        return parserSigner(ByteBuffer.wrap(profile), false);
    }

    public void setIsPrintCert(boolean isPrintCert) {
        this.isPrintCert = isPrintCert;
    }

    private boolean checkCRL(X509CRL crl, List<X509Certificate> certificates) {
        boolean isRet = false;
        for (X509Certificate cert : certificates) {
            if (!crl.getIssuerDN().getName().equals(cert.getIssuerDN().getName())) {
                continue;
            }
            X509CRLEntry entry = crl.getRevokedCertificate(cert);
            if (entry != null) {
                LOGGER.info("cert(subject DN = {}) is revoked by crl (IssuerDN = {})",
                        cert.getSubjectDN().getName(), crl.getIssuerDN().getName());
                isRet = false;
                break;
            }
            isRet = true;
        }
        return isRet;
    }

    private boolean verifyCRL(X509CRL crl, X509Certificate cert, List<X509Certificate> certificates)
            throws SignatureException {
        try {
            crl.verify(cert.getPublicKey());
            return checkCRL(crl, certificates);
        } catch (NoSuchAlgorithmException
            | InvalidKeyException
            | SignatureException
            | CRLException
            | NoSuchProviderException e) {
            throw new SignatureException("crl verify failed.", e);
        }
    }

    private boolean verifyCRL(X509CRL crl, List<X509Certificate> certificates) throws SignatureException {
        boolean isRevoked = true;
        for (X509Certificate cert : certificates) {
            if (!crl.getIssuerDN().getName().equals(cert.getSubjectDN().getName())) {
                continue;
            }
            if (!verifyCRL(crl, cert, certificates)) {
                isRevoked = false;
            }
        }
        return isRevoked;
    }

    private void verifyCRLs(List<X509CRL> crls, List<X509Certificate> certificates) throws VerifyHapException {
        if (crls == null) {
            return;
        }
        boolean isRevoked = true;
        try {
            for (X509CRL crl : crls) {
                if (!verifyCRL(crl, certificates)) {
                    isRevoked = false;
                }
            }
        } catch (SignatureException e) {
            throw new VerifyHapException("Verify CRL error!", e);
        }
        if (!isRevoked) {
            throw new VerifyHapException("Certificate is revoked!");
        }
    }

    private CMSSignedData verifyCmsSignedData(byte[] signingBlock) throws VerifyHapException {
        try {
            CMSSignedData cmsSignedData = new CMSSignedData(signingBlock);
            boolean isVerifyResult = VerifyUtils.verifyCmsSignedData(cmsSignedData);
            if (!isVerifyResult) {
                throw new VerifyHapException("Verify PKCS7 cms data failed!");
            }
            return cmsSignedData;
        } catch (CMSException e) {
            throw new VerifyHapException("Verify PKCS7 cms data error!", e);
        }
    }

    private VerifyResult parserSigner(ByteBuffer signer) {
        return parserSigner(signer, true);
    }

    private VerifyResult parserSigner(ByteBuffer signer, boolean verifyContent) {
        byte[] signingBlock = new byte[signer.remaining()];
        signer.get(signingBlock);
        try {
            CMSSignedData cmsSignedData = verifyCmsSignedData(signingBlock);
            List<X509Certificate> certificates = getCertChain(cmsSignedData);
            List<X509CRL> crlList = getCrlList(cmsSignedData);
            verifyCRLs(crlList, certificates);
            if (verifyContent) {
                checkContentDigest(cmsSignedData);
            }
            List<SignerInformation> signerInfos = getSignerInformations(cmsSignedData);
            VerifyResult result = new VerifyResult(true, VerifyResult.RET_SUCCESS, "Verify success");
            result.setCrls(crlList);
            result.setCertificates(certificates);
            result.setCertificateHolderStore(cmsSignedData.getCertificates());
            result.setSignerInfos(signerInfos);
            result.setOptionalBlocks(optionalBlocks);
            return result;
        } catch (VerifyHapException e) {
            LOGGER.error("Verify profile error!", e);
            return new VerifyResult(false, VerifyResult.RET_UNKNOWN_ERROR, e.getMessage());
        }
    }

    private List<SignerInformation> getSignerInformations(CMSSignedData cmsSignedData) throws VerifyHapException {
        SignerInformationStore signerInfos = cmsSignedData.getSignerInfos();
        int size = signerInfos.size();
        if (size <= 0) {
            throw new VerifyHapException("PKCS7 cms data has no signer info, size: " + size);
        }
        Collection<SignerInformation> signers = signerInfos.getSigners();
        return new ArrayList<>(signers);
    }

    private void checkContentDigest(CMSSignedData cmsSignedData) throws VerifyHapException {
        Object content = cmsSignedData.getSignedContent().getContent();
        byte[] contentBytes = null;
        if (content instanceof byte[]) {
            contentBytes = (byte[]) content;
        } else {
            throw new VerifyHapException("PKCS cms content is not a byte array!");
        }
        try {
            boolean isCheckResult = parserContentinfo(contentBytes);
            if (!isCheckResult) {
                throw new VerifyHapException("Hap content digest check failed.");
            }
        } catch (DigestException | SignatureException | IOException e) {
            throw new VerifyHapException("Check Hap content digest error!", e);
        }
    }

    private List<X509Certificate> getCertChain(CMSSignedData cmsSignedData) throws VerifyHapException {
        Store<X509CertificateHolder> certificates = cmsSignedData.getCertificates();
        try {
            List<X509Certificate> certificateList = certStoreToCertList(certificates);
            if (certificateList.isEmpty()) {
                throw new VerifyHapException("Certificate chain is empty!");
            }
            if (isPrintCert) {
                for (int i = 0; i < certificateList.size(); i++) {
                    LOGGER.info("+++++++++++++++++++++++++++certificate #{} +++++++++++++++++++++++++++++++", i);
                    printCert(certificateList.get(i));
                }
            }
            return certificateList;
        } catch (CertificateException e) {
            throw new VerifyHapException("Get certificate chain error!", e);
        }
    }

    private List<X509CRL> getCrlList(CMSSignedData cmsSignedData) throws VerifyHapException {
        Store<X509CRLHolder> crLs = cmsSignedData.getCRLs();
        if (crLs == null) {
            return Collections.emptyList();
        }
        Collection<X509CRLHolder> matches = crLs.getMatches(null);
        if (matches == null || !matches.iterator().hasNext()) {
            return Collections.emptyList();
        }
        Iterator<X509CRLHolder> iterator = matches.iterator();
        List<X509CRL> crlList = new ArrayList<>();
        try {
            while (iterator.hasNext()) {
                X509CRLHolder crlHolder = iterator.next();
                crlList.add(crlConverter.getCRL(crlHolder));
            }
        } catch (CRLException e) {
            throw new VerifyHapException("Get CRL error!", e);
        }
        return crlList;
    }

    private List<X509Certificate> certStoreToCertList(Store<X509CertificateHolder> certificates)
            throws CertificateException {
        if (certificates == null) {
            return Collections.emptyList();
        }
        Collection<X509CertificateHolder> matches = certificates.getMatches(null);
        if (matches == null || !matches.iterator().hasNext()) {
            return Collections.emptyList();
        }
        List<X509Certificate> certificateList = new ArrayList<>();
        Iterator<X509CertificateHolder> iterator = matches.iterator();
        while (iterator.hasNext()) {
            X509CertificateHolder next = iterator.next();
            certificateList.add(certificateConverter.getCertificate(next));
        }
        return certificateList;
    }

    private boolean parserContentinfo(byte[] data)
            throws DigestException, SignatureException, IOException {
        ByteBuffer digestDatas = ByteBuffer.wrap(data).order(ByteOrder.LITTLE_ENDIAN);
        while (digestDatas.remaining() > 4) {
            /**
             * contentinfo format:
             * int: version
             * int: block number
             * digest blocks:
             * each digest block format:
             * int: length of sizeof(digestblock) - 4
             * int: Algorithm ID
             * int: length of digest
             * byte[]: digest
             */
            int signBlockVersion = digestDatas.getInt();
            int signBlockCount = digestDatas.getInt();
            LOGGER.info("version is: {}, number of block is: {}", signBlockVersion, signBlockCount);
            int digestBlockLen = digestDatas.getInt();
            int signatureAlgId = digestDatas.getInt();
            int digestDataLen = digestDatas.getInt();
            if (digestBlockLen != digestDataLen + 8) {
                throw new SignatureException("digestBlockLen: " + digestBlockLen + ", digestDataLen: " + digestDataLen);
            }
            ByteBuffer digestBuffer = HapUtils.sliceBuffer(digestDatas, digestDataLen);
            byte[] digestData = new byte[digestBuffer.remaining()];
            digestBuffer.get(digestData);
            SignatureAlgorithm signatureAlgorithm = SignatureAlgorithm.findById(signatureAlgId);
            if (signatureAlgorithm == null) {
                throw new SignatureException("Unsupported SignatureAlgorithm ID : " + signatureAlgId);
            }
            digestMap.put(signatureAlgorithm.getContentDigestAlgorithm(), digestData);
        }

        Set<ContentDigestAlgorithm> keySet = digestMap.keySet();
        Map<ContentDigestAlgorithm, byte[]> actualDigestMap = HapUtils.computeDigests(
                keySet, new ZipDataInput[]{beforeApkSigningBlock, centralDirectoryBlock, eocd}, optionalBlocks);
        boolean isResult = true;
        for (Entry<ContentDigestAlgorithm, byte[]> entry : digestMap.entrySet()) {
            ContentDigestAlgorithm digestAlg = entry.getKey();
            byte[] exceptDigest = entry.getValue();
            byte[] actualDigest = actualDigestMap.get(digestAlg);
            if (!Arrays.equals(actualDigest, exceptDigest)) {
                isResult = false;
                LOGGER.error(
                        "digest data do not match! DigestAlgorithm: {}, actualDigest: <{}> VS exceptDigest : <{}>",
                        digestAlg.getDigestAlgorithm(),
                        HapUtils.toHex(actualDigest, ""),
                        HapUtils.toHex(exceptDigest, ""));
            }
            LOGGER.info("Digest verify result: {}, DigestAlgorithm: {}", isResult, digestAlg.getDigestAlgorithm());
        }
        return isResult;
    }

    private void printCert(X509Certificate cert) throws CertificateEncodingException {
        byte[] encodedCert = cert.getEncoded();

        LOGGER.info("Subject: {}", cert.getSubjectX500Principal());
        LOGGER.info("Issuer: {}", cert.getIssuerX500Principal());
        LOGGER.info("SerialNumber: {}", cert.getSerialNumber().toString(16));
        LOGGER.info("Validity: {} ~ {}", formatDateTime(cert.getNotBefore()), formatDateTime(cert.getNotAfter()));
        LOGGER.info("SHA256: {}", HapUtils.toHex(DigestUtils.sha256Digest(encodedCert), ":"));
        LOGGER.info("Signature Algorithm: {}", cert.getSigAlgName());
        PublicKey publicKey = cert.getPublicKey();
        LOGGER.info("Key: {}, key length: {} bits", publicKey.getAlgorithm(), getKeySize(publicKey));
        LOGGER.info("Cert Version: V{}", cert.getVersion());
    }

    private int getKeySize(PublicKey publicKey) {
        int result = -1;
        if (publicKey instanceof RSAKey) {
            result = ((RSAKey) publicKey).getModulus().bitLength();
        }
        if (publicKey instanceof ECKey) {
            result = ((ECKey) publicKey).getParams().getOrder().bitLength();
        }
        if (publicKey instanceof DSAKey) {
            DSAParams dsaParams = ((DSAKey) publicKey).getParams();
            if (dsaParams != null) {
                result = dsaParams.getP().bitLength();
            }
        }
        return result;
    }

    private String formatDateTime(Date date) {
        if (date != null) {
            return FORMAT.format(date);
        }
        return "";
    }

    private static class VerifyHapException extends Exception {
        VerifyHapException(String message) {
            super(message);
        }

        VerifyHapException(String message, Throwable cause) {
            super(message, cause);
        }
    }
}
