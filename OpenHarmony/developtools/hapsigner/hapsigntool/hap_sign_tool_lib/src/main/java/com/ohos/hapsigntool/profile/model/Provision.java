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
 * Json object of provision profile.
 *
 * @since 2021/12/28
 */
public class Provision {
    /**
     * Field DEBUG.
     */
    public static final String DEBUG = "debug";

    /**
     * Field RELEASE.
     */
    public static final String RELEASE = "release";

    /**
     * Field HOS_SYSTEM_APP.
     */
    public static final String HOS_SYSTEM_APP = "hos_system_app";

    /**
     * Field HOS_NORMAL_APP.
     */
    public static final String HOS_NORMAL_APP = "hos_normal_app";

    /**
     * Field NORMAL.
     */
    public static final String NORMAL = "normal";

    /**
     * Field SYSTEM_BASIC.
     */
    public static final String SYSTEM_BASIC = "system_basic";

    /**
     * Field SYSTEM_CORE.
     */
    public static final String SYSTEM_CORE = "system_core";

    /**
     * Field APP_GALLERY.
     */
    public static final String APP_GALLERY = "app_gallery";

    /**
     * Field ENTERPRISE.
     */
    public static final String ENTERPRISE = "enterprise";

    /**
     * Field OS_INTEGRATION.
     */
    public static final String OS_INTEGRATION = "os_integration";

    /**
     * Number 100.
     */
    public static final int NUM_ONE_HUNDRED = 100;

    /**
     * newline character
     */
    public static final String NEWLINE_CHARACTER = System.lineSeparator();

    /**
     * Field version-code.
     */
    @SerializedName("version-code")
    private Integer versionCode;

    /**
     * Field version-name.
     */
    @SerializedName("version-name")
    private String versionName;

    /**
     * Field uuid.
     */
    @SerializedName("uuid")
    private String uuid;

    /**
     * Field type.
     */
    @SerializedName("type")
    private String type;

    /**
     * Field app-distribution-type.
     */
    @SerializedName("app-distribution-type")
    private String appDistributionType;

    /**
     * Field validity.
     */
    @SerializedName("validity")
    private Validity validity;

    /**
     * Field bundle-info.
     */
    @SerializedName("bundle-info")
    private BundleInfo bundleInfo;

    /**
     * Field acls.
     */
    @SerializedName("acls")
    private Acls acls;

    /**
     * Field permissions.
     */
    @SerializedName("permissions")
    private Permissions permissions;

    /**
     * Field debug-info.
     */
    @SerializedName("debug-info")
    private DebugInfo debuginfo;

    /**
     * Field issuer.
     */
    @SerializedName("issuer")
    private String issuer;

    /**
     * Dto for provision profile.
     */
    public Provision() {}

    /**
     * buildType valid
     *
     * @param buildType buildType
     * @return BuildType Valid
     */
    public static boolean isBuildTypeValid(String buildType) {
        return DEBUG.equals(buildType) || RELEASE.equals(buildType);
    }

    /**
     * buildType valid
     *
     * @param buildType buildType
     * @return  Is the buildType release
     */
    public static boolean isBuildTypeRelease(String buildType) {
        return RELEASE.equals(buildType);
    }

    /**
     * Check if dist type in scope.
     *
     * @param appDistType Input type
     * @return Is type in scope
     */
    public static boolean isAppDistTypeValid(String appDistType) {
        return APP_GALLERY.equals(appDistType)
                || ENTERPRISE.equals(appDistType)
                || OS_INTEGRATION.equals(appDistType);
    }

    /**
     * Enforce valid.
     *
     * @param provision provision
     */
    public static void enforceValid(Provision provision) {
        ValidateUtils.throwIfMatches(provision.type == null || !isBuildTypeValid(provision.type),
                ERROR.SIGN_ERROR, SignToolErrMsg.SIGNATURE_FAILED
                        .toString("Require build type must be debug or release, current is :" + provision.type));

        ValidateUtils.throwIfMatches(provision.bundleInfo == null, ERROR.SIGN_ERROR,
                SignToolErrMsg.SIGNATURE_FAILED.toString("Require bundleInfo in provision!"));
        provision.bundleInfo.enforceValid(provision.type);
    }

