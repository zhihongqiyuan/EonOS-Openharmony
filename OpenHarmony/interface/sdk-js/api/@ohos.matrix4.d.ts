/*
 * Copyright (c) 2020-2023 Huawei Device Co., Ltd.
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
 * @kit ArkUI
 */

/**
 * Used to do matrix operations
 *
 * @namespace matrix4
 * @syscap SystemCapability.ArkUI.ArkUI.Full
 * @since 7
 */
/**
 * Used to do matrix operations
 *
 * @namespace matrix4
 * @syscap SystemCapability.ArkUI.ArkUI.Full
 * @crossplatform
 * @since 10
 */
/**
 * Used to do matrix operations
 *
 * @namespace matrix4
 * @syscap SystemCapability.ArkUI.ArkUI.Full
 * @crossplatform
 * @atomicservice
 * @since 11
 */
declare namespace matrix4 {
  /**
   * Set translation parameters
   *
   * @interface TranslateOption
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @since 7
   */
  /**
   * Set translation parameters
   *
   * @interface TranslateOption
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @since 10
   */
  /**
   * Set translation parameters
   *
   * @interface TranslateOption
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  interface TranslateOption {
    /**
     * Indicates the translation distance of the x-axis, in px.
     *
     * @type { ?number }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @since 7
     */
    /**
     * Indicates the translation distance of the x-axis, in px.
     *
     * @type { ?number }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @since 10
     */
    /**
     * Translation distance along the x-axis.
     * <br>Unit: px.
     * <br>Default value: **0**.
     * <br>Value range: (-∞, +∞).
     *
     * @type { ?number }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    x?: number;

    /**
     * Indicates the translation distance of the y-axis, in px.
     *
     * @type { ?number }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @since 7
     */
    /**
     * Indicates the translation distance of the y-axis, in px.
     *
     * @type { ?number }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @since 10
     */
    /**
     * Translation distance along the y-axis.
     * <br>Unit: px.
     * <br>Default value: **0**.
     * <br>Value range: (-∞, +∞).
     *
     * @type { ?number }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    y?: number;

    /**
     * Indicates the translation distance of the z-axis, in px.
     *
     * @type { ?number }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @since 7
     */
    /**
     * Indicates the translation distance of the z-axis, in px.
     *
     * @type { ?number }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @since 10
     */
    /**
     * Translation distance along the z-axis.
     * <br>Unit: px.
     * <br>Default value: **0**.
     * <br>Value range: (-∞, +∞).
     *
     * @type { ?number }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    z?: number;
  }

  /**
   * Set scaling parameters
   *
   * @interface ScaleOption
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @since 7
   */
  /**
   * Set scaling parameters
   *
   * @interface ScaleOption
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @since 10
   */
  /**
   * Set scaling parameters
   *
   * @interface ScaleOption
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  interface ScaleOption {
    /**
     * Zoom factor of the x-axis.
     *
     * @type { ?number }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @since 7
     */
    /**
     * Zoom factor of the x-axis.
     *
     * @type { ?number }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @since 10
     */
    /**
     * Scaling multiple along the x-axis. x > 1: The image is scaled up along the x-axis.
     * <br>0 < x < 1: The image is scaled down along the x-axis.
     * <br>x < 0: The image is scaled in the reverse direction of the x-axis.
     * <br>Default value: **1**.
     * <br>Value range: (-∞, +∞).
     *
     * @type { ?number }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    x?: number;

    /**
     * Zoom factor of the y-axis.
     *
     * @type { ?number }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @since 7
     */
    /**
     * Zoom factor of the y-axis.
     *
     * @type { ?number }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @since 10
     */
    /**
     * Scaling multiple along the y-axis. y > 1: The image is scaled up along the y-axis.
     * <br>0 < y < 1: The image is scaled down along the y-axis.
     * <br>y < 0: The image is scaled in the reverse direction of the y-axis.
     * <br>Default value: **1**.
     * <br>Value range: (-∞, +∞).
     *
     * @type { ?number }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    y?: number;

    /**
     * Zoom factor of the z-axis.
     *
     * @type { ?number }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @since 7
     */
    /**
     * Zoom factor of the z-axis.
     *
     * @type { ?number }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @since 10
     */
    /**
     * Scaling multiple along the z-axis. z > 1: The image is scaled up along the z-axis.
     * <br>0 < z < 1: The image is scaled down along the z-axis.
     * <br>z < 0: The image is scaled in the reverse direction of the z-axis.
     * <br>Default value: **1**.
     * <br>Value range: (-∞, +∞).
     *
     * @type { ?number }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    z?: number;

    /**
     * Transform the x-axis coordinate of the center point.
     *
     * @type { ?number }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @since 7
     */
    /**
     * Transform the x-axis coordinate of the center point.
     *
     * @type { ?number }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @since 10
     */
    /**
     * X coordinate of the center point.
     * <br>Unit: px.
     * <br>Default value: X-coordinate of the component center.
     * <br>Value range: (-∞, +∞).
     *
     * @type { ?number }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    centerX?: number;

    /**
     * Transform the y-axis coordinate of the center point.
     *
     * @type { ?number }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @since 7
     */
    /**
     * Transform the y-axis coordinate of the center point.
     *
     * @type { ?number }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @since 10
     */
    /**
     * Y coordinate of the center point.
     * <br>Unit: px.
     * <br>Default value: Y-coordinate of the component center.
     * <br>Value range: (-∞, +∞).
     *
     * @type { ?number }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    centerY?: number;
  }

  /**
   * Set Rotation Parameters.
   *
   * @interface RotateOption
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @since 7
   */
  /**
   * Set Rotation Parameters.
   *
   * @interface RotateOption
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @since 10
   */
  /**
   * Set Rotation Parameters.
   *
   * @interface RotateOption
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  interface RotateOption {
    /**
     * Axis of rotation vector x coordinate.
     *
     * @type { ?number }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @since 7
     */
    /**
     * Axis of rotation vector x coordinate.
     *
     * @type { ?number }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @since 10
     */
    /**
     * X coordinate of the rotation axis vector.
     * <br>Default value: **0**.
     * <br>Value range: (-∞, +∞).
     *
     * @type { ?number }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    x?: number;

    /**
     * Axis of rotation vector y coordinate.
     *
     * @type { ?number }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @since 7
     */
    /**
     * Axis of rotation vector y coordinate.
     *
     * @type { ?number }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @since 10
     */
    /**
     * Y coordinate of the rotation axis vector.
     * <br>Default value: **0**.
     * <br>Value range: (-∞, +∞).
     *
     * @type { ?number }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    y?: number;

    /**
     * Axis of rotation vector z coordinate.
     *
     * @type { ?number }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @since 7
     */
    /**
     * Axis of rotation vector z coordinate.
     *
     * @type { ?number }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @since 10
     */
    /**
     * Z coordinate of the rotation axis vector.
     * <br>Default value: **0**.
     * <br>Value range: (-∞, +∞).
     * <br>**NOTE**<br>The rotation axis vector is valid only when at least one of **x**, **y**, and **z** is not 0.
     *
     * @type { ?number }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    z?: number;

    /**
     * Transform the x-axis coordinate of the center point.
     *
     * @type { ?number }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @since 7
     */
    /**
     * Transform the x-axis coordinate of the center point.
     *
     * @type { ?number }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @since 10
     */
    /**
     * Additional x-axis offset of the transformation center relative to the component's anchor.
     * <br>Unit: px.
     * <br>Default value: **0**.
     * <p>**NOTE**:
     * <br>The value **0** indicates that the transformation center coincides with the component's x-axis anchor. 
     * </p>
     *
     * @type { ?number }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    centerX?: number;

    /**
     * Transform the y-axis coordinate of the center point.
     *
     * @type { ?number }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @since 7
     */
    /**
     * Transform the y-axis coordinate of the center point.
     *
     * @type { ?number }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @since 10
     */
    /**
     * Additional y-axis offset of the transformation center relative to the component's anchor.
     * <br>Unit: px.
     * <br>Default value: **0**.
     * <p>**NOTE**
     * <br>The value **0** indicates that the transformation center coincides with the component's y-axis anchor.
     * </p>
     *
     * @type { ?number }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    centerY?: number;

    /**
     * Rotation angle.
     *
     * @type { ?number }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @since 7
     */
    /**
     * Rotation angle.
     *
     * @type { ?number }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @since 10
     */
    /**
     * Rotation angle.
     * <br>Default value: **0**.
     *
     * @type { ?number }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    angle?: number;
  }


  /**
   * Set poly to poly point.
   *
   * @interface Point
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 12
   */
  export interface Point {

