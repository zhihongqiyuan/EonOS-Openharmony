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

import Paint, {
  PathPaint,
  ImagePaint,
  Style,
  LinePaint,
  IndexPositionPaint,
  CirclePaint,
  TextPaint,
} from '../data/Paint';
import ChartAnimator from '../animation/ChartAnimator';
import RadarData from '../data/RadarData';
import RadarEntry from '../data/RadarEntry';
import Highlight from '../highlight/Highlight';
import IRadarDataSet from '../interfaces/datasets/IRadarDataSet';
import ColorTemplate from '../utils/ColorTemplate';
import MPPointF from '../utils/MPPointF';
import Utils from '../utils/Utils';
import LineRadarRenderer from './LineRadarRenderer';
import MyRect from '../data/Rect';
import RadarChartMode from '../data/RadarChartMode';
export default class RadarChartRenderer extends LineRadarRenderer {
  public TYPE_POINT: number = 1;
  public TYPE_PATH: number = 2;
  public radarChartMode: RadarChartMode = new RadarChartMode();
  /**
   * paint for drawing the web
   */
  public mWebPaint: Paint;
  protected mHighlightCirclePaint: Paint;

  constructor(radarChartMode: RadarChartMode) {
    super(radarChartMode.mAnimator, radarChartMode.handler);
    this.radarChartMode = radarChartMode;

    this.mHighlightPaint = new Paint();
    this.mHighlightPaint.setStrokeWidth(2);
    this.mHighlightPaint.setColor(0xffbb73);

    this.mWebPaint = new LinePaint();
    this.mWebPaint.setStyle(Style.STROKE);

    this.mHighlightCirclePaint = new Paint();
    if (this.mAnimator == null || this.mAnimator == undefined) {
      this.mAnimator = new ChartAnimator();
    }
  }

  public getWebPaint(): Paint {
    return this.mWebPaint;
  }

  public initBuffers(): void {
    // TODO Auto-generated method stub
  }

  public drawData(): Paint[] {
    return this.drawDataByType(this.TYPE_PATH);
  }
  public drawDataByType(byType: number): Paint[] {
    let paint: Paint[] = [];
    let radarData: RadarData = this.radarChartMode.getData();

    let mostEntries: number = radarData.getMaxEntryCountSet().getEntryCount();
    let i = 0;
    for (let set1 of radarData.getDataSets().dataSouce) {
      if (set1.isVisible()) {
        paint = paint.concat(this.drawDataSetByType(set1, mostEntries, byType, i));
      }
      i++;
    }
    return paint;
  }

