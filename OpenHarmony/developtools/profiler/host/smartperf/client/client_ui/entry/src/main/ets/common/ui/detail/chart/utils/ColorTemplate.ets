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

import { JArrayList } from '../utils/JArrayList';

class Color {
  build() {}
  public static argb(a: number, r: number, g: number, b: number): number {
    return 0xff000000 | (a << 24) | (r << 16) | (g << 8) | b;
  }

  public static rgb(r: number, g: number, b: number): number {
    return 0xff000000 | (r << 16) | (g << 8) | b;
  }

  public static alpha(color: number): number {
    return (color >> 24) & 0xff;
  }

  public static red(color: number): number {
    return (color >> 16) & 0xff;
  }

  public static green(color: number): number {
    return (color >> 8) & 0xff;
  }

  public static blue(color: number): number {
    return color & 0xff;
  }

  public static RGBToHSV(r, g, b, hsv: number[]) {
    let h = 0;
    let s = 0;
    let v = 0;
    let arr = [r, g, b];
    arr.sort(function (a, b) {
      return a - b;
    });
    var max = arr[2];
    var min = arr[0];
    v = max / 255;
    if (max === 0) {
      s = 0;
    } else {
      s = 1 - min / max;
    }
    if (max === min) {
      h = 0; //事实上，max===min的时候，h无论为多少都无所谓
    } else if (max === r && g >= b) {
      h = 60 * ((g - b) / (max - min)) + 0;
    } else if (max === r && g < b) {
      h = 60 * ((g - b) / (max - min)) + 360;
    } else if (max === g) {
      h = 60 * ((b - r) / (max - min)) + 120;
    } else if (max === b) {
      h = 60 * ((r - g) / (max - min)) + 240;
    }
    h = Math.floor(h);
    s = Math.floor(s * 100);
    v = Math.floor(v * 100);
    hsv.splice(0, hsv.length);
    hsv.push(h, s, v);
  }

  public static HSVToColor(alpha: number, arr: number[]): number {
    let h = arr[0];
    let s = arr[1];
    let v = arr[2];
    s = s / 100;
    v = v / 100;
    let r = 0;
    let g = 0;
    let b = 0;
    var i = Math.floor((h / 60) % 6);
    var f = h / 60 - i;
    var p = v * (1 - s);
    var q = v * (1 - f * s);
    var t = v * (1 - (1 - f) * s);
    switch (i) {
      case 0:
        r = v;
        g = t;
        b = p;
        break;
      case 1:
        r = q;
        g = v;
        b = p;
        break;
      case 2:
        r = p;
        g = v;
        b = t;
        break;
      case 3:
        r = p;
        g = q;
        b = v;
        break;
      case 4:
        r = t;
        g = p;
        b = v;
        break;
      case 5:
        r = v;
        g = p;
        b = q;
        break;
      default:
        break;
    }
    r = Math.floor(r * alpha);
    g = Math.floor(g * alpha);
    b = Math.floor(b * alpha);
    return Color.rgb(r, g, b);
  }
}

/**
 * Class that holds predefined color integer arrays (e.g.
 * ColorTemplate.VORDIPLOM_COLORS) and convenience methods for loading colors
 * from resources.
 *
 * @author Philipp Jahoda
 */
class ColorTemplate {
  build() {}
  /**
   * an "invalid" color that indicates that no color is set
   */
  public static COLOR_NONE: number = 0x00112233;

  /**
   * this "color" is used for the Legend creation and indicates that the next
   * form should be skipped
   */
  public static COLOR_SKIP: number = 0x00112234;

  /**
   * THE COLOR THEMES ARE PREDEFINED (predefined color integer arrays), FEEL
   * FREE TO CREATE YOUR OWN WITH AS MANY DIFFERENT COLORS AS YOU WANT
   */
  public static LIBERTY_COLORS: number[] = [
    Color.rgb(207, 248, 246),
    Color.rgb(148, 212, 212),
    Color.rgb(136, 180, 187),
    Color.rgb(118, 174, 175),
    Color.rgb(42, 109, 130),
  ];
  public static JOYFUL_COLORS: number[] = [
    Color.rgb(217, 80, 138),
    Color.rgb(254, 149, 7),
    Color.rgb(254, 247, 120),
    Color.rgb(106, 167, 134),
    Color.rgb(53, 194, 209),
  ];
  public static PASTEL_COLORS: number[] = [
    Color.rgb(64, 89, 128),
    Color.rgb(149, 165, 124),
    Color.rgb(217, 184, 162),
    Color.rgb(191, 134, 134),
    Color.rgb(179, 48, 80),
  ];
  public static COLORFUL_COLORS: number[] = [
    Color.rgb(193, 37, 82),
    Color.rgb(255, 102, 0),
    Color.rgb(245, 199, 0),
    Color.rgb(106, 150, 31),
    Color.rgb(179, 100, 53),
  ];
  public static VORDIPLOM_COLORS: number[] = [
    Color.rgb(192, 255, 140),
    Color.rgb(255, 247, 140),
    Color.rgb(255, 208, 140),
    Color.rgb(140, 234, 255),
    Color.rgb(255, 140, 157),
  ];
  public static MATERIAL_COLORS: number[] = [0x2ecc71, 0xf1c40f, 0xe74c3c, 0x3498db];

  /**
   * Converts the given hex-color-string to rgb.
   *
   * @param hex
   * @return
   */
  public static rgb(hex: string): number {
    var color: number = Number(hex.replace('#', ''));
    var r: number = (color >> 16) & 0xff;
    var g: number = (color >> 8) & 0xff;
    var b: number = (color >> 0) & 0xff;
    return Color.rgb(r, g, b);
  }

  /**
   *
   * @return
   */
  public static getHoloBlue(): number {
    return Color.rgb(51, 181, 229);
  }

  /**
   * Sets the alpha component of the given color.
   *
   * @param color
   * @param alpha 0 - 255
   * @return
   */
  public static colorWithAlpha(color: number, alpha: number): number {
    return (color & 0xffffff) | ((alpha & 0xff) << 24);
  }

  /**
   * turn an array of resource-colors (contains resource-id integers) into an
   * array list of actual color integers
   *
   * @param r
   * @param colors an integer array of resource id's of colors
   * @return
   */
  public static createColors(colors?: number[]): JArrayList<number> {
    var result: JArrayList<number> = new JArrayList<number>();
    for (var i = 0; i < colors.length; i++) {
      result.add(colors[i]);
    }
    return result;
  }

  /**
   * from Color.rgb()
   * @param r
   * @param g
   * @param b
   */
  public static colorRgb(r: number, g: number, b: number): number {
    return 0xff000000 | (r << 16) | (g << 8) | b;
  }

  /**
   * from Color.argb()
   * @param alpha
   * @param red
   * @param green
   * @param blue
   */
  public static argb(alpha: number, red: number, green: number, blue: number): number {
    return (alpha << 24) | (red << 16) | (green << 8) | blue;
  }

  /**
   * from Color red()
   * @param color
   */
  public static red(color: number): number {
    return (color >> 16) & 0xff;
  }

  /**
   * from Color green()
   * @param color
   */
  public static green(color: number): number {
    return (color >> 8) & 0xff;
  }

  /**
   * from Color blue()
   * @param color
   */
  public static blue(color: number): number {
    return color & 0xff;
  }
}

export { Color, ColorTemplate };
export default ColorTemplate;
