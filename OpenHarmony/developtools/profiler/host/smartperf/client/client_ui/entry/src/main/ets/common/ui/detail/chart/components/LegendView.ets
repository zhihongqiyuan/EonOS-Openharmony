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

import Legend, {LegendForm} from './Legend'
import LegendRenderer from '../renderer/LegendRenderer'
import ViewPortHandler from '../utils/ViewPortHandler'
import Paint, {LinePaint, CirclePaint, RectPaint, TextPaint} from '../data/Paint'

@Component
@Preview
@Entry
struct LegendView {
  @State model: LegendView.Model = new LegendView.Model()

  aboutToAppear() {
    this.model.mLegend.calculateNeededSizes(this.model.mHandler);
    this.model.mHandler.setChartDimens(
      this.model.mWidth, 
      (this.model.mLegend.mNeededHeight > this.model.mHeight) ?
        (this.model.mLegend.mNeededHeight + 5) : this.model.mHeight
    );

    var mLegendRenderer : LegendRenderer = new LegendRenderer(this.model.mHandler, this.model.mLegend);
    this.model.paints = this.model.paints.concat(mLegendRenderer.renderLegend())
    console.log('mLegendRenderer.computeLegend' + JSON.stringify(this.model.paints));
  }

  build() {
    Stack() {
      Flex({ direction: FlexDirection.Row, alignItems: ItemAlign.Center, justifyContent: FlexAlign.Center }) {
        ForEach(this.model.paints, (item:Paint) => {
          if (item instanceof CirclePaint) {
            Circle()
              .width(item.width)
              .height(item.height)
              .fill(item.stroke)
              .position({ x: item.x, y: item.y })
          } else if (item.constructor.name == 'RectPaint') {
            Rect({width: item.width, height: item.height})
              .fill(item.stroke)
              .position({ x: item.x, y: item.y })
          } else if (item.constructor.name == 'TextPaint') {
            Text((<TextPaint>item).text)
              .fontWeight(item.typeface)
              .fontSize(item.textSize)
              .fontColor(Color.Black)
              .textAlign(item.textAlign)
              .position({ x: item.x, y: item.y })
          }
        }, (item: Paint) => (item.alpha + '').toString())
      }
      .size({ width: '90%', height: '90%' })
    }
    .size({ width: this.model.mWidth, height: this.model.mHeight })
  }
}

namespace LegendView {
  export class Model {
    public paints: Paint[] = []

    public mHandler: ViewPortHandler = new ViewPortHandler();
    public yEntrySpace : number = 5
    public formToTextSpace : number = 2
    public mLegend: Legend = new Legend();
    public mWidth: number = 300;
    public mHeight: number = 50;

    setLegend(mLegend:Legend):Model {
      this.mLegend = mLegend
      return this
    }

    setWidth(mWidth:number):Model {
      this.mWidth = mWidth
      return this
    }

    setHeight(mHeight:number):Model {
      this.mHeight = mHeight
      return this
    }

    public refresh() {
      this.paints = []
      this.mLegend.calculateNeededSizes(this.mHandler);
      this.mHandler.setChartDimens(
        this.mWidth, (this.mLegend.mNeededHeight > this.mHeight) ? (this.mLegend.mNeededHeight + 5) : this.mHeight
      );

      var mLegendRenderer : LegendRenderer = new LegendRenderer(this.mHandler, this.mLegend);
      this.paints = this.paints.concat(mLegendRenderer.renderLegend())
    }
  }
}
export default LegendView