  protected mDrawDataSetSurfacePathBuffer: string = '';
  /**
   * Draws the RadarDataSet
   *
   * @param c
   * @param dataSet
   * @param mostEntries the entry count of the dataset with the most entries
   */
  protected drawDataSet(dataSet: IRadarDataSet, mostEntries: number): Paint[] {
    return [];
  }
  protected drawDataSetByType(
    dataSet: IRadarDataSet,
    mostEntries: number,
    byType: number,
    parentIndex: number
  ): Paint[] {
    let phaseX: number = this.mAnimator.getPhaseX();
    let phaseY: number = this.mAnimator.getPhaseY();

    let sliceangle: number = this.radarChartMode.getSliceAngle();

    // calculate the factor that is needed for transforming the value to
    // pixels
    let factor: number = this.radarChartMode.getFactor();

    let center: MPPointF = this.radarChartMode.getCenterOffsets();
    let pOut: MPPointF = MPPointF.getInstance(0, 0);
    let surface: string = this.mDrawDataSetSurfacePathBuffer;
    surface = '';
    let pointTypeArr: Paint[] = [];

    let hasMovedToPoint: boolean = false;

    for (let j = 0; j < dataSet.getEntryCount(); j++) {
      let pointType: IndexPositionPaint = new IndexPositionPaint();
      pointType.setDataSetIndex(parentIndex);
      pointType.setDataIndex(j);
      this.mRenderPaint.setColor(dataSet.getColor(j));

      let e: RadarEntry = dataSet.getEntryForIndex(j);

      Utils.getPosition(
        center,
        (e.getY() - this.radarChartMode.getYChartMin()) * factor * phaseY,
        sliceangle * j * phaseX + this.radarChartMode.getRotationAngle(),
        pOut
      );
      //
      if (Number.isNaN(pOut.x)) {
        continue;
      }

      if (!hasMovedToPoint) {
        surface = 'M' + Utils.convertDpToPixel(pOut.x) + ' ' + Utils.convertDpToPixel(pOut.y) + ' ';
        hasMovedToPoint = true;
      } else {
        surface += 'L' + Utils.convertDpToPixel(pOut.x) + ' ' + Utils.convertDpToPixel(pOut.y) + ' ';
      }
      pointType.x = pOut.x;
      pointType.y = pOut.y;
      pointTypeArr.push(pointType);
    }
    //
    if (dataSet.getEntryCount() > mostEntries) {
      // if this is not the largest set, draw a line to the center before closing
      surface += 'L' + Utils.convertDpToPixel(center.x) + ' ' + Utils.convertDpToPixel(center.y) + ' ';
      let pointType: IndexPositionPaint = new IndexPositionPaint();
      pointType.x = center.x;
      pointType.y = center.y;
      pointType.setDataSetIndex(-1);
      pointType.setDataIndex(-1);
      pointTypeArr.push(pointType);
    }
    this.mRenderPaint.setStrokeWidth(dataSet.getLineWidth());
    this.mRenderPaint.setStyle(Style.STROKE);
    surface += ' Z';
    let pathPaint: PathPaint = new PathPaint();
    pathPaint.set(this.mRenderPaint);
    pathPaint.setCommands(surface);
    if (dataSet.isDrawFilledEnabled()) {
      pathPaint.setFill(dataSet.getFillColor());
    } else {
      pathPaint.setFill(0x01ffffff);
    }
    return byType == this.TYPE_PATH ? [pathPaint] : pointTypeArr;
  }

  public drawValues(): Paint[] {
    let paints: Paint[] = [];

    let phaseX: number = this.mAnimator.getPhaseX();
    let phaseY: number = this.mAnimator.getPhaseY();

    let sliceangle: number = this.radarChartMode.getSliceAngle();

    // calculate the factor that is needed for transforming the value to
    // pixels
    let factor: number = this.radarChartMode.getFactor();

    let center: MPPointF = this.radarChartMode.getCenterOffsets();
    let pOut: MPPointF = MPPointF.getInstance(0, 0);
    let pIcon: MPPointF = MPPointF.getInstance(0, 0);

    let yoffset: number = Utils.convertDpToPixel(5);
    let extraWidth = Utils.calcTextWidth(this.mValuePaint, this.radarChartMode.getYChartMax() + '');
    for (let i = 0; i < this.radarChartMode.getData().getDataSetCount(); i++) {
      let dataSet: IRadarDataSet = this.radarChartMode.getData().getDataSetByIndex(i);

      // apply the text-styling defined by the DataSet
      this.applyValueTextStyle(dataSet);
      let iconsOffset: MPPointF = MPPointF.getInstance(null, null, dataSet.getIconsOffset());

      iconsOffset.x = Utils.convertDpToPixel(iconsOffset.x);
      iconsOffset.y = Utils.convertDpToPixel(iconsOffset.y);

      for (let j = 0; j < dataSet.getEntryCount(); j++) {
        let entry: RadarEntry = dataSet.getEntryForIndex(j);
        Utils.getPosition(
          center,
          (entry.getY() - this.radarChartMode.getYChartMin()) * factor * phaseY,
          sliceangle * j * phaseX + this.radarChartMode.getRotationAngle(),
          pOut
        );

        if (this.radarChartMode.getData().getDataSets().get(i).isDrawValuesEnabled()) {
          paints = paints.concat(
          this.drawValue(
          dataSet.getValueFormatter(),
          entry.getY(),
            entry,
            i,
            pOut.x - extraWidth / 2,
            pOut.y - yoffset,
          dataSet.getValueTextColor(j)
          )
          );
        } else {
          paints = paints.concat(new TextPaint());
        }
      }

      MPPointF.recycleInstance(iconsOffset);
    }

    MPPointF.recycleInstance(center);
    MPPointF.recycleInstance(pOut);
    MPPointF.recycleInstance(pIcon);
    return paints;
  }

