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
 * @kit ArkUI
 */

import { AsyncCallback } from './@ohos.base';

/**
 * Window animation manager.
 *
 * @namespace windowAnimationManager
 * @syscap SystemCapability.WindowManager.WindowManager.Core
 * @systemapi Hide this for inner system use.
 * @since 9
 */
declare namespace windowAnimationManager {
  /**
   * Set the window animation controller.
   *
   * @param { WindowAnimationController } controller - Window animation controller.
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function setController(controller: WindowAnimationController): void;

  /**
   * Minimize the window target with animation.
   *
   * @param { WindowAnimationTarget } windowTarget - The window target to be minimized.
   * @param { AsyncCallback<WindowAnimationFinishedCallback> } callback - Returns the animation finished callback.
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function minimizeWindowWithAnimation(windowTarget: WindowAnimationTarget,
    callback: AsyncCallback<WindowAnimationFinishedCallback>): void;

  /**
   * Minimize the window target with animation.
   *
   * @param { WindowAnimationTarget }windowTarget - The window target to be minimized.
   * @returns { Promise<WindowAnimationFinishedCallback> } Promise used to return the animation finished callback.
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function minimizeWindowWithAnimation(windowTarget: WindowAnimationTarget): Promise<WindowAnimationFinishedCallback>;

  /**
   * Round rect.
   *
   * @interface RRect
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  export interface RRect {
    /**
     * The X-axis coordinate of the upper left vertex of the round rect, in pixels.
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    left: number;

    /**
     * The Y-axis coordinate of the upper left vertex of the round rect, in pixels.
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    top: number;

    /**
     * Width of the round rect, in pixels.
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    width: number;

    /**
     * Height of the round rect, in pixels.
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    height: number;

    /**
     * Radius of the round corner of the round rect, in pixels.
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    radius: number;
  }

  /**
   * Window animation target.
   *
   * @interface WindowAnimationTarget
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  export interface WindowAnimationTarget {
    /**
     * The bundle name of the window animation target.
     * @type { string }
     * @readonly
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    readonly bundleName: string;

    /**
    /* The ability name of the window animation target.
     * @type { string }
     * @readonly
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    readonly abilityName: string;

    /**
    /* The window bounds of the window animation target.
     * @type { RRect }
     * @readonly
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    readonly windowBounds: RRect;

    /**
    /* The mission id of the window animation target.
     * @type { number }  
     * @readonly
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    readonly missionId: number;
  }

  /**
   * Window animation finished callback.
   *
   * @interface WindowAnimationFinishedCallback
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  export interface WindowAnimationFinishedCallback {
    /**
     * The function of window animation finished callback.
     *
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    onAnimationFinish(): void;
  }

  /**
   * Window animation controller.
   *
   * @interface WindowAnimationController
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  export interface WindowAnimationController {
    /**
     * Called on starting an application form launcher.
     *
     * @param { WindowAnimationTarget } startingWindowTarget  - indicates Window target of the starting application.
     * @param { WindowAnimationFinishedCallback } finishCallback - Animation finished callback.
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    onStartAppFromLauncher(startingWindowTarget: WindowAnimationTarget,
      finishCallback: WindowAnimationFinishedCallback): void;

    /**
     * Called on starting an application form recent.
     *
     * @param { WindowAnimationTarget } startingWindowTarget - Window target of the starting application.
     * @param { WindowAnimationFinishedCallback } finishCallback - Animation finished callback.
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    onStartAppFromRecent(startingWindowTarget: WindowAnimationTarget,
      finishCallback: WindowAnimationFinishedCallback): void;

    /**
     * Called on starting an application form other.
     *
     * @param { WindowAnimationTarget } startingWindowTarget - Window target of the starting application.
     * @param { WindowAnimationFinishedCallback } finishCallback - Animation finished callback.
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    onStartAppFromOther(startingWindowTarget: WindowAnimationTarget,
      finishCallback: WindowAnimationFinishedCallback): void;

    /**
     * Called on application transition.
     *
     * @param { WindowAnimationTarget } fromWindowTarget - Window target of the source application.
     * @param { WindowAnimationTarget } toWindowTarget - Window target of the destination application.
     * @param { WindowAnimationFinishedCallback } finishCallback - Animation finished callback.
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    onAppTransition(fromWindowTarget: WindowAnimationTarget, toWindowTarget: WindowAnimationTarget,
      finishCallback: WindowAnimationFinishedCallback): void;

    /**
     * Called on minimizing a window.
     *
     * @param { WindowAnimationTarget } minimizingWindowTarget - Window target of the minimizing window.
     * @param { WindowAnimationFinishedCallback } finishCallback - Animation finished callback.
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    onMinimizeWindow(minimizingWindowTarget: WindowAnimationTarget,
      finishCallback: WindowAnimationFinishedCallback): void;

    /**
     * Called on closing a window.
     *
     * @param { WindowAnimationTarget }closingWindowTarget - Window target of the closing window.
     * @param { WindowAnimationFinishedCallback } finishCallback - Animation finished callback.
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    onCloseWindow(closingWindowTarget: WindowAnimationTarget, finishCallback: WindowAnimationFinishedCallback): void;

    /**
     * Called on unlocking the screen.
     *
     * @param {WindowAnimationFinishedCallback } finishCallback - Animation finished callback.
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    onScreenUnlock(finishCallback: WindowAnimationFinishedCallback): void;


    /**
     * Called on window animation targets update.
     *
     * @param { WindowAnimationTarget } fullScreenWindowTarget - The fullscreen window target.
     * @param { Array<WindowAnimationTarget> } floatingWindowTargets - All the floating window targets.
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    onWindowAnimationTargetsUpdate(fullScreenWindowTarget: WindowAnimationTarget,
      floatingWindowTargets: Array<WindowAnimationTarget>): void;
  }
}

export default windowAnimationManager;
