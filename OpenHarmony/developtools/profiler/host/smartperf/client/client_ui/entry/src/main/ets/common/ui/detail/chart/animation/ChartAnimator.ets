// @ts-nocheck
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

export default class ChartAnimator {
  /** object that is updated upon animation update */
  //    private AnimatorUpdateListener mListener;

  /** The phase of drawn values on the y-axis. 0 - 1 */
  protected mPhaseY: number = 1;

  /** The phase of drawn values on the x-axis. 0 - 1 */
  protected mPhaseX: number = 1;

  constructor() {}

  private xAnimator(duration: number, easing: EasingFunction): ObjectAnimator {
    let animatorX = ObjectAnimator.ofFloat(this, 'phaseX', 0, 1);
    animatorX.setInterpolator(easing);
    animatorX.setDuration(duration);

    return animatorX;
  }

  private yAnimator(duration: number, easing: EasingFunction): ObjectAnimator {
    let animatorY = ObjectAnimator.ofFloat(this, 'phaseY', 0, 1);
    animatorY.setInterpolator(easing);
    animatorY.setDuration(duration);

    return animatorY;
  }

  /**
   * Animates values along the X axis, in a linear fashion.
   *
   * @param durationMillis animation duration
   */
  public animateX(durationMillis: number) {
    animateX(durationMillis, Easing.Linear);
  }

  /**
   * Animates values along the X axis.
   *
   * @param durationMillis animation duration
   * @param easing EasingFunction
   */

  public animateX(durationMillis: number, easing: EasingFunction) {
    let animatorX = this.xAnimator(durationMillis, easing);
    animatorX.addUpdateListener(mListener);
    animatorX.start();
  }

  /**
   * Animates values along both the X and Y axes, in a linear fashion.
   *
   * @param durationMillisX animation duration along the X axis
   * @param durationMillisY animation duration along the Y axis
   */

  public animateXY(durationMillisX: number, durationMillisY: number) {
    animateXY(durationMillisX, durationMillisY, Easing.Linear, Easing.Linear);
  }

  /**
   * Animates values along both the X and Y axes.
   *
   * @param durationMillisX animation duration along the X axis
   * @param durationMillisY animation duration along the Y axis
   * @param easing EasingFunction for both axes
   */

  public animateXY(durationMillisX: number, durationMillisY: number, easing: EasingFunction) {
    let xAnimator = xAnimator(durationMillisX, easing);
    let yAnimator = yAnimator(durationMillisY, easing);

    if (durationMillisX > durationMillisY) {
      xAnimator.addUpdateListener(mListener);
    } else {
      yAnimator.addUpdateListener(mListener);
    }

    xAnimator.start();
    yAnimator.start();
  }

  /**
   * Animates values along both the X and Y axes.
   *
   * @param durationMillisX animation duration along the X axis
   * @param durationMillisY animation duration along the Y axis
   * @param easingX EasingFunction for the X axis
   * @param easingY EasingFunction for the Y axis
   */

  public animateXY(durationMillisX: number, durationMillisY: number, easingX: EasingFunction, easingY: EasingFunction) {
    let xAnimator = xAnimator(durationMillisX, easingX);
    let yAnimator = yAnimator(durationMillisY, easingY);

    if (durationMillisX > durationMillisY) {
      xAnimator.addUpdateListener(mListener);
    } else {
      yAnimator.addUpdateListener(mListener);
    }

    xAnimator.start();
    yAnimator.start();
  }

  /**
   * Animates values along the Y axis, in a linear fashion.
   *
   * @param durationMillis animation duration
   */

  public animateY(durationMillis: number) {
    this.animateY(durationMillis, Easing.Linear);
  }

  /**
   * Animates values along the Y axis.
   *
   * @param durationMillis animation duration
   * @param easing EasingFunction
   */

  public animateY(durationMillis: number, easing: EasingFunction) {
    let animatorY: ObjectAnimator = yAnimator(durationMillis, easing);
    animatorY.addUpdateListener(mListener);
    animatorY.start();
  }

  /**
   * Gets the Y axis phase of the animation.
   *
   * @return float value of {@link #mPhaseY}
   */
  public getPhaseY(): number {
    return this.mPhaseY;
  }

  /**
   * Sets the Y axis phase of the animation.
   *
   * @param phase float value between 0 - 1
   */
  public setPhaseY(phase: number) {
    if (phase > 1) {
      phase = 1;
    } else if (phase < 0) {
      phase = 0;
    }
    this.mPhaseY = phase;
  }

  /**
   * Gets the X axis phase of the animation.
   *
   * @return float value of {@link #mPhaseX}
   */
  public getPhaseX(): number {
    return this.mPhaseX;
  }

  /**
   * Sets the X axis phase of the animation.
   *
   * @param phase float value between 0 - 1
   */
  public setPhaseX(phase: number) {
    if (phase > 1) {
      phase = 1;
    } else if (phase < 0) {
      phase = 0;
    }
    this.mPhaseX = phase;
  }
}
