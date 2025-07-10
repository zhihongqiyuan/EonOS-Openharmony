/*
 * Copyright (c) 2023-2024 Huawei Device Co., Ltd.
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

import type image from './@ohos.multimedia.image';
import type common2D from './@ohos.graphics.common2D';
import { Resource } from './global/resource';

/**
 * The common2D module defines some common data types in the 2D graphics field.
 *
 * @namespace drawing
 * @syscap SystemCapability.Graphics.Drawing
 * @since 11
 */
declare namespace drawing {
  /**
   * Enumerates the blend modes. In blend mode, each operation generates a new color from two colors (source color and destination color).
   * These operations are the same for the red, green, and blue color channels (the alpha channel follows a different rule).
   * For simplicity, the following description uses the alpha channel as an example rather than naming each channel individually.
   * 
   * For brevity, the following abbreviations are used:
   * 
   * s: source.
   * d: destination.
   * sa: source alpha.
   * da: destination alpha.
   * The following abbreviations are used in the calculation result:
   * 
   * r: used when the calculation method is the same for the four channels (alpha, red, green, and blue channels).
   * ra: used when only the alpha channel is manipulated.
   * rc: used when the other three color channels are manipulated.
   * The table below shows the effect of each blend mode, where the yellow rectangle is the source and the blue circle is the destination.
   *
   * @enum { number }
   * @syscap SystemCapability.Graphics.Drawing
   * @since 11
   */
  enum BlendMode {
    /**
     * r = 0, sets the the destination pixels to fully transparent.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    CLEAR = 0,
    /**
     * r = s (all channels of the result equal those of the source), replaces the destination pixels with the source pixels.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    SRC = 1,
    /**
     * r = d (all channels of the result equal those of the destination), keeps the destination pixels unchanged.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    DST = 2,
    /**
     * r = s + (1 - sa) * d, draws the source pixels over the destination pixels, considering the source's transparency.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    SRC_OVER = 3,
    /**
     * r = d + (1 - da) * s, draws the destination pixels over the source pixels, considering the destination's transparency.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    DST_OVER = 4,
    /**
     * r = s * da, retains only the intersection of the source pixels with the opaque parts of the destination.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    SRC_IN = 5,
    /**
     * r = d * sa, retains only the intersection of the destination pixels with the opaque parts of the source.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    DST_IN = 6,
    /**
     * r = s * (1 - da), retains the parts of the source pixels that do not overlap with the destination.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    SRC_OUT = 7,
    /**
     * r = d * (1 - sa), retains the parts of the destination pixels that do not overlap with the source.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    DST_OUT = 8,
    /**
     * r = s * da + d * (1 - sa), covers the destination pixels with the source pixels, showing the source only in the opaque parts of the destination.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    SRC_ATOP = 9,
    /**
     * r = d * sa + s * (1 - da), covers the source pixels with the destination pixels, showing the destination only in the opaque parts of the source.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    DST_ATOP = 10,
    /**
     * r = s * (1 - da) + d * (1 - sa), shows only the non-overlapping parts of the source and destination pixels.	
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    XOR = 11,
    /**
     * r = min(s + d, 1), adds the color values of the source and destination pixels.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    PLUS = 12,
    /**
     * r = s * d, multiplies the color values of the source and destination pixels.	
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    MODULATE = 13,
    /**
     * r = s + d - s * d, inverts the color values of the source and destination pixels, multiplies them,
     * and then inverts the result, typically producing a brighter outcome.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    SCREEN = 14,
    /**
     * Selectively applies MULTIPLY or SCREEN based on the brightness of the destination pixels, enhancing contrast.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    OVERLAY = 15,
    /**
     * rc = s + d - max(s * da, d * sa), ra = s + (1 - sa) * d, takes the darker color values between the source and destination pixels.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    DARKEN = 16,
    /**
     * rc = s + d - min(s * da, d * sa), ra = s + (1 - sa) * d, takes the lighter color values between the source and destination pixels.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    LIGHTEN = 17,
    /**
     * Brightens the destination pixels by reducing contrast to reflect the source pixels.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    COLOR_DODGE = 18,
    /**
     * Darkens the destination pixels by increasing contrast to reflect the source pixels.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    COLOR_BURN = 19,
    /**
     * Selectively applies MULTIPLY or SCREEN based on the brightness of the source pixels.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    HARD_LIGHT = 20,
    /**
     * Softly brightens or darkens the destination pixels based on the brightness of the source pixels.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    SOFT_LIGHT = 21,
    /**
     * rc = s + d - 2 * (min(s * da, d * sa)), ra = s + (1 - sa) * d, calculates the difference between the color values of the source and destination pixels.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    DIFFERENCE = 22,
    /**
     * rc = s + d - two(s * d), ra = s + (1 - sa) * d, similar to DIFFERENCE but with lower contrast.	
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    EXCLUSION = 23,
    /**
     * r = s * (1 - da) + d * (1 - sa) + s * d, multiplies the color values of the source and destination pixels, typically resulting in a darker outcome.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    MULTIPLY = 24,
    /**
     * Uses the hue of the source pixels and the saturation and brightness of the destination pixels.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    HUE = 25,
    /**
     * Uses the saturation of the source pixels and the hue and brightness of the destination pixels.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    SATURATION = 26,
    /**
     * Uses the hue and saturation of the source pixels and the brightness of the destination pixels.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    COLOR = 27,
    /**
     * Uses the brightness of the source pixels and the hue and saturation of the destination pixels.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    LUMINOSITY = 28,
  }

  /**
   * Enumerates the directions of a closed contour.
   * @enum { number }
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  enum PathDirection {
    /**
     * Adds a closed contour clockwise.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    CLOCKWISE = 0,

    /**
     * Adds a closed contour counterclockwise.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    COUNTER_CLOCKWISE = 1,
  }

  /**
   * Enumerates the fill types of a path.
   * @enum { number }
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  enum PathFillType {
    /**
     * Specifies that "inside" is computed by a non-zero sum of signed edge crossings. Specifically, draws a point and emits a ray in any direction.
     * A count is used to record the number of intersection points of the ray and path, and the initial count is 0.
     * When encountering a clockwise intersection point (the path passes from the left to the right of the ray), the count increases by 1.
     * When encountering a counterclockwise intersection point (the path passes from the right to the left of the ray), the count decreases by 1.
     * If the final count is not 0, the point is inside the path and needs to be colored. If the final count is 0, the point is not colored.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    WINDING = 0,

    /**
     * Specifies that "inside" is computed by an odd number of edge crossings. Specifically, draws a point and emits a ray in any direction.
     * If the number of intersection points of the ray and path is an odd number, the point is considered to be inside the path and needs to be colored.
     * If the number is an even number, the point is not colored.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    EVEN_ODD = 1,

    /**
     * Same as WINDING, but draws outside of the path, rather than inside.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    INVERSE_WINDING = 2,

    /**
     * Same as EVEN_ODD, but draws outside of the path, rather than inside.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    INVERSE_EVEN_ODD = 3,
  }

  /**
  * Enumerates the dimensions of matrix information in path measurement.
  * It is often used in animation scenarios where objects move along a path.
  * @enum { number }
  * @syscap SystemCapability.Graphics.Drawing
  * @since 12
  */
  enum PathMeasureMatrixFlags {
    /**
     * Matrix corresponding to the position information.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    GET_POSITION_MATRIX = 0,
    /**
     * Matrix corresponding to the tangent information.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    GET_TANGENT_MATRIX = 1,
    /**
     * Matrix corresponding to the position and tangent information.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    GET_POSITION_AND_TANGENT_MATRIX = 2,
  }

  /**
   * Implements a rounded rectangle.
   *
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  class RoundRect {

    /**
     * Creates a deep copy of the specified round rect object.
     * @param { RoundRect } roundRect - The round rect object to copy.
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    constructor(roundRect: RoundRect);

    /**
     * A constructor used to create a RoundRect object. A rounded rectangle is created when both xRadii and yRadii are greater than 0.
     * Otherwise, only a rectangle is created.
     * @param { common2D.Rect } rect - Rectangle that encloses the rounded rectangle to create.
     * @param { number } xRadii - Radius of the rounded corner on the X axis. The value is a floating point number. A negative number is invalid.
     * @param { number } yRadii - Radius of the rounded corner on the Y axis. The value is a floating point number. A negative number is invalid.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    constructor(rect: common2D.Rect, xRadii: number, yRadii: number);

    /**
     * Sets the radii of the specified rounded corner in this rounded rectangle.
     * @param { CornerPos } pos - Position of the rounded corner.
     * @param { number } x - Radius of the rounded corner on the X axis. The value is a floating point number. A negative number is invalid.
     * @param { number } y - Radius of the rounded corner on the Y axis. The value is a floating point number. A negative number is invalid.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    setCorner(pos: CornerPos, x: number, y: number): void;

    /**
     * Obtains the radii of the specified rounded corner in this rounded rectangle.
     * @param { CornerPos } pos - Position of the rounded corner.
     * @returns { common2D.Point } Point. The horizontal coordinate indicates the radius of the rounded corner on the X axis,
     * and the vertical coordinate indicates the radius on the Y axis.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    getCorner(pos: CornerPos): common2D.Point;

    /**
     * Translates this rounded rectangle by an offset along the X axis and Y axis.
     * @param { number } dx - Horizontal distance to translate. A positive number indicates a translation towards the positive direction of the X axis,
     * and a negative number indicates a translation towards the negative direction of the X axis. The value is a floating point number.
     * @param { number } dy - Vertical distance to translate. A positive number indicates a translation towards the positive direction of the Y axis,
     * and a negative number indicates a translation towards the negative direction of the Y axis. The value is a floating point number.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    offset(dx: number, dy: number): void;
  }

  /**
   * Enumerates the path operation types. It is often used in path combination and clipping scenarios.
   * @enum { number }
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  enum PathOp {
    /**
     * Difference operation.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    DIFFERENCE = 0,

    /**
     * Intersection operation.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    INTERSECT = 1,

    /**
     * Union operation.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    UNION = 2,

    /**
     * Xor operation.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    XOR = 3,

    /**
     * Reverse difference operation.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    REVERSE_DIFFERENCE = 4,
  }

  /**
   * Enumerates the path operation types contained in an iterator. It is used to read path operation instructions.
   * @enum { number }
   * @syscap SystemCapability.Graphics.Drawing
   * @since 18
   */
  enum PathIteratorVerb {
    /**
     * Sets the start point.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    MOVE = 0,

    /**
     * Adds a line segment.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    LINE = 1,

    /**
     * Adds a quadratic Bezier curve for smooth transitions.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    QUAD = 2,

    /**
     * Adds a conic curve.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    CONIC = 3,

    /**
     * Adds a cubic Bezier curve for smooth transitions.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    CUBIC = 4,

    /**
     * Closes a path.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    CLOSE = 5,

    /**
     * The path setting is complete.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    DONE = CLOSE + 1,
  }

  /**
   * Implements a path operation iterator. You can read path operation instructions by traversing the iterator.
   *
   * @syscap SystemCapability.Graphics.Drawing
   * @since 18
   */
  class PathIterator {
    /**
     * Creates an iterator and binds it with a path.
     * @param { Path } path - Path object bound to the iterator.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    constructor(path: Path);

    /**
     * Retrieves the next operation in this path and moves the iterator to that operation.
     * @param { Array<common2D.Point> } points - Indicates the point array.
     * @param { number } offset - Indicates the offset into the array where entries should be placed. The default value is 0.
     * @returns { PathIteratorVerb } Returns the next verb in this iterator's path.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    next(points: Array<common2D.Point>, offset?: number): PathIteratorVerb;

    /**
     * Retrieves the next operation in this path, without moving the iterator.
     * @returns { PathIteratorVerb } Returns the next verb in the iteration.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    peek(): PathIteratorVerb;

    /**
     * Checks whether there is a next operation in the path operation iterator.
     * @returns { boolean } Returns true if there are more elements to be iterated through, false otherwise.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    hasNext(): boolean;
  }

  /**
   * A compound geometric path consisting of line segments, arcs, quadratic Bezier curves, and cubic Bezier curves.
   *
   * @syscap SystemCapability.Graphics.Drawing
   * @since 11
   */
  class Path {
    /**
     * Constructs a path.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    constructor();

    /**
     * Constructs a copy of an existing path.
     * @param { Path } path - Path to copy.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    constructor(path: Path);

    /**
     * Sets the Path with the same content of another.
     * @param { Path } src - the path to copy content from.
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    set(src: Path): void;

    /**
     * Sets the start point of this path.
     * @param { number } x - X coordinate of the start point. The value is a floating point number.
     * @param { number } y - Y coordinate of the start point. The value is a floating point number.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    moveTo(x: number, y: number): void;

    /**
     * Draws a line segment from the last point of this path to the target point. If the path is empty, the start point (0, 0) is used.
     * @param { number } x - X coordinate of the target point. The value is a floating point number.
     * @param { number } y - Y coordinate of the target point. The value is a floating point number.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    lineTo(x: number, y: number): void;

    /**
     * Draws an arc to this path using angle arc mode. This mode first defines a rectangle and takes its inscribed ellipse.
     * Then, it specifies a start angle and a sweep angle. The arc is the portion of the ellipse's circumference defined by the start angle
     * and the sweep angle. By default, a line segment from the last point of the path to the start point of the arc is also added.
     * @param { number } x1 - X coordinate of the upper left corner of the rectangle. The value is a floating point number.
     * @param { number } y1 - Y coordinate of the upper left corner of the rectangle. The value is a floating point number.
     * @param { number } x2 - X coordinate of the lower right corner of the rectangle. The value is a floating point number.
     * @param { number } y2 - Y coordinate of the lower right corner of the rectangle. The value is a floating point number.
     * @param { number } startDeg - Start angle. The start direction (0°) of the angle is the positive direction of the X axis.
     * @param { number } sweepDeg - Angle to sweep, in degrees. A positive number indicates a clockwise sweep,
     * and a negative value indicates a counterclockwise swipe. The actual swipe degree is the modulo operation result of the input parameter by 360.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    arcTo(x1: number, y1: number, x2: number, y2: number, startDeg: number, sweepDeg: number): void;

    /**
     * Draws a quadratic Bezier curve from the last point of this path to the target point. If the path is empty, the start point (0, 0) is used.
     * @param { number } ctrlX - X coordinate of the control point. The value is a floating point number.
     * @param { number } ctrlY - Y coordinate of the control point. The value is a floating point number.
     * @param { number } endX - X coordinate of the target point. The value is a floating point number.
     * @param { number } endY - Y coordinate of the target point. The value is a floating point number.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    quadTo(ctrlX: number, ctrlY: number, endX: number, endY: number): void;

    /**
     * Draws a conic curve from the last point of this path to the target point. If the path is empty, the start point (0, 0) is used.
     * @param { number } ctrlX - X coordinate of the control point. The value is a floating point number.
     * @param { number } ctrlY - Y coordinate of the control point. The value is a floating point number.
     * @param { number } endX - X coordinate of the target point. The value is a floating point number.
     * @param { number } endY - Y coordinate of the target point. The value is a floating point number.
     * @param { number } weight - Weight of the curve, which determines its shape. The larger the value,
     * the closer of the curve to the control point. If the value is less than or equal to 0,
     * this API is equivalent to lineTo, that is, adding a line segment from the last point of the path to the target point.
     * If the value is 1, this API is equivalent to quadTo. The value is a floating point number.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    conicTo(ctrlX: number, ctrlY: number, endX: number, endY: number, weight: number): void;

    /**
     * Draws a cubic Bezier curve from the last point of this path to the target point. If the path is empty, the start point (0, 0) is used.
     * @param { number } ctrlX1 - X coordinate of the first control point. The value is a floating point number.
     * @param { number } ctrlY1 - Y coordinate of the first control point. The value is a floating point number.
     * @param { number } ctrlX2 - X coordinate of the second control point. The value is a floating point number.
     * @param { number } ctrlY2 - Y coordinate of the second control point. The value is a floating point number.
     * @param { number } endX - X coordinate of the target point. The value is a floating point number.
     * @param { number } endY - Y coordinate of the target point. The value is a floating point number.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    cubicTo(ctrlX1: number, ctrlY1: number, ctrlX2: number, ctrlY2: number, endX: number, endY: number): void;

    /**
     * Sets the start position relative to the last point of this path. If the path is empty, the start point (0, 0) is used.
     * @param { number } dx - X offset of the start point relative to the last point. A positive number indicates a rightward shift from the last point,
     * and a negative number indicates a leftward shift from the last point. The value is a floating point number.
     * @param { number } dy - Y offset of the start point relative to the last point. A positive number indicates an upward shift from the last point,
     * and a negative number indicates a downward shift from the last point. The value is a floating point number.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    rMoveTo(dx: number, dy: number): void;

    /**
     * Draws a line segment from the last point of this path to a point relative to the last point. If the path is empty, the start point (0, 0) is used.
     * @param { number } dx - X offset of the target point relative to the last point. A positive number indicates a rightward shift from the last point,
     * and a negative number indicates a leftward shift from the last point. The value is a floating point number.
     * @param { number } dy - Y offset of the target point relative to the last point. A positive number indicates an upward shift from the last point,
     * and a negative number indicates a downward shift from the last point. The value is a floating point number.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    rLineTo(dx: number, dy: number): void;

    /**
     * Draws a quadratic Bezier curve from the last point of this path to a point relative to the last point.
     * If the path is empty, the start point (0, 0) is used.
     * @param { number } dx1 - X offset of the control point relative to the last point. A positive number indicates a rightward shift from the last point,
     * and a negative number indicates a leftward shift from the last point. The value is a floating point number.
     * @param { number } dy1 - Y offset of the control point relative to the last point. A positive number indicates an upward shift from the last point,
     * and a negative number indicates a downward shift from the last point. The value is a floating point number.
     * @param { number } dx2 - X offset of the target point relative to the last point. A positive number indicates a rightward shift from the last point,
     * and a negative number indicates a leftward shift from the last point. The value is a floating point number.
     * @param { number } dy2 - Y offset of the target point relative to the last point. A positive number indicates an upward shift from the last point,
     * and a negative number indicates a downward shift from the last point. The value is a floating point number.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    rQuadTo(dx1: number, dy1: number, dx2: number, dy2: number): void;

    /**
     * Draws a conic curve from the last point of this path to a point relative to the last point. If the path is empty, the start point (0, 0) is used.
     * @param { number } ctrlX - X offset of the control point relative to the last point. A positive number indicates a rightward shift from the last point,
     * and a negative number indicates a leftward shift from the last point. The value is a floating point number.
     * @param { number } ctrlY - Y offset of the control point relative to the last point. A positive number indicates an upward shift from the last point,
     * and a negative number indicates a downward shift from the last point. The value is a floating point number.
     * @param { number } endX - X offset of the target point relative to the last point. A positive number indicates a rightward shift from the last point,
     * and a negative number indicates a leftward shift from the last point. The value is a floating point number.
     * @param { number } endY - Y offset of the target point relative to the last point. A positive number indicates an upward shift from the last point,
     * and a negative number indicates a downward shift from the last point. The value is a floating point number.
     * @param { number } weight - Weight of the curve, which determines its shape. The larger the value, the closer of the curve to the control point.
     * If the value is less than or equal to 0, this API is equivalent to rLineTo, that is, adding a line segment from the last point of the path
     * to the target point. If the value is 1, this API is equivalent to rQuadTo. The value is a floating point number.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    rConicTo(ctrlX: number, ctrlY: number, endX: number, endY: number, weight: number): void;

    /**
     * Draws a cubic Bezier curve from the last point of this path to a point relative to the last point.
     * If the path is empty, the start point (0, 0) is used.
     * @param { number } ctrlX1 - X offset of the first control point relative to the last point. A positive number indicates a rightward shift
     * from the last point, and a negative number indicates a leftward shift from the last point. The value is a floating point number.
     * @param { number } ctrlY1 - Y offset of the first control point relative to the last point. A positive number indicates an upward shift
     * from the last point, and a negative number indicates a downward shift from the last point. The value is a floating point number.
     * @param { number } ctrlX2 - X offset of the second control point relative to the last point. A positive number indicates a rightward shift
     * from the last point, and a negative number indicates a leftward shift from the last point. The value is a floating point number.
     * @param { number } ctrlY2 - Y offset of the second control point relative to the last point. A positive number indicates an upward shift
     * from the last point, and a negative number indicates a downward shift from the last point. The value is a floating point number.
     * @param { number } endX - X offset of the target point relative to the last point. A positive number indicates a rightward shift
     * from the last point, and a negative number indicates a leftward shift from the last point. The value is a floating point number.
     * @param { number } endY - Y offset of the target point relative to the last point. A positive number indicates an upward shift
     * from the last point, and a negative number indicates a downward shift from the last point. The value is a floating point number.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    rCubicTo(ctrlX1: number, ctrlY1: number, ctrlX2: number, ctrlY2: number, endX: number, endY: number): void;

    /**
     * Adds a polygon to this path.
     * @param { Array<common2D.Point> } points - Array that holds the vertex coordinates of the polygon.
     * @param { boolean } close - Whether to close the path, that is, whether to add a line segment from the start point
     * to the end point of the path. The value true means to close the path, and false means the opposite.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    addPolygon(points: Array<common2D.Point>, close: boolean): void;

    /**
     * Combines this path with the passed-in path based on the specified operation mode.
     * @param { Path } path - Path object, which will be combined with the current path.
     * @param { PathOp } pathOp - Operation mode.
     * @returns { boolean } boolean - Result of the path combination result. The value true means that the path combination is successful,
     * and false means the opposite.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    op(path: Path, pathOp: PathOp): boolean;

    /**
     * Adds an arc to this path.
     * 
     * When startAngle and sweepAngle meet the following conditions, an oval instead of an arc is added:
     * 
     * The result of startAngle modulo 90 is close to 0.
     * The value of sweepAngle is not in the range of (-360, 360).
     * 
     * In other cases, this API adds an arc by applying the result of sweepAngle modulo 360 to the path.
     * @param { common2D.Rect } rect - Rectangular boundary that encapsulates the oval including the arc.
     * @param { number } startAngle - Start angle of the arc, in degrees. The value 0 indicates the positive direction of the X axis.
     * The value is a floating point number.
     * @param { number } sweepAngle - Angle to sweep, in degrees. A positive number indicates a clockwise sweep,
     * and a negative number indicates a counterclockwise sweep. The value is a floating point number.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    addArc(rect: common2D.Rect, startAngle: number, sweepAngle: number): void;

    /**
     * Adds a circle to this path in the specified direction. The start point of the circle is (x + radius, y).
     * @param { number } x - X coordinate of the center of the circle. The value is a floating point number.
     * @param { number } y - Y coordinate of the center of the circle. The value is a floating point number.
     * @param { number } radius - Radius of the circle. The value is a floating point number.
     * If the value is less than or equal to 0, there is no effect.
     * @param { PathDirection } pathDirection - Direction of the path. The default direction is clockwise.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    addCircle(x: number, y: number, radius: number, pathDirection?: PathDirection): void;

    /**
     * Adds the inscribed ellipse of a rectangle to this path in the specified direction.
     * @param { common2D.Rect } rect - Rectangular boundary of the oval.
     * @param { number } start - Start point of the oval, where 0, 1, 2, and 3 correspond to the upper, right, lower, and left points, respectively.
     * The value is an integer greater than or equal to 0. If the value is greater than or equal to 4, the remainder of 4 is used.
     * @param { PathDirection } pathDirection - Direction of the path. The default direction is clockwise.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    addOval(rect: common2D.Rect, start: number, pathDirection?: PathDirection): void;

    /**
     * Adds a rectangle to this path in the specified direction. The start point is the upper left corner of the rectangle.
     * @param { common2D.Rect } rect - Rectangle.
     * @param { PathDirection } pathDirection - Direction of the path. The default direction is clockwise.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    addRect(rect: common2D.Rect, pathDirection?: PathDirection): void;

    /**
     * Adds a rounded rectangle to this path in the specified direction. When the path direction is clockwise,
     * the start point is at the intersection of the rounded rectangle's left boundary and its lower left corner.
     * When the path direction is counterclockwise, the start point is at the intersection point
     * between the left boundary and the upper left corner.
     * @param { RoundRect } roundRect - Rounded rectangle.
     * @param { PathDirection } pathDirection - The default value is CLOCKWISE.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    addRoundRect(roundRect: RoundRect, pathDirection?: PathDirection): void;

    /**
     * Transforms the points in a path by a matrix and stores the resulting path in the current Path object.
     * @param { Path } path - Source Path object.
     * @param { Matrix | null } matrix - Matrix object. The default value is an identity matrix.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    addPath(path: Path, matrix?: Matrix | null): void;

    /**
     * Transforms the points in this path by a matrix.
     * @param { Matrix } matrix - Matrix object.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    transform(matrix: Matrix): void;

    /**
     * Checks whether a coordinate point is included in this path. For details, see PathFillType.
     * @param { number } x - X coordinate. The value is a floating point number.
     * @param { number } y - Y coordinate. The value is a floating point number.
     * @returns { boolean } Check result. The value true means that the coordinate point is included in the path, and false means the opposite.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    contains(x: number, y: number): boolean;

    /**
     * Changes the last point of the path to specific value.
     * @param { number } x - Indicates the new x-axis value for the last point.
     * @param { number } y - Indicates the new y-axis value for the last point.
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    setLastPoint(x: number, y: number): void;

    /**
     * Sets the fill type of this path. The fill type determines how "inside" of the path is drawn.
     * For example, when the fill type Winding is used, "inside" of the path is determined by a non-zero sum of signed edge crossings.
     * When EvenOdd is used, "inside" of the path is determined by an odd number of edge crossings.
     * @param { PathFillType } pathFillType - Fill type of the path.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    setFillType(pathFillType: PathFillType): void;

    /**
     * Gets fill type, the rule used to fill path.
     * @returns { PathFillType } Returns the pathFillType.
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    getFillType(): PathFillType;

    /**
     * Obtains the minimum bounding rectangle that encloses this path.
     * @returns { common2D.Rect } Rect object.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    getBounds(): common2D.Rect;

    /**
     * Closes this path by adding a line segment from the start point to the last point of the path.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    close(): void;

    /**
     * Offsets this path by specified distances along the X axis and Y axis and stores the resulting path in the Path object returned.
     * @param { number } dx - X offset. A positive number indicates an offset towards the positive direction of the X axis,
     * and a negative number indicates an offset towards the negative direction of the X axis. The value is a floating point number.
     * @param { number } dy - Y offset. A positive number indicates an offset towards the positive direction of the Y axis,
     * and a negative number indicates an offset towards the negative direction of the Y axis. The value is a floating point number.
     * @returns { Path } New path generated.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    offset(dx: number, dy: number): Path;

    /**
     * Resets the path data.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    reset(): void;

    /**
     * Clears any lines and curves from the path but keeps the internal storage for faster reuse.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 20
     */
    rewind(): void;

