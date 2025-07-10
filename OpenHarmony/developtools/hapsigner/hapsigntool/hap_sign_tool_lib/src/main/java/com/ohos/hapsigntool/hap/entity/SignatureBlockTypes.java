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

package com.ohos.hapsigntool.hap.entity;

/**
 * Define class of hap signature block types
 *
 * @since 2024/03/16
 */
public class SignatureBlockTypes {
    /**
     * type-value of hap signature block
     */
    public static final char SIGNATURE_BLOCK = 0;

    /**
     * type-value of unsigned profile
     */
    public static final char PROFILE_NOSIGNED_BLOCK = 1;

    /**
     * type-value of signed profile
     */
    public static final char PROFILE_SIGNED_BLOCK = 2;

    /**
     * type-value of key rotation block
     */
    public static final char KEY_ROTATION_BLOCK = 3;

    /**
     * Constructor of Method
     */
    private SignatureBlockTypes() {
    }

    /**
     * get profile block type by the flag of isSigned
     *
     * @param isSigned the flag whether profile is signed
     * @return profile block type value
     */
    public static char getProfileBlockTypes(String isSigned) {
        if ("0".equals(isSigned)) {
            return PROFILE_NOSIGNED_BLOCK;
        }
        if ("1".equals(isSigned)) {
            return PROFILE_SIGNED_BLOCK;
        }
        return PROFILE_NOSIGNED_BLOCK;
    }
}