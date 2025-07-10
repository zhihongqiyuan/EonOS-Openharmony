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

import Runnable from '../data/Runnable';
import { JArrayList } from '../utils/JArrayList';
import ChartHighlighter from '../highlight/ChartHighlighter';
import MyRect from '../data/Rect';
import Highlight from '../highlight/Highlight';
import ChartAnimator from '../animation/ChartAnimator';
import ViewPortHandler from '../utils/ViewPortHandler';
import LegendRenderer from '../renderer/LegendRenderer';
import Legend from '../components/Legend';
import Description from '../components/Description';
import { XAxis } from '../components/XAxis';
import Paint, { TextPaint } from '../data/Paint';
import DefaultValueFormatter from '../formatter/DefaultValueFormatter';

import IDataSet from '../interfaces/datasets/IDataSet';
import EntryOhos from '../data/EntryOhos';

import OnChartValueSelectedListener from '../listener/OnChartValueSelectedListener';
import ChartTouchListener from '../listener/ChartTouchListener';
import OnChartGestureListener from '../listener/OnChartGestureListener';
import DataRenderer from '../renderer/DataRenderer';
import IHighlighter from '../highlight/IHighlighter';
import Utils from '../utils/Utils';
import { Color } from '../utils/ColorTemplate';
import MPPointF from '../utils/MPPointF';
import IMarker from '../components/IMarker';
import IValueFormatter from '../formatter/IValueFormatter';
import worker from '@ohos.worker';

/**
 * Baseclass of all Chart-Views.
 *
 * @author Philipp Jahoda
 */
export default abstract class Chart<T> {
  //  <T extends ChartData<? extends IDataSet<? extends EntryOhos>>> implements ChartInterface

  public static LOG_TAG: string = 'ohos-MPChart';

  /**
   * flag that indicates if logging is enabled or not
   */
  protected mLogEnabled: boolean = false;

  /**
   * object that holds all data that was originally set for the chart, before
   * it was modified or any filtering algorithms had been applied
   */
  protected mData: T = null;

  /**
   * Flag that indicates if highlighting per tap (touch) is enabled
   */
  protected mHighLightPerTapEnabled: boolean = true;

  /**
   * If set to true, chart continues to scroll after touch up
   */
  private mDragDecelerationEnabled: boolean = true;

  /**
   * Deceleration friction coefficient in [0 ; 1] interval, higher values
   * indicate that speed will decrease slowly, for example if it set to 0, it
   * will stop immediately. 1 is an invalid value, and will be converted to
   * 0.999f automatically.
   */
  private mDragDecelerationFrictionCoef: number = 0.9;

  /**
   * default value-formatter, number of digits depends on provided chart-data
   */
  protected mDefaultValueFormatter: DefaultValueFormatter = new DefaultValueFormatter(0);

  /**
   * paint object used for drawing the description text in the bottom right
   * corner of the chart
   */
  protected mDescPaint: Paint;

  /**
   * paint object for drawing the information text when there are no values in
   * the chart
   */
  protected mInfoPaint: Paint;

  /**
   * the object representing the labels on the x-axis
   */
  protected mXAxis: XAxis;

  /**
   * if true, touch gestures are enabled on the chart
   */
  protected mTouchEnabled: boolean = true;

  /**
   * the object responsible for representing the description text
   */
  protected mDescription: Description;

  /**
   * the legend object containing all data associated with the legend
   */
  protected mLegend: Legend;

  /**
   * listener that is called when a value on the chart is selected
   */
  protected mSelectionListener: OnChartValueSelectedListener;
  // @ts-ignore 泛型
  protected mChartTouchListener: ChartTouchListener;

  /**
   * text that is displayed when the chart is empty
   */
  private mNoDataText: string = 'No chart data available.';

  /**
   * Gesture listener for custom callbacks when making gestures on the chart.
   */
  private mGestureListener: OnChartGestureListener;
  protected mLegendRenderer: LegendRenderer;

  /**
   * object responsible for rendering the data
   */
  protected mRenderer: DataRenderer;
  protected mHighlighter: IHighlighter;

  /**
   * object that manages the bounds and drawing constraints of the chart
   */
  protected mViewPortHandler: ViewPortHandler = new ViewPortHandler();

