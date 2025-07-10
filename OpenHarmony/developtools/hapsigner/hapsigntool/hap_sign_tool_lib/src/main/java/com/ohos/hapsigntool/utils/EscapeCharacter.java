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

/**
 * Provider function of escape character.
 *
 * @since 2021/12/21
 */
public class EscapeCharacter {
    /**
     * The length of  character  "%"
     */
    private static final int ESCAPE_STRING1_LEN = 1;

    /**
     * The length of  character  "%u"
     */
    private static final int ESCAPE_STRING2_LEN = 2;

    /**
     * If it starts with "%", the intercept length is 2
     */
    private static final int INTERCEPT1_LEN = 2;

    /**
     * If it starts with "%u", the intercept length is 4
     */
    private static final int INTERCEPT2_LEN = 4;

    /**
     * Base number
     */
    private static final int RADIX_NUM = 16;

    /**
     * Constructor of Method
     */
    private EscapeCharacter() {
    }

    /**
     * Phase string which is escaped
     *
     * @param src escaped string
     * @return string after unescape.
     */
    public static String unescape(String src) {
        StringBuilder tmp = new StringBuilder();
        tmp.ensureCapacity(src.length());
        int lastPos = 0;
        int pos = 0;
        while (lastPos < src.length()) {
            pos = src.indexOf('%', lastPos);
            if (pos == lastPos) {
                if (src.charAt(pos + 1) == 'u') {
                    char ch = (char) Integer.parseInt(src.substring(pos + ESCAPE_STRING2_LEN,
                            pos + ESCAPE_STRING2_LEN + INTERCEPT2_LEN), RADIX_NUM);
                    tmp.append(ch);
                    lastPos = pos + ESCAPE_STRING2_LEN + INTERCEPT2_LEN;
                } else {
                    char ch = (char) Integer.parseInt(src.substring(pos + ESCAPE_STRING1_LEN,
                            pos + ESCAPE_STRING1_LEN + INTERCEPT1_LEN), RADIX_NUM);
                    tmp.append(ch);
                    lastPos = pos + ESCAPE_STRING1_LEN + INTERCEPT1_LEN;
                }
            } else if (pos == -1) {
                tmp.append(src.substring(lastPos));
                lastPos = src.length();
            } else {
                tmp.append(src.substring(lastPos, pos));
                lastPos = pos;
            }
        }
        return tmp.toString();
    }
}
