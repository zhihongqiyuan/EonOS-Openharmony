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
import { JArrayList } from '../utils/JArrayList';
import { AxisDependency } from '../components/YAxis';
import { FillStyle } from './LineDataSet';
/**
 * 画笔属性类，用于绘制时使用画笔属性
 * @param value
 */
export default class Paint {
  public color: Color | number | string | Resource = Color.Black;
  public textSize: number = 10;
  public fill: Color | number | string | Resource = null;
  public stroke: Color | number | string | Resource = null;
  public strokeWidth: number = 0;
  public strokeRadius: number = 0;
  public style: Style = null;
  public alpha: number = 1;
  public typeface: FontWeight = FontWeight.Normal;
  public textAlign: TextAlign = TextAlign.Start;
  public align: Align = Align.LEFT;
  public strokeDashArray: number[] = [0, 0];
  public strokeDashOffset: number = 0;
  public dashPathEffect: DashPathEffect;
  public x: number = 0;
  public y: number = 0;
  public width: number | string = null;
  public height: number | string = null;
  public visibility: Visibility = Visibility.Visible;
  public clickPosition = -1;
  public value = 0;
  public translateX = 0;
  public translateY = 0;

  constructor(paint?: Paint) {
    if (paint != null && paint != undefined) {
      this.color = paint.color;
      this.textSize = paint.textSize;
      this.fill = paint.fill;
      this.stroke = paint.stroke;
      this.strokeWidth = paint.strokeWidth;
      this.strokeRadius = paint.strokeRadius;
      this.style = paint.style;
      this.alpha = paint.alpha;
      this.typeface = paint.typeface;
      this.textAlign = paint.textAlign;
      this.strokeDashArray = paint.strokeDashArray;
      this.strokeDashOffset = paint.strokeDashOffset;
      this.dashPathEffect = paint.dashPathEffect;
      this.x = paint.x;
      this.y = paint.y;
      this.width = paint.width;
      this.height = paint.height;
      this.visibility = paint.visibility;
      this.clickPosition = paint.clickPosition;
      this.value = paint.value;
    }
  }

  public set(paint: Paint) {
    this.color = paint.color;
    this.textSize = paint.textSize;
    this.fill = paint.fill;
    this.stroke = paint.stroke;
    this.strokeWidth = paint.strokeWidth;
    this.strokeRadius = paint.strokeRadius;
    this.style = paint.style;
    this.alpha = paint.alpha;
    this.typeface = paint.typeface;
    this.textAlign = paint.textAlign;
    this.strokeDashArray = paint.strokeDashArray;
    this.strokeDashOffset = paint.strokeDashOffset;
    this.dashPathEffect = paint.dashPathEffect;
    this.x = paint.x;
    this.y = paint.y;
    this.width = paint.width;
    this.height = paint.height;
    this.visibility = paint.visibility;
    this.clickPosition = paint.clickPosition;
    this.value = paint.value;
  }

  setColor(value: Color | number | string | Resource) {
    this.color = value;
    switch (this.style) {
      case Style.STROKE:
        this.setStroke(value);
        break;
      case Style.FILL:
        this.setFill(value);
        break;
      case Style.FILL_AND_STROKE:
        this.setStroke(value);
        this.setFill(value);
        break;
    }
  }

  getColor(): Color | number | string | Resource {
    return this.color;
  }

  setTextSize(value: number) {
    this.textSize = value;
  }

  getTextSize(): number {
    return this.textSize;
  }

  setFill(value: Color | number | string | Resource) {
    this.fill = value;
  }

  setStroke(value: Color | number | string | Resource) {
    this.stroke = value;
  }

  setStrokeWidth(value: number) {
    this.strokeWidth = value;
  }

  getStrokeWidth(): number {
    return this.strokeWidth;
  }

  setStrokeRadius(value: number) {
    this.strokeRadius = value;
  }

  getStrokeRadius(): number {
    return this.strokeRadius;
  }

