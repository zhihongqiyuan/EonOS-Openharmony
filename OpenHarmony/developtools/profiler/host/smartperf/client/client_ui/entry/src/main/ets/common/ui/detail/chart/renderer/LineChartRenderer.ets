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

import YAxis, { AxisDependency } from '../components/YAxis';
import EntryOhos from '../data/EntryOhos';
import MyRect from '../data/Rect';
import { LineDataSet, Mode } from '../data/LineDataSet';
import MPPointD from '../utils/MPPointD';
import { PathViewModel } from '../components/PathView';
import { XAxis } from '../components/XAxis';
import LineDataProvider from '../interfaces/dataprovider/LineDataProvider';
import LineData from '../data/LineData';
import Paint, { PathPaint, TextPaint, CirclePaint, PathFillPaint, LinePaint, ImagePaint } from '../data/Paint';
import { Rounding } from '../data/DataSet';
import Utils from '../utils/Utils';
import { FillStyle } from '../data/LineDataSet';
import LineScatterCandleRadarRenderer from '../renderer/LineScatterCandleRadarRenderer';

export default class LineChartRenderer extends LineScatterCandleRadarRenderer {
  protected mChart: LineDataProvider;
  private min;
  private max: number;
  private range: number;
  private phaseY: number = 1;
  private phaseX: number = 1;
  private isInverted: boolean = false;
  private pathViewModel: PathViewModel;
  private lineData: LineData;
  private yleftAxis: YAxis;
  private yRightAxis: YAxis;
  private animateXIndex: number = Number.MAX_VALUE;
  private animateYValue: number = 1;

  private mLineBuffer: number[] = new Array(4);

  private clickPaint: Paint[] = [];
  private xStartPoint: number[] = [];
  private xEndPoint: number[] = [];
  private yStartPoint: number[] = [];
  private yEndPoint: number[] = [];

  constructor(pathViewModel: PathViewModel, yleftAxis: YAxis, yRightAxis: YAxis, isInverted: boolean) {
    super(null, null);
    this.pathViewModel = pathViewModel;
    this.lineData = pathViewModel.getLineData();
    this.yleftAxis = yleftAxis;
    this.yRightAxis = yRightAxis;
    this.isInverted = isInverted;
  }

  public drawExtras(): Paint[] {
    return [];
  }

