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

import Paint, {LinePaint, TextPaint, PathPaint} from '../../data/Paint'
import YAxis, {YAxisLabelPosition, AxisDependency} from '../../components/YAxis'
import YAxisRenderer from '../../renderer/YAxisRenderer'
import Transformer from '../../utils/Transformer'
import ViewPortHandler from '../../utils/ViewPortHandler'
import Matrix from '../../utils/Matrix'
import MyRect from '../../data/Rect'
import LimitLine, {LimitLabelPosition} from '../../components/LimitLine'
import Utils from '../../utils/Utils'

@Component
export default struct YAxisView {
  @State model: YAxisModel = new YAxisModel();
  public yAxis: YAxis = null;
  public minOffset: number = 15;

  public aboutToAppear() {
    this.model.invalidate();
  }

  build() {
    Stack(){
      Stack() {
        ForEach(this.model.paints, (item: Paint) => {
          if (item instanceof LinePaint) {
            Line()
              .startPoint(item.startPoint)
              .endPoint(item.endPoint)
              .fill(item.fill)
              .stroke(item.stroke)
              .strokeWidth(item.strokeWidth)
              .strokeDashArray(item.strokeDashArray)
              .strokeDashOffset(item.strokeDashOffset)
              .strokeOpacity(item.alpha)
              .position({ x: 0, y: 0 })
          } else if (item instanceof TextPaint) {
            Text(item.text)
              .width(item.width)
              .height(item.height)
              .position({ x: item.x, y: item.y})
              .fontWeight(item.typeface)
              .fontSize(item.textSize)
              .textAlign(item.textAlign)
              .padding({ left: 5, right: 5 })
          } else if (item instanceof PathPaint) {
            Path()
              .commands(item.commands)
              .fill(item.fill)
              .stroke(item.stroke)
              .strokeWidth(item.strokeWidth == 0 ? 1 : item.strokeWidth)
              .strokeDashArray(item.strokeDashArray)
              .strokeDashOffset(item.strokeDashOffset)
              .strokeOpacity(item.alpha)
              .position({ x: item.x, y: item.y })
          }
        }, (item: Paint) => (item.alpha + '').toString())
      }
      .width(this.model.width)
      .height(this.model.height)
      .position({y: -(this.model.height - this.model.lastHeight) + this.model.translateY})
    }
    .clip(new Path().commands(this.model.clipPath))
  }
}

export class YAxisModel {
  public width: number = 300;
  public height: number = 300;
  public handler: ViewPortHandler = new ViewPortHandler();
  public paints: Paint[] = []
  public minOffset: number = 15;
  public mTran: Transformer = new Transformer(this.handler);
  public yAxis: YAxis = null;
  public mAxisRenderer: YAxisRenderer;
  public lastHeight: number = 0;
  public translateY: number = 0;
  public clipPath: string = '';

  public initYAxis() {
    this.mTran.prepareMatrixOffset(this.yAxis.isInverted());
    this.mAxisRenderer = new YAxisRenderer(this.handler, this.yAxis, this.mTran);
    this.mAxisRenderer.computeAxis(this.yAxis.mAxisMinimum, this.yAxis.mAxisMaximum, this.yAxis.isInverted())
  }

  public initViewPortHandler() {
    this.handler.restrainViewPort(this.minOffset, this.minOffset, this.minOffset, this.minOffset)
    this.handler.setChartDimens(this.width, this.height);
  }

  public invalidate() {
    this.initViewPortHandler();
    this.initYAxis();
    this.paints.length = 0;
    this.paints = this.paints.concat(this.mAxisRenderer.renderAxisLine());
    this.paints = this.paints.concat(this.mAxisRenderer.renderAxisLabels());
    this.paints = this.paints.concat(this.mAxisRenderer.renderGridLines());
    this.paints = this.paints.concat(this.mAxisRenderer.renderLimitLines());
    this.calcXAixsModeClipPath();
  }

  public setWidth(width: number) {
    this.width = width
  }

  public setHeight(height: number) {
    this.height = height;
    this.lastHeight = height;
  }

  public setMinOffset(minOffset: number) {
    this.minOffset = minOffset;
  }

  public setYAxis(yAxis: YAxis) {
    this.yAxis = yAxis;
  }

  public scale(scaleY: number) {
    this.height = this.lastHeight * scaleY;
    this.invalidate();
  }

  public translate(translateY: number) {
    this.translateY = translateY;
    this.calcXAixsModeClipPath();
  }

  public calcXAixsModeClipPath() {
    this.clipPath = 'M' + Utils.convertDpToPixel(0) + ' ' + Utils.convertDpToPixel(this.translateY >= (this.height - this.lastHeight) ? 0 : this.minOffset) +
    'L' + Utils.convertDpToPixel(this.width) + ' ' + Utils.convertDpToPixel(this.translateY <= (this.height - this.lastHeight) ? 0 : this.minOffset) +
    'L' + Utils.convertDpToPixel(this.width) + ' ' + Utils.convertDpToPixel(this.translateY > 0 ? this.lastHeight - this.minOffset : this.lastHeight) +
    'L' + Utils.convertDpToPixel(0) + ' ' + Utils.convertDpToPixel(this.translateY > 0 ? this.lastHeight - this.minOffset : this.lastHeight) +
    ' Z'
  }

}
