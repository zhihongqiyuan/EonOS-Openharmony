/*
 * Copyright (c) 2023-2023 Huawei Device Co., Ltd.
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

package com.ohos.hapsigntool.codesigning.exception;

/**
 * Exception occurs when the required parameters are missed
 *
 * @since 2023/06/05
 */
public class VerifyCodeSignException extends Exception {
    private static final long serialVersionUID = -8922730964374794468L;

    /**
     * Exception occurs when the required parameters are not entered
     *
     * @param message msg
     */
    public VerifyCodeSignException(String message) {
        super(message);
    }

    /**
     * Exception occurs when the required parameters are not entered
     *
     * @param message msg
     * @param cause   cause
     */
    public VerifyCodeSignException(String message, Throwable cause) {
        super(message, cause);
    }
}

