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
 * Exception that occurs if the Hap file format is incorrect.
 *
 * @since 2021/12/20
 */
public class HapFormatException extends SignatureException {
    private static final long serialVersionUID = -8095203467304334741L;

    private static final int HAP_FORMAT_ERROR = 20001;

    public HapFormatException(String message) {
        super(HAP_FORMAT_ERROR, message);
    }

    public HapFormatException(String message, Throwable cause) {
        super(HAP_FORMAT_ERROR, message, cause);
    }

    public HapFormatException(int errorCode, String message) {
        super(errorCode, message);
    }

    public HapFormatException(int errorCode, String message, Throwable cause) {
        super(errorCode, message, cause);
    }
}
