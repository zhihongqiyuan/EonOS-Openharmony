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

import com.ohos.hapsigntool.codesigning.exception.CodeSignErrMsg;
import com.ohos.hapsigntool.codesigning.exception.CodeSignException;
import com.ohos.hapsigntool.codesigning.utils.CmsUtils;
import com.ohos.hapsigntool.codesigning.utils.DigestUtils;
import com.ohos.hapsigntool.entity.Pair;
import com.ohos.hapsigntool.entity.ContentDigestAlgorithm;
import com.ohos.hapsigntool.entity.SignatureAlgorithm;
import com.ohos.hapsigntool.hap.config.SignerConfig;
import com.ohos.hapsigntool.utils.LogUtils;
import org.bouncycastle.asn1.ASN1EncodableVector;
import org.bouncycastle.asn1.ASN1Encoding;
import org.bouncycastle.asn1.ASN1Integer;
import org.bouncycastle.asn1.ASN1Set;
import org.bouncycastle.asn1.BERSet;
import org.bouncycastle.asn1.DEROctetString;
import org.bouncycastle.asn1.DERSet;
import org.bouncycastle.asn1.ASN1ObjectIdentifier;
import org.bouncycastle.asn1.DERUTF8String;
import org.bouncycastle.asn1.cms.Time;
import org.bouncycastle.asn1.pkcs.Attribute;
import org.bouncycastle.asn1.pkcs.ContentInfo;
import org.bouncycastle.asn1.pkcs.IssuerAndSerialNumber;
import org.bouncycastle.asn1.pkcs.PKCSObjectIdentifiers;
import org.bouncycastle.asn1.pkcs.SignedData;
import org.bouncycastle.asn1.pkcs.SignerInfo;
import org.bouncycastle.cert.jcajce.JcaX509CRLHolder;
import org.bouncycastle.cert.jcajce.JcaX509CertificateHolder;
import org.bouncycastle.cms.CMSException;
import org.bouncycastle.operator.DefaultDigestAlgorithmIdentifierFinder;
import org.bouncycastle.operator.DefaultSignatureAlgorithmIdentifierFinder;
import org.bouncycastle.operator.DigestAlgorithmIdentifierFinder;
import org.bouncycastle.operator.SignatureAlgorithmIdentifierFinder;

import java.io.IOException;
import java.security.InvalidAlgorithmParameterException;
import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;
import java.security.PublicKey;
import java.security.Signature;
import java.security.SignatureException;
import java.security.cert.CRLException;
import java.security.cert.CertificateEncodingException;
import java.security.cert.X509CRL;
import java.security.cert.X509Certificate;
import java.security.spec.AlgorithmParameterSpec;
import java.util.Date;
import java.util.List;

/**
 * BC implementation
 *
 * @since 2023/06/05
 */
public class BcSignedDataGenerator implements SignedDataGenerator {
    /**
     * OID of the signer identity
     */
    public static final String SIGNER_OID = "1.3.6.1.4.1.2011.2.376.1.4.1";

    private static final LogUtils LOGGER = new LogUtils(BcSignedDataGenerator.class);

    private static final SignatureAlgorithmIdentifierFinder SIGN_ALG_ID_FINDER
        = new DefaultSignatureAlgorithmIdentifierFinder();

    private static final DigestAlgorithmIdentifierFinder DIGEST_ALG_ID_FINDER
        = new DefaultDigestAlgorithmIdentifierFinder();

    private String ownerID;

    public void setOwnerID(String ownerID) {
        this.ownerID = ownerID;
    }

    @Override
    public byte[] generateSignedData(byte[] content, SignerConfig signConfig) throws CodeSignException {
        if (content == null) {
            throw new CodeSignException(
                CodeSignErrMsg.CODE_SIGN_INTERNAL_ERROR.toString("The content to be signed is empty"));
        }
        Pair<DERSet, DERSet> pairDigestAndSignInfo = getSignInfo(content, signConfig);
        // Unsupported certificate revocation, SignedData's _crls is null
        SignedData signedData = new SignedData(new ASN1Integer(1), pairDigestAndSignInfo.getFirst(),
            new ContentInfo(PKCSObjectIdentifiers.data, null), createBerSetFromLst(signConfig.getCertificates()),
            createBerSetFromLst(null), pairDigestAndSignInfo.getSecond());
        return encodingUnsignedData(content, signedData);
    }

