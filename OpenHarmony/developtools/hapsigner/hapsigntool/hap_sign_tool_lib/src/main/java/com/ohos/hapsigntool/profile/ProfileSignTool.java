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

package com.ohos.hapsigntool.profile;

import com.ohos.hapsigntool.adapter.LocalizationAdapter;
import com.ohos.hapsigntool.error.CustomException;
import com.ohos.hapsigntool.error.ERROR;
import com.ohos.hapsigntool.error.SignToolErrMsg;
import com.ohos.hapsigntool.error.VerifyException;
import com.ohos.hapsigntool.profile.model.VerificationResult;
import com.ohos.hapsigntool.signer.ISigner;
import com.ohos.hapsigntool.signer.SignerFactory;
import com.ohos.hapsigntool.utils.LogUtils;
import com.ohos.hapsigntool.utils.ValidateUtils;

import org.bouncycastle.asn1.ASN1EncodableVector;
import org.bouncycastle.asn1.ASN1Set;
import org.bouncycastle.asn1.BERSet;
import org.bouncycastle.asn1.DEROctetString;
import org.bouncycastle.asn1.DERSet;
import org.bouncycastle.asn1.cms.Attribute;
import org.bouncycastle.asn1.cms.CMSObjectIdentifiers;
import org.bouncycastle.asn1.cms.ContentInfo;
import org.bouncycastle.asn1.cms.IssuerAndSerialNumber;
import org.bouncycastle.asn1.cms.SignedData;
import org.bouncycastle.asn1.cms.SignerIdentifier;
import org.bouncycastle.asn1.cms.SignerInfo;
import org.bouncycastle.asn1.cms.Time;
import org.bouncycastle.asn1.pkcs.PKCSObjectIdentifiers;
import org.bouncycastle.asn1.x509.AlgorithmIdentifier;
import org.bouncycastle.cert.jcajce.JcaX509CRLHolder;
import org.bouncycastle.cert.jcajce.JcaX509CertificateHolder;
import org.bouncycastle.operator.DefaultDigestAlgorithmIdentifierFinder;
import org.bouncycastle.operator.DefaultSignatureAlgorithmIdentifierFinder;
import org.bouncycastle.operator.DigestCalculator;
import org.bouncycastle.operator.DigestCalculatorProvider;
import org.bouncycastle.operator.OperatorCreationException;
import org.bouncycastle.operator.jcajce.JcaDigestCalculatorProviderBuilder;

import java.io.IOException;
import java.security.cert.CRLException;
import java.security.cert.CertificateEncodingException;
import java.security.cert.X509CRL;
import java.security.cert.X509Certificate;
import java.time.LocalDateTime;
import java.time.ZoneId;
import java.util.Date;
import java.util.List;

/**
 * To sign and verify profile.
 *
 * @since 2021/12/28
 */
public final class ProfileSignTool {
    /**
     * Empty byte array.
     */
    private static final byte[] NO_BYTE = {};

    /**
     * logger
     */
    private static final LogUtils LOGGER = new LogUtils(ProfileSignTool.class);

    private ProfileSignTool() {
    }

    /**
     * generateP7b.
     *
     * @param adapter local adapter with params
     * @param content content to sign
     * @return signed content
     */
    public static byte[] generateP7b(LocalizationAdapter adapter, byte[] content) {
        ISigner signer = new SignerFactory().getSigner(adapter);
        byte[] p7b = signProfile(content, signer, adapter.getSignAlg());
        VerifyHelper verifyHelper = new VerifyHelper();
        VerificationResult verificationResult = null;
        try {
            verificationResult = verifyHelper.verify(p7b);
        } catch (VerifyException e) {
            CustomException.throwException(ERROR.VERIFY_ERROR, SignToolErrMsg.VERIFY_PROFILE_FAILED
                    .toString("Generate Profile Failed! " + e.getMessage()));
        }
        ValidateUtils.throwIfNotMatches(verificationResult.isVerifiedPassed(),
                ERROR.SIGN_ERROR, SignToolErrMsg.SIGNATURE_FAILED.toString(verificationResult.getMessage()));
        return p7b;
    }

