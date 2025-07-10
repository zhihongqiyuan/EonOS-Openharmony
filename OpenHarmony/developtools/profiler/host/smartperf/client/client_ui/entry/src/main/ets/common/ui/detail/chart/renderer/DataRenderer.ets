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

import MyRect from '../data/Rect';
import Highlight from '../highlight/Highlight';
import EntryOhos from '../data/EntryOhos';
import Renderer from '../renderer/Renderer';
import ChartAnimator from '../animation/ChartAnimator';
import Paint, { Style, LinePaint, PathPaint, TextPaint } from '../data/Paint';
import ViewPortHandler from '../utils/ViewPortHandler';
import Utils from '../utils/Utils';
import IDataSet from '../interfaces/datasets/IDataSet';
import ChartInterface from '../interfaces/dataprovider/ChartInterface';
import IValueFormatter from '../formatter/IValueFormatter';

/**
 * Superclass of all render classes for the different data types (line, bar, ...).
 *
 * @author Philipp Jahoda
 */
export default abstract class DataRenderer extends Renderer {
  /**
   * the animator object used to perform animations on the chart data
   */
  protected mAnimator: ChartAnimator = new ChartAnimator();

  /**
   * main paint object used for rendering
   */
  protected mRenderPaint: Paint;

  protected mPathPaint: PathPaint;

  /**
   * paint used for highlighting values
   */
  protected mHighlightPaint: Paint;

  protected mDrawPaint: Paint;

  /**
   * paint object for drawing values (text representing values of chart
   * entries)
   */
  public mValuePaint: Paint;

  constructor(animator: ChartAnimator, viewPortHandler: ViewPortHandler) {
    super(viewPortHandler);
    this.mAnimator = animator;

    this.mRenderPaint = new Paint();
    this.mRenderPaint.setStyle(Style.FILL);

    this.mPathPaint = new PathPaint();

    this.mDrawPaint = new Paint();

    this.mValuePaint = new Paint();
    this.mValuePaint.setColor(Color.Green);
    this.mValuePaint.setTextAlign(TextAlign.Center);
    this.mValuePaint.setTextSize(10);

    this.mHighlightPaint = new Paint();
    this.mHighlightPaint.setStyle(Style.STROKE);
    this.mHighlightPaint.setStrokeWidth(2);
    this.mHighlightPaint.setColor(Color.Orange);
  }

  protected isDrawingValuesAllowed(chart: ChartInterface): boolean {
    return chart.getData().getEntryCount() < chart.getMaxVisibleCount() * this.mViewPortHandler.getScaleX();
  }

  /**
   * Returns the Paint object this renderer uses for drawing the values
   * (value-text).
   *
   * @return
   */
  public getPaintValues(): Paint {
    return this.mValuePaint;
  }

  /**
   * Returns the Paint object this renderer uses for drawing highlight
   * indicators.
   *
   * @return
   */
  public getPaintHighlight(): Paint {
    return this.mHighlightPaint;
  }

  /**
   * Returns the Paint object used for rendering.
   *
   * @return
   */
  public getPaintRender(): Paint {
    return this.mRenderPaint;
  }

  public getPathPaint(): PathPaint {
    return this.mPathPaint;
  }

  /**
   * Applies the required styling (provided by the DataSet) to the value-paint
   * object.
   *
   * @param set
   */
  protected applyValueTextStyle(dateSet: IDataSet<EntryOhos>) {
    this.mValuePaint.setTypeface(dateSet.getValueTypeface());
    this.mValuePaint.setTextSize(dateSet.getValueTextSize());
  }

  /**
   * Initializes the buffers used for rendering with a new size. Since this
   * method performs memory allocations, it should only be called if
   * necessary.
   */
  public abstract initBuffers();

  /**
   * Draws the actual data in form of lines, bars, ... depending on Renderer subclass.
   *
   * @param c
   */
  public abstract drawData(): Paint[];

  /**
   * Loops over all Entrys and draws their values.
   *
   * @param c
   */
  public abstract drawValues(): Paint[];

  /**
   * Draws the value of the given entry by using the provided IValueFormatter.
   *
   * @param c            canvas
   * @param formatter    formatter for custom value-formatting
   * @param value        the value to be drawn
   * @param entry        the entry the value belongs to
   * @param dataSetIndex the index of the DataSet the drawn Entry belongs to
   * @param x            position
   * @param y            position
   * @param color
   */
  public drawValue(
    formatter: IValueFormatter,
    value: number,
    entry: EntryOhos,
    dataSetIndex: number,
    x: number,
    y: number,
    color: number
  ): Paint[] {
    this.mValuePaint.setColor(color);
    let textPaint = new TextPaint();
    textPaint.set(this.mValuePaint);
    textPaint.setText(formatter.getFormattedValue(value, entry, dataSetIndex, this.mViewPortHandler));
    textPaint.setX(x);
    textPaint.setY(y);
    return [textPaint];
  }

  /**
   * Draws any kind of additional information (e.g. line-circles).
   *
   * @param c
   */
  public abstract drawExtras(): Paint[];

  /**
   * Draws all highlight indicators for the values that are currently highlighted.
   *
   * @param c
   * @param indices the highlighted values
   */
  public abstract drawHighlighted(indices: Highlight[]): Paint[];
}
