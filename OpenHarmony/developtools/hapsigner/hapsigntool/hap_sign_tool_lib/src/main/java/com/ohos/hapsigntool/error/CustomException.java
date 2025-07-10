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

package com.ohos.hapsigntool.error;

/**
 * Runtime exception for programs.
 *
 * @since 2021/12/28
 */
public class CustomException extends RuntimeException {
    private ERROR error;

    private String message;

    /**
     * Create custom exception with params.
     *
     * @param error   Error enum to throw
     * @param message Error msg to throw
     */
    CustomException(ERROR error, String message) {
        super(message);
        this.error = error;
        this.message = message;
    }

    /**
     * Throw custom exception with params.
     *
     * @param error   Error enum to throw
     * @param message Error msg to throw
     */
    public static void throwException(ERROR error, String message) {
        throw new CustomException(error, message);
    }

    public ERROR getError() {
        return error;
    }

    public void setError(ERROR error) {
        this.error = error;
    }

    @Override
    public String getMessage() {
        return message;
    }

    public void setMessage(String message) {
        this.message = message;
    }
}
