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

package com.ohos.hapsigntool.zip;

import java.nio.ByteBuffer;

/**
 * Information of ZIP file
 *
 * @since 2021/12/20
 */
public class ZipFileInfo {
    private final long centralDirectoryOffset;
    private final int centralDirectorySize;
    private final int centralDirectoryEntryCount;
    private final long eocdOffset;
    private final ByteBuffer eocd;

    public ZipFileInfo(long centralDirectoryOffset, int centralDirectorySize, int centralDirectoryEntryCount,
        long eocdOffset, ByteBuffer eocd) {
        this.centralDirectoryOffset = centralDirectoryOffset;
        this.centralDirectorySize = centralDirectorySize;
        this.centralDirectoryEntryCount = centralDirectoryEntryCount;
        this.eocdOffset = eocdOffset;
        this.eocd = eocd;
    }

    public long getCentralDirectoryOffset() {
        return centralDirectoryOffset;
    }

    public int getCentralDirectorySize() {
        return centralDirectorySize;
    }

    public int getCentralDirectoryEntryCount() {
        return centralDirectoryEntryCount;
    }

    public long getEocdOffset() {
        return eocdOffset;
    }

    public ByteBuffer getEocd() {
        return eocd;
    }
}