/*
* Copyright (C) 2022 Huawei Device Co., Ltd.
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

/**
 * @file
 * @kit PerformanceAnalysisKit
 */

/**
 * Provide interfaces related to debugger access and obtaining CPU,
 * memory and other virtual machine information during runtime for JS programs
 *
 * @namespace hidebug
 * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
 * @since 8
 */

/**
 * This module provides multiple methods for debugging and profiling applications. With these methods, you can obtain
 * memory, CPU, GPU, and GC data, collect process trace and profiler data, and dump VM heap snapshots. Since most APIs
 * of this module are both performance-consuming and time-consuming, and are defined based on the HiDebug module,
 * you are advised to use these APIs only during the application debugging and profiling phases. If the APIs are
 * required in other scenarios, evaluate the impact of the APIs on application performance.
 *
 * @namespace hidebug
 * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
 * @atomicservice
 * @since 12
 */
declare namespace hidebug {
  /**
   * Obtains the total number of bytes occupied by the total space (the sum of uordblks and fordblks obtained from
   * mallinfo) held by a process, which is measured by the memory allocator.
   *
   * @returns { bigint } Returns the total number of bytes occupied by the total space.
   * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
   * @since 8
   */
  function getNativeHeapSize(): bigint;

  /**
   * Obtains the total number of bytes occupied by the total allocated space (uordblks obtained from mallinfo) held by
   * a process, which is measured by the memory allocator.
   * @returns { bigint } Returns the total number of bytes occupied by the total allocated space.
   * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
   * @since 8
   */
  function getNativeHeapAllocatedSize(): bigint;

  /**
   * Obtains the total number of bytes occupied by the total free space (fordblks obtained from mallinfo)
   * held by a process, which is measured by the memory allocator.
   *
   * @returns { bigint } Returns the size of the memory occupied by the free normal blocks held by the process, in bytes.
   * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
   * @since 8
   */
  function getNativeHeapFreeSize(): bigint;

  /**
   * Obtains the virtual set size used by the application process. This API is implemented by multiplying the value of
   * size (number of memory pages) in the /proc/{pid}/statm node by the page size (4 KB per page).
   *
   * @returns { bigint } Returns the virtual set size used by the application process, in KB.
   * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
   * @since 11
   */
  function getVss(): bigint;

  /**
   * Obtains the size of the physical memory actually used by the application process. This API is implemented by
   * summing up the values of **Pss** and **SwapPss** in the /proc/{pid}/smaps_rollup** node.
   *
   * @returns { bigint } Returns the size of the physical memory actually used by the application process, in KB.
   * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
   * @since 8
   */
  function getPss(): bigint;

  /**
   * Obtains the size of the shared dirty memory of a process. This API is implemented by reading the value of
   * Shared_Dirty in the /proc/{pid}/smaps_rollup node.
   *
   * @returns { bigint } Returns the size of the shared dirty memory of the process, in KB.
   * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
   * @since 8
   */
  function getSharedDirty(): bigint;

  /**
   * Obtains the size of the private dirty memory of a process. This API is implemented by reading the value of
   * Private_Dirty in the /proc/{pid}/smaps_rollup node.
   *
   * @returns { bigint } Returns the size of the private dirty memory of the process, in KB.
   * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
   * @since 9
   */
  function getPrivateDirty(): bigint;

  /**
   * Obtains the CPU usage of a process.
   *
   * @returns { number } Returns the CPU usage of the process.
   * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
   * @since 9
   */
  function getCpuUsage(): number;

  /**
   *
   * Starts the VM profiling method. startProfiling(filename: string) and stopProfiling() are called in pairs.
   * startProfiling(filename: string) always occurs before stopProfiling(). You are advised not to call either of these
   * methods repeatedly. Otherwise, an exception may occur. The generated file is in the files folder under the
   * application directory. Such as "/data/accounts/account_0/appdata/[package name]/files/cpuprofiler-xxx.json"
   *
   * @param { string } filename - User-defined file name of the sampling data. The .json file is generated
   * in the files directory of the application based on the specified file name.
   * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.hidebug/hidebug.startJsCpuProfiling
   */
  function startProfiling(filename: string): void;

