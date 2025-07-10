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

import AxisRenderer from './AxisRenderer';
import { XAxis, XAxisPosition } from '../components/XAxis';
import ViewPortHandler from '../utils/ViewPortHandler';
import Transformer from '../utils/Transformer';
import Utils from '../utils/Utils';
import FSize from '../utils/FSize';
import MPPointF from '../utils/MPPointF';
import MPPointD from '../utils/MPPointD';
import LimitLine, { LimitLabelPosition } from '../components/LimitLine';
import { JArrayList } from '../utils/JArrayList';
import MyRect from '../data/Rect';
import Paint, { Style, LinePaint, TextPaint, PathPaint } from '../data/Paint';

export default class XAxisRenderer extends AxisRenderer {
  protected mXAxis: XAxis;
  public yLeftLongestLabel: string = 'AAA';
  public yRightLongestLabel: string = 'AAA';
  constructor(viewPortHandler: ViewPortHandler, xAxis: XAxis, trans: Transformer) {
    super(viewPortHandler, trans, xAxis);
    this.mXAxis = xAxis;
    this.mAxisLabelPaint.setColor(Color.Black);
    this.mAxisLabelPaint.setTextAlign(TextAlign.Center);
    this.mAxisLabelPaint.setTextSize(10);
  }

  protected setupGridPaint() {
    this.mGridPaint.setColor(this.mXAxis.getGridColor());
    this.mGridPaint.setStrokeWidth(this.mXAxis.getGridLineWidth());
    this.mGridPaint.setDashPathEffect(this.mXAxis.getGridDashPathEffect());
  }

  public computeAxis(min: number, max: number, inverted: boolean) {
    // calculate the starting and entry point of the y-labels (depending on
    // zoom / contentrect bounds)
    if (this.mViewPortHandler.contentWidth() > 10 && !this.mViewPortHandler.isFullyZoomedOutX()) {
      var p1: MPPointD = this.mTrans.getValuesByTouchPoint(
      this.mViewPortHandler.contentLeft(),
      this.mViewPortHandler.contentTop()
      );
      var p2: MPPointD = this.mTrans.getValuesByTouchPoint(
      this.mViewPortHandler.contentRight(),
      this.mViewPortHandler.contentTop()
      );

      if (inverted) {
        min = p2.x;
        max = p1.x;
      } else {
        min = p1.x;
        max = p2.x;
      }

      MPPointD.recycleInstance(p1);
      MPPointD.recycleInstance(p2);
    }

    this.computeAxisValues(min, max);
  }

  //     @Override
  protected computeAxisValues(min: number, max: number) {
    super.computeAxisValues(min, max);
    this.computeSize();
  }

  protected computeSize() {
    let longest = this.mXAxis.getLongestLabel();

    this.mAxisLabelPaint.setTypeface(this.mXAxis.getTypeface());
    this.mAxisLabelPaint.setTextSize(this.mXAxis.getTextSize());

    let labelSize: FSize = Utils.calcTextSize(this.mAxisLabelPaint, longest);

    let labelWidth = labelSize.width;
    let labelHeight = Utils.calcTextHeight(this.mAxisLabelPaint, 'Q');

    let labelRotatedSize: FSize = Utils.getSizeOfRotatedRectangleByDegrees(
      labelWidth,
      labelHeight,
    this.mXAxis.getLabelRotationAngle()
    );

    this.mXAxis.mLabelWidth = Math.round(labelWidth);
    this.mXAxis.mLabelHeight = Math.round(labelHeight);
    this.mXAxis.mLabelRotatedWidth = Math.round(labelRotatedSize.width);
    this.mXAxis.mLabelRotatedHeight = Math.round(labelRotatedSize.height);

    FSize.recycleInstance(labelRotatedSize);
    FSize.recycleInstance(labelSize);
  }

