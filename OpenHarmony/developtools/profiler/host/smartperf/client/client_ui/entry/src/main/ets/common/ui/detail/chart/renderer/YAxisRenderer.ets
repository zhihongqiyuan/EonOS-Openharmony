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

import MyRect from '../data/Rect';
import YAxis, { AxisDependency, YAxisLabelPosition } from '../components/YAxis';
import Paint, { Style, LinePaint, PathPaint, TextPaint } from '../data/Paint';
import Utils from '../utils/Utils';
import AxisRenderer from '../renderer/AxisRenderer';
import LimitLine, { LimitLabelPosition } from '../components/LimitLine';
import { JArrayList } from '../utils/JArrayList';
import ViewPortHandler from '../utils/ViewPortHandler';
import Transformer from '../utils/Transformer';
import MPPointD from '../utils/MPPointD';

export default class YAxisRenderer extends AxisRenderer {
  protected mYAxis: YAxis;
  protected mZeroLinePaint: Paint;

  constructor(viewPortHandler: ViewPortHandler, yAxis: YAxis, trans: Transformer) {
    super(viewPortHandler, trans, yAxis);
    this.mYAxis = yAxis;

    if (viewPortHandler != null) {
      this.mAxisLabelPaint.setColor(Color.Black);
      this.mAxisLabelPaint.setTextSize(10);

      this.mZeroLinePaint = new PathPaint();
      this.mZeroLinePaint.setColor(Color.Gray);
      this.mZeroLinePaint.setStrokeWidth(1);
      this.mZeroLinePaint.setStyle(Style.STROKE);
    }
  }

  /**
   * draws the y-axis labels to the screen
   */
  public renderAxisLabels(): Paint[] {
    if (!this.mYAxis.isEnabled() || !this.mYAxis.isDrawLabelsEnabled()) {
      return [];
    }

    let positions: number[] = this.getTransformedPositions();

    this.mAxisLabelPaint.setTypeface(this.mYAxis.getTypeface());
    this.mAxisLabelPaint.setTextSize(this.mYAxis.getTextSize());
    this.mAxisLabelPaint.setColor(this.mYAxis.getTextColor());

    let xOffset = this.mYAxis.getXOffset();
    let yOffset = Utils.calcTextHeight(this.mAxisLabelPaint, 'A') + this.mYAxis.getYOffset();

    let dependency: AxisDependency = this.mYAxis.getAxisDependency();
    let labelPosition: YAxisLabelPosition = this.mYAxis.getLabelPosition();

    let xPos: number = 0;

    if (dependency == AxisDependency.LEFT) {
      if (labelPosition == YAxisLabelPosition.OUTSIDE_CHART) {
        this.mAxisLabelPaint.setTextAlign(TextAlign.End);
        xPos = this.mViewPortHandler.offsetLeft() - xOffset;
      } else {
        this.mAxisLabelPaint.setTextAlign(TextAlign.Start);
        xPos = this.mViewPortHandler.offsetLeft() + xOffset;
      }
    } else {
      if (labelPosition == YAxisLabelPosition.OUTSIDE_CHART) {
        this.mAxisLabelPaint.setTextAlign(TextAlign.Start);
        xPos = this.mViewPortHandler.contentRight();
      } else {
        this.mAxisLabelPaint.setTextAlign(TextAlign.End);
        xPos = this.mViewPortHandler.contentRight() - xOffset;
      }
    }

    return this.drawYLabels(xPos, positions, yOffset);
  }

  public renderAxisLine(): Paint[] {
    if (!this.mYAxis.isEnabled() || !this.mYAxis.isDrawAxisLineEnabled()) {
      return [];
    }

    this.mAxisLinePaint.setColor(this.mYAxis.getAxisLineColor());
    this.mAxisLinePaint.setStrokeWidth(this.mYAxis.getAxisLineWidth());

    this.mViewPortHandler.getContentRect().right =
    this.mViewPortHandler.contentRight() - Utils.calcTextWidth(this.mAxisLabelPaint, this.mAxis.getLongestLabel());
    if (this.mYAxis.getAxisDependency() == AxisDependency.LEFT) {
      (this.mAxisLinePaint as LinePaint).setStartPoint([
        this.mViewPortHandler.contentLeft() + Utils.calcTextWidth(this.mAxisLabelPaint, this.mAxis.getLongestLabel()),
        this.mViewPortHandler.contentTop(),
      ]);
      (this.mAxisLinePaint as LinePaint).setEndPoint([
        this.mViewPortHandler.contentLeft() + Utils.calcTextWidth(this.mAxisLabelPaint, this.mAxis.getLongestLabel()),
        this.mViewPortHandler.contentBottom(),
      ]);
    } else {
      (this.mAxisLinePaint as LinePaint).setStartPoint([
        this.mViewPortHandler.contentRight(),
        this.mViewPortHandler.contentTop(),
      ]);
      (this.mAxisLinePaint as LinePaint).setEndPoint([
        this.mViewPortHandler.contentRight(),
        this.mViewPortHandler.contentBottom(),
      ]);
    }

    return [this.mAxisLinePaint];
  }

