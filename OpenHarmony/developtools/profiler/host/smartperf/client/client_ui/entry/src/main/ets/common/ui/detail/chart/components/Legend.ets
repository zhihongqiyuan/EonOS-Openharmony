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

import ColorTemplate from '../utils/ColorTemplate';
import FSize from '../utils/FSize';
import Utils from '../utils/Utils';
import ViewPortHandler from '../utils/ViewPortHandler';
import { JArrayList } from '../utils/JArrayList';

import LegendEntry from './LegendEntry';
import ComponentBase from './ComponentBase';
import Paint from '../data/Paint';

export enum LegendHorizontalAlignment {
  LEFT,
  CENTER,
  RIGHT,
}

export enum LegendVerticalAlignment {
  TOP,
  CENTER,
  BOTTOM,
}

export enum LegendOrientation {
  HORIZONTAL,
  VERTICAL,
}

export enum LegendDirection {
  LEFT_TO_RIGHT,
  RIGHT_TO_LEFT,
}
/**
 * Class representing the legend of the chart. The legend will contain one entry
 * per color and DataSet. Multiple colors in one DataSet are grouped together.
 * The legend object is NOT available before setting data to the chart.
 *
 */
export default class Legend extends ComponentBase {
  /**
   * The legend entries array
   */
  private mEntries: LegendEntry[] = [];

  /**
   * Entries that will be appended to the end of the auto calculated entries after calculating the legend.
   */
  private mExtraEntries: LegendEntry[];

  /**
   * Are the legend labels/colors a custom value or auto calculated? If false,
   * then it's auto, if true, then custom. default false (automatic legend)
   */
  private mIsLegendCustom: boolean = false;

  private mHorizontalAlignment: LegendHorizontalAlignment = LegendHorizontalAlignment.LEFT;
  private mVerticalAlignment: LegendVerticalAlignment = LegendVerticalAlignment.BOTTOM;
  private mOrientation: LegendOrientation = LegendOrientation.HORIZONTAL;
  private mDrawInside: boolean = false;

  /**
   * the text direction for the legend
   */
  private mDirection: LegendDirection = LegendDirection.LEFT_TO_RIGHT;

  /**
   * the shape/form the legend colors are drawn in
   */
  private mShape: LegendForm = LegendForm.SQUARE;

  /**
   * the size of the legend forms/shapes
   */
  private mFormSize: number = 8;

  /**
   * the size of the legend forms/shapes
   */
  private mFormLineWidth: number = 3;

  /**
   * Line dash path effect used for shapes that consist of lines.
   */
  private mFormLineDashEffect: Object /*DashPathEffect*/ = null;

  /**
   * the space between the legend entries on a horizontal axis, default 6f
   */
  private mXEntrySpace: number = 6;

  /**
   * the space between the legend entries on a vertical axis, default 5f
   */
  private mYEntrySpace: number = 0;

  /**
   * the space between the legend entries on a vertical axis, default 2f
   * private float mYEntrySpace = 2f; /** the space between the form and the
   * actual label/text
   */
  private mFormToTextSpace: number = 5;

  /**
   * the space that should be left between stacked forms
   */
  private mStackSpace: number = 3;

  /**
   * the maximum relative size out of the whole chart view in percent
   */
  private mMaxSizePercent: number = 0.95;

  /**
   * default constructor
   */
  constructor(entries?: LegendEntry[]) {
    super();
    this.mTextSize = 20;
    this.mXOffset = 5;
    this.mYOffset = 3;
    if (entries != null && entries != undefined) {
      if (entries.length == 0) {
        throw new Error('entries array is NULL');
      }
      this.mEntries.concat(entries);
    }
  }
  /**
   * This method sets the automatically computed colors for the legend. Use setCustom(...) to set custom colors.
   *
   * @param entries
   */
  public setEntries(entries: JArrayList<LegendEntry>): void {
    this.mEntries = entries.toArray(new Array(entries.size()));
  }

  public getEntries(): LegendEntry[] {
    return this.mEntries;
  }