  public drawExtras(): Paint[] {
    return this.drawWeb();
  }

  protected drawWeb(): Paint[] {
    let sliceangle: number = this.radarChartMode.getSliceAngle();

    // calculate the factor that is needed for transforming the value to
    // pixels
    let factor: number = this.radarChartMode.getFactor();
    let rotationangle = this.radarChartMode.getRotationAngle();

    let center: MPPointF = this.radarChartMode.getCenterOffsets();

    // draw the web lines that come from the center
    this.mWebPaint.setStrokeWidth(this.radarChartMode.getWebLineWidth());
    this.mWebPaint.setColor(this.radarChartMode.getWebColor());
    this.mWebPaint.setAlpha(this.radarChartMode.getWebAlpha());

    let linePaintArr: LinePaint[] = [];

    const xIncrements: number = 1 + this.radarChartMode.getSkipWebLineCount();
    let maxEntryCount: number = this.radarChartMode.getData().getMaxEntryCountSet().getEntryCount();

    let p: MPPointF = MPPointF.getInstance(0, 0);
    for (let i = 0; i < maxEntryCount; i += xIncrements) {
      Utils.getPosition(center, this.radarChartMode.getYRange() * factor, sliceangle * i + rotationangle, p);
      let linePaint: LinePaint = new LinePaint(this.mWebPaint as LinePaint);
      linePaint.setStartPoint([center.x, center.y]);
      linePaint.setEndPoint([p.x, p.y]);
      linePaintArr.push(linePaint);
    }
    MPPointF.recycleInstance(p);

    // draw the inner-web
    this.mWebPaint.setStrokeWidth(this.radarChartMode.getWebLineWidthInner());
    this.mWebPaint.setColor(this.radarChartMode.getWebColorInner());
    this.mWebPaint.setAlpha(this.radarChartMode.getWebAlpha());

    let labelCount = this.radarChartMode.getYAxis().mEntryCount;

    let p1out: MPPointF = MPPointF.getInstance(0, 0);
    let p2out: MPPointF = MPPointF.getInstance(0, 0);
    for (let j = 0; j < labelCount; j++) {
      for (let i = 0; i < this.radarChartMode.getData().getEntryCount(); i++) {
        let r: number = (this.radarChartMode.getYAxis().mEntries[j] - this.radarChartMode.getYChartMin()) * factor;

        Utils.getPosition(center, r, sliceangle * i + rotationangle, p1out);
        Utils.getPosition(center, r, sliceangle * (i + 1) + rotationangle, p2out);

        let linePaint: LinePaint = new LinePaint(this.mWebPaint as LinePaint);
        linePaint.setStartPoint([p1out.x, p1out.y]);
        linePaint.setEndPoint([p2out.x, p2out.y]);
        linePaintArr.push(linePaint);
      }
    }
    MPPointF.recycleInstance(p1out);
    MPPointF.recycleInstance(p2out);
    return linePaintArr;
  }

