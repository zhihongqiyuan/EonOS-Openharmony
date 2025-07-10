/*
 * Copyright (c) 2024-2024 Huawei Device Co., Ltd.
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

package com.ohos.entity;

import com.ohos.hapsigntool.entity.Options;
import com.ohos.hapsigntool.error.ParamException;

/**
 * VerifyProfileParameters.
 *
 * @since 2024/04/06
 */
public class VerifyProfileParameters implements Parameters {
    /**
     * signed Provision Profile file, p7b format, required fields
     */
    private String inFile;

    /**
     * Verification result file, json format, optional
     */
    private String outFile;

    @Override
    public Options toOptions() throws ParamException {
        Options options = new Options();
        if (inFile == null) {
            throw new ParamException(Options.IN_FILE);
        }
        options.put(Options.IN_FILE, inFile);

        if (outFile != null) {
            options.put(Options.OUT_FILE, outFile);
        }
        return options;
    }

    public String getInFile() {
        return inFile;
    }

    public void setInFile(String inFile) {
        this.inFile = inFile;
    }

    public String getOutFile() {
        return outFile;
    }

    public void setOutFile(String outFile) {
        this.outFile = outFile;
    }
}