  /**
   * object responsible for animations
   */
  protected mAnimator: ChartAnimator;

  /**
   * Extra offsets to be appended to the viewport
   */
  private mExtraTopOffset: number = 0;
  private mExtraRightOffset: number = 0;
  private mExtraBottomOffset: number = 0;
  private mExtraLeftOffset: number = 0;
  protected width: number = Utils.convertDpToPixel(50);
  protected height: number = Utils.convertDpToPixel(50);
  private background: Color | string | number | Resource;
  private workerInstance = new worker.Worker('workers/worker.js', { name: 'chart worker' });

  /**
   * default constructor for initialization in code
   */
  constructor() {
    this.init();
  }

  /**
   * initialize all paints and stuff
   */
  protected init() {
    this.mAnimator = new ChartAnimator();

    // initialize the utils
    Utils.init();
    this.mMaxHighlightDistance = Utils.convertDpToPixel(500);

    this.mDescription = new Description();
    this.mLegend = new Legend();

    this.mLegendRenderer = new LegendRenderer(this.mViewPortHandler, this.mLegend);

    this.mXAxis = new XAxis();

    this.mDescPaint = new TextPaint();

    this.mInfoPaint = new Paint();
    this.mInfoPaint.setColor(Color.rgb(247, 189, 51)); // orange
    this.mInfoPaint.setTextAlign(TextAlign.Center);
    this.mInfoPaint.setTextSize(Utils.convertDpToPixel(12));

    if (this.mLogEnabled) {
      console.info('Chart.init()');
    }
  }

  /**
   * Sets a new data object for the chart. The data object contains all values
   * and information needed for displaying.
   *
   * @param data
   */
  public setData(data: T) {
    this.mData = data;
    this.mOffsetsCalculated = false;

    if (data == null) {
      return;
    }

    // calculate how many digits are needed
    // @ts-ignore 缺失方法
    this.setupDefaultFormatter(data.getYMin(), data.getYMax());

    // @ts-ignore 缺失方法
    for (let item of this.mData.getDataSets()) {
      if (item.needsFormatter() || item.getValueFormatter() == this.mDefaultValueFormatter) {
        item.setValueFormatter(this.mDefaultValueFormatter);
      }
    }

    // let the chart know there is new data
    this.notifyDataSetChanged();

    if (this.mLogEnabled) {
      console.info(Chart.LOG_TAG + ':Data is set.');
    }
  }

  /**
   * Clears the chart from all data (sets it to null) and refreshes it (by
   * calling invalidate()).
   */
  public clear() {
    this.mData = null;
    this.mOffsetsCalculated = false;
    this.mIndicesToHighlight = null;
    this.mChartTouchListener.setLastHighlighted(null);
    this.invalidate();
  }

  /**
   * Removes all DataSets (and thereby Entries) from the chart. Does not set the data object to null. Also refreshes the
   * chart by calling invalidate().
   */
  public clearValues() {
    // @ts-ignore 缺失方法
    this.mData.clearValues();
    this.invalidate();
  }

  /**
   * Returns true if the chart is empty (meaning it's data object is either
   * null or contains no entries).
   *
   * @return
   */
  public isEmpty(): boolean {
    if (this.mData == null) {
      return true;
    } else {
      // @ts-ignore 缺失方法
      if (this.mData.getEntryCount() <= 0) {
        return true;
      } else {
        return false;
      }
    }
  }

  /**
   * Lets the chart know its underlying data has changed and performs all
   * necessary recalculations. It is crucial that this method is called
   * everytime data is changed dynamically. Not calling this method can lead
   * to crashes or unexpected behaviour.
   */
  public abstract notifyDataSetChanged();

  /**
   * Calculates the offsets of the chart to the border depending on the
   * position of an eventual legend or depending on the length of the y-axis
   * and x-axis labels and their position
   */
  protected abstract calculateOffsets();

  /**
   * Calculates the y-min and y-max value and the y-delta and x-delta value
   */
  protected abstract calcMinMax();

