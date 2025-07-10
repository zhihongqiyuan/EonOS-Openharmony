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

import ChartAnimator from '../animation/ChartAnimator';
import ViewPortHandler from '../utils/ViewPortHandler';
import RadarChartRenderer from '../renderer/RadarChartRenderer';
import XAxisRendererRadarChart from '../renderer/XAxisRendererRadarChart';
import YAxisRendererRadarChart from '../renderer/YAxisRendererRadarChart';
import { XAxis } from '../components/XAxis';
import RadarData from './RadarData';
import YAxis from '../components/YAxis';
import Paint, { IndexPositionPaint, CirclePaint, TextPaint } from './Paint';
import MPPointF from '../utils/MPPointF';
import MyRect from './Rect';
import Utils from '../utils/Utils';
import Highlight from '../highlight/Highlight';

export default class RadarChartMode {
    public width: number;
    public height: number;
    public minOffset: number;
    public paddingTop: number;
    public paddingLeft: number = 30;
    public xExtraOffset = 0;
    public yExtraOffset = 0;
    public mRotateEnabled: boolean = true;
    /**
     * width of the main web lines
     */
    public mWebLineWidth: number = 2.5;

    /**
     * width of the inner web lines
     */
    public mInnerWebLineWidth: number = 1.5;

    /**
     * color for the main web lines
     */
    public mWebColor: number = 0x64cccccc;

    /**
     * color for the inner web
     */
    public mWebColorInner: number = 0x64cccccc;

    /**
     * transparency the grid is drawn with (0-255)
     */
    public mWebAlpha: number = 150;

    /**
     * flag indicating if the web lines should be drawn or not
     */
    public mDrawWeb: boolean = true;

    /**
     * modulus that determines how many labels and web-lines are skipped before the next is drawn
     */
    public mSkipWebLineCount: number = 0;

    public yAxis: YAxis;
    public xAxis: XAxis;
    public xScale: number;
    public yScale: number;

    public data: RadarData = new RadarData();
    public displayCenterY: number = 0;
    public mRotationAngle: number = 270;
    public mRawRotationAngle = 270;
    public mStartAngle: number = 0;
    public yAxisRenderer: YAxisRendererRadarChart;
    public xAxisRenderer: XAxisRendererRadarChart;
    public radarRender: RadarChartRenderer;
    public handler: ViewPortHandler;
    public mAnimator: ChartAnimator;
    constructor(yAxis?: YAxis, data?: RadarData) {
        if (yAxis) {
            this.yAxis = yAxis;
        }
        if (data) {
            this.data = data;
        }
    }
    public getRotationAngle(): number {
        return this.mRotationAngle;
    }
    public setYAxis(mYAxis: YAxis): RadarChartMode {
        this.yAxis = mYAxis;
        return this;
    }
    public getYAxis(): YAxis {
        return this.yAxis;
    }
    public setXAxis(xAxis: XAxis): RadarChartMode {
        this.xAxis = xAxis;
        return this;
    }
    public setYExtraOffset(yExtraOffset: number) {
        this.yExtraOffset = yExtraOffset;
        return this;
    }
    public getXAxis(): XAxis {
        return this.xAxis;
    }
    public getWebLineWidth(): number {
        return this.mWebLineWidth;
    }
    public getWebLineWidthInner(): number {
        return this.mInnerWebLineWidth;
    }
    public getWebAlpha(): number {
        return this.mWebAlpha;
    }
    public getWebColor(): number {
        return this.mWebColor;
    }
    public getWebColorInner(): number {
        return this.mWebColorInner;
    }
    public getSkipWebLineCount(): number {
        return this.mSkipWebLineCount;
    }
    public getYChartMax(): number {
        return this.yAxis.mAxisMaximum;
    }

    /**
     * Returns the minimum value this chart can display on it's y-axis.
     */
    public getYChartMin(): number {
        return this.yAxis.mAxisMinimum;
    }