  public drawHighlighted(): Paint[] {
    // 1、拿到点击的物理坐标，转换成图形坐标
    // 2、拿点击转换后的坐标和数据中所有的点作比较，取最近的点，
    // 3、以最近的点为中心画十字架
    this.clickPaint = [];

    let x = this.pathViewModel.eventX;
    let y = this.pathViewModel.eventY;

    if (x > 0 && x < this.pathViewModel.rect.right && y > 0 && y < this.pathViewModel.rect.bottom) {
      let ccTemp: number = Number.MAX_VALUE;
      let positionX: number;
      let positionY: number;
      let entryX: number;
      let entryY: number;
      let maxY: number;
      let isLeftAxis;
      let yAxis: YAxis;
      let yScale: number;
      let size = this.lineData.getDataSets().length();
      for (let i = 0; i < size; i++) {
        let entryArray: EntryOhos[] = (this.lineData.getDataSetByIndex(i) as LineDataSet).getEntries().toArray();
        for (let j = 0; j < entryArray.length; j++) {
          isLeftAxis = this.pathViewModel.lineData.getDataSetByIndex(i).getAxisDependency() == AxisDependency.LEFT;
          yAxis = isLeftAxis ? this.pathViewModel.yleftAxis : this.pathViewModel.yRightAxis;
          yScale = isLeftAxis ? this.pathViewModel.yLeftScale : this.pathViewModel.yRightScale;

          entryX =
          entryArray[j].getX() * this.pathViewModel.xScale * this.pathViewModel.scaleX +
          this.pathViewModel.moveX -
          this.pathViewModel.currentXSpace;
          maxY = yAxis.getAxisMaximum();
          if (!this.pathViewModel.isInverted) {
            entryY = (maxY - entryArray[j].getY()) * yScale;
          } else {
            entryY = entryArray[j].getY() * yScale;
          }

          entryY = entryY * this.pathViewModel.scaleY + this.pathViewModel.moveY - this.pathViewModel.currentYSpace;
          let a = Math.abs(x - entryX);
          let b = Math.abs(y - entryY);
          let cc = Math.sqrt(a * a + b * b);

          if (ccTemp > cc) {
            ccTemp = cc;
            positionX = entryX;
            positionY = entryY;
          }
        }
      }

      let textPaint: TextPaint = new TextPaint();
      let value: string = Math.floor(maxY - positionY / yScale).toString();
      textPaint.setText(value);
      textPaint.setColor(Color.White);
      textPaint.setX(positionX - Utils.calcTextWidth(textPaint, value) / 2);
      textPaint.setY(
        positionY - Utils.calcTextHeight(textPaint, value) - (Utils.calcTextHeight(textPaint, value) + 20) / 2
      );

      let imagePaint: ImagePaint = new ImagePaint();
      imagePaint.setWidth(Utils.calcTextWidth(textPaint, value) + 20);
      imagePaint.setHeight(Utils.calcTextHeight(textPaint, value) + 20);
      imagePaint.setX(positionX - (Utils.calcTextWidth(textPaint, value) + 20) / 2);
      imagePaint.setY(positionY - Utils.calcTextHeight(textPaint, value) - 20);

      let yLinePaint: LinePaint = new LinePaint();
      yLinePaint.setStartPoint([positionX, 0]);
      yLinePaint.setEndPoint([positionX, this.pathViewModel.rect.bottom - this.pathViewModel.minOffset]);
      yLinePaint.setColor(Color.Red);
      yLinePaint.setStrokeWidth(0.5);

      let xLinePaint: LinePaint = new LinePaint();
      xLinePaint.setStartPoint([0, positionY]);
      if (!yAxis || yAxis == null || yAxis == undefined) {
        yAxis = this.pathViewModel?.yleftAxis || this.yleftAxis;
      }
      xLinePaint.setEndPoint([
        this.pathViewModel.rect.right -
        this.pathViewModel.minOffset -
        Utils.calcTextWidth(textPaint, yAxis.getLongestLabel()),
        positionY,
      ]);
      xLinePaint.setColor(Color.Red);
      xLinePaint.setStrokeWidth(0.5);

      this.clickPaint.push(xLinePaint);
      this.clickPaint.push(yLinePaint);
      this.clickPaint.push(imagePaint);
      this.clickPaint.push(textPaint);
    }
    return this.clickPaint;
  }

  public initBuffers() {}

  public drawData(): Paint[] {
    let pathPaintArr: Paint[] = [];

    let firstPointX: number = 0;
    let lastPointX: number = 0;
    let minPointY: number = 0;
    let maxPointY: number = 0;

    let xScale: number = this.pathViewModel.xScale;
    let yScale: number = 1;
    let lineDataSet: LineDataSet;
    for (let i = 0; i < this.lineData.getDataSetCount(); i++) {
      lineDataSet = this.lineData.getDataSetByIndex(i) as LineDataSet;
      if (lineDataSet.getEntries().size() <= 0) {
        continue;
      }

      this.animateXIndex =
        this.animateXIndex <= lineDataSet.getEntries().length() - 1
        ? this.animateXIndex
        : lineDataSet.getEntries().length() - 1;

      firstPointX = Utils.convertDpToPixel(
      this.getXPosition(lineDataSet.getEntries().at(0).getX() * xScale * this.pathViewModel.scaleX)
      );
      lastPointX = Utils.convertDpToPixel(
      this.getXPosition(lineDataSet.getEntries().at(this.animateXIndex).getX() * xScale)
      );
      if (lineDataSet.getAxisDependency() == AxisDependency.LEFT) {
        yScale = this.pathViewModel.yLeftScale;
        minPointY = Utils.convertDpToPixel(
        this.getYPosition(
          (this.yleftAxis.getAxisMaximum() - this.yleftAxis.getAxisMinimum() * this.animateYValue) * yScale
        )
        );
        maxPointY = this.getYPosition(0); // 屏幕上Y值的0点在最上方，而图例的最大是在最上方，所以取0
      } else {
        yScale = this.pathViewModel.yRightScale;
        minPointY = Utils.convertDpToPixel(
        this.getYPosition(
          (this.yRightAxis.getAxisMaximum() - this.yRightAxis.getAxisMinimum() * this.animateYValue) * yScale
        )
        );
        maxPointY = this.getYPosition(0);
      }

      let pathPaint: Paint[] = this.computePathDataSet(
        lineDataSet,
        xScale,
        yScale,
        firstPointX,
        lastPointX,
        minPointY,
        maxPointY
      );
      pathPaintArr = pathPaintArr.concat(pathPaint);
    }

    return pathPaintArr;
  }

