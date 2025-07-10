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

package com.ohos.hapsigntool.profile.model;

import com.google.gson.annotations.SerializedName;
import com.ohos.hapsigntool.error.ERROR;
import com.ohos.hapsigntool.error.SignToolErrMsg;
import com.ohos.hapsigntool.utils.ValidateUtils;

/**
 * Sub dto of Provision.
 *
 * @since 2021/12/28
 */
public class BundleInfo {
    /**
     * Field developer-id.
     */
    @SerializedName("developer-id")
    private String developerId;

    /**
     * Field development-certificate.
     */
    @SerializedName("development-certificate")
    private String developmentCertificate;

    /**
     * Field distribution-certificate.
     */
    @SerializedName("distribution-certificate")
    private String distributionCertificate;

    /**
     * Field bundle-name.
     */
    @SerializedName("bundle-name")
    private String bundleName;

    /**
     * Field apl.
     */
    @SerializedName("apl")
    private String apl;

    /**
     * Field app-feature.
     */
    @SerializedName("app-feature")
    private String appFeature;

    /**
     * Sub dto of Provision.
     */
    public BundleInfo() {}

    public String getDeveloperId() {
        return developerId;
    }

    public void setDeveloperId(String developerId) {
        this.developerId = developerId;
    }

    public String getDevelopmentCertificate() {
        return developmentCertificate;
    }

    public void setDevelopmentCertificate(String developmentCertificate) {
        this.developmentCertificate = developmentCertificate;
    }

    public String getDistributionCertificate() {
        return distributionCertificate;
    }

    public void setDistributionCertificate(String distributionCertificate) {
        this.distributionCertificate = distributionCertificate;
    }

    public String getBundleName() {
        return bundleName;
    }

    public void setBundleName(String bundleName) {
        this.bundleName = bundleName;
    }

    public String getApl() {
        return apl;
    }

    public void setApl(String apl) {
        this.apl = apl;
    }

    public String getAppFeature() {
        return appFeature;
    }

    public void setAppFeature(String appFeature) {
        this.appFeature = appFeature;
    }

    /**
     * Enforce valid.
     *
     * @param buildType build type
     */
    public void enforceValid(String buildType) {
        if (Provision.isBuildTypeRelease(buildType)) {
            ValidateUtils.throwIfMatches(this.distributionCertificate == null,
                    ERROR.SIGN_ERROR, SignToolErrMsg.SIGNATURE_FAILED.toString("Require cert in bundleInfo!"));
        } else {
            ValidateUtils.throwIfMatches(this.developmentCertificate == null,
                    ERROR.SIGN_ERROR, SignToolErrMsg.SIGNATURE_FAILED.toString("Require cert in bundleInfo!"));
        }
    }
}
