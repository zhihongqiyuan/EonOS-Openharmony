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

import * as fs from 'fs';
import type { IOptions } from '../configs/IOptions';
import { performancePrinter, performanceTimeAndMemPrinter } from '../ArkObfuscator';
import type { IPrinterOption } from '../configs/INameObfuscationOption';
import { eventListTimeAndMemValues } from './PrinterTimeAndMemUtils';

export enum EventList {
  OBFUSCATION_INITIALIZATION = 'Obfuscation initialization',
  SCAN_SYSTEMAPI = 'Scan system api',
  SCAN_SOURCEFILES = 'Scan source files',
  ALL_FILES_OBFUSCATION = 'All files obfuscation',
  OBFUSCATE = 'Obfuscate',
  CREATE_AST = 'Create AST',
  OBFUSCATE_AST = 'Obfuscate AST',
  VIRTUAL_CONSTRUCTOR_OBFUSCATION = 'Virtual constructor obfuscation',
  SHORT_HAND_OBFUSCATION = 'Shorthand obfuscation',
  REMOVE_CONSOLE = 'Remove console',
  PROPERTY_OBFUSCATION = 'Property obfuscation',
  IDENTIFIER_OBFUSCATION = 'Identifier obfuscation',
  CREATE_CHECKER = 'Create checker',
  CREATE_PROGRAM = 'Create program',
  GET_CHECKER = 'Get checker',
  SCOPE_ANALYZE = 'Scope analyze',
  CREATE_OBFUSCATED_NAMES = 'Create obfuscated names',
  OBFUSCATE_NODES = 'Obfuscate nodes',
  FILENAME_OBFUSCATION = 'Filename obfuscation',
  CREATE_PRINTER = 'Create Printer',
  GET_SOURCEMAP_GENERATOR = 'Get sourcemap generator',
  SOURCEMAP_MERGE = 'Sourcemap merge',
  CREATE_NAMECACHE = 'Create namecache',
  WRITE_FILE = 'Write file'
}

export enum EventIndentation {
  FOURSPACE = 4,
  THREESPACE = 3,
  TWOSPACE = 2,
  ONESPACE = 1,
  NOSPACE = 0,
};

export const eventList = new Map<string, number>([
  [EventList.OBFUSCATION_INITIALIZATION, EventIndentation.ONESPACE],
  [EventList.SCAN_SYSTEMAPI, EventIndentation.TWOSPACE],
  [EventList.SCAN_SOURCEFILES, EventIndentation.ONESPACE],
  [EventList.ALL_FILES_OBFUSCATION, EventIndentation.ONESPACE],
  [EventList.OBFUSCATE, EventIndentation.ONESPACE],
  [EventList.CREATE_AST, EventIndentation.TWOSPACE],
  [EventList.OBFUSCATE_AST, EventIndentation.TWOSPACE],
  [EventList.VIRTUAL_CONSTRUCTOR_OBFUSCATION, EventIndentation.THREESPACE],
  [EventList.SHORT_HAND_OBFUSCATION, EventIndentation.THREESPACE],
  [EventList.REMOVE_CONSOLE, EventIndentation.THREESPACE],
  [EventList.PROPERTY_OBFUSCATION, EventIndentation.THREESPACE],
  [EventList.IDENTIFIER_OBFUSCATION, EventIndentation.THREESPACE],
  [EventList.CREATE_CHECKER, EventIndentation.THREESPACE],
  [EventList.CREATE_PROGRAM, EventIndentation.FOURSPACE],
  [EventList.GET_CHECKER, EventIndentation.FOURSPACE],
  [EventList.SCOPE_ANALYZE, EventIndentation.THREESPACE],
  [EventList.CREATE_OBFUSCATED_NAMES, EventIndentation.THREESPACE],
  [EventList.OBFUSCATE_NODES, EventIndentation.THREESPACE],
  [EventList.FILENAME_OBFUSCATION, EventIndentation.THREESPACE],
  [EventList.CREATE_PRINTER, EventIndentation.TWOSPACE],
  [EventList.GET_SOURCEMAP_GENERATOR, EventIndentation.TWOSPACE],
  [EventList.SOURCEMAP_MERGE, EventIndentation.TWOSPACE],
  [EventList.CREATE_NAMECACHE, EventIndentation.TWOSPACE],
  [EventList.WRITE_FILE, EventIndentation.ONESPACE]
]);