  public drawHighlighted(indices: Highlight[]): Paint[] {
    let paintData: Paint[] = [];

    let sliceangle: number = this.radarChartMode.getSliceAngle();

    // calculate the factor that is needed for transforming the value to
    // pixels
    let factor: number = this.radarChartMode.getFactor();

    let center: MPPointF = this.radarChartMode.getCenterOffsets();
    let pOut: MPPointF = MPPointF.getInstance(0, 0);

    let radarData: RadarData = this.radarChartMode.getData();

    for (let high of indices) {
      let dataSet: IRadarDataSet = radarData.getDataSetByIndex(high.getDataSetIndex());

      if (dataSet == null || !dataSet.isHighlightEnabled()) {
        continue;
      }

      let e: RadarEntry = dataSet.getEntryForIndex(high.getDataIndex());
      if (!this.isInBoundsX(e, dataSet)) {
        continue;
      }

      let y: number = e.getY() - this.radarChartMode.getYChartMin();

      Utils.getPosition(
        center,
        y * factor * this.mAnimator.getPhaseY(),
        sliceangle * high.getDataIndex() * this.mAnimator.getPhaseX() + this.radarChartMode.getRotationAngle(),
        pOut
      );

      let content: string = Math.round(Number(e.getY().toFixed(1))) + '%';
      let textPaint: TextPaint = new TextPaint();
      let textWidth = Utils.calcTextWidth(textPaint, content) * 2 + 10;

      textPaint.setTextSize(10);
      textPaint.setText(content);
      textPaint.setColor(Color.White);
      textPaint.setWidth(textWidth + 10);
      textPaint.setHeight(10);
      textPaint.setX(pOut.x - textWidth / 2 - 5);
      textPaint.setY(pOut.y - textWidth + 2);
      textPaint.setTextAlign(TextAlign.Center);
      paintData.push(textPaint);

      let imagePaint: ImagePaint = new ImagePaint();
      imagePaint.setX(pOut.x - textWidth / 2 - 5);
      imagePaint.setY(pOut.y - textWidth - 5);
      imagePaint.setWidth(textWidth + 10);
      imagePaint.setHeight(textWidth);
      paintData.push(imagePaint);

      if (dataSet.isDrawHighlightCircleEnabled()) {
        if (!Number.isNaN(pOut.x) && !Number.isNaN(pOut.y)) {
          let strokeColor: number = dataSet.getHighlightCircleStrokeColor();

          paintData = paintData.concat(
          this.drawHighlightCircle(
            pOut,
          px2vp(dataSet.getHighlightCircleInnerRadius()),
          px2vp(dataSet.getHighlightCircleOuterRadius()),
          dataSet.getHighlightCircleFillColor(),
            strokeColor,
          px2vp(dataSet.getHighlightCircleStrokeWidth())
          )
          );
        }
      }
    }

    MPPointF.recycleInstance(center);
    MPPointF.recycleInstance(pOut);
    return paintData;
  }

  protected mDrawHighlightCirclePathBuffer: string = '';
  public drawHighlightCircle(
    point: MPPointF,
    innerRadius: number,
    outerRadius: number,
    fillColor: number,
    strokeColor: number,
    strokeWidth: number
  ): Paint[] {
    outerRadius = Utils.convertDpToPixel(outerRadius);
    innerRadius = Utils.convertDpToPixel(innerRadius);
    let paint: Paint[] = [];
    if (fillColor != ColorTemplate.COLOR_NONE) {
      let p: string = this.mDrawHighlightCirclePathBuffer;
      let outerPaintCircle: CirclePaint = new CirclePaint();
      outerPaintCircle.setX(point.x - outerRadius);
      outerPaintCircle.setY(point.y - outerRadius);
      outerPaintCircle.setWidth(outerRadius * 2);
      outerPaintCircle.setHeight(outerRadius * 2);
      if (innerRadius > 0) {
        let innerPaintCir: CirclePaint = new CirclePaint();
        innerPaintCir.setX(point.x - innerRadius);
        innerPaintCir.setY(point.y - innerRadius);
        innerPaintCir.setWidth(innerRadius * 2);
        innerPaintCir.setHeight(innerRadius * 2);
      }
      this.mHighlightCirclePaint.setColor(fillColor);
      this.mHighlightCirclePaint.setStyle(Style.FILL);
    }

    if (strokeColor != ColorTemplate.COLOR_NONE) {
      this.mHighlightCirclePaint.setFill(fillColor);
      this.mHighlightCirclePaint.setStroke(strokeColor);
      this.mHighlightCirclePaint.setStrokeWidth(strokeWidth);

      let strokePaintCir: CirclePaint = new CirclePaint(this.mHighlightCirclePaint);
      strokePaintCir.setX(point.x - outerRadius);
      strokePaintCir.setY(point.y - outerRadius);
      strokePaintCir.setWidth(outerRadius * 2);
      strokePaintCir.setHeight(outerRadius * 2);
      paint.push(strokePaintCir);
    }

    return paint;
  }
}