  protected computePathDataSet(
    dataSet: LineDataSet,
    xScale: number,
    yScale: number,
    firstPointX: number,
    lastPointX: number,
    minPointY: number,
    maxPointY: number
  ): Paint[] {
    if (this.lineData.getDataSetCount() < 1) {
      return null;
    }

    let chartMode: Mode = dataSet.getMode();
    switch (chartMode) {
      case Mode.LINEAR:
      case Mode.STEPPED:
        return this.computeLinear(dataSet, xScale, yScale, firstPointX, lastPointX, minPointY, maxPointY);
        break;
      case Mode.CUBIC_BEZIER:
        return this.computeCubicBezier(dataSet, xScale, yScale, firstPointX, lastPointX, minPointY, maxPointY);
        break;
      case Mode.HORIZONTAL_BEZIER:
        return this.computeHorizontalBezier(dataSet, xScale, yScale, firstPointX, lastPointX, minPointY, maxPointY);
        break;

      default:
        break;
    }
  }

  protected computeCubicBezier(
    dataSet: LineDataSet,
    xScale: number,
    yScale: number,
    firstPointX: number,
    lastPointX: number,
    minY: number,
    maxY: number
  ): Paint[] {
    let intensity = dataSet.getCubicIntensity();

    let mXAxis = new XAxis();
    let posForGetLowestVisibleX = MPPointD.getInstance(0, 0);
    let posForGetHighestVisibleX = MPPointD.getInstance(0, 0);

    let low = Math.max(mXAxis.mAxisMinimum, posForGetLowestVisibleX.x);
    let high = Math.min(mXAxis.mAxisMaximum, posForGetHighestVisibleX.x);

    let entryFrom = dataSet.getEntryForXValue(low, Number.NaN, Rounding.DOWN);
    let entryTo = dataSet.getEntryForXValue(high, Number.NaN, Rounding.UP);

    this.min = entryFrom == null ? 0 : dataSet.getEntryIndexByEntry(entryFrom);
    this.max = entryTo == null ? 0 : dataSet.getEntryIndexByEntry(entryTo);

    this.range = dataSet.getEntryCount() - 1;

    if (this.range >= 1) {
      let prevDx = 0;
      let prevDy = 0;
      let curDx = 0;
      let curDy = 0;

      let firstIndex: number = this.min + 1;
      let lastIndex: number = this.min + this.range;

      let prevPrev;
      let prev = dataSet.getEntryForIndex(Math.max(firstIndex - 2, 0));
      let cur = dataSet.getEntryForIndex(Math.max(firstIndex - 1, 0));
      let next = cur;
      let nextIndex = -1;

      if (cur == null) {
        return;
      }

      var commandsData: string = '';
      var commandsDataFill: string = '';

      if (dataSet.getFillStyle() == FillStyle.MIN) {
        commandsDataFill = 'M' + firstPointX + ' ' + minY;
      } else {
        commandsDataFill = 'M' + firstPointX + ' ' + minY;
      }

      let startAddY: number;
      let yAxis: YAxis = dataSet.getAxisDependency() == AxisDependency.LEFT ? this.yleftAxis : this.yRightAxis;
      if (!this.isInverted) {
        startAddY = Utils.convertDpToPixel(
        this.getYPosition((yAxis.getAxisMaximum() - cur.getY() * this.animateYValue) * yScale)
        );
      } else {
        startAddY = Utils.convertDpToPixel(this.getYPosition(cur.getY() * this.animateYValue * yScale));
      }

      commandsDataFill += ' L' + Utils.convertDpToPixel(this.getXPosition(cur.getX() * xScale)) + ' ' + startAddY;

      commandsData = 'M' + Utils.convertDpToPixel(this.getXPosition(cur.getX() * xScale)) + ' ' + startAddY;

      for (let j = this.min + 1; j <= this.range + this.min; j++) {
        if (j > this.animateXIndex) {
          break;
        }
        prevPrev = prev;
        prev = cur;
        cur = nextIndex == j ? next : dataSet.getEntryForIndex(j);

        nextIndex = j + 1 < dataSet.getEntryCount() ? j + 1 : j;
        next = dataSet.getEntryForIndex(nextIndex);

        prevDx = (cur.getX() - prevPrev.getX()) * intensity;
        prevDy =
        (yAxis.getAxisMaximum() -
        cur.getY() * this.animateYValue -
        (yAxis.getAxisMaximum() - prevPrev.getY() * this.animateYValue)) *
        intensity;
        curDx = (next.getX() - prev.getX()) * intensity;
        curDy = 0;

        let realY1: number = this.isInverted
          ? prev.getY() * this.animateYValue + prevDy
          : yAxis.getAxisMaximum() - prev.getY() * this.animateYValue + prevDy;
        let x1 = Utils.convertDpToPixel(this.getXPosition((prev.getX() + prevDx) * xScale));
        let y1 = Utils.convertDpToPixel(this.getYPosition(realY1 * yScale));

        let realY2: number = this.isInverted
          ? cur.getY() * this.animateYValue
          : yAxis.getAxisMaximum() - cur.getY() * this.animateYValue;
        let x2 = Utils.convertDpToPixel(this.getXPosition((cur.getX() - curDx) * xScale));
        let y2 = Utils.convertDpToPixel(this.getYPosition((realY2 - curDy) * yScale));

        let realY3: number = this.isInverted
          ? cur.getY() * this.animateYValue
          : yAxis.getAxisMaximum() - cur.getY() * this.animateYValue;
        let x3 = Utils.convertDpToPixel(this.getXPosition(cur.getX() * xScale));
        let y3 = Utils.convertDpToPixel(this.getYPosition(realY3 * yScale));

        commandsData += ' C' + x1 + ' ' + y1 + ' ' + x2 + ' ' + y2 + ' ' + x3 + ' ' + y3;
        commandsDataFill += ' C' + x1 + ' ' + y1 + ' ' + x2 + ' ' + y2 + ' ' + x3 + ' ' + y3;
      }
      if (dataSet.getFillStyle() == FillStyle.MIN) {
        commandsDataFill += ' L' + lastPointX + ' ' + minY;
      } else {
        commandsDataFill += ' L' + lastPointX + ' ' + maxY;
      }
    }

    return this.creatPathPaintArr(dataSet, commandsData, commandsDataFill);
  }

