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
 * Fail to compute FsVerity digest of a file
 *
 * @since 2023/06/05
 */
public class FsVerityDigestException extends Exception {
    private static final long serialVersionUID = 5788641970791287892L;

    public FsVerityDigestException(String message) {
        super(message);
    }

    public FsVerityDigestException(String message, Throwable cause) {
        super(message, cause);
    }
}
