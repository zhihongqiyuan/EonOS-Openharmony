/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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
 * @kit BasicServicesKit
 */

import type { AsyncCallback, Callback } from './@ohos.base';
import type Context from './application/Context';

/**
 * System print
 *
 * @namespace print
 * @syscap SystemCapability.Print.PrintFramework
 * @since 10
 */
declare namespace print {

  /**
   * PrintTask provide event callback.
   * @interface PrintTask
   * @syscap SystemCapability.Print.PrintFramework
   * @since 10
   */
  interface PrintTask {
    /**
     * Register event callback when the current print task is in process.
     * @permission ohos.permission.PRINT
     * @param { 'block' } type - Indicates the print task has been blocked.
     * @param { Callback<void> } callback - The callback function for print task change event
     * @throws { BusinessError } 201 - the application does not have permission to call this function.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 10
     */
    on(type: 'block', callback: Callback<void>): void;

    /**
     * Register event callback when the current print task is in process.
     * @permission ohos.permission.PRINT
     * @param { 'succeed' } type - Indicates the print task succeed.
     * @param { Callback<void> } callback - The callback function for print task change event
     * @throws { BusinessError } 201 - the application does not have permission to call this function.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 10
     */
    on(type: 'succeed', callback: Callback<void>): void;

    /**
     * Register event callback when the current print task is in process.
     * @permission ohos.permission.PRINT
     * @param { 'fail' } type - Indicates the print task has completed with failure.
     * @param { Callback<void> } callback - The callback function for print task change event
     * @throws { BusinessError } 201 - the application does not have permission to call this function.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 10
     */
    on(type: 'fail', callback: Callback<void>): void;

    /**
     * Register event callback when the current print task is in process.
     * @permission ohos.permission.PRINT
     * @param { 'cancel' } type - Indicates the print task has been cancelled.
     * @param { Callback<void> } callback - The callback function for print task change event
     * @throws { BusinessError } 201 - the application does not have permission to call this function.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 10
     */
    on(type: 'cancel', callback: Callback<void>): void;

    /**
     * Unregister event callback when the current print task is in process.
     * @permission ohos.permission.PRINT
     * @param { 'block' } type - Indicates the print task has been blocked.
     * @param { Callback<void> } callback - The callback function for print task change event
     * @throws { BusinessError } 201 - the application does not have permission to call this function.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 10
     */
    off(type: 'block', callback?: Callback<void>): void;

    /**
     * Unregister event callback when the current print task is in process.
     * @permission ohos.permission.PRINT
     * @param { 'succeed' } type - Indicates the print task succeed.
     * @param { Callback<void> } callback - The callback function for print task change event
     * @throws { BusinessError } 201 - the application does not have permission to call this function.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 10
     */
    off(type: 'succeed', callback?: Callback<void>): void;

    /**
     * Unregister event callback when the current print task is in process.
     * @permission ohos.permission.PRINT
     * @param { 'fail' } type - Indicates the print task has completed with failure.
     * @param { Callback<void> } callback - The callback function for print task change event
     * @throws { BusinessError } 201 - the application does not have permission to call this function.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 10
     */
    off(type: 'fail', callback?: Callback<void>): void;

    /**
     * Unregister event callback when the current print task is in process.
     * @permission ohos.permission.PRINT
     * @param { 'cancel' } type - Indicates the print task has been cancelled.
     * @param { Callback<void> } callback - The callback function for print task change event
     * @throws { BusinessError } 201 - the application does not have permission to call this function.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 10
     */
    off(type: 'cancel', callback?: Callback<void>): void;
  }

  /**
   * Third-party application implement this interface to render files to be printed.
   * @interface PrintDocumentAdapter
   * @syscap SystemCapability.Print.PrintFramework
   * @since 11
   */
  interface PrintDocumentAdapter {

    /**
     * Implement this function to update the print file.
     * @permission ohos.permission.PRINT
     * @param { string } jobId - Indicates print job id.
     * @param { PrintAttributes } oldAttrs - Indicates old print attributes.
     * @param { PrintAttributes } newAttrs - Indicates new print attributes.
     * @param { number } fd - Indicates print file fd.
     * @param { function } writeResultCallback - Indicates this function should execute after the file is updated.
     * @throws { BusinessError } 201 - the application does not have permission to call this function.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 11
     */
    onStartLayoutWrite(jobId: string, oldAttrs: PrintAttributes, newAttrs: PrintAttributes, fd: number,
      writeResultCallback: (jobId: string, writeResult: PrintFileCreationState) => void): void;

    /**
     * Implement this function to listen job status change.
     * @permission ohos.permission.PRINT
     * @param { string } jobId - Indicates print job id.
     * @param { PrintDocumentAdapterState } state - Indicates job changes to this state.
     * @throws { BusinessError } 201 - the application does not have permission to call this function.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 11
     */
    onJobStateChanged(jobId: string, state: PrintDocumentAdapterState): void;
  }

  /**
   * Start new print task for App.
   * @permission ohos.permission.PRINT
   * @param { Array<string> } files - Indicates the filepath list to be printed. Only pdf and picture filetype are supported.
   * @param { AsyncCallback<PrintTask> } callback - The callback function for print task.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
   * @syscap SystemCapability.Print.PrintFramework
   * @since 10
   */
  function print(files: Array<string>, callback: AsyncCallback<PrintTask>): void;

  /**
   * Start new print task for App.
   * @permission ohos.permission.PRINT
   * @param { Array<string> } files - Indicates the filepath list to be printed. Only pdf and picture filetype are supported.
   * @returns { Promise<PrintTask> } the promise returned by the function.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
   * @syscap SystemCapability.Print.PrintFramework
   * @since 10
   */
  function print(files: Array<string>): Promise<PrintTask>;

  /**
   * Start new print task for App.
   * @permission ohos.permission.PRINT
   * @param { Array<string> } files - Indicates the filepath list to be printed. Only pdf and picture filetype are supported.
   * @param { Context } context - The ability context that initiates the call print request.
   * @param { AsyncCallback<PrintTask> } callback - The callback function for print task.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
   * @syscap SystemCapability.Print.PrintFramework
   * @since 11
   */
  function print(files: Array<string>, context: Context, callback: AsyncCallback<PrintTask>): void;

  /**
   * Start new print task for App.
   * @permission ohos.permission.PRINT
   * @param { Array<string> } files - Indicates the filepath list to be printed. Only pdf and picture filetype are supported.
   * @param { Context } context - The ability context that initiates the call print request.
   * @returns { Promise<PrintTask> } the promise returned by the function.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
   * @syscap SystemCapability.Print.PrintFramework
   * @since 11
   */
  function print(files: Array<string>, context: Context): Promise<PrintTask>;

  /**
   * Start new print task for App And the App need update print file.
   * @permission ohos.permission.PRINT
   * @param { string } jobName - Indicates print file Name.
   * @param { PrintDocumentAdapter } printAdapter - Indicates functions implemented by the cpp.
   * @param { PrintAttributes } printAttributes - Indicates print attributes.
   * @param { Context } context - The ability context that initiates the call print request.
   * @returns { Promise<PrintTask> } the promise returned by the function.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
   * @syscap SystemCapability.Print.PrintFramework
   * @since 11
   */
  function print(jobName: string, printAdapter: PrintDocumentAdapter, printAttributes: PrintAttributes,
    context: Context): Promise<PrintTask>;

  /**
   * defines print attributes.
   * @typedef PrintAttributes
   * @syscap SystemCapability.Print.PrintFramework
   * @since 11
   */
  interface PrintAttributes {
    /**
     * Copies of document list.
     * @type { ?number }
     * @syscap SystemCapability.Print.PrintFramework
     * @since 11
     */
    copyNumber?: number;

