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

import Paint from '../data/Paint';
import { PathPaint } from '../data/Paint';
import BarLineScatterCandleBubbleRenderer from './BarLineScatterCandleBubbleRenderer';
import ChartAnimator from '../animation/ChartAnimator';
import ViewPortHandler from '../utils/ViewPortHandler';
import ILineScatterCandleRadarDataSet from '../interfaces/datasets/ILineScatterCandleRadarDataSet';

/**
 * Created by Philipp Jahoda on 11/07/15.
 */
export default abstract class LineScatterCandleRadarRenderer extends BarLineScatterCandleBubbleRenderer {
  /**
   * path that is used for drawing highlight-lines (drawLines(...) cannot be used because of dashes)
   */
  private mHighlightLinePath: PathPaint = new PathPaint();

  constructor(animator: ChartAnimator, viewPortHandler: ViewPortHandler) {
    super(animator, viewPortHandler);
  }

  /**
   * Draws vertical & horizontal highlight-lines if enabled.
   *
   * @param c
   * @param x x-position of the highlight line intersection
   * @param y y-position of the highlight line intersection
   * @param set the currently drawn dataset
   */
  protected drawHighlightLines(x: number, y: number, dataSet: ILineScatterCandleRadarDataSet<any>): Paint[] {
    let pathPaints: Paint[] = [];

    // set color and stroke-width
    this.mHighlightPaint.setColor(dataSet.getHighLightColor());
    this.mHighlightPaint.setStrokeWidth(dataSet.getHighlightLineWidth());

    // draw highlighted lines (if enabled)
    this.mHighlightPaint.setDashPathEffect(dataSet.getDashPathEffectHighlight());

    // draw vertical highlight lines
    if (dataSet.isVerticalHighlightIndicatorEnabled()) {
      let pathPath = new PathPaint();
      pathPath.set(this.mHighlightPaint);
      pathPath.setCommands(
        'M' + x + ' ' + this.mViewPortHandler.contentTop() + ' L' + x + ' ' + this.mViewPortHandler.contentBottom()
      ) + ' Z';
      // create vertical path
      pathPaints.push(pathPath);
    }

    // draw horizontal highlight lines
    if (dataSet.isHorizontalHighlightIndicatorEnabled()) {
      let pathPath = new PathPaint();
      pathPath.set(this.mHighlightPaint);
      pathPath.setCommands(
        'M' + this.mViewPortHandler.contentLeft() + ' ' + y + ' L' + this.mViewPortHandler.contentRight() + ' ' + y
      ) + ' Z';
      // create horizontal path
      pathPaints.push(pathPath);
    }
    return pathPaints;
  }
}
