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

package com.ohos.hapsigntool.codesigning.fsverity;

/**
 * FsVerity hash algorithm
 *
 * @since 2023/06/05
 */
public enum FsVerityHashAlgorithm {
    SHA256((byte) 1, "SHA-256", 256 / 8),
    SHA512((byte) 2, "SHA-512", 512 / 8);

    private final byte id;

    private final String hashAlgorithm;

    private final int outputByteSize;

    FsVerityHashAlgorithm(byte id, String hashAlgorithm, int outputByteSize) {
        this.id = id;
        this.hashAlgorithm = hashAlgorithm;
        this.outputByteSize = outputByteSize;
    }

    public byte getId() {
        return id;
    }

    public String getHashAlgorithm() {
        return hashAlgorithm;
    }

    public int getOutputByteSize() {
        return outputByteSize;
    }
}
