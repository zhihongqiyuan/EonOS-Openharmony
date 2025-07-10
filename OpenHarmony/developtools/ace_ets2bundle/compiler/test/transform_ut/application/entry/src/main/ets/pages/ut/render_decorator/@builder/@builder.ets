/*
 * Copyright (c) 2022-2024 Huawei Device Co., Ltd.
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
@Builder function noParam(){
  Row(){
    Text('this is a no param builder')
  }
}
@Builder function specificParam(label1: string, label2: string) {
  Column() {
    Text(label1)
    Text(label2)
  }
}
@Entry
@Component
struct MyComponent {
  private arr: number[] = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
  private controller: TabsController = new TabsController()
  @State hideBar: boolean = true
  @Builder textBuilder() {
    Text("文本")
      .fontSize(30)
  }
  @Builder NavigationTitlePara(label:string) {
    Column() {
      Text(label)
        .width(80)
        .bindMenu(this.textBuilder)
    }
  }
  @Builder MenuBuilder() {
    Flex({ direction: FlexDirection.Column, justifyContent: FlexAlign.Center, alignItems: ItemAlign.Center }) {
      Text('Test menu item 1')
        .fontSize(20)
      Divider().height(10)
      Text('Test menu item 2')
        .fontSize(20)
    }.width(100)
  }

  build() {
    Column() {
      Row(){
        Text("Drag Me")
          .onDragStart((event: DragEvent, extraParams: string) => {
            console.log('Text onDragStarts, ' + extraParams)
          })
        specificParam('test1', 'test2')
      }
      .padding(10)
      .bindMenu(this.NavigationTitlePara("111"))
      Row(){
        Text('Test Text')
      }
      .padding(10)
      .bindPopup(false, {
        builder: this.MenuBuilder,
        onStateChange: (e) => {
          if(!e.isVisible){
            console.warn(JSON.stringify(e.isVisible))
          }
        }
      })
      Row() {
        Text('rightclick for menu')
      }
      .padding(10)
      .bindContextMenu(this.MenuBuilder, ResponseType.RightClick)
      Row(){
        Navigation() {
          List({ space: 5, initialIndex: 0 }) {
            ForEach(this.arr, (item) => {
              ListItem() {
                Text('' + item)
                  .width('90%')
                  .height(80)
                  .backgroundColor('#3366CC')
                  .borderRadius(15)
                  .fontSize(16)
                  .textAlign(TextAlign.Center)
              }.editable(true)
            }, item => item)
          }
          .listDirection(Axis.Vertical)
          .height(300)
          .margin({ top: 10, left: 18 })
          .width('100%')
          Button(this.hideBar ? "tool bar" : "hide bar")
            .onClick(() => {
              this.hideBar = !this.hideBar
            })
            .margin({ left: 135, top: 60 })
        }
        .title(noParam)
        .menus(this.textBuilder)
        .toolBar({ items: [
          { value: 'app', text: 'Grid', action: () => {
            console.log("app")
          } },
          { value: 'add', text: 'Add', action: () => {
            console.log("add")
          } },
          { value: 'collect', text: 'Collect', action: () => {
            console.log("collect")
          } }] })
        .hideToolBar(this.hideBar)
      }
      .padding(10)
      Row(){
        Tabs({ barPosition: BarPosition.Start, controller: this.controller }) {
          TabContent() {
            Text('111').width('100%').height('20').backgroundColor(Color.Pink)
          }
          .tabBar('pink')
          TabContent() {
            Text('222').width('100%').height('20').backgroundColor(Color.Yellow)
          }
          .tabBar('yellow')
          TabContent() {
            Text('333').width('100%').height('20').backgroundColor(Color.Blue)
          }
          .tabBar('blue')
        }
      }
      .padding(10)
    }
  }
}