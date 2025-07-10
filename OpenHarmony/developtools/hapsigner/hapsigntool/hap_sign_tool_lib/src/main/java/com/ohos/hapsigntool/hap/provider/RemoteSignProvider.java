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

package com.ohos.hapsigntool.hap.provider;

import com.ohos.hapsigntool.entity.Options;
import com.ohos.hapsigntool.error.InvalidParamsException;

import java.security.cert.X509Certificate;

/**
 * Remote Sign Provider
 *
 * @since 2021/12/22
 */
public class RemoteSignProvider extends SignProvider {
    @Override
    public void checkParams(Options options) throws InvalidParamsException {
        super.checkParams(options);
        // add remote params check here
    }

    @Override
    protected boolean checkInputCertMatchWithProfile(X509Certificate inputCert, X509Certificate certInProfile) {
        return inputCert != null && inputCert.equals(certInProfile);
    }
}