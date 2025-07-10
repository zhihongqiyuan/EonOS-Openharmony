/*
 * Copyright (c) 2022-2023 Huawei Device Co., Ltd.
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

package com.ohos.hapsigntool.utils;

import com.ohos.hapsigntool.error.VerifyCertificateChainException;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;
import java.security.NoSuchProviderException;
import java.security.SignatureException;
import java.security.cert.CertificateException;
import java.security.cert.CertificateExpiredException;
import java.security.cert.CertificateFactory;
import java.security.cert.X509Certificate;
import java.security.cert.CertificateNotYetValidException;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.List;

/**
 * Utils of certificate processing.
 *
 * @since 2021/12/28
 */
public class CertificateUtils {
    /**
     * Constructor of Method
     */
    private CertificateUtils() {
    }

    /**
     * Check cert list is cert chain.
     *
     * @param certs certs
     * @throws VerifyCertificateChainException certificates in file are not certificate chain
     */
    public static void verifyCertChain(List<X509Certificate> certs) throws VerifyCertificateChainException {
        if (certs.size() <= 1) {
            return;
        }
        for (int i = 1; i < certs.size(); i++) {
            try {
                certs.get(i - 1).verify(certs.get(i).getPublicKey());
                certs.get(i - 1).checkValidity();
                if (!certs.get(i - 1).getIssuerDN().equals(certs.get(i).getSubjectDN())) {
                    throw new CertificateException("verify certificate chain issuer subject failed") ;
                }
                if (i == certs.size() - 1) {
                    certs.get(i).checkValidity();
                }
            } catch (CertificateExpiredException | CertificateNotYetValidException e) {
                throw new VerifyCertificateChainException("The certificate has expired! " + e.getMessage());
            } catch (CertificateException | NoSuchAlgorithmException | InvalidKeyException
                | NoSuchProviderException | SignatureException e) {
                throw new VerifyCertificateChainException("verify certificate chain failed! " + e.getMessage());
            }
        }
    }

    /**
     * Get certificates chains from input file.
     *
     * @param certsFile input file
     * @return list of certificate chain
     * @throws IOException file is not exist
     * @throws CertificateException data in file is not certificate
     * @throws VerifyCertificateChainException certificates in file are not certificate chain
     */
    @SuppressWarnings("unchecked")
    public static List<X509Certificate> getCertListFromFile(String certsFile) throws IOException, CertificateException,
        VerifyCertificateChainException {
        try (FileInputStream fileInputStream = FileUtils.openInputStream(new File(certsFile))) {
            CertificateFactory cf = CertificateFactory.getInstance("X.509");
            Collection<X509Certificate> certificates =
                (Collection<X509Certificate>) cf.generateCertificates(fileInputStream);
            if (certificates != null && !certificates.isEmpty()) {
                List<X509Certificate> certs = new ArrayList<X509Certificate>(certificates);
                CertUtils.sortCertificateChain(certs);
                verifyCertChain(certs);
                return certs;
            }
        }
        return Collections.emptyList();
    }
}
