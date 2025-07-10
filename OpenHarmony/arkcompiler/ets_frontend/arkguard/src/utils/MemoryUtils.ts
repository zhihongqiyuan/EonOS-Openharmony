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

export class MemoryUtils {
  private static baseMemorySize: number | undefined = undefined; 
  private static allowGC: boolean = typeof global.gc === 'function';
  private static MIN_GC_THRESHOLD: number = 64 * 1024 * 1024; // 64MB
  private static memoryGCThreshold: number = MemoryUtils.MIN_GC_THRESHOLD;
  public static GC_THRESHOLD_RATIO: number = 0.3;
  
  /**
   * Try garbage collection if obfuscaction starts or the memory usage exceeds MEMORY_BASELINE.
   */
  public static tryGC(): void {
    if (!MemoryUtils.allowGC) {
      return;
    }
  
    const currentMemory = process.memoryUsage().heapUsed;
    if (MemoryUtils.baseMemorySize === undefined || (currentMemory - MemoryUtils.baseMemorySize > MemoryUtils.memoryGCThreshold)) {
      global.gc();
      MemoryUtils.updateBaseMemory();
      return;
    }

    if (MemoryUtils.baseMemorySize > currentMemory) {
      MemoryUtils.updateBaseMemory(currentMemory);
      return;
    }
  }

  // For ut only
  public static setGC(allowGC: boolean): void {
    MemoryUtils.allowGC = allowGC;
  }

  // For ut only
  public static getBaseMemorySize(): number {
    return MemoryUtils.baseMemorySize;
  }

  // For ut only
  public static setBaseMemorySize(baseMemorySize: number | undefined): void {
    MemoryUtils.baseMemorySize = baseMemorySize;
  }

  // For ut only
  public static setMinGCThreshold(threshold: number): void {
    MemoryUtils.MIN_GC_THRESHOLD = threshold;
  }

  // For ut only
  public static getMinGCThreshold(): number {
    return MemoryUtils.MIN_GC_THRESHOLD;
  }

  // For ut only
  public static getGCThreshold(): number {
    return MemoryUtils.memoryGCThreshold;
  }

  public static updateBaseMemory(currentMemory?: number): void {
    currentMemory = currentMemory ?? process.memoryUsage().heapUsed;
    const targetGCThreshold: number = currentMemory * MemoryUtils.GC_THRESHOLD_RATIO;
    MemoryUtils.memoryGCThreshold = Math.max(targetGCThreshold, MemoryUtils.MIN_GC_THRESHOLD);
    MemoryUtils.baseMemorySize = currentMemory;
  }
}