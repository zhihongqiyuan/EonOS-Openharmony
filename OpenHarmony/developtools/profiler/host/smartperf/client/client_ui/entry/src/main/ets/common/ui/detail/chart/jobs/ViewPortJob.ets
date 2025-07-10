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

import Transformer from '../utils/Transformer';
import ViewPortHandler from '../utils/ViewPortHandler';
import Runnable from '../data/Runnable';
import Chart from '../charts/Chart';

/**
 * Runnable that is used for viewport modifications since they cannot be
 * executed at any time. This can be used to delay the execution of viewport
 * modifications until the onSizeChanged(...) method of the chart-view is called.
 * This is especially important if viewport modifying methods are called on the chart
 * directly after initialization.
 *
 * @author Philipp Jahoda
 */
export default abstract class ViewPortJob extends Runnable {
    protected pts: number[] = new Array(2);

    protected mViewPortHandler: ViewPortHandler;
    protected xValue: number = 0;
    protected yValue: number = 0;
    protected mTrans: Transformer;
    protected view: Chart<any>;

    constructor(viewPortHandler: ViewPortHandler, xValue: number, yValue: number, trans: Transformer, v: Chart<any>) {
        super(null, null);
        this.mViewPortHandler = viewPortHandler;
        this.xValue = xValue;
        this.yValue = yValue;
        this.mTrans = trans;
        this.view = v;
    }

    public getXValue(): number {
        return this.xValue;
    }

    public getYValue(): number {
        return this.yValue;
    }
}
