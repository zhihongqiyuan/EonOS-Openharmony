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
import com.ohos.hapsigntool.utils.FileUtils;
import com.ohos.hapsigntool.utils.LogUtils;

import java.io.IOException;
import java.io.InputStream;
import java.nio.charset.StandardCharsets;

/**
 * HelpDocument.
 *
 * @since 2021/12/28
 */
public final class HelpDocument {
    private HelpDocument() {
    }

    /**
     * Print help.txt into logger.info.
     *
     * @param logger log4j
     */
    public static void printHelp(LogUtils logger) {
        ClassLoader classLoader = HelpDocument.class.getClassLoader();
        if (classLoader == null) {
            return;
        }

        String page = "help.txt";

        try(InputStream inputStream = classLoader.getResourceAsStream(page)) {
            if (inputStream == null) {
                return;
            }
            byte[] helpData = FileUtils.read(inputStream);
            String helpStr = new String(helpData, StandardCharsets.UTF_8);
            logger.info(helpStr);
        } catch (IOException ioe) {
            logger.debug(ioe.getMessage(), ioe);
            CustomException.throwException(ERROR.READ_FILE_ERROR, SignToolErrMsg.FILE_READ_FAILED.toString(page));
        }
    }
}