  /**
   * Calculates the required number of digits for the values that might be
   * drawn in the chart (if enabled), and creates the default-value-formatter
   */
  protected setupDefaultFormatter(min: number, max: number) {
    let reference: number = 0;

    // @ts-ignore 缺失方法
    if (this.mData == null || this.mData.getEntryCount() < 2) {
      reference = Math.max(Math.abs(min), Math.abs(max));
    } else {
      reference = Math.abs(max - min);
    }

    var digits: number = Utils.getDecimals(reference);

    // setup the formatter with a new number of digits
    this.mDefaultValueFormatter.setup(digits);
  }

  /**
   * flag that indicates if offsets calculation has already been done or not
   */
  private mOffsetsCalculated: boolean = false;

  protected onDraw(): Paint[] {
    if (this.mData == null) {
      let hasText: boolean = !this.strIsEmpty(this.mNoDataText);

      if (hasText) {
        let pt: MPPointF = this.getCenter();

        let textPaint: TextPaint = new TextPaint();
        textPaint.set(this.mInfoPaint);

        switch (this.mInfoPaint.getTextAlign()) {
          case TextAlign.Start:
            pt.x = 0;
            break;

          case TextAlign.End:
            pt.x *= 2.0;
            break;

          default:
            break;
        }
        textPaint.setText(this.mNoDataText);
        textPaint.setX(pt.x);
        textPaint.setY(pt.y);
        return [textPaint];
      }

      return [];
    }

    if (!this.mOffsetsCalculated) {
      this.calculateOffsets();
      this.mOffsetsCalculated = true;
    }
  }

  public strIsEmpty(str: string): boolean {
    return str == null || str.length == 0;
  }

  /**
   * Draws the description text in the bottom right corner of the chart (per default)
   */
  protected drawDescription(): Paint[] {
    if (this.mDescription != null && this.mDescription.isEnabled()) {
      let position: MPPointF = this.mDescription.getPosition();
      this.mDescPaint.setTypeface(this.mDescription.getTypeface());
      this.mDescPaint.setTextSize(this.mDescription.getTextSize());
      this.mDescPaint.setColor(this.mDescription.getTextColor());
      this.mDescPaint.setTextAlign(this.mDescription.getTextAlign());
      let x: number = 0;
      let y: number = 0;
      if (position == null) {
        x = this.width - this.mViewPortHandler.offsetRight() - this.mDescription.getXOffset();
        y = this.height - this.mViewPortHandler.offsetBottom() - this.mDescription.getYOffset();
      } else {
        x = position.x;
        y = position.y;
      }
      (this.mDescPaint as TextPaint).setText(this.mDescription.getText());
      this.mDescPaint.setX(x);
      this.mDescPaint.setY(y);
      return [this.mDescPaint];
    }
    return [];
  }

  /**
   * ################ ################ ################ ################
   */
  /** BELOW THIS CODE FOR HIGHLIGHTING */

  /**
   * array of Highlight objects that reference the highlighted slices in the
   * chart
   */
  protected mIndicesToHighlight: Highlight[];

  /**
   * The maximum distance in dp away from an entry causing it to highlight.
   */
  protected mMaxHighlightDistance: number = 0;

  public getMaxHighlightDistance(): number {
    return this.mMaxHighlightDistance;
  }

  /**
   * Sets the maximum distance in screen dp a touch can be away from an entry to cause it to get highlighted.
   * Default: 500dp
   *
   * @param distDp
   */
  public setMaxHighlightDistance(distDp: number) {
    this.mMaxHighlightDistance = Utils.convertDpToPixel(distDp);
  }

  /**
   * Returns the array of currently highlighted values. This might a null or
   * empty array if nothing is highlighted.
   *
   * @return
   */
  public getHighlighted(): Highlight[] {
    return this.mIndicesToHighlight;
  }

  /**
   * Returns true if values can be highlighted via tap gesture, false if not.
   *
   * @return
   */
  public isHighlightPerTapEnabled(): boolean {
    return this.mHighLightPerTapEnabled;
  }

  /**
   * Set this to false to prevent values from being highlighted by tap gesture.
   * Values can still be highlighted via drag or programmatically. Default: true
   *
   * @param enabled
   */
  public setHighlightPerTapEnabled(enabled: boolean) {
    this.mHighLightPerTapEnabled = enabled;
  }