    /**
     * signProfile.
     *
     * @param content content to sign
     * @param signer signer
     * @param sigAlg sign algorithm
     * @return signed data
     */
    public static byte[] signProfile(byte[] content, ISigner signer, String sigAlg) {
        try {
            AlgorithmIdentifier sigAlgId = (new DefaultSignatureAlgorithmIdentifierFinder()).find(sigAlg);
            ASN1EncodableVector digestAlgIds = new ASN1EncodableVector();
            AlgorithmIdentifier digestAlgId = (new DefaultDigestAlgorithmIdentifierFinder()).find(sigAlgId);
            digestAlgIds.add(digestAlgId);
            byte[] digest = getContentDigest(content, digestAlgId);
            ASN1Set signedAttr = generatePKCS9Attributes(digest);
            byte[] signature = signer.getSignature(signedAttr.getEncoded("DER"), sigAlg, null);
            // To validate cert(public key) and private key
            VerifyHelper.verifySignature(signer.getCertificates().get(0), signature,
                    signedAttr.getEncoded("DER"), sigAlg);
            SignerIdentifier signerIdentifier = generateSignerIdentifier(signer.getCertificates().get(0));
            SignerInfo signerInfo = new SignerInfo(signerIdentifier, digestAlgId, signedAttr, sigAlgId,
                    new DEROctetString(signature), null);
            ASN1EncodableVector signerInfos = new ASN1EncodableVector();
            signerInfos.add(signerInfo);
            ASN1Set certList = createBerSetFromCerts(signer.getCertificates());
            List<X509CRL> crls = signer.getCrls();
            ASN1Set crlList = createBerSetFromCrls(crls);
            ContentInfo encryptInfo = new ContentInfo(CMSObjectIdentifiers.data, new DEROctetString(content));
            SignedData sd = new SignedData(new DERSet(digestAlgIds), encryptInfo, certList, crlList,
                    new DERSet(signerInfos));
            ContentInfo contentInfo = new ContentInfo(CMSObjectIdentifiers.signedData, sd);
            return contentInfo.getEncoded("DER");
        } catch (OperatorCreationException | IOException | CertificateEncodingException | CRLException e) {
            LOGGER.debug(e.getMessage(), e);
            CustomException.throwException(ERROR.SIGN_ERROR, SignToolErrMsg.SIGNATURE_FAILED.toString(e.getMessage()));
        }
        return NO_BYTE;
    }

    private static SignerIdentifier generateSignerIdentifier(X509Certificate certificate)
            throws CertificateEncodingException {
        return new SignerIdentifier(new IssuerAndSerialNumber(
                (new JcaX509CertificateHolder(certificate)).toASN1Structure()));
    }

    private static ASN1Set generatePKCS9Attributes(byte[] digest) {
        ASN1EncodableVector vector = new ASN1EncodableVector();
        Attribute signTime = new Attribute(PKCSObjectIdentifiers.pkcs_9_at_signingTime,
                new DERSet(new Time(Date.from(LocalDateTime.now().atZone(ZoneId.systemDefault()).toInstant()))));
        Attribute contentType = new Attribute(PKCSObjectIdentifiers.pkcs_9_at_contentType,
                new DERSet(PKCSObjectIdentifiers.data));
        Attribute digestAtt = new Attribute(PKCSObjectIdentifiers.pkcs_9_at_messageDigest,
                new DERSet(new DEROctetString(digest)));
        vector.add(signTime);
        vector.add(contentType);
        vector.add(digestAtt);
        return new DERSet(vector);
    }

    private static byte[] getContentDigest(byte[] content, AlgorithmIdentifier digestAlgorithmIdentifier)
            throws OperatorCreationException, IOException {
        DigestCalculatorProvider digestCalculatorProvider = (new JcaDigestCalculatorProviderBuilder()).build();
        DigestCalculator digestCalculator = digestCalculatorProvider.get(digestAlgorithmIdentifier);
        digestCalculator.getOutputStream().write(content);
        return digestCalculator.getDigest();
    }

    private static ASN1Set createBerSetFromCrls(List<X509CRL> crls) throws CRLException {
        if (crls != null && crls.size() != 0) {
            ASN1EncodableVector vector = new ASN1EncodableVector(crls.size());
            for (X509CRL crl : crls) {
                vector.add((new JcaX509CRLHolder(crl)).toASN1Structure());
            }
            return new BERSet(vector);
        } else {
            return null;
        }
    }

    private static ASN1Set createBerSetFromCerts(List<X509Certificate> certs) throws CertificateEncodingException {
        if (certs != null && certs.size() != 0) {
            ASN1EncodableVector vector = new ASN1EncodableVector(certs.size());
            for (X509Certificate cert : certs) {
                vector.add((new JcaX509CertificateHolder(cert)).toASN1Structure());
            }
            return new BERSet(vector);
        } else {
            return null;
        }
    }
}