    /**
     * Check if the path is empty (has no line or curve).
     *
     * @returns { boolean } Returns true if the path is empty; returns false otherwise.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 20
     */
    isEmpty(): boolean;

    /**
     * Check if the path represents a rectangle.
     *
     * @param { common2D.Rect | null } rect - Indicates the Rect object.
     * @returns { boolean } Returns true if the path represents a rectangle; returns false otherwise.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 20
     */
    isRect(rect: common2D.Rect | null): boolean;

    /**
     * Obtains the path length.
     * @param { boolean } forceClosed - Whether the path is measured as a closed path.
     * The value true means that the path is considered closed during measurement,
     * and false means that the path is measured based on the actual closed status.
     * @returns { number } Return path length.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    getLength(forceClosed: boolean): number;

    /**
     * Obtains the coordinates and tangent at a distance from the start point of this path.
     * 
     * @param { boolean } forceClosed - Whether the path is measured as a closed path.
     * The value true means that the path is considered closed during measurement,
     * and false means that the path is measured based on the actual closed status.
     * @param { number } distance - Distance from the start point. If a negative number is passed in, the value 0 is used.
     * If a value greater than the path length is passed in, the path length is used. The value is a floating point number.
     * @param { common2D.Point } position - Coordinates obtained.
     * @param { common2D.Point } tangent - Tangent obtained, where tangent.x and tangent.y represent the cosine
     * and sine of the tangent of the point, respectively.
     * @returns { boolean } - Check result. The value true means that they are obtained, and false means the opposite.
     * The values of position and tangent are not changed.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    getPositionAndTangent(forceClosed: boolean, distance: number, position: common2D.Point, tangent: common2D.Point): boolean;

    /**
     * Extracts a segment of this path and appends it to a destination path.
     *
     * @param { boolean } forceClosed - Whether the path is measured as a closed path. The value true means that the path is considered closed
     * during measurement, and false means that the path is measured based on the actual closed status.
     * @param { number } start - Distance from the start point of the path to the start point of the segment. If it is less than 0, it defaults to 0.
     * If it is greater than or equal to stop, the extraction fails. The value is a floating point number.
     * @param { number } stop - Distance from the start point of the path to the end point of the segment. If it is less than or equal to start,
     * the extraction fails. If it is greater than the path length, it defaults to the path length. The value is a floating point number.
     * @param { boolean } startWithMoveTo - Whether to execute moveto in the destination path to move to its start point.
     * The value true means to move to the start point, and false means the opposite.
     * @param { Path } dst - Destination path. If the extraction succeeds, the segment is appended to the path. If the extraction fails, nothing changes.
     * @returns { boolean } - Extraction result. The value **true** means that the extraction is successful, and **false** means the opposite.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    getSegment(forceClosed: boolean, start: number, stop: number, startWithMoveTo: boolean, dst: Path): boolean;

    /**
     * Checks whether a path is closed.
     * 
     * @returns { boolean } - Check result. The value true means that the path is closed, and false means the opposite.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    isClosed(): boolean;

    /**
     * Obtains a transformation matrix at a specific position along the path, which represents the coordinates and orientation of that point.
     * 
     * @param { boolean } forceClosed - Whether the path is measured as a closed path. The value true means that the path is considered closed
     * during measurement, and false means that the path is measured based on the actual closed status.
     * @param { number } distance - Distance from the start point. If a negative number is passed in, the value 0 is used.
     * If a value greater than the path length is passed in, the path length is used. The value is a floating point number.
     * @param { Matrix } matrix - Matrix object used to store the matrix obtained.
     * @param { PathMeasureMatrixFlags } flags - Type of the matrix information obtained.
     * @returns { boolean } - Check result. The value true means that a transformation matrix is obtained, and false means the opposite.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: Mandatory parameters are left unspecified.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
    */
    getMatrix(forceClosed: boolean, distance: number, matrix: Matrix, flags: PathMeasureMatrixFlags): boolean;