    /**
     * Range size to be printed.
     * @type { ?PrintPageRange }
     * @syscap SystemCapability.Print.PrintFramework
     * @since 11
     */
    pageRange?: PrintPageRange;

    /**
     * Page size.
     * @type { ?(PrintPageSize | PrintPageType) }
     * @syscap SystemCapability.Print.PrintFramework
     * @since 11
     */
    pageSize?: PrintPageSize | PrintPageType;

    /**
     * Print direction.
     * @type { ?PrintDirectionMode }
     * @syscap SystemCapability.Print.PrintFramework
     * @since 11
     */
    directionMode?: PrintDirectionMode;

    /**
     * Color mode.
     * @type { ?PrintColorMode }
     * @syscap SystemCapability.Print.PrintFramework
     * @since 11
     */
    colorMode?: PrintColorMode;

    /**
     * Duplex mode.
     * @type { ?PrintDuplexMode }
     * @syscap SystemCapability.Print.PrintFramework
     * @since 11
     */
    duplexMode?: PrintDuplexMode;
  }

  /**
   * defines print page range.
   * @typedef PrintPageRange
   * @syscap SystemCapability.Print.PrintFramework
   * @since 11
   */
  interface PrintPageRange {
    /**
     * Start page of sequence.
     * @type { ?number }
     * @syscap SystemCapability.Print.PrintFramework
     * @since 11
     */
    startPage?: number;

    /**
     * End page of sequence.
     * @type { ?number }
     * @syscap SystemCapability.Print.PrintFramework
     * @since 11
     */
    endPage?: number;

    /**
     * Discrete page of sequence.
     * @type { ?Array<number> }
     * @syscap SystemCapability.Print.PrintFramework
     * @since 11
     */
    pages?: Array<number>;
  }

  /**
   * defines print margin.
   * @typedef PrintMargin
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  interface PrintMargin {
    /**
     * Top margin.
     * @type { ?number }
     * @syscap SystemCapability.Print.PrintFramework
     * @systemapi
     * @since 10
     */
    top?: number;

    /**
     * Bottom margin.
     * @type { ?number }
     * @syscap SystemCapability.Print.PrintFramework
     * @systemapi
     * @since 10
     */
    bottom?: number;

    /**
     * Left margin.
     * @type { ?number }
     * @syscap SystemCapability.Print.PrintFramework
     * @systemapi
     * @since 10
     */
    left?: number;

