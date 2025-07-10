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

import java.util.List;

/**
 * Sub dto of Provision.
 *
 * @since 2021/12/28
 */
public class DebugInfo {
    /**
     * Max number of debug device.
     */
    private static final int MAX_DEBUG_DEVICE_NUM = 100;

    /**
     * Field device-id-type.
     */
    @SerializedName("device-id-type")
    private String deviceIdType;

    /**
     * Field device-ids.
     */
    @SerializedName("device-ids")
    private List<String> deviceIds;

    /**
     * Sub dto of Provision.
     */
    public DebugInfo() {
        // Empty constructor of DebugInfo.
    }

    public String getDeviceIdType() {
        return deviceIdType;
    }

    public void setDeviceIdType(String deviceIdType) {
        this.deviceIdType = deviceIdType;
    }

    public List<String> getDeviceIds() {
        return deviceIds;
    }

    public void setDeviceIds(List<String> deviceIds) {
        this.deviceIds = deviceIds;
    }

    /**
     * Enforce valid.
     */
    public void enforceValid() {
        if (this.deviceIds != null) {
            ValidateUtils.throwIfMatches(this.deviceIds.size() > MAX_DEBUG_DEVICE_NUM, ERROR.SIGN_ERROR,
                    SignToolErrMsg.SIGNATURE_FAILED.toString("Support at most: 100 devices!"));
            ValidateUtils.throwIfMatches(!this.isDeviceIdTypeValid(), ERROR.SIGN_ERROR, SignToolErrMsg.SIGNATURE_FAILED
                    .toString("Device id type must be sn or udid, current is " + this.deviceIdType));
        }
    }

    private boolean isDeviceIdTypeValid() {
        return "sn".equals(this.deviceIdType) || "udid".equals(this.deviceIdType);
    }
}
