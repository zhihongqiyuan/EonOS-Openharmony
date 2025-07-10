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

import Paint, { TextPaint, ImagePaint } from '../data/Paint';
import MyRect from '../data/Rect';
import FSize from './FSize';
import MPPointF from './MPPointF';
import IValueFormatter from '../formatter/IValueFormatter';
import DefaultValueFormatter from '../formatter/DefaultValueFormatter';
import deviceInfo from '@ohos.deviceInfo';

export default abstract class Utils {
  private static scaledDensity: number = 3.3125;
  private static mMinimumFlingVelocity: number = 50;
  private static mMaximumFlingVelocity: number = 8000;
  public static DEG2RAD: number = Math.PI / 180.0;
  public static FDEG2RAD: number = Math.PI / 180;
  public static DOUBLE_EPSILON: number = 4.9e-324;
  public static FLOAT_EPSILON: number = 1.4e-45;

  public static init() {}

  public static convertDpToPixel(dp: number): number {
    return vp2px(dp);
  }

  public static setScaledDensity(value: number) {
    this.scaledDensity = value;
  }

  public static calcTextWidth(paint: Paint, demoText: string): number {
    return (demoText.length * paint.getTextSize()) / 2;
  }

  private static mCalcTextHeightRect: MyRect = new MyRect();
 
  public static calcTextHeight(paint: Paint, demoText: string): number {
    return paint.getTextSize();
  }

  public static getLineHeight(paint: Paint): number {
    return paint.getTextSize();
  }

  public static getLineSpacing(paint: Paint): number {
    return 1.2;
  }

  public static calcTextSize(paint: Paint, demoText: string): FSize {
    var fsize: FSize = new FSize(paint.getTextSize() * demoText.length, paint.getTextSize());
    return fsize;
  }

  private static mDefaultValueFormatter: IValueFormatter = Utils.generateDefaultValueFormatter();
  private static generateDefaultValueFormatter(): IValueFormatter {
    var formatter: DefaultValueFormatter = new DefaultValueFormatter(1);
    return formatter;
  }


  public static getDefaultValueFormatter(): IValueFormatter {
    return Utils.mDefaultValueFormatter;
  }


  public static roundToNextSignificant(number: number): number {
    if (number == Infinity || isNaN(number) || number == 0.0) {
      return 0;
    }

    const d: number = Math.ceil(Math.log10(number < 0 ? -number : number));
    const pw: number = 1 - Math.floor(d);
    const magnitude: number = Math.pow(10, pw);
    const shifted: number = Math.round(number * magnitude);
    return shifted / magnitude;
  }

  public static getDecimals(number: number): number {
    let i: number = this.roundToNextSignificant(number);

    if (i == Infinity) {
      return 0;
    }

    return Math.floor(Math.ceil(-Math.log10(i)) + 2);
  }

  public static nextUp(d: number): number {
    if (d == Infinity) {
      return d;
    } else {
      d += 0.0;
      return d >= 0.0 ? (d += 0.000000001) : (d -= 0.000000001);
    }
  }

  public static getPosition(center: MPPointF, dist: number, angle: number, outputPoint?: MPPointF): MPPointF {
    let p: MPPointF = outputPoint == null || outputPoint == undefined ? MPPointF.getInstance(0, 0) : outputPoint;
    p.x = center.x + dist * Math.cos((angle * Math.PI) / 180);
    p.y = center.y + dist * Math.sin((angle * Math.PI) / 180);
    return p;
  }

  public static getMinimumFlingVelocity(): number {
    return Utils.mMinimumFlingVelocity;
  }

  public static getNormalizedAngle(angle: number): number {
    while (angle < 0) {
      angle += 360;
    }

    return angle % 360;
  }

  private static mDrawableBoundsCache: MyRect = new MyRect();

  public static drawImage(icon: string | Resource, x: number, y: number, width: number, height: number): Paint[] {
    let drawOffset: MPPointF = MPPointF.getInstance();
    drawOffset.x = x - width / 2;
    drawOffset.y = y - height / 2;

    let drawable: ImagePaint = new ImagePaint();
    drawable.setX(this.mDrawableBoundsCache.left);
    drawable.setY(this.mDrawableBoundsCache.top);
    drawable.setWidth(width);
    drawable.setHeight(width);
    drawable.setIcon(icon);

    drawable.setX(drawable.x + drawOffset.x);
    drawable.setY(drawable.y + drawOffset.y);
    return [drawable];
  }

  public static drawXAxisValue(
    text: string,
    x: number,
    y: number,
    paint: TextPaint,
    anchor: MPPointF,
    angleDegrees: number
  ): Paint {
    var drawOffsetX: number = 0;
    var drawOffsetY: number = 0;

    var labelSize: FSize = Utils.calcTextSize(paint, text);

    drawOffsetX -= labelSize.width;

    drawOffsetY += -labelSize.height;

    paint.setTextAlign(TextAlign.Start);
    paint.setText(text);
    if (angleDegrees != 0) {
      drawOffsetX -= labelSize.width * 0.5;
      drawOffsetY -= labelSize.height * 0.5;

      var translateX: number = x;
      var translateY: number = y;

      if (anchor.x != 0.5 || anchor.y != 0.5) {
        var rotatedSize: FSize = Utils.getSizeOfRotatedRectangleByDegrees(
        labelSize.width,
        labelSize.height,
          angleDegrees
        );

        translateX -= rotatedSize.width * (anchor.x - 0.5);
        translateY -= rotatedSize.height * (anchor.y - 0.5);
        FSize.recycleInstance(rotatedSize);
      }
      paint.setTranslateX(translateX);
      paint.setTranslateY(translateY);
      paint.setRotate(angleDegrees);
      paint.setX(drawOffsetX);
      paint.setY(drawOffsetY);
    } else {
      if (anchor.x != 0 || anchor.y != 0) {
        drawOffsetX = (labelSize.width / 2) * anchor.x;
        drawOffsetY = 12 * anchor.y;
      }
      x -= drawOffsetX;
      y -= drawOffsetY;
      paint.setX(x);
      paint.setY(y);
    }
    return paint;
  }

  public static getSizeOfRotatedRectangleByDegrees(
    rectangleWidth: number,
    rectangleHeight: number,
    degrees: number
  ): FSize {
    var radians: number = degrees * Utils.FDEG2RAD;
    return Utils.getSizeOfRotatedRectangleByRadians(rectangleWidth, rectangleHeight, radians);
  }
 
  public static getSizeOfRotatedRectangleByRadians(
    rectangleWidth: number,
    rectangleHeight: number,
    radians: number
  ): FSize {
    return FSize.getInstance(
      Math.abs(rectangleWidth * Math.cos(radians)) + Math.abs(rectangleHeight * Math.sin(radians)),
      Math.abs(rectangleWidth * Math.sin(radians)) + Math.abs(rectangleHeight * Math.cos(radians))
    );
  }

  public static getSDKInt(): number {
    return deviceInfo.sdkApiVersion;
  }
}
