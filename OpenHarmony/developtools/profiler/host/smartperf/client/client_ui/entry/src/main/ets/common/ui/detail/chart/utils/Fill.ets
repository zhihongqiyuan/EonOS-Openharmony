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

import Paint, { Style, ImagePaint, RectPaint, BackGroundPaint, PathPaint } from '../data/Paint';
import MyRect from '../data/Rect';
import Utils from '../utils/Utils';
import { ColorStop } from '../data/LineDataSet';

export enum Type {
  EMPTY,
  COLOR,
  LINEAR_GRADIENT,
  DRAWABLE,
}

export enum MyDirection {
  DOWN,
  UP,
  RIGHT,
  LEFT,
}
export default class Fill {
  /**
   * the type of fill
   */
  private mType: Type = Type.EMPTY;

  /**
   * the color that is used for filling
   */
  private mColor: number = null;

  private mFinalColor: number = null;

  /**
   * the drawable to be used for filling
   */
  protected mDrawable: ImagePaint;/*mDrawable*/

  private mGradientColors: number[];

  private mGradientPositions: number[];

  /**
   * transparency used for filling
   */
  private mAlpha: number = 255;

  constructor(
    color?: number,
    startColor?: number,
    endColor?: number,
    gradientColors?: number[],
    gradientPositions?: number[],
    drawable?: ImagePaint
  ) {
    if (color != null && color != undefined) {
      this.mType = Type.COLOR;
      this.mColor = color;
      this.calculateFinalColor();
      return;
    }
    if (startColor != null && startColor != undefined && endColor != null && endColor != undefined) {
      this.mType = Type.LINEAR_GRADIENT;
      this.mGradientColors = [startColor, endColor];
      this.mGradientPositions = [0.0, 1.0];
      return;
    }
    if (gradientColors != null && gradientColors != undefined) {
      this.mType = Type.LINEAR_GRADIENT;
      this.mGradientColors = gradientColors;
      this.mGradientPositions = gradientPositions;
      return;
    }
    if (drawable != null && drawable != undefined) {
      this.mType = Type.DRAWABLE;
      this.mDrawable = drawable;
      return;
    }
  }

  public getType(): Type {
    return this.mType;
  }

  public setType(type: Type): void {
    this.mType = type;
  }

  public getColor(): number {
    return this.mColor;
  }

  public setColor(color: number): void {
    this.mColor = color;
    this.calculateFinalColor();
  }

  public getGradientColors(): number[] {
    return this.mGradientColors;
  }

  public getGradientPositions(): number[] {
    return this.mGradientPositions;
  }

  public setGradientPositions(positions: number[]): void {
    this.mGradientPositions = positions;
  }

  public setGradientColors(colors?: number[], startColor?: number, endColor?: number): void {
    if (colors != null && colors != undefined) {
      this.mGradientColors = colors;
      return;
    }
    this.mGradientColors = [startColor, endColor];
  }

  public getAlpha(): number {
    return this.mAlpha;
  }

  public setAlpha(alpha: number): void {
    this.mAlpha = alpha;
    this.calculateFinalColor();
  }

  private calculateFinalColor(): void {
    if (this.mColor == null) {
      this.mFinalColor = null;
    } else {
      let alpha: number = Math.floor(((this.mColor >> 24) / 255.0) * (this.mAlpha / 255.0) * 255.0);
      this.mFinalColor = (alpha << 24) | (this.mColor & 0xffffff);
    }
  }