  public renderAxisLabels(): Paint[] {
    if (!this.mXAxis.isEnabled() || !this.mXAxis.isDrawLabelsEnabled()) {
      return [];
    }

    let yoffset = this.mXAxis.getYOffset();

    this.mAxisLabelPaint.setTypeface(this.mXAxis.getTypeface());
    this.mAxisLabelPaint.setTextSize(this.mXAxis.getTextSize());
    this.mAxisLabelPaint.setColor(this.mXAxis.getTextColor());

    let pointF: MPPointF = MPPointF.getInstance(0, 0);
    if (this.mXAxis.getPosition() == XAxisPosition.TOP) {
      pointF.x = 0.5;
      pointF.y = 1.0;
      return this.drawLabels(this.mViewPortHandler.contentTop() - yoffset, pointF);
    } else if (this.mXAxis.getPosition() == XAxisPosition.TOP_INSIDE) {
      pointF.x = 0.5;
      pointF.y = 1.0;
      return this.drawLabels(this.mViewPortHandler.contentTop() + yoffset + this.mXAxis.mLabelRotatedHeight, pointF);
    } else if (this.mXAxis.getPosition() == XAxisPosition.BOTTOM) {
      pointF.x = 0.5;
      pointF.y = 0.0;
      return this.drawLabels(this.mViewPortHandler.contentBottom() + yoffset, pointF);
    } else if (this.mXAxis.getPosition() == XAxisPosition.BOTTOM_INSIDE) {
      pointF.x = 0.5;
      pointF.y = 0.0;
      return this.drawLabels(this.mViewPortHandler.contentBottom() - yoffset - this.mXAxis.mLabelRotatedHeight, pointF);
    } else {
      // BOTH SIDED
      let paintArr: Paint[] = [];
      pointF.x = 0.5;
      pointF.y = 1.0;
      paintArr = paintArr.concat(this.drawLabels(this.mViewPortHandler.contentTop() - yoffset, pointF));
      pointF.x = 0.5;
      pointF.y = 0.0;
      paintArr = paintArr.concat(this.drawLabels(this.mViewPortHandler.contentBottom() + yoffset, pointF));
      return paintArr;
    }
    MPPointF.recycleInstance(pointF);
  }
  public renderAxisLine(): Paint[] {
    if (!this.mXAxis.isDrawAxisLineEnabled() || !this.mXAxis.isEnabled()) {
      return [];
    }

    this.mAxisLinePaint.setColor(this.mXAxis.getAxisLineColor());
    this.mAxisLinePaint.setStrokeWidth(this.mXAxis.getAxisLineWidth());
    this.mAxisLinePaint.setDashPathEffect(this.mXAxis.getAxisLineDashPathEffect());
    let linePaint: LinePaint = this.mAxisLinePaint as LinePaint;
    let leftTextWidth = this.getLeftYTextWidth();
    let leftRightWidth = this.getRightYTextWidth();
    if (
    this.mXAxis.getPosition() == XAxisPosition.TOP ||
    this.mXAxis.getPosition() == XAxisPosition.TOP_INSIDE ||
    this.mXAxis.getPosition() == XAxisPosition.BOTH_SIDED
    ) {
      console.log('mViewPortHandler:' + this.mViewPortHandler.contentLeft() + ' leftTextWidth:' + leftTextWidth);
      console.log('mViewPortHandler:' + this.mViewPortHandler.contentRight() + ' leftTextWidth:' + leftRightWidth);

      linePaint.setStartPoint([
        this.mViewPortHandler.contentLeft() + leftTextWidth,
        this.mViewPortHandler.contentTop(),
      ]);
      linePaint.setEndPoint([
        this.mViewPortHandler.contentRight() - leftRightWidth,
        this.mViewPortHandler.contentTop(),
      ]);
    }

    if (
    this.mXAxis.getPosition() == XAxisPosition.BOTTOM ||
    this.mXAxis.getPosition() == XAxisPosition.BOTTOM_INSIDE ||
    this.mXAxis.getPosition() == XAxisPosition.BOTH_SIDED
    ) {
      linePaint.setStartPoint([
        this.mViewPortHandler.contentLeft() + leftTextWidth,
        this.mViewPortHandler.contentBottom(),
      ]);
      linePaint.setEndPoint([
        this.mViewPortHandler.contentRight() - leftRightWidth,
        this.mViewPortHandler.contentBottom(),
      ]);
      linePaint.setStrokeWidth(1.5);
    }
    return [linePaint];
  }