  private getXPosition(x: number): number {
    return x * this.pathViewModel.scaleX + this.pathViewModel.moveX - this.pathViewModel.currentXSpace;
  }

  private getYPosition(y: number): number {
    return y * this.pathViewModel.scaleY + this.pathViewModel.moveY - this.pathViewModel.currentYSpace;
  }

  private computeLinear(
    dataSet: LineDataSet,
    xScale: number,
    yScale: number,
    firstPointX: number,
    lastPointX: number,
    minY: number,
    maxY: number
  ): Paint[] {
    let x: number = 0;
    let y: number = 0;

    var commandsData: string = '';
    var commandsFillData: string = '';

    if (dataSet.getFillStyle() == FillStyle.MIN) {
      commandsFillData = 'M' + firstPointX + ' ' + minY;
    } else {
      commandsFillData = 'M' + firstPointX + ' ' + maxY;
    }

    let entryArray: EntryOhos[] = [];
    entryArray = dataSet.getEntries().toArray();
    let entryOhosY: number;
    for (let i = 0; i < entryArray.length; i++) {
      if (i > this.animateXIndex) {
        break;
      }
      if (!this.isInverted) {
        if (dataSet.getAxisDependency() == AxisDependency.LEFT) {
          entryOhosY = this.yleftAxis.getAxisMaximum() - entryArray[i].getY() * this.animateYValue;
        } else {
          entryOhosY = this.yRightAxis.getAxisMaximum() - entryArray[i].getY() * this.animateYValue;
        }
      } else {
        entryOhosY = entryArray[i].getY() * this.animateYValue;
      }

      x = Utils.convertDpToPixel(this.getXPosition(entryArray[i].getX() * xScale));
      y = Utils.convertDpToPixel(this.getYPosition(entryOhosY * yScale));
      if (i == 0) {
        commandsData = 'M' + x + ' ' + y;
      } else {
        commandsData += ' L' + x + ' ' + y;
      }
      commandsFillData += ' L' + x + ' ' + y;
    }
    if (dataSet.getFillStyle() == FillStyle.MIN) {
      commandsFillData += ' L' + lastPointX + ' ' + minY;
    } else {
      commandsFillData += ' L' + lastPointX + ' ' + maxY;
    }

    return this.creatPathPaintArr(dataSet, commandsData, commandsFillData);
  }

