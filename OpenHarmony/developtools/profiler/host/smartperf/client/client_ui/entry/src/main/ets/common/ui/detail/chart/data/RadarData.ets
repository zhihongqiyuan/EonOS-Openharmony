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

import Highlight from '../highlight/Highlight';
import IRadarDataSet from '../interfaces/datasets/IRadarDataSet';
import { JArrayList } from '../utils/JArrayList';
import ChartData from './ChartData';
import EntryOhos from './EntryOhos';

/**
 * Data container for the RadarChart.
 */
export default class RadarData extends ChartData<IRadarDataSet> {
    private mLabels: JArrayList<string>;

    constructor(dataSets?: JArrayList<IRadarDataSet>) {
        super(dataSets);
    }

    /**
     * Sets the labels that should be drawn around the RadarChart at the end of each web line.
     *
     * @param labels
     */
    public setLabels(labels: JArrayList<string>): void {
        this.mLabels = labels;
    }

    public getLabels(): JArrayList<string> {
        return this.mLabels;
    }

    // @Override
    public getEntryForHighlight(highlight: Highlight): EntryOhos {
        return super.getDataSetByIndex(highlight.getDataSetIndex()).getEntryForIndex(highlight.getX());
    }
}