export interface TimeAndMemInfo {
  start: number;
  duration: number;
  startMemory: number;
  endMemory: number;
  memoryUsage: number;
  filePath?: string;
}

const eventListValues = new Set<string>(Object.values(EventList));

const MILLISECOND_TO_SECOND = 1000;
const BYTE_TO_MB = 1024 * 1024;
const SIG_FIGS = 3; // Significant figures
const INDENT = '    ';
const DEFAULT_DEPTH = 2; // Default indent length

abstract class BasePrinter {
  protected outputPath: string | undefined;

  protected abstract getCurrentEventData(): string;

  protected enablePrinter = true;

  disablePrinter(): void {
    this.enablePrinter = false;
  }

  /**
   * Only for ut
   */
  isEnabled(): boolean {
    return this.enablePrinter;
  }

  constructor(outputPath: string = '') {
    this.outputPath = outputPath;
  }

  setOutputPath(outputPath: string | undefined): void {
    this.outputPath = outputPath;
  }

  print(message: string): void {
    if (this.outputPath && this.outputPath !== '') {
      fs.appendFileSync(`${this.outputPath}`, message + '\n');
    } else {
      console.log(message);
    }
  }

  outputData(): void {
    const eventData = this.getCurrentEventData();
    this.print(eventData);
  }

  // Only used for ut
  getOutputPath(): string {
    return this.outputPath;
  }
}

export class TimeTracker extends BasePrinter {
  private eventStack: Map<string, TimeAndMemInfo> = new Map<string, TimeAndMemInfo>();
  private filesTimeSum: number = 0;
  private maxTimeUsage = 0;
  private maxTimeFile = '';
  private maxMemoryUsage: number = 0;
  private maxMemoryFile: string = '';

  startEvent(eventName: string, timeSumPrinter?: TimeSumPrinter, currentFile?: string): void {
    this.eventStack.set(eventName, {start: Date.now(), duration: 0, startMemory: process.memoryUsage().heapUsed,
      endMemory: 0, memoryUsage: 0, filePath: currentFile});
    timeSumPrinter?.addEventDuration(eventName, 0);
  }

  endEvent(eventName: string, timeSumPrinter?: TimeSumPrinter, isFilesPrinter?: boolean, triggerSingleFilePrinter?: boolean): void {
    if (!this.eventStack.get(eventName)) {
      throw new Error(`Event "${eventName}" not started`);
    }

    const eventStartTime = this.eventStack.get(eventName).start;
    const duration = (Date.now() - eventStartTime) / MILLISECOND_TO_SECOND;
    const eventEndMemory = process.memoryUsage().heapUsed;
    const eventStartMemory = this.eventStack.get(eventName).startMemory;
    const memoryUsage = eventEndMemory - eventStartMemory;

    if (isFilesPrinter) {
      this.filesTimeSum += duration;

      if (duration > this.maxTimeUsage) {
        this.maxTimeUsage = duration;
        this.maxTimeFile = eventName;
      }

      if (eventStartMemory > this.maxMemoryUsage) {
        this.maxMemoryUsage = eventStartMemory;
        this.maxMemoryFile = eventName;
      }

      if (eventEndMemory > this.maxMemoryUsage) {
        this.maxMemoryUsage = eventEndMemory;
        this.maxMemoryFile = eventName;
      }
    }

    this.eventStack.get(eventName).duration = duration;
    this.eventStack.get(eventName).endMemory = eventEndMemory;
    this.eventStack.get(eventName).memoryUsage = memoryUsage;

    timeSumPrinter?.addEventDuration(eventName, duration);

    // Output data of singleFilePrinter if it is enabled and triggered
    if (triggerSingleFilePrinter && this.enablePrinter) {
      this.outputData();
    }

    // Output data of filesPrinter if it is enabled and triggered
    if ((eventName === EventList.ALL_FILES_OBFUSCATION)) {
      this.eventStack.get(eventName).duration = this.filesTimeSum;
      if (this.enablePrinter) {
        this.outputData();
      }
      const totalTimeUsage = this.getTotalTime();
      const maxTimeUsage = this.maxTimeUsage.toFixed(SIG_FIGS);
      const maxMemoryUsage = (this.maxMemoryUsage / BYTE_TO_MB).toFixed(SIG_FIGS);
      this.print(`Obfuscation time cost: ${totalTimeUsage} s`);
      this.print(`Max time cost of single file: ${this.maxTimeFile}: ${maxTimeUsage} s`);
      this.print(`Max memory usage of single file: ${this.maxMemoryFile}: ${maxMemoryUsage}MB\n`);
    }
  }