    /**
     * Point x.
     *
     * @type { number }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @atomicservice
     * @since 12
     */
    x: number;

    /**
     * Point y.
     *
     * @type { number }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @atomicservice
     * @since 12
     */
    y: number;
  }

  /**
   * Set poly to poly point options.
   *
   * @interface PolyToPolyOptions
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 12
   */
  export interface PolyToPolyOptions {

    /**
     * Array of point coordinates for the source polygon.
     *
     * @type { Array<Point> }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @atomicservice
     * @since 12
     */
    src: Array<Point>;

    /**
     * Start point index of the source polygon, which defaults to 0.
     * @type { ?number }
     * @default 0
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @atomicservice
     * @since 12
     */
    srcIndex?: number;

    /**
     * Array of point coordinates for the target polygon.
     *
     * @type { Array<Point> }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @atomicservice
     * @since 12
     */
    dst:Array<Point>;

    /**
     * Start index of the target polygon, which defaults to 0.
     *
     * @type { ?number }
     * @default src.Length/2
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @atomicservice
     * @since 12
     */
    dstIndex?: number;

    /**
     * The number of points to be used.
     * If it is 0, it returns the identity matrix.
     * If it is 1, it returns a translation matrix that changed before two points.
     * If it is 2-4, it returns a transformation matrix.
     * @type { ?number }
     * @default 0
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @atomicservice
     * @since 12
     */
    pointCount?:number;

  }
  /**
   * Implements a **Matrix4Transit** object.
   *
   * @interface Matrix4Transit
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @since 7
   */
  /**
   * Implements a **Matrix4Transit** object.
   *
   * @interface Matrix4Transit
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @since 10
   */
  /**
   * Implements a **Matrix4Transit** object.
   *
   * @interface Matrix4Transit
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  interface Matrix4Transit {
    /**
     * Copy function of Matrix, which can copy a copy of the current matrix object.
     *
     * @returns { Matrix4Transit } Return to Matrix4Transit
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @since 7
     */
    /**
     * Copy function of Matrix, which can copy a copy of the current matrix object.
     *
     * @returns { Matrix4Transit } Return to Matrix4Transit
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @since 10
     */
    /**
     * Copies this matrix object.
     *
     * @returns { Matrix4Transit } Copy object of the current matrix.
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    copy(): Matrix4Transit;

    /**
     * The inverse function of Matrix returns an inverse matrix of the current matrix object, that is, the effect is exactly the opposite.
     *
     * @returns { Matrix4Transit } Return to Matrix4Transit
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @since 7
     */
    /**
     * The inverse function of Matrix returns an inverse matrix of the current matrix object, that is, the effect is exactly the opposite.
     *
     * @returns { Matrix4Transit } Return to Matrix4Transit
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @since 10
     */
    /**
     * Inverts this matrix object. The original matrix that calls this API will be changed.
     *
     * @returns { Matrix4Transit } Inverse matrix object of the current matrix.
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    invert(): Matrix4Transit;

    /**
     * Matrix superposition function, which can superpose the effects of two matrices to generate a new matrix object.
     *
     * @param { Matrix4Transit } options
     * @returns { Matrix4Transit } Return to Matrix4Transit
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @since 7
     */
    /**
     * Matrix superposition function, which can superpose the effects of two matrices to generate a new matrix object.
     *
     * @param { Matrix4Transit } options
     * @returns { Matrix4Transit } Return to Matrix4Transit
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @since 10
     */
    /**
     * Combines the effects of two matrices to generate a new matrix object. The original matrix that calls this API will be changed.
     *
     * @param { Matrix4Transit } options - Matrix object to be combined.
     * @returns { Matrix4Transit } Object after matrix combination.
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    combine(options: Matrix4Transit): Matrix4Transit;

    /**
     * Matrix translation function, which can add the x-axis, Y-axis, or Z-axis translation effect to the current matrix.
     *
     * @param { TranslateOption } options
     * @returns { Matrix4Transit } Return to Matrix4Transit
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @since 7
     */
    /**
     * Matrix translation function, which can add the x-axis, Y-axis, or Z-axis translation effect to the current matrix.
     *
     * @param { TranslateOption } options
     * @returns { Matrix4Transit } Return to Matrix4Transit
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @since 10
     */
    /**
     * Translates this matrix object along the x, y, and z axes. The original matrix that calls this API will be changed.
     *
     * @param { TranslateOption } options - Translation configuration.
     * @returns { Matrix4Transit } Matrix object after the translation.
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    translate(options: TranslateOption): Matrix4Transit;

    /**
     * Scaling function of the Matrix, which can add the x-axis, Y-axis, or Z-axis scaling effect to the current matrix.
     *
     * @param { ScaleOption } options
     * @returns { Matrix4Transit } Return to Matrix4Transit
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @since 7
     */
    /**
     * Scaling function of the Matrix, which can add the x-axis, Y-axis, or Z-axis scaling effect to the current matrix.
     *
     * @param { ScaleOption } options
     * @returns { Matrix4Transit } Return to Matrix4Transit
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @since 10
     */
    /**
     * Scales this matrix object along the x, y, and z axes. The matrix that calls this API will be changed.
     *
     * @param { ScaleOption } options - Scaling configuration.
     * @returns { Matrix4Transit } Matrix object after the scaling.
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    scale(options: ScaleOption): Matrix4Transit;

    /**
     * Skews this matrix object along the x and y axes. The matrix that calls this API will be changed.
     *
     * @param { number } x - Amount of skewing on the x-axis.
     * @param { number } y - Amount of skewing on the y-axis.
     * @returns { Matrix4Transit } Matrix object after the skewing.
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @atomicservice
     * @since 12
     */
    skew(x: number, y: number): Matrix4Transit;