  /**
   * draws the x-labels on the specified y-position
   *
   * @param pos
   */
  protected drawLabels(pos: number, anchor: MPPointF): Paint[] {
    let labelRotationAngleDegrees = this.mXAxis.getLabelRotationAngle();
    let centeringEnabled = this.mXAxis.isCenterAxisLabelsEnabled();

    let positions: number[] = new Array<number>(this.mXAxis.mEntryCount * 2);

    for (var i = 0; i < positions.length; i += 2) {
      // only fill x values
      if (centeringEnabled) {
        positions[i] = this.mXAxis.mCenteredEntries[i / 2];
      } else {
        positions[i] = this.mXAxis.mEntries[i / 2];
      }
    }

    this.mTrans.pointValuesToPixel(positions);
    let labelPaint: Paint[] = [];
    for (let i = 0; i < positions.length; i += 2) {
      let x = positions[i];

      if (this.mViewPortHandler.isInBoundsX(this.getXRelativeValue(x) / this.getAxisPercent())) {
        let label = this.mXAxis.getValueFormatter().getFormattedValue(this.mXAxis.mEntries[i / 2], this.mXAxis);

        if (this.mXAxis.isAvoidFirstLastClippingEnabled()) {
          // avoid clipping of the last
          if (i / 2 == this.mXAxis.mEntryCount - 1 && this.mXAxis.mEntryCount > 1) {
            let width = Utils.calcTextWidth(this.mAxisLabelPaint, label);

            if (width > this.mViewPortHandler.offsetRight() * 2 && x + width > this.mViewPortHandler.getChartWidth()) {
              x -= width / 2;
            }
            // avoid clipping of the first
          } else if (i == 0) {
            let width = Utils.calcTextWidth(this.mAxisLabelPaint, label);
            x += width / 2;
          }
        }

        labelPaint.push(this.drawLabel(label, this.getXRelativeValue(x), pos, anchor, labelRotationAngleDegrees));
      }
      if (x - this.mAxis.getAxisMinimum() == 0) {
        let label = this.mXAxis.getValueFormatter().getFormattedValue(this.mXAxis.mEntries[i / 2], this.mXAxis);
        labelPaint.push(this.drawLabel(label, this.getXRelativeValue(x), pos, anchor, labelRotationAngleDegrees));
      }
    }
    return labelPaint;
  }

  protected drawLabel(formattedLabel: string, x: number, y: number, anchor: MPPointF, angleDegrees: number): Paint {
    let xResult = this.calcXLeftOffset(x);
    let labelPaint = new TextPaint(this.mAxisLabelPaint as TextPaint);
    return Utils.drawXAxisValue(formattedLabel, xResult, y, labelPaint, anchor, angleDegrees);
  }

  protected mRenderGridLinesPath: string = '';
  protected mRenderGridLinesBuffer = new Array(2);

  public renderGridLines(): Paint[] {
    if (!this.mXAxis.isDrawGridLinesEnabled() || !this.mXAxis.isEnabled()) {
      return [];
    }
    this.mGridPaint.setColor(this.mXAxis.getGridColor());
    this.mGridPaint.setStrokeWidth(this.mXAxis.getGridLineWidth());
    this.mGridPaint.setDashPathEffect(this.mXAxis.getGridDashPathEffect());

    if (this.mRenderGridLinesBuffer.length != this.mAxis.mEntryCount * 2) {
      this.mRenderGridLinesBuffer = new Array<number>(this.mXAxis.mEntryCount * 2);
    }
    let positions = this.mRenderGridLinesBuffer;

    for (var i = 0; i < positions.length; i += 2) {
      positions[i] = this.mXAxis.mEntries[i / 2];
      positions[i + 1] = this.mXAxis.mEntries[i / 2];
    }

    this.mTrans.pointValuesToPixel(positions);

    this.setupGridPaint();

    var gridLinePath: string = this.mRenderGridLinesPath;
    gridLinePath = '';
    var girdLinePaint: Paint[] = [];

    for (var i = 0; i < positions.length; i += 2) {
      let x = positions[i];
      if (this.mViewPortHandler.isInBoundsX(this.getXRelativeValue(x) / this.getAxisPercent())) {
        if (
        this.getXRelativeValue(x) / this.getAxisPercent() <
        this.mViewPortHandler.contentRight() - this.getRightYTextWidth()
        ) {
          var pathPaintF: Paint = this.drawGridLine(this.getXRelativeValue(x), positions[i + 1], gridLinePath);
          girdLinePaint.push(pathPaintF);
        }
      }
    }
    return girdLinePaint;
  }

