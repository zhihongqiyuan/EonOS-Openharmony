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

package com.ohos.hapsigntool.hap.entity;

import com.ohos.hapsigntool.utils.FileUtils;

/**
 * sign block data
 *
 * @since 2023-11-07
 */
public class SignBlockData {
    private char type;
    private byte[] blockHead;
    private byte[] signData;
    private String signFile;
    private long len;
    private boolean isByte;

    public SignBlockData(byte[] signData, char type) {
        this.signData = signData;
        this.type = type;
        this.len = signData == null ? 0 : signData.length;
        this.isByte = true;
    }

    public SignBlockData(String signFile, char type) {
        this.signFile = signFile;
        this.type = type;
        this.len = FileUtils.getFileLen(signFile);
        this.isByte = false;
    }

    public char getType() {
        return type;
    }

    public void setType(char type) {
        this.type = type;
    }

    public byte[] getBlockHead() {
        return blockHead;
    }

    public void setBlockHead(byte[] blockHead) {
        this.blockHead = blockHead;
    }

    public byte[] getSignData() {
        return signData;
    }

    public void setSignData(byte[] signData) {
        this.signData = signData;
    }

    public String getSignFile() {
        return signFile;
    }

    public void setSignFile(String signFile) {
        this.signFile = signFile;
    }

    public long getLen() {
        return len;
    }

    public void setLen(long len) {
        this.len = len;
    }

    public boolean isByte() {
        return isByte;
    }

    public void setByte(boolean isByte) {
        this.isByte = isByte;
    }
}