  /**
   * Stops the VM profiling method. stopProfiling() and startProfiling(filename: string) are called in pairs.
   * startProfiling(filename: string) always occurs before stopProfiling(). You are advised not to call either of these
   * methods repeatedly. Otherwise, an exception may occur. It takes effect only when the CPU profiler is turned on.
   *
   * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.hidebug/hidebug.stopJsCpuProfiling
   */
  function stopProfiling(): void;

  /**
   * Exports the VM heap data and generates a filename.heapsnapshot file.
   * The input parameter is a user-defined file name, excluding the file suffix.
   * The generated file is in the files folder under the application directory.
   * Such as "/data/accounts/account_0/appdata/[package name]/files/xxx.heapsnapshot".
   *
   * @param { string } filename - Indicates the user-defined file name, excluding the file suffix.
   * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.hidebug/hidebug.dumpJsHeapData
   */
  function dumpHeapData(filename: string): void;

  /**
   * Starts the VM profiling method. startJsCpuProfiling(filename: string) and stopJsCpuProfiling() are called in pairs.
   * startJsCpuProfiling(filename: string) always occurs before stopJsCpuProfiling(). You are advised not to call either
   * of these methods repeatedly. Otherwise, an exception may occur.
   *
   * @param { string } filename - User-defined heap file name. The .heapsnapshot file is generated in the files
   * directory of the application based on the specified file name.
   * @throws {BusinessError} 401 - the parameter check failed, Parameter type error
   * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
   * @since 9
   */
  function startJsCpuProfiling(filename: string): void;

  /**
   * Stops the VM profiling method. stopJsCpuProfiling() and startJsCpuProfiling(filename: string) are called in pairs.
   * startJsCpuProfiling() always occurs before stopJsCpuProfiling(). You are advised not to call either of these
   * methods repeatedly. Otherwise, an exception may occur. It takes effect only when the CPU profiler is turned on
   *
   * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
   * @since 9
   */
  function stopJsCpuProfiling(): void;

  /**
   * Exports the heap data.
   * The input parameter is a user-defined file name, excluding the file suffix.
   * The generated file is in the files folder under the application directory.
   *
   * @param { string } filename - User-defined file name of the sampling data. The .heapsnapshot file is generated
   * in the files directory of the application based on the specified file name.
   * @throws {BusinessError} 401 - the parameter check failed, Parameter type error.
   * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
   * @since 9
   */
  function dumpJsHeapData(filename: string): void;

  /**
   * Obtains system service information.
   * It need dump permission.
   * This API can be called only by system application.
   *
   * @permission ohos.permission.DUMP
   * @param { number } serviceid - Obtains the system service information based on the specified service ID.
   * @param { number } fd - File descriptor to which data is written by the API.
   * @param { Array<string> } args - Parameter list of the Dump API of the system service.
   * @throws {BusinessError} 401 - the parameter check failed,Possible causes:
   *                               1.the parameter type error
   *                               2.the args parameter is not string array.
   * @throws {BusinessError} 11400101 - ServiceId invalid. The system ability does not exist.
   * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
   * @since 9
   */
  function getServiceDump(serviceid: number, fd: number, args: Array<string>): void;

  /**
   * Obtains the CPU usage of the system.
   *
   * @returns { number } Returns the CPU usage of the system.
   * @throws { BusinessError } 11400104 - The status of the system CPU usage is abnormal.
   * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
   * @since 12
   */
  function getSystemCpuUsage(): number;

  /**
   * Describes the CPU usage of a thread.
   *
   * @interface ThreadCpuUsage
   * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
   * @since 12
   */
  interface ThreadCpuUsage {
    /**
     * Thread id
     *
     * @type { number }
     * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
     * @since 12
     */
    threadId: number;
    /**
     * CPU usage of the thread.
     *
     * @type { number }
     * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
     * @since 12
     */
    cpuUsage: number;
  }