    /**
     * Rotation function of the Matrix. You can add the x-axis, Y-axis, or Z-axis rotation effect to the current matrix.
     *
     * @param { RotateOption } options
     * @returns { Matrix4Transit } Return to Matrix4Transit
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @since 7
     */
    /**
     * Rotation function of the Matrix. You can add the x-axis, Y-axis, or Z-axis rotation effect to the current matrix.
     *
     * @param { RotateOption } options
     * @returns { Matrix4Transit } Return to Matrix4Transit
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @since 10
     */
    /**
     * Rotates this matrix object along the x, y, and z axes. The matrix that calls this API will be changed.
     *
     * @param { RotateOption } options - Rotation configuration.
     * @returns { Matrix4Transit } Matrix object after the rotation.
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    rotate(options: RotateOption): Matrix4Transit;

    /**
     * Matrix coordinate point conversion function, which can apply the current transformation effect to a coordinate point.
     *
     * @param { [number, number] } options
     * @returns { [number, number] } Return to Matrix4Transit
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @since 7
     */
    /**
     * Matrix coordinate point conversion function, which can apply the current transformation effect to a coordinate point.
     *
     * @param { [number, number] } options
     * @returns { [number, number] } Return to Matrix4Transit
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @since 10
     */
    /**
     * Applies the current transformation effect to a coordinate point.
     *
     * @param { [number, number] } options - Point to be transformed.
     * @returns { [number, number] } Point object after matrix transformation
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    transformPoint(options: [number, number]): [number, number];

    /**
     * Sets matrix to map src to dst.
     *
     * @param { PolyToPolyOptions } options - polyToPoly options
     * @returns { Matrix4Transit } Return to Matrix4Transit
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @atomicservice
     * @since 12
     */
    setPolyToPoly(options: PolyToPolyOptions): Matrix4Transit

  }

  /**
   * Constructor of Matrix, which can create a fourth-order matrix based on the input parameters. The matrix is column-first.
   *
   * @param { [number,number,number,number,number,number,number,number,number,number,number,number,number,number,number,number] } options
   * options indicates a fourth-order matrix
   * The default value：
   * [1, 0, 0, 0,
   * 0, 1, 0, 0,
   * 0, 0, 1, 0,
   * 0, 0, 0, 1]
   * @returns { Matrix4Transit } Return to Matrix4Transit
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @since 7
   */
  /**
   * Constructor of Matrix, which can create a fourth-order matrix based on the input parameters. The matrix is column-first.
   *
   * @param { [number,number,number,number,number,number,number,number,number,number,number,number,number,number,number,number] } options
   * options indicates a fourth-order matrix
   * The default value：
   * [1, 0, 0, 0,
   * 0, 1, 0, 0,
   * 0, 0, 1, 0,
   * 0, 0, 0, 1]
   * @returns { Matrix4Transit } Return to Matrix4Transit
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @since 10
   */
  /**
   * Constructor of Matrix, which can create a fourth-order matrix based on the input parameters. The matrix is column-first.
   *
   * @param { [number,number,number,number,number,number,number,number,number,number,number,number,number,number,number,number] } options
   * options indicates a fourth-order matrix
   * The default value：
   * [1, 0, 0, 0,
   * 0, 1, 0, 0,
   * 0, 0, 1, 0,
   * 0, 0, 0, 1]
   * Fourth-order matrix notes:
   * m00 { number } -The x-axis scale value, the identity matrix defaults to 1.
   * m01 { number } -The second value, the rotation of the xyz axis affects this value.
   * m02 { number } -The third value, the rotation of the xyz axis affects this value.
   * m03 { number } -Meaningless.
   * m10 { number } -The fifth value, the rotation of the xyz axis affects this value.
   * m11 { number } -The y-axis scales the value, and the identity matrix defaults to 1.
   * m12 { number } -The 7th value, the rotation of the xyz axis affects this value.
   * m13 { number } -Meaningless.
   * m20 { number } -The 9th value, the rotation of the xyz axis affects this value.
   * m21 { number } -The 10th value, xyz axis rotation affects this value.
   * m22 { number } -The z-axis scale value, the identity matrix defaults to 1.
   * m23 { number } -Meaningless.
   * m30 { number } -The x-axis translation value in px, the identity matrix defaults to 0.
   * m31 { number } -Y-axis translation value, in px, the identity matrix defaults to 0.
   * m32 { number } -The z-axis translation value in px, the identity matrix defaults to 0.
   * m33 { number } -It takes effect in homogeneous coordinates to produce a perspective projection effect.
   * @returns { Matrix4Transit } Return to Matrix4Transit
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  /**
   * Matrix constructor, which is used to create a 4 x 4 matrix with the input parameters. Column-major order is used.
   *
   * @param { [number,number,number,number,number,number,number,number,number,number,number,number,number,number,number,number] } options
   * A number array whose length is 16 (4 x 4). For details, see **4 x 4 matrix description**.
   * <br>Value range of each number: (-∞, +∞).
   * <br>Default value:<br>[1, 0, 0, 0,<br>0, 1, 0, 0,<br>0, 0, 1, 0,<br>0, 0, 0, 1].
   * Fourth-order matrix notes:
   * m00 { number } -Scaling value of the x-axis. The default value is **1** for the identity matrix.
   * m01 { number } -The second value, which is affected by the rotation or tilt of the x, y, and z axes.
   * m02 { number } -The third value, which is affected by the rotation of the x, y, and z axes.
   * m03 { number } -The fourth value, which is affected by perspective projection.
   * m10 { number } -The fifth value, which is affected by the rotation or tilt of the x, y, and z axes.
   * m11 { number } -Scaling value of the y-axis. The default value is **1** for the identity matrix.
   * m12 { number } -The seventh value, which is affected by the rotation of the x, y, and z axes.
   * m13 { number } -The eighth value, which is affected by perspective projection.
   * m20 { number } -The ninth value, which is affected by the rotation of the x, y, and z axes.
   * m21 { number } -The tenth value, which is affected by the rotation of the x, y, and z axes.
   * m22 { number } -Scaling value of the z-axis. The default value is **1** for the identity matrix.
   * m23 { number } -The 12th value, which is affected by perspective projection.
   * m30 { number } -Translation value of the x-axis, in px. The default value is **0** for the identity matrix.
   * m31 { number } -Translation value of the y-axis, in px. The default value is **0** for the identity matrix.
   * m32 { number } -Translation value of the z-axis, in px. The default value is **0** for the identity matrix.
   * m33 { number } -Valid in homogeneous coordinates, presenting the perspective projection effect.
   * @returns { Matrix4Transit } Return to Matrix4Transit
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 12
   */
  function init(
    options: [
      number,
      number,
      number,
      number,
      number,
      number,
      number,
      number,
      number,
      number,
      number,
      number,
      number,
      number,
      number,
      number
    ]
  ): Matrix4Transit;

  /**
   * Matrix initialization function, which can return an identity matrix object.
   *
   * @returns { Matrix4Transit } Return to Matrix4Transit
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @since 7
   */
  /**
   * Matrix initialization function, which can return an identity matrix object.
   *
   * @returns { Matrix4Transit } Return to Matrix4Transit
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @since 10
   */
  /**
   * Constructs an identity matrix.
   *
   * @returns { Matrix4Transit } Identity matrix object.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  function identity(): Matrix4Transit;

  /**
   * Copy function of Matrix, which can copy a copy of the current matrix object.
   *
   * @returns { Matrix4Transit } Return to Matrix4Transit
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @since 7
   * @deprecated since 10
   */
  function copy(): Matrix4Transit;

  /**
   * The inverse function of Matrix returns an inverse matrix of the current matrix object, that is, the effect is exactly the opposite.
   *
   * @returns { Matrix4Transit } Return to Matrix4Transit
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @since 7
   * @deprecated since 10
   */
  function invert(): Matrix4Transit;

  /**
   * Matrix superposition function, which can superpose the effects of two matrices to generate a new matrix object.
   *
   * @param { Matrix4Transit } options
   * @returns { Matrix4Transit } Return to Matrix4Transit
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @since 7
   * @deprecated since 10
   */
  function combine(options: Matrix4Transit): Matrix4Transit;

  /**
   * Matrix translation function, which can add the x-axis, Y-axis, or Z-axis translation effect to the current matrix.
   *
   * @param { TranslateOption } options
   * @returns { Matrix4Transit } Return to Matrix4Transit
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @since 7
   * @deprecated since 10
   */
  function translate(options: TranslateOption): Matrix4Transit;

  /**
   * Scaling function of the Matrix, which can add the x-axis, Y-axis, or Z-axis scaling effect to the current matrix.
   *
   * @param { ScaleOption } options
   * @returns { Matrix4Transit } Return to Matrix4Transit
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @since 7
   * @deprecated since 10
   */
  function scale(options: ScaleOption): Matrix4Transit;

  /**
   * Rotation function of the Matrix. You can add the x-axis, Y-axis, or Z-axis rotation effect to the current matrix.
   *
   * @param { RotateOption } options
   * @returns { Matrix4Transit } Return to Matrix4Transit
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @since 7
   * @deprecated since 10
   */
  function rotate(options: RotateOption): Matrix4Transit;

  /**
   * Matrix coordinate point conversion function, which can apply the current transformation effect to a coordinate point.
   *
   * @param { [number, number] } options
   * @returns { [number, number] } Return to Matrix4Transit
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @since 7
   * @deprecated since 10
   */
  function transformPoint(options: [number, number]): [number, number];
}

export default matrix4;