    /**
     * Returns the range of y-values this chart can display.
     *
     * @return
     */
    public getYRange(): number {
        return this.yAxis.mAxisRange;
    }
    public getSliceAngle(): number {
        return 360 / this.data.getMaxEntryCountSet().getEntryCount();
    }
    public setWidth(width: number): RadarChartMode {
        this.width = width;
        return this;
    }
    public setHeight(height: number): RadarChartMode {
        this.height = height;
        return this;
    }
    public setMinOffset(minOffset: number): RadarChartMode {
        this.minOffset = minOffset;
        return this;
    }
    public setPaddingTop(paddingTop: number): RadarChartMode {
        this.paddingTop = paddingTop;
        return this;
    }
    public setPaddingLeft(paddingLeft: number): RadarChartMode {
        this.paddingLeft = paddingLeft;
        return this;
    }
    public getWidth(): number {
        return this.width;
    }
    public getHeight(): number {
        return this.height;
    }
    public getMinOffset(): number {
        return this.minOffset;
    }
    public getPaddingTop(): number {
        return this.paddingTop;
    }
    public getPaddingLeft(): number {
        return this.paddingLeft;
    }

    public setData(data: RadarData): RadarChartMode {
        this.data = data;
        return this;
    }
    public getData(): RadarData {
        return this.data;
    }
    public setXScale(xScale: number): RadarChartMode {
        this.xScale = xScale;
        return this;
    }
    public getXScale(): number {
        return this.xScale;
    }
    public setYScale(yScale: number): RadarChartMode {
        this.yScale = yScale;
        return this;
    }
    public getYScale(): number {
        return this.yScale;
    }
    public setDisplayCenterY(displayCenterY: number): RadarChartMode {
        this.displayCenterY = displayCenterY;
        return this;
    }
    public getDisplayCenterY(): number {
        return this.displayCenterY;
    }
    public getRawRotationAngle(): number {
        return this.mRawRotationAngle;
    }
    public setRotationAngle(angle: number) {
        this.mRawRotationAngle = angle;
        this.mRotationAngle = Utils.getNormalizedAngle(this.mRawRotationAngle);
    }
    public calcScale() {
        let rect = this.data.mDisplayRect;
        this.displayCenterY = (rect.bottom - rect.top) / 2;
        let minX = this.xAxis.getAxisMinimum() > 0 ? 0 : this.xAxis.getAxisMinimum();
        let miny = this.yAxis.getAxisMinimum() > 0 ? 0 : this.xAxis.getAxisMinimum();

        this.xScale = (rect.right - rect.left) / (this.xAxis.getAxisMaximum() - minX);
        this.yScale = (rect.bottom - rect.top) / (this.yAxis.getAxisMaximum() - miny);
    }
    public init() {
        this.calcScale();
        this.handler = new ViewPortHandler();
        this.handler.restrainViewPort(this.minOffset, this.minOffset, this.minOffset, this.minOffset);
        this.handler.setChartDimens(this.width, this.height);

        this.xAxisRenderer = new XAxisRendererRadarChart(this);
        this.yAxisRenderer = new YAxisRendererRadarChart(this);
        this.yAxisRenderer.computeAxis(this.yAxis.mAxisMinimum, this.yAxis.mAxisMaximum, this.yAxis.isInverted());

        this.mAnimator = new ChartAnimator();
        this.radarRender = new RadarChartRenderer(this);
    }
    public getFactor(): number {
        let content: MyRect = this.handler.getContentRect();
        return Math.min(content.width() / 2, content.height() / 2) / this.getYRange();
    }

