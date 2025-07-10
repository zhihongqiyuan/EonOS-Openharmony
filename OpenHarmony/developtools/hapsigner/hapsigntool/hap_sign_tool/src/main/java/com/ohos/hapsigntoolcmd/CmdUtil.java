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

import com.ohos.hapsigntool.error.CustomException;
import com.ohos.hapsigntool.error.ERROR;
import com.ohos.hapsigntool.error.SignToolErrMsg;
import com.ohos.hapsigntool.utils.StringUtils;
import com.ohos.hapsigntool.utils.ValidateUtils;

import java.util.Arrays;
import java.util.List;
import java.util.Locale;
import java.util.regex.Pattern;

/**
 * CmdUtil.
 *
 * @since 2021/12/28
 */
public final class CmdUtil {
    /**
     * Minimum length of input args.
     */
    private static final int ARGS_MIN_LEN = 2;

    /**
     * Match size String.
     */
    private static final Pattern INTEGER_PATTERN = Pattern.compile("\\d{1,10}");

    private CmdUtil() {
    }

    /**
     * Analyze and convert args to Params object.
     *
     * @param args Command line args
     * @return Params
     */
    public static Params convert2Params(String[] args) {
        ValidateUtils.throwIfNotMatches(args.length >= ARGS_MIN_LEN, ERROR.COMMAND_ERROR,
                SignToolErrMsg.PARAM_NUM_ERROR.toString());

        Params params = new Params();
        String method = args[0];
        params.setMethod(method);
        String keyStandBy = null;
        boolean readKey = true;
        List<String> trustList = ParamsTrustlist.getTrustList(method);
        if (trustList.isEmpty()) {
            CustomException.throwException(ERROR.COMMAND_ERROR, SignToolErrMsg.UNSUPPORTED_METHOD.toString(method));
        }
        for (int i = 1; i < args.length; i++) {
            String value = args[i];
            if (StringUtils.isEmpty(value)) {
                CustomException.throwException(ERROR.COMMAND_ERROR, SignToolErrMsg.PARAM_VALUE_EMPTY
                        .toString("first param"));
            }
            if (readKey) {
                // prepare key
                if (value.startsWith("-")) {
                    value = value.equals("-signcode") ? "-signCode" : value;
                    boolean isTrust = trustList.contains(value);
                    ValidateUtils.throwIfNotMatches(isTrust,
                            ERROR.COMMAND_PARAM_ERROR, SignToolErrMsg.PARAM_NOT_TRUSTED.toString());
                    keyStandBy = value.substring(1);
                    readKey = false;
                } else {
                    CustomException.throwException(ERROR.COMMAND_PARAM_ERROR, SignToolErrMsg
                            .PARAM_NOT_IN_PAIRS.toString());
                }
            } else {
                // prepare value
                boolean success = validAndPutParam(params, keyStandBy, value);
                if (success) {
                    keyStandBy = null;
                    readKey = true;
                }
            }
        }
        return params;
    }

    private static boolean validAndPutParam(Params params, String key, String value) {
        boolean result;
        if (StringUtils.isEmpty(key)) {
            result = false;
        } else if (StringUtils.isEmpty(value)) {
            CustomException.throwException(ERROR.COMMAND_ERROR,
                    SignToolErrMsg.PARAM_VALUE_EMPTY.toString(key));
            result = false;
        } else if (params.getOptions().containsKey(key)) {
            CustomException.throwException(ERROR.COMMAND_ERROR, SignToolErrMsg.PARAM_DUPLICATE.toString(key));
            result = false;
        } else if (key.toLowerCase(Locale.ROOT).endsWith("pwd")) {
            params.getOptions().put(key, value.toCharArray());
            result = true;
        } else {
            params.getOptions().put(key, value);
            result = true;
        }
        return result;
    }

    /**
     * Alg type must between RSA and ECC.
     *
     * @param alg Incoming string
     */
    public static void judgeAlgType(String alg) {
        if (!"RSA".equalsIgnoreCase(alg) && !"ECC".equalsIgnoreCase(alg)) {
            CustomException.throwException(ERROR.COMMAND_ERROR, SignToolErrMsg.PARAM_CHECK_FAILED
                            .toString("keyAlg", "Key algorithm only allowed to be ECC or RSA."));
        }
    }