  /**
   * returns the maximum length in pixels across all legend labels + formsize
   * + formtotextspace
   *
   * @param p the paint object used for rendering the text
   * @return
   */
  public getMaximumEntryWidth(p: Paint): number {
    var max: number = 0;
    var maxFormSize: number = 0;
    var formToTextSpace: number = Utils.convertDpToPixel(this.mFormToTextSpace);

    for (let entry of this.mEntries) {
      var formSize: number = Utils.convertDpToPixel(Number.isNaN(entry.formSize) ? this.mFormSize : entry.formSize);
      if (formSize > maxFormSize) {
        maxFormSize = formSize;
      }

      var label: string = entry.label;
      if (label == null) {
        continue;
      }

      var length: number = Utils.calcTextWidth(p, label);

      if (length > max) {
        max = length;
      }
    }

    return max + maxFormSize + formToTextSpace;
  }

  /**
   * returns the maximum height in pixels across all legend labels
   *
   * @param p the paint object used for rendering the text
   * @return
   */
  public getMaximumEntryHeight(p: Paint): number {
    var max: number = 0;

    for (let entry of this.mEntries) {
      var label: string = entry.label;
      if (label == null) {
        continue;
      }

      var length: number = Utils.calcTextHeight(p, label);

      if (length > max) {
        max = length;
      }
    }

    return max;
  }

  public getExtraEntries(): LegendEntry[] {
    return this.mExtraEntries;
  }

  /**
   * Entries that will be appended to the end of the auto calculated
   *   entries after calculating the legend.
   * (if the legend has already been calculated, you will need to call notifyDataSetChanged()
   *   to let the changes take effect)
   */
  public setExtra(colors?: number[], labels?: string[], entries?: JArrayList<LegendEntry>): void {
    if (entries && !colors && !labels) {
      this.mExtraEntries = entries.toArray(new LegendEntry[entries.size()]());
      return;
    }

    var entries: JArrayList<LegendEntry> = new JArrayList<LegendEntry>();

    for (var i: number = 0; i < Math.min(colors.length, labels.length); i++) {
      var entry: LegendEntry = new LegendEntry();
      entry.formColor = colors[i];
      entry.label = labels[i];

      if (entry.formColor == ColorTemplate.COLOR_SKIP || entry.formColor == 0) {
        entry.form = LegendForm.NONE;
      } else if (entry.formColor == ColorTemplate.COLOR_NONE) {
        entry.form = LegendForm.EMPTY;
      }

      entries.add(entry);
    }

    this.mExtraEntries = entries.toArray(new LegendEntry[entries.size()]());
  }

  /**
   * Sets a custom legend's entries array.
   * * A null label will start a group.
   * This will disable the feature that automatically calculates the legend
   *   entries from the datasets.
   * Call resetCustom() to re-enable automatic calculation (and then
   *   notifyDataSetChanged() is needed to auto-calculate the legend again)
   */
  public setCustom(entries: JArrayList<LegendEntry>): void {
    this.mEntries = [];
    for (var i: number = 0; i < entries.size(); i++) {
      this.mEntries[i] = entries.at(i);
    }
    this.mIsLegendCustom = true;
  }

  /**
   * Calling this will disable the custom legend entries (set by
   * setCustom(...)). Instead, the entries will again be calculated
   * automatically (after notifyDataSetChanged() is called).
   */
  public resetCustom(): void {
    this.mIsLegendCustom = false;
  }

  /**
   * @return true if a custom legend entries has been set default
   * false (automatic legend)
   */
  public isLegendCustom(): boolean {
    return this.mIsLegendCustom;
  }

  /**
   * returns the horizontal alignment of the legend
   *
   * @return
   */
  public getHorizontalAlignment(): LegendHorizontalAlignment {
    return this.mHorizontalAlignment;
  }

  /**
   * sets the horizontal alignment of the legend
   *
   * @param value
   */
  public setHorizontalAlignment(value: LegendHorizontalAlignment): void {
    this.mHorizontalAlignment = value;
  }

