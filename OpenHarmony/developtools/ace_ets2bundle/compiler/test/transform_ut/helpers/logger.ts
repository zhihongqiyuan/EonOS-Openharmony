import { ARKUI_ERROR_CODE_IN_TS_DIAGNOSTIC, ARKUI_SUBSYSTEM_CODE, TSC_SYSTEM_CODE } from "./common";

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
enum LogType {
	ERROR = 'ERROR',
	WARN = 'WARN',
	NOTE = 'NOTE'
  }

interface MoreInfo {
    cn: string;
    en: string;
}

interface HvigorLogInfo {
	code: string;
    description?: string;
    cause: string;
    position?: string;
    solutions?: string[];
    moreInfo?: MoreInfo;
}

interface LogInfo extends Partial<HvigorLogInfo> {
  type?: LogType,
  message: string,
  pos?: number,
  line?: number,
  column?: number,
  fileName?: string
}

class Logger {
	private prefix: string;
	private errorInfos: LogInfo[] = [];
	private warnInfos: LogInfo[] = [];
	private noteInfos: LogInfo[] = [];
	static instances: Logger[] = [];

	constructor(prefix: string) {
		this.prefix = prefix;
	}

	public getErrorInfos(): LogInfo[] {
		return this.errorInfos;
	}

	public getWarnInfos(): LogInfo[] {
		return this.warnInfos;
	}

	public getNoteInfos(): LogInfo[] {
		return this.noteInfos;
	}

	public pushErrorInfos(infos: LogInfo[]): void {
		this.errorInfos = this.errorInfos.concat(infos);
	}

	public pushWarnInfos(infos: LogInfo[]): void {
		this.warnInfos = this.warnInfos.concat(infos);
	}

	public pushNoteInfos(infos: LogInfo[]): void {
		this.noteInfos = this.noteInfos.concat(infos);
	}

	public debug(msg: string) {
		console.debug(msg);
	}

	public printError(info: LogInfo) {
		if (this.prefix === ARKUI_SUBSYSTEM_CODE) {
			this.errorInfos.push(info);
		} else if (this.prefix === TSC_SYSTEM_CODE && ARKUI_ERROR_CODE_IN_TS_DIAGNOSTIC.includes(info.code)) {
			this.errorInfos.push(info);
		}
	}

	public printWarn(msg: string) {
		if (this.prefix === ARKUI_SUBSYSTEM_CODE) {
			this.warnInfos.push({ message: msg });
		}
	}

	public printInfo(msg: string) {
		if (this.prefix === ARKUI_SUBSYSTEM_CODE) {
			this.noteInfos.push({ message: msg });
		}
	}

	public error(msg: string) {
		this.errorInfos.push({ message: msg });
	}

	public warn(msg: string) {
		this.warnInfos.push({ message: msg });
	}

	public info(msg: string) {
		this.noteInfos.push({ message: msg });
	}

	public getPrefix() {
		return this.prefix;
	}

	public static getLogger(prefix: string): Logger {
		for (const instance of Logger.instances) {
			if (instance.getPrefix() == prefix) {
				return instance;
			}
		}
	}

	public static createLogger(prefix: string): Logger {
		const logger = new Logger(prefix);
		Logger.instances.push(logger);
		return logger;
	}

	public static flush(): void {
		Logger.instances = [];
	}

	public static removeLogger(prefix: string): void {
		Logger.instances = Logger.instances.filter(
			(instance) => instance.getPrefix() !== prefix
		);
	}

	public static mergeLoggers(...prefixes: string[]): Logger {
		const logger = new Logger('__merged__');
		for (const instance of Logger.instances) {
			if (prefixes.includes(instance.getPrefix())) {
				logger.pushErrorInfos(instance.getErrorInfos());
				logger.pushWarnInfos(instance.getWarnInfos());
				logger.pushNoteInfos(instance.getNoteInfos());
			}
		}
		return logger;
	}
}

export {
	Logger,
	HvigorLogInfo,
	LogInfo
}