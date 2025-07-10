/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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

import Constants, {NotificationItemData, NotificationLayout as Layout} from '../../common/constants';

@Component
export default struct MultiItem {
  @Prop itemData: NotificationItemData
  @Prop isExpand: boolean

  build() {
    Column() {
      Text(this.isExpand ? this.itemData.longTitle : this.itemData.title)
        .maxLines(Constants.SINGLE_LINE)
        .textOverflow({ overflow: TextOverflow.Ellipsis })
        .fontColor($r("sys.color.ohos_fa_text_primary"))
        .fontSize($r("sys.float.ohos_id_text_size_body2"))
        .fontWeight(FontWeight.Medium)
        .lineHeight(Layout.TEXT_LINE_HEIGHT)
      if (this.isExpand) {
        ForEach(this.itemData.lines,
          (item: string) => {
            Text(item)
              .maxLines(Constants.SINGLE_LINE)
              .textOverflow({ overflow: TextOverflow.Ellipsis })
              .fontColor($r("sys.color.ohos_fa_text_secondary"))
              .fontSize($r("sys.float.ohos_id_text_size_body2"))
              .fontWeight(FontWeight.Regular)
              .lineHeight(Layout.TEXT_LINE_HEIGHT)
          }, (item: string) => item.toString()
        )
      } else {
        Text(this.itemData.text)
          .maxLines(Constants.SINGLE_LINE)
          .textOverflow({ overflow: TextOverflow.Ellipsis })
          .fontColor($r("sys.color.ohos_fa_text_secondary"))
          .fontSize($r("sys.float.ohos_id_text_size_body2"))
          .fontWeight(FontWeight.Regular)
          .lineHeight(Layout.TEXT_LINE_HEIGHT)
      }
    }.alignItems(HorizontalAlign.Start)
  }
}