  /**
   * returns the vertical alignment of the legend
   *
   * @return
   */
  public getVerticalAlignment(): LegendVerticalAlignment {
    return this.mVerticalAlignment;
  }

  /**
   * sets the vertical alignment of the legend
   *
   * @param value
   */
  public setVerticalAlignment(value: LegendVerticalAlignment): void {
    this.mVerticalAlignment = value;
  }

  /**
   * returns the orientation of the legend
   *
   * @return
   */
  public getOrientation(): LegendOrientation {
    return this.mOrientation;
  }

  /**
   * sets the orientation of the legend
   *
   * @param value
   */
  public setOrientation(value: LegendOrientation): void {
    this.mOrientation = value;
  }

  /**
   * returns whether the legend will draw inside the chart or outside
   *
   * @return
   */
  public isDrawInsideEnabled(): boolean {
    return this.mDrawInside;
  }

  /**
   * sets whether the legend will draw inside the chart or outside
   *
   * @param value
   */
  public setDrawInside(value: boolean): void {
    this.mDrawInside = value;
  }

  /**
   * returns the text direction of the legend
   *
   * @return
   */
  public getDirection(): LegendDirection {
    return this.mDirection;
  }

  /**
   * sets the text direction of the legend
   *
   * @param pos
   */
  public setDirection(pos: LegendDirection): void {
    this.mDirection = pos;
  }

  /**
   * returns the current form/shape that is set for the legend
   *
   * @return
   */
  public getForm(): LegendForm {
    return this.mShape;
  }

  /**
   * sets the form/shape of the legend forms
   *
   * @param shape
   */
  public setForm(shape: LegendForm): void {
    this.mShape = shape;
  }

  /**
   * sets the size in dp of the legend forms, default 8f
   *
   * @param size
   */
  public setFormSize(size: number): void {
    this.mFormSize = size;
  }

  /**
   * returns the size in dp of the legend forms
   *
   * @return
   */
  public getFormSize(): number {
    return this.mFormSize;
  }

  /**
   * sets the line width in dp for forms that consist of lines, default 3f
   *
   * @param size
   */
  public setFormLineWidth(size: number): void {
    this.mFormLineWidth = size;
  }

  /**
   * returns the line width in dp for drawing forms that consist of lines
   *
   * @return
   */
  public getFormLineWidth(): number {
    return this.mFormLineWidth;
  }

  /**
   * Sets the line dash path effect used for shapes that consist of lines.
   *
   * @param dashPathEffect
   */
  public setFormLineDashEffect(dashPathEffect: Object): void {
    this.mFormLineDashEffect = dashPathEffect;
  }

  /**
   * @return The line dash path effect used for shapes that consist of lines.
   */
  public getFormLineDashEffect(): Object {
    return this.mFormLineDashEffect;
  }

  /**
   * returns the space between the legend entries on a horizontal axis in
   * pixels
   *
   * @return
   */
  public getXEntrySpace(): number {
    return this.mXEntrySpace;
  }

  /**
   * sets the space between the legend entries on a horizontal axis in pixels,
   * converts to dp internally
   *
   * @param space
   */
  public setXEntrySpace(space: number): void {
    this.mXEntrySpace = space;
  }

  /**
   * returns the space between the legend entries on a vertical axis in pixels
   *
   * @return
   */
  public getYEntrySpace(): number {
    return this.mYEntrySpace;
  }

  /**
   * sets the space between the legend entries on a vertical axis in pixels,
   * converts to dp internally
   *
   * @param space
   */
  public setYEntrySpace(space: number): void {
    this.mYEntrySpace = space;
  }

  /**
   * returns the space between the form and the actual label/text
   *
   * @return
   */
  public getFormToTextSpace(): number {
    return this.mFormToTextSpace;
  }

  /**
   * sets the space between the form and the actual label/text, converts to dp
   * internally
   *
   * @param space
   */
  public setFormToTextSpace(space: number): void {
    this.mFormToTextSpace = space;
  }

  /**
   * returns the space that is left out between stacked forms (with no label)
   *
   * @return
   */
  public getStackSpace(): number {
    return this.mStackSpace;
  }

