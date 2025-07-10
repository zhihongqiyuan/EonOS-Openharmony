/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the 'License');
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an 'AS IS' BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

import { LineChartModel } from '../charts/LineChart';
import ScaleMode from '../data/ScaleMode';
import MyRect from '../data/Rect';
import Paint from '../data/Paint';
import { XAxis } from './XAxis';
import YAxis from './YAxis';
import {PathPaint} from '../data/Paint';
import { CirclePaint, PathFillPaint, TextPaint, LinePaint, ImagePaint} from '../data/Paint';
import LineChartRenderer from '../renderer/LineChartRenderer';
import LineData from '../data/LineData';
import Utils from '../utils/Utils';

@Component
@Preview
export default struct PathView {
  @State
  model:PathViewModel = new PathViewModel();

  public aboutToAppear() {
    this.model.calcClipPath();
  }
  public test() : boolean {
    console.log('pathview:' + this.model.width)
    return true
  }

  build() {
    Stack({ alignContent: Alignment.TopStart }) {
      if (this.test()) { }
      if (this.model.paintArry.length > 0) {
        ForEach(this.model.paintArry, (item: Paint) => {

          if (item instanceof PathPaint) {
            Shape() {
              Path()
                .commands(item.commands)
                .strokeWidth(item.strokeWidth)
                .stroke(item.stroke)
                .strokeDashArray(item.strokeDashArray)
                .strokeDashOffset(item.strokeDashOffset)
            }
            .fill('none')
          } else if (item instanceof PathFillPaint) {
            Shape() {
              Path()
                .commands(item.commandsFill)
            }
            .fill('none')
            .linearGradient({ colors: item.linearGradientColors })
            .clip(new Path().commands(item.commandsFill))
          } else if (item instanceof TextPaint) {

            Text(item.text)
              .fontSize(8)
              .position({ x: item.getX(), y: item.getY() })
          } else if (item instanceof CirclePaint) {
            if (item.isDrawCirclesEnabled()) {
              Circle({ width: item.radius * 2, height: item.radius * 2 })
                .position({ x: item.getX() - item.radius, y: item.getY() - item.radius })
                .fill(item.colors != null ? item.colors.at(0) : item.circleColor)
              if (item.circleHoleEnabled) {
                Circle({ width: item.circleHoleRadius * 2, height: item.circleHoleRadius * 2 })
                  .position({ x: item.getX() - item.circleHoleRadius, y: item.getY() - item.circleHoleRadius })
                  .fill(item.circleHoleColor)
              }
            }
          }
        }, (item: Paint) => '')
        if (this.model.heightlightPaint.length > 0) {
          ForEach(this.model.heightlightPaint, (item: Paint) => {
            if (item instanceof LinePaint) {
              Line()
                .startPoint(item.startPoint)
                .endPoint(item.endPoint)
                .fill(item.color)
                .stroke(item.color)
                .strokeWidth(item.strokeWidth)
                .position({ x: 0, y: 0 })
                .visibility(this.model.isShowHeightlight ? Visibility.Visible : Visibility.None)
            } else if (item instanceof ImagePaint) {
            } else if (item instanceof TextPaint) {
              Text(item.text)
                .position({ x: item.x, y: item.y })
                .fontSize(item.textSize)
                .textAlign(item.textAlign)
                .fontColor(item.color)
                .visibility(this.model.isShowClickValue ? Visibility.Visible : Visibility.None)
            }
          }, (item: Paint) => '')
        }
      }
    }
    .width(this.model.width)
    .height(this.model.height)
    .backgroundColor(this.model.backgroundColor)
    .clip(new Path().commands(this.model.clipPath))
    .position({ x: this.model.positionX, y: this.model.minOffset })
    .onClick((event: ClickEvent)=>{
      this.model.onClick(event);
    })
    .onTouch((event: TouchEvent) => {
      this.model.onTouch(event)
    })
  }

}

export class PathViewModel extends ScaleMode {
  public yleftAxis: YAxis;
  public yRightAxis: YAxis;
  public xAxis: XAxis;
  public width: number = 300;
  public height: number = 300;
  public isInverted: boolean = false;
  public isShowHeightlight: boolean = true;
  public isShowClickValue: boolean = true;
  public minOffset: number = 15; //X轴线偏移量
  public positionX: number = 0;
  public lineData: LineData = new LineData()
  public xScale: number = 1;
  public yLeftScale: number = 1;
  public yRightScale: number = 1;
  public backgroundColor: Color | string | number = '#00FFFFFF'; // chart区域背景色 默认透明色
  public rect: MyRect;
  public paintArry: Paint[] = [];
  public valuePaint: Paint[] = [];
  public heightlightPaint: Paint[] = [];
  public mRenderer: LineChartRenderer;
  public eventX: number = 0;
  public eventY: number = 0;
  public clickTransX: number = 0;
  public clickTransY: number = 0;
  public clipPath: string = '';
  public lineChartModel: LineChartModel = null;