  /**
   * draws the y-labels on the specified x-position
   *
   * @param fixedPosition
   * @param positions
   */
  protected drawYLabels(fixedPosition: number, positions: number[], offset: number): Paint[] {
    let paints = [];
    const fromIndex = this.mYAxis.isDrawBottomYLabelEntryEnabled() ? 0 : 1;
    const to = this.mYAxis.isDrawTopYLabelEntryEnabled() ? this.mYAxis.mEntryCount : this.mYAxis.mEntryCount - 1;

    let xOffset: number = this.mYAxis.getLabelXOffset();

    // draw
    for (let i = fromIndex; i < to; i++) {
      let newLabelPaint = new TextPaint(this.mAxisLabelPaint as TextPaint);
      let text: string = this.mYAxis.getFormattedLabel(i);
      newLabelPaint.setText(text);
      newLabelPaint.setX(fixedPosition + xOffset);
      let interval =
        ((this.mAxisLinePaint as LinePaint).endPoint[1] - (this.mAxisLinePaint as LinePaint).startPoint[1]) / (to - 1);
      let lastNumber = this.mAxis.mEntries[this.mAxis.mEntries.length - 1];
      let topOffset =
        ((this.mAxis.getAxisMaximum() - lastNumber) /
        (lastNumber - this.mAxis.mEntries[this.mAxis.mEntries.length - 2])) *
        interval;
      let bottomOffset =
        ((this.mAxis.mEntries[0] - this.mAxis.mAxisMinimum) / (this.mAxis.mEntries[1] - this.mAxis.mEntries[0])) *
        interval;
      interval =
      ((this.mAxisLinePaint as LinePaint).endPoint[1] -
      (this.mAxisLinePaint as LinePaint).startPoint[1] -
      topOffset -
      bottomOffset) /
      (to - 1);
      let nowOffset = offset;
      if (this.mYAxis.isInverted()) {
        nowOffset += bottomOffset;
        if (!this.mYAxis.isDrawBottomYLabelEntryEnabled()) {
          newLabelPaint.setY(interval * (to - i - 1) + nowOffset);
        } else {
          newLabelPaint.setY(interval * i + nowOffset);
        }
      } else {
        nowOffset += topOffset;
        if (!this.mYAxis.isDrawBottomYLabelEntryEnabled()) {
          newLabelPaint.setY(interval * i + nowOffset);
        } else {
          newLabelPaint.setY(interval * (to - i - 1) + nowOffset);
        }
      }
      if (newLabelPaint.textAlign == TextAlign.End) {
        newLabelPaint.setWidth((this.mAxisLinePaint as LinePaint).startPoint[0]);
      }
      paints.push(newLabelPaint);
    }
    return paints;
  }

  protected mRenderGridLinesPath: string = '';