  /**
   * Obtains the CPU usage of application threads.
   *
   * @returns { ThreadCpuUsage[] } Returns the CPU usage of all threads of the current application process.
   * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
   * @since 12
   */
  function getAppThreadCpuUsage(): ThreadCpuUsage[];

  /**
   * Describes the system memory information, including the total memory, free memory, and available memory.
   *
   * @interface SystemMemInfo
   * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
   * @since 12
   */
  interface SystemMemInfo {
    /**
     * Total memory of the system, in KB. The value of this parameter is obtained by reading the value of
     * MemTotal in the /proc/meminfo node.
     *
     * @type { bigint }
     * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
     * @since 12
     */
    totalMem: bigint;
    /**
     * Free memory of the system, in KB. The value of this parameter is obtained by reading the value of
     * MemFree in the /proc/meminfo node.
     *
     * @type { bigint }
     * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
     * @since 12
     */
    freeMem: bigint;
    /**
     * Available memory of the system, in KB. The value of this parameter is obtained by reading the value of
     * MemAvailable in the /proc/meminfo node.
     *
     * @type { bigint }
     * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
     * @since 12
     */
    availableMem: bigint;
  }

  /**
   * Obtains system memory information. This API is implemented by reading data from the /proc/meminfo node.
   *
   * @returns { SystemMemInfo } Returns the system memory information.
   * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
   * @since 12
   */
  function getSystemMemInfo(): SystemMemInfo;

  /**
   * Describes memory information of the application process.
   *
   * @interface NativeMemInfo
   * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
   * @since 12
   */
  interface NativeMemInfo {
    /**
     * Size of the occupied physical memory (including the proportionally allocated memory occupied by the shared
     * library), in KB. The value of this parameter is obtained by summing up the values of Pss and SwapPss in the
     * /proc/{pid}/smaps_rollup node.
     *
     * @type { bigint }
     * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
     * @since 12
     */
    pss: bigint;
    /**
     * Size of the occupied virtual memory (including the memory occupied by the shared library), in KB. The value of
     * this parameter is obtained by multiplying the value of size (number of memory pages) in the /proc/{pid}/statm
     * node by the page size (4 KB per page).
     *
     * @type { bigint }
     * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
     * @since 12
     */
    vss: bigint;
    /**
     * Size of the occupied physical memory (including the memory occupied by the shared library), in KB.
     * The value of this parameter is obtained by reading the value of Rss in the /proc/{pid}/smaps_rollup node.
     *
     * @type { bigint }
     * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
     * @since 12
     */
    rss: bigint;
    /**
     * Size of the shared dirty memory, in KB. The value of this parameter is obtained by reading the value of
     * Shared_Dirty in the /proc/{pid}/smaps_rollup node.
     *
     * @type { bigint }
     * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
     * @since 12
     */
    sharedDirty: bigint;
    /**
     * Size of the private dirty memory, in KB. The value of this parameter is obtained by reading the value of
     * Private_Dirty in the /proc/{pid}/smaps_rollup node.
     *
     * @type { bigint }
     * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
     * @since 12
     */
    privateDirty: bigint;
    /**
     * Size of the shared clean memory, in KB. The value of this parameter is obtained by reading the value of
     * Shared_Clean in the /proc/{pid}/smaps_rollup node.
     *
     * @type { bigint }
     * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
     * @since 12
     */
    sharedClean: bigint;
    /**
     * Size of the private clean memory, in KB. The value of this parameter is obtained by reading the value of
     * Private_Clean in the /proc/{pid}/smaps_rollup node.
     *
     * @type { bigint }
     * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
     * @since 12
     */
    privateClean: bigint;
  }

  /**
   * Obtains the memory information of the application process. This API is implemented by reading data from the
   * /proc/{pid}/smaps_rollup and /proc/{pid}/statm node.
   *
   * @returns { NativeMemInfo } Returns the memory information of the application process.
   * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
   * @since 12
   */
  function getAppNativeMemInfo(): NativeMemInfo;

