/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

export const ERROR_DESCRIPTION = 'ArkTS Compiler Error';
export const ARKUI_SUBSYSTEM_CODE = '109'; // ArkUI subsystem coding
export const LINTER_SUBSYSTEM_CODE = '106'; // Linter subsystem coding
export const ERROR_TYPE_CODE = '05'; // Error Type Code
export const EXTENSION_CODE = '999'; // Extended Codes Defined by Various Subsystems

interface MoreInfo {
    cn: string;
    en: string;
}

export interface HvigorLogInfo {
    code?: string;
    description?: string;
    cause?: string;
    position?: string;
    solutions?: string[];
    moreInfo?: MoreInfo;
}

export class HvigorErrorInfo implements HvigorLogInfo {
    code: string = '';
    description: string = ERROR_DESCRIPTION;
    cause: string = '';
    position: string = '';
    solutions: string[] = [];

    getCode(): string {
        return this.code;
    }

    setCode(code: string): HvigorErrorInfo {
        this.code = code;
        return this;
    }

    getDescription(): string {
        return this.description;
    }

    setDescription(description: string): HvigorErrorInfo {
        this.description = description;
        return this;
    }

    getCause(): string {
        return this.cause;
    }
    
    setCause(cause: string): HvigorErrorInfo {
        this.cause = cause;
        return this;
    }

    getPosition(): string {
        return this.position;
    }

    setPosition(position: string): HvigorErrorInfo {
        this.position = position;
        return this;
    }

    getSolutions(): string[] {
        return this.solutions;
    }

    setSolutions(solutions: string[]): HvigorErrorInfo {
        this.solutions = solutions;
        return this;
    }
}