    /**
     * Right margin.
     * @type { ?number }
     * @syscap SystemCapability.Print.PrintFramework
     * @systemapi
     * @since 10
     */
    right?: number;
  }

  /**
   * defines print range.
   * @typedef PrinterRange
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  interface PrinterRange {
    /**
     * Start page of sequence.
     * @type { ?number }
     * @syscap SystemCapability.Print.PrintFramework
     * @systemapi
     * @since 10
     */
    startPage?: number;

    /**
     * End page of sequence.
     * @type { ?number }
     * @syscap SystemCapability.Print.PrintFramework
     * @systemapi
     * @since 10
     */
    endPage?: number;

    /**
     * Discrete page of sequence.
     * @type { ?Array<number> }
     * @syscap SystemCapability.Print.PrintFramework
     * @systemapi
     * @since 10
     */
    pages?: Array<number>;
  }

  /**
   * defines print preview attribute.
   * @typedef PreviewAttribute
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  interface PreviewAttribute {
    /**
     * Preview page range.
     * @type { PrinterRange }
     * @syscap SystemCapability.Print.PrintFramework
     * @systemapi
     * @since 10
     */
    previewRange: PrinterRange;

    /**
     * Preview file result.
     * @type { ?number }
     * @syscap SystemCapability.Print.PrintFramework
     * @systemapi
     * @since 10
     */
    result?: number;
  }

  /**
   * defines print resolution.
   * @typedef PrintResolution
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  interface PrintResolution {
    /**
     * Resolution id.
     * @type { string }
     * @syscap SystemCapability.Print.PrintFramework
     * @systemapi
     * @since 10
     */
    id: string;

    /**
     * Horizontal DPI.
     * @type { number }
     * @syscap SystemCapability.Print.PrintFramework
     * @systemapi
     * @since 10
     */
    horizontalDpi: number;

    /**
     * Vertical DPI.
     * @type { number }
     * @syscap SystemCapability.Print.PrintFramework
     * @systemapi
     * @since 10
     */
    verticalDpi: number;
  }

  /**
   * defines print page size.
   * @typedef PrintPageSize
   * @syscap SystemCapability.Print.PrintFramework
   * @since 11
   */
  interface PrintPageSize {
    /**
     * Page size id.
     * @type { string }
     * @syscap SystemCapability.Print.PrintFramework
     * @since 11
     */
    id: string;

    /**
     * Page size name.
     * @type { string }
     * @syscap SystemCapability.Print.PrintFramework
     * @since 11
     */
    name: string;

    /**
     * Unit: millimeter width.
     * @type { number }
     * @syscap SystemCapability.Print.PrintFramework
     * @since 11
     */
    width: number;

    /**
     * Unit: millimeter height.
     * @type { number }
     * @syscap SystemCapability.Print.PrintFramework
     * @since 11
     */
    height: number;
  }

  /**
   * defines print capability.
   * @typedef PrinterCapability
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  interface PrinterCapability {
    /**
     * Color mode.
     * @type { number }
     * @syscap SystemCapability.Print.PrintFramework
     * @systemapi
     * @since 10
     */
    colorMode: number;

    /**
     * Duplex mode.
     * @type { number }
     * @syscap SystemCapability.Print.PrintFramework
     * @systemapi
     * @since 10
     */
    duplexMode: number;

    /**
     * The page size list supported by the printer.
     * @type { Array<PrintPageSize> }
     * @syscap SystemCapability.Print.PrintFramework
     * @systemapi
     * @since 10
     */
    pageSize: Array<PrintPageSize>;

    /**
     * The resolution list supported by the printer.
     * @type { ?Array<PrintResolution> }
     * @syscap SystemCapability.Print.PrintFramework
     * @systemapi
     * @since 10
     */
    resolution?: Array<PrintResolution>;

    /**
     * Min margin of printer.
     * @type { ?PrintMargin }
     * @syscap SystemCapability.Print.PrintFramework
     * @systemapi
     * @since 10
     */
    minMargin?: PrintMargin;

    /**
     * JSON object string.
     * @type { ?Object }
     * @syscap SystemCapability.Print.PrintFramework
     * @systemapi
     * @since 11
     */
    options?: Object;
  }

  /**
   * defines print info.
   * @typedef PrinterInfo
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  interface PrinterInfo {
    /**
     * Printer id.
     * @type { string }
     * @syscap SystemCapability.Print.PrintFramework
     * @systemapi
     * @since 10
     */
    printerId: string;

    /**
     * Printer name.
     * @type { string }
     * @syscap SystemCapability.Print.PrintFramework
     * @systemapi
     * @since 10
     */
    printerName: string;

    /**
     * Current printer state.
     * @type { PrinterState }
     * @syscap SystemCapability.Print.PrintFramework
     * @systemapi
     * @since 10
     */
    printerState: PrinterState;

    /**
     * Resource id of printer icon.
     * @type { ?number }
     * @syscap SystemCapability.Print.PrintFramework
     * @systemapi
     * @since 10
     */
    printerIcon?: number;

    /**
     * Printer description.
     * @type { ?string }
     * @syscap SystemCapability.Print.PrintFramework
     * @systemapi
     * @since 10
     */
    description?: string;

    /**
     * Printer capability.
     * @type { ?PrinterCapability }
     * @syscap SystemCapability.Print.PrintFramework
     * @systemapi
     * @since 10
     */
    capability?: PrinterCapability;

    /**
     * JSON object string.
     * @type { ?Object }
     * @syscap SystemCapability.Print.PrintFramework
     * @systemapi
     * @since 10
     */
    options?: Object;
  }

  /**
   * defines print job.
   * @typedef PrintJob
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  interface PrintJob {
    /**
     * Document fd list to be printed.
     * @type { Array<number> }
     * @syscap SystemCapability.Print.PrintFramework
     * @systemapi
     * @since 10
     */
    fdList: Array<number>;

    /**
     * Job id.
     * @type { string }
     * @syscap SystemCapability.Print.PrintFramework
     * @systemapi
     * @since 10
     */
    jobId: string;

    /**
     * Printer id to take charge of printing.
     * @type { string }
     * @syscap SystemCapability.Print.PrintFramework
     * @systemapi
     * @since 10
     */
    printerId: string;

    /**
     * Current print job state.
     * @type { PrintJobState }
     * @syscap SystemCapability.Print.PrintFramework
     * @systemapi
     * @since 10
     */
    jobState: PrintJobState;

    /**
     * Current print job sub state.
     * @type { PrintJobSubState }
     * @syscap SystemCapability.Print.PrintFramework
     * @systemapi
     * @since 11
     */
    jobSubstate: PrintJobSubState;

    /**
     * Copies of document list.
     * @type { number }
     * @syscap SystemCapability.Print.PrintFramework
     * @systemapi
     * @since 10
     */
    copyNumber: number;

    /**
     * Range size to be printed.
     * @type { PrinterRange }
     * @syscap SystemCapability.Print.PrintFramework
     * @systemapi
     * @since 10
     */
    pageRange: PrinterRange;

    /**
     * Sequential print.
     * @type { boolean }
     * @syscap SystemCapability.Print.PrintFramework
     * @systemapi
     * @since 10
     */
    isSequential: boolean;

    /**
     * The selected page size.
     * @type { PrintPageSize }
     * @syscap SystemCapability.Print.PrintFramework
     * @systemapi
     * @since 10
     */
    pageSize: PrintPageSize;

    /**
     * Vertical printing.
     * @type { boolean }
     * @syscap SystemCapability.Print.PrintFramework
     * @systemapi
     * @since 10
     */
    isLandscape: boolean;

    /**
     * Color mode.
     * @type { number }
     * @syscap SystemCapability.Print.PrintFramework
     * @systemapi
     * @since 10
     */
    colorMode: number;

    /**
     * Duplex mode.
     * @type { number }
     * @syscap SystemCapability.Print.PrintFramework
     * @systemapi
     * @since 10
     */
    duplexMode: number;

    /**
     * Current margin setting.
     * @type { ?PrintMargin }
     * @syscap SystemCapability.Print.PrintFramework
     * @systemapi
     * @since 10
     */
    margin?: PrintMargin;

    /**
     * Preview setting.
     * @type { ?PreviewAttribute }
     * @syscap SystemCapability.Print.PrintFramework
     * @systemapi
     * @since 10
     */
    preview?: PreviewAttribute;

    /**
     * JSON object string.
     * @type { ?Object }
     * @syscap SystemCapability.Print.PrintFramework
     * @systemapi
     * @since 10
     */
    options?: Object;
  }

  /**
   * Enumeration of Print Direction Mode.
   * @enum { number } PrintDirectionMode
   * @syscap SystemCapability.Print.PrintFramework
   * @since 11
   */
  enum PrintDirectionMode {
    /**
     * Automatically select direction.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 11
     */
    DIRECTION_MODE_AUTO = 0,

    /**
     * Print portrait.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 11
     */
    DIRECTION_MODE_PORTRAIT = 1,

    /**
     * Print landscape.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 11
     */
    DIRECTION_MODE_LANDSCAPE = 2,
  }

  /**
   * Enumeration of Print Color Mode.
   * @enum { number } PrintColorMode
   * @syscap SystemCapability.Print.PrintFramework
   * @since 11
   */
  enum PrintColorMode {
    /**
     * Print monochrome.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 11
     */
    COLOR_MODE_MONOCHROME = 0,

    /**
     * Color printing.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 11
     */
    COLOR_MODE_COLOR = 1,
  }

  /**
   * Enumeration of Print Duplex Mode.
   * @enum { number } PrintDuplexMode
   * @syscap SystemCapability.Print.PrintFramework
   * @since 11
   */
  enum PrintDuplexMode {
    /**
     * Single side printing.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 11
     */
    DUPLEX_MODE_NONE = 0,

    /**
     * Long-edge flip-up duplex printing.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 11
     */
    DUPLEX_MODE_LONG_EDGE = 1,

    /**
     * Short-edge flip-up duplex printing.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 11
     */
    DUPLEX_MODE_SHORT_EDGE = 2,
  }

  /**
   * Enumeration of Print Page Type.
   * @enum { number } PrintPageType
   * @syscap SystemCapability.Print.PrintFramework
   * @since 11
   */
  enum PrintPageType {
    /**
     * A3 page.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 11
     */
    PAGE_ISO_A3 = 0,

    /**
     * A4 page.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 11
     */
    PAGE_ISO_A4 = 1,

    /**
     * A5 page.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 11
     */
    PAGE_ISO_A5 = 2,

    /**
     * B5 page.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 11
     */
    PAGE_JIS_B5 = 3,

    /**
     * C5 page.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 11
     */
    PAGE_ISO_C5 = 4,

    /**
     * DL Envelope.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 11
     */
    PAGE_ISO_DL = 5,

    /**
     * Letter.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 11
     */
    PAGE_LETTER = 6,

    /**
     * Legal.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 11
     */
    PAGE_LEGAL = 7,

    /**
     * Photo 4x6.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 11
     */
    PAGE_PHOTO_4X6 = 8,

    /**
     * Photo 5x7.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 11
     */
    PAGE_PHOTO_5X7 = 9,

    /**
     * Envelope INT DL.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 11
     */
    PAGE_INT_DL_ENVELOPE = 10,

    /**
     * Tabloid B.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 11
     */
    PAGE_B_TABLOID = 11,
  }

  /**
   * Enumeration of Print Document Adapter State.
   * @enum { number } PrintDocumentAdapterState
   * @syscap SystemCapability.Print.PrintFramework
   * @since 11
   */
  enum PrintDocumentAdapterState {
    /**
     * Preview failed.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 11
     */
    PREVIEW_DESTROY = 0,

    /**
     * Print state is succeed.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 11
     */
    PRINT_TASK_SUCCEED = 1,

    /**
     * Print state is fail.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 11
     */
    PRINT_TASK_FAIL = 2,

    /**
     * Print state is cancel.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 11
     */
    PRINT_TASK_CANCEL = 3,

    /**
     * Print state is block.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 11
     */
    PRINT_TASK_BLOCK = 4,
  }

  /**
   * Enumeration of Print File Creation State.
   * @enum { number } PrintFileCreationState
   * @syscap SystemCapability.Print.PrintFramework
   * @since 11
   */
  enum PrintFileCreationState {
    /**
     * Print file created success.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 11
     */
    PRINT_FILE_CREATED = 0,

    /**
     * Print file created fail.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 11
     */
    PRINT_FILE_CREATION_FAILED = 1,

    /**
     * Print file created success but unrendered.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 11
     */
    PRINT_FILE_CREATED_UNRENDERED = 2,
  }

  /**
   * Enumeration of Printer State.
   * @enum { number } PrinterState
   * @syscap SystemCapability.Print.PrintFramework
   * @since 14
   */
  enum PrinterState {
    /**
     * New printers arrival.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    PRINTER_ADDED = 0,

    /**
     * Printer lost.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    PRINTER_REMOVED = 1,

    /**
     * Printer update.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    PRINTER_CAPABILITY_UPDATED = 2,

    /**
     * Printer has been connected.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    PRINTER_CONNECTED = 3,

    /**
     * Printer has been disconnected.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    PRINTER_DISCONNECTED = 4,

    /**
     * Printer is working.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    PRINTER_RUNNING = 5,
  }

  /**
   * Enumeration of  Print Job State.
   * @enum { number } PrintJobState
   * @syscap SystemCapability.Print.PrintFramework
   * @since 14
   */
  enum PrintJobState {
    /**
     * Initial state of print job.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    PRINT_JOB_PREPARE = 0,

    /**
     * Deliver print job to the printer.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    PRINT_JOB_QUEUED = 1,

    /**
     * Executing print job.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    PRINT_JOB_RUNNING = 2,

    /**
     * Print job has been blocked.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    PRINT_JOB_BLOCKED = 3,

    /**
     * Print job completed.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    PRINT_JOB_COMPLETED = 4,
  }

  /**
   * Enumeration of  Print Job Sub State.
   * @enum { number } PrintJobSubState
   * @syscap SystemCapability.Print.PrintFramework
   * @since 14
   */
  enum PrintJobSubState {
    /**
     * Print job succeed.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    PRINT_JOB_COMPLETED_SUCCESS = 0,

    /**
     * Print job fail.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    PRINT_JOB_COMPLETED_FAILED = 1,

    /**
     * Print job has been cancelled.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    PRINT_JOB_COMPLETED_CANCELLED = 2,

    /**
     * Print job has been corrupted.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    PRINT_JOB_COMPLETED_FILE_CORRUPTED = 3,

    /**
     * Print is offline.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    PRINT_JOB_BLOCK_OFFLINE = 4,

    /**
     * Print is occupied by other process.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    PRINT_JOB_BLOCK_BUSY = 5,

    /**
     * Print job has been cancelled.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    PRINT_JOB_BLOCK_CANCELLED = 6,

    /**
     * Print out of paper.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    PRINT_JOB_BLOCK_OUT_OF_PAPER = 7,

    /**
     * Print out of ink.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    PRINT_JOB_BLOCK_OUT_OF_INK = 8,

    /**
     * Print out of toner.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    PRINT_JOB_BLOCK_OUT_OF_TONER = 9,

    /**
     * Print paper jam.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    PRINT_JOB_BLOCK_JAMMED = 10,

    /**
     * Print cover open.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    PRINT_JOB_BLOCK_DOOR_OPEN = 11,

    /**
     * Print service request.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    PRINT_JOB_BLOCK_SERVICE_REQUEST = 12,

    /**
     * Print low on ink.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    PRINT_JOB_BLOCK_LOW_ON_INK = 13,

    /**
     * Print low on toner.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    PRINT_JOB_BLOCK_LOW_ON_TONER = 14,

    /**
     * Print really low on ink.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    PRINT_JOB_BLOCK_REALLY_LOW_ON_INK = 15,

    /**
     * Print bad certification.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    PRINT_JOB_BLOCK_BAD_CERTIFICATE = 16,

    /**
     * Print an error occurred when printing the account.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    PRINT_JOB_BLOCK_ACCOUNT_ERROR = 18,

    /**
     * Print the printing permission is abnormal.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    PRINT_JOB_BLOCK_PRINT_PERMISSION_ERROR = 19,

    /**
     * Print color printing permission exception.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    PRINT_JOB_BLOCK_PRINT_COLOR_PERMISSION_ERROR = 20,

    /**
     * Print the device is not connected to the network.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    PRINT_JOB_BLOCK_NETWORK_ERROR = 21,

    /**
     * Print unable to connect to the server.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    PRINT_JOB_BLOCK_SERVER_CONNECTION_ERROR = 22,

    /**
     * Print large file exception.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    PRINT_JOB_BLOCK_LARGE_FILE_ERROR = 23,

    /**
     * Print file parsing exception.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    PRINT_JOB_BLOCK_FILE_PARSING_ERROR = 24,

    /**
     * Print the file conversion is too slow.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    PRINT_JOB_BLOCK_SLOW_FILE_CONVERSION = 25,

    /**
     * Print uploading file.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    PRINT_JOB_RUNNING_UPLOADING_FILES = 26,

    /**
     * Print converting files.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    PRINT_JOB_RUNNING_CONVERTING_FILES = 27,

    /**
     * Print file uploading exception.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 18
     */
    PRINT_JOB_BLOCK_FILE_UPLOADING_ERROR = 30,

    /**
     * Print unknown issue.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    PRINT_JOB_BLOCK_UNKNOWN = 99,
  }

  /**
   * Enumeration of  Print error Code.
   * @enum { number } PrintErrorCode
   * @syscap SystemCapability.Print.PrintFramework
   * @since 14
   */
  enum PrintErrorCode {
    /**
     * No error.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    E_PRINT_NONE = 0,

    /**
     * No permission.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    E_PRINT_NO_PERMISSION = 201,

    /**
     * Invalid parameter.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    E_PRINT_INVALID_PARAMETER = 401,

    /**
     * Generic failure of print.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    E_PRINT_GENERIC_FAILURE = 13100001,

    /**
     * RPC failure.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    E_PRINT_RPC_FAILURE = 13100002,

    /**
     * Failure of print service.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    E_PRINT_SERVER_FAILURE = 13100003,

    /**
     * Invalid print extension.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    E_PRINT_INVALID_EXTENSION = 13100004,

    /**
     * Invalid printer.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    E_PRINT_INVALID_PRINTER = 13100005,

    /**
     * Invalid print job.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    E_PRINT_INVALID_PRINT_JOB = 13100006,

    /**
     * File i/o error.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    E_PRINT_FILE_IO = 13100007,

    /**
     * Number of files exceeding the upper limit.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 18
     */
    E_PRINT_TOO_MANY_FILES = 13100010,
  }

  /**
   * Enumeration of application event.
   * @enum { number } ApplicationEvent
   * @syscap SystemCapability.Print.PrintFramework
   * @since 14
   */
  enum ApplicationEvent {
    /**
     * Application created.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    APPLICATION_CREATED = 0,

    /**
     * Application closed for printing started.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    APPLICATION_CLOSED_FOR_STARTED = 1,

    /**
     * Application closed for printing canceled.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    APPLICATION_CLOSED_FOR_CANCELED = 2,
  }

  /**
   * defines print ExtensionInfo.
   * @typedef PrinterExtensionInfo
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  interface PrinterExtensionInfo {
    /**
     * Extension id of printer extension.
     * @type { string }
     * @syscap SystemCapability.Print.PrintFramework
     * @systemapi
     * @since 10
     */
    extensionId: string;

    /**
     * Vendor id of extension.
     * @type { string }
     * @syscap SystemCapability.Print.PrintFramework
     * @systemapi
     * @since 10
     */
    vendorId: string;

    /**
     * Vendor name.
     * @type { string }
     * @syscap SystemCapability.Print.PrintFramework
     * @systemapi
     * @since 10
     */
    vendorName: string;

    /**
     * Vendor icon.
     * @type { number }
     * @syscap SystemCapability.Print.PrintFramework
     * @systemapi
     * @since 10
     */
    vendorIcon: number;

    /**
     * Version of current printer extension.
     * @type { string }
     * @syscap SystemCapability.Print.PrintFramework
     * @systemapi
     * @since 10
     */
    version: string;
  }

  /**
   * Get all printer extension installed.
   * @permission ohos.permission.MANAGE_PRINT_JOB
   * @param { AsyncCallback<Array<PrinterExtensionInfo>> } callback - The callback function for handling the extension list found.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 202 - not system application
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function queryAllPrinterExtensionInfos(callback: AsyncCallback<Array<PrinterExtensionInfo>>): void;

  /**
   * Get all printer extension installed.
   * @permission ohos.permission.MANAGE_PRINT_JOB
   * @returns { Promise<Array<PrinterExtensionInfo>> } the promise returned by the function.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 202 - not system application
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function queryAllPrinterExtensionInfos(): Promise<Array<PrinterExtensionInfo>>;

  /**
   * Load the specific printer extension and start to discover printer.
   * @permission ohos.permission.MANAGE_PRINT_JOB
   * @param { Array<string> } extensionList - Indicates the list of printer extension.
   *        empty list of extensionList Indicates to find printer with all installed extension.
   * @param { AsyncCallback<void> } callback - The callback function for indcating the result of API execution.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 202 - not system application
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function startDiscoverPrinter(extensionList: Array<string>, callback: AsyncCallback<void>): void;

  /**
   * Load the specific printer extension and start to discover printer.
   * @permission ohos.permission.MANAGE_PRINT_JOB
   * @param { Array<string> } extensionList - Indicates the list of printer extension.
   *        empty list of extensionList Indicates to find printer with all installed extension.
   * @returns { Promise<void> } the promise returned by the function.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 202 - not system application
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function startDiscoverPrinter(extensionList: Array<string>): Promise<void>;

  /**
   * Stop discovering the printer with specific printer extension.
   * @permission ohos.permission.MANAGE_PRINT_JOB
   * @param { AsyncCallback<void> } callback - The callback function for indcating the result of API execution.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 202 - not system application
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function stopDiscoverPrinter(callback: AsyncCallback<void>): void;

  /**
   * Stop discovering the printer with specific printer extension.
   * @permission ohos.permission.MANAGE_PRINT_JOB
   * @returns { Promise<void> } the promise returned by the function.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 202 - not system application
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function stopDiscoverPrinter(): Promise<void>;

  /**
   * Connect the specific printer.
   * @permission ohos.permission.MANAGE_PRINT_JOB
   * @param { string } printerId - Indicates id of the printer.
   * @param { AsyncCallback<void> } callback - The callback function for indcating the result of API execution.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 202 - not system application
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function connectPrinter(printerId: string, callback: AsyncCallback<void>): void;

  /**
   * Connect the specific printer.
   * @permission ohos.permission.MANAGE_PRINT_JOB
   * @param { string } printerId - Indicates id of the printer.
   * @returns { Promise<void> } the promise returned by the function.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 202 - not system application
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function connectPrinter(printerId: string): Promise<void>;

  /**
   * Disconnect the specific printer.
   * @permission ohos.permission.MANAGE_PRINT_JOB
   * @param { string } printerId - Indicates id of the printer.
   * @param { AsyncCallback<void> } callback - The callback function for indcating the result of API execution.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 202 - not system application
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function disconnectPrinter(printerId: string, callback: AsyncCallback<void>): void;

  /**
   * Disconnect the specific printer.
   * @permission ohos.permission.MANAGE_PRINT_JOB
   * @param { string } printerId - Indicates id of the printer.
   * @returns { Promise<void> } the promise returned by the function.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 202 - not system application
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function disconnectPrinter(printerId: string): Promise<void>;

  /**
   * Query capability for the specific printer.
   * @permission ohos.permission.MANAGE_PRINT_JOB
   * @param { string } printerId - Indicates id of the printer.
   * @param { AsyncCallback<void> } callback - The callback function for capability from printer.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 202 - not system application
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function queryPrinterCapability(printerId: string, callback: AsyncCallback<void>): void;

  /**
   * Query capability for the specific printer.
   * @permission ohos.permission.MANAGE_PRINT_JOB
   * @param { string } printerId - Indicates id of the printer.
   * @returns { Promise<void> } the promise returned by the function.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 202 - not system application
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function queryPrinterCapability(printerId: string): Promise<void>;

  /**
   * Start print job.
   * @permission ohos.permission.MANAGE_PRINT_JOB
   * @param { PrintJob } jobInfo - Indicates the information of print job.
   * @param { AsyncCallback<void> } callback - The callback function for indcating the result of API execution.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 202 - not system application
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function startPrintJob(jobInfo: PrintJob, callback: AsyncCallback<void>): void;

  /**
   * Start print job.
   * @permission ohos.permission.MANAGE_PRINT_JOB
   * @param { PrintJob } jobInfo - Indicates the information of print job.
   * @returns { Promise<void> } the promise returned by the function.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 202 - not system application
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function startPrintJob(jobInfo: PrintJob): Promise<void>;

  /**
   * Cancel the print job has been sent to printer.
   * @permission ohos.permission.MANAGE_PRINT_JOB
   * @param { string } jobId - Indicates the specific print job.
   * @param { AsyncCallback<void> } callback - The callback function for indcating the result of API execution.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 202 - not system application
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function cancelPrintJob(jobId: string, callback: AsyncCallback<void>): void;

  /**
   * Cancel the print job has been sent to printer.
   * @permission ohos.permission.MANAGE_PRINT_JOB
   * @param { string } jobId - Indicates the specific print job.
   * @returns { Promise<void> } the promise returned by the function.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 202 - not system application
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function cancelPrintJob(jobId: string): Promise<void>;

  /**
   * Request preview of the print job.
   * @permission ohos.permission.MANAGE_PRINT_JOB
   * @param { PrintJob } jobInfo - Indicates the information of print job.
   * @param { Callback<number> } callback - The callback function for indcating the result of API execution.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 202 - not system application
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function requestPrintPreview(jobInfo: PrintJob, callback: Callback<number>): void;

  /**
   * Request preview of the print job.
   * @permission ohos.permission.MANAGE_PRINT_JOB
   * @param { PrintJob } jobInfo - Indicates the information of print job.
   * @returns { Promise<number> } the promise returned by the function.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 202 - not system application
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function requestPrintPreview(jobInfo: PrintJob): Promise<number>;

  /**
   * Register event callback for the state change of printer.
   * @permission ohos.permission.MANAGE_PRINT_JOB
   * @param { 'printerStateChange' } type - Indicates state change of printer.
   * @param { function } callback - The callback function for state change of printer.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 202 - not system application
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function on(type: 'printerStateChange', callback: (state: PrinterState, info: PrinterInfo) => void): void;

  /**
   * Unregister event callback for the state change of printer.
   * @permission ohos.permission.MANAGE_PRINT_JOB
   * @param { 'printerStateChange' } type - Indicates state change of printer.
   * @param { Callback<boolean> } callback - The callback function for state change of printer.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 202 - not system application
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function off(type: 'printerStateChange', callback?: Callback<boolean>): void;

  /**
   * Register event callback for the state change of print job.
   * @permission ohos.permission.MANAGE_PRINT_JOB
   * @param { 'jobStateChange' } type - Indicates state change of printer.
   * @param { function } callback - The callback function for state change of printer.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 202 - not system application
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function on(type: 'jobStateChange', callback: (state: PrintJobState, job: PrintJob) => void): void;

  /**
   * Unregister event callback for the state change of print job.
   * @permission ohos.permission.MANAGE_PRINT_JOB
   * @param { 'jobStateChange' } type - Indicates state change of printer.
   * @param { Callback<boolean> } callback - The callback function for state change of printer.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 202 - not system application
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function off(type: 'jobStateChange', callback?: Callback<boolean>): void;

  /**
   * Register event callback for the information change of print extension.
   * @permission ohos.permission.MANAGE_PRINT_JOB
   * @param { 'extInfoChange' } type - Indicates information change of print extension.
   * @param { function } callback - The callback function for information change of print extension.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 202 - not system application
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function on(type: 'extInfoChange', callback: (extensionId: string, info: string) => void): void;

  /**
   * Unregister event callback for the information change of print extension.
   * @permission ohos.permission.MANAGE_PRINT_JOB
   * @param { 'extInfoChange' } type - Indicates state change of printer.
   * @param { Callback<boolean> } callback - The callback function for state change of printer.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 202 - not system application
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function off(type: 'extInfoChange', callback?: Callback<boolean>): void;

  /**
   * New printers have been found and notify Print SA.
   * @permission ohos.permission.MANAGE_PRINT_JOB
   * @param { Array<PrinterInfo> } printers - Indicates new arrived printer lists.
   * @param { AsyncCallback<void> } callback - The callback function for indcating the result of API execution.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 202 - not system application
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function addPrinters(printers: Array<PrinterInfo>, callback: AsyncCallback<void>): void;

  /**
   * New printers have been found and notify Print SA.
   * @permission ohos.permission.MANAGE_PRINT_JOB
   * @param { Array<PrinterInfo> } printers - Indicates new arrived printer lists.
   * @returns { Promise<void> } the promise returned by the function.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 202 - not system application
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function addPrinters(printers: Array<PrinterInfo>): Promise<void>;

  /**
   * Printers have been lost and notify Print SA.
   * @permission ohos.permission.MANAGE_PRINT_JOB
   * @param { Array<string> } printerIds - Indicates the lost printer lists.
   * @param { AsyncCallback<void> } callback - The callback function for indcating the result of API execution.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 202 - not system application
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function removePrinters(printerIds: Array<string>, callback: AsyncCallback<void>): void;

  /**
   * Printers have been lost and notify Print SA.
   * @permission ohos.permission.MANAGE_PRINT_JOB
   * @param { Array<string> } printerIds - Indicates the lost printer lists.
   * @returns { Promise<void> } the promise returned by the function.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 202 - not system application
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function removePrinters(printerIds: Array<string>): Promise<void>;

  /**
   * Update the information of the specific printers.
   * @permission ohos.permission.MANAGE_PRINT_JOB
   * @param { Array<PrinterInfo> } printers - Indicates to be updated printer lists.
   * @param { AsyncCallback<void> } callback - The callback function for indcating the result of API execution.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 202 - not system application
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function updatePrinters(printers: Array<PrinterInfo>, callback: AsyncCallback<void>): void;

  /**
   * Update the information of the specific printers.
   * @permission ohos.permission.MANAGE_PRINT_JOB
   * @param { Array<PrinterInfo> } printers - Indicates to be updated printer lists.
   * @returns { Promise<void> } the promise returned by the function.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 202 - not system application
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function updatePrinters(printers: Array<PrinterInfo>): Promise<void>;

  /**
   * Notify Print SA the state of printer has been changed.
   * @permission ohos.permission.MANAGE_PRINT_JOB
   * @param { string } printerId - Indicates id of the printer.
   * @param { PrinterState } state - Indicates state of the printer.
   * @param { AsyncCallback<void> } callback - The callback function for indcating the result of API execution.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 202 - not system application
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function updatePrinterState(printerId: string, state: PrinterState, callback: AsyncCallback<void>): void;

  /**
   * Notify Print SA the state of printer has been changed.
   * @permission ohos.permission.MANAGE_PRINT_JOB
   * @param { string } printerId - Indicates id of the printer.
   * @param { PrinterState } state - Indicates state of the printer.
   * @returns { Promise<void> } the promise returned by the function.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 202 - not system application
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function updatePrinterState(printerId: string, state: PrinterState): Promise<void>;

  /**
   * Notify Print SA the state of print job has been changed.
   * @permission ohos.permission.MANAGE_PRINT_JOB
   * @param { string} jobId - Indicates id of the print job.
   * @param { PrintJobState } state - Indicates new state of print job.
   * @param { PrintJobSubState } subState - Indicates sub state of print job.
   * @param { AsyncCallback<void> } callback - The callback function for indcating the result of API execution.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 202 - not system application
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function updatePrintJobState(jobId: string, state: PrintJobState, subState: PrintJobSubState, callback: AsyncCallback<void>): void;

  /**
   * Notify Print SA the state of print job has been changed.
   * @permission ohos.permission.MANAGE_PRINT_JOB
   * @param { string} jobId - Indicates id of the print job.
   * @param { PrintJobState } state - Indicates new state of print job.
   * @param { PrintJobSubState } subState - Indicates sub state of print job.
   * @returns { Promise<void> } the promise returned by the function.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 202 - not system application
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function updatePrintJobState(jobId: string, state: PrintJobState, subState: PrintJobSubState): Promise<void>;

  /**
   * Notify Print SA the state of print extension has been changed.
   * @permission ohos.permission.MANAGE_PRINT_JOB
   * @param { string } info - Indicates changed information of print extension.
   * @param { AsyncCallback<void> } callback - The callback function for indcating the result of API execution.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 202 - not system application
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function updateExtensionInfo(info: string, callback: AsyncCallback<void>): void;

  /**
   * Notify Print SA the state of print extension has been changed.
   * @permission ohos.permission.MANAGE_PRINT_JOB
   * @param { string } info - Indicates changed information of print extension.
   * @returns { Promise<void> } the promise returned by the function.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 202 - not system application
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function updateExtensionInfo(info: string): Promise<void>;

  /**
   * Get all the printJobs in the queue.
   * @permission ohos.permission.MANAGE_PRINT_JOB
   * @param { AsyncCallback<void> } callback - The callback function for handling the printJob list found.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 202 - not system application
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 10
   * @deprecated since 11
   * @useinstead print#queryPrintJobList
   */
  function queryAllPrintJobs(callback: AsyncCallback<void>): void;

  /**
   * Get all the printJobs in the queue.
   * @permission ohos.permission.MANAGE_PRINT_JOB
   * @returns { Promise<void> } the promise returned by the function.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 202 - not system application
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 10
   * @deprecated since 11
   * @useinstead print#queryPrintJobList
   */
  function queryAllPrintJobs(): Promise<void>;

  /**
   * Get all the printJobs in the queue.
   * @permission ohos.permission.MANAGE_PRINT_JOB
   * @param { AsyncCallback<Array<PrintJob>> } callback - The callback function for handling the printJob list found.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 202 - not system application
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 11
   */
  function queryPrintJobList(callback: AsyncCallback<Array<PrintJob>>): void;

  /**
   * Get all the printJobs in the queue.
   * @permission ohos.permission.MANAGE_PRINT_JOB
   * @returns { Promise<Array<PrintJob>> } the promise returned by the function.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 202 - not system application
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 11
   */
  function queryPrintJobList(): Promise<Array<PrintJob>>;

  /**
   * Get printJob by jobId.
   * @permission ohos.permission.MANAGE_PRINT_JOB
   * @param { string } jobId - Indicates id of the print job.
   * @param { AsyncCallback<PrintJob> } callback - The callback function for get printJob by jobId.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 202 - not system application
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 11
   */
  function queryPrintJobById(jobId: string, callback: AsyncCallback<PrintJob>): void;

  /**
   * Get printJob by jobId.
   * @permission ohos.permission.MANAGE_PRINT_JOB
   * @param { string } jobId - Indicates id of the print job.
   * @returns { Promise<PrintJob> } the promise returned by the function.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 202 - not system application
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 11
   */
  function queryPrintJobById(jobId: string): Promise<PrintJob>;

  /**
   * Start getting print file.
   * @permission ohos.permission.MANAGE_PRINT_JOB
   * @param { string } jobId - Indicates id of the print job.
   * @param { PrintAttributes } printAttributes - Indicates print attributes.
   * @param { number } fd - Indicates print file fd.
   * @param { Callback<PrintFileCreationState> } onFileStateChanged - The callback function for update the file state.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 202 - not system application
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 11
   */
  function startGettingPrintFile(jobId: string, printAttributes: PrintAttributes, fd: number,
    onFileStateChanged: Callback<PrintFileCreationState>): void;

  /**
   * Notify print service the information.
   * @permission ohos.permission.MANAGE_PRINT_JOB
   * @param { string } jobId - Indicates id of the print job.
   * @param { 'spooler_closed_for_cancelled' | 'spooler_closed_for_started' } type - Indicates notify information.
   * @param { AsyncCallback<void> } callback - The callback function for indcating the result of API execution.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 202 - not system application
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 11
   */
  function notifyPrintService(jobId: string, type: 'spooler_closed_for_cancelled' | 'spooler_closed_for_started', callback: AsyncCallback<void>): void;

  /**
   * Notify print service the information.
   * @permission ohos.permission.MANAGE_PRINT_JOB
   * @param { string } jobId - Indicates id of the print job.
   * @param { 'spooler_closed_for_cancelled' | 'spooler_closed_for_started' } type - Indicates notify information.
   * @returns { Promise<void> } the promise returned by the function.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 202 - not system application
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 11
   */
  function notifyPrintService(jobId: string, type: 'spooler_closed_for_cancelled' | 'spooler_closed_for_started'): Promise<void>;

  /**
   * Get all added printers.
   * @permission ohos.permission.MANAGE_PRINT_JOB or ohos.permission.PRINT
   * @returns { Promise<Array<string>> } the promise returned by the function.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @syscap SystemCapability.Print.PrintFramework
   * @since 18
   */
  function getAddedPrinters(): Promise<Array<string>>;

  /**
   * Get printer info by printer id.
   * @permission ohos.permission.MANAGE_PRINT_JOB
   * @param { string } printerId - Indicates id of the printer.
   * @returns { Promise<PrinterInfo> } the promise returned by the function.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 202 - not system application
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 12
   */
  function getPrinterInfoById(printerId: string): Promise<PrinterInfo>;

  /**
   * Notify print service of application event.
   * @permission ohos.permission.MANAGE_PRINT_JOB
   * @param { ApplicationEvent } event - Indicates the event to be notified.
   * @returns { Promise<void> } the promise returned by the function.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 202 - not system application
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 12
   */
  function notifyPrintServiceEvent(event: ApplicationEvent): Promise<void>;

  /**
   * New printers have been found and notify Print SA.
   * @permission ohos.permission.PRINT
   * @param { PrinterInformation } printerInformation - Indicates new arrived printer lists.
   * @returns { Promise<void> } the promise returned by the function.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
   * @syscap SystemCapability.Print.PrintFramework
   * @since 14
   */
  function addPrinterToDiscovery(printerInformation: PrinterInformation): Promise<void>;

  /**
   * Update the information of the specific printer.
   * @permission ohos.permission.PRINT
   * @param { PrinterInformation } printerInformation - Indicates to be updated printer lists.
   * @returns { Promise<void> } the promise returned by the function.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
   * @syscap SystemCapability.Print.PrintFramework
   * @since 14
   */
  function updatePrinterInDiscovery(printerInformation: PrinterInformation): Promise<void>;

  /**
   * Notify Print SA to remove printer.
   * @permission ohos.permission.PRINT
   * @param { string } printerId - Indicates the lost printer lists.
   * @returns { Promise<void> } the promise returned by the function.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
   * @syscap SystemCapability.Print.PrintFramework
   * @since 14
   */
  function removePrinterFromDiscovery(printerId: string): Promise<void>;

  /**
   * Get printerInformation by printer id.
   * @permission ohos.permission.PRINT
   * @param { string } printerId - Indicates id of the printer.
   * @returns { Promise<PrinterInformation> } the promise returned by the function.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
   * @syscap SystemCapability.Print.PrintFramework
   * @since 14
   */
  function getPrinterInformationById(printerId: string): Promise<PrinterInformation>;

  /**
   * defines printer information.
   * @typedef PrinterInformation
   * @syscap SystemCapability.Print.PrintFramework
   * @since 14
   */
  interface PrinterInformation {
    /**
     * Printer id.
     * @type { string }
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    printerId: string;

    /**
     * Printer name.
     * @type { string }
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    printerName: string;

    /**
     * Current printer status.
     * @type { PrinterStatus }
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    printerStatus: PrinterStatus;

    /**
     * Printer description.
     * @type { ?string }
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    description?: string;

    /**
     * Printer capabilities.
     * @type { ?PrinterCapabilities }
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    capability?: PrinterCapabilities;

    /**
     * Printer uri.
     * @type { ?string }
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    uri?: string;

    /**
     * Printer make.
     * @type { ?string }
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    printerMake?: string;

    /**
     * Printer preferences.
     * @type { ?PrinterPreferences }
     * @syscap SystemCapability.Print.PrintFramework
     * @since 18
     */
    preferences?: PrinterPreferences;

    /**
     * Printer alias.
     * @type { ?string }
     * @syscap SystemCapability.Print.PrintFramework
     * @since 18
     */
    alias?: string;

    /**
     * Detail information in json format.
     * @type { ?string }
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    options?: string;
  }

  /**
   * defines printer capabilities.
   * @typedef PrinterCapabilities
   * @syscap SystemCapability.Print.PrintFramework
   * @since 14
   */
  interface PrinterCapabilities {
    /**
     * The page size list supported by the printer.
     * @type { Array<PrintPageSize> }
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    supportedPageSizes: Array<PrintPageSize>;

    /**
     * Array of supported color mode.
     * @type { Array<PrintColorMode> }
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    supportedColorModes: Array<PrintColorMode>;

    /**
     * Array of supported duplex mode.
     * @type { Array<PrintDuplexMode> }
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    supportedDuplexModes: Array<PrintDuplexMode>;

    /**
     * Array of supported print media types.
     * @type { ?Array<string> }
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    supportedMediaTypes?: Array<string>;

    /**
     * Array of supported print quality.
     * @type { ?Array<PrintQuality> }
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    supportedQualities?: Array<PrintQuality>;

    /**
     * Array of supported print orientation.
     * @type { ?Array<PrintOrientationMode> }
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    supportedOrientations?: Array<PrintOrientationMode>;

    /**
     * Advanced capability in json format.
     * @type { ?string }
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    options?: string;
  }

  /**
   * Enumeration of Print Quality.
   * @enum { number } PrintQuality
   * @syscap SystemCapability.Print.PrintFramework
   * @since 14
   */
  enum PrintQuality {
    /**
     * Draft quality mode.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    QUALITY_DRAFT = 3,

    /**
     * Normal quality mode.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    QUALITY_NORMAL = 4,

    /**
     * High quality mode.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    QUALITY_HIGH = 5,
  }

  /**
   * Enumeration of Print OrientationMode.
   * @enum { number } PrintOrientationMode
   * @syscap SystemCapability.Print.PrintFramework
   * @since 14
   */
  enum PrintOrientationMode {
    /**
     * Portrait mode.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    ORIENTATION_MODE_PORTRAIT = 0,

    /**
     * Landscape mode.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    ORIENTATION_MODE_LANDSCAPE= 1,

    /**
     * Reverse landscape mode.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    ORIENTATION_MODE_REVERSE_LANDSCAPE = 2,

    /**
     * Reverse portrait mode.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    ORIENTATION_MODE_REVERSE_PORTRAIT = 3,

    /**
     * Not specified.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    ORIENTATION_MODE_NONE = 4,
  }

  /**
   * Enumeration of Printer Status.
   * @enum { number } PrinterStatus
   * @syscap SystemCapability.Print.PrintFramework
   * @since 14
   */
  enum PrinterStatus {
    /**
     * Printer idle.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    PRINTER_IDLE = 0,

    /**
     * Printer busy.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    PRINTER_BUSY = 1,

    /**
     * Printer not available.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 14
     */
    PRINTER_UNAVAILABLE = 2,
  }

  /**
   * defines printer preferences.
   * @typedef PrinterPreferences
   * @syscap SystemCapability.Print.PrintFramework
   * @since 18
   */
  interface PrinterPreferences {
    /**
     * Default duplex mode.
     * @type { ?PrintDuplexMode }
     * @syscap SystemCapability.Print.PrintFramework
     * @since 18
     */
    defaultDuplexMode?: PrintDuplexMode;

    /**
     * Default quality.
     * @type { ?PrintQuality }
     * @syscap SystemCapability.Print.PrintFramework
     * @since 18
     */
    defaultPrintQuality?: PrintQuality;

    /**
     * Default media type.
     * @type { ?string }
     * @syscap SystemCapability.Print.PrintFramework
     * @since 18
     */
    defaultMediaType?: string;

    /**
     * Default page size id.
     * @type { ?string }
     * @syscap SystemCapability.Print.PrintFramework
     * @since 18
     */
    defaultPageSizeId?: string;

    /**
     * Default orientation mode.
     * @type { ?PrintOrientationMode }
     * @syscap SystemCapability.Print.PrintFramework
     * @since 18
     */
    defaultOrientation?: PrintOrientationMode;

    /**
     * Default margins.
     * @type { ?boolean }
     * @syscap SystemCapability.Print.PrintFramework
     * @since 18
     */
    borderless?: boolean;

    /**
     * Detailed printer preferences in json format.
     * @type { ?string }
     * @syscap SystemCapability.Print.PrintFramework
     * @since 18
     */
    options?: string;
  }

  /**
   * Enumeration of Printer Change Events.
   * @enum { number } PrinterEvent
   * @syscap SystemCapability.Print.PrintFramework
   * @since 18
   */
  enum PrinterEvent {
    /**
     * Printer added.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 18
     */
    PRINTER_EVENT_ADDED = 0,

    /**
     * Printer deleted.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 18
     */
    PRINTER_EVENT_DELETED = 1,

    /**
     * Printer state changed.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 18
     */
    PRINTER_EVENT_STATE_CHANGED = 2,

    /**
     * Printer info changed.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 18
     */
    PRINTER_EVENT_INFO_CHANGED = 3,

    /**
     * Printer preference changed.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 18
     */
    PRINTER_EVENT_PREFERENCE_CHANGED = 4,

    /**
     * Last used printer changed.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 18
     */
    PRINTER_EVENT_LAST_USED_PRINTER_CHANGED = 5,
  }

  /**
   * Enumeration of default printer type.
   * @enum { number } DefaultPrinterType
   * @syscap SystemCapability.Print.PrintFramework
   * @since 18
   */
  enum DefaultPrinterType {
    /**
     * Default printer set by user.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 18
     */
    DEFAULT_PRINTER_TYPE_SET_BY_USER = 0,

    /**
     * The last used printer is used as the default printer.
     * @syscap SystemCapability.Print.PrintFramework
     * @since 18
     */
    DEFAULT_PRINTER_TYPE_LAST_USED_PRINTER = 1,
  }

  /**
   * Update the information of the specific added printer.
   * @permission ohos.permission.MANAGE_PRINT_JOB
   * @param { PrinterInformation } printerInformation - Indicates the printer to be updated.
   * @returns { Promise<void> } the promise returned by the function.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 202 - not system application
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 18
   */
  function updatePrinterInformation(printerInformation: PrinterInformation): Promise<void>;

  /**
   * Save the preferences set by the user.
   * @permission ohos.permission.MANAGE_PRINT_JOB
   * @param { string } printerId - Indicates the printer to be updated.
   * @param { PrinterPreferences } printerPreferences - Indicates the printer preferences set by the user.
   * @returns { Promise<void> } the promise returned by the function.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 202 - not system application
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 18
   */
  function setPrinterPreferences(printerId: string, printerPreferences: PrinterPreferences): Promise<void>;

  /**
   * Discover all usb printers.
   * @permission ohos.permission.MANAGE_PRINT_JOB
   * @returns { Promise<Array<PrinterInformation>> } the promise returned by the function.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 202 - not system application
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 18
   */
  function discoverUsbPrinters(): Promise<Array<PrinterInformation>>;

  /**
   * Save the default printer set by the user.
   * @permission ohos.permission.MANAGE_PRINT_JOB
   * @param { string } printerId - Indicates the printer to be set as the default printer.
   * @param { DefaultPrinterType } type - Indicates the default printer type.
   * @returns { Promise<void> } the promise returned by the function.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 202 - not system application
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 18
   */
  function setDefaultPrinter(printerId: string, type: DefaultPrinterType): Promise<void>;

  /**
   * Notify print service of application event.
   * @permission ohos.permission.MANAGE_PRINT_JOB
   * @param { ApplicationEvent } event - Indicates the event to be notified.
   * @param { string } jobId - Indicates the job id.
   * @returns { Promise<void> } the promise returned by the function.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 202 - not system application
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
   * @syscap SystemCapability.Print.PrintFramework
   * @systemapi Hide this for inner system use.
   * @since 18
   */
  function notifyPrintServiceEvent(event: ApplicationEvent, jobId: string): Promise<void>;

  /**
   * Defines the callback type used in registering to listen for PrinterEvent.
   * The value of event indicates the information of PrinterEvent.
   * The value of printerInformation indicates the latest printer information.
   *
   * @typedef { function } PrinterChangeCallback
   * @param { PrinterEvent } event - the information of PrinterEvent
   * @param { PrinterInformation } printerInformation - the information of the latest printer
   * @syscap SystemCapability.Print.PrintFramework
   * @since 18
   */
  type PrinterChangeCallback = (event: PrinterEvent, printerInformation: PrinterInformation) => void;

  /**
   * Register event callback for the change of printer.
   * @permission ohos.permission.PRINT
   * @param { 'printerChange' } type - Indicates change of printer.
   * @param { PrinterChangeCallback } callback - The callback function for change of printer.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
   * @syscap SystemCapability.Print.PrintFramework
   * @since 18
   */
  function on(type: 'printerChange', callback: PrinterChangeCallback): void;

  /**
   * Unregister event callback for the change of printer.
   * @permission ohos.permission.PRINT
   * @param { 'printerChange' } type - Indicates change of printer.
   * @param { PrinterChangeCallback } [callback] - The callback function for change of printer.
   * @throws { BusinessError } 201 - the application does not have permission to call this function.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
   * @syscap SystemCapability.Print.PrintFramework
   * @since 18
   */
  function off(type: 'printerChange', callback?: PrinterChangeCallback): void;
}

export default print;