  public renderGridLines(): Paint[] {
    let paints = [];
    const fromIndex = this.mYAxis.isDrawBottomYLabelEntryEnabled() ? 0 : 1;
    const to = this.mYAxis.isDrawTopYLabelEntryEnabled() ? this.mYAxis.mEntryCount : this.mYAxis.mEntryCount - 1;

    if (!this.mYAxis.isEnabled()) {
      return [];
    }

    if (this.mYAxis.isDrawGridLinesEnabled()) {
      let positions: number[] = this.getTransformedPositions();

      this.mGridPaint.setColor(this.mYAxis.getGridColor());
      this.mGridPaint.setStrokeWidth(this.mYAxis.getGridLineWidth());
      this.mGridPaint.setDashPathEffect(this.mYAxis.getGridDashPathEffect());

      let gridLinePath: string = this.mRenderGridLinesPath;
      gridLinePath = '';

      // draw the grid
      for (let i = 0; i < positions.length; i += 2) {
        let newGridPaint = new PathPaint(this.mGridPaint as PathPaint);
        gridLinePath = '';
        let interval =
          ((this.mAxisLinePaint as LinePaint).endPoint[1] - (this.mAxisLinePaint as LinePaint).startPoint[1]) /
          (to - 1);
        let lastNumber = this.mAxis.mEntries[this.mAxis.mEntries.length - 1];
        let topOffset =
          ((this.mAxis.getAxisMaximum() - lastNumber) /
          (lastNumber - this.mAxis.mEntries[this.mAxis.mEntries.length - 2])) *
          interval;
        let bottomOffset =
          ((this.mAxis.mEntries[0] - this.mAxis.mAxisMinimum) / (this.mAxis.mEntries[1] - this.mAxis.mEntries[0])) *
          interval;
        interval =
        ((this.mAxisLinePaint as LinePaint).endPoint[1] -
        (this.mAxisLinePaint as LinePaint).startPoint[1] -
        topOffset -
        bottomOffset) /
        (to - 1);
        positions[i + 1] = interval * Math.floor(i / 2) + this.mViewPortHandler.offsetTop() + topOffset;
        //
        newGridPaint.setCommands(this.linePath(gridLinePath, i, positions));
        if (this.mYAxis.getGridDashPathEffect() != null) {
          // @ts-ignore
          newGridPaint.setStrokeDashArray(this.mYAxis.getGridDashPathEffect().dash);
          // @ts-ignore
          newGridPaint.setStrokeDashOffset(this.mYAxis.getGridDashPathEffect().offset);
        }
        paints.push(newGridPaint);
      }
    }

    if (this.mYAxis.isDrawZeroLineEnabled()) {
      paints.push(this.drawZeroLine());
    }

    return paints;
  }

  protected mGridClippingRect: MyRect = new MyRect();

  public getGridClippingRect(): MyRect {
    this.mGridClippingRect.set(
    this.mViewPortHandler.getContentRect().left,
    this.mViewPortHandler.getContentRect().top,
    this.mViewPortHandler.getContentRect().right,
    this.mViewPortHandler.getContentRect().bottom
    );

    this.mGridClippingRect.inset(0, -this.mAxis.getGridLineWidth(), 0, -this.mAxis.getGridLineWidth());
    return this.mGridClippingRect;
  }

  /**
   * Calculates the path for a grid line.
   *
   * @param p
   * @param i
   * @param positions
   * @return
   */
  protected linePath(p: string, i: number, positions: number[]): string {
    p =
    'M' +
    Utils.convertDpToPixel((this.mAxisLinePaint as LinePaint).startPoint[0]) +
    ' ' +
    Utils.convertDpToPixel(positions[i + 1]) +
    ' L' +
    Utils.convertDpToPixel(this.mViewPortHandler.contentRight()) +
    ' ' +
    Utils.convertDpToPixel(positions[i + 1]) +
    ' Z';
    return p;
  }

  protected mGetTransformedPositionsBuffer: number[] = new Array<number>(2);
  /**
   * Transforms the values contained in the axis entries to screen pixels and returns them in form of a float array
   * of x- and y-coordinates.
   *
   * @return
   */
  protected getTransformedPositions(): number[] {
    if (this.mGetTransformedPositionsBuffer.length != this.mYAxis.mEntryCount * 2) {
      this.mGetTransformedPositionsBuffer = new Array<number>(this.mYAxis.mEntryCount * 2);
    }
    let positions: number[] = this.mGetTransformedPositionsBuffer;

    for (let i = 0; i < positions.length; i += 2) {
      // only fill y values, x values are not needed for y-labels
      positions[i + 1] = this.mYAxis.mEntries[i / 2];
    }

    this.mTrans.pointValuesToPixel(positions);
    return positions;
  }

  protected mDrawZeroLinePath: string = '';
  protected mZeroLineClippingRect: MyRect = new MyRect();

  /**
   * Draws the zero line.
   */
  protected drawZeroLine(): Paint {
    this.mZeroLineClippingRect.set(
    this.mViewPortHandler.getContentRect().left,
    this.mViewPortHandler.getContentRect().top,
    this.mViewPortHandler.getContentRect().right,
    this.mViewPortHandler.getContentRect().bottom
    );
    this.mZeroLineClippingRect.inset(0, -this.mYAxis.getZeroLineWidth(), 0, -this.mYAxis.getZeroLineWidth());

    // draw zero line
    let pos: MPPointD = this.mTrans.getPixelForValues(0, 0);

    this.mZeroLinePaint.setColor(this.mYAxis.getZeroLineColor());
    this.mZeroLinePaint.setStrokeWidth(this.mYAxis.getZeroLineWidth());

    let zeroLinePath: string = this.mDrawZeroLinePath;
    zeroLinePath =
    'M' +
    this.mViewPortHandler.contentLeft() +
    ' ' +
    pos.y +
    ' L' +
    this.mViewPortHandler.contentRight() +
    ' ' +
    pos.y;
    (this.mZeroLinePaint as PathPaint).setCommands(zeroLinePath);
    return this.mZeroLinePaint;
  }