  private computeHorizontalBezier(
    dataSet: LineDataSet,
    xScale: number,
    yScale: number,
    firstPointX: number,
    lastPointX: number,
    minY: number,
    maxY: number
  ): Paint[] {
    let mXAxis = new XAxis();
    let posForGetLowestVisibleX = MPPointD.getInstance(0, 0);
    let posForGetHighestVisibleX = MPPointD.getInstance(0, 0);

    let low = Math.max(mXAxis.mAxisMinimum, posForGetLowestVisibleX.x);
    let high = Math.min(mXAxis.mAxisMaximum, posForGetHighestVisibleX.x);

    let entryFrom = dataSet.getEntryForXValue(low, Number.NaN, Rounding.DOWN);
    let entryTo = dataSet.getEntryForXValue(high, Number.NaN, Rounding.UP);

    this.min = entryFrom == null ? 0 : dataSet.getEntryIndexByEntry(entryFrom);
    this.max = entryTo == null ? 0 : dataSet.getEntryIndexByEntry(entryTo);
    this.range = dataSet.getEntryCount() - 1;

    if (this.range >= 1) {
      let prev: EntryOhos = dataSet.getEntryForIndex(this.min);
      let cur: EntryOhos = prev;

      var commandsData: string = '';
      var commandsDataFill: string = '';

      if (dataSet.getFillStyle() == FillStyle.MIN) {
        commandsDataFill = 'M' + firstPointX + ' ' + minY;
      } else {
        commandsDataFill = 'M' + firstPointX + ' ' + maxY;
      }

      let startAddY: number;
      let yAxis: YAxis = dataSet.getAxisDependency() == AxisDependency.LEFT ? this.yleftAxis : this.yRightAxis;
      if (!this.isInverted) {
        startAddY = Utils.convertDpToPixel(
        this.getYPosition((yAxis.getAxisMaximum() - cur.getY() * this.animateYValue) * yScale)
        );
      } else {
        startAddY = Utils.convertDpToPixel(this.getYPosition(cur.getY() * this.animateYValue * yScale));
      }

      commandsDataFill += ' L' + Utils.convertDpToPixel(this.getXPosition(cur.getX() * xScale)) + ' ' + startAddY;

      commandsData = 'M' + Utils.convertDpToPixel(this.getXPosition(cur.getX()) * xScale) + ' ' + startAddY;

      for (let j = this.min + 1; j <= this.range + this.min; j++) {
        if (j > this.animateXIndex) {
          break;
        }

        prev = cur;
        cur = dataSet.getEntryForIndex(j);

        let realY1: number = this.isInverted
          ? prev.getY() * this.animateYValue
          : yAxis.getAxisMaximum() - prev.getY() * this.animateYValue;
        let x1 = Utils.convertDpToPixel(this.getXPosition((prev.getX() + (cur.getX() - prev.getX()) / 2) * xScale));
        let y1 = Utils.convertDpToPixel(this.getYPosition(realY1 * yScale));

        let realY2: number = this.isInverted
          ? cur.getY() * this.animateYValue
          : yAxis.getAxisMaximum() - cur.getY() * this.animateYValue;
        let x2 = Utils.convertDpToPixel(this.getXPosition((prev.getX() + (cur.getX() - prev.getX()) / 2) * xScale));
        let y2 = Utils.convertDpToPixel(this.getYPosition(realY2 * yScale));

        let realY3: number = this.isInverted
          ? cur.getY() * this.animateYValue
          : yAxis.getAxisMaximum() - cur.getY() * this.animateYValue;
        let x3 = Utils.convertDpToPixel(this.getXPosition(cur.getX() * xScale));
        let y3 = Utils.convertDpToPixel(this.getYPosition(realY3 * yScale));

        commandsData += ' C' + x1 + ' ' + y1 + ' ' + x2 + ' ' + y2 + ' ' + x3 + ' ' + y3;
        commandsDataFill += ' C' + x1 + ' ' + y1 + ' ' + x2 + ' ' + y2 + ' ' + x3 + ' ' + y3;
      }
      if (dataSet.getFillStyle() == FillStyle.MIN) {
        commandsDataFill += ' L' + lastPointX + ' ' + minY;
      } else {
        commandsDataFill += ' L' + lastPointX + ' ' + maxY;
      }
    }

    return this.creatPathPaintArr(dataSet, commandsData, commandsDataFill);
  }

