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

package com.ohos.hapsigntool.codesigning.datastructure;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.Locale;

/**
 * Extension is an optional field in relative to SignInfo.
 * It is the base class for all types of extensions, i.e. MerkleTreeExtension.
 * <p>
 * Structure:
 * u32 type: Indicates the type of extension
 * <p>
 * u32 size: byte size of extension data
 *
 * @since 2023/09/08
 */
public class Extension {
    /**
     * Byte size of Extension base class.
     */
    public static final int EXTENSION_HEADER_SIZE = 8;

    private final int type;

    private int size;

    public Extension(int type, int size) {
        this.type = type;
        this.size = size;
    }

    public int size() {
        return EXTENSION_HEADER_SIZE;
    }

    public void setSize(int size) {
        this.size = size;
    }

    public boolean isType(int type) {
        return this.type == type;
    }

    /**
     * Converts Extension to a newly created byte array
     *
     * @return Byte array representation of Extension
     */
    public byte[] toByteArray() {
        ByteBuffer bf = ByteBuffer.allocate(EXTENSION_HEADER_SIZE).order(ByteOrder.LITTLE_ENDIAN);
        bf.putInt(this.type);
        bf.putInt(this.size);
        return bf.array();
    }

    /**
     * Return a string representation of the object
     *
     * @return string representation of the object
     */
    public String toString() {
        return String.format(Locale.ROOT, "Extension: type[%d], size[%d]", this.type, this.size);
    }
}
