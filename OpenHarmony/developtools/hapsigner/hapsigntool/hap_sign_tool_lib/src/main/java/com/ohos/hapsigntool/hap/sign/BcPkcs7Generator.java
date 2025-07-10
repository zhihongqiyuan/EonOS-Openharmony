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

import com.ohos.hapsigntool.entity.ContentDigestAlgorithm;
import com.ohos.hapsigntool.entity.SignatureAlgorithm;
import com.ohos.hapsigntool.hap.config.SignerConfig;
import com.ohos.hapsigntool.entity.Pair;
import com.ohos.hapsigntool.error.SignatureException;
import com.ohos.hapsigntool.hap.verify.VerifyUtils;

import com.ohos.hapsigntool.utils.LogUtils;
import org.bouncycastle.asn1.ASN1EncodableVector;
import org.bouncycastle.asn1.ASN1Encoding;
import org.bouncycastle.asn1.ASN1Integer;
import org.bouncycastle.asn1.ASN1ObjectIdentifier;
import org.bouncycastle.asn1.ASN1Set;
import org.bouncycastle.asn1.BERSet;
import org.bouncycastle.asn1.DEROctetString;
import org.bouncycastle.asn1.DERSet;
import org.bouncycastle.asn1.cms.Attribute;
import org.bouncycastle.asn1.cms.AttributeTable;
import org.bouncycastle.asn1.cms.Time;
import org.bouncycastle.asn1.pkcs.ContentInfo;
import org.bouncycastle.asn1.pkcs.IssuerAndSerialNumber;
import org.bouncycastle.asn1.pkcs.PKCSObjectIdentifiers;
import org.bouncycastle.asn1.pkcs.SignedData;
import org.bouncycastle.asn1.pkcs.SignerInfo;
import org.bouncycastle.asn1.x509.AlgorithmIdentifier;
import org.bouncycastle.cert.jcajce.JcaX509CRLHolder;
import org.bouncycastle.cert.jcajce.JcaX509CertificateHolder;
import org.bouncycastle.cms.CMSException;
import org.bouncycastle.cms.CMSSignedData;
import org.bouncycastle.operator.DefaultDigestAlgorithmIdentifierFinder;
import org.bouncycastle.operator.DefaultSignatureAlgorithmIdentifierFinder;
import org.bouncycastle.operator.DigestAlgorithmIdentifierFinder;
import org.bouncycastle.operator.SignatureAlgorithmIdentifierFinder;

import java.io.IOException;
import java.security.InvalidAlgorithmParameterException;
import java.security.InvalidKeyException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.security.PublicKey;
import java.security.Signature;
import java.security.cert.CRLException;
import java.security.cert.CertificateEncodingException;
import java.security.cert.X509CRL;
import java.security.cert.X509Certificate;
import java.security.spec.AlgorithmParameterSpec;
import java.util.Date;
import java.util.Hashtable;
import java.util.List;

/**
 * BC implementation
 *
 * @since 2021/12/21
 */
public class BcPkcs7Generator implements Pkcs7Generator {
    private static final LogUtils LOGGER = new LogUtils(BcPkcs7Generator.class);
    private static final SignatureAlgorithmIdentifierFinder SIGN_ALG_FINDER =
        new DefaultSignatureAlgorithmIdentifierFinder();
    private static final DigestAlgorithmIdentifierFinder DIGEST_ALG_FINDER =
        new DefaultDigestAlgorithmIdentifierFinder();

    private static final String SIGNATURE_VERIFY_FAILED = "Signature did not verify";


    @Override
    public byte[] generateSignedData(byte[] content, SignerConfig signerConfig) throws SignatureException {
        if (content == null) {
            throw new SignatureException("unsigned data is null");
        }
        ASN1EncodableVector signerInfoLst = new ASN1EncodableVector();
        ASN1EncodableVector algorithmIdLst = new ASN1EncodableVector();
        for (SignatureAlgorithm signatureAlgorithm : signerConfig.getSignatureAlgorithms()) {
            try {
                SignerInfo signerInfo = getSignerInfo(signatureAlgorithm, content, signerConfig);
                    algorithmIdLst.add(signerInfo.getDigestAlgorithm());
                    signerInfoLst.add(signerInfo);
                LOGGER.info("Add sign data in sign info list success.");
            } catch (NoSuchAlgorithmException e) {
                throw new SignatureException(
                        "Invalid algorithm: " + signatureAlgorithm.getContentDigestAlgorithm().name(), e);
            } catch (IOException e) {
                throw new SignatureException("sign IOException" + e.getMessage(), e);
            }
        }
        return packagePKCS7(signerConfig, new DERSet(signerInfoLst), new DERSet(algorithmIdLst), content);
    }