  public drawCircle(): Paint[] {
    let circlePaints: Paint[] = [];

    let xScale: number = this.pathViewModel.xScale;
    let yScale: number = 1;
    for (let index = 0; index < this.lineData.getDataSetCount(); index++) {
      let lineDataSet: LineDataSet = this.lineData.getDataSetByIndex(index) as LineDataSet;
      if (!lineDataSet.isDrawCirclesEnabled()) {
        continue;
      }

      if (lineDataSet.getAxisDependency() == AxisDependency.LEFT) {
        yScale = this.pathViewModel.yLeftScale;
      } else {
        yScale = this.pathViewModel.yRightScale;
      }

      let entryOhosY: number;
      for (let i = 0; i < lineDataSet.getEntries().length(); i++) {
        if (i > this.animateXIndex) {
          break;
        }
        let circlePaint: CirclePaint = new CirclePaint();

        if (!this.isInverted) {
          if (lineDataSet.getAxisDependency() == AxisDependency.LEFT) {
            entryOhosY = this.yleftAxis.getAxisMaximum() - lineDataSet.getEntries().at(i).getY() * this.animateYValue;
          } else {
            entryOhosY = this.yRightAxis.getAxisMaximum() - lineDataSet.getEntries().at(i).getY() * this.animateYValue;
          }
        } else {
          entryOhosY = lineDataSet.getEntries().at(i).getY() * this.animateYValue;
        }
        let xx: number = lineDataSet.getEntries().at(i).getX() * xScale;
        let yy: number = entryOhosY * yScale;
        circlePaint.setDrawCircles(lineDataSet.isDrawCirclesEnabled());
        circlePaint.setCirclesColor(lineDataSet.getCircleColor());
        circlePaint.setCircleRadius(lineDataSet.getCircleRadius());
        circlePaint.setDrawCircleHole(lineDataSet.isDrawCircleHoleEnabled());
        circlePaint.setCircleHoleRadius(lineDataSet.getCircleHoleRadius());
        circlePaint.setCircleHoleColor(lineDataSet.getCircleHoleColor());
        circlePaint.setX(xx * this.pathViewModel.scaleX + this.pathViewModel.moveX - this.pathViewModel.currentXSpace);
        circlePaint.setY(yy * this.pathViewModel.scaleY + this.pathViewModel.moveY - this.pathViewModel.currentYSpace);

        circlePaints.push(circlePaint);
      }
    }
    return circlePaints;
  }

