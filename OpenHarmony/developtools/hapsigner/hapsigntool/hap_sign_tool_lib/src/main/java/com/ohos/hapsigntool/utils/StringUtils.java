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
 * StringUtils.
 *
 * @since 2021/12/28
 */
public final class StringUtils {
    private StringUtils() {
    }

    /**
     * Check whether the input string is empty.
     *
     * @param cs input string
     * @return true, if cs is empty
     */
    public static boolean isEmpty(final CharSequence cs) {
        return cs == null || cs.length() == 0;
    }

    /**
     * Check whether the array contains string ignoring case.
     *
     * @param array input string array
     * @param str input string
     * @return true, if the array contains the str ignoring case
     */
    public static boolean containsIgnoreCase(String[] array, String str) {
        if (array == null) {
            return false;
        }
        for (String s : array) {
            if (s != null && s.equalsIgnoreCase(str)) {
                return true;
            }
        }
        return false;
    }
}
