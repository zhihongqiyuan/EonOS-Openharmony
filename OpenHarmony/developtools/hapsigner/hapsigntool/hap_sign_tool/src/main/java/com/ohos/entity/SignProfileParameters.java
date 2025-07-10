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
 * SignProfileParameters.
 *
 * @since 2024/04/06
 */
public class SignProfileParameters implements Parameters {
    /**
     * signature mode, required fields, including localSign/remoteSign
     */
    private Mode mode;

    /**
     * key alias, required fields
     */
    private String keyAlias;

    /**
     * key password, optional fields on localSign mode
     */
    private char[] keyPwd;

    /**
     * profile signing certificate, required fields
     */
    private String profileCertFile;

    /**
     * input original profile file, required fields
     */
    private String inFile;

    /**
     * signature algorithm, required fields
     */
    private String signAlg;

    /**
     * keystore file, if signature mode is localSign, required fields on localSign mode, JKS or P12 format
     */
    private String keyStoreFile;

    /**
     * keystore password, optional fields on localSign mode
     */
    private char[] keystorePwd;

    /**
     * output the signed Provision Profile file, required fields
     */
    private String outFile;

    @Override
    public Options toOptions() throws ParamException {
        Options options = new Options();
        if (mode == null) {
            throw new ParamException(Options.MODE);
        }
        options.put(Options.MODE, mode.getValue());

        if (keyAlias == null) {
            throw new ParamException(Options.KEY_ALIAS);
        }
        options.put(Options.KEY_ALIAS, keyAlias);

        if (keyPwd != null) {
            options.put(Options.KEY_RIGHTS, keyPwd);
        }

        if (profileCertFile == null) {
            throw new ParamException(Options.PROFILE_CERT_FILE);
        }
        options.put(Options.PROFILE_CERT_FILE, profileCertFile);

        if (inFile == null) {
            throw new ParamException(Options.IN_FILE);
        }
        options.put(Options.IN_FILE, inFile);

        if (signAlg == null) {
            throw new ParamException(Options.SIGN_ALG);
        }
        options.put(Options.SIGN_ALG, signAlg);

        if (keyStoreFile == null) {
            throw new ParamException(Options.KEY_STORE_FILE);
        }
        options.put(Options.KEY_STORE_FILE, keyStoreFile);

        if (keystorePwd != null) {
            options.put(Options.KEY_STORE_RIGHTS, keystorePwd);
        }

        if (outFile == null) {
            throw new ParamException(Options.OUT_FILE);
        }
        options.put(Options.OUT_FILE, outFile);
        return options;
    }

    public Mode getMode() {
        return mode;
    }

    public void setMode(Mode mode) {
        this.mode = mode;
    }

    public String getKeyAlias() {
        return keyAlias;
    }

    public void setKeyAlias(String keyAlias) {
        this.keyAlias = keyAlias;
    }

    public char[] getKeyPwd() {
        return keyPwd;
    }

    public void setKeyPwd(char[] keyPwd) {
        this.keyPwd = keyPwd;
    }

    public String getProfileCertFile() {
        return profileCertFile;
    }

    public void setProfileCertFile(String profileCertFile) {
        this.profileCertFile = profileCertFile;
    }

    public String getInFile() {
        return inFile;
    }

    public void setInFile(String inFile) {
        this.inFile = inFile;
    }

    public String getSignAlg() {
        return signAlg;
    }

    public void setSignAlg(String signAlg) {
        this.signAlg = signAlg;
    }

    public String getKeyStoreFile() {
        return keyStoreFile;
    }

    public void setKeyStoreFile(String keyStoreFile) {
        this.keyStoreFile = keyStoreFile;
    }

    public char[] getKeystorePwd() {
        return keystorePwd;
    }

    public void setKeystorePwd(char[] keystorePwd) {
        this.keystorePwd = keystorePwd;
    }

    public String getOutFile() {
        return outFile;
    }

    public void setOutFile(String outFile) {
        this.outFile = outFile;
    }
}
