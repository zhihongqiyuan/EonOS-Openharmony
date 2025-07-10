/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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
 * @file Defines the arc button component
 * @kit ArkUI
 */

import { ColorMetrics, LengthMetrics } from '@ohos.arkui.node';

 /**
 * Describes the Position of arc button.
 *
 * @enum { number }
 * @syscap SystemCapability.ArkUI.ArkUI.Circle
 * @crossplatform
 * @atomicservice
 * @since 18
 */
export declare enum ArcButtonPosition {
    /**
     * The top edge button is located above the circular screen.
     *
     * @syscap SystemCapability.ArkUI.ArkUI.Circle
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    TOP_EDGE = 0,
    /**
     * The bottom edge button is located below the circular screen.
     *
     * @syscap SystemCapability.ArkUI.ArkUI.Circle
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    BOTTOM_EDGE = 1
}

/**
 * Provides an arc button style.
 *
 * @enum { number }
 * @syscap SystemCapability.ArkUI.ArkUI.Circle
 * @crossplatform
 * @atomicservice
 * @since 18
 */
export declare enum ArcButtonStyleMode {
    /**
     * Emphasis status, light style.
     *
     * @syscap SystemCapability.ArkUI.ArkUI.Circle
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    EMPHASIZED_LIGHT = 0,
    /**
     * Emphasis status, dark style.
     *
     * @syscap SystemCapability.ArkUI.ArkUI.Circle
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    EMPHASIZED_DARK = 1,
    /**
     * Normal status, light style.
     *
     * @syscap SystemCapability.ArkUI.ArkUI.Circle
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    NORMAL_LIGHT = 2,
    /**
     * Normal status, dark style.
     *
     * @syscap SystemCapability.ArkUI.ArkUI.Circle
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    NORMAL_DARK = 3,
    
    /**
     * Custom status, custom style.
     *
     * @syscap SystemCapability.ArkUI.ArkUI.Circle
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    CUSTOM = 4
}

/**
 * Defines an arc button status.
 *
 * @enum { number }
 * @syscap SystemCapability.ArkUI.ArkUI.Circle
 * @crossplatform
 * @atomicservice
 * @since 18
 */
export declare enum ArcButtonStatus {
    /**
     * Normal state.
     *
     * @syscap SystemCapability.ArkUI.ArkUI.Circle
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    NORMAL = 0,
    /**
     * Pressed state.
     *
     * @syscap SystemCapability.ArkUI.ArkUI.Circle
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    PRESSED = 1,
    /**
     * Disabled state.
     *
     * @syscap SystemCapability.ArkUI.ArkUI.Circle
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    DISABLED = 2
}

/**
 * Defines the arc button options.
 *
 * @interface CommonArcButtonOptions
 * @syscap SystemCapability.ArkUI.ArkUI.Circle
 * @crossplatform
 * @atomicservice
 * @since 18
 */
interface CommonArcButtonOptions {
    /**
     * Describes the arc button position.
     *
     * @type { ? ArcButtonPosition}
     * @syscap SystemCapability.ArkUI.ArkUI.Circle
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    position?: ArcButtonPosition;
    /**
     * Describes the arc button style mode.
     *
     * @type { ?ArcButtonStyleMode }
     * @syscap SystemCapability.ArkUI.ArkUI.Circle
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    styleMode?: ArcButtonStyleMode;
    /**
     * Describes the arc button status.
     *
     * @type { ?ArcButtonStatus }
     * @syscap SystemCapability.ArkUI.ArkUI.Circle
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    status?: ArcButtonStatus;
    /**
     * Describes the arc button displays text.
     *
     * @type { ?ResourceStr }
     * @syscap SystemCapability.ArkUI.ArkUI.Circle
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    label?: ResourceStr;
    /**
     * Describe the blurred background style of the arc-shaped button.
     *
     * @type { ?BlurStyle }
     * @syscap SystemCapability.ArkUI.ArkUI.Circle
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    backgroundBlurStyle?: BlurStyle;
    /**
     * Describes the arc button background color.
     *
     * @type { ?ColorMetrics }
     * @syscap SystemCapability.ArkUI.ArkUI.Circle
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    backgroundColor?: ColorMetrics;
    /**
     * Describes the arc button shadow color.
     *
     * @type { ?ColorMetrics }
     * @syscap SystemCapability.ArkUI.ArkUI.Circle
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    shadowColor?: ColorMetrics;
    /**
     * Describes the arc button shadow switch.
     *
     * @type { ?boolean }
     * @syscap SystemCapability.ArkUI.ArkUI.Circle
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    shadowEnabled?: boolean;
    /**
     * Describes the arc button text size.
     *
     * @type { ?LengthMetrics }
     * @syscap SystemCapability.ArkUI.ArkUI.Circle
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    fontSize?: LengthMetrics;
    /**
     * Describes the arc button text color.
     *
     * @type { ?ColorMetrics }
     * @syscap SystemCapability.ArkUI.ArkUI.Circle
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    fontColor?: ColorMetrics;
    /**
     * Describes the arc button pressed text color.
     *
     * @type { ?ColorMetrics }
     * @syscap SystemCapability.ArkUI.ArkUI.Circle
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    pressedFontColor?: ColorMetrics;
    /**
     * Describes the arc button text style.
     *
     * @type { ?FontStyle }
     * @syscap SystemCapability.ArkUI.ArkUI.Circle
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    fontStyle?: FontStyle;
    /**
     * Describes the arc button text family.
     *
     * @type { ?(string | Resource) }
     * @syscap SystemCapability.ArkUI.ArkUI.Circle
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    fontFamily?: string | Resource;
    /**
     * Describes the arc button text margin.
     *
     * @type { ?LocalizedMargin }
     * @syscap SystemCapability.ArkUI.ArkUI.Circle
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    fontMargin?: LocalizedMargin;
    
    /**
     * Describes the arc button touch event.
     *
     * @type { ?Callback<TouchEvent> }
     * @syscap SystemCapability.ArkUI.ArkUI.Circle
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    onTouch?: Callback<TouchEvent>;
    
    /**
     * Describes the arc button click event.
     *
     * @type { ?Callback<ClickEvent> }
     * @syscap SystemCapability.ArkUI.ArkUI.Circle
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    onClick?: Callback<ClickEvent>;
}
/**
 * The class for ArcButtonOptions.
 *
 * @syscap SystemCapability.ArkUI.ArkUI.Circle
 * @crossplatform
 * @atomicservice
 * @since 18
 */
export declare class ArcButtonOptions {
    /**
     * Describes the position of button on screen.
     *
     * @type { ArcButtonPosition }
     * @syscap SystemCapability.ArkUI.ArkUI.Circle
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    position: ArcButtonPosition;
    /**
     * Describes the controller for scrollable container.
     *
     * @type { ArcButtonStyleMode }
     * @syscap SystemCapability.ArkUI.ArkUI.Circle
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    styleMode: ArcButtonStyleMode;
    /**
     * Describes the arc button status.
     *
     * @type { ArcButtonStatus }
     * @syscap SystemCapability.ArkUI.ArkUI.Circle
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    status: ArcButtonStatus;
    /**
     * Describes the arc button displays text.
     *
     * @type { ResourceStr }
     * @syscap SystemCapability.ArkUI.ArkUI.Circle
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    label: ResourceStr;
    /**
     * Describe the blurred background style of the arc-shaped button.
     *
     * @type { BlurStyle }
     * @syscap SystemCapability.ArkUI.ArkUI.Circle
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    backgroundBlurStyle: BlurStyle;
    /**
     * Describes the arc button background color.
     *
     * @type { ColorMetrics }
     * @syscap SystemCapability.ArkUI.ArkUI.Circle
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    backgroundColor: ColorMetrics;
    /**
     * Describes the arc button shadow color.
     *
     * @type { ColorMetrics }
     * @syscap SystemCapability.ArkUI.ArkUI.Circle
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    shadowColor: ColorMetrics;
    /**
     * Describes the arc button shadow switch.
     *
     * @type { boolean }
     * @syscap SystemCapability.ArkUI.ArkUI.Circle
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    shadowEnabled: boolean;
    /**
     * Describes the arc button text size.
     *
     * @type { LengthMetrics }
     * @syscap SystemCapability.ArkUI.ArkUI.Circle
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    fontSize: LengthMetrics;
    /**
     * Describes the arc button text color.
     *
     * @type { ColorMetrics }
     * @syscap SystemCapability.ArkUI.ArkUI.Circle
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    fontColor: ColorMetrics;
    /**
     * Describes the arc button pressed text color.
     *
     * @type { ColorMetrics }
     * @syscap SystemCapability.ArkUI.ArkUI.Circle
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    pressedFontColor: ColorMetrics;
    /**
     * Describes the arc button text style.
     *
     * @type { FontStyle }
     * @syscap SystemCapability.ArkUI.ArkUI.Circle
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    fontStyle: FontStyle;
    /**
     * Describes the arc button text family.
     *
     * @type { (string | Resource) }
     * @syscap SystemCapability.ArkUI.ArkUI.Circle
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    fontFamily: string | Resource;
    /**
     * Describes the arc button text margin.
     *
     * @type { LocalizedMargin }
     * @syscap SystemCapability.ArkUI.ArkUI.Circle
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    fontMargin: LocalizedMargin;
    
    /**
     * Describes the arc button touch event.
     *
     * @type { ?Callback<TouchEvent> }
     * @syscap SystemCapability.ArkUI.ArkUI.Circle
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    onTouch?: Callback<TouchEvent>;
    
    /**
     * Describes the arc button click event.
     *
     * @type { ?Callback<ClickEvent> }
     * @syscap SystemCapability.ArkUI.ArkUI.Circle
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    onClick?: Callback<ClickEvent>;
    
    /**
     * Constructor of the CommonArcButtonOptions.
     *
     * @param { CommonArcButtonOptions } options
     * @syscap SystemCapability.ArkUI.ArkUI.Circle
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    constructor(options: CommonArcButtonOptions);
}
/**
 * Declare Component ArcButton
 *
 * @syscap SystemCapability.ArkUI.ArkUI.Circle
 * @crossplatform
 * @atomicservice
 * @since 18
 */
@Component
export declare struct ArcButton {
    /**
     * Describes the arc button options.
     *
     * @type { ArcButtonOptions }
     * @syscap SystemCapability.ArkUI.ArkUI.Circle
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    @Require
    readonly options: ArcButtonOptions;
}
