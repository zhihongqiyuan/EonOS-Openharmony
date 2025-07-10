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

import { Style, Align, FontMetrics } from '../data/Paint';
import Legend from '../components/Legend';
import {
    LegendForm,
    LegendHorizontalAlignment,
    LegendVerticalAlignment,
    LegendOrientation,
    LegendDirection,
} from '../components/Legend';
import LegendEntry from '../components/LegendEntry';
import ChartData from '../data/ChartData';
import IBarDataSet from '../interfaces/datasets/IBarDataSet';
import IDataSet from '../interfaces/datasets/IDataSet';
import EntryOhos from '../data/EntryOhos';
import ColorTemplate from '../utils/ColorTemplate';
import FSize from '../utils/FSize';
import Utils from '../utils/Utils';
import ViewPortHandler from '../utils/ViewPortHandler';
import Renderer from './Renderer';
import { JArrayList } from '../utils/JArrayList';
import Paint, { LinePaint, TextPaint, PathPaint, CirclePaint, RectPaint } from '../data/Paint';
class Point {
    private x1: number = 0;
    private y1: number = 0;
    private x2: number = 0;
    private y2: number = 0;
    constructor(x1: number, y1: number, x2: number, y2: number) {
        this.x1 = x1;
        this.y1 = y1;
        this.x2 = x2;
        this.y2 = y2;
    }
}

export default class LegendRenderer extends Renderer {
    /**
     * paint for the legend labels
     */
    protected mLegendLabelPaint: Paint;

    /**
     * paint used for the legend forms
     */
    protected mLegendFormPaint: Paint;

    /**
     * the legend object this renderer renders
     */
    protected mLegend: Legend;

    constructor(viewPortHandler: ViewPortHandler, legend: Legend) {
        super(viewPortHandler);

        this.mLegend = legend;

        this.mLegendLabelPaint = new Paint();
        this.mLegendLabelPaint.setTextSize(9);
        this.mLegendLabelPaint.setAlign(Align.LEFT);

        this.mLegendFormPaint = new Paint();
        this.mLegendFormPaint.setStyle(Style.FILL);
    }

    /**
     * Returns the Paint object used for drawing the Legend labels.
     *
     * @return
     */
    public getLabelPaint(): Paint {
        return this.mLegendLabelPaint;
    }

    /**
     * Returns the Paint object used for drawing the Legend forms.
     *
     * @return
     */
    public getFormPaint(): Paint {
        return this.mLegendFormPaint;
    }

    protected computedEntries: JArrayList<LegendEntry> = new JArrayList<LegendEntry>();

    /**
     * Prepares the legend and calculates all needed forms, labels and colors.
     *
     * @param data
     */
    public computeLegend(data?: ChartData<IDataSet<EntryOhos>>): void {
        if (!this.mLegend.isLegendCustom() && data) {
            this.computedEntries.clear();

            // loop for building up the colors and labels used in the legend
            for (let i: number = 0; i < data.getDataSetCount(); i++) {
                let dataSet: IDataSet<EntryOhos> = data.getDataSetByIndex(i);
                if (dataSet == null) {
                    continue;
                }

                let clrs: JArrayList<Number> = dataSet.getColors();
                let entryCount: number = dataSet.getEntryCount();

                // if we have a barchart with stacked bars
                if (dataSet.constructor.name == 'IBarDataSet<EntryOhos>' && (<IBarDataSet> dataSet).isStacked()) {
                    let bds: IBarDataSet = <IBarDataSet> dataSet;
                    let sLabels: string[] = bds.getStackLabels();

                    let minEntries: number = Math.min(clrs.size(), bds.getStackSize());

                    for (let j: number = 0; j < minEntries; j++) {
                        let label: string;
                        if (sLabels.length > 0) {
                            let labelIndex: number = j % minEntries;
                            label = labelIndex < sLabels.length ? sLabels[labelIndex] : null;
                        } else {
                            label = null;
                        }

                        this.computedEntries.add(
                            new LegendEntry(
                                label,
                                dataSet.getForm(),
                                dataSet.getFormSize(),
                                dataSet.getFormLineWidth(),
                                dataSet.getFormLineDashEffect(),
                                clrs.get(j).valueOf()
                            )
                        );
                    }

                    if (bds.getLabel() != null) {
                        // add the legend description label
                        this.computedEntries.add(
                            new LegendEntry(
                                dataSet.getLabel(),
                                LegendForm.NONE,
                                Number.NaN,
                                Number.NaN,
                                null,
                                ColorTemplate.COLOR_NONE
                            )
                        );
                    }
                } else {
                    // all others

                    for (let j: number = 0; j < clrs.size() && j < entryCount; j++) {
                        let label: string;

                        // if multiple colors are set for a DataSet, group them
                        if (j < clrs.size() - 1 && j < entryCount - 1) {
                            label = null;
                        } else {
                            // add label to the last entry
                            label = data.getDataSetByIndex(i).getLabel();
                        }

                        this.computedEntries.add(
                            new LegendEntry(
                                label,
                                dataSet.getForm(),
                                dataSet.getFormSize(),
                                dataSet.getFormLineWidth(),
                                dataSet.getFormLineDashEffect(),
                                clrs.get(j).valueOf()
                            )
                        );
                    }
                }
            }

            if (this.mLegend.getExtraEntries() != null) {
                let datas: LegendEntry[] = this.mLegend.getExtraEntries();
                for (let i: number = 0; i < datas.length; i++) {
                    this.computedEntries.add(datas[i]);
                }
            }

            this.mLegend.setEntries(this.computedEntries);
        }

        let tf: FontWeight /*Typeface*/ = this.mLegend.getTypeface();

        if (tf != null) {
            this.mLegendLabelPaint.setTypeface(tf);
        }

        this.mLegendLabelPaint.setTextSize(this.mLegend.getTextSize());
        this.mLegendLabelPaint.setColor(this.mLegend.getTextColor());

        // calculate all dimensions of the mLegend
        this.mLegend.calculateDimensions(this.mLegendLabelPaint, this.mViewPortHandler);
    }