  /**
   * Defines the memory limit of the application process.
   *
   * @interface MemoryLimit
   * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
   * @since 12
   */
  interface MemoryLimit {
    /**
     * Limit on the resident set size, in KB.
     *
     * @type { bigint }
     * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
     * @since 12
     */
    rssLimit: bigint;
    /**
     * Limit on the virtual memory size, in KB.
     *
     * @type { bigint }
     * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
     * @since 12
     */
    vssLimit: bigint;
    /**
     * Limit on the JS VM heap size of the calling thread, in KB.
     *
     * @type { bigint }
     * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
     * @since 12
     */
    vmHeapLimit: bigint;
    /**
     * Size limit of the JS heap memory of the process, in KB.
     *
     * @type { bigint }
     * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
     * @since 12
     */
    vmTotalHeapSize: bigint;
  }

  /**
   * Obtains the memory limit of an application process.
   *
   * @returns { MemoryLimit } Returns the memory limit of the application process.
   * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
   * @since 12
   */
  function getAppMemoryLimit(): MemoryLimit;

  /**
   * Describes the VM memory information.
   *
   * @interface VMMemoryInfo
   * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
   * @since 12
   */
  interface VMMemoryInfo {
    /**
     * Total heap size of the current VM, in KB.
     *
     * @type { bigint }
     * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
     * @since 12
     */
    totalHeap: bigint;
    /**
     * Heap size used by the current VM, in KB.
     *
     * @type { bigint }
     * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
     * @since 12
     */
    heapUsed: bigint;
    /**
     * Size of all array objects of the current VM, in KB.
     *
     * @type { bigint }
     * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
     * @since 12
     */
    allArraySize: bigint;
  }

  /**
   * Obtains VM memory information.
   *
   * @returns { VMMemoryInfo } Returns the VM memory information.
   * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
   * @since 12
   */
  function getAppVMMemoryInfo(): VMMemoryInfo;

  /**
   * Describes types of trace collection threads, including the main thread and all threads.
   *
   * @enum { number }
   * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
   * @since 12
   */
  enum TraceFlag {
    /**
     * The main thread of the application.
     *
     * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
     * @since 12
     */
    MAIN_THREAD = 1,
    /**
     * All threads of the application.
     *
     * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
     * @since 12
     */
    ALL_THREADS = 2
  }