  protected mGridClippingRect: MyRect = new MyRect();

  public getGridClippingRect(): MyRect {
    this.mGridClippingRect.set(
    this.mViewPortHandler.getContentRect().left,
    this.mViewPortHandler.getContentRect().top,
    this.mViewPortHandler.getContentRect().right,
    this.mViewPortHandler.getContentRect().bottom
    );
    this.mGridClippingRect.inset(-this.mAxis.getGridLineWidth(), 0, -this.mAxis.getGridLineWidth(), 0);
    return this.mGridClippingRect;
  }

  /**
   * Draws the grid line at the specified position using the provided path.
   *
   * @param c
   * @param x
   * @param y
   * @param gridLinePath
   */
  protected drawGridLine(x: number, y: number, path: string): Paint {
    let xResult: number = this.calcXLeftOffset(x);
    path =
    'M' +
    Utils.convertDpToPixel(xResult) +
    ' ' +
    Utils.convertDpToPixel(this.mViewPortHandler.contentBottom()) +
    'L' +
    Utils.convertDpToPixel(xResult) +
    ' ' +
    Utils.convertDpToPixel(this.mViewPortHandler.contentTop());
    var pathPaint: PathPaint = new PathPaint(this.mGridPaint as PathPaint);
    pathPaint.setCommands(path);
    return pathPaint;
  }

  protected mRenderLimitLinesBuffer = new Array<number>(2);
  protected mLimitLineClippingRect = new MyRect();

  /**
   * retrurn the LimitLines draw data.
   *
   * @param c
   */
  public renderLimitLines(): Paint[] {
    let limitLines: JArrayList<LimitLine> = this.mXAxis.getLimitLines();

    if (limitLines == null ||
            limitLines.size() <= 0 ||
                !this.mAxis.isDrawLimitLinesBehindDataEnabled()) {
      return [];
    }

    let limitPaint: Paint[] = [];

    let position = this.mRenderLimitLinesBuffer;
    position[0] = 0;
    position[1] = 0;

    for (var i = 0; i < limitLines.size(); i++) {
      let l: LimitLine = limitLines.get(i);
      if (!l.isEnabled()) {
        continue;
      }

      this.mLimitLineClippingRect.set(
      this.mViewPortHandler.getContentRect().left,
      this.mViewPortHandler.getContentRect().top,
      this.mViewPortHandler.getContentRect().right,
      this.mViewPortHandler.getContentRect().bottom
      );
      this.mGridClippingRect.inset(-this.mAxis.getGridLineWidth(), 0, -this.mAxis.getGridLineWidth(), 0);

      position[0] = this.getXRelativeValue(l.getLimit());
      position[1] = 0;

      this.mTrans.pointValuesToPixel(position);
      limitPaint.push(this.renderLimitLineLine(l, position));
      limitPaint.push(this.renderLimitLineLabel(l, position, 2 + l.getYOffset()));
    }
    return limitPaint;
  }

  private mLimitLineSegmentsBuffer = new Array<number>(4);
  private mLimitLinePath = new Paint();