    protected legendFontMetrics: FontMetrics = new FontMetrics();

    // public void renderLegend(Canvas c) {
    public renderLegend(): Paint[] {
        let datas: Point[] = [];
        if (!this.mLegend.isEnabled()) {
            return;
        }
        let tf: FontWeight = this.mLegend.getTypeface();
        if (tf != null) {
            this.mLegendLabelPaint.setTypeface(tf);
        }
        this.mLegendLabelPaint.setTextSize(this.mLegend.getTextSize());
        this.mLegendLabelPaint.setColor(this.mLegend.getTextColor());
        let labelLineHeight: number = Utils.getLineHeight(this.mLegendLabelPaint);
        let labelLineSpacing: number = Utils.getLineSpacing(this.mLegendLabelPaint) + this.mLegend.getYEntrySpace();
        let formYOffset: number = labelLineHeight - Utils.calcTextHeight(this.mLegendLabelPaint, 'ABC') / 2.0;
        let entries: LegendEntry[] = this.mLegend.getEntries();
        let formToTextSpace: number = this.mLegend.getFormToTextSpace();
        let xEntrySpace: number = this.mLegend.getXEntrySpace();
        let orientation: LegendOrientation = this.mLegend.getOrientation(); // "HORIZONTAL" 
        let horizontalAlignment: LegendHorizontalAlignment = this.mLegend.getHorizontalAlignment(); // "LEFT" 
        let verticalAlignment: LegendVerticalAlignment = this.mLegend.getVerticalAlignment(); // "BOTTOM
        let direction: LegendDirection = this.mLegend.getDirection(); // "LEFT_TO_RIGHT" 
        let defaultFormSize: number = this.mLegend.getFormSize();
        let stackSpace: number = this.mLegend.getStackSpace();
        let yoffset: number = this.mLegend.getYOffset();
        let xoffset: number = this.mLegend.getXOffset();
        let originPosX: number = 0.0;
        let paints: Paint[] = [];
        switch (horizontalAlignment) {
          case LegendHorizontalAlignment.LEFT: {
              if (orientation === LegendOrientation.VERTICAL) {
                  originPosX = xoffset;
              } else {
                  originPosX = this.mViewPortHandler.contentLeft() + xoffset;
              }
              console.log(
                  '1. originPosX(' +
                  JSON.stringify(originPosX)+
                      ')= xoffset(' +
                      xoffset +
                      ') + mViewPortHandler.contentLeft(' +
                  this.mViewPortHandler.contentLeft()+
                      ')'
                );
              if (direction === LegendDirection.RIGHT_TO_LEFT) {
                  originPosX += this.mLegend.mNeededWidth;
              }
              break;
          }
          case LegendHorizontalAlignment.RIGHT: {
              if (orientation === LegendOrientation.VERTICAL) {
                  originPosX = this.mViewPortHandler.getChartWidth() - xoffset;
              } else {
                  originPosX = this.mViewPortHandler.contentRight() - xoffset;
              }
              if (direction == LegendDirection.LEFT_TO_RIGHT) {
                  originPosX -= this.mLegend.mNeededWidth;
              }
              break;
          }
          case LegendHorizontalAlignment.CENTER: {
              if (orientation == LegendOrientation.VERTICAL) {
                  originPosX = this.mViewPortHandler.getChartWidth() / 2.0;
              } else {
                  originPosX = this.mViewPortHandler.contentLeft() + this.mViewPortHandler.contentWidth() / 2.0;
              }
              originPosX += direction === LegendDirection.LEFT_TO_RIGHT ? +xoffset : -xoffset;
              if (orientation === LegendOrientation.VERTICAL) {
                  originPosX +=
                  direction === LegendDirection.LEFT_TO_RIGHT ?
                      -this.mLegend.mNeededWidth / 2.0 + xoffset :
                      this.mLegend.mNeededWidth / 2.0 - xoffset;
              }
              break;
            }
            default:
                break;
        }
        switch ( 
        orientation // "HORIZONTAL"
        ) {
          case LegendOrientation.HORIZONTAL: {
              let calculatedLineSizes: JArrayList<FSize> = this.mLegend.getCalculatedLineSizes();
              let calculatedLabelSizes: JArrayList<FSize> = this.mLegend.getCalculatedLabelSizes();
              let calculatedLabelBreakPoints: JArrayList<Boolean> = this.mLegend.getCalculatedLabelBreakPoints();
              let posX: number = originPosX;
              let posY: number = 0.0;
              switch (
              verticalAlignment // "BOTTOM"
              ) {
                case LegendVerticalAlignment.TOP: {
                    posY = yoffset;
                    console.log('2.0 posY(' + JSON.stringify(posY) + ')= yoffset(' + yoffset + ')');
                    break;
                }
                case LegendVerticalAlignment.BOTTOM: {
                    posY = this.mViewPortHandler.getChartHeight() - yoffset - this.mLegend.mNeededHeight;
                    console.log(
                        '2.1 posY(' +
                            posY +
                            ')= mViewPortHandler.getChartHeight(' +
                        this.mViewPortHandler.getChartHeight() +
                            ')-mNeededHeight(' +
                        this.mLegend.mNeededHeight +
                            ')-yoffset(' +
                            yoffset +
                            ')'
                    );
                    break;
                }
                case LegendVerticalAlignment.CENTER: {
                    posY = (this.mViewPortHandler.getChartHeight() - this.mLegend.mNeededHeight) / 2.0 + yoffset;
                    console.log(
                        '2.1 posY('+
                            posY +
                            ')= mViewPortHandler.getChartHeight(' +
                        this.mViewPortHandler.getChartHeight() +
                            ')-mNeededHeight/2(' +
                            this.mLegend.mNeededHeight / 2 +
                            ')+yoffset(' +
                            yoffset +
                            ')'
                    );
                    break;
                }
                default:
                    break;
              }
              let lineIndex : number = 0;
              for (let i : number = 0, count = entries.length; i < count; i++) {
                  let x1: number = 0;
                  let y1: number = 0;
                  let x2: number = 0;
                  let y2: number = 0;
                  let e : LegendEntry = entries[i];
                  let drawingForm : boolean = e.form != LegendForm.NONE;
                  let formSize : number = Number.isNaN(e.formSize) ? 
                      defaultFormSize : Utils.convertDpToPixel(e.formSize);
                  if (i < calculatedLabelBreakPoints.size() && calculatedLabelBreakPoints.get(i)) {
                      posX = originPosX;
                      posY += labelLineHeight + labelLineSpacing;
                      console.log(
                          '3.0 i(' +
                              i +
                              ') posX=' +
                              posX +
                              '; posY(' +
                              posY +
                              ')=labelLineHeight(' +
                              labelLineHeight +
                              ')+labelLineSpacing(' +
                              labelLineSpacing +
                              ')'
                      );
                  }
                  if (
                      posX == originPosX &&
                          horizontalAlignment == LegendHorizontalAlignment.CENTER && // "LEFT"
                          lineIndex < calculatedLineSizes.size()
                  ) {
                      posX +=
                      (direction == LegendDirection.RIGHT_TO_LEFT?
                      calculatedLineSizes.get(lineIndex).width :
                          -calculatedLineSizes.get(lineIndex).width) / 2.0;
                      lineIndex++;
                  }
                  let isStacked: boolean = e.label == null; // grouped forms have null labels
                  if (drawingForm) {
                      if (direction == LegendDirection.RIGHT_TO_LEFT) {
                          posX -= formSize;
                      }
                      x1 = posX
                      y1 = posY
                      console.log(
                           '3.1 i(' + i +') drawForm <==> posX=' + posX + '; posY(' + posY + ')+formYOffset(' + formYOffset + ')'
                      );
                      paints.push(this.drawForm(posX, posY, e, this.mLegend));
                      if (direction == LegendDirection.LEFT_TO_RIGHT) { 
                          console.log(
                              '3.2 i(' +
                                  i +
                                  ') posX(' +
                                  posX +
                                  ')+=Utils.convertDpToPixel(formSize)(' +
                              Utils.convertDpToPixel(formSize) +
                                  ')'
                          );
                          posX += Utils.convertDpToPixel(formSize);
                      }
                  }
                  if (!isStacked) {
                      if (drawingForm) {
                          console.log('3.3 i(' + i +') posX(' + posX + ')+=formToTextSpace(' + formToTextSpace + ')');
                          posX += direction == LegendDirection.RIGHT_TO_LEFT ? -formToTextSpace : formToTextSpace;
                      }
                      if (direction == LegendDirection.RIGHT_TO_LEFT) {
                          posX -= calculatedLabelSizes.get(i).width;
                      }
                      x2 = posX
                      y2 = posY
                      console.log(
                          '3.4 i(' + i + ') drawLabel <==> posX=' + posX + '; posY(' + posY + ')+formYOffset(' + formYOffset + ')'
                      );
                      paints.push(this.drawLabel(posX, posY, e.label));
                      if (direction == LegendDirection.LEFT_TO_RIGHT) {
                          console.log(
                              '3.5 i(' +
                                  i +
                                  ') posX(' +
                                  posX +
                                  ')+=calcTextWidth(' +
                              JSON.stringify(Utils.calcTextWidth(this.mLegendLabelPaint, e.label)) +
                              ')'
                          );
                          posX += Utils.calcTextWidth(this.mLegendLabelPaint, e.label);
                      }
                      console.log(
                          '3.6 i(' + i + ') posX(' + posX + ')+=xEntrySpace(' + xEntrySpace + '); stackSpace(' + stackSpace + ')'
                      );
                      posX += direction == LegendDirection.RIGHT_TO_LEFT ? -xEntrySpace : xEntrySpace;
                  } else posX += direction == LegendDirection.RIGHT_TO_LEFT ? -stackSpace : stackSpace;
                  datas.push(new Point(x1, y1, x2, y2))
              }
              break;
          }
          case LegendOrientation.VERTICAL: {
              let stack : number = 0;
              let wasStacked : boolean = false;
              let posY : number = 0.1;
              switch (verticalAlignment) {
                case LegendVerticalAlignment.TOP: {
                    posY = (horizontalAlignment == LegendHorizontalAlignment.CENTER
                        ? 0.1 : this.mViewPortHandler.contentTop());
                    posY += yoffset;
                    break;
                }
                case LegendVerticalAlignment.BOTTOM: {
                    posY = 
                        horizontalAlignment == LegendHorizontalAlignment.CENTER
                            ? this.mViewPortHandler.getChartHeight()
                            : this.mViewPortHandler.contentBottom();
                    posY -= this.mLegend.mNeededHeight + yoffset;
                    break;
                }
                case LegendVerticalAlignment.CENTER: {
                    posY = 
                        this.mViewPortHandler.getChartHeight() / 2.0 -
                            this.mLegend.mNeededHeight / 2.0 +
                        this.mLegend.getYOffset();
                    break;
                }
                default:
                    break;
              }
              for (let i: number = 0; i < entries.length; i++) {
                  let e: LegendEntry = entries[i];
                  let drawingForm : boolean = e.form != LegendForm.NONE;
                  let formSize : number = Number.isNaN(e.formSize) ?
                      defaultFormSize : Utils.convertDpToPixel(e.formSize);
                  let posX : number = originPosX;
                  if (drawingForm) {
                      if (direction == LegendDirection.LEFT_TO_RIGHT) {
                          posX += stack;
                      } else {
                          posX -= formSize - stack;
                      }
                      this.mLegendFormPaint = this.drawForm(posX, posY + formYOffset, e, this.mLegend);
                      if (direction == LegendDirection.LEFT_TO_RIGHT) {
                          posX += formSize;
                      }
                  }
                  if (e.label != null) {
                      if (drawingForm && !wasStacked) {
                          posX += direction == LegendDirection.LEFT_TO_RIGHT ? formToTextSpace : -formToTextSpace;
                      } else if (wasStacked) {
                          posX = originPosX;
                      }
                      if (direction == LegendDirection.RIGHT_TO_LEFT) {
                          posX -= Utils.calcTextWidth(this.mLegendLabelPaint[i], e.label);
                      }
                      if (!wasStacked) {
                          this.mLegendLabelPaint = this.drawLabel(/*c,*/ posX, posY + formYOffset, e.label);
                      } else {
                          posY += formYOffset + labelLineSpacing;
                          this.mLegendLabelPaint = this.drawLabel(/*c,*/ posX, posY + formYOffset, e.label);
                      }
                      posY += formYOffset + labelLineSpacing;
                      stack = 0;
                  } else {
                      stack += formSize + stackSpace;
                      wasStacked = true;
                  }
                  paints.push(this.mLegendFormPaint);
                  paints.push(this.mLegendLabelPaint);
              }
              break;
          }
          default:
              break;
        }
        console.log('6. LegendRenderer datas:' + JSON.stringify(datas));
        return paints;
    }