  /**
   * Enumerates the tags used in trace collection.
   *
   * @namespace tags
   * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
   * @since 12
   */
  namespace tags {
    /**
     * Ability Manager tag. The corresponding HiTrace command is tagName:ability.
     *
     * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
     * @since 12
     */
    const ABILITY_MANAGER: number;
    /**
     * ArkUI development framework. The corresponding HiTrace command is tagName:ace.
     *
     * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
     * @since 12
     */
    const ARKUI: number;
    /**
     * JSVM VM. The corresponding HiTrace command is tagName:ark.
     *
     * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
     * @since 12
     */
    const ARK: number;
    /**
     * Bluetooth. The corresponding HiTrace command is tagName:bluetooth.
     *
     * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
     * @since 12
     */
    const BLUETOOTH: number;
    /**
     * Common library subsystem. The corresponding HiTrace command is tagName:commonlibrary.
     *
     * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
     * @since 12
     */
    const COMMON_LIBRARY: number;
    /**
     * Distributed hardware device management. The corresponding HiTrace command is tagName:devicemanager.
     *
     * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
     * @since 12
     */
    const DISTRIBUTED_HARDWARE_DEVICE_MANAGER: number;
    /**
     * Distributed audio. The corresponding HiTrace command is tagName:daudio.
     *
     * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
     * @since 12
     */
    const DISTRIBUTED_AUDIO: number;
    /**
     * Distributed camera. The corresponding HiTrace command is tagName:dcamera.
     *
     * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
     * @since 12
     */
    const DISTRIBUTED_CAMERA: number;
    /**
     * Distributed data management. The corresponding HiTrace command is tagName:distributeddatamgr.
     *
     * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
     * @since 12
     */
    const DISTRIBUTED_DATA: number;
    /**
     * Distributed hardware framework. The corresponding HiTrace command is tagName:dhfwk.
     *
     * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
     * @since 12
     */
    const DISTRIBUTED_HARDWARE_FRAMEWORK: number;
    /**
     * Distributed input. The corresponding HiTrace command is tagName:dinput.
     *
     * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
     * @since 12
     */
    const DISTRIBUTED_INPUT: number;
    /**
     * Distributed screen. The corresponding HiTrace command is tagName:dscreen.
     *
     * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
     * @since 12
     */
    const DISTRIBUTED_SCREEN: number;
    /**
     * Distributed scheduler. The corresponding HiTrace command is tagName:dsched.
     *
     * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
     * @since 12
     */
    const DISTRIBUTED_SCHEDULER: number;
    /**
     * FFRT task. The corresponding HiTrace command is tagName:ffrt.
     *
     * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
     * @since 12
     */
    const FFRT: number;
    /**
     * File management system. The corresponding HiTrace command is tagName:filemanagement.
     *
     * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
     * @since 12
     */
    const FILE_MANAGEMENT: number;
    /**
     * Global resource management. The corresponding HiTrace command is tagName:gresource.
     *
     * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
     * @since 12
     */
    const GLOBAL_RESOURCE_MANAGER: number;
    /**
     * Graphics module. The corresponding HiTrace command is tagName:graphic.
     *
     * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
     * @since 12
     */
    const GRAPHICS: number;
    /**
     * HDF subsystem. The corresponding HiTrace command is tagName:hdf.
     *
     * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
     * @since 12
     */
    const HDF: number;
    /**
     * MISC module. The corresponding HiTrace command is tagName:misc.
     *
     * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
     * @since 12
     */
    const MISC: number;
    /**
     * Multi-modal input module. The corresponding HiTrace command is tagName:multimodalinput.
     *
     * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
     * @since 12
     */
    const MULTIMODAL_INPUT: number;
    /**
     * Network. The corresponding HiTrace command is tagName:net.
     *
     * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
     * @since 12
     */
    const NET: number;
    /**
     * Notification module. The corresponding HiTrace command is tagName:notification.
     *
     * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
     * @since 12
     */
    const NOTIFICATION: number;
    /**
     * Nweb. The corresponding HiTrace command is tagName:nweb.
     *
     * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
     * @since 12
     */
    const NWEB: number;
    /**
     * OHOS. The corresponding HiTrace command is tagName:ohos.
     *
     * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
     * @since 12
     */
    const OHOS: number;
    /**
     * Power management. The corresponding HiTrace command is tagName:power.
     *
     * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
     * @since 12
     */
    const POWER_MANAGER: number;
    /**
     * RPC. The corresponding HiTrace command is tagName:rpc.
     *
     * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
     * @since 12
     */
    const RPC: number;
    /**
     * System capability management. The corresponding HiTrace command is tagName:samgr.
     *
     * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
     * @since 12
     */
    const SAMGR: number;
    /**
     * Window management. The corresponding HiTrace command is tagName:window.
     *
     * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
     * @since 12
     */
    const WINDOW_MANAGER: number;
    /**
     * Audio module. The corresponding HiTrace command is tagName:zaudio.
     *
     * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
     * @since 12
     */
    const AUDIO: number;
    /**
     * Camera module. The corresponding HiTrace command is tagName:zcamera.
     *
     * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
     * @since 12
     */
    const CAMERA: number;
    /**
     * Image module. The corresponding HiTrace command is tagName:zimage.
     *
     * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
     * @since 12
     */
    const IMAGE: number;
    /**
     * Media module. The corresponding HiTrace command is tagName:zmedia.
     *
     * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
     * @since 12
     */
    const MEDIA: number;
  }