    private Pair<DERSet, DERSet> getSignInfo(byte[] content, SignerConfig signConfig) throws CodeSignException {
        ASN1EncodableVector signInfoVector = new ASN1EncodableVector();
        ASN1EncodableVector digestVector = new ASN1EncodableVector();
        for (SignatureAlgorithm signAlgorithm : signConfig.getSignatureAlgorithms()) {
            SignerInfo signInfo = createSignInfo(signAlgorithm, content, signConfig);
            signInfoVector.add(signInfo);
            digestVector.add(signInfo.getDigestAlgorithm());
            LOGGER.info("Create a sign info successfully.");
        }
        return Pair.create(new DERSet(digestVector), new DERSet(signInfoVector));
    }

    private SignerInfo createSignInfo(SignatureAlgorithm signAlgorithm, byte[] unsignedDataDigest,
        SignerConfig signConfig) throws CodeSignException {
        ContentDigestAlgorithm hashAlgorithm = signAlgorithm.getContentDigestAlgorithm();
        byte[] digest = computeDigest(unsignedDataDigest, hashAlgorithm.name());
        ASN1Set authed = getPKCS9Attributes(digest);
        byte[] codeAuthed = getEncoded(authed);
        Pair<String, ? extends AlgorithmParameterSpec> signPair = signAlgorithm.getSignatureAlgAndParams();
        byte[] signBytes = signConfig.getSigner().getSignature(codeAuthed, signPair.getFirst(), signPair.getSecond());
        if (signBytes == null) {
            throw new CodeSignException(CodeSignErrMsg.CODE_SIGN_INTERNAL_ERROR.toString("Get signature failed"));
        }
        if (signConfig.getCertificates().isEmpty()) {
            throw new CodeSignException(
                CodeSignErrMsg.CERTIFICATES_CONFIGURE_ERROR.toString("No certificate is configured for sign"));
        }
        X509Certificate cert = signConfig.getCertificates().get(0);
        if (!verifySignFromServer(cert.getPublicKey(), signBytes, signPair, codeAuthed)) {
            throw new CodeSignException(
                CodeSignErrMsg.CODE_SIGN_INTERNAL_ERROR.toString("Verify signed result failed"));
        }
        JcaX509CertificateHolder certificateHolder = getJcaX509CertificateHolder(cert);
        return new SignerInfo(new ASN1Integer(1),
            new IssuerAndSerialNumber(certificateHolder.getIssuer(), certificateHolder.getSerialNumber()),
            DIGEST_ALG_ID_FINDER.find(hashAlgorithm.getDigestAlgorithm()), authed,
            SIGN_ALG_ID_FINDER.find(signPair.getFirst()), new DEROctetString(signBytes), null);
    }

    private byte[] computeDigest(byte[] unsignedDataDigest, String algorithm) throws CodeSignException {
        byte[] digest;
        try {
            digest = DigestUtils.computeDigest(unsignedDataDigest, algorithm);
        } catch (NoSuchAlgorithmException e) {
            throw new CodeSignException(CodeSignErrMsg.ALGORITHM_NOT_SUPPORT_ERROR.toString(algorithm), e);
        }
        return digest;
    }

    private byte[] getEncoded(ASN1Set authed) throws CodeSignException {
        byte[] codeAuthed;
        try {
            codeAuthed = authed.getEncoded();
        } catch (IOException e) {
            throw new CodeSignException(CodeSignErrMsg.CODE_SIGN_INTERNAL_ERROR.toString("Encode data error"), e);
        }
        return codeAuthed;
    }

    private JcaX509CRLHolder getJcaX509CRLHolder(X509CRL crl) throws CodeSignException {
        JcaX509CRLHolder crlHolder;
        try {
            crlHolder = new JcaX509CRLHolder(crl);
        } catch (CRLException e) {
            throw new CodeSignException(CodeSignErrMsg.CODE_SIGN_INTERNAL_ERROR.toString("Create crl failed"), e);
        }
        return crlHolder;
    }

