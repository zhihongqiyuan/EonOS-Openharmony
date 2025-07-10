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

import IRadarDataSet from '../interfaces/datasets/IRadarDataSet';
import ColorTemplate from '../utils/ColorTemplate';
import { JArrayList } from '../utils/JArrayList';
import RadarEntry from './RadarEntry';
import LineRadarDataSet from './LineRadarDataSet';
import { DataSet } from './DataSet';

export default class RadarDataSet extends LineRadarDataSet<RadarEntry> implements IRadarDataSet {
    // flag indicating whether highlight circle should be drawn or not
    protected mDrawHighlightCircleEnabled: boolean = false;

    protected mHighlightCircleFillColor: number = Color.White;

    // The stroke color for highlight circle.
    // If Utils.COLOR_NONE, the color of the dataset is taken.
    protected mHighlightCircleStrokeColor: number = ColorTemplate.COLOR_NONE;

    protected mHighlightCircleStrokeAlpha: number = 0.3 * 255;
    protected mHighlightCircleInnerRadius: number = 3.0;
    protected mHighlightCircleOuterRadius: number = 4.0;
    protected mHighlightCircleStrokeWidth: number = 2.0;

    constructor(yVals: JArrayList<RadarEntry>, label: string) {
        super(yVals, label);
    }

    // Returns true if highlight circle should be drawn, false if not
    // @Override
    public isDrawHighlightCircleEnabled(): boolean {
        return this.mDrawHighlightCircleEnabled;
    }

    // Sets whether highlight circle should be drawn or not
    // @Override
    public setDrawHighlightCircleEnabled(enabled: boolean): void {
        this.mDrawHighlightCircleEnabled = enabled;
    }

    // @Override
    public getHighlightCircleFillColor(): number {
        return this.mHighlightCircleFillColor;
    }

    public setHighlightCircleFillColor(color: number): void {
        this.mHighlightCircleFillColor = color;
    }

    // Returns the stroke color for highlight circle.
    // If Utils.COLOR_NONE, the color of the dataset is taken.
    // @Override
    public getHighlightCircleStrokeColor(): number {
        return this.mHighlightCircleStrokeColor;
    }

    // Sets the stroke color for highlight circle.
    // Set to Utils.COLOR_NONE in order to use the color of the dataset;
    public setHighlightCircleStrokeColor(color): void {
        this.mHighlightCircleStrokeColor = color;
    }

    // @Override
    public getHighlightCircleStrokeAlpha(): number {
        return this.mHighlightCircleStrokeAlpha;
    }

    public setHighlightCircleStrokeAlpha(alpha: number): void {
        this.mHighlightCircleStrokeAlpha = alpha;
    }

    // @Override
    public getHighlightCircleInnerRadius(): number {
        return this.mHighlightCircleInnerRadius;
    }

    public setHighlightCircleInnerRadius(radius: number): void {
        this.mHighlightCircleInnerRadius = radius;
    }

    // @Override
    public getHighlightCircleOuterRadius(): number {
        return this.mHighlightCircleOuterRadius;
    }

    public setHighlightCircleOuterRadius(radius: number): void {
        this.mHighlightCircleOuterRadius = radius;
    }

    // @Override
    public getHighlightCircleStrokeWidth(): number {
        return this.mHighlightCircleStrokeWidth;
    }

    public setHighlightCircleStrokeWidth(strokeWidth: number): void {
        this.mHighlightCircleStrokeWidth = strokeWidth;
    }

    public copy(): DataSet<RadarEntry> {
        let entries = new JArrayList<RadarEntry>();
        for (let i = 0; i < this.mEntries.size(); i++) {
            entries.add(this.mEntries.get(i).copy());
        }
        let copied = new RadarDataSet(entries, this.getLabel());
        this.copyTo(copied);
        return copied;
    }

    protected copyTo(radarDataSet: RadarDataSet): void {
        super.copyTo(radarDataSet);
        radarDataSet.mDrawHighlightCircleEnabled = this.mDrawHighlightCircleEnabled;
        radarDataSet.mHighlightCircleFillColor = this.mHighlightCircleFillColor;
        radarDataSet.mHighlightCircleInnerRadius = this.mHighlightCircleInnerRadius;
        radarDataSet.mHighlightCircleStrokeAlpha = this.mHighlightCircleStrokeAlpha;
        radarDataSet.mHighlightCircleStrokeColor = this.mHighlightCircleStrokeColor;
        radarDataSet.mHighlightCircleStrokeWidth = this.mHighlightCircleStrokeWidth;
    }
}