  /**
   * Starts automatic trace collection in a specified scope. This API is a supplement to the HiTrace module.
   * The performance consumption during trace collection increases with the collection scope. Therefore, before
   * using this API, you are advised to run the hitrace command to capture trace logs and select the key scope
   * of trace collection to improve the API performance.
   *
   * @param { number[] } tags - Scope for trace collection. For details, see tags.
   * @param { TraceFlag } flag - For details, see TraceFlag.
   * @param { number } limitSize - Limit on the trace file size, in bytes. The maximum size of a single file is 500 MB.
   * @returns { string } Returns the path of the trace file.
   * @throws { BusinessError } 401 - Invalid argument, Possible causes:
   *                           1.The limit parameter is too small
   *                           2.The parameter is not within the enumeration type
   *                           3.The parameter type error or parameter order error
   * @throws { BusinessError } 11400102 - Capture trace already enabled.
   * @throws { BusinessError } 11400103 - No write permission on the file.
   * @throws { BusinessError } 11400104 - Abnormal trace status.
   * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
   * @since 12
   */
  function startAppTraceCapture(tags: number[], flag: TraceFlag, limitSize: number): string;

  /**
   * Stops application trace collection. Use startAppTraceCapture() to start collection before calling this API.
   * If this API is called before trace collection or it is repeatedly called, an exception will occur.
   *
   * @throws { BusinessError } 11400104 - The status of the trace is abnormal.
   * @throws { BusinessError } 11400105 - No capture trace running.
   * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
   * @since 12
   */
  function stopAppTraceCapture(): void;

  /**
   * Describes the key-value pair used to store GC statistics. This type does not support multi-thread operations.
   * If this type is operated by multiple threads at the same time in an application, use a lock for it.
   *
   * @typedef { Record<string, number> } GcStats
   * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
   * @since 12
   */
  type GcStats = Record<string, number>;

  /**
   * Obtains all system GC statistics.
   *
   * @returns { GcStats } Returns the system GC statistics.
   * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
   * @since 12
   */
  function getVMRuntimeStats(): GcStats;

  /**
   * Obtains the specified system GC statistics based on parameters.
   *
   * @param { string } item - statistical item.
   * @returns { number } Returns the item of the GC statistics to be obtained.
   * @throws { BusinessError } 401 - Possible causes:
   *                           1. Invalid parameter, a string parameter required.
   *                           2. Invalid parameter, unknown property.
   * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
   * @since 12
   */
  function getVMRuntimeStat(item: string): number;

  /**
   * Sets the number of FDs, number of threads, JS memory, or native memory limit of the application.
   * 
   * @param { string } type - resource type. It could be pss_memory、js_heap、fd、or thread.
   * @param { number } value - For different resource type, values could have different meaning:
   *                           1.For pss_memory, it means the baseline PSS memory size for the application,
   *                             system memory control will be triggered if exceed the value too much.
   *                           2.For js_heap, it means the percentage of the used JS heap memory to the maximum limit exceed
   *                            which heap dump will be triggered if enableDebugLog set as true, it can be set between 85 and 95.
   *                           3.For fd, it means the maximum fd number can be opened.
   *                           4.For thread, it means the maximum thread number can be created.
   * @param { boolean } enableDebugLog - Whether to enable external debug log. Default is false, pls make sure set
   *                                      it as true only in gray release because collecting debug log will cost too much cpu or memory.
   * @throws { BusinessError } 401 - Invalid argument, Possible causes:
   *                           1.The limit parameter is too small
   *                           2.The parameter is not in the specified type
   *                           3.The parameter type error or parameter order error
   * @throws { BusinessError } 11400104 - Set limit failed due to remote exception
   * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
   * @atomicservice
   * @since 12
   */
  function setAppResourceLimit(type: string, value: number, enableDebugLog: boolean): void;

  /**
   * Obtains the debugging state of an application process. If the Ark or native layer of the application process is in
   * debugging state, true is returned. Otherwise, false is returned.
   *
   * @returns { boolean } true if the application is in the debugging state.
   * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
   * @since 12
   */
  function isDebugState(): boolean;

