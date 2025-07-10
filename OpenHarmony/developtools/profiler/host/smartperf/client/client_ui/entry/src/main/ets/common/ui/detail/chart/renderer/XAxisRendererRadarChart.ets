import MyRect from '../data/Rect';
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

import { XAxis } from '../components/XAxis';
import MPPointF from '../utils/MPPointF';
import Utils from '../utils/Utils';
import ViewPortHandler from '../utils/ViewPortHandler';
import XAxisRenderer from './XAxisRenderer';
import Paint, { TextPaint } from '../data/Paint';
import RadarData from '../data/RadarData';
import RadarChartMode from '../data/RadarChartMode';
export default class XAxisRendererRadarChart extends XAxisRenderer {
    public radarChartMode: RadarChartMode = new RadarChartMode();
    constructor(radarChartMode: RadarChartMode) {
        super(radarChartMode.handler, radarChartMode.xAxis, null);
        this.radarChartMode = radarChartMode;
    }

    public renderAxisLabels(): Paint[] {
        let paintArr: Paint[] = [];

        let labelRotationAngleDegrees: number = this.mXAxis.getLabelRotationAngle();
        let drawLabelAnchor: MPPointF = MPPointF.getInstance(0.5, 0.25);

        this.mAxisLabelPaint.setTypeface(this.mXAxis.getTypeface());
        this.mAxisLabelPaint.setTextSize(this.mXAxis.getTextSize());
        this.mAxisLabelPaint.setColor(this.mXAxis.getTextColor());

        let sliceangle: number = this.radarChartMode.getSliceAngle();

        let factor: number = this.radarChartMode.getFactor();

        let center: MPPointF = this.radarChartMode.getCenterOffsets();
        let pOut: MPPointF = MPPointF.getInstance(0, 0);
        let extraWidth = Utils.calcTextWidth(this.mAxisLabelPaint, this.mXAxis.longest);
        let r: number = this.radarChartMode.getYRange() * factor + extraWidth / 2 + 3;
        for (let i = 0; i < this.radarChartMode.getData().getMaxEntryCountSet().getEntryCount(); i++) {
            let label: string = this.mXAxis.getValueFormatter().getFormattedValue(i, this.mXAxis);
            let angle: number = (sliceangle * i + this.radarChartMode.getRotationAngle()) % 360;
            Utils.getPosition(center, r, angle, pOut);
            if (!this.mXAxis.isEnabled() || !this.mXAxis.isDrawLabelsEnabled()) {
                paintArr.push(new TextPaint());
            } else {
                var labelPaint = new TextPaint(this.mAxisLabelPaint as TextPaint);
                paintArr.push(
                Utils.drawXAxisValue(
                    label,
                pOut.x,
                    pOut.y - this.mXAxis.mLabelRotatedHeight / 2,
                    labelPaint,
                    drawLabelAnchor,
                    labelRotationAngleDegrees
                )
                );
            }
        }

        MPPointF.recycleInstance(center);
        MPPointF.recycleInstance(pOut);
        MPPointF.recycleInstance(drawLabelAnchor);
        return paintArr;
    }

    /**
     * XAxis LimitLines on RadarChart not yet supported.
     *
     * @param c
     */
    public renderLimitLines(): Paint[] {
        // this space intentionally left blank
        return [];
    }
}
