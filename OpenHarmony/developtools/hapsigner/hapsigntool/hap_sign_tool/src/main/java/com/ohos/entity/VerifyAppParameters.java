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
 * VerifyAppParameters.
 *
 * @since 2024/04/06
 */
public class VerifyAppParameters implements Parameters {
    /**
     * verify application package file, hap or elf format, required fields
     */
    private String inFile;

    /**
     * verify certificate chain file, required fields
     */
    private String outCertChain;

    /**
     * profile file in application package, required fields
     */
    private String outProfile;

    /**
     * Enter the format of the original file. The supported file formats include ZIP, BIN, and ELF, default value ZIP
     */
    private InForm inForm = InForm.ZIP;

    @Override
    public Options toOptions() throws ParamException {
        Options options = new Options();
        if (inFile == null) {
            throw new ParamException(Options.IN_FILE);
        }
        options.put(Options.IN_FILE, inFile);

        if (outCertChain == null) {
            throw new ParamException(Options.OUT_CERT_CHAIN);
        }
        options.put(Options.OUT_CERT_CHAIN, outCertChain);

        if (outProfile == null) {
            throw new ParamException(Options.OUT_PROFILE);
        }
        options.put(Options.OUT_PROFILE, outProfile);

        if (inForm != null) {
            options.put(Options.IN_FORM, inForm.getValue());
        }
        return options;
    }

    public String getInFile() {
        return inFile;
    }

    public void setInFile(String inFile) {
        this.inFile = inFile;
    }

    public String getOutCertChain() {
        return outCertChain;
    }

    public void setOutCertChain(String outCertChain) {
        this.outCertChain = outCertChain;
    }

    public String getOutProfile() {
        return outProfile;
    }

    public void setOutProfile(String outProfile) {
        this.outProfile = outProfile;
    }

    public InForm getInForm() {
        return inForm;
    }

    public void setInForm(InForm inForm) {
        this.inForm = inForm;
    }
}