  /**
   * sets the space that is left out between stacked forms (with no label)
   *
   * @param space
   */
  public setStackSpace(space: number): void {
    this.mStackSpace = space;
  }

  /**
   * the total width of the legend (needed width space)
   */
  public mNeededWidth: number = 0;

  /**
   * the total height of the legend (needed height space)
   */
  public mNeededHeight: number = 0;

  public mTextHeightMax: number = 0;

  public mTextWidthMax: number = 0;

  /**
   * flag that indicates if word wrapping is enabled
   */
  private mWordWrapEnabled: boolean = false;

  /**
   * Should the legend word wrap? / this is currently supported only for:
   * BelowChartLeft, BelowChartRight, BelowChartCenter. / note that word
   * wrapping a legend takes a toll on performance. / you may want to set
   * maxSizePercent when word wrapping, to set the point where the text wraps.
   * / default: false
   *
   * @param enabled
   */
  public setWordWrapEnabled(enabled: boolean): void {
    this.mWordWrapEnabled = enabled;
  }

  /**
   * If this is set, then word wrapping the legend is enabled. This means the
   * legend will not be cut off if too long.
   *
   * @return
   */
  public isWordWrapEnabled(): boolean {
    return this.mWordWrapEnabled;
  }

  /**
   * The maximum relative size out of the whole chart view. / If the legend is
   * to the right/left of the chart, then this affects the width of the
   * legend. / If the legend is to the top/bottom of the chart, then this
   * affects the height of the legend. / If the legend is the center of the
   * piechart, then this defines the size of the rectangular bounds out of the
   * size of the "hole". / default: 0.95f (95%)
   *
   * @return
   */
  public getMaxSizePercent(): number {
    return this.mMaxSizePercent;
  }

  /**
   * The maximum relative size out of the whole chart view. / If
   * the legend is to the right/left of the chart, then this affects the width
   * of the legend. / If the legend is to the top/bottom of the chart, then
   * this affects the height of the legend. / default: 0.95f (95%)
   *
   * @param maxSize
   */
  public setMaxSizePercent(maxSize: number): void {
    this.mMaxSizePercent = maxSize;
  }

  private mCalculatedLabelSizes: JArrayList<FSize> = new JArrayList<FSize>(/*16*/);
  private mCalculatedLabelBreakPoints: JArrayList<Boolean> = new JArrayList<Boolean>(/*16*/);
  private mCalculatedLineSizes: JArrayList<FSize> = new JArrayList<FSize>(/*16*/);

  public getCalculatedLabelSizes(): JArrayList<FSize> {
    return this.mCalculatedLabelSizes;
  }

  public getCalculatedLabelBreakPoints(): JArrayList<Boolean> {
    return this.mCalculatedLabelBreakPoints;
  }

  public getCalculatedLineSizes(): JArrayList<FSize> {
    return this.mCalculatedLineSizes;
  }

