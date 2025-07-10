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

package com.ohos.hapsigntool.error;

import java.io.IOException;

/**
 * Zip exception for programs.
 *
 * @since 2023/12/07
 */
public class ZipException extends IOException {
    /**
     * new ZipException
     *
     * @param message exception message
     */
    public ZipException(String message) {
        super(message);
    }

    /**
     * new ZipException
     *
     * @param message exception message
     * @param e exception
     */
    public ZipException(String message, Exception e) {
        super(message, e);
    }
}