  public fillRect(
    paint: RectPaint,
    left: number,
    top: number,
    right: number,
    bottom: number,
    gradientDirection: MyDirection
  ): Paint {
    switch (this.mType) {
      case Type.EMPTY:
        return;
      case Type.COLOR:
        if (this.mFinalColor == null) {
          return;
        }
        if (this.isClipPathSupported()) {
          let rectB: BackGroundPaint = new BackGroundPaint();
          rectB.setBackgroundColor(this.mFinalColor);
          return rectB;
        } else {
          let previous: Style = paint.getStyle();
          let previousColor: number = paint.getColor() as number;
          paint.setStyle(Style.FILL);
          paint.setColor(this.mFinalColor);
          let rectP: RectPaint = new RectPaint(paint);
          rectP.setStartPoint([left, top]);
          rectP.setWidth(right - left);
          rectP.setHeight(bottom - top);
          paint.setColor(previousColor);
          paint.setStyle(previous);
          return rectP;
        }
      case Type.LINEAR_GRADIENT:
        if (this.mGradientColors == null) {
          return;
        }
        let gradient: RectPaint = new RectPaint();
        let leftResult: number = left;
        let topResult = top;
        let rightResult: number = right;
        let bottomResult = bottom;
        gradient.setX(leftResult);
        gradient.setY(topResult);
        gradient.setWidth(rightResult - leftResult);
        gradient.setHeight(bottomResult - topResult);
        gradient.setStyle(paint.getStyle());
        gradient.setColor(paint.getColor());
        let colorArr: ColorStop[] = [];
        for (let i = 0; i < this.mGradientColors.length; i++) {
          colorArr.push([this.mGradientColors[i], this.mGradientPositions[i]]);
        }
        gradient.setGradientFillColor(colorArr);
        return gradient;
      case Type.DRAWABLE:
        if (this.mDrawable == null || this.mDrawable) {
          return;
        }
        let imagePaint = new ImagePaint(this.mDrawable);
        imagePaint.x = left;
        imagePaint.y = top;
        imagePaint.setWidth(right - left);
        imagePaint.setHeight(bottom - top);
        return imagePaint;
    }
  }

  public fillPath(
    chartWidth: number,
    chartHeight: number,
    path: string /*Path*/,
    paint: PathPaint,
    clipRect: MyRect
  ): Paint {
    switch (this.mType) {
      case Type.EMPTY:
        return;

      case Type.COLOR:
        if (this.mFinalColor == null || this.mFinalColor == undefined) {
          return;
        }
        if (clipRect != null && this.isClipPathSupported()) {
          let rectP: BackGroundPaint = new BackGroundPaint();
          rectP.setBackgroundColor(this.mFinalColor);
          return rectP;
        } else {
          // save
          let previous: Style = paint.getStyle();
          let previousColor = paint.getColor();

          // set
          paint.setStyle(Style.FILL);
          paint.setColor(this.mFinalColor);
          let pathP: PathPaint = new PathPaint(paint);
          pathP.setCommands(path);
          // restore
          paint.setColor(previousColor);
          paint.setStyle(previous);
          return pathP;
        }
      case Type.LINEAR_GRADIENT:
        if (this.mGradientColors == null) {
          return;
        }
        let gradient: PathPaint = new PathPaint();
        let leftResult: number = 0;
        let topResult: number = 0;
        let rightResult: number = chartWidth;
        let bottomResult: number = chartHeight;
        gradient.setX(leftResult);
        gradient.setY(topResult);
        gradient.setWidth(rightResult - leftResult);
        gradient.setHeight(bottomResult - topResult);
        let colorArr: ColorStop[] = [];
        for (let i = 0; i < this.mGradientColors.length; i++) {
          colorArr.push([this.mGradientColors[i], this.mGradientPositions[i]]);
        }
        gradient.setStyle(paint.getStyle());
        gradient.setColor(paint.getColor());
        gradient.setGradientFillColor(colorArr);
        return gradient;
      case Type.DRAWABLE:
        if (this.mDrawable == null) {
          return;
        }

        this.ensureClipPathSupported();

        let imagePaint = new ImagePaint(this.mDrawable);
        let leftImage: number = clipRect == null ? 0 : clipRect.left;
        let topImage: number = clipRect == null ? 0 : clipRect.top;
        let rightImage: number = clipRect == null ? chartWidth : clipRect.right;
        let bottomImage: number = clipRect == null ? chartHeight : clipRect.bottom;
        imagePaint.x = leftImage;
        imagePaint.y = topImage;
        imagePaint.setWidth(rightImage - leftImage);
        imagePaint.setHeight(bottomImage - topImage);

        return this.mDrawable;
    }
  }

  private isClipPathSupported(): boolean {
    return Utils.getSDKInt() >= 8;
  }

  private ensureClipPathSupported(): void {
    if (Utils.getSDKInt() < 8) {
      throw new Error(
        'Fill-drawables not (yet) supported below API level 18, ' +
        'this code was run on API level ' +
        Utils.getSDKInt() +
        '.'
      );
    }
  }
}