  public drawValues(): Paint[] {
    let textPaints: Paint[] = [];

    let xScale: number = this.pathViewModel.xScale;
    let yScale: number = 1;
    let entryArray: EntryOhos[] = [];

    for (let i = 0; i < this.lineData.getDataSetCount(); i++) {
      let lineDataSet: LineDataSet = this.pathViewModel.getLineData().getDataSetByIndex(i) as LineDataSet;
      if (!lineDataSet.isDrawValuesEnabled()) {
        return [];
      }

      entryArray = lineDataSet.getEntries().toArray();

      if (lineDataSet.getAxisDependency() == AxisDependency.LEFT) {
        yScale = this.pathViewModel.yLeftScale;
      } else {
        yScale = this.pathViewModel.yRightScale;
      }

      let entryOhosY: number;
      let xx: number;
      let yy: number;
      let value: string;
      for (let i = 0; i < entryArray.length; i++) {
        if (i > this.animateXIndex) {
          break;
        }
        let textPaint: TextPaint = new TextPaint();
        if (!this.isInverted) {
          if (lineDataSet.getAxisDependency() == AxisDependency.LEFT) {
            entryOhosY = this.yleftAxis.getAxisMaximum() - entryArray[i].getY() * this.animateYValue;
          } else {
            entryOhosY = this.yRightAxis.getAxisMaximum() - entryArray[i].getY() * this.animateYValue;
          }
        } else {
          entryOhosY = entryArray[i].getY() * this.animateYValue;
        }
        value = Math.floor(entryArray[i].getY()).toString();
        xx = entryArray[i].getX() * xScale * this.pathViewModel.scaleX - Utils.calcTextWidth(textPaint, value) / 2;
        yy = entryOhosY * yScale * this.pathViewModel.scaleY - Utils.calcTextHeight(textPaint, value) - 2;
        textPaint.setText(value);
        textPaint.setX(xx + this.pathViewModel.moveX - this.pathViewModel.currentXSpace);
        textPaint.setY(yy + this.pathViewModel.moveY - this.pathViewModel.currentYSpace);
        textPaints.push(textPaint);
      }
    }
    return textPaints;
  }

  private creatPathPaintArr(dataSet: LineDataSet, commandsData: string, commandsDataFill?: string): Paint[] {
    let pathPaints: Paint[] = [];

    let pathPaint: PathPaint = new PathPaint();
    pathPaint.setCommands(commandsData);
    pathPaint.setAxisDependency(dataSet.getAxisDependency());
    pathPaint.setStrokeWidth(dataSet.getLineWidth());
    pathPaint.setStroke(dataSet.getColor());
    pathPaint.setDashPathEffect(dataSet.getDashPathEffect());

    if (dataSet.isDrawFilledEnabled()) {
      let pathFillPaint: PathFillPaint = new PathFillPaint();
      pathFillPaint.setCommandsFill(commandsDataFill);
      pathFillPaint.setDrawFilled(dataSet.isDrawFilledEnabled());
      pathFillPaint.setGradientFillColor(dataSet.getGradientFillColor());
      pathPaints.push(pathFillPaint);
    }

    pathPaints.push(pathPaint);

    return pathPaints;
  }

  public animateX(animateIndex: number) {
    this.animateXIndex = animateIndex;
  }

  public animateY(animateYValue: number) {
    this.animateYValue = animateYValue;
  }
}
