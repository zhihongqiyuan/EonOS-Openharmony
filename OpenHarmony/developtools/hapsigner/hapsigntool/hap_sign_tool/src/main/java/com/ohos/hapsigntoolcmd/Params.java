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

package com.ohos.hapsigntoolcmd;

import com.ohos.hapsigntool.entity.Options;

import java.util.Map;

/**
 * Params.
 *
 * @since 2021/12/28
 */
public class Params {
    /**
     * Method names in the command line.
     */
    private String method;

    /**
     * Hashmap for storing parameters.
     */
    private final Options options = new Options();

    /**
     * Constructor of Params.
     */
    public Params() {}

    public String getMethod() {
        return method;
    }

    public void setMethod(String method) {
        this.method = method;
    }

    public Options getOptions() {
        return options;
    }

    @Override
    public String toString() {
        StringBuilder stringBuilder = new StringBuilder();
        stringBuilder.append("Params{ method: ");
        stringBuilder.append(method);
        stringBuilder.append(", params: ");
        for (Map.Entry<String, Object> item : options.entrySet()) {
            stringBuilder.append('-');
            stringBuilder.append(item.getKey());
            stringBuilder.append("='");
            stringBuilder.append(item.getValue());
            stringBuilder.append("';");
        }
        stringBuilder.append('}');

        return stringBuilder.toString();
    }
}
