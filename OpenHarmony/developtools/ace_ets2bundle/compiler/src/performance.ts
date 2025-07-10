/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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
import * as ts from 'typescript';

export interface CompileEvent {
  createSubEvent?: (eventName: string) => CompileEvent;
  createEvent: (eventName: string) => CompileEvent;
  startAsyncEvent?: () => void;
  start: () => void;
  stopAsyncEvent?: () => void;
  stop: () => void;
  setTotalTime: (time: number) => void;
}

interface Event {
  // id: Unique identifier of the event.
  id: string;
  // name: The name of the event. Duplicates exist.
  name: string;
  startTime: number;
  endTime: number;
  parentEvent: string;
  parentId: string;
  duration: number;
}

export enum AnalyzeMode {
  DEFAULT,
  VERBOSE,
  FALSE,
  TRACE
}

export enum ExternalEventType {
  TSC,
  ES2ABC
}

let asyncEvents: Map<string, CompileEvent> = new Map();

export function isNeedPerformanceDotting(projectConfig: Object): boolean {
  return projectConfig.perf === AnalyzeMode.TRACE || projectConfig.perf === AnalyzeMode.VERBOSE;
}

export function getHookEventFactory(share: Object, pluginName: string, hookName: string): CompileEvent | undefined {
  if (typeof share.getHookEventFactory === 'function' && isNeedPerformanceDotting(share.projectConfig)) {
    return share.getHookEventFactory(pluginName, hookName);
  } else {
    return undefined;
  }
}

export function createAndStartEvent(eventOrEventFactory: CompileEvent | undefined, eventName: string,
  isAsyncEvent = false): CompileEvent | undefined {
  if (eventOrEventFactory === undefined || eventOrEventFactory === null) {
    return undefined;
  }
  let event: CompileEvent;
  if (isSubEvent(eventOrEventFactory)) {
    event = eventOrEventFactory.createSubEvent(eventName);
  } else {
    event = eventOrEventFactory.createEvent(eventName);
  }
  if (isAsyncEvent) {
    event.startAsyncEvent();
  } else {
    event.start();
  }
  return event;
}

function isSubEvent(event: CompileEvent): boolean {
  return typeof event.createSubEvent === 'function';
}

export function stopEvent(event: CompileEvent | undefined, isAsyncEvent: boolean = false): void {
  if (event === undefined || event === null) {
    return;
  }
  if (isAsyncEvent) {
    event.stopAsyncEvent();
  } else {
    event.stop();
  }
}

export function processExternalEvents(projectConfig: Object, eventType: number, externalEventsInfo: Object): void {
  if (!isNeedPerformanceDotting(projectConfig)) {
    return;
  }
  const isTsc: boolean = isTscEvents(eventType);
  const { parentEvent, filePath } = externalEventsInfo;
  let events: Event[] = isTsc ? ts.PerformanceDotting.getEventData() :
    (fs.existsSync(filePath) ? JSON.parse(fs.readFileSync(filePath, 'utf-8')): []);
  if (events && events.length) {
    events = events.sort((a, b) => a.startTime - b.startTime);
    asyncEvents.set(isTsc ? events[0].parentId : events[0].parentEvent, parentEvent);
    for (const event of events) {
      setTotalTime(event, isTsc);
    }
  }
  asyncEvents.clear();
}

function isTscEvents(eventType: number): boolean {
  return eventType === ExternalEventType.TSC;
}

function setTotalTime(event: Event, isTsc: boolean): void {
  const parentEvent: CompileEvent | undefined = getCurrentAsyncEvent(isTsc ? event.parentId : event.parentEvent);
  if (parentEvent) {
    const subEvent: CompileEvent = parentEvent.createSubEvent(event.name);
    subEvent.start();
    subEvent.stop();
    subEvent.setTotalTime(event.duration);
    asyncEvents.set(isTsc ? event.id : event.name, subEvent)
  }
}

function getCurrentAsyncEvent(eventName): CompileEvent | undefined {
  if (asyncEvents && asyncEvents.has(eventName)) {
    return asyncEvents.get(eventName);
  }
  return undefined;
}

export function cleanUpAsyncEvents(): void {
  asyncEvents.clear();
}