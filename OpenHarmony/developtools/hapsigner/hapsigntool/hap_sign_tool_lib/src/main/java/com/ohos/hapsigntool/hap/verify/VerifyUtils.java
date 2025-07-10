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

package com.ohos.hapsigntool.hap.verify;

import org.bouncycastle.cert.X509CertificateHolder;
import org.bouncycastle.cms.CMSException;
import org.bouncycastle.cms.CMSSignedData;
import org.bouncycastle.cms.jcajce.JcaSimpleSignerInfoVerifierBuilder;
import org.bouncycastle.jce.provider.BouncyCastleProvider;
import org.bouncycastle.operator.OperatorCreationException;
import org.bouncycastle.util.Store;

import java.security.Provider;
import java.security.Security;
import java.security.cert.CertificateException;
import java.util.Collection;

/**
 * Utils for verify CMS signed data
 *
 * @since 2021/12/20
 */
public class VerifyUtils {
    static {
        Provider bc = Security.getProvider("BC");
        if (bc == null) {
            Security.addProvider(new BouncyCastleProvider());
        }
    }

    /**
     * Constructor of Method
     */
    private VerifyUtils() {
    }

    /**
     * Verify cms signed data
     *
     * @param cmsSignedData cms signed data
     * @return true, if verify success
     * @throws CMSException if error occurs
     */
    @SuppressWarnings("unchecked")
    public static boolean verifyCmsSignedData(CMSSignedData cmsSignedData) throws CMSException {
        Store<X509CertificateHolder> certs = cmsSignedData.getCertificates();
        return cmsSignedData.verifySignatures(sid -> {
            try {
                Collection<X509CertificateHolder> collection = certs.getMatches(sid);
                if ((collection == null) || (collection.size() != 1)) {
                    throw new OperatorCreationException(
                        "No matched cert or more than one matched certs: " + collection);
                }
                X509CertificateHolder cert = collection.iterator().next();
                return new JcaSimpleSignerInfoVerifierBuilder().setProvider("BC").build(cert);
            } catch (CertificateException e) {
                throw new OperatorCreationException("Failed to verify BC signatures: " + e.getMessage(), e);
            }
        });
    }
}
