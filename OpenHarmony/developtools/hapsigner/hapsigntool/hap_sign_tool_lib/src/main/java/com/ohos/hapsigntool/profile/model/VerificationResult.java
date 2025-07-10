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

import com.google.gson.JsonObject;

/**
 * VerificationResult.
 *
 * @since 2021/12/28
 */
public class VerificationResult {
    /**
     * Field verifiedPassed.
     */
    private boolean verifiedPassed;

    /**
     * Field message.
     */
    private String message;

    /**
     * Field content.
     */
    private JsonObject content;

    public boolean isVerifiedPassed() {
        return verifiedPassed;
    }

    public void setVerifiedPassed(boolean verifiedPassed) {
        this.verifiedPassed = verifiedPassed;
    }

    public String getMessage() {
        return this.message;
    }

    public void setMessage(String string) {
        this.message = string;
    }

    public JsonObject getContent() {
        return content;
    }

    public void setContent(JsonObject provision) {
        this.content = provision;
    }
}
