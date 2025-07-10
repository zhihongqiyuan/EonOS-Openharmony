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

/**
 * Sub dto of Provision.
 *
 * @since 2021/12/28
 */
public class Validity {
    /**
     * Field not-before.
     */
    @SerializedName("not-before")
    private Long notBefore;

    /**
     * Field not-after.
     */
    @SerializedName("not-after")
    private Long notAfter;

    /**
     * Sub dto of Provision.
     */
    public Validity() {}

    public Long getNotBefore() {
        return notBefore;
    }

    public void setNotBefore(Long notBefore) {
        this.notBefore = notBefore;
    }

    public Long getNotAfter() {
        return notAfter;
    }

    public void setNotAfter(Long notAfter) {
        this.notAfter = notAfter;
    }
}
