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

import LineScatterCandleRadarDataSet from './LineScatterCandleRadarDataSet';
import EntryOhos from './EntryOhos';
import IScatterDataSet from '../interfaces/datasets/IScatterDataSet';
import ColorTemplate from '../utils/ColorTemplate';
import { JArrayList } from '../utils/JArrayList';
import IShapeRenderer from '../renderer/scatter/IShapeRenderer';

export class ScatterDataSet extends LineScatterCandleRadarDataSet<EntryOhos> implements IScatterDataSet {
    /**
     * the size the scattershape will have, in density pixels
     */
    private mShapeSize: number = 15;

    /**
     * Renderer responsible for rendering this DataSet, default: square
     */

    /**
     * The radius of the hole in the shape (applies to Square, Circle and Triangle)
     * - default: 0.0
     */
    private mScatterShapeHoleRadius: number = 0;

    /**
     * Color for the hole in the shape.
     * Setting to `ColorTemplate.COLOR_NONE` will behave as transparent.
     * - default: ColorTemplate.COLOR_NONE
     */
    private mScatterShapeHoleColor: number = ColorTemplate.COLOR_NONE;

    constructor(yVals: JArrayList<EntryOhos>, label: string) {
        super(yVals, label);
    }

    public copy(): ScatterDataSet /*DataSet<EntryOhos>*/ {
        let entries = new JArrayList<EntryOhos>();
        for (let i = 0; i < this.mEntries.size(); i++) {
            entries.add(this.mEntries.get(i).copy());
        }
        let copied = new ScatterDataSet(entries, this.getLabel());
        this.copyTo(copied);
        return copied;
    }

    public copyTo(scatterDataSet: ScatterDataSet): void {
        super.copyTo(scatterDataSet);
        scatterDataSet.mShapeSize = this.mShapeSize;

        scatterDataSet.mScatterShapeHoleRadius = this.mScatterShapeHoleRadius;
        scatterDataSet.mScatterShapeHoleColor = this.mScatterShapeHoleColor;
    }

    /**
     * Sets the size in density pixels the drawn scattershape will have. This
     * only applies for non custom shapes.
     *
     * @param size
     */
    public setScatterShapeSize(size: number): void {
        this.mShapeSize = size;
    }

    public getScatterShapeSize(): number {
        return this.mShapeSize;
    }

    /**
     * Sets the ScatterShape this DataSet should be drawn with.
     * This will search for an available IShapeRenderer and set this
     * renderer for the DataSet.
     *
     * @param shape
     */

    /**
     * Sets a new IShapeRenderer responsible for drawing this DataSet.
     * This can also be used to set a custom IShapeRenderer aside from the default ones.
     *
     * @param shapeRenderer
     */

    /**
     * Sets the radius of the hole in the shape (applies to Square, Circle and Triangle)
     * Set this to <= 0 to remove holes.
     *
     * @param holeRadius
     */
    public setScatterShapeHoleRadius(holeRadius: number): void {
        this.mScatterShapeHoleRadius = holeRadius;
    }

    public getScatterShapeHoleRadius(): number {
        return this.mScatterShapeHoleRadius;
    }

    /**
     * Sets the color for the hole in the shape.
     *
     * @param holeColor
     */
    public setScatterShapeHoleColor(holeColor: number): void {
        this.mScatterShapeHoleColor = holeColor;
    }

    public getScatterShapeHoleColor(): number {
        return this.mScatterShapeHoleColor;
    }
}