  /**
   * Calculates the dimensions of the Legend. This includes the maximum width
   * and height of a single entry, as well as the total width and height of
   * the Legend.
   *
   * @param labelpaint
   */
  public calculateDimensions(labelpaint: Paint, viewPortHandler: ViewPortHandler): void {
    var defaultFormSize: number = Utils.convertDpToPixel(this.mFormSize);
    var stackSpace: number = Utils.convertDpToPixel(this.mStackSpace);
    var formToTextSpace: number = Utils.convertDpToPixel(this.mFormToTextSpace);
    var xEntrySpace: number = Utils.convertDpToPixel(this.mXEntrySpace);
    var yEntrySpace: number = Utils.convertDpToPixel(this.mYEntrySpace);
    var wordWrapEnabled: boolean = this.mWordWrapEnabled;
    var entries: LegendEntry[] = this.mEntries;
    var entryCount: number = entries.length;

    this.mTextWidthMax = this.getMaximumEntryWidth(labelpaint);
    this.mTextHeightMax = this.getMaximumEntryHeight(labelpaint);

    switch (this.mOrientation) {
      case LegendOrientation.VERTICAL: {
        var maxWidth: number = 0;
        var maxHeight: number = 0;
        var width: number = 0;
        var labelLineHeight: number = Utils.getLineHeight(labelpaint);
        var wasStacked: boolean = false;

        for (var i: number = 0; i < entryCount; i++) {
          var e: LegendEntry = entries[i];
          var drawingForm: boolean = e.form != LegendForm.NONE;
          var formSize: number = Number.isNaN(e.formSize) ? defaultFormSize : Utils.convertDpToPixel(e.formSize);
          var label: string = e.label;

          if (!wasStacked) {
            width = 0.1;
          }

          if (drawingForm) {
            if (wasStacked) {
              width += stackSpace;
            }
            width += formSize;
          }

          // grouped forms have null labels
          if (label != null) {
            // make a step to the left
            if (drawingForm && !wasStacked) {
              width += formToTextSpace;
            } else if (wasStacked) {
              maxWidth = Math.max(maxWidth, width);
              maxHeight += labelLineHeight + yEntrySpace;
              width = 0.1;
              wasStacked = false;
            }

            width += Utils.calcTextWidth(labelpaint, label);

            maxHeight += labelLineHeight + yEntrySpace;
          } else {
            wasStacked = true;
            width += formSize;
            if (i < entryCount - 1) {
              width += stackSpace;
            }
          }

          maxWidth = Math.max(maxWidth, width);
        }

        this.mNeededWidth = maxWidth;
        this.mNeededHeight = maxHeight;

        break;
      }
      case LegendOrientation.HORIZONTAL: {
        var labelLineHeight: number = Utils.getLineHeight(labelpaint);
        var labelLineSpacing: number = Utils.getLineSpacing(labelpaint) + yEntrySpace;
        var contentWidth: number = viewPortHandler.contentWidth() * this.mMaxSizePercent;

        // Start calculating layout
        var maxLineWidth: number = 0.0;
        var currentLineWidth: number = 0.0;
        var requiredWidth: number = 0.0;
        var stackedStartIndex: number = -1;

        this.mCalculatedLabelBreakPoints.clear();
        this.mCalculatedLabelSizes.clear();
        this.mCalculatedLineSizes.clear();

        for (var i: number = 0; i < entryCount; i++) {
          var e: LegendEntry = entries[i];
          var drawingForm: boolean = e.form != LegendForm.NONE;
          var formSize: number = Number.isNaN(e.formSize) ? defaultFormSize : Utils.convertDpToPixel(e.formSize);
          var label: string = e.label;

          this.mCalculatedLabelBreakPoints.add(false);

          if (stackedStartIndex == -1) {
            // we are not stacking, so required width is for this label
            // only
            requiredWidth = 0.0;
          } else {
            // add the spacing appropriate for stacked labels/forms
            requiredWidth += stackSpace;
          }

          // grouped forms have null labels
          if (label != null) {
            this.mCalculatedLabelSizes.add(Utils.calcTextSize(labelpaint, label)); // ��ǩ��length
            requiredWidth += drawingForm ? formToTextSpace + formSize : 0.0;
            requiredWidth += this.mCalculatedLabelSizes.get(i).width;
          } else {
            this.mCalculatedLabelSizes.add(FSize.getInstance(0.0, 0.0));
            requiredWidth += drawingForm ? formSize : 0.0;

            if (stackedStartIndex == -1) {
              // mark this index as we might want to break here later
              stackedStartIndex = i;
            }
          }

          if (label != null || i == entryCount - 1) {
            var requiredSpacing: number = currentLineWidth == 0.0 ? 0.0 : xEntrySpace;

            if (
            !wordWrapEnabled || // No word wrapping, it must fit.
            // The line is empty, it must fit
            currentLineWidth == 0.0 ||
            // It simply fits
            contentWidth - currentLineWidth >= requiredSpacing + requiredWidth
            ) {
              // Expand current line
              currentLineWidth += requiredSpacing + requiredWidth;
            } else {
              // It doesn't fit, we need to wrap a line

              // Add current line size to array
              this.mCalculatedLineSizes.add(FSize.getInstance(currentLineWidth, labelLineHeight));
              maxLineWidth = Math.max(maxLineWidth, currentLineWidth);

              // Start a new line
              if (stackedStartIndex > -1) {
                this.mCalculatedLabelBreakPoints.append(true);
              }

              currentLineWidth = requiredWidth;
            }

            if (i == entryCount - 1) {
              // Add last line size to array
              this.mCalculatedLineSizes.add(FSize.getInstance(currentLineWidth, labelLineHeight));
              maxLineWidth = Math.max(maxLineWidth, currentLineWidth);
            }
          }

          stackedStartIndex = label != null ? -1 : stackedStartIndex;
        }

        this.mNeededWidth = maxLineWidth;
        this.mNeededHeight =
        labelLineHeight * this.mCalculatedLineSizes.size() +
        labelLineSpacing * (this.mCalculatedLineSizes.size() == 0 ? 0 : this.mCalculatedLineSizes.size() - 1);

        break;
      }
    }

    this.mNeededHeight += this.mYOffset;
    this.mNeededWidth += this.mXOffset;
  }
  public getMaxEntryHeight(): number {
    var max: number = 0;
    for (let entry of this.mEntries) {
      var label: string = entry.label;
      if (label == null) {
        continue;
      }
      var length: number = this.getTextSize();
      if (length > max) {
        max = length;
      }
    }
    return max;
  }
  public getMaxEntryWidth(): number {
    var max: number = 0;
    var maxFormSize: number = 0;
    var formToTextSpace: number = this.mFormToTextSpace;
    for (let entry of this.mEntries) {
      var formSize: number = Utils.convertDpToPixel(Number.isNaN(entry.formSize) ? this.mFormSize : entry.formSize);
      if (formSize > maxFormSize) {
        maxFormSize = formSize;
      }
      var label: string = entry.label;
      if (label == null) {
        continue;
      }
      var length: number = (label.length * this.getTextSize()) / 2;
      if (length > max) {
        max = length;
      }
    }
    return max + maxFormSize + formToTextSpace;
  }
  public calculateNeededSizes(viewPortHandler: ViewPortHandler): void {
    var defaultFormSize: number = Utils.convertDpToPixel(this.mFormSize);
    var stackSpace: number = this.mStackSpace;
    var formToTextSpace: number = this.mFormToTextSpace;
    var xEntrySpace: number = this.mXEntrySpace;
    var yEntrySpace: number = this.mYEntrySpace;
    var wordWrapEnabled: boolean = this.mWordWrapEnabled;
    var entries: LegendEntry[] = this.mEntries;
    var entryCount: number = entries.length;
    console.log(
      'calculateNeededSizes: mFormSize(' +
      this.mFormSize +
      ')+mStackSpace(' +
      this.mStackSpace +
      ')+mFormToTextSpace(' +
      this.mFormToTextSpace +
      ')+mXEntrySpace(' +
      this.mXEntrySpace +
      ')+mWordWrapEnabled(' +
      this.mWordWrapEnabled +
      ')+xoffset(' +
      this.mXOffset +
      ')+yOffset(' +
      this.mYOffset +
      ')'
    );
    this.mTextWidthMax = this.getMaxEntryWidth();
    this.mTextHeightMax = this.getMaxEntryHeight();
    switch (this.mOrientation) {
      case LegendOrientation.VERTICAL: {
        var maxWidth: number = 0;
        var maxHeight: number = 0;
        var width: number = 0;
        var labelLineHeight: number = this.getTextSize();
        var wasStacked: boolean = false;
        for (var i: number = 0; i < entryCount; i++) {
          var e: LegendEntry = entries[i];
          var drawingForm: boolean = e.form != LegendForm.NONE;
          var formSize: number = Number.isNaN(e.formSize) ? defaultFormSize : Utils.convertDpToPixel(e.formSize);
          var label: string = e.label;
          if (!wasStacked) {
            width = 0.0;
          }
          if (drawingForm) {
            if (wasStacked) {
              width += stackSpace;
            }
            width += formSize;
          }
          // grouped forms have null labels
          if (label != null) {
            // make a step to the left
            if (drawingForm && !wasStacked) {
              width += formToTextSpace;
            } else if (wasStacked) {
              maxWidth = Math.max(maxWidth, width);
              maxHeight += labelLineHeight + yEntrySpace;
              width = 0.1;
              wasStacked = false;
            }
            width += (label.length * this.getTextSize()) / 2;
            maxHeight += labelLineHeight + yEntrySpace;
          } else {
            wasStacked = true;
            width += formSize;
            if (i < entryCount - 1) {
              width += stackSpace;
            }
          }
          maxWidth = Math.max(maxWidth, width);
        }
        this.mNeededWidth = maxWidth;
        this.mNeededHeight = maxHeight;
        break;
      }
      case LegendOrientation.HORIZONTAL: {
        var labelLineHeight: number = this.getTextSize();
        var defaultLineSpace: number = 1.2;
        var labelLineSpacing: number = defaultLineSpace + yEntrySpace;
        var contentWidth: number = viewPortHandler.contentWidth() * this.mMaxSizePercent;
        // Start calculating layout
        var maxLineWidth: number = 0.0;
        var currentLineWidth: number = 0.0;
        var requiredWidth: number = 0.0;
        var stackedStartIndex: number = -1;
        this.mCalculatedLabelBreakPoints.clear();
        this.mCalculatedLabelSizes.clear();
        this.mCalculatedLineSizes.clear();
        for (var i: number = 0; i < entryCount; i++) {
          var e: LegendEntry = entries[i];
          var drawingForm: boolean = e.form != LegendForm.NONE;
          var formSize: number = Number.isNaN(e.formSize) ? defaultFormSize : Utils.convertDpToPixel(e.formSize);
          var label: string = e.label;
          this.mCalculatedLabelBreakPoints.add(false);
          if (stackedStartIndex == -1) {
            // we are not stacking, so required width is for this label
            // only
            requiredWidth = 0.0;
          } else {
            // add the spacing appropriate for stacked labels/forms
            requiredWidth += stackSpace;
          }
          console.log('1.0  i(' + i + ')requiredWidth(' + requiredWidth + ') ?= stackSpace(' + stackSpace + ')');
          // grouped forms have null labels
          if (label != null) {
            this.mCalculatedLabelSizes.add(new FSize(this.getTextSize() * label.length, this.getTextSize()));
            console.log(
              '1.1  i(' +
              i +
              ')FSize(this.getTextSize()*label.length(' +
              this.getTextSize() * label.length +
              '), getTextSize(' +
              this.getTextSize() +
              ')'
            );
            requiredWidth += drawingForm ? formToTextSpace + formSize : 0.0;
            console.log(
              '1.2  i(' +
              i +
              ')requiredWidth(' +
              requiredWidth +
              ') += formToTextSpace(' +
              formToTextSpace +
              ') + formSize(' +
              formSize +
              ')'
            );
            requiredWidth += this.mCalculatedLabelSizes.get(i).width;
            console.log(
              '1.3  i(' +
              i +
              ')requiredWidth(' +
              requiredWidth +
              ') += this.mCalculatedLabelSizes.get(i).width(' +
              this.mCalculatedLabelSizes.get(i).width +
              ')'
            );
          } else {
            this.mCalculatedLabelSizes.add(FSize.getInstance(0.0, 0.0));
            requiredWidth += drawingForm ? formSize : 0.0;
            if (stackedStartIndex == -1) {
              // mark this index as we might want to break here later
              stackedStartIndex = i;
            }
          }
          if (label != null || i == entryCount - 1) {
            var requiredSpacing: number = currentLineWidth == 0.0 ? 0.0 : xEntrySpace;
            console.log(
              '2.0  i(' +
              i +
              ')requiredSpacing(' +
              requiredSpacing +
              ')= xEntrySpace(' +
              xEntrySpace +
              ') ? currentLineWidth(' +
              currentLineWidth +
              ')'
            );
            console.log(
              '3.0  i(' + i + ')contentWidth(' + contentWidth + '); currentLineWidth(' + currentLineWidth + ')'
            );
            if (
            !wordWrapEnabled || // No word wrapping, it must fit.
            // The line is empty, it must fit
            currentLineWidth == 0.0 ||
            // It simply fits
            contentWidth - currentLineWidth >= requiredSpacing + requiredWidth
            ) {
              // Expand current line
              currentLineWidth += requiredSpacing + requiredWidth;
              console.log(
                '4.0  i(' +
                i +
                ')line <==> currentLineWidth(' +
                currentLineWidth +
                ')= requiredSpacing(' +
                requiredSpacing +
                ') + requiredWidth(' +
                requiredWidth +
                ')'
              );
            } else {
              // It doesn't fit, we need to wrap a line
              // Add current line size to array
              this.mCalculatedLineSizes.add(FSize.getInstance(currentLineWidth, labelLineHeight));
              console.log(
                '5.0  i(' +
                i +
                ')new line <==>  FSize(currentLineWidth(' +
                currentLineWidth +
                '), labelLineHeight(' +
                labelLineHeight +
                ')'
              );
              maxLineWidth = Math.max(maxLineWidth, currentLineWidth);
              console.log('5.1  i(' + i + ')new line <==>  maxLineWidth(' + maxLineWidth + ')');
              // Start a new line
              if (stackedStartIndex > -1) {
                this.mCalculatedLabelBreakPoints.append(true);
              }
              // Start a new line
              this.mCalculatedLabelBreakPoints.append(true);
              currentLineWidth = requiredWidth;
              console.log(
                '5.2  i(' +
                i +
                ')new line <==> currentLineWidth(' +
                currentLineWidth +
                ')= requiredWidth(' +
                requiredWidth +
                ')'
              );
            }
            if (i == entryCount - 1) {
              //���һ��legend
              // Add last line size to array
              this.mCalculatedLineSizes.add(FSize.getInstance(currentLineWidth, labelLineHeight));
              console.log(
                '6.0  i(' +
                i +
                ')FSize(currentLineWidth(' +
                currentLineWidth +
                '), labelLineHeight(' +
                labelLineHeight +
                ')'
              );
              maxLineWidth = Math.max(maxLineWidth, currentLineWidth);
              console.log(
                '6.1  i(' +
                i +
                ')maxLineWidth=Max(maxLineWidth' +
                maxLineWidth +
                '), currentLineWidth(' +
                currentLineWidth +
                '))'
              );
            }
          }
          stackedStartIndex = label != null ? -1 : stackedStartIndex;
        }
        this.mNeededWidth = maxLineWidth;
        this.mNeededHeight =
        labelLineHeight * this.mCalculatedLineSizes.size() +
        labelLineSpacing * (this.mCalculatedLineSizes.size() == 0 ? 0 : this.mCalculatedLineSizes.size() - 1);
        console.log(
          '7.0 mNeededHeight(' +
          this.mNeededHeight +
          ')= labelLineHeight(' +
          labelLineHeight +
          ')*mCalculatedLineSizes.size(' +
          this.mCalculatedLineSizes.size() +
          ')+labelLineSpacing(' +
          labelLineSpacing +
          ')'
        );
        break;
      }
    }
    this.mNeededHeight += this.mYOffset;
    this.mNeededWidth += this.mXOffset;
    console.log('0.0 this.mNeededHeight=' + JSON.stringify(this.mNeededHeight));
    console.log('0.1 this.mNeededWidth=' + JSON.stringify(this.mNeededWidth));
  }
}

export enum LegendForm {
  /**
   * Avoid drawing a form
   */
  NONE,

  /**
   * Do not draw the a form, but leave space for it
   */
  EMPTY,

  /**
   * Use default (default dataset's form to the legend's form)
   */
  DEFAULT,

  /**
   * Draw a square
   */
  SQUARE,

  /**
   * Draw a circle
   */
  CIRCLE,

  /**
   * Draw a horizontal line
   */
  LINE,
}