  getCurrentEventData(): string {
    let eventData = '';
    for (const eventName of this.eventStack.keys()) {
      if (eventName === EventList.OBFUSCATION_INITIALIZATION) {
        const totalTimeUsage = this.getTotalTime();
        eventData += `Obfuscation time cost: ${totalTimeUsage} s\n`;
      }
      let depth = eventList.get(eventName) ?? DEFAULT_DEPTH;
      let eventInfo = this.eventStack.get(eventName);
      const duration = eventInfo.duration;
      const startMemory = eventInfo.startMemory / BYTE_TO_MB;
      const endMemory = eventInfo.endMemory / BYTE_TO_MB;
      const memoryUsage = eventInfo.memoryUsage / BYTE_TO_MB;
      if (eventInfo.filePath) {
        eventData += eventInfo.filePath + `\n`;
      }
      eventData += this.formatEvent(eventName, duration, startMemory, endMemory, memoryUsage, depth);
    }
    return eventData;
  }

  private formatEvent(eventName: string, duration: number, startMemory: number, endMemory: number,
    memoryUsage: number, depth: number): string {
    const indent = INDENT.repeat(depth);
    const formattedDuration = duration.toFixed(SIG_FIGS) + ' s';
    const formatttedStartMemory = startMemory.toFixed(SIG_FIGS) + 'MB';
    const formatttedEndMemory = endMemory.toFixed(SIG_FIGS) + 'MB';
    const formatttedMemoryUsage = memoryUsage.toFixed(SIG_FIGS) + 'MB';
    return `${indent}${eventName}: timeCost:${formattedDuration} startMemory:${formatttedStartMemory} ` + 
    `endMemory:${formatttedEndMemory} memoryUsage:${formatttedMemoryUsage}\n`;
  }

  private getTotalTime(): string {
    let totalTime = (this.eventStack.get(EventList.OBFUSCATION_INITIALIZATION)?.duration ?? 0) +
    (this.eventStack.get(EventList.SCAN_SOURCEFILES)?.duration ?? 0) +
    (this.eventStack.get(EventList.ALL_FILES_OBFUSCATION)?.duration ?? 0);
    return totalTime.toFixed(SIG_FIGS);
  }

  // Only used for ut
  getEventStack(): Map<string, TimeAndMemInfo> {
    return this.eventStack;
  }

  getFilesTimeSum(): number {
    return this.filesTimeSum;
  }

  getMaxTimeUsage(): number {
    return this.maxTimeUsage;
  }

  getMaxTimeFile(): string {
    return this.maxTimeFile;
  }

  getMaxMemoryUsage(): number {
    return this.maxMemoryUsage;
  }

  getMaxMemoryFile(): string {
    return this.maxMemoryFile;
  }
}

export class TimeSumPrinter extends BasePrinter {
  private eventSum: Map<string, number> = new Map<string, number>();

  addEventDuration(eventName: string, duration: number): void {
    const currentValue = this.eventSum.get(eventName) ?? 0;
    this.eventSum.set(eventName, currentValue + duration);
  }

  summarizeEventDuration(): void {
    if (this.enablePrinter) {
      const eventData = this.getCurrentEventData();
      this.print(eventData);
    }
  }

  getCurrentEventData(): string {
    let eventData = '';
    for (const eventName of this.eventSum.keys()) {
      let depth = eventList.get(eventName) ?? 0;
      const duration = this.eventSum.get(eventName);
      eventData += this.formatEvent(eventName, duration, depth);
    }
    return eventData;
  }

  private formatEvent(eventName: string, duration: number, depth: number): string {
    const indent = INDENT.repeat(depth);
    const formattedDuration = duration.toFixed(SIG_FIGS) + ' s';
    return `${indent}${eventName}: ${formattedDuration}\n`;
  }

