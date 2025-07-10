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

import com.google.gson.JsonObject;
import com.ohos.hapsigntool.error.CustomException;
import com.ohos.hapsigntool.error.ERROR;
import com.ohos.hapsigntool.error.SignToolErrMsg;
import com.ohos.hapsigntool.error.VerifyException;
import com.ohos.hapsigntool.hap.verify.VerifyUtils;
import com.ohos.hapsigntool.profile.model.VerificationResult;
import com.ohos.hapsigntool.utils.CertChainUtils;
import com.ohos.hapsigntool.utils.CertUtils;
import com.ohos.hapsigntool.utils.FileUtils;
import com.ohos.hapsigntool.utils.LogUtils;
import com.ohos.hapsigntool.utils.ValidateUtils;

import org.bouncycastle.asn1.ASN1Encodable;
import org.bouncycastle.asn1.ASN1Set;
import org.bouncycastle.asn1.cms.Attribute;
import org.bouncycastle.asn1.pkcs.PKCSObjectIdentifiers;
import org.bouncycastle.asn1.x509.Time;
import org.bouncycastle.cert.X509CertificateHolder;
import org.bouncycastle.cert.jcajce.JcaX509CertificateConverter;
import org.bouncycastle.cms.CMSException;
import org.bouncycastle.cms.CMSSignedData;
import org.bouncycastle.cms.SignerId;
import org.bouncycastle.cms.SignerInformation;
import org.bouncycastle.cms.SignerInformationStore;
import org.bouncycastle.util.Store;

import javax.security.auth.x500.X500Principal;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;
import java.security.Signature;
import java.security.SignatureException;
import java.security.cert.CertificateException;
import java.security.cert.X509Certificate;
import java.time.LocalDateTime;
import java.time.ZoneId;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Iterator;
import java.util.List;
import java.util.Date;

/**
 * Signed provision profile verifier.
 *
 * @since 2021/12/28
 */
public class VerifyHelper implements IProvisionVerifier {
    /**
     * LOGGER.
     */
    private static final LogUtils LOGGER = new LogUtils(VerifyHelper.class);

    /**
     * Signed provision profile verifier.
     */
    public VerifyHelper() {}

    /**
     * Checked signed data with public key.
     *
     * @param cert         public key
     * @param signedData   signed data with private key
     * @param unsignedData unsigned data
     * @param algorithm    algorithm
     */
    public static void verifySignature(X509Certificate cert, byte[] signedData, byte[] unsignedData, String algorithm) {
        try {
            Signature signature = Signature.getInstance(algorithm);
            signature.initVerify(cert);
            signature.update(unsignedData);
            ValidateUtils.throwIfNotMatches(signature.verify(signedData), ERROR.SIGN_ERROR,
                    SignToolErrMsg.SIGNATURE_NOT_MATCHED.toString());
        } catch (InvalidKeyException | SignatureException | NoSuchAlgorithmException exception) {
            LOGGER.debug(exception.getMessage(), exception);
            CustomException.throwException(ERROR.SIGN_ERROR, SignToolErrMsg.VERIFY_FAILED
                    .toString(exception.getMessage()));
        }
    }