  /**
   * Returns true if there are values to highlight, false if there are no
   * values to highlight. Checks if the highlight array is null, has a length
   * of zero or if the first object is null.
   *
   * @return
   */
  public valuesToHighlight(): boolean {
    return this.mIndicesToHighlight == null ||
           this.mIndicesToHighlight.length <= 0 ||
           this.mIndicesToHighlight[0] == null
      ? false
      : true;
  }

  /**
   * Sets the last highlighted value for the touchlistener.
   *
   * @param highs
   */
  protected setLastHighlighted(highs: Highlight[]) {
    if (highs == null || highs.length <= 0 || highs[0] == null) {
      this.mChartTouchListener.setLastHighlighted(null);
    } else {
      this.mChartTouchListener.setLastHighlighted(highs[0]);
    }
  }

  /**
   * Highlights the values at the given indices in the given DataSets. Provide
   * null or an empty array to undo all highlighting. This should be used to
   * programmatically highlight values.
   * This method *will not* call the listener.
   *
   * @param highs
   */
  public highlightValuesForArray(highs: Highlight[]) {
    // set the indices to highlight
    this.mIndicesToHighlight = highs;

    this.setLastHighlighted(highs);

    // redraw the chart
    this.invalidate();
  }

  /**
   * Highlights any y-value at the given x-value in the given DataSet.
   * Provide -1 as the dataSetIndex to undo all highlighting.
   * @param x The x-value to highlight
   * @param y The y-value to highlight. Supply `NaN` for "any"
   * @param dataSetIndex The dataset index to search in
   * @param dataIndex The data index to search in (only used in CombinedChartView currently)
   * @param callListener Should the listener be called for this change
   */
  public highlightValue(x: number, y?: number, dataSetIndex?: number, dataIndex?: number, callListener?: boolean) {
    if (y == null || y == undefined) {
      y = NaN;
    }
    if (dataIndex == null || dataIndex == undefined) {
      dataIndex = -1;
    }
    if (callListener == null || callListener == undefined) {
      callListener = true;
    }
    // @ts-ignore  缺失方法
    if (dataSetIndex < 0 || dataSetIndex >= this.mData.getDataSetCount()) {
      this.highlightValueForObject(null, callListener);
    } else {
      this.highlightValueForObject(new Highlight(x, y, dataSetIndex, dataIndex), callListener);
    }
  }

  /**
   * Highlights the value selected by touch gesture. Unlike
   * highlightValues(...), this generates a callback to the
   * OnChartValueSelectedListener.
   *
   * @param high         - the highlight object
   * @param callListener - call the listener
   */
  public highlightValueForObject(high: Highlight, callListener?: boolean) {
    if (callListener == null || callListener == undefined) {
      callListener = true;
    }
    let e: EntryOhos = null;

    if (high == null) {
      this.mIndicesToHighlight = null;
    } else {
      if (this.mLogEnabled) {
        console.info(Chart.LOG_TAG + ':Highlighted: ' + high.toString());
      }

      // @ts-ignore  缺失方法
      e = this.mData.getEntryForHighlight(high);
      if (e == null) {
        this.mIndicesToHighlight = null;
        high = null;
      } else {
        // set the indices to highlight
        this.mIndicesToHighlight = [high];
      }
    }
    this.setLastHighlighted(this.mIndicesToHighlight);

    if (callListener && this.mSelectionListener != null) {
      if (!this.valuesToHighlight()) {
        this.mSelectionListener.onNothingSelected();
      } else {
        // notify the listener
        this.mSelectionListener.onValueSelected(e, high);
      }
    }

    // redraw the chart
    this.invalidate();
  }

  public abstract invalidate();

  /**
   * Returns the Highlight object (contains x-index and DataSet index) of the
   * selected value at the given touch point inside the Line-, Scatter-, or
   * CandleStick-Chart.
   *
   * @param x
   * @param y
   * @return
   */
  public getHighlightByTouchPoint(x: number, y: number): Highlight {
    if (this.mData == null) {
      console.error(Chart.LOG_TAG + ":Can't select by touch. No data set.");
      return null;
    } else return this.getHighlighter().getHighlight(x, y);
  }

  /**
   * Set a new (e.g. custom) ChartTouchListener NOTE: make sure to
   * setTouchEnabled(true); if you need touch gestures on the chart
   *
   * @param l
   */
  // @ts-ignore 泛型
  public setOnTouchListener(l: ChartTouchListener) {
    this.mChartTouchListener = l;
  }