  public renderLimitLineLine(limitLine: LimitLine, position: number[]): Paint {
    let leftXResult = this.calcXLeftOffset(position[0]);
    this.mLimitLineSegmentsBuffer[0] = leftXResult;
    this.mLimitLineSegmentsBuffer[1] = this.mViewPortHandler.contentTop();
    this.mLimitLineSegmentsBuffer[2] = leftXResult;
    this.mLimitLineSegmentsBuffer[3] = this.mViewPortHandler.contentBottom();

    this.mLimitLinePaint.setStyle(Style.STROKE);
    this.mLimitLinePaint.setColor(limitLine.getLineColor());
    this.mLimitLinePaint.setStrokeWidth(limitLine.getLineWidth());
    this.mLimitLinePaint.setDashPathEffect(limitLine.getDashPathEffect());
    let path: string =
      'M' +
      Utils.convertDpToPixel(this.mLimitLineSegmentsBuffer[0]) +
      ' ' +
      Utils.convertDpToPixel(this.mLimitLineSegmentsBuffer[1]) +
      'L' +
      Utils.convertDpToPixel(this.mLimitLineSegmentsBuffer[2]) +
      ' ' +
      Utils.convertDpToPixel(this.mLimitLineSegmentsBuffer[3]);

    let pathPaint: PathPaint = new PathPaint(this.mLimitLinePaint as PathPaint);
    pathPaint.setCommands(path);
    return pathPaint;
  }
  public renderLimitLineLabel(limitLine: LimitLine, position: number[], yOffset: number): Paint {
    let label = limitLine.getLabel();

    // if drawing the limit-value label is enabled
    if (label != null && label.length > 0) {
      this.mLimitLinePaint.setStyle(limitLine.getTextStyle());
      this.mLimitLinePaint.setDashPathEffect(null);
      this.mLimitLinePaint.setColor(limitLine.getTextColor());
      this.mLimitLinePaint.setStrokeWidth(0.5);
      this.mLimitLinePaint.setTextSize(limitLine.getTextSize());

      let textPaint: TextPaint = new TextPaint();
      textPaint.setTextSize(limitLine.getTextSize());
      textPaint.setStyle(limitLine.getTextStyle());
      textPaint.setColor(limitLine.getTextColor());
      textPaint.setText(label);

      let xOffset = limitLine.getLineWidth() + limitLine.getXOffset();

      let labelPosition: LimitLabelPosition = limitLine.getLabelPosition();

      if (labelPosition == LimitLabelPosition.RIGHT_TOP) {
        textPaint.setTextAlign(TextAlign.Start);
        textPaint.x = this.calcXLeftOffset(position[0]) + xOffset;
        textPaint.y = this.mViewPortHandler.contentTop() + yOffset;
      } else if (labelPosition == LimitLabelPosition.RIGHT_BOTTOM) {
        let labelLineHeight = Utils.calcTextHeight(this.mLimitLinePaint, label);
        textPaint.setTextAlign(TextAlign.Start);
        textPaint.x = this.calcXLeftOffset(position[0]) + xOffset;
        textPaint.y = this.mViewPortHandler.contentBottom() - yOffset - labelLineHeight;
      } else if (labelPosition == LimitLabelPosition.LEFT_TOP) {
        textPaint.setTextAlign(TextAlign.End);
        let labelLineWidth = Utils.calcTextWidth(this.mLimitLinePaint, label);
        textPaint.x = this.calcXLeftOffset(position[0]) - xOffset - labelLineWidth;
        textPaint.y = this.mViewPortHandler.contentTop() + yOffset;
      } else {
        textPaint.setTextAlign(TextAlign.End);
        let labelLineWidth = Utils.calcTextWidth(this.mLimitLinePaint, label);
        let labelLineHeight = Utils.calcTextHeight(this.mLimitLinePaint, label);
        textPaint.x = this.calcXLeftOffset(position[0]) - xOffset - labelLineWidth;
        textPaint.y = this.mViewPortHandler.contentBottom() - yOffset - labelLineHeight;
      }
      return textPaint;
    }
  }
  public calcXLeftOffset(xVlaus: number): number {
    var xResult = xVlaus / this.getAxisPercent() +
                  this.mViewPortHandler.contentLeft() + this.getLeftYTextWidth();
    return xResult;
  }
  public getAxisPercent(): number {
    return (
      (this.mAxis.getAxisMaximum() - (this.mAxis.getAxisMinimum() >= 0 ? 0 : this.mAxis.getAxisMinimum())) /
      this.getLineRange()
    );
  }
  public getLeftYTextWidth(): number {
    return Utils.calcTextWidth(this.mAxisLabelPaint, this.yLeftLongestLabel);
  }
  public getRightYTextWidth(): number {
    return Utils.calcTextWidth(this.mAxisLabelPaint, this.yRightLongestLabel);
  }
  public getLineRange(): number {
    return (
      this.mViewPortHandler.contentRight() -
      this.mViewPortHandler.contentLeft() -
      this.getLeftYTextWidth() -
      this.getRightYTextWidth()
    );
  }
  public getXRelativeValue(x: number): number {
    return x - (this.mAxis.getAxisMinimum() >= 0 ? 0 : this.mAxis.getAxisMinimum());
  }
}