    /**
     * Parses the path represented by an SVG string.
     *
     * @param { string } str - String in SVG format, which is used to describe the path.
     * @returns { boolean } Check result. The value true means that the parsing is successful, and false means the opposite.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: Mandatory parameters are left unspecified.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    buildFromSvgString(str: string): boolean;

    /**
     * Obtains the operation iterator of this path.
     *
     * @returns { PathIterator } Indicates the pointer to an pathIterator object.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    getPathIterator(): PathIterator;
  }

  /**
   * Enumerates the modes for drawing multiple points in an array.
   * @enum { number }
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  enum PointMode {
    /**
     * Draws each point separately.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    POINTS = 0,

    /**
     * Draws every two points as a line segment.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    LINES = 1,

    /**
     * Draws an array of points as an open polygon.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    POLYGON = 2,
  }

  /**
   * Enumerates the filter modes.
   * @enum { number }
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  enum FilterMode {
    /**
     * Nearest filter mode.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    FILTER_MODE_NEAREST = 0,

    /**
     * Linear filter mode.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    FILTER_MODE_LINEAR = 1,
  }

  /**
   * Enumerates the shadow drawing behaviors.
   * @enum { number }
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  enum ShadowFlag {
    /**
     * None of the flags are enabled.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    NONE = 0,

    /**
     * The occluder is transparent.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    TRANSPARENT_OCCLUDER = 1,

    /**
     * Only the geometric shadow effect is used.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    GEOMETRIC_ONLY = 2,

    /**
     * All the flags are enabled.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    ALL = 3,
  }

  /**
   * Implements sampling options.
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  class SamplingOptions {
    /**
     * Creates a SamplingOptions object. The default value of FilterMode is FILTER_MODE_NEAREST.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    constructor();
    /**
     * Creates a SamplingOptions object.
     * @param { FilterMode } filterMode - Storage filter mode.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    constructor(filterMode: FilterMode);
  }

  /**
   * A carrier that carries the drawn content and drawing status.
   * @syscap SystemCapability.Graphics.Drawing
   * @since 11
   */
  class Canvas {
    /**
     * Creates a Canvas object that uses a PixelMap as the drawing target.
     * @param { image.PixelMap } pixelmap - PixelMap used to create the object.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    constructor(pixelmap: image.PixelMap);

    /**
     * Draws a rectangle. By default, black is used for filling.
     * @param { common2D.Rect } rect - Rectangle to draw.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    drawRect(rect: common2D.Rect): void;

    /**
     * Draws a rectangle. By default, black is used for filling. This API provides better performance than drawRect and is recommended.
     * @param { number } left - X coordinate of the upper left corner of the rectangle. The value is a floating point number.
     * @param { number } top - Y coordinate of the upper left corner of the rectangle. The value is a floating point number.
     * @param { number } right - X coordinate of the lower right corner of the rectangle. The value is a floating point number.
     * @param { number } bottom - Y coordinate of the lower right corner of the rectangle. The value is a floating point number.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    drawRect(left: number, top: number, right: number, bottom: number): void;

    /**
     * Draws a rounded rectangle.
     * @param { RoundRect } roundRect - Indicates the RectRound object.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    drawRoundRect(roundRect: RoundRect): void;

    /**
     * Draws two nested rounded rectangles. The outer rectangle boundary must contain the inner rectangle boundary.
     * Otherwise, there is no drawing effect.
     * @param { RoundRect } outer - Outer rounded rectangle.
     * @param { RoundRect } inner - Inner rounded rectangle.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    drawNestedRoundRect(outer: RoundRect, inner: RoundRect): void;

    /**
     * Uses a brush to fill the drawable area of the canvas.
     * @param { Brush } brush - Indicates the Brush object.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    drawBackground(brush: Brush): void;

    /**
     * Draws a spot shadow and uses a given path to outline the ambient shadow.
     * @param { Path } path - Path object, which is used to outline the shadow.
     * @param { common2D.Point3d } planeParams - 3D vector, which is used to determine the z-axis offset of an occluder relative to the canvas,
     * based on its x and y coordinates.
     * @param { common2D.Point3d } devLightPos - Position of the light relative to the canvas.
     * @param { number } lightRadius - Radius of the light. The value is a floating point number.
     * @param { common2D.Color } ambientColor - Color of the ambient shadow.
     * @param { common2D.Color } spotColor - Color of the spot shadow.
     * @param { ShadowFlag } flag - Shadow flag.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    drawShadow(path: Path, planeParams: common2D.Point3d, devLightPos: common2D.Point3d, lightRadius: number,
      ambientColor: common2D.Color, spotColor: common2D.Color, flag: ShadowFlag) : void;

    /**
     * Draws a spot shadow and uses a given path to outline the ambient shadow.
     * @param { Path } path - Path object, which is used to outline the shadow.
     * @param { common2D.Point3d } planeParams - 3D vector, which is used to calculate the offset in the Z axis.
     * @param { common2D.Point3d } devLightPos - Position of the light relative to the canvas.
     * @param { number } lightRadius - Radius of the light. The value is a floating point number.
     * @param { common2D.Color | number } ambientColor - Ambient shadow color, represented by a 32-bit unsigned integer in hexadecimal ARGB format.
     * @param { common2D.Color | number } spotColor - Spot shadow color, represented by a 32-bit unsigned integer in hexadecimal ARGB format.
     * @param { ShadowFlag } flag - Indicates the flag to control opaque occluder, shadow, and light position.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    drawShadow(path: Path, planeParams: common2D.Point3d, devLightPos: common2D.Point3d, lightRadius: number,
      ambientColor: common2D.Color | number, spotColor: common2D.Color | number, flag: ShadowFlag) : void;

    /**
     * Draws a circle. If the radius is less than or equal to zero, nothing is drawn. By default, black is used for filling.
     * @param { number } x - X coordinate of the center of the circle. The value is a floating point number.
     * @param { number } y - Y coordinate of the center of the circle. The value is a floating point number.
     * @param { number } radius - Radius of the circle. The value is a floating point number greater than 0.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    drawCircle(x: number, y: number, radius: number): void;

    /**
     * Draw a pixelmap, with the upper left corner at (left, top).
     * @param { image.PixelMap } pixelmap - PixelMap.
     * @param { number } left - Left side of image.
     * @param { number } top - Top side of image.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    /**
     * Draws an image. The coordinates of the upper left corner of the image are (left, top).
     * @param { image.PixelMap } pixelmap - PixelMap.
     * @param { number } left - X coordinate of the upper left corner of the image. The value is a floating point number.
     * @param { number } top - Y coordinate of the upper left corner of the image. The value is a floating point number.
     * @param { SamplingOptions } samplingOptions - Sampling options. By default, the SamplingOptions object created using the no-argument constructor is used.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    drawImage(pixelmap: image.PixelMap, left: number, top: number, samplingOptions?: SamplingOptions): void;
    
    /**
     * Splits an image into multiple sections based on the lattice object's configuration and
     * draws each section into the specified target rectangle on the canvas.
     * The intersections of even-numbered rows and columns (starting from 0) are fixed points.
     * If the fixed lattice area fits within the target rectangle, it will be drawn without scaling.
     * Otherwise, it will be scaled proportionally to fit the target rectangle.
     * Any remaining space will be filled by stretching or compressing the remaining sections to cover the entire target rectangle.
     * @param { image.PixelMap } pixelmap - The source image.
     * @param { Lattice } lattice - The area of source image.
     * @param { common2D.Rect } dstRect - The area of destination canvas.
     * @param { FilterMode } filterMode - Filter mode.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    drawImageLattice(pixelmap: image.PixelMap, lattice: Lattice, dstRect: common2D.Rect,
      filterMode: FilterMode): void;
    
    /**
     * Splits an image into nine sections using two horizontal and two vertical lines: four edge sections, four corner sections, and a central section.
     * If the four corner sections are smaller than the target rectangle, they will be drawn in the target rectangle without scaling.
     * Otherwise, they will be scaled to fit the target rectangle. Any remaining space will be filled by stretching or
     * compressing the other five sections to cover the entire target rectangle.
     * @param { image.PixelMap } pixelmap - PixelMap to split.
     * @param { common2D.Rect } center - Central rectangle that divides the image into nine sections by extending its four edges.
     * @param { common2D.Rect } dstRect - Target rectangle drawn on the canvas.
     * @param { FilterMode } filterMode - Filter mode.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    drawImageNine(pixelmap: image.PixelMap, center: common2D.Rect, dstRect: common2D.Rect,
      filterMode: FilterMode): void;

    /**
     * Draws an image onto a specified area of the canvas.
     * @param { image.PixelMap } pixelmap - The source image.
     * @param { common2D.Rect } dstRect - Rectangle object, which specifies the area of the canvas onto which the image will be drawn.
     * @param { SamplingOptions } samplingOptions - Sampling options.
     * By default, the SamplingOptions object created using the no-argument constructor is used.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    drawImageRect(pixelmap: image.PixelMap, dstRect: common2D.Rect, samplingOptions?: SamplingOptions): void;

    /**
     * Draws a portion of an image onto a specified area of the canvas.
     * @param { image.PixelMap } pixelmap - The source image.
     * @param { common2D.Rect } srcRect - Rectangle object, which specifies the portion of the image to draw.
     * @param { common2D.Rect } dstRect - Rectangle object, which specifies the area of the canvas onto which the image will be drawn.
     * @param { SamplingOptions } samplingOptions - Sampling options.
     * By default, the SamplingOptions object created using the no-argument constructor is used.
     * @param { SrcRectConstraint } constraint - Constraint type of the source rectangle. The default value is STRICT.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    drawImageRectWithSrc(pixelmap: image.PixelMap, srcRect: common2D.Rect, dstRect: common2D.Rect,
      samplingOptions?: SamplingOptions, constraint?: SrcRectConstraint): void;

    /**
     * Draws the background color.
     * @param { common2D.Color } color - The range of color channels must be [0, 255].
     * @param { BlendMode } blendMode - Used to combine source color and destination. The default value is SRC_OVER.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    drawColor(color: common2D.Color, blendMode?: BlendMode): void;

    /**
     * Fills the drawable area of the canvas with the specified color and blend mode. This API provides better performance and is recommended.
     * @param { number } alpha - Alpha channel value of the color in ARGB format.
     * The value is an integer ranging from 0 to 255. Any passed-in floating point number is rounded down.
     * @param { number } red - Red channel value of the color in ARGB format.
     * The value is an integer ranging from 0 to 255. Any passed-in floating point number is rounded down.
     * @param { number } green - Green channel value of the color in ARGB format.
     * The value is an integer ranging from 0 to 255. Any passed-in floating point number is rounded down.
     * @param { number } blue - Blue channel value of the color in ARGB format.
     * The value is an integer ranging from 0 to 255. Any passed-in floating point number is rounded down.
     * @param { BlendMode } blendMode - Blend mode. The default mode is SRC_OVER.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    drawColor(alpha: number, red: number, green: number, blue: number, blendMode?: BlendMode): void;

    /**
     * Fills the drawable area of the canvas with the specified color and blend mode.
     * @param { number } color - Color in hexadecimal ARGB format.
     * @param { BlendMode } blendMode - Blend mode. The default mode is SRC_OVER.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    drawColor(color: number, blendMode?: BlendMode): void;

    /**
     * Draws an oval on the canvas, where the shape and position of the oval are defined by its bounding rectangle.
     * @param { common2D.Rect } oval - Rectangle. The oval inscribed within the rectangle is the oval to draw.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    drawOval(oval: common2D.Rect): void;

    /**
     * Draws an arc on the canvas, with the start angle and sweep angle specified.
     * If the absolute value of the sweep angle exceeds 360 degrees, an ellipse is drawn.
     * @param { common2D.Rect } arc - Rectangular boundary that encapsulates the oval including the arc.
     * @param { number } startAngle - Start angle, in degrees. The value is a floating point number.
     * When the degree is 0, the start point is located at the right end of the oval.
     * A positive number indicates that the start point is placed clockwise,
     * and a negative number indicates that the start point is placed counterclockwise.
     * @param { number } sweepAngle - Angle to sweep, in degrees. The value is a floating point number.
     * A positive number indicates a clockwise sweep, and a negative value indicates a counterclockwise swipe.
     * The valid range is from -360 degrees to 360 degrees. If the absolute value of the sweep angle exceeds 360 degrees,
     * an ellipse is drawn.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    drawArc(arc: common2D.Rect, startAngle: number, sweepAngle: number): void;

    /**
     * Draws an arc on the canvas. It enables you to define the start angle, sweep angle,
     * and whether the arc's endpoints should connect to its center.
     * @param { common2D.Rect } arc - Rectangular boundary that encapsulates the oval including the arc.
     * @param { number } startAngle - Start angle, in degrees. The value is a floating point number.
     * When the degree is 0, the start point is located at the right end of the oval.
     * A positive number indicates that the start point is placed clockwise,
     * and a negative number indicates that the start point is placed counterclockwise.
     * @param { number } sweepAngle - Angle to sweep, in degrees. The value is a floating point number.
     * A positive number indicates a clockwise sweep, and a negative value indicates a counterclockwise swipe.
     * The swipe angle can exceed 360 degrees, and a complete ellipse is drawn.
     * @param { boolean } useCenter - Whether the start point and end point of the arc are connected to its center.
     * The value true means that they are connected to the center; the value false means the opposite.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    drawArcWithCenter(arc: common2D.Rect, startAngle: number, sweepAngle: number, useCenter: boolean): void;

    /**
     * Draw a point.
     * @param { number } x - X coordinate of the point. The value is a floating point number.
     * @param { number } y - Y coordinate of the point. The value is a floating point number.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    drawPoint(x: number, y: number): void;

    /**
     * Draws a group of points, line segments, or polygons on the canvas, with the specified drawing mode. An array is used to hold these points.
     * @param { Array<common2D.Point> } points - Array that holds the points to draw. The length cannot be 0.
     * @param { PointMode } mode - Mode in which the points are drawn. The default value is drawing.PointMode.POINTS.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    drawPoints(points: Array<common2D.Point>, mode?: PointMode): void;

    /**
     * Draws a custom path, which contains a set of path outlines. Each path outline can be open or closed.
     * @param { Path } path - Path object to draw.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    drawPath(path: Path): void;

    /**
     * Draws a line segment from the start point to the end point. If the coordinates of the start point are the same as those of the end point,
     * nothing is drawn.
     * @param { number } x0 - X coordinate of the start point of the line segment. The value is a floating point number.
     * @param { number } y0 - Y coordinate of the start point of the line segment. The value is a floating point number.
     * @param { number } x1 - X coordinate of the end point of the line segment. The value is a floating point number.
     * @param { number } y1 - Y coordinate of the end point of the line segment. The value is a floating point number.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    drawLine(x0: number, y0: number, x1: number, y1: number): void;

    /**
     * Draws a single character. If the typeface of the current font does not support the character to draw,
     * the system typeface is used to draw the character.
     * @param { string } text - Single character to draw. The length of the string must be 1.
     * @param { Font } font - Font object.
     * @param { number } x - X coordinate of the left point (red point in the figure below) of the character baseline (blue line in the figure below).
     * The value is a floating point number.
     * @param { number } y - Y coordinate of the left point (red point in the figure below) of the character baseline (blue line in the figure below).
     * The value is a floating point number.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    drawSingleCharacter(text: string, font: Font, x: number, y: number): void;

    /**
     * Draws a text blob. If the typeface used to construct blob does not support a character, that character will not be drawn.
     * @param { TextBlob } blob - TextBlob to draw.
     * @param { number } x - X coordinate of the left point (red point in the figure below) of the text baseline (blue line in the figure below).
     * The value is a floating point number.
     * @param { number } y - Y coordinate of the left point (red point in the figure below) of the text baseline (blue line in the figure below).
     * The value is a floating point number.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    drawTextBlob(blob: TextBlob, x: number, y: number): void;

    /**
     * Draws a PixelMap based on a mesh, where mesh vertices are evenly distributed across the PixelMap.
     * @param { image.PixelMap } pixelmap - PixelMap to draw.
     * @param { number } meshWidth - Number of columns in the mesh. The value is an integer greater than 0.
     * @param { number } meshHeight - Number of rows in the mesh. The value is an integer greater than 0.
     * @param { Array<number> } vertices - Array of vertices, which specify the position to draw.
     * The value is a floating-point array and the size must be ((meshWidth+1) * (meshHeight+1) + vertOffset) * 2.
     * @param { number } vertOffset - Number of vert elements to skip before drawing. The value is an integer greater than or equal to 0.
     * @param { Array<number> } colors - Array of colors, which specify the color at each vertex.
     * The value is an integer array and can be null. The size must be (meshWidth+1) * (meshHeight+1) + colorOffset.
     * @param { number } colorOffset - Number of color elements to skip before drawing. The value is an integer greater than or equal to 0.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    drawPixelMapMesh(pixelmap: image.PixelMap, meshWidth: number, meshHeight: number,
      vertices: Array<number>, vertOffset: number, colors: Array<number>, colorOffset: number): void;

    /**
     * Draws a region.
     * @param { Region } region - Region to draw.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    drawRegion(region: Region): void;

    /**
     * Attaches a pen to the canvas. When you draw on the canvas, the pen's style is used to outline shapes.
     * @param { Pen } pen - Pen object.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    attachPen(pen: Pen): void;

    /**
     * Attaches a brush to the canvas. When you draw on the canvas, the brush's style is used to fill the interior of shapes.
     * @param { Brush } brush - Brush object.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    attachBrush(brush: Brush): void;

    /**
     * Detaches the pen from the canvas. When you draw on the canvas, the pen is no longer used to outline shapes.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    detachPen(): void;

    /**
     * Detaches the brush from the canvas. When you draw on the canvas, the brush is no longer used to fill the interior of shapes.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    detachBrush(): void;

    /**
     * Saves the canvas states (canvas matrix and drawable area) to the top of the stack. This API must be used in pair with restore.
     * @returns { number } Number of canvas statuses. The value is a positive integer.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    save(): number;

    /**
     * Saves the matrix and cropping region of the canvas, and allocates a PixelMap for subsequent drawing.
     * If you call restore, changes made to the matrix and clipping region are discarded, and the PixelMap is drawn.
     * @param { common2D.Rect | null} rect - Rect object, which is used to limit the size of the graphics layer.
     * The default value is the current canvas size.
     * @param { Brush | null} brush - Brush object. The alpha value, filter effect, and blend mode of the brush are applied when the bitmap is drawn.
     * If null is passed in, no effect is applied.
     * @returns { number } Number of canvas statuses that have been saved. The value is a positive integer.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: Mandatory parameters are left unspecified.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    saveLayer(rect?: common2D.Rect | null, brush?: Brush | null): number;

    /**
     * Clears the canvas with a given color. This API has the same effect as drawcolor.
     * @param { common2D.Color } color - Color in ARGB format. Each color channel is an integer ranging from 0 to 255.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    clear(color: common2D.Color): void;

    /**
     * Clears the canvas with a given color.
     * @param { common2D.Color | number } color - Color, represented by an unsigned integer in hexadecimal ARGB format.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    clear(color: common2D.Color | number): void;

    /**
     * Restores the canvas state (canvas matrix and clipping area) saved on the top of the stack.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    restore(): void;

    /**
     * Restores the canvas state (canvas matrix and clipping area) to a specified number.
     * @param { number } count - Depth of the canvas statuses to restore. The value is an integer.
     * If the value is less than or equal to 1, the canvas is restored to the initial state.
     * If the value is greater than the number of canvas statuses that have been saved, no operation is performed.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    restoreToCount(count: number): void;

    /**
     * Obtains the number of canvas states (canvas matrix and clipping area) saved in the stack.
     * @returns { number } Number of canvas statuses that have been saved. The value is a positive integer.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    getSaveCount(): number;

    /**
     * Obtains the canvas width.
     * @returns { number } Canvas width. The value is a floating point number.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    getWidth(): number;

    /**
     * Obtains the canvas height.
     * @returns { number } Canvas height. The value is a floating point number.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    getHeight(): number;

    /**
     * Obtains the bounds of the cropping region of the canvas.
     * @returns { common2D.Rect } Rect object.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    getLocalClipBounds(): common2D.Rect;

    /**
     * Obtains the canvas matrix.
     * @returns { Matrix } Canvas matrix.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    getTotalMatrix(): Matrix;

    /**
     * Applies a scaling matrix on top of the current canvas matrix (identity matrix by default).
     * Subsequent drawing and clipping operations will automatically have a scaling effect applied to the shapes and positions.
     * @param { number } sx - Scale ratio on the X axis. The value is a floating point number.
     * @param { number } sy - Scale ratio on the Y axis. The value is a floating point number.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    scale(sx: number, sy: number): void;

    /**
     * Applies a skewing matrix on top of the current canvas matrix (identity matrix by default).
     * Subsequent drawing and clipping operations will automatically have a skewing effect applied to the shapes and positions.
     * @param { number } sx - Amount of tilt on the X axis. The value is a floating point number.
     * A positive number tilts the drawing rightwards along the positive direction of the Y axis,
     * and a negative number tilts the drawing leftwards along the positive direction of the Y axis.
     * @param { number } sy - Amount of tilt on the Y axis. The value is a floating point number.
     * A positive number tilts the drawing downwards along the positive direction of the X axis,
     * and a negative number tilts the drawing upwards along the positive direction of the X axis.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    skew(sx: number, sy: number) : void;

    /**
     * Applies a rotation matrix on top of the current canvas matrix (identity matrix by default).
     * Subsequent drawing and clipping operations will automatically have a rotation effect applied to their shapes and positions.
     * @param { number } degrees - Angle to rotate, in degrees. The value is a floating point number.
     * A positive value indicates a clockwise rotation, and a negative value indicates a counterclockwise rotation.
     * @param { number } sx - X coordinate of the rotation center. The value is a floating point number.
     * @param { number } sy - Y coordinate of the rotation center. The value is a floating point number.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    rotate(degrees: number, sx: number, sy: number) : void;

    /**
     * Applies a translation matrix on top of the current canvas matrix (identity matrix by default).
     * Subsequent drawing and clipping operations will automatically have a translation effect applied to the shapes and positions.
     * @param { number } dx - Distance to translate on the X axis. The value is a floating point number.
     * @param { number } dy - Distance to translate on the Y axis. The value is a floating point number.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    translate(dx: number, dy: number): void;

    /**
     * Clips the drawable area of the canvas using a custom path.
     * @param { Path } path - To combine with clip.
     * @param { ClipOp } clipOp - Clip mode. The default value is INTERSECT.
     * @param { boolean } doAntiAlias - Whether to enable anti-aliasing. The value true means to enable anti-aliasing,
     * and false means the opposite. The default value is false.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    clipPath(path: Path, clipOp?: ClipOp, doAntiAlias?: boolean): void;

    /**
     * Clips the drawable area of the canvas using a rectangle.
     * @param { common2D.Rect } rect - To combine with clipping area.
     * @param { ClipOp } clipOp - Clip mode. The default value is INTERSECT.
     * @param { boolean } doAntiAlias - Whether to enable anti-aliasing. The value true means to enable anti-aliasing,
     * and false means the opposite. The default value is false.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    clipRect(rect: common2D.Rect, clipOp?: ClipOp, doAntiAlias?: boolean): void;

    /**
     * Multiplies the current canvas matrix by the incoming matrix on the left. This API does not affect previous drawing operations,
     * but subsequent drawing and clipping operations will be influenced by this matrix in terms of shape and position.
     * @param { Matrix } matrix - Declares functions related to the matrix object in the drawing module.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    concatMatrix(matrix: Matrix): void;

    /**
     * Clips a region on the canvas.
     * @param { Region } region - Region object, which indicates the range to clip.
     * @param { ClipOp } clipOp - Clipping mode. The default value is INTERSECT.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    clipRegion(region: Region, clipOp?: ClipOp): void;

    /**
     * Clips a rounded rectangle on the canvas.
     * @param { RoundRect } roundRect - To combine with clipping area.
     * @param { ClipOp } clipOp - Clipping mode. The default value is INTERSECT.
     * @param { boolean } doAntiAlias - Whether to enable anti-aliasing. The value true means to enable anti-aliasing,
     * and false means the opposite. The default value is false.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    clipRoundRect(roundRect: RoundRect, clipOp?: ClipOp, doAntiAlias?: boolean): void;

    /**
     * Checks whether the region that can be drawn is empty after clipping.
     * @returns { boolean } Returns true if drawable area is empty.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    isClipEmpty(): boolean;

    /**
     * Sets a matrix for the canvas. Subsequent drawing and clipping operations will be affected by this matrix in terms of shape and position.
     * @param { Matrix } matrix - Declares functions related to the matrix object in the drawing module.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    setMatrix(matrix: Matrix): void;

    /**
     * Resets the matrix of this canvas to an identity matrix.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    resetMatrix(): void;

    /**
     * Checks whether the path is not intersecting with the canvas area. The canvas area includes its boundaries.
     * @param { Path } path - Path to draw.
     * @returns { boolean } Returns true if path is not intersect; returns false otherwise.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    quickRejectPath(path: Path): boolean;

    /**
     * Checks whether the rectangle is not intersecting with the canvas area. The canvas area includes its boundaries.
     * @param { common2D.Rect } rect - Rectangle to determines.
     * @returns { boolean } Returns true if rect and region is not intersect; returns false otherwise.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    quickRejectRect(rect: common2D.Rect): boolean;
  }

  /**
   * Enumerates the canvas clipping modes.
   *
   * @enum { number }
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  enum ClipOp {
    /**
     * Clips a specified area. That is, the difference set is obtained.	
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    DIFFERENCE = 0,
    /**
     * Retains a specified area. That is, the intersection is obtained.	
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    INTERSECT = 1,
  }

  /**
   * Describes a series of consecutive glyphs with the same attributes in a text blob.
   * @typedef TextBlobRunBuffer
   * @syscap SystemCapability.Graphics.Drawing
   * @since 11
   */
  interface TextBlobRunBuffer {
    /**
     * Index of the glyph. The value is an integer. If a floating point number is passed in, the value is rounded down.
     * @type { number }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    glyph: number;
    /**
     * X coordinate of the start point of the text blob. The value is a floating point number.
     * @type { number }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    positionX: number;
    /**
     * Y coordinate of the start point of the text blob. The value is a floating point number.
     * @type { number }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    positionY: number;
  }

  /**
   * Enumerates the text encoding types.
   *
   * @enum { number }
   * @syscap SystemCapability.Graphics.Drawing
   * @since 11
   */
  enum TextEncoding {
    /**
     * One byte is used to indicate UTF-8 or ASCII characters.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    TEXT_ENCODING_UTF8 = 0,
    /**
     * Two bytes are used to indicate most Unicode characters.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    TEXT_ENCODING_UTF16 = 1,
    /**
     * Four bytes are used to indicate all Unicode characters.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    TEXT_ENCODING_UTF32 = 2,
    /**
     * Two bytes are used to indicate the glyph index.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    TEXT_ENCODING_GLYPH_ID = 3,
  }

  /**
   * Defines a block consisting of one or more characters with the same font.
   *
   * class TextBlob
   * @syscap SystemCapability.Graphics.Drawing
   * @since 11
   */
  class TextBlob {
    /**
     * Converts a value of the string type into a TextBlob object.
     * @param { string } text - Content to be used for drawing the text blob.
     * @param { Font } font - Specify text size, font, text scale, etc.
     * @param { TextEncoding } encoding - Encoding type. The default value is TEXT_ENCODING_UTF8.
     * Currently, only TEXT_ENCODING_UTF8 takes effect, and other encoding types are treated as TEXT_ENCODING_UTF8.
     * @returns { TextBlob } TextBlob object.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @static
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    static makeFromString(text: string, font: Font, encoding?: TextEncoding): TextBlob;

    /**
     * Creates a TextBlob object from the text.
     * The coordinates of each font in the TextBlob object are determined by the coordinate information in the points array.
     * @param { string } text - Content to be used for drawing the text blob.
     * @param { number } len - Number of fonts. The value is an integer and is obtained from countText.
     * @param { common2D.Point[] } points - Array of points, which are used to specify the coordinates of each font.
     * The array length must be the same as the value of len.
     * @param { Font } font - Specify text size, font, text scale, etc.
     * @returns { TextBlob } TextBlob object.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @static
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    static makeFromPosText(text: string, len: number, points: common2D.Point[], font: Font): TextBlob;

    /**
     * Creates a Textblob object based on the RunBuffer information.
     * @param { Array<TextBlobRunBuffer> } pos - The array of TextBlobRunBuffer.
     * @param { Font } font - Font used for this run.
     * @param { common2D.Rect } bounds - Optional run bounding box. The default value is null;
     * @returns { TextBlob } TextBlob object.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @static
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    static makeFromRunBuffer(pos: Array<TextBlobRunBuffer>, font: Font, bounds?: common2D.Rect): TextBlob;

    /**
     * Obtains the rectangular bounding box of the text blob.
     * @returns { common2D.Rect } Rect object.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    bounds(): common2D.Rect;

    /**
     * Obtains the unique, non-zero identifier of this TextBlob object.
     * @returns { number } Unique ID.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    uniqueID(): number;
  }

  /**
   * Provides an interface to the drawing, and describe the arguments for a font.
   * @syscap SystemCapability.Graphics.Drawing
   * @crossplatform
   * @since 20
   */
  class TypefaceArguments {
    /**
     * Constructor for the TypefaceArguments.
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    constructor();
    /**
     * Adds variation axis for the TypefaceArguments.
     * @param { string } axis  - Indicates the axis tag, which must contain four ASCII characters.
     * @param { number } value  - Indicates the value of the axis field.
     * @throws { BusinessError } 25900001 - Parameter error. Possible causes: Incorrect parameter range.
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    addVariation(axis: string, value: number);
  }

  /**
   * Describes the style of a typeface, such as SimSun or KaiTi.
   *
   * @syscap SystemCapability.Graphics.Drawing
   * @since 11
   */
  class Typeface {
    /**
     * Obtains the name of the typeface family, which is the name given to a collection of related typeface designs.
     * @returns { string } Family name.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    getFamilyName(): string;

    /**
     * Generate typeface from current typeface and TypefaceArguments.
     * @param { TypefaceArguments } typefaceArguments - TypefaceArguments for typeface.
     * @returns { Typeface } Typeface.
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    makeFromCurrent(typefaceArguments: TypefaceArguments): Typeface;

    /**
     * Constructs a typeface from a file.
     * @param { string } filePath - file path for typeface.
     * @returns { Typeface } Typeface.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
     static makeFromFile(filePath: string): Typeface;

    /**
     * Constructs a typeface from a file, which must be stored in the resources/rawfile directory of the application project.
     * @param { Resource } rawfile - Resource object corresponding to the file.
     * Currently, only resource objects referenced in rawfile format are supported.
     * The corresponding format is rawfile('filePath'), where filePath is the relative path of the file to the resources/rawfile directory in the project.
     * If the file is stored in resources/rawfile, the reference format is rawfile('HarmonyOS_Sans_Bold.ttf').
     * If the file is stored in a subdirectory, for example, in resources/rawfile/ttf, the reference format is rawfile('ttf/HarmonyOS_Sans_Bold.ttf').
     * @returns { Typeface } Typeface.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    static makeFromRawFile(rawfile: Resource): Typeface;

    /**
     * Generate typeface from file and TypefaceArguments.
     * @param { string } filePath - file path for typeface.
     * @param { TypefaceArguments } typefaceArguments - TypefaceArguments for typeface.
     * @returns { Typeface } Typeface.
     * @static
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    static makeFromFileWithArguments(filePath: string, typefaceArguments: TypefaceArguments): Typeface;

    /**
     * Generate typeface from Rawfile and TypefaceArguments.
     * @param { Resource } rawfile - RawFile for typeface.
     * @param { TypefaceArguments } typefaceArguments - TypefaceArguments for typeface.
     * @returns { Typeface } Typeface.
     * @static
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    static makeFromRawFileWithArguments(rawfile: Resource, typefaceArguments: TypefaceArguments): Typeface;
  }

  /**
   * Enumerates the font edging types.
   *
   * @enum { number }
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  enum FontEdging {
    /**
     * No anti-aliasing processing is used.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    ALIAS = 0,

    /**
     * Uses anti-aliasing to smooth the jagged edges.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    ANTI_ALIAS = 1,

    /**
     * Uses sub-pixel anti-aliasing to provide a smoother effect for jagged edges.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    SUBPIXEL_ANTI_ALIAS = 2,
  }

  /**
   * Enumerates the font hinting types.
   *
   * @enum { number }
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  enum FontHinting {
    /**
     * No font hinting is used.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    NONE = 0,

    /**
     * Slight font hinting is used to improve contrast.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    SLIGHT = 1,

    /**
     * Normal font hinting is used to improve contrast.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    NORMAL = 2,

    /**
     * Full font hinting is used to improve contrast.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    FULL = 3,
  }

  /**
   * Describes the attributes, such as the size, used for drawing text.
   *
   * @syscap SystemCapability.Graphics.Drawing
   * @since 11
   */
  class Font {
    /**
     * Enables subpixel font rendering.
     * @param { boolean } isSubpixel - Whether to enable subpixel font rendering.
     * The value true means to enable subpixel font rendering, and false means the opposite.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    enableSubpixel(isSubpixel: boolean): void;

    /**
     * Enables emboldened fonts.
     * @param { boolean } isEmbolden - Whether to enable emboldened fonts.
     * The value true means to enable emboldened fonts, and false means the opposite.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    enableEmbolden(isEmbolden: boolean): void;

    /**
     * Enables linear font scaling.
     * @param { boolean } isLinearMetrics - Whether to enable linear font scaling.
     * The value true means to enable linear font scaling, and false means the opposite.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    enableLinearMetrics(isLinearMetrics: boolean): void;

    /**
     * Sets the text size.
     * @param { number } textSize - Text size. The value is a floating point number.
     * If a negative number is passed in, the size is set to 0. If the size is 0, the text drawn will not be displayed.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    setSize(textSize: number): void;

    /**
     * Obtains the text size.
     * @returns { number } Text size. The value is a floating point number.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    getSize(): number;

    /**
     * Sets the typeface style (including attributes such as font name, weight, and italic) for the font.
     * @param { Typeface } typeface - Font and style used to draw text.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    setTypeface(typeface: Typeface): void;

    /**
     * Obtains the typeface.
     * @returns { Typeface } Typeface object.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    getTypeface(): Typeface;

    /**
     * Obtains the font metrics of the typeface.
     * @returns { FontMetrics } The fontMetrics value returned to the caller.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    getMetrics(): FontMetrics;

    /**
     * Measures the width of a single character.
     * If the typeface of the current font does not support the character to measure, the system typeface is used to measure the character width.
     * @param { string } text - Single character to measure. The length of the string must be 1.
     * @returns { number } Width of the character. The value is a floating point number.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    measureSingleCharacter(text: string): number;

    /**
     * Measures the text width.
     * @param { string } text - Text Symbol Content.
     * @param { TextEncoding } encoding - Encoding format.
     * @returns { number } Width of the text. The value is a floating point number.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    measureText(text: string, encoding: TextEncoding): number;

    /**
     * Sets a horizontal scale factor for this font.
     * @param { number } scaleX - Horizontal scale factor. The value is a floating point number.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    setScaleX(scaleX: number): void;

    /**
     * Sets a horizontal skew factor for this font.
     * @param { number } skewX - Horizontal skew factor.
     * A positive number means a skew to the left, and a negative number means a skew to the right. The value is a floating point number.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    setSkewX(skewX: number): void;

    /**
     * Sets a font edging effect.
     * @param { FontEdging } edging - Font edging effect.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    setEdging(edging: FontEdging): void;

    /**
     * Sets a font hinting effect.
     * @param { FontHinting } hinting - Font hinting effect.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    setHinting(hinting: FontHinting): void;

    /**
     * Obtains the number of glyphs represented by text.
     * @param { string } text - Indicates the character storage encoded with text encoding.
     * @returns { number } Returns the count of text.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    countText(text: string): number;

    /**
     * Sets whether to request that baselines be snapped to pixels when the current canvas matrix is axis aligned.
     * @param { boolean } isBaselineSnap - Whether to request that baselines be snapped to pixels.
     * The value true means to request that baselines be snapped to pixels, and false means the opposite.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    setBaselineSnap(isBaselineSnap: boolean): void;

    /**
     * Checks whether baselines are requested to be snapped to pixels when the current canvas matrix is axis aligned.
     * @returns { boolean } Check result. The value true means that the baselines are requested to be snapped to pixels,
     * and false means the opposite.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    isBaselineSnap(): boolean;

    /**
     * Sets whether to use bitmaps in this font.
     * @param { boolean } isEmbeddedBitmaps - Whether to use bitmaps in the font. The value true means to use bitmaps in the font,
     * and false means the opposite.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    setEmbeddedBitmaps(isEmbeddedBitmaps: boolean): void;

    /**
     * Checks whether bitmaps are used in this font.
     * @returns { boolean } Check result. The value true means that the bitmaps are used, and false means the opposite.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    isEmbeddedBitmaps(): boolean;

    /**
     * Sets whether to forcibly use auto hinting, that is, whether to always hint glyphs.
     * @param { boolean } isForceAutoHinting - Whether to forcibly use auto hinting. The value true means to forcibly use auto hinting,
     * and false means the opposite.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    setForceAutoHinting(isForceAutoHinting: boolean): void;

    /**
     * Checks whether auto hinting is forcibly used.
     * @returns { boolean } Check result. The value true means that auto hinting is forcibly used, and false means the opposite.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    isForceAutoHinting(): boolean;

    /**
     * Obtains the width of each glyph in an array.
     * @param { Array<number> } glyphs - Glyph array, which can be generated by textToGlyphs.
     * @returns { Array<number> } Glyph array, which can be generated by textToGlyphs.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    getWidths(glyphs: Array<number>): Array<number>;

    /**
     * Converts text into glyph indexes.
     * @param { string } text - Text string.
     * @param { number } glyphCount - Number of glyphs represented by the text. The value must be the same as the value obtained from countText.
     * The default value is the number of characters in the text string. The value is an integer.
     * @returns { Array<number> } Returns the storage for glyph indices.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    textToGlyphs(text: string, glyphCount?: number): Array<number>;

    /**
     * Checks whether sub-pixel rendering is used for this font.
     * @returns { boolean } Check result. The value true means that sub-pixel rendering is used, and false means the opposite.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    isSubpixel(): boolean;

    /**
     * Checks whether linear scaling is used for this font.
     * @returns { boolean } Checks whether linear scaling is used for this font.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    isLinearMetrics(): boolean;
    
    /**
     * Obtains the horizontal skew factor of this font.
     * @returns { number } Horizontal skew factor.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    getSkewX(): number;

    /**
     * Checks whether the bold effect is set for this font.
     * @returns { boolean } Check result. The value true means that the bold effect is set, and false means the opposite.
     * returns false otherwise.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    isEmbolden(): boolean;

    /**
     * Obtains the horizontal scale ratio of this font.
     * @returns { number } Horizontal scale ratio.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    getScaleX(): number;

    /**
     * Obtains the font hinting effect.
     * @returns { FontHinting } Font hinting effect.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    getHinting(): FontHinting;

    /**
     * Obtains the font edging effect.
     * @returns { FontEdging } Font edging effect.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    getEdging(): FontEdging;

    /**
     * Obtains the outline path of a glyph.
     * @param { number } index - Index of the glyph.
     * @returns { Path } Outline path of the glyph.
     * Note: Path use y-axis-goes-down system, y axis is inverted to the y-axis-goes-up system.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    createPathForGlyph(index: number): Path;

    /**
     * Obtains the rectangular bounding box of each glyph in an array.
     * @param { Array<number> } glyphs - Glyph array, which can be generated by textToGlyphs.
     * @returns { Array<common2D.Rect> } Array that holds the rectangular bounding boxes.
     * Note: 1. Rect use y-axis-goes-down system, y axis is inverted to the y-axis-goes-up system.
     * <br>2. Rect use two points(left-bottom & right-top) to describe the bound.
     * <br>3. The bound rect will be snap to integral boundaries.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    getBounds(glyphs: Array<number>): Array<common2D.Rect>;

    /**
     * Obtains the outline path of a text.
     * @param { string } text - UTF-8 text-encoded characters.
     * @param { number } byteLength - Length of the outline path,
     * which is obtained based on the minimum value between the passed value of byteLength and the actual text byte size.
     * @param { number } x - X coordinate of the text in the drawing area, with the origin as the start point.
     * @param { number } y - Y coordinate of the text in the drawing area, with the origin as the start point.
     * @returns { Path } Outline path of the text.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    getTextPath(text: string, byteLength: number, x: number, y: number): Path;

    /**
     * Sets whether to follow the theme font. When followed is set to true,
     * the theme font is used if it is enabled by the system and no typeface is set.
     * @param { boolean } followed - Whether to follow the theme font.
     * The value true means to follow the theme font, and false means the opposite.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 15
     */
    setThemeFontFollowed(followed: boolean): void;

