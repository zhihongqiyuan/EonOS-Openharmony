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
 * @kit ArkGraphics2D
 */
import type drawing from './@ohos.graphics.drawing';
import type common2D from './@ohos.graphics.common2D';

/**
 * The Text module provides a set of APIs for text layout and font management. 
 * It aims to deliver high-quality typesetting through features like character-to-glyph
 * conversion, kerning, line breaking, alignment, and text measurement. Additionally,
 * it provides font management capabilities, including font registration, font descriptors,
 * and font collection management.
 * 
 * This module provides the following classes for creating complex text paragraphs:
 * 
 * TextStyle: defines the font type, size, spacing, and other text properties.
 * FontCollection: manages a collection of different fonts.
 * FontDescriptor: provides information about font descriptors.
 * ParagraphStyle: controls line break and word break strategies for the entire paragraph.
 * ParagraphBuilder: used to create different paragraph objects.
 * Paragraph: created by calling build() of the ParagraphBuilder class.
 * LineTypeset: created by calling buildLineTypeset() of the ParagraphBuilder class.
 * TextLine: paragraph text on a line-by-line basis, obtained by calling getTextLines() of the Paragraph class.
 * Run: text typesetting unit, obtained by calling getGlyphRuns() of the TextLine class.
 *
 * @namespace text
 * @syscap SystemCapability.Graphics.Drawing
 * @since 12
 */
declare namespace text {

  /**
   * Refers to how to align the horizontal position of text when displaying text.
   * @enum { number }
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  enum TextAlign {
    /**
     * Use the left side of the text as a reference line for alignment.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    LEFT = 0,

    /**
     * Use the right side of the text as a reference line for alignment.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    RIGHT = 1,

    /**
     * Use the midpoint line the text as a reference line for alignment.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    CENTER = 2,

    /**
     * Justified, which means that each line (except the last line) is stretched so that every line has equal width,
     * and the left and right margins are straight.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    JUSTIFY = 3,

    /**
     * Align text from start, based on the TextDirection, such as left-to-right or right-to-left.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    START = 4,

    /**
     * Align text from end, based on the TextDirection, such as left-to-right or right-to-left, opposite to START.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    END = 5,
  }

  /**
   * Enumerate text runs direction.
   * @enum { number }
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  enum TextDirection {
    /**
     * The text is oriented from right to left.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    RTL,

    /**
     * The text is oriented from left to right.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    LTR,
  }

  /**
   * Enumerate text segmentation strategy.
   * @enum { number }
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  enum BreakStrategy {
    /**
     * Fills the current line as much as possible without adding hyphens.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    GREEDY,

    /**
     * Optimizes layout and may add hyphens when necessary.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    HIGH_QUALITY,

    /**
     * Ensures consistent line width in a paragraph, adding hyphens if needed.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    BALANCED,
  }

  /**
   * Enumerate word break strategy.
   * @enum { number }
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  enum WordBreak {
    /**
     * Default mode that break words based on language-specific conventions.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    NORMAL,

    /**
     * Allows breaks within any character in non-CJK text. (CJK means Chinese, Japanese, and Korean.)
     * This value is suitable for Asian text that contains some non-Asian text. For example, 
     * it can be used to break consecutive English characters.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    BREAK_ALL,

    /**
     * Allows breaks between any two characters in non-CJK text. It prioritizes breaking at whitespace
     * or other natural breakpoints to keep words intact. If no breakpoints are found, it breaks between
     * any two characters. For CJK text, this behaves like NORMAL.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    BREAK_WORD,

    /**
     * Attempts to break words at the end of a line using a hyphen. If a hyphen cannot be added,
     * it behaves like BREAK_WORD.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    BREAK_HYPHEN,
  }

  /**
   * Describes a text decoration.
   * @typedef Decoration
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  interface Decoration {
    /**
     * Type of the decoration. The default value is NONE.
     * @type { ?TextDecorationType }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    textDecoration?: TextDecorationType;

    /**
     * Color of the decoration. The default value is transparent.
     * @type { ?common2D.Color }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    color?: common2D.Color;

    /**
     * Style of the decoration. The default value is SOLID.
     * @type { ?TextDecorationStyle }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    decorationStyle?: TextDecorationStyle;

    /**
     * Scale factor for the thickness of the decoration line. The value is a floating point number.
     * The default value is 1.0.
     * @type { ?number }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    decorationThicknessScale?: number;
  }

  /**
   * Enumerates the text decoration types.
   * @enum { number }
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  enum TextDecorationType {
    /**
     * There are no text decoration.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    NONE,

    /**
     * There is a decoration line below the text.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    UNDERLINE,

    /**
     * There is a decoration line above the text.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    OVERLINE,

    /**
     * There is a decoration line through the middle of the text.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    LINE_THROUGH,
  }

  /**
   * Enumerates the text decoration styles.
   * @enum { number }
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  enum TextDecorationStyle {
    /**
     * Decoration line is solid line.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    SOLID,

    /**
     * Decoration line is double line.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    DOUBLE,

    /**
     * Decoration line is dotted line.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    DOTTED,

    /**
     * Decoration line is dashed line.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    DASHED,

    /**
     * Decoration line is wavy line.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    WAVY,
  }

  /**
   * Enumeration of font weight of text.
   * @enum { number }
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  enum FontWeight {
    /**
     * Thin
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    W100,

    /**
     * Extra-light
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    W200,

    /**
     * Light
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    W300,

    /**
     * Normal/Regular
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    W400,

    /**
     * Medium
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    W500,

    /**
     * Semi-bold
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    W600,

    /**
     * Bold
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    W700,

    /**
     * Extra-bold
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    W800,

    /**
     * Black
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    W900,
  }

  /**
   * Enumeration of font style of text.
   * @enum { number }
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  enum FontStyle {
    /**
     * Upright font type.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    NORMAL,

    /**
     * Slant font. If no italic version is available for the current font, the oblique version will be used instead.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    ITALIC,

    /**
     * Oblique font. If no oblique version is available for the current font, the italic version will be used instead.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    OBLIQUE,
  }

  /**
   * Enumeration of font width of text.
   * @enum { number }
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  enum FontWidth {
    /**
     * Ultra condensed font width.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    ULTRA_CONDENSED = 1,

    /**
     * Extra condensed font width.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    EXTRA_CONDENSED = 2,

    /**
     * Condensed font width.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    CONDENSED = 3,

    /**
     * Semi condensed font width.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    SEMI_CONDENSED = 4,

    /**
     * Normal font width.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    NORMAL = 5,

    /**
     * Semi expanded font width.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    SEMI_EXPANDED = 6,

    /**
     * Expanded font width.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    EXPANDED = 7,

    /**
     * Extra expanded font width.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    EXTRA_EXPANDED = 8,

    /**
     * Ultra expanded font width.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    ULTRA_EXPANDED = 9,
  }

  /**
   * Enumerates the text height modifier patterns.
   * @enum { number }
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  enum TextHeightBehavior {
    /**
     * Allows the first line of the paragraph to rise and the last line to drop.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    ALL = 0x0,

    /**
     * Prevents the first line of a paragraph from rising.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    DISABLE_FIRST_ASCENT = 0x1,

    /**
     * Prevents the last line of a paragraph from dropping.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    DISABLE_LAST_ASCENT = 0x2,

    /**
     * Combines the effects of disabling the first line from rising and the last line from dropping.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    DISABLE_ALL = 0x1 | 0x2,
  }

  /**
   * Enumeration the type of text baseline.
   * @enum { number }
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  enum TextBaseline {
    /**
     * The alphabetic baseline, typically used for Latin-based scripts where the baseline aligns
     * with the base of lowercase letters.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    ALPHABETIC,

    /**
     * The ideographic baseline, commonly used for ideographic scripts such as Chinese, Japanese, and Korean,
     * where the baseline aligns with the center of characters.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    IDEOGRAPHIC,
  }

  /**
   * Enumerates of ellipsis mode.
   * EllipsisMode.START and EllipsisMode.MIDDLE take effect only when text overflows in a single line.
   * @enum { number }
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  enum EllipsisMode {
    /**
     * Places the ellipsis in the text header. It is valid only when maxLines is set to 1 in ParagraphStyle.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    START,

    /**
     * Places the ellipsis in the middle of the text. It is valid only when maxLines is set to 1 in ParagraphStyle.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    MIDDLE,

    /**
     * Places the ellipsis at the end of the text.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    END,
  }

  /**
   * Describes shadow of text.
   * @typedef TextShadow
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  interface TextShadow {
    /**
     * Color of the text shadow. The default value is black (255, 0, 0, 0).
     * @type { ?common2D.Color } The color of text shadow
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    color?: common2D.Color;
    /**
     * Position of the text shadow relative to the text.
     * The horizontal and vertical coordinates must be greater than or equal to 0.
     * @type { ?common2D.Point } The point of shadow
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    point?: common2D.Point;
    /**
     * The value sets special effect radius of blurring text.
     * The value is a floating point number. The default value is 0.0px.
     * @type { ?number } The value about radius of blur, it type is "double"
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    blurRadius?: number;
  }

  /**
   * Describes the style of a rectangle.
   * @typedef RectStyle
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  interface RectStyle {
    /**
     * Color of the rectangle.
     * @type { common2D.Color } The color of rect style
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    color: common2D.Color;

    /**
     * Left top radius of the rectangle.
     * @type { number } it is double type data
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    leftTopRadius: number;

    /**
     * Right top radius of the rectangle.
     * @type { number } it is double type data
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    rightTopRadius: number;

    /**
     * Right bottom radius of the rectangle.
     * @type { number } it is double type data
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    rightBottomRadius: number;

    /**
     * Left bottom radius of the rectangle.
     * @type { number } it is double type data
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    leftBottomRadius: number;
  }

  /**
   * Describes font feature of text.
   * @typedef FontFeature
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  interface FontFeature {
    /**
     * String identified by the keyword in the font feature key-value pair.
     * @type { string } feature name
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    name: string;
    /**
     * 	Value in the font feature key-value pair.
     * @type { number } feature value
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    value: number;
  }

  /**
   * Describes font variation of text.
   * @typedef FontVariation
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  interface FontVariation {
    /**
     * String identified by the keyword in the font variation key-value pair.
     * @type { string } variation axis
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    axis: string;
    /**
     * Value in the font variation key-value pair.
     * @type { number } variation value
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    value: number;
  }

  /**
   * Describes badge type of text.
   * @enum { number }
   * @syscap SystemCapability.Graphics.Drawing
   * @since 20
   */
  enum TextBadgeType {
    /**
     * No badge.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 20
     */
    TEXT_BADGE_NONE,
    /**
     * Super badge.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 20
     */
    TEXT_SUPERSCRIPT,
    /**
     * Sub badge.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 20
     */
    TEXT_SUBSCRIPT,
  }

