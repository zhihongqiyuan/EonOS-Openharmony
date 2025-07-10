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

package com.ohos.hapsigntool.utils;

import com.ohos.hapsigntool.error.CustomException;
import com.ohos.hapsigntool.error.ERROR;

/**
 * ValidateUtils.
 *
 * @since 2021/12/28
 */
public final class ValidateUtils {
    private ValidateUtils() {
    }

    /**
     * Throw exception if not true.
     *
     * @param isMatch Want a true value
     * @param error     Error enum to throw
     * @param errorMsg  Error msg to throw
     * @throws CustomException CustomException
     */
    public static void throwIfNotMatches(boolean isMatch, ERROR error, String errorMsg) throws CustomException {
        if (!isMatch) {
            CustomException.throwException(error, errorMsg);
        }
    }

    /**
     * Throw exception if true.
     *
     * @param isMatch Want a false value
     * @param error     Error enum to throw
     * @param errorMsg  Error msg to throw
     * @throws CustomException CustomException
     */
    public static void throwIfMatches(boolean isMatch, ERROR error, String errorMsg) throws CustomException {
        throwIfNotMatches(!isMatch, error, errorMsg);
    }
}