  /**
   * Returns an instance of the currently active touch listener.
   *
   * @return
   */
  // @ts-ignore 泛型
  public getOnTouchListener(): ChartTouchListener {
    return this.mChartTouchListener;
  }

  /**
   * ################ ################ ################ ################
   */
  /** BELOW CODE IS FOR THE MARKER VIEW */

  /**
   * if set to true, the marker view is drawn when a value is clicked
   */
  protected mDrawMarkers: boolean = true;

  /**
   * the view that represents the marker
   */
  protected mMarker: IMarker;

  /**
   * draws all MarkerViews on the highlighted positions
   */
  protected drawMarkers(): Paint[] {
    let paints: Paint[] = [];

    // if there is no marker view or drawing marker is disabled
    if (this.mMarker == null || !this.isDrawMarkersEnabled() || !this.valuesToHighlight()) {
      return;
    }

    for (let i = 0; i < this.mIndicesToHighlight.length; i++) {
      let highlight: Highlight = this.mIndicesToHighlight[i];

      // @ts-ignore  缺失方法 泛型问题
      let dataSet: IDataSet = this.mData.getDataSetByIndex(highlight.getDataSetIndex());

      // @ts-ignore  缺失方法
      let e: EntryOhos = this.mData.getEntryForHighlight(this.mIndicesToHighlight[i]);
      let entryIndex: number = dataSet.getEntryIndex(e);

      // make sure entry not null
      if (e == null || entryIndex > dataSet.getEntryCount() * this.mAnimator.getPhaseX()) {
        continue;
      }

      let pos: number[] = []
          pos = this.getMarkerPosition(highlight);
      // check bounds
      if (!this.mViewPortHandler.isInBounds(pos[0], pos[1])) {
        continue;
      }

      // callbacks to update the content
      this.mMarker.refreshContent(e, highlight);

      // draw the marker
      paints.concat(this.mMarker.draw(pos[0], pos[1]));
    }
    return paints;
  }

  /**
   * Returns the actual position in pixels of the MarkerView for the given
   * Highlight object.
   *
   * @param high
   * @return
   */
  protected getMarkerPosition(high: Highlight): number[] {
    return [high.getDrawX(), high.getDrawY()];
  }

  /** CODE BELOW THIS RELATED TO ANIMATION */

  /**
   * Returns the animator responsible for animating chart values.
   *
   * @return
   */
  public getAnimator(): ChartAnimator {
    return this.mAnimator;
  }

  /**
   * If set to true, chart continues to scroll after touch up default: true
   */
  public isDragDecelerationEnabled(): boolean {
    return this.mDragDecelerationEnabled;
  }

  /**
   * If set to true, chart continues to scroll after touch up. Default: true.
   *
   * @param enabled
   */
  public setDragDecelerationEnabled(enabled: boolean) {
    this.mDragDecelerationEnabled = enabled;
  }

  /**
   * Returns drag deceleration friction coefficient
   *
   * @return
   */
  public getDragDecelerationFrictionCoef(): number {
    return this.mDragDecelerationFrictionCoef;
  }

  /**
   * Deceleration friction coefficient in [0 ; 1] interval, higher values
   * indicate that speed will decrease slowly, for example if it set to 0, it
   * will stop immediately. 1 is an invalid value, and will be converted to
   * 0.999f automatically.
   *
   * @param newValue
   */
  public setDragDecelerationFrictionCoef(newValue: number) {
    if (newValue < 0) {
      newValue = 0;
    }

    if (newValue >= 1) {
      newValue = 0.999;
    }

    this.mDragDecelerationFrictionCoef = newValue;
  }

  /**
   * Returns the object representing all x-labels, this method can be used to
   * acquire the XAxis object and modify it (e.g. change the position of the
   * labels, styling, etc.)
   *
   * @return
   */
  public getXAxis(): XAxis {
    return this.mXAxis;
  }

  /**
   * Returns the default IValueFormatter that has been determined by the chart
   * considering the provided minimum and maximum values.
   *
   * @return
   */
  public getDefaultValueFormatter(): IValueFormatter {
    return this.mDefaultValueFormatter;
  }