  /**
   * Describes text style.
   * @typedef TextStyle
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  interface TextStyle {

    /**
     * Text decoration. By default, no decoration is used.
     * @type { ?Decoration } decoration for text
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    decoration?: Decoration;

    /**
     * Text color. The default color is white.
     * @type { ?common2D.Color } it is uint32_t type data
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    color?: common2D.Color;

    /**
     * Font weight. The default value is W400. Currently, only the default system font supports font weight adjustment.
     * For other fonts, if the weight is less than semi-bold (W600), there is no variation in stroke thickness.
     * If the weight is greater than or equal to semi-bold, it might result in a fake bold effect.
     * @type { ?FontWeight } it is uint32_t type data
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    fontWeight?: FontWeight;

    /**
     * Font style. The default value is NORMAL.
     * @type { ?FontStyle } it is uint32_t type data
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    fontStyle?: FontStyle;

    /**
     * Text baseline type. The default value is ALPHABETIC.
     * @type { ?TextBaseline } it is uint32_t type data
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    baseline?: TextBaseline;

    /**
     * Array of font families. By default, the array is empty, indicating that all system fonts are matched.
     * @type { ?Array<string> } fontfamily gather
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    fontFamilies?: Array<string>;

    /**
     * Font size, in units of px. The value is a floating point number. The default value is 14.0.
     * @type { ?number } it is double type data
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    fontSize?: number;

    /**
     * Letter spacing, in units of px. The value is a floating point number.
     * The default value is 0.0. A positive value causes characters to spread farther apart,
     * and a negative value bring characters closer together.
     * @type { ?number } it is double type data
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    letterSpacing?: number;

    /**
     * Word spacing, in units of px. The value is a floating point number. The default value is 0.0.
     * @type { ?number } it is double type data
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    wordSpacing?: number;

    /**
     * Scale factor of the line height. The value is a floating point number.
     * The default value is 1.0. This parameter is valid only when heightOnly is set to true.
     * @type { ?number } it is double type data
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    heightScale?: number;

    /**
     * Whether half leading is enabled.
     * Half leading is the leading split in half and applied equally to the top and bottom edges.
     * The value true means that half leading is enabled, and false means the opposite. The default value is false.
     * @type { ?boolean } it is boolean type data
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    halfLeading?: boolean;

    /**
     * How the height of the text box is set.
     * The value true means that the height of the text box is set based on the font size and the value of heightScale,
     * and false means that the height is set based on the line height and line spacing. The default value is false.
     * @type { ?boolean } it is boolean type data
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    heightOnly?: boolean;

    /**
     * Ellipsis content, which will be used to replace the extra content.
     * @type { ?string } it is u16string type data.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    ellipsis?: string;

    /**
     * Ellipsis type. The default value is END, indicating that the ellipsis is at the end of a line.
     * @type { ?EllipsisMode } Ellipsis mode.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    ellipsisMode?: EllipsisMode;

    /**
     * Locale. For example, 'en' indicates English, 'zh-Hans' indicates Simplified Chinese,
     * and 'zh-Hant' indicates Traditional Chinese. For details, see ISO 639-1. The default value is an empty string.
     * @type { ?string } it is string type data.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    locale?: string;

    /**
     * Shift of the baseline. The value is a floating point number. The default value is 0.0px.
     * @type { ?number } it is double type data.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    baselineShift?: number;

    /**
     * Text Style available font features.
     * @type { ?Array<FontFeature> } A collection of font features.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    fontFeatures?: Array<FontFeature>;

    /**
     * Text shadows of text.
     * @type { ?Array<TextShadow> } textShadow gather.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    textShadows?: Array<TextShadow>;

    /**
     * Rectangle style of text.
     * @type { ?RectStyle } rect style for text.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    backgroundRect?: RectStyle;

    /**
     * Text Style available font variations.
     * @type { ?Array<FontVariation> } A collection of font variations.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    fontVariations?: Array<FontVariation>;

    /**
     * Text style available badge type.
     * @type { ?TextBadgeType } The type of text badge type.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 20
     */
    badgeType?: TextBadgeType;
  }

  /**
   * Implements a collection of fonts.
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  class FontCollection {
    /**
     * Get global FontCollection instance of the application.
     * @returns { FontCollection } The FontCollection object.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    static getGlobalInstance(): FontCollection;

    /**
     * Loads a custom font. This API returns the result synchronously.
     * In this API, name specifies the alias of the font, and the custom font effect can be displayed only when
     * the value of name is set in fontFamilies in TextStyle. The supported font file formats are .ttf and .otf.
     * @param { string } name - the font name.
     * @param { string | Resource } path - Path of the font file to import. The value must be
     * **file://**absolute path of the font file or **rawfile/**directory or file name.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    loadFontSync(name: string, path: string | Resource): void;

    /**
     * Loads a custom font. This API uses a promise to return the result.
     * In this API, name specifies the alias of the font, and the custom font effect can be displayed only when
     * the value of name is set in fontFamilies in TextStyle. The supported font file formats are ttf and otf.
     * @param { string } name - Name of the font. Any string is acceptable.
     * @param { string | Resource } path - Path of the font file to load.
     * The value must be **file://**absolute path of the font file or **rawfile/**directory or file name.
     * @returns { Promise<void> } Promise that returns no value.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    loadFont(name: string, path: string | Resource): Promise<void>;

    /**
     * Clear font caches.
     * The font cache has a memory limit and a clearing mechanism. It occupies limited memory.
     * You are not advised to clear it unless otherwise required.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
     clearCaches(): void;
  }

  /**
   * Describes the strut style, which determines the line spacing, baseline alignment mode,
   * and other properties related to the line height when drawing texts. The strut style is disabled by default.
   * @typedef StrutStyle
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  interface StrutStyle {
    /**
     * List of font families. By default, the list corresponds to the system's default fonts.
     * @type { ?Array<string> } fontfamily gather
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    fontFamilies?: Array<string>;

    /**
     * Font style. The default value is NORMAL.
     * @type { ?FontStyle } it is uint32_t type data
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    fontStyle?: FontStyle;

    /**
     * Font width. The default value is NORMAL.
     * @type { ?FontWidth } it is uint32_t type data
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    fontWidth?: FontWidth;

    /**
     * Font weight. The default value is W400. The default system font supports font weight adjustment.
     * For other fonts, if the weight is less than W600, there is no variation in stroke thickness.
     * If the weight is greater than or equal to W600, it might result in a fake bold effect.
     * @type { ?FontWeight } it is uint32_t type data
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    fontWeight?: FontWeight;

    /**
     * Font size, in units of px. The value is a floating point number. The default value is 14.0.
     * @type { ?number } it is double type data
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    fontSize?: number;

    /**
     * Scale factor of the line height. The value is a floating point number. The default value is 1.0.
     * @type { ?number } it is double type data
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    height?: number;

    /**
     * Custom leading to be applied to the strut. The value is a floating point number. The default value is -1.0.
     * @type { ?number } it is double type data
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    leading?: number;

    /**
     * Whether to forcibly use the strut height for all lines. The value true means to forcibly use the strut height
     * for all lines, and false means the opposite. The default value is false.
     * @type { ?boolean } it is boolean type data
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    forceHeight?: boolean;

    /**
     * Whether to enable the strut style.
     * The value true means to enable the strut style, and false means the opposite. The default value is false.
     * @type { ?boolean } it is boolean type data
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    enabled?: boolean;

    /**
     * 	Whether to override the height. The value true means to override the height, and false means the opposite.
     * The default value is false.
     * @type { ?boolean } it is boolean type data
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    heightOverride?: boolean;

    /**
     * Whether half leading is enabled.
     * Half leading is the leading split in half and applied equally to the top and bottom edges.
     * The value true means that half leading is enabled, and false means the opposite. The default value is false.
     * @type { ?boolean } it is boolean type data
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    halfLeading?: boolean;
  }

  /**
   * Determines the configuration used by ParagraphBuilder to position lines within a Paragraph of text.
   * @typedef ParagraphStyle
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  interface ParagraphStyle {
    /**
     * Text style applied to the paragraph. The default value is the initial text style.
     * @type { ?TextStyle }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    textStyle?: TextStyle;

    /**
     * Text direction. The default value is LTR.
     * @type { ?TextDirection }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    textDirection?: TextDirection;

    /**
     * Text alignment mode. The default value is START. This parameter is invalid when the tab parameter is configured.
     * @type { ?TextAlign }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    align?: TextAlign;

    /**
     * Word break type. The default value is BREAK_WORD.
     * @type { ?WordBreak }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    wordBreak?: WordBreak;

    /**
     * Maximum number of lines. The value is an integer. The default value is 1e9.
     * @type { ?number }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    maxLines?: number;

    /**
     * Text break strategy. The default value is GREEDY.
     * @type { ?BreakStrategy }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    breakStrategy?: BreakStrategy;

    /**
     * Strut style. The default value is the initial StrutStyle object.
     * @type { ?StrutStyle }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    strutStyle?: StrutStyle;

    /**
     * Text height modifier pattern. The default value is ALL.
     * @type { ?TextHeightBehavior }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    textHeightBehavior?: TextHeightBehavior;

    /**
     * Alignment mode and position of the text after the tab character in a paragraph. By default, the tab character
     * is replaced with a space. This parameter is invalid when it is used together with the align parameter or
     * the ellipsis parameter in TextStyle.
     * @type { ?TextTab }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    tab?: TextTab;

    /**
     * Whether to optimize white spaces at the end of each line.
     * @type { ?boolean } Boolean type data.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 20
     */
    trailingSpaceOptimized?: boolean;

    /**
     * Whether to enable automatic spacing between Chinese and English for paragraph.
     * @type { ?boolean }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 20
     */
    autoSpace?: boolean;
  }

  /**
   * Enumerates the vertical alignment modes of a placeholder relative to the surrounding text.
   * @enum { number }
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  enum PlaceholderAlignment {
    /**
     * Aligns the baseline of the placeholder to the baseline of the text.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    OFFSET_AT_BASELINE,

    /**
     * Aligns the bottom edge of the placeholder to the baseline of the text.
     * sits on top of the baseline.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    ABOVE_BASELINE,

    /**
     * Aligns the top edge of the placeholder to the baseline of the text.
     * hangs below the baseline.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    BELOW_BASELINE,

    /**
     * Align the top edge of the placeholder with the top edge of the font. When the placeholder is very tall,
     * the extra space will hang from the top and extend through the bottom of the line.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    TOP_OF_ROW_BOX,

    /**
     * Align the bottom edge of the placeholder with the bottom edge of the text. When the placeholder is very tall,
     * the extra space will rise from the bottom and extend through the top of the line.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    BOTTOM_OF_ROW_BOX,

    /**
     * Align the middle of the placeholder with the middle of the text. When the placeholder is very tall,
     * the extra space will grow equally from the top and bottom of the line.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    CENTER_OF_ROW_BOX,
  }

  /**
   * Describes the placeholder style.
   * @typedef PlaceholderSpan
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  interface PlaceholderSpan {
    /**
     * Width of the placeholder, in units of px. The value is a floating point number.
     * @type { number }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    width: number;

    /**
     * Height of the placeholder, in units of px. The value is a floating point number.
     * @type { number }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    height: number;

    /**
     * Vertical alignment of the placeholder relative to the surrounding text.
     * @type { PlaceholderAlignment }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    align: PlaceholderAlignment;

    /**
     * Type of the text baseline.
     * @type { TextBaseline }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    baseline: TextBaseline;

    /**
     * Offset to the text baseline, in units of px. The value is a floating point number.
     * @type { number }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    baselineOffset: number;
  }

  /**
   * Describes a left-closed and right-open interval.
   * @typedef Range
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  interface Range {
    /**
     * Index of the leftmost point of the interval. The value is an integer.
     * @type { number }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    start: number;

    /**
     * Index of the rightmost point of the interval. The value is an integer.
     * @type { number }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    end: number;
  }

  /**
   * Enumerates the font types, which can be combined through bitwise OR operations.
   * @enum { number }
   * @syscap SystemCapability.Graphics.Drawing
   * @since 14
   */
  enum SystemFontType {
    /**
     * All font types, including the system font type, style font type, and user-installed font type.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 14
     */
    ALL = 1 << 0,

    /**
     * System generic font type.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 14
     */
    GENERIC = 1 << 1,

    /**
     * Style font type. The style font type is designed for 2-in-1 devices.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 14
     */
    STYLISH = 1 << 2,

    /**
     * Font type that has been installed.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 14
     */
    INSTALLED = 1 << 3,

    /**
     * Customized font types.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    CUSTOMIZED = 1 << 4,
  }

  /**
   * Describes the font descriptor information.
   * @typedef FontDescriptor
   * @syscap SystemCapability.Graphics.Drawing
   * @since 14
   */
  interface FontDescriptor {
    /**
     * Absolute path of the font. Any string is acceptable, but the value must adhere to the system's path constraints.
     * The default value is an empty string.
     * @type { ?string }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 14
     */
    path?: string;

    /**
     * Unique name of the font. Any string is acceptable. The default value is an empty string.
     * @type { ?string }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 14
     */
    postScriptName?: string;

    /**
     * Font name. Any string is acceptable. The default value is an empty string.
     * @type { ?string }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 14
     */
    fullName?: string;

    /**
     * Family name of the font. Any string is acceptable. The default value is an empty string.
     * @type { ?string }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 14
     */
    fontFamily?: string;

    /**
     * Subfamily name of the font. Any string is acceptable. The default value is an empty string.
     * @type { ?string }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 14
     */
    fontSubfamily?: string;

    /**
     * Font weight. The default value is 0.
     * @type { ?FontWeight }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 14
     */
    weight?: FontWeight;

    /**
     * Font width. The value is an integer ranging from 1 to 9. The default value is 0.
     * @type { ?number }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 14
     */
    width?: number;

    /**
     * Whether the font is italic. The value 0 means that the font is not italic, and 1 means the opposite.
     * The default value is 0.
     * @type { ?number }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 14
     */
    italic?: number;

    /**
     * Whether the font is monospaced. The value true means that the font is monospaced, and false means the opposite.
     * The default value is false.
     * @type { ?boolean }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 14
     */
    monoSpace?: boolean;

    /**
     * Whether the font is symbolic. The value true means that the font is symbolic, and false means the opposite.
     * @type { ?boolean }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 14
     */
    symbolic?: boolean;
  }

  /**
   * Implements a carrier that stores the text content and style. You can perform operations such as layout and drawing.
   * Before calling any of the following APIs, you must use build() of the ParagraphBuilder class to
   * create a Paragraph object.
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  class Paragraph {
    /**
     * Performs layout and calculates the positions of all glyphs.
     * @param { number } width - Maximum width of a single line, in units of px. The value is a floating point number.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    layoutSync(width: number): void;

    /**
     * Performs layout and calculates the positions of all glyphs. This API uses a promise to return the result.
     * @param { number } width - Maximum width of a single line, in units of px. The value is a floating point number.
     * @returns { Promise<void> } Promise that returns no value.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    layout(width: number): Promise<void>;

    /**
     * Paints the text on the canvas with the coordinate point (x, y) as the upper left corner.
     * @param { drawing.Canvas } canvas - Target canvas.
     * @param { number } x - X coordinate of the upper left corner. The value is a floating point number.
     * @param { number } y - Y coordinate of the upper left corner. The value is a floating point number.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    paint(canvas: drawing.Canvas, x: number, y: number): void;

    /**
     * Draw the laid out text onto the supplied canvas along the path and offset.
     * @param { drawing.Canvas } canvas - Canvas used to carry the drawn content and drawing status.
     * @param { drawing.Path } path - Path used to determine the position of the text.
     * @param { number } hOffset - Horizontal offset along the path direction. A positive number indicates a position
     * that is ahead along the path from its start point, and a negative number indicates a position that is behind
     * from the start point.
     * @param { number } vOffset - Vertical offset along the path direction. A positive number indicates a position
     * on the left side of the path, and a negative number indicates a position on the right side of the path.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    paintOnPath(canvas: drawing.Canvas, path: drawing.Path, hOffset: number, vOffset: number): void;

    /**
     * Obtains the maximum width of the line in the text.
     * @returns { number } Maximum line width, in units of px. The value is a floating point number.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    getMaxWidth(): number;

    /**
     * Obtains the total height of the text.
     * @returns { number } Total height, in units of px. The value is a floating point number.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    getHeight(): number;

    /**
     * Obtains the longest line in the text.
     * @returns { number } Longest line, in units of px. The value is a floating point number.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    getLongestLine(): number;

    /**
     * Obtains the width of the longest line, including its indentation, in the text.
     * You are advised to round up the return value. If the text content is empty, 0 is returned.
     * @returns { number } Width of the longest line, including its indentation.
     * The value is a floating point number, in px.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 13
     */
    getLongestLineWithIndent(): number;

    /**
     * Obtains the minimum intrinsic width of the paragraph.
     * @returns { number } Minimum intrinsic width, in units of px. The value is a floating point number.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    getMinIntrinsicWidth(): number;

    /**
     * Obtains the maximum intrinsic width of the paragraph.
     * @returns { number } Maximum intrinsic width, in units of px. The value is a floating point number.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    getMaxIntrinsicWidth(): number;

    /**
     * Obtains the alphabetic baseline.
     * @returns { number } Alphabetic baseline, in units of px. The value is a floating point number.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    getAlphabeticBaseline(): number;

    /**
     * Obtains the ideographic baseline.
     * @returns { number } Ideographic baseline, in units of px. The value is a floating point number.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    getIdeographicBaseline(): number;

    /**
     * Obtains the rectangles occupied by the characters in the range of the text under the given rectangle width and
     * height.
     * @param { Range } range - Range of the text.
     * @param { RectWidthStyle } widthStyle - Width of the rectangle.
     * @param { RectHeightStyle } heightStyle - Height of the rectangle.
     * @returns { Array<TextBox> } Array holding the rectangles obtained.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    getRectsForRange(range: Range, widthStyle: RectWidthStyle, heightStyle: RectHeightStyle): Array<TextBox>;

    /**
     * Obtains the rectangles occupied by all placeholders in the text.
     * @returns { Array<TextBox> } Array holding the rectangles obtained.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    getRectsForPlaceholders(): Array<TextBox>;

    /**
     * Obtains the position of a glyph closest to the given coordinates.
     * @param { number } x - X coordinate. The value is a floating point number.
     * @param { number } y - Y coordinate. The value is a floating point number.
     * @returns { PositionWithAffinity } Position of the glyph.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    getGlyphPositionAtCoordinate(x: number, y: number): PositionWithAffinity;

    /**
     * Obtains the range of the word where the glyph with a given offset is located.
     * @param { number } offset - Offset of the glyph. The value is an integer.
     * @returns { Range } Range of the word.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    getWordBoundary(offset: number): Range;

    /**
     * Obtains the number of text lines.
     * @returns { number } Number of text lines. The value is an integer.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    getLineCount(): number;

    /**
     * Obtains the height of a given line.
     * @param { number } line - Index of the line. The value is an integer ranging from 0 to getLineCount() – 1.
     * @returns { number } The line height value returned to the caller.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    getLineHeight(line: number): number;

    /**
     * Obtains the width of a given line.
     * @param { number } line - Index of the line. The value is an integer ranging from 0 to getLineCount() – 1.
     * @returns { number } The line width value returned to the caller.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    getLineWidth(line: number): number;

    /**
     * Checks whether the number of lines in the paragraph exceeds the maximum.
     * @returns { boolean } Check result. The value true means that the number of lines exceeds the maximum,
     * and false means the opposite.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    didExceedMaxLines(): boolean;

    /**
     * Obtains all the text lines.
     * @returns { Array<TextLine> } Array of text lines.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    getTextLines(): Array<TextLine>;

    /**
     * Obtains the actually visible text range in the specified line, excluding any overflow ellipsis.
     * @param { number } lineNumber - Line number of the text range, starting from 0. This API can only be used to
     * obtain the bounds of existing lines. That is, the line number must start from 0, and the maximum line number
     * is getLineCount - 1.
     * @param { boolean } includeSpaces - Whether spaces are included. The value true means that spaces are contained,
     * and false means the opposite.
     * @returns { Range } Text range obtained. If the line index is invalid, start and end are both 0.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    getActualTextRange(lineNumber: number, includeSpaces: boolean): Range;

    /**
     * Obtains an array of line measurement information.
     * @returns { Array<LineMetrics> } Array of line measurement information.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    getLineMetrics(): Array<LineMetrics>;

    /**
     * Obtains the line measurement information of a line.
     * @param { number } lineNumber - Line number, starting from 0.
     * @returns { LineMetrics | undefined } LineMetrics object containing the measurement information if the specified
     * line number is valid and the measurement information exists. If the line number is invalid or
     * the measurement information cannot be obtained, undefined is returned.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    getLineMetrics(lineNumber: number): LineMetrics | undefined;

    /**
     * Synchronously updates the text color of the typography.
     * @param { common2D.Color } color - Color of text.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 20
     */
    updateColor(color: common2D.Color): void;

    /**
     * Synchronously updates text decoration.
     * @param { Decoration } decoration - Decoration of text.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 20
     */
    updateDecoration(decoration: Decoration): void;
  }

  /**
   * Implements a carrier that stores the text content and style. It can be used to compute layout details for
   * individual lines of text. Before calling any of the following APIs, you must use buildLineTypeset()
   * in the ParagraphBuilder class to create a LineTypeset object.
   * @syscap SystemCapability.Graphics.Drawing
   * @since 18
   */
  class LineTypeset {
    /**
     * Obtains the number of characters that can fit in the layout from the specified position within a limited width.
     * @param { number } startIndex - Start position (inclusive) for calculation. The value is an integer in the range
     * [0, total number of text characters). If the parameter is invalid, an exception is thrown.
     * @param { number } width - Layout width. The value is a floating point number greater than 0, in px.
     * @returns { number } Number of characters.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    getLineBreak(startIndex: number, width: number): number;

    /**
     * Generates a text line object based on the specified layout range.
     * @param { number } startIndex - Start position for layout calculation. The value is an integer in the
     * range [0, total number of text characters).
     * @param { number } count - 	Number of characters from the specified start position. The value is an integer in
     * the range [0, total number of text characters). The sum of startIndex and count cannot be greater than
     * the total number of text characters. When count is 0, the range is [startIndex, end of the text].
     * You can use getLineBreak to obtain the number of characters that can fit in the layout.
     * @returns { TextLine } TextLine object generated based on the characters in the text range.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    createLine(startIndex: number, count: number): TextLine;
  }

  /**
   * Describes the rectangle that holds the text.
   * @typedef TextBox
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  interface TextBox{
    /**
     * Information about the rectangle.
     * @type { common2D.Rect }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    rect: common2D.Rect;

    /**
     * Text direction.
     * @type { TextDirection }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    direction: TextDirection;
  }

  /**
   * Describes the position and affinity of a glyph.
   * @typedef PositionWithAffinity
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  interface PositionWithAffinity {
    /**
     * Index of the glyph relative to the paragraph. The value is an integer.
     * @type { number }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    position: number;

    /**
     * Affinity of the position.
     * @type { Affinity }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    affinity: Affinity;
  }

  /**
   * Enumerates the rectangle width styles.
   * @enum { number }
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  enum RectWidthStyle {
    /**
     * If letterSpacing is not set, the rectangle conforms tightly to the text it contains.
     * However, if letterSpacing is set, a gap is introduced between the rectangle and text.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    TIGHT,

    /**
     * The rectangle's width is extended to align with the widest rectangle across all lines.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    MAX,
  }

  /**
   * Enumerates the rectangle height styles.
   * @enum { number }
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  enum RectHeightStyle {
    /**
     * Tight style.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    TIGHT,

    /**
     * Extends the height to match the highest rectangle in all lines.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    MAX,

    /**
     * The top and bottom of each rect will cover half of the space above and half of the space below the line.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    INCLUDE_LINE_SPACE_MIDDLE,

    /**
     * The line spacing will be added to the top of the rect.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    INCLUDE_LINE_SPACE_TOP,

    /**
     * The line spacing will be added to the bottom of the rect.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    INCLUDE_LINE_SPACE_BOTTOM,

    /**
     * The height of the boxes will be calculated by text strut.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    STRUT,
  }

  /**
   * Enumerates text affinity.When a selection range involves line breaks or other special characters, the
   * affinity determines which side of the characters the start and end of the selection range should be
   * closer to.
   * @enum { number }
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  enum Affinity {
    /**
     * The position has affinity for the upstream side of the text position.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */

    UPSTREAM,
    /**
     * The position has affinity for the downstream side of the text position.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    DOWNSTREAM,
  }

  /**
   * Builds a Paragraph containing text with the given styling information.
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  class ParagraphBuilder {
    /**
     * A constructor used to create a ParagraphBuilder object.
     * @param { ParagraphStyle } paragraphStyle - Paragraph style {@link ParagraphStyle}
     * @param { FontCollection } fontCollection - Font collection {@link FontCollection}
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    constructor(paragraphStyle: ParagraphStyle, fontCollection: FontCollection);

    /**
     * Applies a new style to the current text blob.
     * <p>**NOTE**</p>
     * When you update the style of the current text blob, all text added afterward will use this new style.
     * @param { TextStyle } textStyle - Text style, which describes various visual attributes of text, such as font,
     * font size, color, font weight, word spacing, line spacing, decoration (such as underline and strikethrough),
     * and text shadow. {@link TextStyle}
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    pushStyle(textStyle: TextStyle): void;

    /**
     * Restores the previous text style.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    popStyle(): void;

    /**
     * Inserts a text string into the paragraph being built.
     * @param { string } text - Exact text string inserted into the paragraph. If an invalid Unicode character is
     * provided, it is displayed as �.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    addText(text: string): void;

    /**
     * Inserts a placeholder into the paragraph being built.
     * @param { PlaceholderSpan } placeholderSpan - Placeholder span, which describes the size, alignment,
     * baseline type, and baseline offset of the placeholder. {@link PlaceholderSpan}
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    addPlaceholder(placeholderSpan: PlaceholderSpan): void;

    /**
     * Creates a paragraph object that can be used for subsequent layout and rendering.
     * @returns { Paragraph } Paragraph object that can be used for subsequent rendering.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    build(): Paragraph;

    /**
     * Builds a line typesetter.
     * @returns { LineTypeset } LineTypeset object that can be used for subsequent rendering.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    buildLineTypeset(): LineTypeset;

    /**
     * Inserts a symbol into the paragraph being built.
     * @param { number } symbolId - Symbol code to insert. The value is a hexadecimal number in the
     * range 0xF0000-0xF0C97. For details about the configurable symbol codes (unicode values in the list view),
     * see <a href="https://developer.huawei.com/consumer/cn/design/harmonyos-symbol/">HarmonyOS Symbol</a>.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    addSymbol(symbolId: number): void;
  }

  /**
   * Describes the typographic boundaries of a text line. These boundaries depend on the typographic font and font size,
   * not on the characters themselves. For example, for the string " a b " (which has a space before "a" and a space
   * after "b"), the typographic boundaries include the spaces at the beginning and end of the line. Similarly,
   * the strings "j" and "E" have identical typographic boundaries, which are independent of the characters themselves.
   * @typedef TypographicBounds
   * @syscap SystemCapability.Graphics.Drawing
   * @since 18
   */
  interface TypographicBounds {
    /**
     * Ascent of a text line. The value is a floating point number.
     * @type { number }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    ascent: number;

    /**
     * Descent of a text line. The value is a floating point number.
     * @type { number }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    descent: number;

    /**
     * Leading of a text line. The value is a floating point number.
     * @type { number }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    leading: number;

    /**
     * Width of the typographic boundaries. The value is a floating point number.
     * @type { number }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    width: number;
  }

  /**
   * Defines the callback used to receive the offset and index of each character in a text line object
   * as its parameters.
   *
   * @typedef { function } CaretOffsetsCallback
   * @param { number } offset - Offset of each character in a text line. The value is a floating point number.
   * @param { number } index - Index of each character in a text line. The value is an integer.
   * @param { boolean } leadingEdge - Whether the cursor is located at the front of the character. The value true means
   * that the cursor is located at the front of the character, that is, the offset does not contain the character
   * width. The value false means that the cursor is located at the rear of the character, that is, the offset
   * contains the character width.
   * callback function.
   * @returns { boolean } Whether to stop calling the callback. The value true means to stop calling the callback,
   * and false means to continue calling the callback.
   * @syscap SystemCapability.Graphics.Drawing
   * @since 18
   */
  type CaretOffsetsCallback = (offset: number, index: number, leadingEdge: boolean) => boolean;

  /**
   * Implements a carrier that describes the basic text line structure of a paragraph.
   * Before calling any of the following APIs, you must use getTextLines() of the Paragraph class or createLine() of
   * the LineTypeset class to create a TextLine object.
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  class TextLine {
    /**
     * Obtains the number of glyphs in this text line.
     * @returns { number } Number of glyphs. The value is an integer.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    getGlyphCount(): number;

    /**
     * Obtains the range of the text in this text line in the entire paragraph.
     * @returns { Range } Range of the text in this text line in the entire paragraph.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    getTextRange(): Range;

    /**
     * Obtains the array of glyph runs in the text line.
     * @returns { Array<Run> } Array of the runs obtained.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    getGlyphRuns(): Array<Run>;

    /**
     * Paints this text line on the canvas with the coordinate point (x, y) as the upper left corner.
     * @param { drawing.Canvas } canvas - Target canvas.
     * @param { number } x - X coordinate of the upper left corner. The value is a floating point number.
     * @param { number } y - Y coordinate of the upper left corner. The value is a floating point number.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    paint(canvas: drawing.Canvas, x: number, y: number): void;

    /**
     * Creates a truncated text line object.
     * @param { number } width - Width of the line after truncation. The value is a floating point number.
     * @param { EllipsisMode } ellipsisMode - Ellipsis mode. Currently, only START and END are supported.
     * @param { string } ellipsis - String used to mark a truncation.
     * @returns { TextLine } Truncated text line object.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    createTruncatedLine(width: number, ellipsisMode: EllipsisMode, ellipsis: string): TextLine;

    /**
     * Obtains the typographic boundaries of this text line. These boundaries depend on the typographic font and font
     * size, but not on the characters themselves. For example, for the string " a b " (which has a space before
     * "a" and a space after "b"), the typographic boundaries include the spaces at the beginning and end of the
     * line. Similarly, the strings "j" and "E" have identical typographic boundaries, which are independent of
     * the characters themselves.
     * @returns { TypographicBounds } Typographic boundaries of the text line.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    getTypographicBounds(): TypographicBounds;

    /**
     * Obtains the image boundaries of this text line. The image boundaries, equivalent to visual boundaries, depend on
     * the font, font size, and characters. For example, for the string " a b " (which has a space before "a" and
     * a space after "b"), only "a b" are visible to users, and therefore the image boundaries do not include these
     * spaces at the beginning and end of the line. For the strings "j" and "E", their image boundaries are
     * different. Specifically, the width of the boundary for "j" is narrower than that for "E", and the height of
     * the boundary for "j" is taller than that for "E".
     * @returns { common2D.Rect } Image boundary of the text line.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    getImageBounds(): common2D.Rect;

    /**
     * Obtains the width of the spaces at the end of this text line.
     * @returns { number } Number of spaces at the end of the text line. The value is a floating point number.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    getTrailingSpaceWidth(): number;

    /**
     * Obtains the index of a character at the specified position in the original string.
     * @param { common2D.Point } point - Position of the character.
     * @returns { number } Index of the character in the text line. The value is an integer.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    getStringIndexForPosition(point: common2D.Point): number;

    /**
     * Obtains the offset of a character with the specified index in this text line.
     * @param { number } index - Index of the character. The value is an integer.
     * @returns { number } Offset of the character with the specified index. The value is a floating point number.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    getOffsetForStringIndex(index: number): number;

    /**
     * Enumerates the offset and index of each character in a text line.
     * @param { CaretOffsetsCallback } callback - Custom function, which contains the offset and index of each
     * character in the text line.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    enumerateCaretOffsets(callback: CaretOffsetsCallback): void;

    /**
     * Obtains the offset of this text line after alignment based on the alignment factor and alignment width.
     * @param { number } alignmentFactor - Alignment factor, which determines how text is aligned. The value is a
     * floating point number. A value less than or equal to 0.0 means that the text is left-aligned; a value
     * between 0.0 and 0.5 means that the text is slightly left-aligned; the value 0.5 means that is text
     * is centered; a value between 0.5 and 1 means that the text is slightly right-aligned; a value greater than
     * or equal to 1.0 means that the text is right-aligned.
     * @param { number } alignmentWidth - Alignment width, that is, the width of the text line. The value is a floating
     * point number. If the width is less than the actual width of the text line, 0 is returned.
     * @returns { number } Offset required for alignment. The value is a floating point number.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    getAlignmentOffset(alignmentFactor: number, alignmentWidth: number): number;
  }

  /**
   * Implements a unit for text layout.
   * Before calling any of the following APIs, you must use getGlyphRuns() of the TextLine class to create a Run object.
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  class Run {
    /**
     * Obtains the number of glyphs in this run.
     * @returns { number } Number of glyphs. The value is an integer.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    getGlyphCount(): number;

    /**
     * Obtains the index of each glyph in this run.
     * @returns { Array<number> } Array holding the index of each glyph in the run.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    getGlyphs(): Array<number>;

    /**
     * Obtains the index of each glyph in the specified range of this run.
     * @param { Range } range - Range of the glyphs, where range.start indicates the start position of the range, and
     * range. end indicates the length of the range. If the length is 0, the range is from range.start to the end
     * of the run. If range.end or range.start is set to a negative value, null, or undefined, undefined is
     * returned.
     * @returns { Array<number> } Array holding the index of each glyph in the run.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    getGlyphs(range: Range): Array<number>;

    /**
     * Obtains the position of each glyph relative to the respective line in this run.
     * @returns { Array<common2D.Point> } Array holding the position of each glyph relative to the respective line in
     * the run.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    getPositions(): Array<common2D.Point>;

    /**
     * Obtains the position array of each glyph relative to the respective line within the specified range of this run.
     * @param { Range } range - Range of the glyphs, where range.start indicates the start position of the range, and
     * range. end indicates the length of the range. If the length is 0, the range is from range.start to the end of
     * the run. If range.end or range.start is set to a negative value, null, or undefined, undefined is returned.
     * @returns { Array<common2D.Point> } 	Array holding the position of each glyph relative to the respective line in
     * the run.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    getPositions(range: Range): Array<common2D.Point>;

	  /**
     * Obtains the offset of each glyph in this run relative to its index.
     * @returns { Array<common2D.Point> } Array holding the offset of each glyph in the run relative to its index.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    getOffsets(): Array<common2D.Point>;

    /**
     * Obtains the Font object of this run.
     * @returns { drawing.Font } Font object of this run.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    getFont(): drawing.Font;

    /**
     * Paints this run on the canvas with the coordinate point (x, y) as the upper left corner.
     * @param { drawing.Canvas } canvas - Target canvas.
     * @param { number } x - X coordinate of the upper left corner. The value is a floating point number.
     * @param { number } y - Y coordinate of the upper left corner. The value is a floating point number.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    paint(canvas: drawing.Canvas, x: number, y: number): void;

    /**
     * Obtains an array of character indices for glyphs within a specified range of this run, where the indices are
     * offsets relative to the entire paragraph.
     * @param { Range } range - Range of the glyphs, where range.start indicates the start position of the range, and
     * range.end indicates the length of the range. If the length is 0, the range is from range.start to the end of
     * the run. If range.end or range.start is set to a negative value, null, or undefined, undefined is returned.
     * If this parameter is not passed, the entire run is obtained.
     * @returns { Array<number> } Array of character indices.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    getStringIndices(range?: Range): Array<number>;

    /**
     * Obtains the range of glyphs generated by this run.
     * @returns { Range } 	Range of the glyphs, where start indicates the start position of the range, which is the
     * index relative to the entire paragraph, and end indicates the length of the range.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    getStringRange(): Range;

    /**
     * Obtain the typographic boundaries of this run. These boundaries depend on the typographic font and font size,
     * but not on the characters themselves. For example, for the string " a b " (which has a space before "a" and
     * a space after "b"), the typographic boundaries include the spaces at the beginning and end of the line.
     * @returns { TypographicBounds } Typographic boundaries of the run.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    getTypographicBounds(): TypographicBounds;

    /**
     * Obtains the image boundary of this run. The image boundary, equivalent to a visual boundary, is related to the
     * font, font size, and characters. For example, for the string " a b " (which has a space before "a" and a
     * space after "b"), only "a b" are visible to users, and therefore the image boundary does not include these
     * spaces at the beginning and end.
     * @returns { common2D.Rect } Image boundary of the run.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    getImageBounds(): common2D.Rect;
  }

  /**
   * Describes the layout information and metrics for a continuous piece of text (a run) in a line of text.
   * @typedef RunMetrics
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  interface RunMetrics {
    /**
     * The metrics of an Font.
     * @type { TextStyle }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    textStyle: TextStyle;

    /**
     * Describes text style.
     * @type { drawing.FontMetrics }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    fontMetrics: drawing.FontMetrics;
  }

  /**
   * Describes the measurement information of a single line of text in the text layout.
   * @typedef LineMetrics
   * @syscap SystemCapability.Graphics.Drawing
   * @since 12
   */
  interface LineMetrics {
    /**
     * Start index of the line in the text buffer.
     * @type { number }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    startIndex: number;

    /**
     * End index of the line in the text buffer.
     * @type { number }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    endIndex: number;

    /**
     * Ascent, that is, the distance from the baseline to the top of the character.
     * @type { number }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    ascent: number;

    /**
     * Descent, that is, the distance from the baseline to the bottom of the character.
     * @type { number }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    descent: number;

    /**
     * Height of the line, which is Math.round(ascent + descent).
     * @type { number }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    height: number;

    /**
     * Width of the line.
     * @type { number }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    width: number;

    /**
     * Left edge of the line. The right edge is the value of left plus the value of width.
     * @type { number }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    left: number;

    /**
     * Y coordinate of the baseline in the line relative to the top of the paragraph.
     * @type { number }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    baseline: number;

    /**
     * Line number, starting from 0.
     * @type { number }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    lineNumber: number;

    /**
     * Height from the top to the current line.
     * @type { number }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    topHeight: number;

    /**
     * Mapping between text index ranges and the FontMetrics associated with
     * them. The first run will be keyed under start_index. The metrics here.
     * are before layout and are the base values we calculate from.
     * @type { Map<number, RunMetrics> }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 12
     */
    runMetrics: Map<number, RunMetrics>;
  }

  /**
   * Obtains the full names of all fonts of the specified type. This API uses a promise to return the result.
   * @param { SystemFontType } fontType - System font type.
   * @returns { Promise<Array<string>> } 	Promise used to return the full names of all fonts of the specified type.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.Graphics.Drawing
   * @since 14
   */
  function getSystemFontFullNamesByType(fontType: SystemFontType): Promise<Array<string>>;

  /**
   * Obtains the font descriptor based on the font name and type. This API uses a promise to return the result.
   * A font descriptor is a data structure that describes font features. It contains details of the font appearance and
   * properties.
   * @param { string } fullName - Font name, corresponding to the value of fullName in the name table of the
   * corresponding font file. It is obtained by calling getSystemFontFullNamesByType.
   * @param { SystemFontType } fontType - System font type.
   * @returns { Promise<FontDescriptor> } Promise used to return the font descriptor.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types.
   * @syscap SystemCapability.Graphics.Drawing
   * @since 14
   */
  function getFontDescriptorByFullName(fullName: string, fontType: SystemFontType): Promise<FontDescriptor>;

  /**
   * Obtains all system font descriptors that match the provided font descriptor. This API uses a promise to return the
   * result.
   * @param { FontDescriptor } desc - Font descriptor to match against. If this parameter is left unspecified,
   * all system font descriptors are returned. If a specific value is provided, the matching is performed based on
   * <br.the value provided. If the matching fails, an empty array is returned.
   * @returns { Promise<Array<FontDescriptor>> } Promise used to return all matched system font descriptors, and an
   * empty array will be returned if the matching fails.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.Graphics.Drawing
   * @since 18
   */
  function matchFontDescriptors(desc: FontDescriptor): Promise<Array<FontDescriptor>>;

  /**
   * Implements a paragraph-style text tab, which stores the alignment mode and position.
   * @typedef TextTab
   * @syscap SystemCapability.Graphics.Drawing
   * @since 18
   */
  interface TextTab {
    /**
     * Alignment mode of the text following the tab character in a paragraph. It can be set to LEFT, RIGHT, and CENTER
     * defined in TextAlign. Other enumerated values have the effect of left alignment. The default value is left
     * alignment.
     * @type { TextAlign }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    alignment: TextAlign;
    
    /**
     * Alignment position of the text following the tab character. The value is a floating point number, in px.
     * The minimum value is 1.0. When the value is less than 1.0, the tab character is replaced with a space.
     * @type { number }
     * @syscap SystemCapability.Graphics.Drawing
     * @since 18
     */
    location: number;
  }

  /**
   * Defines text rendering high contrast mode to enhance readability.
   * @enum { number }
   * @syscap SystemCapability.Graphics.Drawing
   * @since 20
   */
  enum TextHighContrast {
    /**
     * Follow system's high contrast settings for text rendering.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 20
     */
    TEXT_FOLLOW_SYSTEM_HIGH_CONTRAST,
    /**
     * Disables high contrast rendering regardless of system settings.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 20
     */
    TEXT_APP_DISABLE_HIGH_CONTRAST,
    /**
     * Enable high contrast rendering regardless of system settings.
     * @syscap SystemCapability.Graphics.Drawing
     * @since 20
     */
    TEXT_APP_ENABLE_HIGH_CONTRAST,
  }

  /**
   * Sets high contrast mode of text rendering.
   * @param { TextHighContrast } action - High contrast mode.
   * @syscap SystemCapability.Graphics.Drawing
   * @since 20
   */
  function setTextHighContrast(action: TextHighContrast): void;
}

export default text;