    /**
     * Check whether the algorithm size is within specified scope.
     *
     * @param size algorithm size
     * @param alg algorithm
     */
    public static void judgeSize(String size, String alg) {
        String[] array = {"2048", "3072", "4096", "NIST-P-256", "NIST-P-384"};
        List<String> arrayList = Arrays.asList(array);
        if (!arrayList.contains(size)) {
            CustomException.throwException(ERROR.COMMAND_ERROR, SignToolErrMsg.PARAM_CHECK_FAILED
                    .toString("KeySize", "Key size include in set of " + arrayList));
        }

        if ("RSA".equalsIgnoreCase(alg)) {
            if (!"2048".equals(size) && !"3072".equals(size) && !"4096".equals(size)) {
                CustomException.throwException(ERROR.COMMAND_ERROR, SignToolErrMsg.PARAM_CHECK_FAILED
                        .toString("KeySize", "Key size of RSA include in set of {2048, 3072, 4096}"));
            }
        } else {
            if (!"NIST-P-256".equalsIgnoreCase(size) && !"NIST-P-384".equalsIgnoreCase(size)) {
                CustomException.throwException(ERROR.COMMAND_ERROR, SignToolErrMsg.PARAM_CHECK_FAILED
                        .toString("KeySize", "Key size include in set of " + arrayList));
            }
        }
    }

    /**
     * Check whether the signature algorithm is within specified scope.
     *
     * @param signAlg signature algorithm
     */
    public static void judgeSignAlgType(String signAlg) {
        List<String> arrayList = Arrays.asList("SHA256withRSA", "SHA384withRSA", "SHA256withECDSA",
                "SHA384withECDSA");
        if (!arrayList.contains(signAlg)) {
            CustomException.throwException(ERROR.COMMAND_ERROR, SignToolErrMsg.PARAM_CHECK_FAILED
                    .toString("SignAlg", "signature algorithms include " + arrayList));
        }
    }

    /**
     * Check whether the signature algorithm is within specified scope.
     *
     * @param signAlg signature algorithm
     */
    public static void judgeEndSignAlgType(String signAlg) {
        List<String> arrayList = Arrays.asList("SHA256withECDSA", "SHA384withECDSA");
        if (!arrayList.contains(signAlg)) {
            CustomException.throwException(ERROR.COMMAND_ERROR, SignToolErrMsg.PARAM_CHECK_FAILED
                    .toString("SignAlg", "signature algorithms include SHA256withECDSA,SHA384withECDSA"));
        }
    }

    /**
     * Verify target type.
     *
     * @param inputType Types with ','
     * @param supportTypes Target types with ','
     */
    public static void verifyType(String inputType, String supportTypes) {
        String[] types = inputType.split(",");
        List<String> supportList = Arrays.asList(supportTypes.split(","));
        for (String type : types) {
            if (StringUtils.isEmpty(type)) {
                continue;
            }
            if (!supportList.contains(type.trim())) {
                CustomException.throwException(ERROR.COMMAND_ERROR, SignToolErrMsg
                        .PARAM_NOT_TRUSTED.toString(inputType));
            }
        }
    }

    /**
     * Convert passed in args to 'integer'.
     *
     * @param size String passed in
     * @return 'integer' String
     */
    public static String convertAlgSize(String size) {
        if (size.startsWith("NIST-P-")) {
            return size.replace("NIST-P-", "");
        } else if (INTEGER_PATTERN.matcher(size).matches()) {
            return size;
        } else {
            CustomException.throwException(ERROR.COMMAND_ERROR, SignToolErrMsg.PARAM_CHECK_FAILED
                    .toString("KeySize", "Key size is incorrect"));
            return size;
        }
    }

    /**
     * Command parameter constant
     *
     */
    public static final class Method {
        /**
         * Generate app cert method name.
         */
        public static final String GENERATE_APP_CERT = "generate-app-cert";

        /**
         * Generate ca method name.
         */
        public static final String GENERATE_CA = "generate-ca";

        /**
         * Generate cert method name.
         */
        public static final String GENERATE_CERT = "generate-cert";

        /**
         * Generate csr method name.
         */
        public static final String GENERATE_CSR = "generate-csr";

        /**
         * Generate key pair method name.
         */
        public static final String GENERATE_KEYPAIR = "generate-keypair";

        /**
         * Generate profile cert method name.
         */
        public static final String GENERATE_PROFILE_CERT = "generate-profile-cert";

        /**
         * Sign app method name.
         */
        public static final String SIGN_APP = "sign-app";

        /**
         * Sign profile method name.
         */
        public static final String SIGN_PROFILE = "sign-profile";

        /**
         * Verify app method name.
         */
        public static final String VERIFY_APP = "verify-app";

        /**
         * Verify profile method name.
         */
        public static final String VERIFY_PROFILE = "verify-profile";
    }
}
