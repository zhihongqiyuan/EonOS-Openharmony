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

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Optional;

/**
 * 入参白名单获取类
 *
 * @since 2022/06/01
 */
public final class ParamsTrustlist {
    /**
     * Define generic string
     */
    public static final String OPTIONS = " [options]:";

    /**
     * Define commond list
     */
    private static final List<String> COMMONDS = new ArrayList<String>();

    /**
     * Define trust map
     */
    private static HashMap<String, List<String>> trustMap = new HashMap<>();

    static {
        COMMONDS.add(CmdUtil.Method.GENERATE_KEYPAIR + OPTIONS);
        COMMONDS.add(CmdUtil.Method.GENERATE_CSR + OPTIONS);
        COMMONDS.add(CmdUtil.Method.GENERATE_CERT + OPTIONS);
        COMMONDS.add(CmdUtil.Method.GENERATE_CA + OPTIONS);
        COMMONDS.add(CmdUtil.Method.GENERATE_APP_CERT + OPTIONS);
        COMMONDS.add(CmdUtil.Method.GENERATE_PROFILE_CERT + OPTIONS);
        COMMONDS.add(CmdUtil.Method.SIGN_PROFILE + OPTIONS);
        COMMONDS.add(CmdUtil.Method.VERIFY_PROFILE + OPTIONS);
        COMMONDS.add(CmdUtil.Method.SIGN_APP + OPTIONS);
        COMMONDS.add(CmdUtil.Method.VERIFY_APP + OPTIONS);
    }

    private ParamsTrustlist() {
    }

    /**
     * Generate Trustlist
     */
    public static void generateTrustlist() {
        ClassLoader classLoader = ParamsTrustlist.class.getClassLoader();
        if (classLoader == null) {
            return ;
        }
        String page = "help.txt";
        try (InputStream inputStream = classLoader.getResourceAsStream(page)) {
            if (inputStream == null) {
                return;
            }
            try (InputStreamReader isr = new InputStreamReader(inputStream, StandardCharsets.UTF_8);
                BufferedReader br = new BufferedReader(isr)) {
                readHelpParam(br);
            }
        } catch (IOException e) {
            CustomException.throwException(ERROR.READ_FILE_ERROR, SignToolErrMsg.FILE_READ_FAILED.toString(page));
        }
    }

    private static void readHelpParam(BufferedReader br) throws IOException {
        String str;
        String cmdStandBy = null;
        while ((str = br.readLine()) != null) {
            String param = str.trim();
            if (COMMONDS.contains(param)) {
                cmdStandBy = param;
            } else {
                putTrustMap(cmdStandBy, param);
            }
        }
    }

    /**
     * Put trustlist map
     *
     * @param cmdStandBy command as key
     * @param param commond as value
     */
    private static void putTrustMap(String cmdStandBy, String param) {
        if (param.startsWith("-")) {
            String subParam = param.substring(0, param.indexOf(":")).trim();
            List<String> trustLists = Optional.ofNullable(
                    trustMap.get(cmdStandBy)).orElse(new ArrayList<>());
            trustLists.add(subParam);
            trustMap.put(cmdStandBy, trustLists);
        }
    }

    /**
     * Get Trustlist
     *
     * @param commond commond
     * @return  TrustList
     */
    public static List<String> getTrustList(String commond) {
        generateTrustlist();
        String keyParam = commond + OPTIONS;
        return Optional.ofNullable(trustMap.get(keyParam)).orElse(new ArrayList<>());
    }

}