  public setYLeftAxis(yleftAxis: YAxis) : void {
    this.yleftAxis = yleftAxis;
  }

  public setYRightAxis(yRightAxis: YAxis) : void {
    this.yRightAxis = yRightAxis;
  }

  public setXAxis(xAxis: XAxis) : void {
    this.xAxis = xAxis;
  }

  public setIsInverted(isInverted: boolean) : void {
    this.isInverted = isInverted;
  }

  public setIsShowHeightlight(isShowHeightlight: boolean) : void {
    this.isShowHeightlight = isShowHeightlight;
  }

  public setIsShowClickValue(isShowClickValue: boolean) : void {
    this.isShowClickValue = isShowClickValue;
  }

  public getLineData() : LineData {
    return this.lineData;
  }

  public setPathViewData(lineData: LineData) : void {
    this.lineData = lineData;

    this.computerScale();

    this.mRenderer = new LineChartRenderer(this, this.yleftAxis, this.yRightAxis, this.isInverted);

    this.ondraw();
  }

  private computerScale() : void {
    this.rect = this.lineData.mDisplayRect;

    this.positionX = this.rect.left;

    this.xScale = (this.rect.right - this.rect.left) / (this.xAxis.getAxisMaximum() - this.xAxis.getAxisMinimum());
    this.yLeftScale = (this.rect.bottom - this.rect.top) /
        (this.yleftAxis.getAxisMaximum() - this.yleftAxis.getAxisMinimum());

    this.yRightScale = (this.rect.bottom - this.rect.top) /
        (this.yRightAxis.getAxisMaximum() - this.yRightAxis.getAxisMinimum());

  }

  private ondraw() {
    let pathPaint: Paint[] = this.mRenderer.drawData();
    let circlePaint: Paint[] = this.mRenderer.drawCircle();
    this.valuePaint = this.mRenderer.drawValues();
    this.heightlightPaint = this.mRenderer.drawHighlighted();
    this.paintArry = [];
    this.paintArry = this.paintArry.concat(pathPaint);
    this.paintArry = this.paintArry.concat(circlePaint);
    this.paintArry = this.paintArry.concat(this.valuePaint);
  }
  public onSingleClick(event: ClickEvent) {
    this.eventX = event.x;
    this.eventY = event.y;
    this.heightlightPaint = [];
    this.heightlightPaint = this.mRenderer.drawHighlighted();
  }
  public transX: number = 0;
  public transMaxY: number = 0;
  public transMinY: number = 0;
  public onDoubleClick(event: ClickEvent) {
    this.currentXSpace = this.centerX * this.scaleX - this.centerX;
    this.currentYSpace = this.centerY * this.scaleY - this.centerY;
    let maxXBefore: number = 0;
    try {
      if (this.paintArry.length > 0) {
        maxXBefore = this.paintArry[this.paintArry.length - 1].x;
      }
    } catch (error) {
      console.log(error);
    }
    let maxYBefore: number = Number.MIN_VALUE;
    let minYBefore: number = Number.MAX_VALUE;
    for (let i = 0; i < this.valuePaint.length; i++) {
      if (maxYBefore < this.valuePaint[i].y) {
        maxYBefore = this.valuePaint[i].y;
      }
      if (minYBefore > this.valuePaint[i].y) {
        minYBefore = this.valuePaint[i].y;
      }
    }
    this.ondraw();
    let tag: string = 'maxXAfter';
    let maxXAfter: number = 0;
    try {
      if (this.paintArry.length > 0) {
        maxXAfter = this.paintArry[this.paintArry.length - 1].x;
      }
    } catch (error) {
      console.log(error);
    }
    let maxYAfter: number = Number.MIN_VALUE;
    let minYAfter: number = Number.MAX_VALUE;
    for (let i = 0; i < this.valuePaint.length; i++) {
      if (maxYAfter < this.valuePaint[i].y) {
        maxYAfter = this.valuePaint[i].y;
      }
      if (minYAfter > this.valuePaint[i].y) {
        minYAfter = this.valuePaint[i].y;
      }
    }
    this.transX += maxXAfter - maxXBefore;
    this.transMaxY += maxYAfter - maxYBefore;
    this.transMinY += minYAfter - minYBefore;
    if (this.lineChartModel != null) {
      this.lineChartModel.test = '' + this.currentYSpace;
    }
    this.setXPosition(this.moveX - this.currentXSpace)
    let moveYSource = this.leftAxisModel.lastHeight * this.scaleY - this.leftAxisModel.lastHeight
    this.leftAxisModel.translate(moveYSource + this.moveY - this.currentYSpace);

  }

