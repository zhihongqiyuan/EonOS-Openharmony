/**
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
@Entry
@Component
struct BounceSymbolEffect_Scope {
  @State isActive: boolean = true;
  @State triggerValueReplace: number = 0;
  replaceFlag: boolean = true;
  @State BounceSymbolEffect_Scope: EffectScope = EffectScope.LAYER
  @State ReplaceSymbolEffect_Scope: EffectScope = EffectScope.WHOLE
  @State BounceSymbolEffectText: string = ''
  @State ReplaceSymbolEffectText: string = ''

  build() {
    Column({ space: 12 }) {
      Text(this.BounceSymbolEffectText)
        .id('BounceSymbolEffect')
      Text(this.ReplaceSymbolEffectText)
      Row({ space: 20 }) {
        SymbolGlyph(this.replaceFlag ? $r('sys.symbol.checkmark_circle') : $r('sys.symbol.repeat_1'))
          .fontSize(96)
          .symbolEffect(new BounceSymbolEffect(this.BounceSymbolEffect_Scope, EffectDirection.DOWN),
            this.triggerValueReplace)
        SymbolGlyph(this.replaceFlag ? $r('sys.symbol.checkmark_circle') : $r('sys.symbol.repeat_1'))
          .fontSize(96)
          .symbolEffect(new ReplaceSymbolEffect(this.ReplaceSymbolEffect_Scope), this.triggerValueReplace)
      }

      Button('trigger').width('50%').onClick(() => {
        this.replaceFlag = !this.replaceFlag;
        this.triggerValueReplace = this.triggerValueReplace + 1;
        this.BounceSymbolEffectText = this.BounceSymbolEffect_Scope + ''
        this.ReplaceSymbolEffectText = this.ReplaceSymbolEffect_Scope + ''
      }).id('btn')
    }.margin({
      left: 30,
      top: 50
    })
  }
}