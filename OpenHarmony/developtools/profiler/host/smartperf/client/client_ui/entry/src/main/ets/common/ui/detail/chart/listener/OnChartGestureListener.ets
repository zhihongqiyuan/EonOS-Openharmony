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

import ChartTouchListener, { ChartGesture } from './ChartTouchListener';
/**
 * Listener for callbacks when doing gestures on the chart.
 *
 * @author Philipp Jahoda
 */
export default interface OnChartGestureListener {
  /**
   * Callbacks when a touch-gesture has started on the chart (ACTION_DOWN)
   *
   * @param me
   * @param lastPerformedGesture
   */
  onChartGestureStart(me: TouchEvent, lastPerformedGesture: ChartGesture);

  /**
   * Callbacks when a touch-gesture has ended on the chart (ACTION_UP, ACTION_CANCEL)
   *
   * @param me
   * @param lastPerformedGesture
   */
  onChartGestureEnd(me: TouchEvent, lastPerformedGesture: ChartGesture);

  /**
   * Callbacks when the chart is longpressed.
   *
   * @param me
   */
  onChartLongPressed(me: TouchEvent);

  /**
   * Callbacks when the chart is double-tapped.
   *
   * @param me
   */
  onChartDoubleTapped(me: TouchEvent);

  /**
   * Callbacks when the chart is single-tapped.
   *
   * @param me
   */
  onChartSingleTapped(me: TouchEvent);

  /**
   * Callbacks then a fling gesture is made on the chart.
   *
   * @param me1
   * @param me2
   * @param velocityX
   * @param velocityY
   */
  onChartFling(me1: TouchEvent, me2: TouchEvent, velocityX: number, velocityY: number);

  /**
   * Callbacks when the chart is scaled / zoomed via pinch zoom / double-tap gesture.
   *
   * @param me
   * @param scaleX scalefactor on the x-axis
   * @param scaleY scalefactor on the y-axis
   */
  onChartScale(me: TouchEvent, scaleX: number, scaleY: number);

  /**
   * Callbacks when the chart is moved / translated via drag gesture.
   *
   * @param me
   * @param dX translation distance on the x-axis
   * @param dY translation distance on the y-axis
   */
  onChartTranslate(me: TouchEvent, dX: number, dY: number);
}
