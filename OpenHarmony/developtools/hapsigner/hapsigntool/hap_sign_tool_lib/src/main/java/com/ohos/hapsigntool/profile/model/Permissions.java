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

import java.util.List;

/**
 * Sub dto of Provision.
 *
 * @since 2021/12/28
 */
public class Permissions {
    /**
     * Field restricted-permissions.
     */
    @SerializedName("restricted-permissions")
    private List<String> restrictedPermissions;

    /**
     * Field restricted-capabilities.
     */
    @SerializedName("restricted-capabilities")
    private List<String> restrictedCapabilities;

    /**
     * Sub dto of Provision.
     */
    public Permissions() {}

    public List<String> getRestrictedPermissions() {
        return restrictedPermissions;
    }

    public void setRestrictedPermissions(List<String> restrictedPermissions) {
        this.restrictedPermissions = restrictedPermissions;
    }

    public List<String> getRestrictedCapabilities() {
        return restrictedCapabilities;
    }

    public void setRestrictedCapabilities(List<String> restrictedCapabilities) {
        this.restrictedCapabilities = restrictedCapabilities;
    }
}