  /**
   * set a selection listener for the chart
   *
   * @param l
   */
  public setOnChartValueSelectedListener(l: OnChartValueSelectedListener) {
    this.mSelectionListener = l;
  }

  /**
   * Sets a gesture-listener for the chart for custom callbacks when executing
   * gestures on the chart surface.
   *
   * @param l
   */
  public setOnChartGestureListener(l: OnChartGestureListener) {
    this.mGestureListener = l;
  }

  /**
   * Returns the custom gesture listener.
   *
   * @return
   */
  public getOnChartGestureListener(): OnChartGestureListener {
    return this.mGestureListener;
  }

  /**
   * returns the current y-max value across all DataSets
   *
   * @return
   */
  public getYMax(): number {
    // @ts-ignore  缺失方法
    return this.mData.getYMax();
  }

  /**
   * returns the current y-min value across all DataSets
   *
   * @return
   */
  public getYMin(): number {
    // @ts-ignore  缺失方法
    return this.mData.getYMin();
  }

  public getXChartMax(): number {
    return this.mXAxis.mAxisMaximum;
  }

  public getXChartMin(): number {
    return this.mXAxis.mAxisMinimum;
  }

  public getXRange(): number {
    return this.mXAxis.mAxisRange;
  }

  /**
   * Returns a recyclable MPPointF instance.
   * Returns the center point of the chart (the whole View) in pixels.
   *
   * @return
   */
  public getCenter(): MPPointF {
    return MPPointF.getInstance(this.width / 2, this.height / 2);
  }

  public getWidth() {
    return this.width;
  }

  public getHeight() {
    return this.height;
  }

  /**
   * Returns a recyclable MPPointF instance.
   * Returns the center of the chart taking offsets under consideration.
   * (returns the center of the content rectangle)
   *
   * @return
   */
  public getCenterOffsets(): MPPointF {
    return this.mViewPortHandler.getContentCenter();
  }

  /**
   * Sets extra offsets (around the chart view) to be appended to the
   * auto-calculated offsets.
   *
   * @param left
   * @param top
   * @param right
   * @param bottom
   */
  public setExtraOffsets(left: number, top: number, right: number, bottom: number) {
    this.setExtraLeftOffset(left);
    this.setExtraTopOffset(top);
    this.setExtraRightOffset(right);
    this.setExtraBottomOffset(bottom);
  }

  /**
   * Set an extra offset to be appended to the viewport's top
   */
  public setExtraTopOffset(offset: number) {
    this.mExtraTopOffset = Utils.convertDpToPixel(offset);
  }

  /**
   * @return the extra offset to be appended to the viewport's top
   */
  public getExtraTopOffset(): number {
    return this.mExtraTopOffset;
  }

  /**
   * Set an extra offset to be appended to the viewport's right
   */
  public setExtraRightOffset(offset: number) {
    this.mExtraRightOffset = Utils.convertDpToPixel(offset);
  }

  /**
   * @return the extra offset to be appended to the viewport's right
   */
  public getExtraRightOffset(): number {
    return this.mExtraRightOffset;
  }

  /**
   * Set an extra offset to be appended to the viewport's bottom
   */
  public setExtraBottomOffset(offset: number) {
    this.mExtraBottomOffset = Utils.convertDpToPixel(offset);
  }

  /**
   * @return the extra offset to be appended to the viewport's bottom
   */
  public getExtraBottomOffset(): number {
    return this.mExtraBottomOffset;
  }

  /**
   * Set an extra offset to be appended to the viewport's left
   */
  public setExtraLeftOffset(offset: number) {
    this.mExtraLeftOffset = Utils.convertDpToPixel(offset);
  }

  /**
   * @return the extra offset to be appended to the viewport's left
   */
  public getExtraLeftOffset(): number {
    return this.mExtraLeftOffset;
  }

  /**
   * Set this to true to enable logcat outputs for the chart. Beware that
   * logcat output decreases rendering performance. Default: disabled.
   *
   * @param enabled
   */
  public setLogEnabled(enabled: boolean) {
    this.mLogEnabled = enabled;
  }

  /**
   * Returns true if log-output is enabled for the chart, fals if not.
   *
   * @return
   */
  public isLogEnabled(): boolean {
    return this.mLogEnabled;
  }