    public getCenterOffsets(): MPPointF {
        return this.handler.getContentCenter();
    }
    public getAngleForPoint(x: number, y: number): number {
        let c: MPPointF = this.getCenterOffsets();
        let tx = x - c.x;
        let ty = y - c.y;
        let length = Math.sqrt(tx * tx + ty * ty);
        let r = Math.acos(ty / length);
        let angle: number = (180 * r) / Math.PI;
        if (x > c.x) {
            angle = 360 - angle;
        }
        angle = angle + 90;

        if (angle > 360) {
            angle = angle - 360;
        }

        MPPointF.recycleInstance(c);
        return angle;
    }
    public paints: Paint[] = [];
    public highLight: Paint[] = [];
    public indexHighLightPaint: IndexPositionPaint = null;
    public drawChart(): Paint[] {
        let paintsTemp: Paint[] = [];
        let webPaint: Paint[] = this.radarRender.drawExtras();
        paintsTemp = paintsTemp.concat(webPaint);

        let dataPaint: Paint[] = this.radarRender.drawData();
        paintsTemp = paintsTemp.concat(dataPaint);

        let xDataPaint: Paint[] = this.xAxisRenderer.renderAxisLabels();
        paintsTemp = paintsTemp.concat(xDataPaint);

        let valuePaint: Paint[] = this.radarRender.drawValues();
        paintsTemp = paintsTemp.concat(valuePaint);
        this.paints = [];
        this.paints = this.paints.concat(paintsTemp);
        return this.paints;
    }
    public drawHighLight() {
        if (this.indexHighLightPaint == null || !this.data.isHighlightEnabled()) {
            this.indexHighLightPaint = null; //便于旋转的时候判断是否需要绘制点击效果
            this.highLight = [];
            this.highLight.push(new CirclePaint());
            this.highLight.push(new TextPaint());
            return;
        }
        let hightL: Highlight = new Highlight(
        this.indexHighLightPaint.x,
        this.indexHighLightPaint.y,
        this.indexHighLightPaint.dataSetIndex,
        this.indexHighLightPaint.dataIndex,
            -1,
        vp2px(this.indexHighLightPaint.x),
        vp2px(this.indexHighLightPaint.y)
        );
        this.highLight = this.radarRender.drawHighlighted([hightL]);
    }
    public drawClick(event: ClickEvent) {
        let x = event.screenX - this.paddingLeft - this.xExtraOffset;
        let y = event.screenY - this.paddingTop - this.yExtraOffset;
        let factor: number = this.getFactor();
        let r: number = this.getYRange() * factor;
        let center: MPPointF = this.getCenterOffsets();
        let clickToCenterSpace = Math.sqrt((x - center.x) * (x - center.x) + (y - center.y) * (y - center.y));
        if (clickToCenterSpace > r) {
            this.indexHighLightPaint = null; //便于旋转的时候判断是否需要绘制点击效果
            this.drawHighLight();
            return;
        }
        if (this.radarRender == null || this.radarRender == undefined) {
            return;
        }
        let dataP: Paint[] = this.radarRender.drawDataByType(this.radarRender.TYPE_POINT);
        if (dataP == null || dataP == undefined || dataP.length == 0) {
            return;
        }

        let minPoint: Paint;
        let minSpaceResult: number;
        for (let hightLight of dataP) {
            let xSpace = Math.abs(x - hightLight.x);
            let ySpace = Math.abs(y - hightLight.y);
            let minSpace = Math.sqrt(xSpace * xSpace + ySpace * ySpace);
            if (minPoint == null || minPoint == undefined || minSpace < minSpaceResult) {
                minSpaceResult = minSpace;
                minPoint = hightLight;
            }
        }
        if (minPoint == null || minPoint == undefined || !(minPoint instanceof IndexPositionPaint)) {
            return;
        }
        this.indexHighLightPaint = minPoint as IndexPositionPaint;
        this.drawHighLight();
    }
    public drawTouch(event: TouchEvent) {
        if (!this.mRotateEnabled) {
            return;
        }
        let x: number = event.touches[0].x;
        let y: number = event.touches[0].y;
        if (event.type === TouchType.Down) {
            this.mStartAngle = this.getAngleForPoint(x, y) - this.getRawRotationAngle();
        }
        if (event.type === TouchType.Up) {
        }
        if (event.type === TouchType.Move) {
            this.setRotationAngle(this.getAngleForPoint(x, y) - this.mStartAngle);
            this.drawChart();
            if (this.indexHighLightPaint == null) {
                return;
            }
            this.drawHighLight();
        }
    }
}
