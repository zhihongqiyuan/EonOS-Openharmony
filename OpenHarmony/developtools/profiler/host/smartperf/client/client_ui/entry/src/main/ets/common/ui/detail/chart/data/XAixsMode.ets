import Transformer from '../utils/Transformer';
import XAxisRenderer from '../renderer/XAxisRenderer';
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

import LimitLine from '../components/LimitLine';
import { XAxis } from '../components/XAxis';
import Paint from './Paint';
import ViewPortHandler from '../utils/ViewPortHandler';

export default class XAixsMode {
    public paints: Paint[] = [];
    public handler: ViewPortHandler = new ViewPortHandler();
    public topAxis: XAxis = new XAxis();
    public bottomAxis: XAxis = new XAxis();
    public width: number = 300;
    public height: number = 300;
    public minOffset: number = 15;
    public yLeftLongestLabel: string = 'AAA';
    public yRightLongestLabel: string = 'AAA';
    public xLimtLine: LimitLine = new LimitLine(35, 'Index 10');

    public xPosition: number = 0;
    public yPosition: number = 0;
    public clipPath: string;

    public draw() {
        this.paints = [];
        let minYOffset = this.topAxis.getTextSize() + this.topAxis.getYOffset();
        this.minOffset = this.minOffset < minYOffset ? minYOffset : this.minOffset;

        this.handler.restrainViewPort(this.minOffset, this.minOffset, this.minOffset, this.minOffset);
        this.handler.setChartDimens(this.width, this.height);
        let mAxisRendererTop: XAxisRenderer =
            new XAxisRenderer(this.handler, this.topAxis, new Transformer(this.handler));
        mAxisRendererTop.yLeftLongestLabel = this.yLeftLongestLabel;
        mAxisRendererTop.yRightLongestLabel = this.yRightLongestLabel;
        mAxisRendererTop.computeAxis(this.topAxis.mAxisMinimum, this.topAxis.mAxisMaximum, false);
        this.paints = this.paints.concat(mAxisRendererTop.renderAxisLine());
        this.paints = this.paints.concat(mAxisRendererTop.renderAxisLabels());
        this.paints = this.paints.concat(mAxisRendererTop.renderGridLines());
        let mAxisRendererBottom: XAxisRenderer = new XAxisRenderer(
        this.handler,
        this.bottomAxis,
            new Transformer(this.handler)
        );
        mAxisRendererBottom.yLeftLongestLabel = this.yLeftLongestLabel;
        mAxisRendererBottom.yRightLongestLabel = this.yRightLongestLabel;
        mAxisRendererBottom.computeAxis(this.bottomAxis.mAxisMinimum, this.bottomAxis.mAxisMaximum, false);
        this.paints = this.paints.concat(mAxisRendererBottom.renderAxisLine());
        this.paints = this.paints.concat(mAxisRendererBottom.renderAxisLabels());
        this.paints = this.paints.concat(mAxisRendererTop.renderLimitLines());

        let arr = [];
        for (let item of this.paints) {
            if (item) {
                arr.push(item);
            }
        }

        this.paints = arr;
    }
}