    /**
     * Checks whether the font follows the theme font. By default, the font follows the theme font.
     * @returns { boolean } Check result. The value true means that the theme font is followed, and false means the opposite.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 15
     */
    isThemeFontFollowed(): boolean;
  }

  /**
   * Enumerates the font measurement flags, which is used to specify whether a field in the font measurement information is valid.
   * @enum { number }
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  enum FontMetricsFlags {
    /**
     * The underlineThickness field in the FontMetrics struct is valid.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    UNDERLINE_THICKNESS_VALID = 1 << 0,

    /**
     * The underlinePosition field in the FontMetrics struct is valid.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    UNDERLINE_POSITION_VALID = 1 << 1,

    /**
     * The strikethroughThickness field in the FontMetrics struct is valid.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    STRIKETHROUGH_THICKNESS_VALID = 1 << 2,

    /**
     * The strikethroughPosition field in the FontMetrics struct is valid.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    STRIKETHROUGH_POSITION_VALID = 1 << 3,

    /**
     * The boundary metrics (such as top, bottom, xMin, and xMax) in the FontMetrics struct are invalid.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    BOUNDS_INVALID = 1 << 4,
  }

  /**
   * Describes the attributes that describe the font size and layout. A typeface has similar font metrics.
   * @typedef FontMetrics
   * @syscap SystemCapability.Graphics.Drawing
   * @since 11
   */
  interface FontMetrics {
    /**
     * Font measurement flags that are valid.
     * @type { ?FontMetricsFlags }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    flags?: FontMetricsFlags;

    /**
     * Maximum distance from the baseline to the highest coordinate of the text. The value is a floating point number.
     * @type { number }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    top: number;
    /**
     * Distance from the baseline to the highest coordinate of the text. The value is a floating point number.
     * @type { number }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    ascent: number;
    /**
     * Distance from the baseline to the lowest coordinate of the text. The value is a floating point number.
     * @type { number }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    descent: number;
    /**
     * Maximum distance from the baseline to the lowest coordinate of the text. The value is a floating point number.
     * @type { number }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    bottom: number;
    /**
     * Interline spacing, that is, the distance from the descent of one line of text to the ascent of the next line.
     * The value is a floating point number.
     * @type { number }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    leading: number;
    /**
     * Average character width.
     * @type { ?number }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
     avgCharWidth?: number;

     /**
      * Maximum character width.
      * @type { ?number }
      * @syscap SystemCapability.Graphics.Drawing
      * @since 12
      */
     maxCharWidth?: number;
 