  /**
   * Obtains the size of the GPU memory. This API uses a promise to return the result.
   *
   * @returns { Promise<number> } Returns the size of the GPU memory, in KB.
   * @throws { BusinessError } 11400104 - Failed to get the application memory due to a remote exception.
   * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
   * @atomicservice
   * @since 14
   */
  function getGraphicsMemory(): Promise<number>;

  /**
   * Obtains the size of the GPU memory synchronously.
   *
   * @returns { number } Returns the size of the GPU memory, in KB.
   * @throws { BusinessError } 11400104 - Failed to get the application memory due to a remote exception.
   * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
   * @atomicservice
   * @since 14
   */
  function getGraphicsMemorySync(): number;

  /**
   * Dumps the original heap snapshot of the VM for the current thread. The API uses a promise to return the path of the
   * .rawheap file. You can use rawheap-translator to convert the generated file into a .heapsnapshot file for parsing.
   * The generated file will be stored in a folder within the application directory. However, since this file is usually
   * large, the system imposes restrictions on the frequency and number of calls to this function. Consequently, you
   * might fail to obtain the dump file due to quota limitations. These failures will persist until the quota is
   * regularly refreshed by the system. Therefore, it is advisable to delete the file immediately after you have
   * finished processing it. Moreover, it is recommended that you use this function in the gray - release version.
   *
   * @param { boolean } needGC - Whether GC is required when a heap snapshot is dumped. The default value is true.
   * If this parameter is not specified, GC is triggered before dumping.
   * @returns { Promise<string> } Returns the path of the generated snapshot file.
   * @throws { BusinessError } 11400106 - Quota exceeded.
   * @throws { BusinessError } 11400107 - Fork operation failed.
   * @throws { BusinessError } 11400108 - Failed to wait for the child process to finish.
   * @throws { BusinessError } 11400109 - Timeout while waiting for the child process to finish.
   * @throws { BusinessError } 11400110 - Disk remaining space too low.
   * @throws { BusinessError } 11400111 - Napi interface call exception.
   * @throws { BusinessError } 11400112 - Repeated data dump.
   * @throws { BusinessError } 11400113 - Failed to create dump file.
   * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
   * @atomicservice
   * @since 18
   */
  function dumpJsRawHeapData(needGC?: boolean): Promise<string>;

  /**
   * GwpAsan Options.
   *
   * @interface GwpAsanOptions
   * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
   * @since 20
   */
  interface GwpAsanOptions {
    /**
     * Control whether to enable GWP-ASan every time
     *
     * @type { ?boolean }
     * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
     * @since 20
     */
    alwaysEnabled?: boolean;
    /**
     * sample rate of GWP-ASAN
     *
     * @type { ?number }
     * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
     * @since 20
     */
    sampleRate?: number;
    /**
     * the max simutaneous allocations of GWP-ASAN
     *
     * @type { ?number }
     * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
     * @since 20
     */
    maxSimutaneousAllocations?: number;
  }

  /**
   * Enable the GWP-ASAN grayscale of your application.
   * @param { GwpAsanOptions } [options] - The options of GWP-ASAN grayscale.
   * @param { number } [duration] - The duration days of GWP-ASAN grayscale.
   * @throws { BusinessError } 11400114 - The number of GWP-ASAN applications of this device overflowed after last boot.
   * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
   * @since 20
   */
  function enableGwpAsanGrayscale(options?: GwpAsanOptions, duration?: number): void;

  /**
   * Disable the GWP-ASAN grayscale of your application.
   *
   * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
   * @since 20
   */
  function disableGwpAsanGrayscale(): void;

  /**
   * Obtain the remaining days of GWP-ASan grayscale for your application.
   *
   * @returns { number } The remaining days of GWP-ASan grayscale.
   * @syscap SystemCapability.HiviewDFX.HiProfiler.HiDebug
   * @since 20
   */
  function getGwpAsanGrayscaleState(): number;
}
export default hidebug;
