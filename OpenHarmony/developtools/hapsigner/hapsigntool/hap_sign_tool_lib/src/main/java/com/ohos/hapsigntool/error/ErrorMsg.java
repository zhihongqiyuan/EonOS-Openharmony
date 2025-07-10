/*
 * Copyright (c) 2025-2025 Huawei Device Co., Ltd.
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

package com.ohos.hapsigntool.error;

import com.ohos.hapsigntool.utils.StringUtils;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.util.ArrayList;
import java.util.List;
import java.util.Locale;
import java.util.MissingFormatArgumentException;

/**
 * ErrorMsg
 *
 * @since 2025/01/06
 */
public class ErrorMsg {
    private static final Logger log = LogManager.getLogger(ErrorMsg.class);

    private static final String CODE_SIGN_SUB_SYSTEM_CODE = "111";

    private static final String SIGN_TOOL_SUB_SYSTEM_CODE = "110";

    private final String code;

    private final String description;

    private final String cause;

    private final String position;

    private final List<String> solutions;

    private MoreInfo moreInfo;

    /**
     * ErrorMsg constructor
     *
     * @param code code
     * @param description description
     * @param cause cause
     * @param position position
     * @param solutions solutions
     */
    public ErrorMsg(String code, String description, String cause,
                    String position, List<String> solutions) {
        this.code = code;
        this.description = description;
        this.cause = cause;
        this.position = position;
        this.solutions = solutions;
        this.moreInfo = new MoreInfo();
    }

    /**
     * getCodeSignErrBuilder
     *
     * @return Builder
     */
    public static Builder getCodeSignErrBuilder() {
        return new Builder(CODE_SIGN_SUB_SYSTEM_CODE);
    }

    /**
     * getSignToolErrBuilder
     *
     * @return Builder
     */
    public static Builder getSignToolErrBuilder() {
        return new Builder(SIGN_TOOL_SUB_SYSTEM_CODE);
    }

    @Override
    public String toString() {
        final StringBuilder sb = new StringBuilder();
        sb.append(System.lineSeparator())
            .append("ERROR: ")
            .append(code)
            .append(" ")
            .append(description)
            .append(System.lineSeparator())
            .append("Error Message: ")
            .append(cause)
            .append(System.lineSeparator());

        if (!StringUtils.isEmpty(position)) {
            sb.append(" At ").append(position).append(System.lineSeparator());
        }

        if (solutions != null && !solutions.isEmpty()) {
            sb.append(System.lineSeparator()).append("* Try the following: ").append(System.lineSeparator());
            for (String s : solutions) {
                sb.append(" > ").append(s).append(System.lineSeparator());
            }
        }
        return sb.toString();
    }

    /**
     * to String
     *
     * @param args args
     * @return String
     */
    public String toString(Object... args) {
        try {
            return String.format(Locale.ROOT, this.toString(), args);
        } catch (MissingFormatArgumentException e) {
            log.error("args format failed: " + args);
            return this.toString();
        }
    }

    static class MoreInfo {
        private String cn;

        private String en;
    }

    /**
     * Builder
     *
     * @since 2025/01/06
     */
    public static class Builder {
        private String sysCode;

        private String errCode;

        private String typeCode;

        private String description;

        private String cause;

        private String position;

        private List<String> solutions = new ArrayList<>();

        /**
         * Builder Constructor
         *
         * @param sysCode sysCode
         */
        public Builder(String sysCode) {
            this.sysCode = sysCode;
        }

        /**
         * addErrCode
         *
         * @param errCode errCode
         * @return Builder
         */
        public Builder addErrCode(String errCode) {
            this.errCode = errCode;
            return this;
        }

        /**
         * addTypeCode
         *
         * @param typeCode typeCode
         * @return Builder
         */
        public Builder addTypeCode(String typeCode) {
            this.typeCode = typeCode;
            return this;
        }

        /**
         * addDescription
         *
         * @param description description
         * @return Builder
         */
        public Builder addDescription(String description) {
            this.description = description;
            return this;
        }

        /**
         * addCause
         *
         * @param cause cause
         * @return Builder
         */
        public Builder addCause(String cause) {
            this.cause = cause;
            return this;
        }

        /**
         * addPosition
         *
         * @param position position
         * @return Builder
         */
        public Builder addPosition(String position) {
            this.position = position;
            return this;
        }

        /**
         * addSolution
         *
         * @param solution solution
         * @return Builder
         */
        public Builder addSolution(String solution) {
            this.solutions.add(solution);
            return this;
        }

        /**
         * build
         *
         * @return ErrorMsg
         */
        public ErrorMsg build() {
            return new ErrorMsg(sysCode + typeCode + errCode, description, cause, position, solutions);
        }
    }
}
