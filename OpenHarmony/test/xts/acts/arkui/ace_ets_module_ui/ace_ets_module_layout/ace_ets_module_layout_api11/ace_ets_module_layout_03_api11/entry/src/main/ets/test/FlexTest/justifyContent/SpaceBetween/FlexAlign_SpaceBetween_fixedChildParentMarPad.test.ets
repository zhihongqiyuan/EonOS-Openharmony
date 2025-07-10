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

import { describe, beforeAll, beforeEach, afterEach, afterAll, it, expect } from "@ohos/hypium";
import router from '@ohos.router';
import CommonFunc, { RectValue } from '../../../../MainAbility/common/Common';
import GlobalContext from '../../../../MainAbility/common/GlobalContext';
import { MessageManager, Callback } from '../../../../MainAbility/common/MessageManager';

export default function flexAlign_SpaceBetween_fixedChildParentMarPad() {
  describe('FlexAlign_SpaceBetween_fixedChildParentMarPad', () => {
    beforeEach(async () => {
      console.info("FlexAlign_SpaceBetween_fixedChildParentMarPad beforeEach called");
      let options: router.RouterOptions = {
        url: 'MainAbility/pages/Flex/justifyContent/SpaceBetween/FlexAlign_SpaceBetween_fixedChildParentMarPad',
      }
      try {
        router.clear();
        await CommonFunc.sleep(1000);
        let pages = router.getState();
        console.info("get FlexAlign_SpaceBetween_fixedChildParentMarPad state pages:" + JSON.stringify(pages));
        if (!("FlexAlign_SpaceBetween_fixedChildParentMarPad" == pages.name)) {
          console.info("get FlexAlign_SpaceBetween_fixedChildParentMarPad state pages.name:" + JSON.stringify(pages.name));
          let result = await router.pushUrl(options);
          console.info("push FlexAlign_SpaceBetween_fixedChildParentMarPad page result:" + JSON.stringify(result));
        }
      } catch (err) {
        console.error("push FlexAlign_SpaceBetween_fixedChildParentMarPad page error:" + JSON.stringify(err));
      }
      await CommonFunc.sleep(2000);

    });

    afterEach(async () => {
      await CommonFunc.sleep(1000);
      console.info("FlexAlign_SpaceBetween_fixedChildParentMarPad afterEach called");
    });

    /**
     * @tc.number SUB_ACE_FLEX_JUSTIFYCONTENT_FLEXALIGN_SPACEBETWEEN_TEST_0600
     * @tc.name testAlignSpaceBetweenRowNoWrapMarPadOutRange
     * @tc.desc The child component is fixed, and parent component is bound with padding and margin attributes,
     *               the layout space of parent is insufficient to meet the spindle layout requirements of child
     */
    it('testAlignSpaceBetweenRowNoWrapMarPadOutRange', 0, async () => {
      console.info('new testAlignSpaceBetweenRowNoWrapMarPadOutRange START');
      GlobalContext.getContext().getObject<number>('value').message.notify({
        name: 'margin', value: 20
      })
      GlobalContext.getContext().getObject<number>('value').message.notify({
        name: 'padding', value: 50
      })
      await CommonFunc.sleep(3000);
      let strJson = getInspectorByKey('FlexAlignSpaceBetween6');

      expect(JSON.parse(strJson).$type).assertEqual('Flex');
      expect(JSON.parse(strJson).$attrs.constructor.direction).assertEqual('FlexDirection.Row');
      expect(JSON.parse(strJson).$attrs.constructor.wrap).assertEqual('FlexWrap.NoWrap');
      expect(JSON.parse(strJson).$attrs.constructor.justifyContent).assertEqual('FlexAlign.SpaceBetween');
      let locationText1: RectValue = CommonFunc.getComponentRect('AlignSpaceBetween16');
      let locationText2: RectValue = CommonFunc.getComponentRect('AlignSpaceBetween17');
      let locationText3: RectValue = CommonFunc.getComponentRect('AlignSpaceBetween18');
      let locationFlex: RectValue = CommonFunc.getComponentRect('FlexAlignSpaceBetween6');
      expect(Math.floor(Math.abs(locationText1.top - locationText2.top)) <= 1).assertTrue();
      expect(Math.floor(Math.abs(locationText2.top - locationText3.top)) <= 1).assertTrue();
      expect(Math.floor(Math.abs((locationText1.top - locationFlex.top) - vp2px(50))) <= 1).assertTrue();
      expect(Math.floor(Math.abs((locationText1.bottom - locationText1.top) - vp2px(50))) <= 1).assertTrue();
      expect(Math.floor(Math.abs((locationText2.bottom - locationText2.top) - vp2px(100))) <= 1).assertTrue();
      expect(Math.floor(Math.abs((locationText3.bottom - locationText3.top) - vp2px(150))) <= 1).assertTrue();
      expect(Math.floor(Math.abs(locationFlex.bottom - locationText3.bottom)) <= 1).assertTrue();
      expect(Math.floor(Math.abs((locationText1.left - locationFlex.left) - vp2px(50))) <= 1).assertTrue();
      expect(Math.floor(Math.abs(locationText1.right - locationText2.left)) <= 1).assertTrue();
      expect(Math.floor(Math.abs(locationText2.right - locationText3.left)) <= 1).assertTrue();
      expect(Math.floor(Math.abs(locationText3.right - (locationFlex.right - vp2px(50)))) <= 1).assertTrue();
      expect(Math.floor(Math.abs((locationText1.right - locationText1.left) - (locationText2.right - locationText2.left))) <= 1)
        .assertTrue();
      expect(Math.floor(Math.abs((locationText3.right - locationText3.left) - (locationText2.right - locationText2.left))) <= 1)
        .assertTrue();
      expect(Math.floor(Math.abs((locationText2.right - locationText2.left) - vp2px(400 / 3))) <= 1).assertTrue();
      console.info('new testAlignSpaceBetweenRowNoWrapMarPadOutRange END');

    });

    /**
     * @tc.number SUB_ACE_FLEX_JUSTIFYCONTENT_FLEXALIGN_SPACEBETWEEN_TEST_0700
     * @tc.name testAlignSpaceBetweenRowNoWrapMarPadInRange
     * @tc.desc The child component is fixed, and parent component is bound with padding and margin attributes,
     *               the layout space of parent component meets the spindle layout requirements of child component
     */
    it('testAlignSpaceBetweenRowNoWrapMarPadInRange', 0, async () => {
      console.info('new testAlignSpaceBetweenRowNoWrapMarPadInRange START');
      GlobalContext.getContext().getObject<number>('value').message.notify({
        name: 'margin', value: 20
      })
      GlobalContext.getContext().getObject<number>('value').message.notify({
        name: 'padding', value: 5
      })
      await CommonFunc.sleep(3000);
      let strJson = getInspectorByKey('FlexAlignSpaceBetween6');

      expect(JSON.parse(strJson).$type).assertEqual('Flex');
      expect(JSON.parse(strJson).$attrs.constructor.direction).assertEqual('FlexDirection.Row');
      expect(JSON.parse(strJson).$attrs.constructor.wrap).assertEqual('FlexWrap.NoWrap');
      expect(JSON.parse(strJson).$attrs.constructor.justifyContent).assertEqual('FlexAlign.SpaceBetween');
      let locationText1: RectValue = CommonFunc.getComponentRect('AlignSpaceBetween16');
      let locationText2: RectValue = CommonFunc.getComponentRect('AlignSpaceBetween17');
      let locationText3: RectValue = CommonFunc.getComponentRect('AlignSpaceBetween18');
      let locationFlex: RectValue = CommonFunc.getComponentRect('FlexAlignSpaceBetween6');
      expect(Math.floor(Math.abs(locationText1.top - locationText2.top)) <= 1).assertTrue();
      expect(Math.floor(Math.abs(locationText2.top - locationText3.top)) <= 1).assertTrue();
      expect(Math.floor(Math.abs((Math.round((locationText1.top - locationFlex.top) * 10) / 10) - (Math.round(vp2px(5) * 10) / 10))) <= 1)
        .assertTrue();
      expect(Math.floor(Math.abs((locationText1.bottom - locationText1.top) - vp2px(50))) <= 1).assertTrue();
      expect(Math.floor(Math.abs((locationText2.bottom - locationText2.top) - vp2px(100))) <= 1).assertTrue();
      expect(Math.floor(Math.abs((locationText3.bottom - locationText3.top) - vp2px(150))) <= 1).assertTrue();
      expect(Math.floor(Math.abs((Math.round((locationFlex.bottom - locationText3.bottom) * 10) / 10) - (Math.round(vp2px(45) * 10) / 10))) <= 1)
        .assertTrue();
      expect(Math.floor(Math.abs((Math.round((locationText1.left - locationFlex.left) * 10) / 10) - (Math.round(vp2px(5) * 10) / 10))) <= 1)
        .assertTrue();
      expect(Math.floor(Math.abs((Math.round((locationText3.right) * 10) / 10) - (Math.round((locationFlex.right - vp2px(5)) * 10) / 10))) <= 1)
        .assertTrue();
      expect(Math.floor(Math.abs((locationText1.right - locationText1.left) - (locationText2.right - locationText2.left))) <= 1)
        .assertTrue();
      expect(Math.floor(Math.abs((locationText3.right - locationText3.left) - (locationText2.right - locationText2.left))) <= 1)
        .assertTrue();
      expect(Math.floor(Math.abs((locationText2.right - locationText2.left) - vp2px(150))) <= 1).assertTrue();
      expect(Math.floor(Math.abs((locationText2.left - locationText1.right) - (locationText3.left - locationText2.right))) <= 1)
        .assertTrue();
      expect(Math.floor(Math.abs((locationText2.left - locationText1.right) - vp2px(20))) <= 1).assertTrue();
      console.info('new testAlignSpaceBetweenRowNoWrapMarPadInRange END');

    });
  })
}