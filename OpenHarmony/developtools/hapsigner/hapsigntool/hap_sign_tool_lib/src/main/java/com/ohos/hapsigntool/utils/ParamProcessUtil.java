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

package com.ohos.hapsigntool.utils;

import com.ohos.hapsigntool.entity.ParamConstants;
import com.ohos.hapsigntool.entity.SignatureAlgorithm;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.util.Arrays;
import java.util.HashSet;
import java.util.Set;

/**
 * Utils functions for processing parameters.
 *
 * @since 2021/12/21
 */
public class ParamProcessUtil {
    private static final LogUtils LOGGER = new LogUtils(ParamProcessUtil.class);

    private ParamProcessUtil() {
    }

    /**
     * Use string array to init string set
     *
     * @param paramFileds input string array
     * @return string set.
     */
    public static Set<String> initParamField(String[] paramFileds) {
        return new HashSet<String>(Arrays.asList(paramFileds));
    }

    /**
     * Delete all files Recursively
     *
     * @param file file path.
     */
    public static void delDir(File file) {
        if (file.isDirectory()) {
            File[] zFiles = file.listFiles();
            if (zFiles != null) {
                for (File file2 : zFiles) {
                    delDir(file2);
                }
            }
        }
        try {
            Files.delete(file.toPath());
        } catch (IOException e) {
            LOGGER.warn("delete files failed!");
        }
    }

    /**
     * Get SignatureAlgorithm value by algorithm name.
     *
     * @param signatureAlgorithm algorithm name.
     * @return SignatureAlgorithm value
     */
    public static SignatureAlgorithm getSignatureAlgorithm(String signatureAlgorithm) {
        SignatureAlgorithm result;
        if (ParamConstants.HAP_SIG_ALGORITHM_SHA256_ECDSA.equalsIgnoreCase(signatureAlgorithm)) {
            result = SignatureAlgorithm.ECDSA_WITH_SHA256;
        } else if (ParamConstants.HAP_SIG_ALGORITHM_SHA384_ECDSA.equalsIgnoreCase(signatureAlgorithm)) {
            result = SignatureAlgorithm.ECDSA_WITH_SHA384;
        } else if (ParamConstants.HAP_SIG_ALGORITHM_SHA512_ECDSA.equalsIgnoreCase(signatureAlgorithm)) {
            result = SignatureAlgorithm.ECDSA_WITH_SHA512;
        } else if (ParamConstants.HAP_SIG_ALGORITHM_SHA256_RSA_PSS.equalsIgnoreCase(signatureAlgorithm)) {
            result = SignatureAlgorithm.RSA_PSS_WITH_SHA256;
        } else if (ParamConstants.HAP_SIG_ALGORITHM_SHA384_RSA_PSS.equalsIgnoreCase(signatureAlgorithm)) {
            result = SignatureAlgorithm.RSA_PSS_WITH_SHA384;
        } else if (ParamConstants.HAP_SIG_ALGORITHM_SHA512_RSA_PSS.equalsIgnoreCase(signatureAlgorithm)) {
            result = SignatureAlgorithm.RSA_PSS_WITH_SHA512;
        } else if (ParamConstants.HAP_SIG_ALGORITHM_SHA256_RSA_MGF1.equalsIgnoreCase(signatureAlgorithm)) {
            result = SignatureAlgorithm.RSA_PSS_WITH_SHA256;
        } else if (ParamConstants.HAP_SIG_ALGORITHM_SHA384_RSA_MGF1.equalsIgnoreCase(signatureAlgorithm)) {
            result = SignatureAlgorithm.RSA_PSS_WITH_SHA384;
        } else if (ParamConstants.HAP_SIG_ALGORITHM_SHA512_RSA_MGF1.equalsIgnoreCase(signatureAlgorithm)) {
            result = SignatureAlgorithm.RSA_PSS_WITH_SHA512;
        } else {
            throw new IllegalArgumentException("Unsupported signature algorithm: " + signatureAlgorithm);
        }
        return result;
    }
}