  setStyle(value: Style) {
    this.style = value;
    switch (value) {
      case Style.STROKE:
        this.setStroke(this.color);
        break;
      case Style.FILL:
        this.setFill(this.color);
        break;
      case Style.FILL_AND_STROKE:
        this.setStroke(this.color);
        this.setFill(this.color);
        break;
    }
  }

  getStyle(): Style {
    return this.style;
  }

  setAlpha(value: number) {
    this.alpha = value;
  }

  getAlpha(): number {
    return this.alpha;
  }

  setTypeface(value: FontWeight) {
    this.typeface = value;
  }

  getTypeface(): number {
    return this.typeface;
  }

  setTextAlign(value: TextAlign) {
    this.textAlign = value;
  }

  getTextAlign(): TextAlign {
    return this.textAlign;
  }

  setAlign(value: Align) {
    this.align = value;
  }

  getAlign(): Align {
    return this.align;
  }

  setStrokeDashArray(value: number[]) {
    this.strokeDashArray = value;
  }

  getStrokeDashArray(): number[] {
    return this.strokeDashArray;
  }

  setStrokeDashOffset(value: number) {
    this.strokeDashOffset = value;
  }

  getStrokeDashOffset(): number {
    return this.strokeDashOffset;
  }

  setDashPathEffect(value: DashPathEffect) {
    this.dashPathEffect = value;
    if (this.dashPathEffect != null) {
      this.setStrokeDashArray(this.dashPathEffect.dash);
      this.setStrokeDashOffset(this.dashPathEffect.offset);
    }
  }

  getDashPathEffect(): DashPathEffect {
    return this.dashPathEffect;
  }

  setX(value: number) {
    this.x = value;
  }

  getX(): number {
    return this.x;
  }

  setY(value: number) {
    this.y = value;
  }

  getY(): number {
    return this.y;
  }

  setWidth(value: number | string) {
    this.width = value;
  }

  getWidth(): number | string {
    return this.width;
  }

  setHeight(value: number | string) {
    this.height = value;
  }

  getHeight(): number | string {
    return this.height;
  }

  setVisibility(visibility: Visibility) {
    this.visibility = visibility;
  }

  setClickPosition(position: number) {
    this.clickPosition = position;
  }
}

/**
 * 用于绘制Line的属性类
 */
export class LinePaint extends Paint {
  public startPoint: number[] = [0, 0];
  public endPoint: number[] = [0, 0];

  constructor(paint?: LinePaint) {
    super(paint);
    if (paint != null && paint != undefined) {
      this.startPoint = paint.startPoint;
      this.endPoint = paint.endPoint;
    }
  }

  setStartPoint(value: number[]) {
    this.startPoint = value;
  }

  getStartPoint(): number[] {
    return this.startPoint;
  }

  setEndPoint(value: number[]) {
    this.endPoint = value;
  }

  getEndPoint(): number[] {
    return this.endPoint;
  }
}

export class TextPaint extends Paint {
  public text: string = '';
  constructor(paint?: TextPaint) {
    super(paint);
    if (paint != null && paint != undefined) {
      this.text = paint.text;
    }
  }
  public translateX: number = 0;
  public translateY: number = 0;
  public rotate: number = 0;
  setText(value: string) {
    this.text = value;
  }

  getText(): string {
    return this.text;
  }

  setTranslateX(value: number) {
    this.translateX = value;
  }

  getTranslateX(): number {
    return this.translateX;
  }
  setTranslateY(value: number) {
    this.translateY = value;
  }

  getTranslateY(): number {
    return this.translateY;
  }
  setRotate(value: number) {
    this.rotate = value;
  }

  getRotate(): number {
    return this.rotate;
  }
}

export class PathPaint extends Paint {
  public commands: string = '';
  public commandsFill: string = '';
  public filled: boolean = false;
  public linearGradientColors: Array<ColorStop> = null;
  public enabled: boolean = false;
  public circleColor: string | number | Color = Color.White;
  public colors: JArrayList<Number> = null;
  public radius: number = 2;
  public circleHoleRadius: number = 1;
  public circleHoleColor: string | number | Color = Color.White;
  public circleHoleEnabled: boolean = true;
  public drawValueEnable: boolean = true;
  public rotate: number;
  public rotateText: number;
  public percentage: string = '';
  public filledColor: number = 0;
  public label: string = '';
  public axisDependency: AxisDependency = AxisDependency.LEFT;
  public lineSvg: string = '';
  public fillStyle: FillStyle = FillStyle.MIN;
  public positionX: number;
  public positionY: number;
  public labelX: number;
  public labelY: number;
  public iconX: number = 0;
  public iconY: number = 0;