     /**
      * Horizontal distance from the leftmost edge of any glyph bounding box to the origin.
      * This value is usually less than 0, indicating the minimum horizontal coordinate across all glyph bounding boxes.
      * @type { ?number }
      * @syscap SystemCapability.Graphics.Drawing
      * @since 12
      */
     xMin?: number;
 
     /**
      * Horizontal distance from the rightmost edge of any glyph bounding box to the origin.
      * The value is a positive number, indicating the maximum horizontal coordinate across all glyph bounding boxes.
      * @type { ?number }
      * @syscap SystemCapability.Graphics.Drawing
      * @since 12
      */
     xMax?: number;
 
     /**
      * Height of the lowercase letter x. The value is usually a negative value.
      * @type { ?number }
      * @syscap SystemCapability.Graphics.Drawing
      * @since 12
      */
     xHeight?: number;
 
     /**
      * Height of a capital letter. The value is usually a negative value.
      * @type { ?number }
      * @syscap SystemCapability.Graphics.Drawing
      * @since 12
      */
     capHeight?: number;
 
     /**
      * Thickness of the underline.
      * @type { ?number }
      * @syscap SystemCapability.Graphics.Drawing
      * @since 12
      */
     underlineThickness?: number;
 
     /**
      * Vertical distance from the baseline to the top of the underline. The value is usually a positive number.
      * @type { ?number }
      * @syscap SystemCapability.Graphics.Drawing
      * @since 12
      */
     underlinePosition?: number;
 
     /**
      * Thickness of the strikethrough.
      * @type { ?number }
      * @syscap SystemCapability.Graphics.Drawing
      * @since 12
      */
     strikethroughThickness?: number;
 
     /**
      * Vertical distance from the baseline to the bottom of the strikethrough. The value is usually a negative value.
      * @type { ?number }
      * @syscap SystemCapability.Graphics.Drawing
      * @since 12
      */
     strikethroughPosition?: number;
  }

  /**
   * Implements a lattice object, which is used to divide an image by lattice.
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  class Lattice {
    /**
     * Divides the image into lattices. The lattices on both even columns and even rows are fixed,
     * and they are drawn at their original size if the target is large enough.
     * If the target is too small to hold the fixed lattices, all the fixed lattices are scaled down to fit the target,
     * and the lattices that are not on even columns and even rows are scaled to accommodate the remaining space.
     * @param { Array<number> } xDivs - Array of X coordinates used to divide the image. The value is an integer.
     * @param { Array<number> } yDivs - Array of Y coordinates used to divide the image. The value is an integer.
     * @param { number } fXCount - Size of the array that holds the X coordinates. The value range is [0, 5].
     * @param { number } fYCount - Size of the array that holds the Y coordinates. The value range is [0, 5].
     * @param { common2D.Rect | null } fBounds - Source bounds to draw. The rectangle parameter must be an integer.
     * The default value is the rectangle size of the original image. If the rectangle parameter is a decimal,
     * the decimal part is discarded and converted into an integer.
     * @param { Array<RectType> | null } fRectTypes - Array that holds the rectangle types. The default value is null.
     * If this parameter is specified, the array size must be (fXCount + 1) * (fYCount + 1).
     * @param { Array<common2D.Color> | null } fColors - Array that holds the colors used to fill the lattices. The default value is null.
     * If this parameter is specified, the array size must be (fXCount + 1) * (fYCount + 1).
     * @returns { Lattice } Lattice object.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @static
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    static createImageLattice(xDivs: Array<number>, yDivs: Array<number>, fXCount: number, fYCount: number,
      fBounds?: common2D.Rect | null, fRectTypes?: Array<RectType> | null, fColors?: Array<common2D.Color> | null): Lattice;

    /**
     * Divides the image into lattices. The lattices on both even columns and even rows are fixed,
     * and they are drawn at their original size if the target is large enough.
     * If the target is too small to hold the fixed lattices, all the fixed lattices are scaled down to fit the target,
     * and the lattices that are not on even columns and even rows are scaled to accommodate the remaining space.
     * @param { Array<number> } xDivs - Array of X coordinates used to divide the image. The value is an integer.
     * @param { Array<number> } yDivs - Array of Y coordinates used to divide the image. The value is an integer.
     * @param { number } fXCount - Size of the array that holds the X coordinates. The value range is [0, 5].
     * @param { number } fYCount - Size of the array that holds the Y coordinates. The value range is [0, 5].
     * @param { common2D.Rect | null } fBounds - Source bounds to draw. The rectangle parameter must be an integer.
     * The default value is the rectangle size of the original image. If the rectangle parameter is a decimal,
     * the decimal part is discarded and converted into an integer.
     * @param { Array<RectType> | null } fRectTypes - Array that holds the rectangle types. The default value is null.
     * If this parameter is specified, the array size must be (fXCount + 1) * (fYCount + 1).
     * @param { Array<number> | null } fColors - Array that holds the colors used to fill the lattices. 
     * Each color is represented by a 32-bit unsigned integer in hexadecimal ARGB format.
     * The default value is null. If this parameter is specified, the array size must be (fXCount + 1) * (fYCount + 1).
     * @returns { Lattice } Lattice object.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @static
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    static createImageLattice(xDivs: Array<number>, yDivs: Array<number>, fXCount: number, fYCount: number,
      fBounds?: common2D.Rect | null, fRectTypes?: Array<RectType> | null, fColors?: Array<number> | null): Lattice;
  }

  /**
   * Enumerates the types of rectangles used to fill the lattices. This enum is used only in Lattice.
   * @enum { number }
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  enum RectType {
    /**
     * Draws an image into the lattice.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    DEFAULT = 0,

    /**
     * Sets the lattice to transparent.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    TRANSPARENT = 1,

    /**
     * Draws the colors in the fColors array in Lattice into the lattice.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    FIXEDCOLOR = 2
  }

  /**
   * Implements a mask filter.
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  class MaskFilter {
    /**
     * Creates a mask filter with a blur effect.
     * @param { BlurType } blurType - Blur type.
     * @param { number } sigma - Standard deviation of the Gaussian blur to apply. The value must be a floating point number greater than 0.
     * @returns { MaskFilter } MaskFilter object.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @static
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    static createBlurMaskFilter(blurType: BlurType, sigma: number): MaskFilter;
  }

  /**
   * Enumerates the styles of the dashed path effect.
   * @enum { number }
   * @syscap SystemCapability.Graphics.Drawing
   * @since 18
   */
   enum PathDashStyle {
    /**
     * Translates only, not rotating with the path.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    TRANSLATE = 0,
    /**
     * Rotates with the path.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    ROTATE = 1,
    /**
     * Rotates with the path and stretches or compresses at turns to enhance smoothness.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    MORPH = 2,
  }

  /**
   * Implements a path effect.
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  class PathEffect {
    /**
     * Creates a PathEffect object that converts a path into a dotted line.
     * @param { Array<number> } intervals - Array of ON and OFF lengths of dotted lines.
     * The number of arrays must be an even number and be greater than or equal to 2.
     * @param { number } phase - Offset used during drawing. The value is a floating point number.
     * @returns { PathEffect } PathEffect object.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @static
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    static createDashPathEffect(intervals: Array<number>, phase: number): PathEffect;

    /**
     * Creates a path effect that transforms the sharp angle between line segments into a rounded corner with the specified radius.
     * @param { number } radius - Radius of the rounded corner. The value must be greater than 0. The value is a floating point number.
     * @returns { PathEffect } PathEffect object.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @static
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    static createCornerPathEffect(radius: number): PathEffect;

    /**
     * Creates an effect that segments the path and scatters the segments in an irregular pattern along the path.
     * @param { number } segLength - Distance along the path at which each segment is fragmented. The value is a floating point number.
     * If a negative number or the value 0 is passed in, no effect is created.
     * @param { number } dev - Maximum amount by which the end points of the segments can be randomly displaced during rendering.
     * The value is a floating-point number.
     * @param { number } seedAssist - Optional parameter to assist in generating a pseudo-random seed for the effect.
     * The default value is 0, and the value is a 32-bit unsigned integer.
     * @returns { PathEffect } PathEffect object.
     * @static
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    static createDiscretePathEffect(segLength: number, dev: number, seedAssist?: number): PathEffect;

     /**
      * Creates a path effect by sequentially applying the inner effect and then the outer effect.
      * @param { PathEffect } outer - Path effect that is applied second, overlaying the first effect.
      * @param { PathEffect } inner - Inner path effect that is applied first.
      * @returns { PathEffect } PathEffect object.
      * @static
      * @syscap SystemCapability.Graphics.Drawing
      * @since 18
      */
    static createComposePathEffect(outer: PathEffect, inner: PathEffect): PathEffect;

     /**
     * Creates a dashed path effect based on the shape described by a path.
     * @param { Path } path - Path that defines the shape to be used for filling each dash in the pattern.
     * @param { number } advance - Distance between two consecutive dashes. The value is a floating point number greater than 0.
     * Otherwise, an error code is thrown.
     * @param { number } phase - Starting offset of the dash pattern. The value is a floating point number.
     * The actual offset used is the absolute value of this value modulo the value of advance.
     * @param { PathDashStyle } style - Style of the dashed path effect.
     * @returns { PathEffect } PathEffect object.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @static
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    static createPathDashEffect(path: Path, advance: number, phase: number, style: PathDashStyle): PathEffect;

     /**
      * Creates an overlay path effect based on two distinct path effects.
      * Different from createComposePathEffect, this API applies each effect separately and then displays them as a simple overlay.
      * @param { PathEffect } firstPathEffect - First path effect.
      * @param { PathEffect } secondPathEffect - Second path effect.
      * @returns { PathEffect } PathEffect object.
      * @static
      * @syscap SystemCapability.Graphics.Drawing
      * @since 18
      */
    static createSumPathEffect(firstPathEffect: PathEffect, secondPathEffect: PathEffect): PathEffect;
  }

