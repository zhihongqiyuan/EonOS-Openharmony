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
 * @file
 * @kit ArkUI
 */
/**
 * Handler of swiper controller, used in finishAnimation.
 *
 * @typedef { function } FinishAnimationHandler
 * @syscap SystemCapability.ArkUI.ArkUI.Circle
 * @crossplatform
 * @atomicservice
 * @since 18
 */
declare type FinishAnimationHandler = () => void;

/**
 * Provide methods for controlling ArcSwiper component.
 *
 * @syscap SystemCapability.ArkUI.ArkUI.Circle
 * @crossplatform
 * @atomicservice
 * @since 18
 */
export class ArcSwiperController {
  /**
   * A constructor used to create a ArcSwiperController object.
   *
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  constructor();

  /**
   * Show next subcomponent.
   *
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  showNext();

  /**
   * Show previous subcomponent.
   *
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  showPrevious();

  /**
   * Finish the swiper animation.
   *
   * @param { FinishAnimationHandler } handler - The handler is used to listen for the end of the animation.
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  finishAnimation(handler?: FinishAnimationHandler);
}

/**
 * Declare the direction of arc indicator.
 *
 * @enum { number }
 * @syscap SystemCapability.ArkUI.ArkUI.Circle
 * @crossplatform
 * @atomicservice
 * @since 18
 */
export enum ArcDirection {
  /**
   * 3 o'clock direction.
   *
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  THREE_CLOCK_DIRECTION = 0,

  /**
   * 6 o'clock direction.
   *
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  SIX_CLOCK_DIRECTION = 1,

  /**
   * 9 o'clock direction.
   *
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  NINE_CLOCK_DIRECTION = 2,
}

/**
 * Define ArcDotIndicator, the indicator type is arc dot.
 *
 * @syscap SystemCapability.ArkUI.ArkUI.Circle
 * @crossplatform
 * @atomicservice
 * @since 18
 */
export class ArcDotIndicator {
  /**
   * A constructor used to create a ArcDotIndicator object.
   *
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  constructor();

  /**
   * Set the direction of arc indicator.
   *
   * @param { Optional<ArcDirection> } direction - the direction of arc indicator, default value is { ArcDirection.SIX_CLOCK_DIRECTION }.
   * @returns { ArcDotIndicator }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  arcDirection(direction: Optional<ArcDirection>): ArcDotIndicator;

  /**
   * Set the navigation point color.
   *
   * @param { Optional<ResourceColor> } color - the indicator item color, default value is { #A9FFFFFF }.
   * @returns { ArcDotIndicator }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  itemColor(color: Optional<ResourceColor>): ArcDotIndicator;

  /**
   * Set the selected navigation point color.
   *
   * @param { Optional<ResourceColor> } color - the indicator item when selected, default value is { #FF5EA1FF }.
   * @returns { ArcDotIndicator }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  selectedItemColor(color: Optional<ResourceColor>): ArcDotIndicator;

  /**
   * Set the background color.
   *
   * @param { Optional<ResourceColor> } color - the background color, default value is { #FF404040 }.
   * @returns { ArcDotIndicator }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  backgroundColor(color: Optional<ResourceColor>): ArcDotIndicator;

  /**
   * Set the gradient color for the mask.
   *
   * @param { Optional<LinearGradient> } color - the gradient color, default start color is { #00000000 }, default end color is { #FF000000 }.
   * @returns { ArcDotIndicator }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  maskColor(color: Optional<LinearGradient>): ArcDotIndicator;
}

/**
 * Provide an interface for ArcSwiper.
 *
 * @interface ArcSwiperInterface
 * @syscap SystemCapability.ArkUI.ArkUI.Circle
 * @crossplatform
 * @atomicservice
 * @since 18
 */
interface ArcSwiperInterface {
  /**
   * Create ArcSwiper component.
   *
   * @param { ArcSwiperController } controller
   * @returns { ArcSwiperAttribute }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  (controller?: ArcSwiperController): ArcSwiperAttribute;
}

/**
 * Handler of swiper, used in OnChange.
 *
 * @typedef { function } IndexChangedHandler
 * @param { number } index - The index of the current swiper.
 * @syscap SystemCapability.ArkUI.ArkUI.Circle
 * @crossplatform
 * @atomicservice
 * @since 18
 */
declare type IndexChangedHandler = (index: number) => void;

/**
 * Handler of swiper, used in OnAnimationStart.
 * 
 * @typedef { function } AnimationStartHandler
 * @param { number } index - The index of the current swiper.
 * @param { number } targetIndex - The index of the target swiper.
 * @param { SwiperAnimationEvent } event - The extra information of the animation.
 * @syscap SystemCapability.ArkUI.ArkUI.Circle
 * @crossplatform
 * @atomicservice
 * @since 18
 */
declare type AnimationStartHandler = (index: number, targetIndex: number, event: SwiperAnimationEvent) => void;

/**
 * Handler of swiper, used in OnAnimationEnd.
 * 
 * @typedef { function } AnimationEndHandler
 * @param { number } index - The index of the current swiper.
 * @param { SwiperAnimationEvent } event - The extra information of the animation.
 * @syscap SystemCapability.ArkUI.ArkUI.Circle
 * @crossplatform
 * @atomicservice
 * @since 18
 */
declare type AnimationEndHandler = (index: number, event: SwiperAnimationEvent) => void;

/**
 * Handler of swiper, used in OnGestureSwipe.
 * 
 * @typedef { function } GestureSwipeHandler
 * @param { number } index - The index of the current swiper.
 * @param { SwiperAnimationEvent } event - The extra information of the animation.
 * @syscap SystemCapability.ArkUI.ArkUI.Circle
 * @crossplatform
 * @atomicservice
 * @since 18
 */
declare type GestureSwipeHandler = (index: number, event: SwiperAnimationEvent) => void;


/**
 * Defines the swiper content animated transition options.
 *
 * @interface SwiperContentAnimatedTransition
 * @syscap SystemCapability.ArkUI.ArkUI.Circle
 * @crossplatform
 * @atomicservice
 * @since 18
 */
declare interface SwiperContentAnimatedTransition {
  /**
   * Defines the timeout of custom content transition animation after the page is moved out of the swiper. The unit is ms.
   * If SwiperContentTransitionProxy.finishTransition() is not invoked, use the timeout as animation end time.
   *
   * @type { ?number }
   * @default 0 ms
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  timeout?: number;

  /**
   * Called when custom content transition animation start.
   *
   * @type { Callback<SwiperContentTransitionProxy> }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  transition: Callback<SwiperContentTransitionProxy>;
}

/**
 *The proxy object returned to the developer during the execution of the Swiper custom content transition animation.
 *
 * @interface SwiperContentTransitionProxy
 * @syscap SystemCapability.ArkUI.ArkUI.Circle
 * @crossplatform
 * @atomicservice
 * @since 18
 */
declare interface SwiperContentTransitionProxy {
  /**
   * the index value of the swiper content selected before animation start.
   *
   * @type { number }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  selectedIndex: number;

  /**
   * The index value of the swiper content.
   *
   * @type { number }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  index: number;

  /**
   * the moving ratio of the swiper content from the start position of the swiper main axis.
   *
   * @type { number }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  position: number;

  /**
   * the swiper main axis length for calculating position.
   *
   * @type { number }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  mainAxisLength: number;

  /**
   * Notifies Swiper page the custom content transition animation is complete.
   *
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  finishTransition(): void;
}

/**
 * Defines the Arc swiper attribute functions.
 *
 * @extends CommonMethod<ArcSwiperAttribute>
 * @syscap SystemCapability.ArkUI.ArkUI.Circle
 * @crossplatform
 * @atomicservice
 * @since 18
 */
declare class ArcSwiperAttribute extends CommonMethod<ArcSwiperAttribute> {
  /**
   * Set the index value of the displayed subcomponent.
   *
   * @param { Optional<number> } index - The index value of the subcomponents to be displayed, default value is { 0 }.
   * @returns { ArcSwiperAttribute }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  index(index: Optional<number>): ArcSwiperAttribute;

  /**
   * Set whether the indicator is available or set the indicator style.
   *
   * @param { Optional<ArcDotIndicator | boolean> } style - The style information of the indicator or whether to
   *                                                display the indicator, default value is { true }.
   * @returns { ArcSwiperAttribute }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  indicator(style: Optional<ArcDotIndicator | boolean>): ArcSwiperAttribute;

  /**
   * Set the animation duration of the switch in ms.
   *
   * @param { Optional<number> } duration - Duration of animation, default value is { 400ms }.
   * @returns { ArcSwiperAttribute }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  duration(duration: Optional<number>): ArcSwiperAttribute;

  /**
   * Set whether to slide vertically.
   *
   * @param { Optional<boolean> } isVertical - The value indicates whether to slide vertically, default value is { false }.
   * @returns { ArcSwiperAttribute }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  vertical(isVertical: Optional<boolean>): ArcSwiperAttribute;

  /**
   * Set whether to disable sliding function.
   *
   * @param { Optional<boolean> } disabled - The value indicates whether the sliding function is enabled, default value is { false }.
   * @returns { ArcSwiperAttribute }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  disableSwipe(disabled: Optional<boolean>): ArcSwiperAttribute;

  /**
   * Set the sensitivity of rotating crown.
   *
   * @param { Optional<CrownSensitivity> } sensitivity - The sensitivity of rotating crown, default value is { MEDIUM }.
   * @returns { ArcSwiperAttribute }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  digitalCrownSensitivity(sensitivity: Optional<CrownSensitivity>): ArcSwiperAttribute;

  /**
   * Called when the index value has changed.
   *
   * @param { Optional<IndexChangedHandler> } handler - The handler is used to listen for index values that have changed.
   * @returns { ArcSwiperAttribute }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  onChange(handler: Optional<IndexChangedHandler>): ArcSwiperAttribute;

  /**
   * Called when the swiper animation has started.
   *
   * @param { Optional<AnimationStartHandler> } handler - The handler is used to listen for the animation has started.
   * @returns { ArcSwiperAttribute }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  onAnimationStart(handler: Optional<AnimationStartHandler>): ArcSwiperAttribute;

  /**
   * Called when the swiper animation has ended.
   *
   * @param { Optional<AnimationEndHandler> } handler - The handler is used to listen for the animation has ended.
   * @returns { ArcSwiperAttribute }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  onAnimationEnd(handler: Optional<AnimationEndHandler>): ArcSwiperAttribute;

  /**
   * Called when swiping the switch using gestures.
   *
   * @param { Optional<GestureSwipeHandler> } handler - The handler is used to listen for swiping through gestures.
   * @returns { ArcSwiperAttribute }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  onGestureSwipe(handler: Optional<GestureSwipeHandler>): ArcSwiperAttribute;

  /**
   * Set effect when scrolling over edge.
   *
   * @param {  Optional<EdgeEffect> } edgeEffect - scrolling effect over edge, default value is { EdgeEffect.Spring }.
   * @returns { ArcSwiperAttribute }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  effectMode(edgeEffect: Optional<EdgeEffect>): ArcSwiperAttribute;

  /**
   * Custom swiper content transition animation.
   *
   * @param { Optional<SwiperContentAnimatedTransition> } transition - custom content transition animation.
   * @returns { ArcSwiperAttribute }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  customContentTransition(transition: Optional<SwiperContentAnimatedTransition>): ArcSwiperAttribute;

  /**
   * Custom swiper content transition animation.
   *
   * @param { Optional<boolean> } disabled - the value indicates whether to disable the transition animation, default value is { false }.
   * @returns { ArcSwiperAttribute }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  disableTransitionAnimation(disabled: Optional<boolean>): ArcSwiperAttribute;
}

/**
 * Defines the ArcSwiper Component that can provide the ability for sub components to swipe and display.
 *
 * @syscap SystemCapability.ArkUI.ArkUI.Circle
 * @crossplatform
 * @atomicservice
 * @since 18
 */
declare let ArcSwiper: ArcSwiperInterface;

/**
 * Defines ArcSwiper Component instance.
 *
 * @syscap SystemCapability.ArkUI.ArkUI.Circle
 * @crossplatform
 * @atomicservice
 * @since 18
 */
declare let ArcSwiperInstance: ArcSwiperAttribute;