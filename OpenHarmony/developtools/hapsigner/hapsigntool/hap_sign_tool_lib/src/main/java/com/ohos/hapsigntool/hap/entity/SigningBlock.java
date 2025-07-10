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

package com.ohos.hapsigntool.hap.entity;

/**
 * Signature block with TLV format
 *
 * @since 2021-12-13
 */
public class SigningBlock {
    private int type;
    private int length;
    private byte[] value;

    private int offset;

    /**
     * Init Signing Block type and value
     *
     * @param type signing type
     * @param value signing value
     */
    public SigningBlock(int type, byte[] value) {
        super();
        this.type = type;
        this.length = value.length;
        this.value = value;
    }

    /**
     * Init Signing Block type and value
     *
     * @param type signing type
     * @param value signing value
     * @param offset signing block offset
     */
    public SigningBlock(int type, byte[] value, int offset) {
        super();
        this.type = type;
        this.length = value.length;
        this.value = value;
        this.offset = offset;
    }

    public int getType() {
        return type;
    }

    public int getLength() {
        return length;
    }

    public byte[] getValue() {
        return value;
    }

    public int getOffset() {
        return offset;
    }
}