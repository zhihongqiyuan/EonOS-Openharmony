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

package com.ohos.hapsigntool.profile;

import com.ohos.hapsigntool.error.VerifyException;
import com.ohos.hapsigntool.profile.model.VerificationResult;

/**
 * IProvisionVerifier.
 *
 * @since 2021/12/28
 */
@FunctionalInterface
public interface IProvisionVerifier {
    /**
     * verify p7b content.
     *
     * @param p7b signed p7b content
     * @return result
     * @throws VerifyException verify p7b failed
     */
    VerificationResult verify(byte[] p7b) throws VerifyException;
}