    private byte[] packagePKCS7(
            SignerConfig signerConfig,
            ASN1Set signerInfoLst,
            ASN1Set algorithmIdLst,
            byte[] unsignedHapDigest)
            throws SignatureException {
        ContentInfo contentInfo = new ContentInfo(PKCSObjectIdentifiers.data, new DEROctetString(unsignedHapDigest));
        ASN1Set certs = null;
        ASN1Set crls = null;
        byte[] signBlock;
        try {
            if (checkListNotNullOrEmpty(signerConfig.getCertificates())) {
                certs = createBerSetFromCerts(signerConfig.getCertificates());
            }
            if (checkListNotNullOrEmpty(signerConfig.getX509CRLs())) {
                crls = createBerSetFromCrls(signerConfig.getX509CRLs());
            }
            SignedData signedData = new SignedData(
                new ASN1Integer(1), algorithmIdLst, contentInfo, certs, crls, signerInfoLst);
            ContentInfo pkcs7 = new ContentInfo(PKCSObjectIdentifiers.signedData, signedData);
            signBlock = pkcs7.getEncoded(ASN1Encoding.DER);
        } catch (CertificateEncodingException | CRLException | IOException e) {
            throw new SignatureException("Packaging PKCS cms data failed!", e);
        }
        boolean verifyResult = false;
        try {
            CMSSignedData cmsSignedData = new CMSSignedData(signBlock);
            verifyResult = VerifyUtils.verifyCmsSignedData(cmsSignedData);
        } catch (CMSException e) {
            throw new SignatureException("PKCS cms data verify failed", e);
        }
        if (!verifyResult) {
            throw new SignatureException("PKCS cms data did not verify");
        }
        return signBlock;
    }

    private SignerInfo getSignerInfo(
            SignatureAlgorithm signatureAlgorithm, byte[] unsignedHapDigest, SignerConfig signerConfig)
            throws SignatureException, IOException, NoSuchAlgorithmException {
        Pair<String, ? extends AlgorithmParameterSpec> signatureParams = signatureAlgorithm.getSignatureAlgAndParams();
        ContentDigestAlgorithm contentDigestAlg = signatureAlgorithm.getContentDigestAlgorithm();
        String jcaSignatureAlg = signatureParams.getFirst();
        MessageDigest md = MessageDigest.getInstance(contentDigestAlg.name());
        byte[] digest = md.digest(unsignedHapDigest);
        ASN1Set authed = generatePKCS9Attributes(digest);

        // Get sign data content from sign server
        byte[] signatureBytes = signerConfig.getSigner().getSignature(
            authed.getEncoded(), jcaSignatureAlg, signatureParams.getSecond());
        if (signatureBytes == null) {
            throw new SignatureException("Generate signature bytes error");
        }
        if (!checkListNotNullOrEmpty(signerConfig.getCertificates())) {
            throw new SignatureException("No certificates configured for signer");
        }

        Pair<String, ? extends AlgorithmParameterSpec> obj = signatureAlgorithm.getSignatureAlgAndParams();
        Pair<String, AlgorithmParameterSpec> signAlgPair = Pair.create(obj.getFirst(), obj.getSecond());
        if (!verifySignatureFromServer(signerConfig, signatureBytes, signAlgPair, authed)) {
            throw new SignatureException(SIGNATURE_VERIFY_FAILED);
        }
        return createSignerInfo(signerConfig, signatureAlgorithm, authed, signatureBytes);
    }

