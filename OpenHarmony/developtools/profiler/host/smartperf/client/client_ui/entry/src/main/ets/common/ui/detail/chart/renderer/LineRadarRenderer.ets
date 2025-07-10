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

import Paint, { ImagePaint, PathPaint } from '../data/Paint';
import LineScatterCandleRadarRenderer from './LineScatterCandleRadarRenderer';
import Utils from '../utils/Utils';
import ChartAnimator from '../animation/ChartAnimator';
import ViewPortHandler from '../utils/ViewPortHandler';

/**
 * Created by Philipp Jahoda on 25/01/16.
 */
export default abstract class LineRadarRenderer extends LineScatterCandleRadarRenderer {
  constructor(animator: ChartAnimator, viewPortHandler: ViewPortHandler) {
    super(animator, viewPortHandler);
  }

  /**
   * Draws the provided path in filled mode with the provided drawable.
   *
   * @param c
   * @param filledPath
   * @param drawable
   */
  protected drawFilledPath(filledPath: string, icon?: ImagePaint, fillColor?: number, fillAlpha?: number): Paint[] {
    if (icon != null) {
      icon.setX(this.mViewPortHandler.contentLeft());
      icon.setY(this.mViewPortHandler.contentTop());
      icon.setWidth(this.mViewPortHandler.contentRight() - this.mViewPortHandler.contentLeft());
      icon.setHeight(this.mViewPortHandler.contentBottom() - this.mViewPortHandler.contentTop());
      return [icon];
    } else {
      let color: number = (fillAlpha << 24) | (fillColor & 0xffffff);
      let pathPaint: PathPaint = new PathPaint();
      pathPaint.setCommands(filledPath);
      pathPaint.setFill(color);
      return [pathPaint];
    }
  }
}