  constructor(paint?: PathPaint) {
    super(paint);
    if (paint != null && paint != undefined) {
      this.commands = paint.commands;
    }
  }

  setIconY(iconY: number) {
    this.iconY = iconY;
  }
  setIconX(iconX: number) {
    this.iconX = iconX;
  }
  setLabelX(labelX: number) {
    this.labelX = labelX;
  }
  setLabelY(labelY: number) {
    this.labelY = labelY;
  }
  setPositionX(positionX: number) {
    this.positionX = positionX;
  }
  setPositionY(positionY: number) {
    this.positionY = positionY;
  }

  setLineSvg(lineSvg: string) {
    this.lineSvg = lineSvg;
  }
  setLabel(label: string) {
    this.label = label;
  }

  getLabel(): string {
    return this.label;
  }
  setFilledColor(filledColor: number) {
    this.filledColor = filledColor;
  }
  getFilledColor(): number {
    return this.filledColor;
  }
  setRotateText(rotateText: number) {
    this.rotateText = rotateText;
  }

  getRotateText(): number {
    return this.rotateText;
  }

  setPercentage(percentage: string) {
    this.percentage = percentage;
  }

  getPercentage(): string {
    return this.percentage;
  }

  setRotate(value: number) {
    this.rotate = value;
  }

  setCommands(value: string) {
    this.commands = value;
  }

  setCommandsFill(commandsFill: string) {
    this.commandsFill = commandsFill;
  }

  public setFillStyle(fillStyle: FillStyle) {
    this.fillStyle = fillStyle;
  }

  setDrawFilled(filled: boolean) {
    this.filled = filled;
  }

  isDrawFilledEnabled(): boolean {
    return this.filled;
  }

  setGradientFillColor(linearGradientColors: Array<ColorStop>): void {
    this.linearGradientColors = linearGradientColors;
  }

  setColors(colors: JArrayList<Number>) {
    this.colors = colors;
  }

  setDrawCircles(enabled: boolean) {
    this.enabled = enabled;
  }

  isDrawCirclesEnabled(): boolean {
    return this.enabled;
  }

  setCirclesColor(circleColor: string | number | Color) {
    this.circleColor = circleColor;
  }

  setCircleRadius(radius: number) {
    this.radius = radius;
  }

  setCircleHoleRadius(circleHoleRadius: number) {
    this.circleHoleRadius = circleHoleRadius;
  }

  setCircleHoleColor(circleHoleColor: string | number | Color) {
    this.circleHoleColor = circleHoleColor;
  }

  setDrawCircleHole(circleHoleEnabled: boolean) {
    this.circleHoleEnabled = circleHoleEnabled;
  }

  setDrawValueEnable(drawValueEnable: boolean) {
    this.drawValueEnable = drawValueEnable;
  }

  setAxisDependency(axisDependency: AxisDependency) {
    this.axisDependency = axisDependency;
  }
}

export class PathFillPaint extends Paint {
  public commandsFill: string = '';
  public filled: boolean = false;
  public linearGradientColors: Array<ColorStop> = null;

  constructor(paint?: PathFillPaint) {
    super(paint);
  }

  setCommandsFill(commandsFill: string) {
    this.commandsFill = commandsFill;
  }

  setDrawFilled(filled: boolean) {
    this.filled = filled;
  }

  isDrawFilledEnabled(): boolean {
    return this.filled;
  }

  setGradientFillColor(linearGradientColors: Array<ColorStop>): void {
    this.linearGradientColors = linearGradientColors;
  }
}

export enum Style {
  FILL,
  STROKE,
  FILL_AND_STROKE,
}

