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

import ScaleMode from '../../data/ScaleMode';
import LimitLine, {LimitLabelPosition} from '../LimitLine';
import Paint, {LinePaint, TextPaint, PathPaint} from '../../data/Paint'
import {XAxis, XAxisPosition} from '../XAxis'
import XAxisRenderer from '../../renderer/XAxisRenderer'
import Transformer from '../..//utils/Transformer'
import ViewPortHandler from '../../utils/ViewPortHandler'
import XAixsMode from '../../data/XAixsMode';

@Component
@Preview
export default struct XAxisView {

  paints:Paint[] = []
  handler:ViewPortHandler = new ViewPortHandler();
  @State
  topAxis:XAxis = new XAxis();
  @State
  bottomAxis:XAxis = new XAxis();
  @State
  minOffset:number = 15;
  @State
  yLeftLongestLabel:string = 'AAA'
  @State
  yRightLongestLabel:string = 'AAA'
  @State
  xLimtLine:LimitLine = new LimitLine(35, 'Index 10');

  @State
  scaleMode:ScaleMode = new ScaleMode()
  aboutToAppear() {
    this.scaleMode.xAixsMode.draw()
  }

  build() {
    Stack({ alignContent: Alignment.TopStart }) {
      if (this.scaleMode.xAixsMode.paints && this.scaleMode.xAixsMode.paints.length > 0) {
        ForEach(this.scaleMode.xAixsMode.paints, (item: Paint) => {
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
              .position({ x: item.x, y: item.y })
              .fontWeight(item.typeface)
              .fontSize(item.textSize)
              .textAlign(item.textAlign)
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
    }
    .width(this.scaleMode.xAixsMode.width)
    .height(this.scaleMode.xAixsMode.height)
    .position({x:this.scaleMode.xAixsMode.xPosition})
  }
}