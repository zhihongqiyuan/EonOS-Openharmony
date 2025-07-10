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
 * CodeSign exception
 *
 * @since 2023/06/05
 */
public class CodeSignException extends Exception {
    private static final long serialVersionUID = -281871003709431259L;

    /**
     * CodeSignException
     *
     * @param message msg
     */
    public CodeSignException(String message) {
        super(message);
    }

    /**
     * CodeSignException
     *
     * @param message msg
     * @param cause   cause
     */
    public CodeSignException(String message, Throwable cause) {
        super(message, cause);
    }
}
