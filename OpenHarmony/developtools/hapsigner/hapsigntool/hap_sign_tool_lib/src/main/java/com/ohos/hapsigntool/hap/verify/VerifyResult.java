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

import com.ohos.hapsigntool.hap.entity.SigningBlock;

import org.bouncycastle.cert.X509CertificateHolder;
import org.bouncycastle.cms.SignerInformation;
import org.bouncycastle.util.Store;

import java.security.cert.X509CRL;
import java.security.cert.X509Certificate;
import java.util.List;

/**
 * Indicate result of verify hap.
 *
 * @since 2021/12/22
 */
public class VerifyResult {
    /**
     * Return code of verification success.
     */
    public static final int RET_SUCCESS = 10000;

    /**
     * Return code of unknown error.
     */
    public static final int RET_UNKNOWN_ERROR = 10001;

    /**
     * Return code of IO error.
     */
    public static final int RET_IO_ERROR = 10002;

    /**
     * Return code of file format error.
     */
    public static final int RET_UNSUPPORTED_FORMAT_ERROR = 10003;

    /**
     * Return code of file not found error.
     */
    public static final int RET_FILE_NOT_FOUND_ERROR = 10004;

    /**
     * Return code of encoding certificates errors.
     */
    public static final int RET_CERTIFICATE_ENCODING_ERROR = 10005;

    /**
     * Return code of unsupported algorithm error.
     */
    public static final int RET_UNSUPPORTED_ALGORITHM_ERROR = 10006;

    /**
     * Return code of digest error.
     */
    public static final int RET_DIGEST_ERROR = 10007;

    /**
     * Return code of signatures not found error.
     */
    public static final int RET_SIGNATURE_NOT_FOUND_ERROR = 10008;

    /**
     * Return code of signatures verify error.
     */
    public static final int RET_SIGNATURE_ERROR = 10009;

    /**
     * Return code of certificate revoked error.
     */
    public static final int RET_CERTIFICATE_REVOKED = 10010;

    /**
     * Return code of certificate revoked error.
     */
    public static final int RET_CRL_ERROR = 10011;

    /**
     * Return code of file code sign data error.
     */
    public static final int RET_CODESIGN_DATA_ERROR = 10012;

    /**
     * Return code of verify code sign error.
     */
    public static final int RET_CODE_SIGN_BLOCK_ERROR = 10013;

    private boolean isResult;
    private int code;
    private String message;

    private List<X509Certificate> certificates;

    private List<X509CRL> crls;

    private List<SignerInformation> signerInfos;

    private List<SigningBlock> optionalBlocks;

    private Store<X509CertificateHolder> certificateHolderStore;

    private int signBlockVersion;

    private byte[] profile;

    /**
     * Empty constructor
     */
    public VerifyResult() {
    }

    /**
     * Verify result constructor
     *
     * @param isResult verify result
     * @param code error code
     * @param message error message
     */
    public VerifyResult(boolean isResult, int code, String message) {
        this.isResult = isResult;
        this.code = code;
        this.message = message;
    }

    public boolean isVerified() {
        return isResult;
    }

    public void setIsResult(boolean isResult) {
        this.isResult = isResult;
    }

    public int getCode() {
        return code;
    }

    public void setCode(int code) {
        this.code = code;
    }

    public String getMessage() {
        return message;
    }

    public void setMessage(String message) {
        this.message = message;
    }

    public List<X509Certificate> getCertificates() {
        return certificates;
    }

    public void setCertificates(List<X509Certificate> certificates) {
        this.certificates = certificates;
    }

    public List<X509CRL> getCrls() {
        return crls;
    }

    public void setCrls(List<X509CRL> crls) {
        this.crls = crls;
    }

    public List<SignerInformation> getSignerInfos() {
        return signerInfos;
    }

    public void setSignerInfos(List<SignerInformation> signerInfos) {
        this.signerInfos = signerInfos;
    }

    public List<SigningBlock> getOptionalBlocks() {
        return optionalBlocks;
    }

    public void setOptionalBlocks(List<SigningBlock> optionalBlocks) {
        this.optionalBlocks = optionalBlocks;
    }

    public Store<X509CertificateHolder> getCertificateHolderStore() {
        return certificateHolderStore;
    }

    public void setCertificateHolderStore(Store<X509CertificateHolder> certificateHolderStore) {
        this.certificateHolderStore = certificateHolderStore;
    }

    public int getSignBlockVersion() {
        return signBlockVersion;
    }

    public void setSignBlockVersion(int signBlockVersion) {
        this.signBlockVersion = signBlockVersion;
    }

    public byte[] getProfile() {
        return profile;
    }

    public void setProfile(byte[] profile) {
        this.profile = profile;
    }
}
