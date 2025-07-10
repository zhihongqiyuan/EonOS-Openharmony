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

/*
 * This is a test case about the use of 'MenuItem', 'AlphabetIndexer' component, and 'bindContentCover' and 'bindMenu' attribute with two-way sync binding.
 */

@Entry
@Component
struct TestMenuItemAndOthers {
  @State num: number = 2
  @State isShow: boolean = false
  @State select: boolean = true
  @State select2: boolean = true
  @State select3: boolean = true
  private value: string[] = ['#', 'A', 'B', 'C', 'D', 'E', 'F', 'G',
    'H', 'I', 'J', 'K', 'L', 'M', 'N',
    'O', 'P', 'Q', 'R', 'S', 'T', 'U',
    'V', 'W', 'X', 'Y', 'Z']

  @Builder bindContentCoverBuilder() {
    Button("transition modal 1")
      .onClick(() => {
        this.isShow = true
      })
      .bindContentCover(this.isShow!!, this.myBuilder(), {
        modalTransition: ModalTransition.NONE,
        backgroundColor: Color.Pink
      })
  }

  @Builder
  myBuilder() {
    Column() {
      Button("close modal 1")
        .onClick(() => {
          this.isShow = false;
        })
    }
  }

  @Builder
  MyMenu(){
    Menu() {
      MenuItem({ content: "菜单选项" })
      MenuItem({ content: "菜单选项" })
        .enabled(false)
      MenuItem({
        content: "菜单选项",
        builder: ():void=>this.SubMenu()
      })
        .selected(this.select!!)
      MenuItemGroup({ header: '小标题' }) {
        MenuItem({
          content: "菜单选项",
          builder: ():void=>this.SubMenu()
        })
          .selected(this.select2!!)
      }
      MenuItem({
        content: "菜单选项",
      })
    }
  }

  @Builder
  SubMenu() {
    Menu() {
      MenuItem({ content: "复制", labelInfo: "Ctrl+C" })
      MenuItem({ content: "粘贴", labelInfo: "Ctrl+V" })
        .selected(this.select3!!)
    }
  }

  build() {
    Column() {
      this.bindContentCoverBuilder()
      AlphabetIndexer({ arrayValue: this.value, selected: this.num!! })
      Column() {
        Text('click to show menu')
      }
      .bindMenu(this.MyMenu)
    }
  }
}