  /**
   * Sets the text that informs the user that there is no data available with
   * which to draw the chart.
   *
   * @param text
   */
  public setNoDataText(text: string) {
    this.mNoDataText = text;
  }

  /**
   * Sets the color of the no data text.
   *
   * @param color
   */
  public setNoDataTextColor(color: number | string) {
    this.mInfoPaint.setColor(color);
  }

  /**
   * Sets the typeface to be used for the no data text.
   *
   * @param tf
   */
  public setNoDataTextTypeface(tf: FontWeight) {
    this.mInfoPaint.setTypeface(tf);
  }

  /**
   * alignment of the no data text
   *
   * @param align
   */
  public setNoDataTextAlignment(align: TextAlign) {
    this.mInfoPaint.setTextAlign(align);
  }

  /**
   * Set this to false to disable all gestures and touches on the chart,
   * default: true
   *
   * @param enabled
   */
  public setTouchEnabled(enabled: boolean) {
    this.mTouchEnabled = enabled;
  }

  /**
   * sets the marker that is displayed when a value is clicked on the chart
   *
   * @param marker
   */
  public setMarker(marker: IMarker) {
    this.mMarker = marker;
  }

  /**
   * returns the marker that is set as a marker view for the chart
   *
   * @return
   */
  public getMarker(): IMarker {
    return this.mMarker;
  }

  public setMarkerView(v: IMarker) {
    this.setMarker(v);
  }

  public getMarkerView(): IMarker {
    return this.getMarker();
  }

  /**
   * Sets a new Description object for the chart.
   *
   * @param desc
   */
  public setDescription(desc: Description) {
    this.mDescription = desc;
  }

  /**
   * Returns the Description object of the chart that is responsible for holding all information related
   * to the description text that is displayed in the bottom right corner of the chart (by default).
   *
   * @return
   */
  public getDescription(): Description {
    return this.mDescription;
  }

  /**
   * Returns the Legend object of the chart. This method can be used to get an
   * instance of the legend in order to customize the automatically generated
   * Legend.
   *
   * @return
   */
  public getLegend(): Legend {
    return this.mLegend;
  }

  /**
   * Returns the renderer object responsible for rendering / drawing the
   * Legend.
   *
   * @return
   */
  public getLegendRenderer(): LegendRenderer {
    return this.mLegendRenderer;
  }

  /**
   * Returns the rectangle that defines the borders of the chart-value surface
   * (into which the actual values are drawn).
   *
   * @return
   */
  public getContentRect(): MyRect {
    return this.mViewPortHandler.getContentRect();
  }

  /**
   * paint for the grid background (only line and barchart)
   */
  public static PAINT_GRID_BACKGROUND: number = 4;

  /**
   * paint for the info text that is displayed when there are no values in the
   * chart
   */
  public static PAINT_INFO: number = 7;

  /**
   * paint for the description text in the bottom right corner
   */
  public static PAINT_DESCRIPTION: number = 11;

  /**
   * paint for the hole in the middle of the pie chart
   */
  public static PAINT_HOLE: number = 13;

  /**
   * paint for the text in the middle of the pie chart
   */
  public static PAINT_CENTER_TEXT: number = 14;

  /**
   * paint used for the legend
   */
  public static PAINT_LEGEND_LABEL: number = 18;

  /**
   * set a new paint object for the specified parameter in the chart e.g.
   * Chart.PAINT_VALUES
   *
   * @param p     the new paint object
   * @param which Chart.PAINT_VALUES, Chart.PAINT_GRID, Chart.PAINT_VALUES,
   *              ...
   */
  public setPaint(p: Paint, which: number) {
    switch (which) {
      case Chart.PAINT_INFO:
        this.mInfoPaint = p;
        break;
      case Chart.PAINT_DESCRIPTION:
        this.mDescPaint = p;
        break;
    }
  }

  /**
   * Returns the paint object associated with the provided constant.
   *
   * @param which e.g. Chart.PAINT_LEGEND_LABEL
   * @return
   */
  public getPaint(which: number): Paint {
    switch (which) {
      case Chart.PAINT_INFO:
        return this.mInfoPaint;
      case Chart.PAINT_DESCRIPTION:
        return this.mDescPaint;
    }

    return null;
  }