    public Integer getVersionCode() {
        return versionCode;
    }

    public void setVersionCode(Integer versionCode) {
        this.versionCode = versionCode;
    }

    public String getVersionName() {
        return versionName;
    }

    public void setVersionName(String versionName) {
        this.versionName = versionName;
    }

    public String getUuid() {
        return uuid;
    }

    public void setUuid(String uuid) {
        this.uuid = uuid;
    }

    public String getType() {
        return type;
    }

    public void setType(String type) {
        this.type = type;
    }

    public String getAppDistributionType() {
        return appDistributionType;
    }

    public void setAppDistributionType(String appDistributionType) {
        this.appDistributionType = appDistributionType;
    }

    public Validity getValidity() {
        return validity;
    }

    public void setValidity(Validity validity) {
        this.validity = validity;
    }

    public BundleInfo getBundleInfo() {
        return bundleInfo;
    }

    public void setBundleInfo(BundleInfo bundleInfo) {
        this.bundleInfo = bundleInfo;
    }

    public Acls getAcls() {
        return acls;
    }

    public void setAcls(Acls acls) {
        this.acls = acls;
    }

    public Permissions getPermissions() {
        return permissions;
    }

    public void setPermissions(Permissions permissions) {
        this.permissions = permissions;
    }

    public DebugInfo getDebuginfo() {
        return debuginfo;
    }

    public void setDebuginfo(DebugInfo debuginfo) {
        this.debuginfo = debuginfo;
    }

    public String getIssuer() {
        return issuer;
    }

    public void setIssuer(String issuer) {
        this.issuer = issuer;
    }

    @Override
    public String toString() {
        return NEWLINE_CHARACTER + "version-code:" + versionCode + NEWLINE_CHARACTER
                + "version-name:" + versionCode + NEWLINE_CHARACTER
                + "uuid:" + uuid + NEWLINE_CHARACTER
                + "type:" + type + NEWLINE_CHARACTER
                + "app-distribution-type:" + appDistributionType + NEWLINE_CHARACTER
                + "validity:" + NEWLINE_CHARACTER
                + "\t not-before:" + getValidity().getNotBefore() + NEWLINE_CHARACTER
                + "\t not-after:" + getValidity().getNotAfter() + NEWLINE_CHARACTER
                + "bundle-info" + NEWLINE_CHARACTER
                + "\t developer-id:" + getBundleInfo().getDeveloperId() + NEWLINE_CHARACTER
                + "\t development-certificate:" + getBundleInfo().getDevelopmentCertificate() + NEWLINE_CHARACTER
                + "\t distribution-certificate:" + getBundleInfo().getDistributionCertificate() + NEWLINE_CHARACTER
                + "\t bundle-name:" + getBundleInfo().getBundleName() + NEWLINE_CHARACTER
                + "\t apl:" + getBundleInfo().getApl() + NEWLINE_CHARACTER
                + "\t app-feature:" + getBundleInfo().getAppFeature() + NEWLINE_CHARACTER
                + "acls:" + NEWLINE_CHARACTER
                + "\t allowed-acls:" + getAcls().getAllowedAcls() + NEWLINE_CHARACTER
                + "permissions:" + NEWLINE_CHARACTER
                + "\t restricted-permissions:" + getPermissions().getRestrictedPermissions() + NEWLINE_CHARACTER
                + "\t restricted-capabilities:" + getPermissions().getRestrictedCapabilities() + NEWLINE_CHARACTER
                + "debug-info" + NEWLINE_CHARACTER
                + "\t device-id-type:" + getDebuginfo().getDeviceIdType() + NEWLINE_CHARACTER
                + "\t device-ids:" + getDebuginfo().getDeviceIds() + NEWLINE_CHARACTER
                + "issuer:" + getIssuer();
    }
}