  protected mRenderLimitLines: string = '';
  protected mRenderLimitLinesBuffer: number[] = new Array<number>(2);
  protected mLimitLineClippingRect: MyRect = new MyRect();

  /**
   * Draws the LimitLines associated with this axis to the screen.
   *
   * @param c
   */
  public renderLimitLines(): Paint[] {
    let limitLines: JArrayList<LimitLine> = this.mYAxis.getLimitLines();
    let paints = [];

    if (limitLines == null || limitLines.size() <= 0) {
      return [];
    }

    let pts: number[] = this.mRenderLimitLinesBuffer;
    pts[0] = 0;
    pts[1] = 0;
    let limitLinePath: string = this.mRenderLimitLines;
    limitLinePath = '';

    for (let i = 0; i < limitLines.size(); i++) {
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
      this.mLimitLineClippingRect.inset(0, -l.getLineWidth(), 0, -l.getLineWidth());

      let newPathLine = new PathPaint();
      newPathLine.set(this.mLimitLinePaint);
      newPathLine.setStyle(Style.STROKE);
      newPathLine.setColor(l.getLineColor());
      newPathLine.setStrokeWidth(l.getLineWidth());
      newPathLine.setDashPathEffect(l.getDashPathEffect());

      pts[1] = l.getLimit();

      this.mTrans.pointValuesToPixel(pts);

      let interval = (this.mAxisLinePaint as LinePaint).endPoint[1] - (this.mAxisLinePaint as LinePaint).startPoint[1];
      let offset =
        interval -
        interval * ((l.getLimit() - this.mAxis.mAxisMinimum) / (this.mAxis.mAxisMaximum - this.mAxis.mAxisMinimum));
      pts[1] = offset + this.mViewPortHandler.offsetTop();

      limitLinePath =
      'M' +
      Utils.convertDpToPixel((this.mAxisLinePaint as LinePaint).startPoint[0]) +
      ' ' +
      Utils.convertDpToPixel(pts[1]) +
      ' L' +
      Utils.convertDpToPixel(this.mViewPortHandler.contentRight()) +
      ' ' +
      Utils.convertDpToPixel(pts[1]);

      (newPathLine as PathPaint).setCommands(limitLinePath);
      limitLinePath = '';
      paints.push(newPathLine);

      let label: string = l.getLabel();
      //
      // if drawing the limit-value label is enabled
      if (label != null && label != '') {
        let textPaint = new TextPaint();
        textPaint.set(this.mLimitLinePaint);
        textPaint.setStyle(l.getTextStyle());
        textPaint.setDashPathEffect(null);
        textPaint.setColor(l.getTextColor());
        textPaint.setTypeface(l.getTypeface());
        textPaint.setStrokeWidth(0.5);
        textPaint.setTextSize(l.getTextSize());

        const labelLineHeight: number = Utils.calcTextHeight(textPaint, label);
        let xOffset: number = Utils.calcTextWidth(textPaint, l.getLabel()) + l.getXOffset() + 4;
        let yOffset: number = l.getLineWidth() + labelLineHeight + l.getYOffset();

        const position: LimitLabelPosition = l.getLabelPosition();

        if (position == LimitLabelPosition.RIGHT_TOP) {
          textPaint.setTextAlign(TextAlign.End);
          textPaint.setText(label);
          textPaint.setX(this.mViewPortHandler.contentRight() - xOffset);
          textPaint.setY(pts[1] - yOffset);
          paints.push(textPaint);
        } else if (position == LimitLabelPosition.RIGHT_BOTTOM) {
          textPaint.setTextAlign(TextAlign.End);
          textPaint.setText(label);
          textPaint.setX(this.mViewPortHandler.contentRight() - xOffset);
          textPaint.setY(pts[1] + l.getYOffset());
          paints.push(textPaint);
        } else if (position == LimitLabelPosition.LEFT_TOP) {
          textPaint.setTextAlign(TextAlign.Start);
          textPaint.setText(label);
          textPaint.setX(this.mViewPortHandler.contentLeft() + xOffset);
          textPaint.setY(pts[1] - yOffset);
          paints.push(textPaint);
        } else {
          textPaint.setTextAlign(TextAlign.Start);
          textPaint.setText(label);
          textPaint.setX(this.mViewPortHandler.offsetLeft() + xOffset);
          textPaint.setY(pts[1] + l.getYOffset());
          paints.push(textPaint);
        }
      }
    }

    return paints;
  }
}