  getEventSum(): Map<string, number> {
    return this.eventSum;
  }
}

/**
 * Initialize performance printer
 */
export function initPerformancePrinter(mCustomProfiles: IOptions): void {
  const printerConfig: IPrinterOption = mCustomProfiles.mPerformancePrinter;

  // If no performance printer configuration is provided, disable the printer and return.
  if (!printerConfig) {
    blockPrinter();
    return;
  }

  // Disable performance printer if no specific printer types (files, single file, or summary) are enabled.
  const isPrinterDisabled = !(
    printerConfig.mFilesPrinter ||
    printerConfig.mSingleFilePrinter ||
    printerConfig.mSumPrinter
  );
  
  if (isPrinterDisabled) {
    blockPrinter();
    return;
  }

  const outputPath: string = printerConfig.mOutputPath;

  // Helper function to configure or disable a printer.
  const configurePrinter = (printer: TimeTracker | TimeSumPrinter, isEnabled: boolean): void => {
    if (!isEnabled) {
      printer?.disablePrinter();
      return;
    }
    printer?.setOutputPath(outputPath);
  };

  // Setup the individual printers based on configuration.
  configurePrinter(performancePrinter.filesPrinter, printerConfig.mFilesPrinter);
  configurePrinter(performancePrinter.singleFilePrinter, printerConfig.mSingleFilePrinter);
  configurePrinter(performancePrinter.timeSumPrinter, printerConfig.mSumPrinter);
}

/**
 * Disable performance printer
 */
export function blockPrinter(): void {
  performancePrinter.filesPrinter = undefined;
  performancePrinter.singleFilePrinter = undefined;
  performancePrinter.timeSumPrinter = undefined;
}

/**
 * Start recording singleFilePrinter event
 */
export function startSingleFileEvent(eventName: string, timeSumPrinter?: TimeSumPrinter, currentFile?: string): void {
  // The second conditional statement ensures that when the input eventName is a path, the current statement is executed
  if (eventListValues.has(eventName) || !eventListTimeAndMemValues.has(eventName)) {
    performancePrinter.singleFilePrinter?.startEvent(eventName, timeSumPrinter, currentFile);
  }
  performanceTimeAndMemPrinter.singleFilePrinter?.startEvent(eventName);
}

/**
 * End recording singleFilePrinter event
 */
export function endSingleFileEvent(
  eventName: string,
  timeSumPrinter?: TimeSumPrinter,
  isFilesPrinter?: boolean,
  triggerSingleFilePrinter?: boolean,
): void {
  if (eventListValues.has(eventName) || !eventListTimeAndMemValues.has(eventName)) {
    performancePrinter.singleFilePrinter?.endEvent(eventName, timeSumPrinter, isFilesPrinter, triggerSingleFilePrinter);
  }
  performanceTimeAndMemPrinter.singleFilePrinter?.endEvent(eventName);
}

/**
 * Start recording filesPrinter event
 */
export function startFilesEvent(eventName: string, timeSumPrinter?: TimeSumPrinter, currentFile?: string): void {
  if (eventListValues.has(eventName) || !eventListTimeAndMemValues.has(eventName)) {
    performancePrinter.filesPrinter?.startEvent(eventName, timeSumPrinter, currentFile);
  }
  performanceTimeAndMemPrinter.filesPrinter?.startEvent(eventName);
}

/**
 * End recording filesPrinter event
 */
export function endFilesEvent(
  eventName: string,
  timeSumPrinter?: TimeSumPrinter,
  isFilesPrinter?: boolean,
  triggerSingleFilePrinter?: boolean,
): void {
  if (eventListValues.has(eventName) || !eventListTimeAndMemValues.has(eventName)) {
    performancePrinter.filesPrinter?.endEvent(eventName, timeSumPrinter, isFilesPrinter, triggerSingleFilePrinter);
  }
  performanceTimeAndMemPrinter.filesPrinter?.endEvent(eventName);
}

/**
 * Print input info for timeSumPrinter
 */
export function printTimeSumInfo(info: string): void {
  performancePrinter.timeSumPrinter?.print(info);
}

/**
 * Print data of timeSumPrinter
 */
export function printTimeSumData(): void {
  performancePrinter.timeSumPrinter?.summarizeEventDuration();
}