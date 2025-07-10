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

import { ImagePaint } from './Paint';
import EntryOhos from './EntryOhos';

/**
 * Subclass of Entry that holds all values for one entry in a CandleStickChart.
 */
export default class CandleEntry extends EntryOhos {
    /** shadow-high value */
    private mShadowHigh: number = 0;

    /** shadow-low value */
    private mShadowLow: number = 0;

    /** close value */
    private mClose: number = 0;

    /** open value */
    private mOpen: number = 0;
    constructor(
        x: number,
        shadowH?: number,
        shadowL?: number,
        open?: number,
        close?: number,
        icon?: ImagePaint,
        data?: Object
    ) {
        super(x, (shadowH + shadowL) / 2, icon, data);
        this.mShadowHigh = shadowH;
        this.mShadowLow = shadowL;
        this.mOpen = open;
        this.mClose = close;
    }
    /**
     * Returns the overall range (difference) between shadow-high and
     * shadow-low.
     *
     * @return
     */
    public getShadowRange(): number {
        return Math.abs(this.mShadowHigh - this.mShadowLow);
    }

    /**
     * Returns the body size (difference between open and close).
     *
     * @return
     */
    public getBodyRange(): number {
        return Math.abs(this.mOpen - this.mClose);
    }

    /**
     * Returns the center value of the candle. (Middle value between high and
     * low)
     */
    public getY(): number {
        return super.getY();
    }

    public copy(): CandleEntry {
        var c: CandleEntry = new CandleEntry(
        this.getX(),
        this.mShadowHigh,
        this.mShadowLow,
        this.mOpen,
        this.mClose,
            null,
        this.getData()
        );

        return c;
    }

    /**
     * Returns the upper shadows highest value.
     *
     * @return
     */
    public getHigh(): number {
        return this.mShadowHigh;
    }

    public setHigh(mShadowHigh: number): void {
        this.mShadowHigh = mShadowHigh;
    }

    /**
     * Returns the lower shadows lowest value.
     *
     * @return
     */
    public getLow(): number {
        return this.mShadowLow;
    }

    public setLow(mShadowLow: number): void {
        this.mShadowLow = mShadowLow;
    }

    /**
     * Returns the bodys close value.
     *
     * @return
     */
    public getClose(): number {
        return this.mClose;
    }

    public setClose(mClose: number): void {
        this.mClose = mClose;
    }

    /**
     * Returns the bodys open value.
     *
     * @return
     */
    public getOpen(): number {
        return this.mOpen;
    }

    public setOpen(mOpen: number): void {
        this.mOpen = mOpen;
    }
}
