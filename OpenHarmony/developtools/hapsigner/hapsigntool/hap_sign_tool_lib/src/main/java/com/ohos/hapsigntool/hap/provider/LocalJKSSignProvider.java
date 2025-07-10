/*
 * Copyright (c) 2021-2023 Huawei Device Co., Ltd.
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

package com.ohos.hapsigntool.hap.provider;

import com.ohos.hapsigntool.entity.Options;
import com.ohos.hapsigntool.error.InvalidParamsException;
import com.ohos.hapsigntool.error.SignToolErrMsg;
import com.ohos.hapsigntool.utils.FileUtils;
import com.ohos.hapsigntool.entity.ParamConstants;
import com.ohos.hapsigntool.utils.LogUtils;
import com.ohos.hapsigntool.utils.ParamProcessUtil;

import com.ohos.hapsigntool.utils.StringUtils;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.security.GeneralSecurityException;
import java.security.cert.CRL;
import java.security.cert.CertificateFactory;
import java.security.cert.X509CRL;
import java.util.Optional;
import java.util.Set;

/**
 * Local keystore sign provider
 *
 * @since 2021/12/22
 */
public class LocalJKSSignProvider extends SignProvider {
    private static final LogUtils LOGGER = new LogUtils(LocalJKSSignProvider.class);

    @Override
    public Optional<X509CRL> getCrl() {
        X509CRL crl = null;
        String crlPath = signParams.get(ParamConstants.PARAM_BASIC_CRL);
        if (StringUtils.isEmpty(crlPath)) {
            return Optional.ofNullable(crl);
        }
        try (FileInputStream input = new FileInputStream(crlPath)) {
            CertificateFactory cf = CertificateFactory.getInstance("X.509");
            CRL baseCrl = cf.generateCRL(input);
            if (!(baseCrl instanceof X509CRL)) {
                LOGGER.error("crl is not X509CRL");
                return Optional.ofNullable(crl);
            }
            crl = (X509CRL) baseCrl;
        } catch (IOException e) {
            LOGGER.error("read CRL File has IOException!");
            crl = null;
        } catch (GeneralSecurityException e) {
            LOGGER.error("Generate x509 CRL failed!");
            crl = null;
        }
        return Optional.ofNullable(crl);
    }

    /**
     * check public cert
     *
     * @throws InvalidParamsException Exception occurs when the appCertFile is invalid.
     */
    private void checkPublicKeyPath() throws InvalidParamsException {
        String publicCertsFile = signParams.get(ParamConstants.PARAM_LOCAL_PUBLIC_CERT);
        File publicKeyFile = new File(publicCertsFile);
        try {
            FileUtils.isValidFile(publicKeyFile);
        } catch (IOException e) {
            LOGGER.error("file is invalid: " + publicCertsFile + System.lineSeparator(), e);
            throw new InvalidParamsException(SignToolErrMsg.PARAM_CHECK_FAILED
                    .toString(ParamConstants.PARAM_LOCAL_PUBLIC_CERT, "Invalid file: " + publicCertsFile));
        }
    }

    @Override
    public void checkParams(Options options) throws InvalidParamsException {
        super.checkParams(options);
        String[] paramFileds = {
                ParamConstants.PARAM_LOCAL_JKS_KEYSTORE,
                ParamConstants.PARAM_LOCAL_JKS_KEYSTORE_CODE,
                ParamConstants.PARAM_LOCAL_JKS_KEYALIAS_CODE
        };

        Set<String> paramSet = ParamProcessUtil.initParamField(paramFileds);

        for (String paramKey : options.keySet()) {
            if (paramSet.contains(paramKey)) {
                if (paramKey.endsWith("Pwd")) {
                    signParams.put(paramKey, new String(options.getChars(paramKey)));
                } else {
                    signParams.put(paramKey, options.getString(paramKey));
                }
            }
        }
        checkSignCode();
        checkPublicKeyPath();
    }
}