    private SignerInfo createSignerInfo(
            SignerConfig signerConfig,
            SignatureAlgorithm signatureAlgorithm,
            ASN1Set authed,
            byte[] signedHapDigest)
            throws SignatureException {
        String digestAlgorithm = signatureAlgorithm.getContentDigestAlgorithm().getDigestAlgorithm();
        String signAlg = signatureAlgorithm.getSignatureAlgAndParams().getFirst();
        try {
            JcaX509CertificateHolder certificateHolder =
                new JcaX509CertificateHolder(signerConfig.getCertificates().get(0));
            AlgorithmIdentifier digestAlgId = DIGEST_ALG_FINDER.find(digestAlgorithm);
            AlgorithmIdentifier signAlgId = SIGN_ALG_FINDER.find(signAlg);
            IssuerAndSerialNumber issuerAndSerialNumber =
                new IssuerAndSerialNumber(certificateHolder.getIssuer(), certificateHolder.getSerialNumber());
            return new SignerInfo(new ASN1Integer(1), issuerAndSerialNumber, digestAlgId,
                    authed, signAlgId, new DEROctetString(signedHapDigest), null);
        } catch (CertificateEncodingException e) {
            throw new SignatureException("Generate signer info error", e);
        }
    }

    private ASN1Set generatePKCS9Attributes(byte[] digest) {
        Hashtable<ASN1ObjectIdentifier, Attribute> tab = new Hashtable<>();
        Attribute signTime = new Attribute(PKCSObjectIdentifiers.pkcs_9_at_signingTime,
            new DERSet(new Time(new Date())));
        Attribute contentType = new Attribute(PKCSObjectIdentifiers.pkcs_9_at_contentType,
            new DERSet(PKCSObjectIdentifiers.data));
        Attribute digestAtt = new Attribute(PKCSObjectIdentifiers.pkcs_9_at_messageDigest,
            new DERSet(new DEROctetString(digest)));
        tab.put(signTime.getAttrType(), signTime);
        tab.put(contentType.getAttrType(), contentType);
        tab.put(digestAtt.getAttrType(), digestAtt);
        return new DERSet(new AttributeTable(tab).toASN1EncodableVector());
    }

    private ASN1Set createBerSetFromCrls(List<X509CRL> crls) throws CRLException {
        if (crls == null || crls.isEmpty()) {
            return null;
        }
        ASN1EncodableVector vector = new ASN1EncodableVector();
        for (X509CRL crl : crls) {
            vector.add(new JcaX509CRLHolder(crl).toASN1Structure());
        }
        return new BERSet(vector);
    }

    private ASN1Set createBerSetFromCerts(List<X509Certificate> certs) throws CertificateEncodingException {
        if (certs == null || certs.isEmpty()) {
            return null;
        }
        ASN1EncodableVector vector = new ASN1EncodableVector();

        for (X509Certificate cert : certs) {
            vector.add(new JcaX509CertificateHolder(cert).toASN1Structure());
        }
        return new BERSet(vector);
    }

    private boolean checkListNotNullOrEmpty(List<?> lists) {
        return (lists != null) && (!lists.isEmpty());
    }

    private boolean verifySignatureFromServer(
            SignerConfig signerConfig,
            byte[] signatureBytes,
            Pair<String, AlgorithmParameterSpec> signAlgPair,
            ASN1Set authed)
            throws SignatureException {
        try {
            PublicKey publicKey = signerConfig.getCertificates().get(0).getPublicKey();
            Signature signature = Signature.getInstance(signAlgPair.getFirst());
            signature.initVerify(publicKey);
            if (signAlgPair.getSecond() != null) {
                signature.setParameter(signAlgPair.getSecond());
            }
            signature.update(authed.getEncoded());
            if (signatureBytes == null) {
                LOGGER.error("signatureBytes is null");
                throw new SignatureException(SIGNATURE_VERIFY_FAILED);
            }
            if (!signature.verify(signatureBytes)) {
                throw new SignatureException(SIGNATURE_VERIFY_FAILED);
            }
            return true;
        } catch (InvalidKeyException | java.security.SignatureException e) {
            LOGGER.error("Failed to verify generated signature using public key from certificate", e);
        } catch (NoSuchAlgorithmException e) {
            LOGGER.error("Failed to verify generated " + signAlgPair.getFirst()
                    + " signature using public key from certificate", e);
        } catch (InvalidAlgorithmParameterException e) {
            LOGGER.error("Failed to verify generated " + signAlgPair.getSecond()
                    + " signature using public key from certificate", e);
        } catch (IOException e) {
            LOGGER.error("PKCS9 Attributes encode failed.", e);
        }
        return false;
    }
}
