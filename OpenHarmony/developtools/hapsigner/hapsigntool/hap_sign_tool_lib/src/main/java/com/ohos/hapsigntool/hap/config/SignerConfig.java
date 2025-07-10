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

package com.ohos.hapsigntool.hap.config;

import com.ohos.hapsigntool.adapter.LocalizationAdapter;
import com.ohos.hapsigntool.entity.Options;
import com.ohos.hapsigntool.entity.SignatureAlgorithm;
import com.ohos.hapsigntool.signer.ISigner;
import com.ohos.hapsigntool.signer.SignerFactory;

import java.security.cert.X509CRL;
import java.security.cert.X509Certificate;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Sign config super class
 *
 * @since 2021-12-13
 */
public class SignerConfig {
    /**
     * params inputted by users
     */
    private Options options;

    /**
     * certificate chain used for sign hap
     */
    private List<X509Certificate> certificates;

    /**
     * certificate revocation list return from server
     */
    private List<X509CRL> x509CRLs;

    /**
     * Signature Algorithms used for sign hap
     */
    private List<SignatureAlgorithm> signatureAlgorithms;

    /**
     * parameters for sign hap
     */
    private Map<String, String> signParamMap = new HashMap<String, String>();

    /**
     * Signer used for sign hap
     */
    private ISigner signer;

    /**
     * Minimum api version to run the application
     */
    private int compatibleVersion;

    /**
     * Get options.
     *
     * @return options
     */
    public Options getOptions() {
        return options;
    }

    /**
     * set options.
     *
     * @param options parameters
     */
    public void setOptions(Options options) {
        this.options = options;
    }

    /**
     * Get certificates.
     *
     * @return certificates
     */
    public List<X509Certificate> getCertificates() {
        if (isInputCertChainNotEmpty() || signer == null) {
            return certificates;
        }
        return signer.getCertificates();
    }

    /**
     * set certificate
     *
     * @param certificates certificate chain
     */
    public void setCertificates(List<X509Certificate> certificates) {
        this.certificates = certificates;
    }

    /**
     * get crl
     *
     * @return crl list
     */
    public List<X509CRL> getX509CRLs() {
        if (isInputCertChainNotEmpty() || isInputCrlNotEmpty() || signer == null) {
            return x509CRLs;
        }
        return signer.getCrls();
    }

    /**
     * set crl
     *
     * @param crls cert revocation list
     */
    public void setX509CRLs(List<X509CRL> crls) {
        this.x509CRLs = crls;
    }

    /**
     * get signature algorithm
     *
     * @return signature algorithm
     */
    public List<SignatureAlgorithm> getSignatureAlgorithms() {
        return signatureAlgorithms;
    }

    /**
     * set signature algorithm
     *
     * @param signatureAlgorithms sign algorithm
     */
    public void setSignatureAlgorithms(List<SignatureAlgorithm> signatureAlgorithms) {
        this.signatureAlgorithms = signatureAlgorithms;
    }

    /**
     * get param map
     *
     * @return param map
     */
    public Map<String, String> getSignParamMap() {
        return this.signParamMap;
    }

    /**
     * set param map
     *
     * @param params params map
     */
    public void setParameters(Map<String, String> params) {
        this.signParamMap = params;
    }

    /**
     * get signer
     *
     * @return content signer
     */
    public ISigner getSigner() {
        if (signer == null) {
            signer = new SignerFactory().getSigner(new LocalizationAdapter(options));
        }
        return signer;
    }

    /**
     * get compatible version
     *
     * @return compatible version
     */
    public int getCompatibleVersion() {
        return compatibleVersion;
    }

    /**
     * set param compatible version
     *
     * @param compatibleVersion compatible version
     */
    public void setCompatibleVersion(int compatibleVersion) {
        this.compatibleVersion = compatibleVersion;
    }

    private boolean isInputCertChainNotEmpty() {
        return certificates != null && !certificates.isEmpty();
    }

    private boolean isInputCrlNotEmpty() {
        return x509CRLs != null && !x509CRLs.isEmpty();
    }

    /**
     * create a copy of this object
     *
     * @return a copy of this object
     */
    public SignerConfig copy() {
        SignerConfig signerConfig = new SignerConfig();
        signerConfig.setParameters(signParamMap);
        signerConfig.setCertificates(certificates);
        signerConfig.setOptions(options);
        signerConfig.setSignatureAlgorithms(signatureAlgorithms);
        signerConfig.setCompatibleVersion(compatibleVersion);
        signerConfig.setX509CRLs(x509CRLs);
        signerConfig.getSigner();
        return signerConfig;
    }
}