    /**
     * Draws the Legend-form at the given position with the color at the given
     * index.
     *
     * @param c      canvas to draw with
     * @param x      position
     * @param y      position
     * @param entry  the entry to render
     * @param legend the legend context
     */
    protected drawForm(x : number, y : number, entry : LegendEntry, legend : Legend): Paint {
        if (
            entry.formColor == ColorTemplate.COLOR_SKIP ||
                entry.formColor == ColorTemplate.COLOR_NONE ||
                entry.formColor == 0
        ) {
            return;
        }

        let form: LegendForm = entry.form;
        if (form == LegendForm.DEFAULT) {
            form = legend.getForm();
        }
        
        let formSize: number = Utils.convertDpToPixel(Number.isNaN(entry.formSize) ?
        legend.getFormSize() : entry.formSize);
        let half: number = formSize / 2;

        switch (form) {
          case LegendForm.NONE:
          // Do nothing
              break;
          case LegendForm.EMPTY:
          // Do not draw, but keep space for the form
              break;
          case LegendForm.DEFAULT:
          
          case LegendForm.CIRCLE: {
              let circlePaint: CirclePaint = new CirclePaint(); //= this.mLegendFormPaint as CirclePaint;
              circlePaint.setColor(entry.formColor);
              circlePaint.setStyle(Style.FILL);
              circlePaint.setX(x + half);
              circlePaint.setY(y);
              circlePaint.setWidth(half);
              circlePaint.setHeight(half);
              return circlePaint;
              break;
          }
          case LegendForm.SQUARE: {
              let rectPaint: RectPaint = new RectPaint();//= this.mLegendFormPaint as RectPaint;
              rectPaint.setStroke(entry.formColor)
              rectPaint.setStyle(Style.FILL)
              rectPaint.setStartPoint([x, y])
              rectPaint.setWidth(formSize)
              rectPaint.setHeight(half)
              return rectPaint
              break;
          }
          case LegendForm.LINE: {
              let linePaint: LinePaint //= this.mLegendFormPaint as LinePaint;
              let formLineWidth: number = Utils.convertDpToPixel(
                  Number.isNaN(entry.formLineWidth) ? legend.getFormLineWidth() : entry.formLineWidth
              );

              linePaint.setStyle(Style.STROKE);
              linePaint.setStrokeWidth(formLineWidth);

              linePaint.setStartPoint([x, y]);
              linePaint.setEndPoint([x + formSize, y]);
              return linePaint;
              break;
          }
          default:
              break;
        }
    }

    /**
     * Draws the provided label at the given position.
     *
     * @param c      to draw with
     * @param x
     * @param y
     * @param label the label to draw
     */
    protected drawLabel(x : number, y : number, label : string): Paint {
        let textPaint : TextPaint = new TextPaint(this.mLegendLabelPaint as TextPaint);
        textPaint.setText(label);
        textPaint.setX(x);
        textPaint.setY(y);
        return textPaint;
    }
}