  /**
   * Implements the shader effect. After a shader effect is set for a pen or brush,
   * the shader effect instead of the color attribute is used for drawing. In this case,
   * the alpha value set for the pen or brush still takes effect.
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  class ShaderEffect {
    /**
     * Creates a ShaderEffect object with a single color.
     * @param { number } color - Color in the ARGB format. The value is a 32-bit unsigned integer.
     * @returns { ShaderEffect } Returns the shader with single color ShaderEffect object.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    static createColorShader(color: number): ShaderEffect;

    /**
     * Creates a ShaderEffect object that generates a linear gradient between two points.
     * @param { common2D.Point } startPt - Start point.
     * @param { common2D.Point } endPt - End point.
     * @param { Array<number> } colors - Array of colors to distribute between the two points.
     * The values in the array are 32-bit (ARGB) unsigned integers.
     * @param { TileMode } mode - Tile mode of the shader effect.
     * @param { Array<number> | null } pos - Relative position of each color in the color array.
     * The array length must be the same as that of colors. The first element in the array must be 0.0,
     * the last element must be 1.0, and the middle elements must be between 0.0 and 1.0 and increase by index.
     * The default value is null, indicating that colors are evenly distributed between the two points.
     * @param { Matrix | null } matrix - Matrix object used to perform matrix transformation on the shader effect.
     * The default value is null, indicating the identity matrix.
     * @returns { ShaderEffect } Returns a linear gradient ShaderEffect object.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    static createLinearGradient(startPt: common2D.Point, endPt: common2D.Point, colors: Array<number>,
      mode: TileMode, pos?: Array<number> | null, matrix?: Matrix | null): ShaderEffect;

    /**
     * Creates a ShaderEffect object that generates a radial gradient based on the center and radius of a circle.
     * The radial gradient transitions colors from the center to the ending shape in a radial manner.
     * @param { common2D.Point } centerPt - Center of the circle.
     * @param { number } radius - Radius of the gradient. A negative number is invalid. The value is a floating point number.
     * @param { Array<number> } colors - Array of colors to distribute between the center and ending shape of the circle.
     * The values in the array are 32-bit (ARGB) unsigned integers.
     * @param { TileMode } mode - Tile mode of the shader effect.
     * @param { Array<number> | null } pos - Relative position of each color in the color array.
     * The array length must be the same as that of colors. The first element in the array must be 0.0, the last element must be 1.0,
     * and the middle elements must be between 0.0 and 1.0 and increase by index.
     * The default value is null, indicating that colors are evenly distributed between the center and ending shape of the circle.
     * @param { Matrix | null } matrix - Matrix object used to perform matrix transformation on the shader effect.
     * The default value is null, indicating the identity matrix.
     * @returns { ShaderEffect } Returns a radial gradient ShaderEffect object.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    static createRadialGradient(centerPt: common2D.Point, radius: number, colors: Array<number>,
      mode: TileMode, pos?: Array<number> | null, matrix?: Matrix | null): ShaderEffect;

    /**
     * Creates a ShaderEffect object that generates a sweep gradient based on the center.
     * A sweep gradient paints a gradient of colors in a clockwise or counterclockwise direction based on a given circle center.
     * @param { common2D.Point } centerPt - Center of the circle.
     * @param { Array<number> } colors - Array of colors to distribute between the start angle and end angle.
     * The values in the array are 32-bit (ARGB) unsigned integers.
     * @param { TileMode } mode - Tile mode of the shader effect.
     * @param { number } startAngle - Start angle of the sweep gradient, in degrees.
     * The value 0 indicates the positive direction of the X axis. A positive number indicates an offset towards the positive direction,
     * and a negative number indicates an offset towards the negative direction. The value is a floating point number.
     * @param { number } endAngle - End angle of the sweep gradient, in degrees.
     * The value 0 indicates the positive direction of the X axis. A positive number indicates an offset towards the positive direction,
     * and a negative number indicates an offset towards the negative direction. A value less than the start angle is invalid.
     * The value is a floating point number.
     * @param { Array<number> | null } pos - Relative position of each color in the color array. The array length must be the same as that of colors.
     * The first element in the array must be 0.0, the last element must be 1.0, and the middle elements must be between 0.0 and 1.0 and increase by index.
     * The default value is null, indicating that the colors are evenly distributed between the start angle and end angle.
     * @param { Matrix | null } matrix - Matrix object used to perform matrix transformation on the shader effect.
     * The default value is null, indicating the identity matrix.
     * @returns { ShaderEffect } Returns a sweep gradient ShaderEffect object.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    static createSweepGradient(centerPt: common2D.Point, colors: Array<number>,
      mode: TileMode, startAngle: number, endAngle: number, pos?: Array<number> | null,
      matrix?: Matrix | null): ShaderEffect;

    /**
     * Creates a ShaderEffect object that generates a conical gradient between two given circles.
     * @param { common2D.Point } startPt - Center of the start circle of the gradient.
     * @param { number } startRadius - Radius of the start circle of the gradient. A negative number is invalid.
     * The value is a floating point number.
     * @param { common2D.Point } endPt - Center of the end circle of the gradient.
     * @param { number } endRadius - Radius of the end circle of the gradient. A negative value is invalid.
     * The value is a floating point number.
     * @param { Array<number> } colors - Array of colors to distribute between the start circle and end circle.
     * The values in the array are 32-bit (ARGB) unsigned integers.
     * @param { TileMode } mode - Tile mode of the shader effect.
     * @param { Array<number> | null } pos - Relative position of each color in the color array. The array length must be the same as that of colors.
     * The first element in the array must be 0.0, the last element must be 1.0, and the middle elements must be between 0.0 and 1.0 and increase by index.
     * The default value is null, indicating that colors are evenly distributed between the two circles.
     * @param { Matrix | null } matrix - Matrix object used to perform matrix transformation on the shader effect.
     * The default value is null, indicating the identity matrix.
     * @returns { ShaderEffect } Returns a conical gradient ShaderEffect object.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    static createConicalGradient(startPt: common2D.Point, startRadius: number, endPt: common2D.Point,
      endRadius: number, colors: Array<number>, mode: TileMode,
      pos?: Array<number> | null, matrix?: Matrix | null): ShaderEffect;

    /**
     * Creates an ShaderEffect object that generates a shader with single image.
     * @param { image.PixelMap } pixelmap - PixelMap.
     * @param { TileMode } tileX - Indicates the type of tile mode for horizontal shader effect.
     * @param { TileMode } tileY - Indicates the type of tile mode for vertical shader effect.
     * @param { SamplingOptions } samplingOptions - SamplingOptions used to describe the sampling mode.
     * @param { Matrix | null } matrix - Indicates the Matrix object. The default value is null.
     * @returns { ShaderEffect } Returns the shader with single image ShaderEffect object.
     * @throws { BusinessError } 25900001 - Parameter error. Possible causes: Incorrect parameter range.
     * @static
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    static createImageShader(pixelmap: image.PixelMap, tileX: TileMode, tileY: TileMode,
      samplingOptions: SamplingOptions, matrix?: Matrix | null): ShaderEffect;
    
    /**
     * Creates an ShaderEffect object that generates a blend ShaderEffect object by two shaders.
     * @param { ShaderEffect } dstShaderEffect - Indicates a destination ShaderEffect pointer.
     * @param { ShaderEffect } srcShaderEffect - Indicates a source ShaderEffect pointer.
     * @param { BlendMode } blendMode - BlendMode.
     * @returns { ShaderEffect } Returns a blend ShaderEffect object.
     * @throws { BusinessError } 25900001 - Parameter error. Possible causes: Incorrect parameter range.
     * @static
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    static createComposeShader(dstShaderEffect: ShaderEffect, srcShaderEffect: ShaderEffect,
      blendMode: BlendMode): ShaderEffect;
  }

  /**
   * Enumerates the tile modes of the shader effect.
   * @enum { number }
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  enum TileMode {
    /**
     * Replicates the edge color if the shader effect draws outside of its original boundary.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    CLAMP = 0,

    /**
     * Repeats the shader effect in both horizontal and vertical directions.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    REPEAT = 1,

    /**
     * Repeats the shader effect in both horizontal and vertical directions, alternating mirror images.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    MIRROR = 2,

    /**
     * Renders the shader effect only within the original boundary.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    DECAL = 3,
  }

  /**
   * Implements a shadow layer.
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  class ShadowLayer {
    /**
     * Creates a ShadowLayer object.
     *
     * @param { number } blurRadius - Radius of the shadow layer. The value must be a floating point number greater than 0.
     * @param { number } x - Offset on the X axis. The value is a floating point number.
     * @param { number } y - Offset on the Y axis. The value is a floating point number.
     * @param { common2D.Color } color - Color in ARGB format. The value of each color channel is an integer ranging from 0 to 255.
     * @returns { ShadowLayer } ShadowLayer object.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @static
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    static create(blurRadius: number, x: number, y: number, color: common2D.Color): ShadowLayer;
    
    /**
     * Creates a ShadowLayer object.
     *
     * @param { number } blurRadius - Radius of the shadow layer. The value must be a floating point number greater than 0.
     * @param { number } x - Offset on the X axis. The value is a floating point number.
     * @param { number } y - Offset on the Y axis. The value is a floating point number.
     * @param { common2D.Color | number } color - Color, represented by an unsigned integer in hexadecimal ARGB format.
     * @returns { ShadowLayer } ShadowLayer object.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @static
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    static create(blurRadius: number, x: number, y: number, color: common2D.Color | number): ShadowLayer;
  }

  /**
   * Defines a color filter.
   *
   * @syscap SystemCapability.Graphics.Drawing
   * @since 11
   */
  class ColorFilter {
    /**
     * Creates a ColorFilter object with a given color and blend mode.
     * @param { common2D.Color } color - Color in ARGB format. The value of each color channel is an integer ranging from 0 to 255.
     * @param { BlendMode } mode - Blend mode.
     * @returns { ColorFilter } Colorfilter object.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @static
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    static createBlendModeColorFilter(color: common2D.Color, mode: BlendMode): ColorFilter;

    /**
     * Creates a ColorFilter object with a given color and blend mode.
     * @param { common2D.Color | number } color - Color, represented by an unsigned integer in hexadecimal ARGB format.
     * @param { BlendMode } mode - Blend mode.
     * @returns { ColorFilter } Colorfilter object.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @static
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    static createBlendModeColorFilter(color: common2D.Color | number, mode: BlendMode): ColorFilter;

    /**
     * Creates a ColorFilter object by combining another two color filters.
     * @param { ColorFilter } outer - Color filter that takes effect later in the new filter.
     * @param { ColorFilter } inner - Color filter that takes effect first in the new filter.
     * @returns { ColorFilter } Colorfilter object.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @static
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    static createComposeColorFilter(outer: ColorFilter, inner: ColorFilter): ColorFilter;

    /**
     * Creates a ColorFilter object that applies the sRGB gamma curve to the RGB channels.
     * @returns { ColorFilter } Colorfilter object.
     * @static
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    static createLinearToSRGBGamma(): ColorFilter;

    /**
     * Creates a ColorFilter object that applies the RGB channels to the sRGB gamma curve.
     * @returns { ColorFilter } Colorfilter object.
     * @static
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    static createSRGBGammaToLinear(): ColorFilter;

    /**
     * Creates a ColorFilter object that multiplies the luma into the alpha channel and sets the RGB channels to zero.
     * @returns { ColorFilter } Colorfilter.
     * @static
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    static createLumaColorFilter(): ColorFilter;
    /**
     * Creates a color filter object with a 4*5 color matrix.
     * @param { Array<number> } matrix - An array of 20 numbers, indicating the 4*5 matrix.
     * @returns { ColorFilter } Colorfilter object.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @static
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    static createMatrixColorFilter(matrix: Array<number>): ColorFilter;
    
    /**
     * Makes a color filter with the given mutColor and addColor.
     * @param { common2D.Color | number } mutColor - The range of color channels must be [0, 255], used to multiply source color.
     * @param { common2D.Color | number } addColor - The range of color channels must be [0, 255], used to add to source color.
     * @returns { ColorFilter } Colorfilter object.
     * @static
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    static createLightingColorFilter(mutColor: common2D.Color | number, addColor: common2D.Color | number): ColorFilter;

  }

  /**
   * Implements an image filter.
   *
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  class ImageFilter {
    /**
     * Creates an image filter with a given blur effect.
     * @param { number } sigmaX - Standard deviation of the Gaussian blur along the X axis. The value must be a floating point number greater than 0.
     * @param { number } sigmaY - Standard deviation of the Gaussian blur along the Y axis. The value must be a floating point number greater than 0.
     * @param { TileMode } tileMode - Tile mode to apply to the edges.
     * @param { ImageFilter | null } imageFilter - Filter to which the image filter will be applied.
     * The default value is null, indicating that the image filter is directly applied to the original image.
     * @returns { ImageFilter } ImageFilter object.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @static
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    static createBlurImageFilter(sigmaX: number, sigmaY: number,
      tileMode: TileMode, imageFilter?: ImageFilter | null): ImageFilter;
    /**
     * Creates an image filter object with a given color filter effect.
     * @param { ColorFilter } colorFilter - Color filter.
     * @param { ImageFilter | null } imageFilter - Filter to which the image filter will be applied. The default value is null,
     * indicating that the image filter is directly applied to the original image.
     * @returns { ImageFilter } ImageFilter object.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @static
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    static createFromColorFilter(colorFilter: ColorFilter, imageFilter?: ImageFilter | null): ImageFilter;

    /**
     * Makes an ImageFilter object that instance with the provided x and y offset.
     * @param { number } dx - Indicates the offset in the X direction.
     * @param { number } dy - Indicates the offset in the Y direction.
     * @param { ImageFilter | null } input - Indicates the input image filter used to generate offset effects, or uses
     * the source bitmap if this is null.
     * @returns { ImageFilter } ImageFilter object.
     * @static
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    static createOffsetImageFilter(dx: number, dy: number, input?: ImageFilter | null): ImageFilter;
    
    /**
     * Makes an ImageFilter object that applies the bitmap to the input.
     * @param { image.PixelMap } pixelmap - The source input image.
     * @param { common2D.Rect | null } srcRect - Indicates the input srcRect, or uses the source bitmap if this is null.
     * @param { common2D.Rect | null } dstRect - Indicates the input dstRect, or uses the source bitmap if this is null.
     * @returns { ImageFilter } ImageFilter object.
     * @static
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    static createFromImage(pixelmap: image.PixelMap, srcRect?: common2D.Rect | null, dstRect?: common2D.Rect | null): ImageFilter;
  
    /**
     * Makes an ImageFilter object that applies the blend to the input.
     * @param { BlendMode } mode - Blendmode.
     * @param { ImageFilter } background - Indicates the input background filter.
     * @param { ImageFilter } foreground - Indicates the input foreground filter.
     * @returns { ImageFilter } ImageFilter object.
     * @throws { BusinessError } 25900001 - Parameter error. Possible causes: Incorrect parameter range.
     * <br>2. Incorrect parameter types.
     * @static
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
    */
    static createBlendImageFilter(mode: BlendMode, background: ImageFilter, foreground: ImageFilter): ImageFilter;

    /**
     * Makes an ImageFilter object that combines the "inner" and "outer" filters, allowing the output of the "inner"
     * filter to serve as the input source bitmap for the "outer" filter.
     * @param { ImageFilter } cOuter - Indicates the instance to apply its effects to the output of the 'inner'
     * filter.
     * @param { ImageFilter } cInner - Indicates the output as input for "outer" filters.
     * @returns { ImageFilter } ImageFilter object.
     * @static
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    static createComposeImageFilter(cOuter: ImageFilter, cInner: ImageFilter): ImageFilter;

    /**
     * Makes an ImageFilter object that renders the contents of the input Shader.
     * 
     * @param { ShaderEffect } shader - Indicates the shader effect to be applied to the image.
     * @returns { ImageFilter } ImageFilter object.
     * @static
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    static createFromShaderEffect(shader: ShaderEffect): ImageFilter;
  }
  /**
   * Enumerates the join styles of a pen. The join style defines the shape of the joints of a polyline segment drawn by the pen.
   * @enum { number }
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  enum JoinStyle {
    /**
     * Mitered corner. If the angle of a polyline is small, its miter length may be inappropriate.
     * In this case, you need to use the miter limit to limit the miter length.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    MITER_JOIN = 0,

    /**
     * Round corner.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    ROUND_JOIN = 1,

    /**
     * Bevel corner.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    BEVEL_JOIN = 2
  }

  /**
   * Enumerates the cap styles of a pen. The cap style defines the style of both ends of a line segment drawn by the pen.
   * @enum { number }
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  enum CapStyle {
    /**
     * There is no cap style. Both ends of the line segment are cut off square.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    FLAT_CAP = 0,

    /**
     * Square cap style. Both ends have a square, the height of which is half of the width of the line segment, with the same width.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    SQUARE_CAP = 1,

    /**
     * Round cap style. Both ends have a semicircle centered, the diameter of which is the same as the width of the line segment.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    ROUND_CAP = 2
  }

  /**
   * Enumerates the blur types of a mask filter.
   * @enum { number }
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  enum BlurType {
    /**
     * Both the outer edges and the inner solid parts are blurred.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    NORMAL = 0,

    /**
     * The inner solid part remains unchanged, while only the outer edges are blurred.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    SOLID = 1,

    /**
     * Only the outer edges are blurred, with the inner solid part being fully transparent.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    OUTER = 2,

    /**
     * Only the inner solid part is blurred, while the outer edges remain sharp.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    INNER = 3
  }

  /**
   * Defines a pen, which is used to describe the style and color to outline a shape.
   * @syscap SystemCapability.Graphics.Drawing
   * @since 11
   */
  class Pen {
    /**
     * A constructor used to create a Pen object.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    constructor();

    /**
     * Copies a Pen object to create a new one.
     * @param { Pen } pen - Pen object to copy.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    constructor(pen: Pen);

    /**
     * Sets the maximum ratio allowed between the sharp corner length of a polyline and its line width.
     * When drawing a polyline with the pen, if JoinStyle is set to MITER_JOIN and this maximum ratio is exceeded,
     * the corner will be displayed as beveled instead of mitered.
     * @param { number } miter - Maximum ratio of the sharp corner length of the polyline to the line width.
     * A negative number is processed as 4.0 during drawing. Non-negative numbers take effect normally. The value is a floating point number.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    setMiterLimit(miter: number): void;

    /**
     * Obtains the maximum ratio allowed between the sharp corner length of a polyline and its line width.
     * @returns { number } Returns the miter limit.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    getMiterLimit(): number;

    /**
     * Sets the shader effect for this pen.
     * @param { ShaderEffect } shaderEffect -  ShaderEffect object. If null is passed in, the shader effect will be cleared.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    setShaderEffect(shaderEffect: ShaderEffect): void;

    /**
    * Sets a color for this pen.
    * @param { common2D.Color } color - Color in ARGB format. The value of each color channel is an integer ranging from 0 to 255.
    * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
    * <br>2. Incorrect parameter types; 3. Parameter verification failed.
    * @syscap SystemCapability.Graphics.Drawing
    * @since 11
    */
    setColor(color: common2D.Color): void;

    /**
    * Sets a color for this pen. This API provides better performance than setColor and is recommended.
     * @param { number } alpha - Alpha channel value of the color in ARGB format. The value is an integer ranging from 0 to 255
     *  Any passed-in floating point number is rounded down.
     * @param { number } red - Red channel value of the color in ARGB format. The value is an integer ranging from 0 to 255.
     * Any passed-in floating point number is rounded down.
     * @param { number } green - Green channel value of the color in ARGB format. The value is an integer ranging from 0 to 255.
     * Any passed-in floating point number is rounded down.
     * @param { number } blue - Blue channel value of the color in ARGB format. The value is an integer ranging from 0 to 255.
     * Any passed-in floating point number is rounded down.
    * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
    * @syscap SystemCapability.Graphics.Drawing
    * @since 12
    */
    setColor(alpha: number, red: number, green: number, blue: number): void;

    /**
    * Sets a color for this pen.
    * @param { number } color - Color in hexadecimal ARGB format.
    * @syscap SystemCapability.Graphics.Drawing
    * @since 18
    */
    setColor(color: number): void;

    /**
     * Obtains the color of this pen.
     * @returns { common2D.Color } Returns a 32-bit (ARGB) variable that describes the color.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    getColor(): common2D.Color;

    /**
     * Obtains the color of this pen.
     * @returns { number } Returns a 32-bit (ARGB) variable that describes the color of hexadecimal format.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    getHexColor(): number;

    /**
    * Sets the stroke width for this pen. The value 0 is treated as an unusually thin width. During drawing,
    * the width of 0 is always drawn as 1 pixel wide, regardless of any scaling applied to the canvas.
    * Negative values are also regarded as the value 0 during the drawing process.
    *
    * @param { number } width - Stroke width. The value is a floating point number.
    * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
    * <br>2. Incorrect parameter types.
    * @syscap SystemCapability.Graphics.Drawing
    * @since 11
    */
    setStrokeWidth(width: number): void;

    /**
     * Obtains the stroke width of this pen. The width describes the thickness of the outline of a shape.
     * @returns { number } Returns the thickness.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    getWidth(): number;

    /**
    * Enables anti-aliasing for this pen. Anti-aliasing makes the edges of the content smoother.
    *
    * @param { boolean } aa - Whether to enable anti-aliasing. The value true means to enable anti-aliasing, and false means the opposite.
    * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
    * <br>2. Incorrect parameter types.
    * @syscap SystemCapability.Graphics.Drawing
    * @since 11
    */
    setAntiAlias(aa: boolean): void;

    /**
     * Checks whether anti-aliasing is enabled for this pen.
     * @returns { boolean } Returns true if the anti-aliasing is enabled; returns false otherwise.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    isAntiAlias(): boolean;

    /**
    * Sets an alpha value for this pen.
    *
    * @param { number } alpha - Alpha value. The value is an integer in the range [0, 255]. If a floating point number is passed in, the value is rounded down.
    * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
    * <br>2. Incorrect parameter types; 3. Parameter verification failed.
    * @syscap SystemCapability.Graphics.Drawing
    * @since 11
    */
    setAlpha(alpha: number): void;

    /**
     * Obtains the alpha value of this pen.
     * @returns { number } Returns a 8-bit variable that describes the alpha.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    getAlpha(): number;

    /**
    * Sets a color filter for this pen.
    *
    * @param { ColorFilter } filter - Color filter. If null is passed in, the color filter is cleared.
    * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
    * <br>2. Incorrect parameter types.
    * @syscap SystemCapability.Graphics.Drawing
    * @since 11
    */
    setColorFilter(filter: ColorFilter): void;
    /**
     * Obtains the color filter of this pen.
     * @returns { ColorFilter } ColorFilter.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    getColorFilter(): ColorFilter;
    /**
     * Sets an image filter for this pen.
     * @param { ImageFilter | null } filter - Image filter. If null is passed in, the image filter effect of the pen will be cleared.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    setImageFilter(filter: ImageFilter | null): void;
    /**
     * Adds a mask filter for this pen.
     *
     * @param { MaskFilter } filter - Mask filter. If null is passed in, the mask filter is cleared.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    setMaskFilter(filter: MaskFilter): void;

    /**
     * Sets the path effect for this pen.
     *
     * @param { PathEffect } effect - Path effect. If null is passed in, the path filter is cleared.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    setPathEffect(effect: PathEffect): void;

    /**
     * Sets a shadow layer for this pen. The shadow layer effect takes effect only when text is drawn.
     *
     * @param { ShadowLayer } shadowLayer - Shadow layer. If null is passed in, the shadow layer is cleared.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    setShadowLayer(shadowLayer: ShadowLayer): void;

    /**
    * Sets a blend mode for this pen.
    *
    * @param { BlendMode } mode - Blend mode.
    * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
    * <br>2. Incorrect parameter types; 3. Parameter verification failed.
    * @syscap SystemCapability.Graphics.Drawing
    * @since 11
    */
    setBlendMode(mode: BlendMode): void;