/**
 * 用于绘制Legend的属性类
 */
export class RectPaint extends Paint {
  public startPoint: number[] = [0, 0];
  public linearGradientColors: Array<ColorStop>;

  constructor(paint?: RectPaint) {
    super(paint);
    if (paint != null && paint != undefined) {
      this.startPoint = paint.startPoint;
    }
  }

  setStartPoint(value: number[]) {
    this.x = value[0];
    this.y = value[1];
    this.startPoint = value;
  }

  getStartPoint(): number[] {
    return this.startPoint;
  }

  setGradientFillColor(linearGradientColors: Array<ColorStop>): void {
    this.linearGradientColors = linearGradientColors;
  }
}

export class BackGroundPaint extends RectPaint {
  public backgroundColor: number = 0xffffff;

  constructor(paint?: BackGroundPaint) {
    super(paint);
    if (paint != null && paint != undefined) {
      this.backgroundColor = paint.backgroundColor;
    }
  }

  setBackgroundColor(value: number) {
    this.backgroundColor = value;
  }

  getBackgroundColor(): number {
    return this.backgroundColor;
  }
}
export class LinearGradientPaint extends RectPaint {
  constructor(paint?: BackGroundPaint) {
    super(paint);
  }
}
export class IndexPositionPaint extends RectPaint {
  public dataSetIndex: number;
  public dataIndex: number;
  constructor(paint?: IndexPositionPaint) {
    super(paint);
  }
  setDataSetIndex(value: number) {
    this.dataSetIndex = value;
  }

  getDataSetIndex(): number {
    return this.dataSetIndex;
  }
  setDataIndex(value: number) {
    this.dataIndex = value;
  }

  getDataIndex(): number {
    return this.dataIndex;
  }
}

export class CirclePaint extends Paint {
  public enabled: boolean = false;
  public circleColor: string | number | Color = Color.White;
  public colors: JArrayList<number> = null;
  public radius: number = 2;
  public circleHoleRadius: number = 1;
  public circleHoleColor: string | number | Color = Color.White;
  public circleHoleEnabled: boolean = true;

  constructor(paint?: Paint) {
    super(paint);
  }

  setDrawCircles(enabled: boolean) {
    this.enabled = enabled;
  }

  isDrawCirclesEnabled(): boolean {
    return this.enabled;
  }

  setCirclesColor(circleColor: string | number | Color) {
    this.circleColor = circleColor;
  }

  setCircleRadius(radius: number) {
    this.radius = radius;
  }

  setCircleHoleRadius(circleHoleRadius: number) {
    this.circleHoleRadius = circleHoleRadius;
  }

  setCircleHoleColor(circleHoleColor: string | number | Color) {
    this.circleHoleColor = circleHoleColor;
  }

  setDrawCircleHole(circleHoleEnabled: boolean) {
    this.circleHoleEnabled = circleHoleEnabled;
  }
}

export class ImagePaint extends Paint {
  public icon: string | Resource = null;
  constructor(paint?: ImagePaint) {
    super(paint);
    if (paint != null && paint != undefined) {
      this.icon = paint.icon;
    }
  }

  setIcon(value: string | Resource) {
    this.icon = value;
  }

  getIcon(): string | Resource {
    return this.icon;
  }
}

export enum Align {
  LEFT,
  CENTER,
  RIGHT,
}

export class DashPathEffect {
  public dash: number[];
  public offset: number;

  constructor(dash: number[], offset: number) {
    this.dash = dash;
    this.offset = offset;
  }
}

export class FontMetrics {
  /**
   * The maximum distance above the baseline for the tallest glyph in
   * the font at a given text size.
   */
  public top: number;
  /**
   * The recommended distance above the baseline for singled spaced text.
   */
  public ascent: number;
  /**
   * The recommended distance below the baseline for singled spaced text.
   */
  public descent: number;
  /**
   * The maximum distance below the baseline for the lowest glyph in
   * the font at a given text size.
   */
  public bottom: number;
  /**
   * The recommended additional space to add between lines of text.
   */
  public leading: number;
}