    private JcaX509CertificateHolder getJcaX509CertificateHolder(X509Certificate cert) throws CodeSignException {
        JcaX509CertificateHolder certificateHolder;
        try {
            certificateHolder = new JcaX509CertificateHolder(cert);
        } catch (CertificateEncodingException e) {
            throw new CodeSignException(CodeSignErrMsg.CODE_SIGN_INTERNAL_ERROR.toString("Create sign info failed"), e);
        }
        return certificateHolder;
    }

    private ASN1Set getPKCS9Attributes(byte[] digest) {
        ASN1EncodableVector table = new ASN1EncodableVector();
        Attribute signingTimeAttr = new Attribute(PKCSObjectIdentifiers.pkcs_9_at_signingTime,
            new DERSet(new Time(new Date())));
        Attribute contentTypeAttr = new Attribute(PKCSObjectIdentifiers.pkcs_9_at_contentType,
            new DERSet(PKCSObjectIdentifiers.data));
        Attribute messageDigestAttr = new Attribute(PKCSObjectIdentifiers.pkcs_9_at_messageDigest,
            new DERSet(new DEROctetString(digest)));
        table.add(signingTimeAttr);
        table.add(contentTypeAttr);
        table.add(messageDigestAttr);
        if (ownerID != null) {
            Attribute ownerIDAttr = new Attribute(new ASN1ObjectIdentifier(SIGNER_OID),
                new DERSet(new DERUTF8String(ownerID)));
            table.add(ownerIDAttr);
        }
        return new DERSet(table);
    }

    private boolean verifySignFromServer(PublicKey publicKey, byte[] signBytes,
        Pair<String, ? extends AlgorithmParameterSpec> signPair, byte[] authed) throws CodeSignException {
        try {
            Signature signature = Signature.getInstance(signPair.getFirst());
            signature.initVerify(publicKey);
            if (signPair.getSecond() != null) {
                signature.setParameter(signPair.getSecond());
            }
            signature.update(authed);
            if (!signature.verify(signBytes)) {
                throw new CodeSignException(CodeSignErrMsg.SIGNATURE_VERIFY_FAILED_ERROR.toString());
            }
            return true;
        } catch (InvalidKeyException | SignatureException e) {
            LOGGER.error("The generated signature could not be verified " + " using the public key in the certificate",
                e);
        } catch (NoSuchAlgorithmException e) {
            LOGGER.error("The generated signature " + signPair.getFirst()
                + " could not be verified using the public key in the certificate", e);
        } catch (InvalidAlgorithmParameterException e) {
            LOGGER.error("The generated signature " + signPair.getSecond()
                + " could not be verified using the public key in the certificate", e);
        }
        return false;
    }

    private ASN1Set createBerSetFromLst(List<?> lists) throws CodeSignException {
        if (lists == null || lists.size() == 0) {
            return null;
        }
        ASN1EncodableVector vector = new ASN1EncodableVector();
        for (Object obj : lists) {
            if (obj instanceof X509CRL) {
                vector.add(getJcaX509CRLHolder((X509CRL) obj).toASN1Structure());
            } else if (obj instanceof X509Certificate) {
                vector.add(getJcaX509CertificateHolder((X509Certificate) obj).toASN1Structure());
            }
        }
        return new BERSet(vector);
    }

    private byte[] encodingUnsignedData(byte[] unsignedDataDigest, SignedData signedData) throws CodeSignException {
        byte[] signResult;
        try {
            ContentInfo contentInfo = new ContentInfo(PKCSObjectIdentifiers.signedData, signedData);
            signResult = contentInfo.getEncoded(ASN1Encoding.DER);
        } catch (IOException e) {
            throw new CodeSignException(CodeSignErrMsg.CODE_SIGN_INTERNAL_ERROR.toString("Encode data error"), e);
        }
        verifySignResult(unsignedDataDigest, signResult);
        return signResult;
    }

    private void verifySignResult(byte[] unsignedDataDigest, byte[] signResult) throws CodeSignException {
        boolean result = false;
        try {
            result = CmsUtils.verifySignDataWithUnsignedDataDigest(unsignedDataDigest, signResult);
        } catch (CMSException e) {
            throw new CodeSignException(
                CodeSignErrMsg.CODE_SIGN_INTERNAL_ERROR.toString("PKCS cms data verify failed"), e);
        }
        if (!result) {
            throw new CodeSignException(
                CodeSignErrMsg.CODE_SIGN_INTERNAL_ERROR.toString("PKCS cms data did not verify"));
        }
    }
}