  public onMove(event: TouchEvent) {
    let finalMoveX = this.currentXSpace - this.moveX
    let finalMoveY = this.currentYSpace - this.moveY
    if (this.moveX > 0 && finalMoveX <= 0) {
      this.moveX = this.currentXSpace
    }
    if (this.moveY > 0 && finalMoveY <= 0) {
      this.moveY = this.currentYSpace
    }

    if (this.moveX - this.currentXSpace <= this.width - this.xAixsMode.width) {
      this.moveX = this.width - this.xAixsMode.width + this.currentXSpace
    }
    let scaleYHeight = this.height * this.scaleY
    if (this.moveY - this.currentYSpace <= this.height - scaleYHeight) {
      this.moveY = this.height - scaleYHeight + this.currentYSpace
    }


    let moveYSource = this.leftAxisModel.height - this.leftAxisModel.lastHeight
    this.leftAxisModel.translate(moveYSource + this.moveY - this.currentYSpace);
    this.rightAxisModel.translate(moveYSource + this.moveY - this.currentYSpace);
    if (this.lineChartModel != null) {
      this.lineChartModel.test = '' + this.moveY;
    }
    this.setXPosition(this.moveX - this.currentXSpace)
    this.ondraw();
  }

  public onScale(event: TouchEvent) {
    this.currentXSpace = this.centerX * this.scaleX - this.centerX;
    this.currentYSpace = this.centerY * this.scaleY - this.centerY;

    this.ondraw();

    if (this.lineChartModel != null) {
      this.lineChartModel.test = '' + this.currentYSpace;
    }
    this.setXPosition(this.moveX - this.currentXSpace)
    let moveYSource = this.leftAxisModel.lastHeight * this.scaleY - this.leftAxisModel.lastHeight
    this.leftAxisModel.translate(moveYSource + this.moveY - this.currentYSpace);
  }

  public calcClipPath() {
    this.clipPath = 'M' + Utils.convertDpToPixel(this.rect.left - this.positionX) + ' ' + Utils.convertDpToPixel(this.rect.top - this.minOffset) +
    'L' + Utils.convertDpToPixel(this.rect.right - this.positionX) + ' ' + Utils.convertDpToPixel(this.rect.top - this.minOffset) +
    'L' + Utils.convertDpToPixel(this.rect.right - this.positionX) + ' ' + Utils.convertDpToPixel(this.rect.bottom - this.minOffset) +
    'L' + Utils.convertDpToPixel(this.rect.left - this.positionX) + ' ' + Utils.convertDpToPixel(this.rect.bottom - this.minOffset) +
    ' Z'
  }

  public setBackgroundColor(color: Color | string | number): void {
    this.backgroundColor = color;
  }

  public setMinOffset(minOffset: number) : void {
    this.minOffset = minOffset;
  }

  public getYLeftAxis() : YAxis {
    return this.yleftAxis;
  }

  public getYRightAxis() : YAxis {
    return this.yRightAxis;
  }

  public getXAxis() : XAxis {
    return this.xAxis;
  }

  // ================================== 动画相关 ======================================

  private timerId: number = -1;
  private curX: number = 0;
  private curY: number = 0;
  private pathPaint: Paint[] = [];
  private circlePaint: Paint[] = [];
  private textPaint: Paint[] = [];
  private tempArry = []

  public animateX(durationMillis: number) {
    this.clearPaint();

    let maxCount = this.lineData.getMaxEntryCountSet().getEntryCount();

    this.timerId = setInterval(()=>{
      this.clearPaint();

      this.mRenderer.animateX(this.curX++);

      this.onDraw();

      if (this.curX >= maxCount) {
        this.curX = 0;
        clearInterval(this.timerId)
      }
    }, durationMillis)
  }

  public animateY(durationMillis: number) {
    this.clearPaint();

    let maxCount = this.lineData.getMaxEntryCountSet().getEntryCount();

    this.timerId = setInterval(()=>{
      this.clearPaint();

      this.mRenderer.animateY(this.curY += 1 / maxCount);

      this.onDraw();

      if (this.curY >= 1) {
        this.curY = 0;
        clearInterval(this.timerId)
      }
    }, durationMillis)
  }

  public animateXY(durationMillis: number) {
    this.clearPaint();
    let maxCount = this.lineData.getMaxEntryCountSet().getEntryCount();

    this.timerId = setInterval(()=>{
      this.clearPaint();

      this.mRenderer.animateX(this.curX++);
      this.mRenderer.animateY(this.curY += 1 / maxCount);

      this.onDraw();

      if (this.curY >= 1) {
        this.curX = 0;
        this.curY = 0;
        clearInterval(this.timerId)
      }
    }, durationMillis)
  }

  private clearPaint() {
    this.pathPaint = [];
    this.circlePaint = [];
    this.textPaint = [];
    this.tempArry = []
    this.paintArry = [];
  }

  private onDraw() {
    this.pathPaint = this.mRenderer.drawData();
    this.circlePaint = this.mRenderer.drawCircle();
    this.textPaint = this.mRenderer.drawValues();
    this.heightlightPaint = this.mRenderer.drawHighlighted();

    this.tempArry = this.tempArry.concat(this.pathPaint);
    this.tempArry = this.tempArry.concat(this.circlePaint);
    this.tempArry = this.tempArry.concat(this.textPaint);

    this.paintArry = this.paintArry.concat(this.tempArry);
  }
}