    /**
    * Enables dithering for this pen. Dithering make the drawn color more realistic.
    *
    * @param { boolean } dither - Whether to enable dithering. The value true means to enable dithering, and false means the opposite.
    * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
    * <br>2. Incorrect parameter types.
    * @syscap SystemCapability.Graphics.Drawing
    * @since 11
    */
    setDither(dither: boolean): void;

    /**
     * Sets the join style for this pen.
     *
     * @param { JoinStyle } style - Join style.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    setJoinStyle(style: JoinStyle): void;

    /**
     * Obtains the join style of this pen.
     *
     * @returns { JoinStyle } The JoinStyle.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    getJoinStyle(): JoinStyle;

    /**
     * Sets the cap style for this pen.
     *
     * @param { CapStyle } style - Cap style.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    setCapStyle(style: CapStyle): void;

    /**
     * Obtains the cap style of this pen.
     *
     * @returns { CapStyle } The CapStyle.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    getCapStyle(): CapStyle;

    /**
     * Resets this pen to the initial state.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    reset(): void;
    /**
     * Obtains the source path outline drawn using this pen and represents it using a destination path.
     *
     * @param { Path } src - Source path.
     * @param { Path } dst - Destination path.
     * @returns { boolean } true if the path should be filled, or false if it should be drawn with a hairline (width == 0)
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    getFillPath(src: Path, dst: Path): boolean;
  }

  /**
   * Defines a brush, which is used to describe the style and color to fill in a shape.
   * @syscap SystemCapability.Graphics.Drawing
   * @since 11
   */
  class Brush {
    /**
     * A constructor used to create a Brush object.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    constructor();

    /**
     * Copies a Brush object to create a new one.
     * @param { Brush } brush - Indicates the Brush object.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    constructor(brush: Brush);

    /**
     * Sets a color for this brush.
     * @param { common2D.Color } color - Color in ARGB format. The value of each color channel is an integer ranging from 0 to 255.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    setColor(color: common2D.Color): void;

    /**
     * Sets a color for this brush. This API provides better performance than setColor and is recommended.
     * @param { number } alpha - Alpha channel value of the color in ARGB format. The value is an integer ranging from 0 to 255.
     * Any passed-in floating point number is rounded down.
     * @param { number } red - Red channel value of the color in ARGB format. The value is an integer ranging from 0 to 255.
     * Any passed-in floating point number is rounded down.
     * @param { number } green - Green channel value of the color in ARGB format. The value is an integer ranging from 0 to 255.
     * Any passed-in floating point number is rounded down.
     * @param { number } blue - Blue channel value of the color in ARGB format. The value is an integer ranging from 0 to 255.
     * Any passed-in floating point number is rounded down.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    setColor(alpha: number, red: number, green: number, blue: number): void;

    /**
     * Sets a color for this brush.
     * @param { number } color - Color in hexadecimal ARGB format.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    setColor(color: number): void;

    /**
     * Obtains the color of this brush.
     * @returns { common2D.Color } Returns a 32-bit (ARGB) variable that describes the color.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    getColor(): common2D.Color;

    /**
     * Obtains the color of this brush.
     * @returns { number } Returns a 32-bit (ARGB) variable that describes the color of hexadecimal format.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    getHexColor(): number;

    /**
     * Enables anti-aliasing for this brush. Anti-aliasing makes the edges of the content smoother.
     * @param { boolean } aa - Whether to enable anti-aliasing. The value true means to enable anti-aliasing, and false means the opposite.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    setAntiAlias(aa: boolean): void;

    /**
     * Checks whether anti-aliasing is enabled for this brush.
     * @returns { boolean } Returns true if anti-aliasing is enabled; returns false otherwise.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    isAntiAlias(): boolean;

    /**
     * Sets an alpha value for this brush.
     * @param { number } alpha - Alpha value. The value is an integer in the range [0, 255]. If a floating point number is passed in, the value is rounded down.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    setAlpha(alpha: number): void;

    /**
     * Obtains the alpha value of this brush.
     * @returns { number } Returns a 8-bit variable that describes the alpha.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    getAlpha(): number;

    /**
     * Sets a color filter for this brush.
     * @param { ColorFilter } filter - Color filter. If null is passed in, the color filter is cleared.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    setColorFilter(filter: ColorFilter): void;

    /**
     * Obtains the color filter of this brush.
     * @returns { ColorFilter } ColorFilter.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    getColorFilter(): ColorFilter;
    /**
     * Sets an image filter for this brush.
     * @param { ImageFilter | null } filter - Image filter. If null is passed in, the image filter effect of the brush will be cleared.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    setImageFilter(filter: ImageFilter | null): void;
    /**
     * Adds a mask filter for this brush.
     * @param { MaskFilter } filter - Mask filter. If null is passed in, the mask filter is cleared.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    setMaskFilter(filter: MaskFilter): void;

    /**
     * Sets a shadow layer for this brush. The shadow layer effect takes effect only when text is drawn.
     *
     * @param { ShadowLayer } shadowLayer - Shadow layer. If null is passed in, the shadow layer is cleared.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    setShadowLayer(shadowLayer: ShadowLayer): void;

    /**
     * Sets the shader effect for this brush.
     * @param { ShaderEffect } shaderEffect - ShaderEffect object. If null is passed in, the shader effect will be cleared.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    setShaderEffect(shaderEffect: ShaderEffect): void;

    /**
     * Sets a blend mode for this brush.
     * @param { BlendMode } mode - Blend mode.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 11
     */
    setBlendMode(mode: BlendMode): void;

    /**
     * Resets this brush to the initial state.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    reset(): void;
  }

  /**
   * Implements a matrix.
   *
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  class Matrix {
    /**
     * Creates a Matrix object.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    constructor();

    /**
     * Creates a deep copy of the specified matrix object.
     * @param { Matrix } matrix - The matrix object to copy.
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    constructor(matrix: Matrix);

    /**
     * Query whether the current matrix is affine or not.
     * @returns { boolean } Returns true if the matrix is affine; returns false otherwise.
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    isAffine(): boolean;

    /**
     * Query whether a rectangle will map to another rectangle after applying this matrix.
     * @returns { boolean } Returns true if the transformation keeps rectangles as rectangles; returns false otherwise.
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    rectStaysRect(): boolean;

    /**
     * Sets this matrix as an identity matrix and rotates it by a given degree around the rotation point (px, py).
     * @param { number } degree - Angle to rotate, in degrees. A positive number indicates a clockwise rotation,
     * and a negative number indicates a counterclockwise rotation. The value is a floating point number.
     * @param { number } px - X coordinate of the rotation point. The value is a floating point number.
     * @param { number } py - Y coordinate of the rotation point. The value is a floating point number.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    setRotation(degree: number, px: number, py: number): void;

    /**
     * Sets this matrix as an identity matrix and scales it with the coefficients (sx, sy) at the scale point (px, py).
     * @param { number } sx - Scale coefficient along the X axis. If a negative number is passed in,
     * the matrix is mirrored around y = px before being scaled. The value is a floating point number.
     * @param { number } sy - Scale coefficient along the Y axis. If a negative number is passed in,
     * the matrix is mirrored around x = py before being scaled. The value is a floating point number.
     * @param { number } px - X coordinate of the scale point. The value is a floating point number.
     * @param { number } py - Y coordinate of the scale point. The value is a floating point number.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    setScale(sx: number, sy: number, px: number, py: number): void;

    /**
     * Sets this matrix as an identity matrix and translates it by a given distance (dx, dy).
     * @param { number } dx - Horizontal distance to translate. A positive number indicates a translation towards the positive direction of the X axis,
     * and a negative number indicates a translation towards the negative direction of the X axis. The value is a floating point number.
     * @param { number } dy - Vertical distance to translate. A positive number indicates a translation towards the positive direction of the Y axis,
     * and a negative number indicates a translation towards the negative direction of the Y axis. The value is a floating point number.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    setTranslation(dx: number, dy: number): void;

    /**
     * Sets the skew transformation with a pivot point.
     * @param { number } kx - The skew factor along the x-axis.
     * @param { number } ky - The skew factor along the y-axis.
     * @param { number } px - The x-coordinate of the point around which to apply the skew.
     * @param { number } py - The y-coordinate of the point around which to apply the skew.
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    setSkew(kx: number, ky: number, px: number, py: number): void;

    /**
     * Sets the sine and cosine values for a rotation transformation around a point.
     * @param { number } sinValue - The sine of the angle of rotation.
     * @param { number } cosValue - The cosine of the angle of rotation.
     * @param { number } px - The x-coordinate of the point around which to rotate.
     * @param { number } py - The y-coordinate of the point around which to rotate.
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    setSinCos(sinValue: number, cosValue: number, px: number, py: number): void;

    /**
     * Sets parameters for this matrix.
     * @param { Array<number> } values - Each value in the array represents the following parameters:
     * values[0] - horizontal scale factor to store.
     * values[1] - horizontal skew factor to store.
     * values[2] - horizontal translation to store.
     * values[3] - vertical skew factor to store.
     * values[4] - vertical scale factor to store.
     * values[5] - vertical translation to store.
     * values[6] - input x-axis values perspective factor to store.
     * values[7] - input y-axis values perspective factor to store.
     * values[8] - perspective scale factor to store.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    setMatrix(values: Array<number>): void;

    /**
     * Sets matrix to the copy of anther matrix object.
     * @param { Array<number> | Matrix } matrix - Indicates the Matrix object to copy.
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    setMatrix(matrix: Array<number> | Matrix): void;

    /**
     * Sets matrix to the product of matrix A and matrix B.
     * @param { Matrix } matrixA - Indicates the MatrixA object.
     * @param { Matrix } matrixB - Indicates the MatrixB object.
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    setConcat(matrixA: Matrix, matrixB: Matrix): void;

    /**
     * Sets matrix to the product of 'this' and another.
     * @param { Matrix } matrix - Indicates the other Matrix object.
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    postConcat(matrix: Matrix): void;

    /**
     * Preconcats the existing matrix with the passed-in matrix.
     * @param { Matrix } matrix - Matrix object, which is on the right of a multiplication expression.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    preConcat(matrix: Matrix): void;

    /**
     * Checks whether this matrix is equal to another matrix.
     * @param { Matrix } matrix - Matrix to compare.
     * @returns { Boolean } Returns true if the two matrices are equal; returns false otherwise.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    isEqual(matrix: Matrix): Boolean;

    /**
     * Inverts this matrix and returns the result.
     * @param { Matrix } matrix - Matrix object used to store the inverted matrix.
     * @returns { Boolean } Returns true if matrix can be inverted; returns false otherwise.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    invert(matrix: Matrix): Boolean;

    /**
     * Checks whether this matrix is an identity matrix.
     * @returns { Boolean } Returns true if matrix is identity; returns false otherwise.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    isIdentity(): Boolean;

    /**
     * Obtains the value of a given index in this matrix. The index ranges from 0 to 8.
     * @param { number } index - Index. The value is an integer ranging from 0 to 8.
     * @returns { number } Returns value corresponding to index.Returns 0 if out of range.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    getValue(index: number): number;
    /**
     * Post multiplies this matrix by a matrix that is derived from an identity matrix after it has been rotated by a
     * given degree around the rotation point (px, py).
     * @param { number } degree - Angle to rotate, in degrees. A positive number indicates a clockwise rotation,
     * and a negative number indicates a counterclockwise rotation. The value is a floating point number.
     * @param { number } px - X coordinate of the rotation point. The value is a floating point number.
     * @param { number } py - Y coordinate of the rotation point. The value is a floating point number.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    postRotate(degree: number, px: number, py: number): void;
    /**
     * Post multiplies this matrix by a matrix that is derived from an identity matrix after it has been
     * scaled with the coefficient (sx, sy) at the scale point (px, py).
     * @param { number } sx - Scale coefficient along the X axis. If a negative number is passed in,
     * the matrix is mirrored around y = px before being scaled. The value is a floating point number.
     * @param { number } sy - Scale coefficient along the Y axis. If a negative number is passed in,
     * the matrix is mirrored around x = py before being scaled. The value is a floating point number.
     * @param { number } px - X coordinate of the scale point. The value is a floating point number.
     * @param { number } py - Y coordinate of the scale point. The value is a floating point number.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    postScale(sx: number, sy: number, px: number, py: number): void;
    /**
     * Post multiplies this matrix by a matrix that is derived from an identity matrix after it has been translated by a given distance (dx, dy).
     * @param { number } dx - Horizontal distance to translate. A positive number indicates a translation towards the positive direction of the X axis,
     * and a negative number indicates a translation towards the negative direction of the X axis. The value is a floating point number.
     * @param { number } dy - Vertical distance to translate. A positive number indicates a translation towards the positive direction of the Y axis,
     * and a negative number indicates a translation towards the negative direction of the Y axis. The value is a floating point number.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    postTranslate(dx: number, dy: number): void;
  
    /**
     * Premultiplies this matrix by a matrix that is derived from an identity matrix after it has been rotated by a
     * given degree around the rotation point (px, py).
     * @param { number } degree - Angle to rotate, in degrees. A positive number indicates a clockwise rotation,
     * and a negative number indicates a counterclockwise rotation. The value is a floating point number.
     * @param { number } px - X coordinate of the rotation point. The value is a floating point number.
     * @param { number } py - Y coordinate of the rotation point. The value is a floating point number.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    preRotate(degree: number, px: number, py: number): void;

    /**
     * Sets matrix to matrix constructed from skewing by (kx, ky) about pivot point (px, py), multiplied by matrix.
     * This can be thought of as skewing relative to a pivot point after applying matrix.
     * @param { number } kx - Indicates the horizontal skew factor.
     * @param { number } ky - Indicates the vertical skew factor.
     * @param { number } px - Indicates the pivot on x-axis.
     * @param { number } py - Indicates the pivot on y-axis.
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    postSkew(kx: number, ky: number, px: number, py: number): void;

    /**
     * Premultiplies this matrix by a matrix that is derived from an identity matrix after it has been scaled with the
     * coefficient (sx, sy) at the scale point (px, py).
     * @param { number } sx - Scale coefficient along the X axis. If a negative number is passed in,
     * the matrix is mirrored around y = px before being scaled. The value is a floating point number.
     * @param { number } sy - Scale coefficient along the Y axis. If a negative number is passed in,
     * the matrix is mirrored around x = py before being scaled. The value is a floating point number.
     * @param { number } px - X coordinate of the scale point. The value is a floating point number.
     * @param { number } py - Y coordinate of the scale point. The value is a floating point number.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    preScale(sx: number, sy: number, px: number, py: number): void;
    /**
     * Premultiplies this matrix by a matrix that is derived from an identity matrix after it has been translated by a given distance (dx, dy).
     * @param { number } dx - Horizontal distance to translate. A positive number indicates a translation towards the positive direction of the X axis,
     * and a negative number indicates a translation towards the negative direction of the X axis. The value is a floating point number.
     * @param { number } dy - Vertical distance to translate. A positive number indicates a translation towards the positive direction of the Y axis,
     * and a negative number indicates a translation towards the negative direction of the Y axis. The value is a floating point number.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    preTranslate(dx: number, dy: number): void;

    /**
     * Sets matrix to matrix multiplied by matrix constructed from skewing by (kx, ky) about pivot point (px, py).
     * This can be thought of as scaling relative to a pivot point before applying matrix.
     * @param { number } kx - Indicates the horizontal skew factor.
     * @param { number } ky - Indicates the vertical skew factor.
     * @param { number } px - Indicates the pivot on x-axis.
     * @param { number } py - Indicates the pivot on y-axis.
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    preSkew(kx: number, ky: number, px: number, py: number): void;

    /**
     * Resets this matrix to an identity matrix.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    reset(): void;
    /**
     * Maps a source point array to a destination point array by means of matrix transformation.
     * @param { Array<common2D.Point> } src - Array of source points.
     * @returns { Array<common2D.Point> } Return mapped points array.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    mapPoints(src: Array<common2D.Point>): Array<common2D.Point>;

    /**
     * Forms a circle by radius and maps it to a ellipse, returns the average radius of the ellipse.
     * The average radius is equal to the square root of the product of the major axis length and the minor axis length.
     * @param { number } radius - circle size to map.
     * @returns { number } Return average mapped radius.
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    mapRadius(radius: number): number;

    /**
     * Obtains all element values of this matrix.
     * @returns { Array<number> } nine scalar values contained by Matrix.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    getAll(): Array<number>;
    /**
     * Sets the destination rectangle to the bounding rectangle of the shape obtained after transforming the source rectangle
     * with a matrix transformation. As shown in the figure below, the blue rectangle represents the source rectangle,
     * and the yellow rectangle is the shape obtained after a matrix transformation is applied to the source rectangle.
     * Since the edges of the yellow rectangle are not aligned with the coordinate axes, it cannot be represented by a rectangle object.
     * To address this issue, a destination rectangle (black rectangle) is defined as the bounding rectangle.
     * @param { common2D.Rect } dst - Rectangle object, which is used to store the bounding rectangle.
     * @param { common2D.Rect } src - Source rectangle.
     * @returns { boolean } Returns true if the mapped src is equal to the dst; returns false is not equal.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    mapRect(dst: common2D.Rect, src: common2D.Rect): boolean;
    /**
     * Sets this matrix to a transformation matrix that maps a source rectangle to a destination rectangle.
     * @param { common2D.Rect } src - Source rectangle.
     * @param { common2D.Rect } dst - Destination rectangle.
     * @param { ScaleToFit } scaleToFit - Mapping mode from the source rectangle to the target rectangle.
     * @returns { boolean } Returns true if dst is empty, and sets matrix to:
               | 0 0 0 |
               | 0 0 0 |
               | 0 0 1 |.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    setRectToRect(src: common2D.Rect, dst: common2D.Rect, scaleToFit: ScaleToFit): boolean;
    /**
     * Sets this matrix to a transformation matrix that maps the source point array to the destination point array.
     * Both the number of source points and that of destination points must be in the range [0, 4].
     * @param { Array<common2D.Point> } src - Array of source points. The array length must be the same as the value of count.
     * @param { Array<common2D.Point> } dst - Array of destination points. The array length must be the same as the value of count.
     * @param { number } count - Number of points in each array. The value is an integer.
     * @returns { boolean } Returns true if Matrix was constructed successfully
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    setPolyToPoly(src: Array<common2D.Point>, dst: Array<common2D.Point>, count: number): boolean;
  }

  /**
   * Enumerates the modes of scaling a source rectangle into a destination rectangle.
   * @enum { number }
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  enum ScaleToFit {
    /**
     * Scales the source rectangle to completely fill the destination rectangle, potentially changing the aspect ratio of the source rectangle.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    FILL_SCALE_TO_FIT = 0,

    /**
     * Scales the source rectangle, preserving its aspect ratio, to align it to the upper left corner of the destination rectangle.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    START_SCALE_TO_FIT = 1,

    /**
     * Scales the source rectangle, preserving its aspect ratio, to align it to the center of the destination rectangle.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    CENTER_SCALE_TO_FIT = 2,

    /**
     * Scales the source rectangle, preserving its aspect ratio, to align it to the lower right corner of the destination rectangle.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    END_SCALE_TO_FIT = 3
  }

  /**
   * Describes a region, which is used to describe the region where the shape can be drawn.
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  class Region {
    /**
     * Creates an empty region.
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    constructor();

    /**
     * Creates a deep copy of the specified region object.
     * @param { Region } region - The region object to copy.
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    constructor(region: Region);

    /**
     * Creates a region with a rectangle.
     * @param { number } left - Indicates the left edge of the rectangle.
     * @param { number } top - Indicates the top edge of the rectangle.
     * @param { number } right - Indicates the right edge of the rectangle.
     * @param { number } bottom - Indicates the bottom edge of the rectangle.
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    constructor(left: number, top: number, right: number, bottom: number);

    /**
     * Query whether this region is equal to the other region.
     * @param { Region } other - Indicates the region object for comparasion.
     * @returns { boolean } Returns compare result.
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    isEqual(other: Region): boolean;

    /**
     * Query whether the region contains multiple rectangles.
     * @returns { boolean } Returns true if the region contains more than one rectangle;
     * <br>returns false otherwise.
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    isComplex(): boolean;

    /**
     * Query whether the region is empty .
     * @returns { boolean } Returns true if the region is empty; returns false otherwise.
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    isEmpty(): boolean;

    /**
     * Gets the bounds of the region.
     * @returns { common2D.Rect } Returns Rect object.
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    getBounds(): common2D.Rect;

    /** Gets the boundary of the region, which represents by a path.
     * Gets the bounds of the region.
     * @returns { Path } Returns Path object.
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    getBoundaryPath(): Path;

    /**
     * Checks whether a point is contained in this region.
     * @param { number } x - X coordinate of the point. The value must be an integer. If a decimal is passed in, the decimal part is rounded off.
     * @param { number } y - Y coordinate of the point. The value must be an integer. If a decimal is passed in, the decimal part is rounded off.
     * @returns { boolean } Returns true if (x, y) is inside region; returns false otherwise.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    isPointContained(x: number, y:number): boolean;

    /**
     * Checks whether another region is contained in this region.
     * @param { Region } other - Region object.
     * @returns { boolean } Returns true if other region is completely inside the region object;
     * <br>returns false otherwise.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    isRegionContained(other: Region): boolean;

    /**
     * Performs an operation on this region and another region, and stores the resulting region in this Region object.
     * @param { Region } region - Region object.
     * @param { RegionOp } regionOp - Operation mode of the region.
     * @returns { boolean } Returns true if replaced region is not empty; returns false otherwise.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    op(region: Region, regionOp: RegionOp): boolean;

    /**
     * Offsets the region by adding dx along the x-axis and dy along the y-axis.
     * @param { number } dx - Indicates the x coordinate of the point. The parameter must be an integer.
     * @param { number } dy - Indicates the y coordinate of the point. The parameter must be an integer.
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    offset(dx: number, dy: number): void;

    /**
     * Checks whether a rectangle do not intersect with this region. Actually,
     * this API determines whether the rectangle does not intersect with the bounding rectangle of the region, and therefore the result may not be accurate.
     * @param { number } left - Left position of the rectangle. The value must be an integer. If a decimal is passed in, the decimal part is rounded off.
     * @param { number } top - Top position of the rectangle. The value must be an integer. If a decimal is passed in, the decimal part is rounded off.
     * @param { number } right - Right position of the rectangle. The value must be an integer. If a decimal is passed in, the decimal part is rounded off.
     * @param { number } bottom - Bottom position of the rectangle. The value must be an integer. If a decimal is passed in, the decimal part is rounded off.
     * @returns { boolean } Returns true if rect and region is not intersect; returns false otherwise.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    quickReject(left: number, top: number, right: number, bottom: number): boolean;

    /**
     * Determines whether region is intersect with another.
     * @param { Region } region - Indicates the other region for comparasion.
     * @returns { boolean } Returns true if the region dose not intersect the other, or the region is empty;
     * <br>returns false otherwise.
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    quickRejectRegion(region: Region): boolean;

    /**
     * Sets a region that matches the outline of a path within the cropping area.
     * @param { Path } path - Path object.
     * @param { Region } clip - Region object.
     * @returns { boolean } Returns true if constructed region is not empty; returns false otherwise.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    setPath(path: Path, clip: Region): boolean;

    /**
     * Sets a rectangle.
     * @param { number } left - Left position of the rectangle. The value must be an integer. If a decimal is passed in, the decimal part is rounded off.
     * @param { number } top - Top position of the rectangle. The value must be an integer. If a decimal is passed in, the decimal part is rounded off.
     * @param { number } right - Right position of the rectangle. The value must be an integer. If a decimal is passed in, the decimal part is rounded off.
     * @param { number } bottom - Bottom position of the rectangle. The value must be an integer. If a decimal is passed in, the decimal part is rounded off.
     * @returns { boolean } Returns true if constructed region is not empty; returns false otherwise.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    setRect(left: number, top: number, right: number, bottom: number): boolean;

    /**
     * Sets the region to the specified region.
     * @param { Region } region - Region object.
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    setRegion(region: Region): void;

    /**
     * Sets the region to empty.
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    setEmpty(): void;
  }

  /**
   * Enumerates the operations for combining two regions.
   * @enum { number }
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  enum RegionOp {
    /**
     * Difference operation.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    DIFFERENCE = 0,

    /**
     * Intersect operation.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    INTERSECT = 1,

    /**
     * Union operation.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    UNION = 2,

    /**
     * XOR operation.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    XOR = 3,

    /**
     * Reverse difference operation.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    REVERSE_DIFFERENCE = 4,

    /**
     * Replace operation.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    REPLACE = 5
  }

  /**
   * Enumerates the corner positions of a rounded rectangle.
   *
   * @enum { number }
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  enum CornerPos {
    /**
     * Top left corner of the rounded rectangle.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    TOP_LEFT_POS = 0,

    /**
     * Top right corner of the rounded rectangle.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    TOP_RIGHT_POS = 1,

    /**
     * Bottom right corner of the rounded rectangle.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    BOTTOM_RIGHT_POS = 2,

    /**
     * Bottom left corner of the rounded rectangle.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    BOTTOM_LEFT_POS = 3
  }

  /**
   * Enumerates the constraint types of the source rectangle.
   *
   * @enum { number }
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  enum SrcRectConstraint {

    /**
     * The sampling range is strictly confined to the source rectangle, resulting in a slow sampling speed.
     *
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    STRICT = 0,

    /**
     * The sampling range is not limited to the source rectangle and can extend beyond it, allowing for a high sampling speed.
     *
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    FAST = 1
  }

  /**
   * A utility class that provides only static methods to convert data structs defined in other modules and common2D.
   *
   * @syscap SystemCapability.Graphics.Drawing
   * @since 15
   */
  class Tool {
    /**
     * Converts a color value of the ResourceColor type to a common2D.Color object.
     * @param { ResourceColor } resourceColor - Color value of the ResourceColor type. (All four types of inputs are supported.
     * The following provides 13 example inputs.) The fourth type of Resource supports only the construction method $r('belonging.type.name').
     * Ensure that the resource has been defined in the main/resources/base/element directory. (The types color, string,
     * and integer are available for the belonging app, whereas only the type color is available for the belonging sys.)
     * @returns { common2D.Color } Returns a 32-bit (ARGB) variable that describes the color.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 15
     */
    static makeColorFromResourceColor(resourceColor: ResourceColor): common2D.Color;
  }