    /**
     * Convert store collection to list.
     *
     * @param certificates certificates from cmsSignedData
     * @return List<X509Certificate>
     */
    public static List<X509Certificate> certStoreToCertList(Store<X509CertificateHolder> certificates) {
        String errorMsg = "Verify failed, not found cert chain";
        JcaX509CertificateConverter converter = new JcaX509CertificateConverter();
        ValidateUtils.throwIfMatches(certificates == null, ERROR.VERIFY_ERROR, SignToolErrMsg.VERIFY_FAILED
                .toString(errorMsg));
        Collection<X509CertificateHolder> matches = certificates.getMatches(null);
        ValidateUtils.throwIfMatches(matches == null || !matches.iterator().hasNext(),
                ERROR.VERIFY_ERROR, SignToolErrMsg.VERIFY_FAILED.toString(errorMsg));

        Iterator<X509CertificateHolder> iterator = matches.iterator();
        List<X509Certificate> certificateList = new ArrayList<>();
        try {
            while (iterator.hasNext()) {
                X509CertificateHolder next = iterator.next();
                certificateList.add(converter.getCertificate(next));
            }
        } catch (CertificateException exception) {
            LOGGER.debug(exception.getMessage(), exception);
            CustomException.throwException(ERROR.VERIFY_ERROR, SignToolErrMsg.VERIFY_FAILED.toString(errorMsg));
        }
        ValidateUtils.throwIfMatches(certificateList.size() == 0,
                ERROR.VERIFY_ERROR, SignToolErrMsg.VERIFY_FAILED.toString(errorMsg));
        return certificateList;
    }

    /**
     * verify p7b content.
     *
     * @param p7b signed p7b content
     * @return result
     * @throws VerifyException verify p7b failed
     */
    @Override
    public VerificationResult verify(byte[] p7b) throws VerifyException {
        VerificationResult result = new VerificationResult();

        try {
            CMSSignedData cmsSignedData = this.verifyPkcs(p7b);
            List<X509Certificate> certificates = certStoreToCertList(cmsSignedData.getCertificates());
            CertUtils.sortCertificateChain(certificates);

            SignerInformationStore signerInfos = cmsSignedData.getSignerInfos();
            Collection<SignerInformation> signers = signerInfos.getSigners();

            for (SignerInformation signer : signers) {
                SignerId sid = signer.getSID();
                Date signTime = getSignTime(signer);

                X500Principal principal = new X500Principal(sid.getIssuer().getEncoded());
                CertChainUtils.verifyCertChain(certificates, principal, sid.getSerialNumber(),
                        certificates.get(certificates.size() - 1), signTime);
            }

            result.setContent(FileUtils.GSON.fromJson(new String((byte[]) (cmsSignedData
                    .getSignedContent().getContent()), StandardCharsets.UTF_8), JsonObject.class));
            result.setMessage("OK");
            result.setVerifiedPassed(true);
            return result;
        } catch (CustomException | IOException exception) {
            LOGGER.debug(exception.getMessage(), exception);
            result.setMessage(exception.getMessage());
            result.setVerifiedPassed(false);
            return result;
        }
    }

    Date getSignTime(SignerInformation signer) {
        Date signTime = Date.from(LocalDateTime.now().atZone(ZoneId.systemDefault()).toInstant());

        Attribute attribute = signer.getSignedAttributes().get(PKCSObjectIdentifiers.pkcs_9_at_signingTime);

        if (attribute == null) {
            LOGGER.warn("sign information does not include signTime");
            return signTime;
        }

        ASN1Set attrValues = attribute.getAttrValues();
        if (attrValues.size() == 0) {
            LOGGER.warn("get sign time false, use local datetime verify profile cert chain");
            return signTime;
        }

        ASN1Encodable objectAt = attrValues.getObjectAt(0);
        signTime = Time.getInstance(objectAt).getDate();

        return signTime;
    }

    CMSSignedData verifyPkcs(byte[] p7b) {
        CMSSignedData cmsSignedData = null;
        try {
            cmsSignedData = new CMSSignedData(p7b);
            boolean verifyResult = VerifyUtils.verifyCmsSignedData(cmsSignedData);
            ValidateUtils.throwIfNotMatches(verifyResult, ERROR.VERIFY_ERROR,
                    SignToolErrMsg.VERIFY_FAILED.toString("Failed to verify BC signatures"));
            return cmsSignedData;
        } catch (CMSException exception) {
            LOGGER.debug(exception.getMessage(), exception);
            CustomException.throwException(ERROR.VERIFY_ERROR, SignToolErrMsg.VERIFY_FAILED
                    .toString("Failed to verify BC signatures" + exception.getMessage()));
        }
        return cmsSignedData;
    }
}
