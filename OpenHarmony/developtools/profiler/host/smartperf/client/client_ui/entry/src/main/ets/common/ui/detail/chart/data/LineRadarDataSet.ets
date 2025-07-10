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

import { ColorStop } from '../data/LineDataSet';
import EntryOhos from './EntryOhos';
import LineScatterCandleRadarDataSet from './LineScatterCandleRadarDataSet';
import ILineRadarDataSet from '../interfaces/datasets/ILineRadarDataSet';
import { JArrayList } from '../utils/JArrayList';
import Utils from '../utils/Utils';

/**
 * Base dataset for line and radar DataSets.
 */
export default abstract class LineRadarDataSet<T extends EntryOhos>
extends LineScatterCandleRadarDataSet<T>
implements ILineRadarDataSet<T> {
  // TODO: Move to using `Fill` class
  /**
   * the color that is used for filling the line surface
   */
  //  private mFillColor: number = Color.rgb(140, 234, 255);
  private mFillColor: number = 0x8ceaff;
  private mLinearGradientColors: Array<ColorStop>;

  /**
   * the drawable to be used for filling the line surface
   */
  protected mFillDrawable: Object;/*Drawable*/

  /**
   * transparency used for filling line surface
   */
  private mFillAlpha: number = 85;

  /**
   * the width of the drawn data lines
   */
  private mLineWidth: number = 2.5;

  /**
   * if true, the data will also be drawn filled
   */
  private mDrawFilled: boolean = false;

  constructor(yVals: JArrayList<T>, label: string) {
    super(yVals, label);
  }

  public getFillColor(): number {
    return this.mFillColor;
  }

  /**
   * Sets the color that is used for filling the area below the line.
   * Resets an eventually set "fillDrawable".
   *
   * @param color
   */
  public setFillColor(color: number): void {
    this.mFillColor = color;
    this.mFillDrawable = null;
  }

  public setGradientFillColor(linearGradientColors: Array<ColorStop>): void {
    this.mLinearGradientColors = linearGradientColors;
    this.mFillDrawable = null;
    this.mFillColor = null;
  }

  public getGradientFillColor(): Array<ColorStop> {
    return this.mLinearGradientColors;
  }

  public getFillDrawable(): Object /*Drawable*/ {
    return this.mFillDrawable;
  }

  /**
   * Sets the drawable to be used to fill the area below the line.
   *
   * @param drawable
   */
  //    @TargetApi(18)
  public setFillDrawable(drawable: Object /*Drawable*/): void {
    this.mFillDrawable = drawable;
  }

  public getFillAlpha(): number {
    return this.mFillAlpha;
  }

  /**
   * sets the alpha value (transparency) that is used for filling the line
   * surface (0-255), default: 85
   *
   * @param alpha
   */
  public setFillAlpha(alpha: number): void {
    this.mFillAlpha = alpha;
  }

  /**
   * set the line width of the chart (min = 0.2f, max = 10f); default 1f NOTE:
   * thinner line == better performance, thicker line == worse performance
   *
   * @param width
   */
  public setLineWidth(width: number): void {
    if (width < 0.0) {
      width = 0.0;
    }
    if (width > 10.0) {
      width = 10.0;
    }
    this.mLineWidth = width;
  }

  public getLineWidth(): number {
    return this.mLineWidth;
  }

  public setDrawFilled(filled: boolean): void {
    this.mDrawFilled = filled;
  }

  public isDrawFilledEnabled(): boolean {
    return this.mDrawFilled;
  }

  protected copyTo(lineRadarDataSet: LineRadarDataSet<T>): void {
    super.copyTo(lineRadarDataSet);
    lineRadarDataSet.mDrawFilled = this.mDrawFilled;
    lineRadarDataSet.mFillAlpha = this.mFillAlpha;
    lineRadarDataSet.mFillColor = this.mFillColor;
    lineRadarDataSet.mFillDrawable = this.mFillDrawable;
    lineRadarDataSet.mLineWidth = this.mLineWidth;
  }
}