  /**
   * This class offers a comprehensive set of operations to handle for common2D Rect object.
   *
   * @syscap SystemCapability.Graphics.Drawing
   * @crossplatform
   * @since 20
   */
  class RectUtils {
    /**
     * Makes an uninitialized 2D rectangular object with zero dimensions and origin at (0, 0).
     *
     * @returns { common2D.Rect } - Returns an empty Rect object with all coordinates (left, top, right, bottom) set to 0.
     * @static
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    static makeEmpty() : common2D.Rect;

    /**
     * Makes a 2D rectangular object from boundary coordinates.
     * @param { number } left - Indicates the X-coordinate of the left edge.
     * @param { number } top - Indicates the Y-coordinate of the top edge.
     * @param { number } right - Indicates the X-coordinate of the right edge.
     * @param { number } bottom - Indicates the Y-coordinate of the bottom edge.
     * @returns { common2D.Rect } - Returns an Rect object with the specific coordinates (left, top, right, bottom).
     * @static
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    static makeLtrb(left: number, top: number, right: number, bottom: number) : common2D.Rect;

    /**
     * Makes a deep copy of a 2D rectangular object.
     * @param { common2D.Rect } src - Indicates the source rectangle to copy.
     * @returns { common2D.Rect } - Returns an Rect object has the same boundary coordinates with the source.
     * @static
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    static makeCopy(src: common2D.Rect) : common2D.Rect;

    /**
     * Gets the width of a 2D rectangular object.
     *
     * @param { common2D.Rect } rect - Indicates the Rect object to query.
     * @returns { number } - Returns the width.
     * @static
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    static getWidth(rect: common2D.Rect): number;

    /**
     * Gets the height of a 2D rectangular object.
     *
     * @param { common2D.Rect } rect - Indicates the Rect object to query.
     * @returns { number } - Returns the height.
     * @static
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    static getHeight(rect: common2D.Rect): number;

    /**
     * Calculates the x-coordinate of the center point of the 2D rectangular object.
     *
     * @param { common2D.Rect } rect - Indicates the Rect object to query.
     * @returns { number } - Returns the center X coordinate.
     * @static
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    static centerX(rect: common2D.Rect): number;

    /**
     * Calculates the y-coordinate of the center point of the 2D rectangular object.
     *
     * @param { common2D.Rect } rect - Indicates the Rect object to query.
     * @returns { number } - Returns the center Y coordinate.
     * @static
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    static centerY(rect: common2D.Rect): number;

    /**
     * Checks if one 2D rectangular object fully contains another.
     *
     * @param { common2D.Rect } rect - The container Rect object.
     * @param { common2D.Rect } other - The Rect object to check for containment.
     * @returns { boolean } - Returns true if 'rect' fully contains 'other'; returns false otherwise.
     * @static
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    static contains(rect: common2D.Rect, other: common2D.Rect): boolean;

    /**
     * Checks if one 2D rectangular object fully contains the specified boundary coordinates.
     *
     * @param { common2D.Rect } rect - The container Rect object.
     * @param { number } left - Indicates the left boundary of the target region.
     * @param { number } top - Indicates the top boundary of the target region.
     * @param { number } right - Indicates the right boundary of the target region.
     * @param { number } bottom - Indicates the bottom boundary of the target region.
     * @returns { boolean } - Returns true if 'rect' fully contains the specified boundary coordinates; returns false otherwise.
     * @static
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    static contains(rect: common2D.Rect, left: number, top: number, right: number, bottom: number): boolean;

    /**
     * Checks if one 2D rectangular object contains a specific point.
     *
     * @param { common2D.Rect } rect - The container Rect object.
     * @param { number } x - Indicates the X-coordinate of the point to check.
     * @param { number } y - Indicates the Y-coordinate of the point to check.
     * @returns { boolean } - Returns true if 'rect' contains the specified point; returns false otherwise.
     * @static
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    static contains(rect: common2D.Rect, x: number, y: number): boolean;

    /**
     * Modifies a 2D rectangular's boundaries by inward offsets.
     *
     * @param { common2D.Rect } rect - The Rect object to adjust.
     * @param { number } left - Indicates the amount to add from the left boundary.
     * @param { number } top - Indicates the amount to add from the top boundary.
     * @param { number } right - Indicates the amount to substract from the right boundary.
     * @param { number } bottom - Indicates the amount to substract from the bottom boundary.
     * @static
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    static inset(rect: common2D.Rect, left: number, top: number, right: number, bottom: number): void;

    /**
     * Relpace a 2D rectangular object by the intersection of itself and another.
     * If the intersection is empty, nothing is done.
     *
     * @param { common2D.Rect } rect - Indicates the Rect object.
     * @param { common2D.Rect } other - Indicates the other Rect object.
     * @returns { boolean } - Returns true if have area in common and 'rect' will be replaced by the intersection;
     * returns false if the intersection is empty and 'rect' will not be modified.
     * @static
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    static intersect(rect: common2D.Rect, other: common2D.Rect): boolean;

    /**
     * Checks if two 2D rectangular objects intersect.
     *
     * @param { common2D.Rect } rect - Indicates the Rect object.
     * @param { common2D.Rect } other - Indicates the other Rect object.
     * @returns { boolean } - Returns true if have area in common, otherwise return false.
     * @static
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    static isIntersect(rect: common2D.Rect, other: common2D.Rect): boolean;

    /**
     * Sets a 2D rectangular object to the union of itself and another.
     *
     * @param { common2D.Rect } rect - Indicates the Rect object.
     * @param { common2D.Rect } other - Indicates the other Rect object.
     * @static
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    static union(rect: common2D.Rect, other: common2D.Rect): void;

    /**
     * Checks if the 2D rectangular object is empty.
     *
     * @param { common2D.Rect } rect - Indicates the Rect object.
     * @returns { boolean } - Returns true if the rectangle is empty (left >= right or top >= bottom); returns false otherwise.
     * @static
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    static isEmpty(rect: common2D.Rect): boolean;

    /**
     * Offsets the 2D rectangular object by adding dx to its left and right coordinates,
     * and adding dy to its top and bottom coordinates.
     *
     * @param { common2D.Rect } rect - Indicates the Rect object.
     * @param { number } dx - Indicates the amount to add to the rectangle's left and right coordinates.
     * @param { number } dy - Indicates the amount to add to the rectangle's top and bottom coordinates.
     * @static
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    static offset(rect: common2D.Rect, dx: number, dy: number): void;

    /**
     * Offsets the rectangle to a specific position and kepps the width and height unchanged.
     *
     * @param { common2D.Rect } rect - Indicates the Rect object.
     * @param { number } newLeft - Indicates the new left coordinates.
     * @param { number } newTop - Indicates the new top coordinates.
     * @static
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    static offsetTo(rect: common2D.Rect, newLeft: number, newTop: number): void;

    /**
     * Sets the boundary coordinates of a 2D rectangular object with that of another.
     * @param { common2D.Rect } rect - Indicates the Rect object to be modified.
     * @param { common2D.Rect } other - Indicates the source rectangle to copy.
     * @static
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    static setRect(rect: common2D.Rect, other: common2D.Rect): void;

    /**
     * Sets the boundary coordinates of a 2D rectangular object with that of specific value.
     * @param { common2D.Rect } rect - Indicates the Rect object to be modified.
     * @param { number } left - Indicates the X-coordinate of the left edge.
     * @param { number } top - Indicates the Y-coordinate of the top edge.
     * @param { number } right - Indicates the X-coordinate of the right edge.
     * @param { number } bottom - Indicates the Y-coordinate of the bottom edge.
     * @static
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    static setLtrb(rect: common2D.Rect, left: number, top: number, right: number, bottom: number): void;

    /**
     * Sets the boundary coordinates of a 2D rectangular object to zero.
     * @param { common2D.Rect } rect - Indicates the Rect object to be modified.
     * @static
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    static setEmpty(rect: common2D.Rect): void;

    /**
     * Normalizes the 2D rectangular object to ensuer validity.
     * Swaps the left and right if the left is greater than right;
     * and swaps top and bottom if the top is greater than bottom.
     * If the edges are already valid, then nothing is done.
     *
     * @param { common2D.Rect } rect - Indicates the Rect object.
     * @static
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    static sort(rect: common2D.Rect): void;

    /**
     * Checks if the 2D rectangular object has the same coordinates value with another.
     *
     * @param { common2D.Rect } rect - Indicates the Rect object.
     * @param { common2D.Rect } other - Indicates the other Rect object for comparison.
     * @returns { boolean } Returns true if left, top, right and bottom of rect and other are equal; returns false otherwise.
     * @static
     * @syscap SystemCapability.Graphics.Drawing
     * @crossplatform
     * @since 20
     */
    static isEqual(rect: common2D.Rect, other: common2D.Rect): boolean;
  }
}

export default drawing;