  public isDrawMarkerViewsEnabled(): boolean {
    return this.isDrawMarkersEnabled();
  }

  public setDrawMarkerViews(enabled: boolean) {
    this.setDrawMarkers(enabled);
  }

  /**
   * returns true if drawing the marker is enabled when tapping on values
   * (use the setMarker(IMarker marker) method to specify a marker)
   *
   * @return
   */
  public isDrawMarkersEnabled(): boolean {
    return this.mDrawMarkers;
  }

  /**
   * Set this to true to draw a user specified marker when tapping on
   * chart values (use the setMarker(IMarker marker) method to specify a
   * marker). Default: true
   *
   * @param enabled
   */
  public setDrawMarkers(enabled: boolean) {
    this.mDrawMarkers = enabled;
  }

  /**
   * Returns the ChartData object that has been set for the chart.
   *
   * @return
   */
  public getData(): T {
    return this.mData;
  }

  /**
   * Returns the ViewPortHandler of the chart that is responsible for the
   * content area of the chart and its offsets and dimensions.
   *
   * @return
   */
  public getViewPortHandler(): ViewPortHandler {
    return this.mViewPortHandler;
  }

  /**
   * Returns the Renderer object the chart uses for drawing data.
   *
   * @return
   */
  public getRenderer(): DataRenderer {
    return this.mRenderer;
  }

  /**
   * Sets a new DataRenderer object for the chart.
   *
   * @param renderer
   */
  public setRenderer(renderer: DataRenderer) {
    if (renderer != null) {
      this.mRenderer = renderer;
    }
  }

  public getHighlighter(): IHighlighter {
    return this.mHighlighter;
  }

  /**
   * Sets a custom highligher object for the chart that handles / processes
   * all highlight touch events performed on the chart-view.
   *
   * @param highlighter
   */
  // @ts-ignore 泛型问题
  public setHighlighter(highlighter: ChartHighlighter) {
    this.mHighlighter = highlighter;
  }

  /**
   * Returns a recyclable MPPointF instance.
   *
   * @return
   */
  public getCenterOfView(): MPPointF {
    return this.getCenter();
  }

  /**
   * Returns the bitmap that represents the chart.
   *
   * @return
   */
  public getChartBitmap(): Paint[] {
    let returnedBitmap: Paint[] = [];
    let background = this.getBackground();
    if (background == null) {
      background = '#ffffff';
    }
    return returnedBitmap;
  }

  setBackground(background: Color | string | number | Resource) {
    this.background = background;
  }

  getBackground(): Color | string | number | Resource {
    return this.background;
  }

  /**
   * tasks to be done after the view is setup
   */
  protected mJobs: JArrayList<Runnable> = new JArrayList<Runnable>();

  public removeViewportJob(job: Runnable) {
    // @ts-ignore
    this.workerInstance.removeEventListener(job._type);
    this.mJobs.remove(job);
  }

  public clearAllViewportJobs() {
    // @ts-ignore EventListener
    this.workerInstance.removeAllListener();
    this.mJobs.clear();
  }

  /**
   * Either posts a job immediately if the chart has already setup it's
   * dimensions or adds the job to the execution queue.
   *
   * @param job
   */
  public addViewportJob(job: Runnable) {
    if (this.mViewPortHandler.hasChartDimens()) {
      this.post(job);
    } else {
      this.mJobs.add(job);
    }
  }

  protected post(job: Runnable) {
    // @ts-ignore
    this.workerInstance.dispatchEvent({ type: job._type });
  }

  /**
   * Returns all jobs that are scheduled to be executed after
   * onSizeChanged(...).
   *
   * @return
   */
  public getJobs(): JArrayList<Runnable> {
    return this.mJobs;
  }

  /**
   * unbind flag
   */
  private mUnbind: boolean = false;

  /**
   * Set this to true to enable "unbinding" of drawables. When a View is detached
   * from a window. This helps avoid memory leaks.
   * Default: false
   * Link: http://stackoverflow.com/a/6779164/1590502
   *
   * @param enabled
   */
  public setUnbindEnabled(enabled: boolean) {
    this.mUnbind = enabled;
  }
}
