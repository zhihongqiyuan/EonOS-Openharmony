/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

/**
 * @file
 * @kit ArkGraphics2D
 */
/**
 * The date structure that provides the basis for graphics.
 *
 * @namespace common2D
 * @syscap SystemCapability.Graphics.Drawing
 * @since 11
 */
declare namespace common2D {
  /**
   * Describes a color in ARGB format.
   * @typedef Color
   * @syscap SystemCapability.Graphics.Drawing
   * @since 11
   */
  interface Color {
    /**
     * Alpha component of the color. The value is an integer ranging from 0 to 255.
     * @type { number }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    alpha: number;
    /**
     * Red component of the color. The value is an integer ranging from 0 to 255.
     * @type { number }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    red: number;
    /**
     * Green component of the color. The value is an integer ranging from 0 to 255.
     * @type { number }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    green: number;
    /**
     * Blue component of the color. The value is an integer ranging from 0 to 255.
     * @type { number }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    blue: number;
  }

  /**
   * Describes a rectangle, which can be defined by two coordinate points: upper left corner point and lower right corner point.
   * @typedef Rect
   * @syscap SystemCapability.Graphics.Drawing
   * @since 11
   */
  interface Rect {
    /**
     * X coordinate of the upper left corner of the rectangle. The value is a floating point number.
     * @type { number }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    left: number;
    /**
     * Y coordinate of the upper left corner of the rectangle. The value is a floating point number.
     * @type { number }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    top: number;
    /**
     * X coordinate of the lower right corner of the rectangle. The value is a floating point number.
     * @type { number }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    right: number;
    /**
     * Y coordinate of the lower right corner of the rectangle. The value is a floating point number.
     * @type { number }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    bottom: number;
  }

  /**
   * Describes a coordinate point.
   * @typedef Point
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  interface Point {
    /**
     * Horizontal coordinate. The value is a floating point number.
     * @type { number }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    x: number;

    /**
     * Vertical coordinate. The value is a floating point number.
     * @type { number }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    y: number;
  }

  /**
   * Describes a 3D coordinate point.
   *
   * @typedef Point3d
   * @extends Point
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  interface Point3d extends Point {
    /**
     * Z-axis coordinate. The value is a floating point number.
     * @type { number }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    z: number;
  }
  
}

export default common2D;