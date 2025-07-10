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
@Entry
@Component
struct TestBuilder1 {
  build() {
    Column() {
      Text("hello")
    }
  }
  @Builder
  innerBuidler(value: string) {
    Column() {
      Text("hello")
    }
  }
}

@Component
struct TestBuilderChild {
  build() {
    Column() {
      Text("hello")
    }
  }
  @Builder
  innerBuidler(value: object) {
    Column() {
      Text("hello")
    }
  }
}

@Reusable
@Component
struct TestBuilderReusable {
  build() {
    Column() {
      Text("hello")
    }
  }

  @Builder
  innerBuidler(value: string) {
    Column() {
      Text("hello")
    }
  }
}

@Builder
function commonBuilder() {}

@Builder
function testIfComponent(value: number) {
  Column() {
    if (value === 1) {
      Text('toggle1')
    } else if(value === 2) {
      Text('toggle2')
    } else if (value === 3) {
      Text('toggle3')
    } else {
      Text('toggle no thing')
    }
    if (value === 1) {
      Text('toggle1 Single')
    }
  }
}

@Builder
function testIfIdComponent(value: number) {
  Column() {
    if(value) {
      if (value < 0) {
        Text('count is negative')
          .fontSize(32)
          .id('id1')
      } else if (value % 2 === 0) {
        Divider()
          .id('id2')
        Text('even')
          .fontSize(32)
          .id('id3')
      } else {
        Divider()
          .id('id4')
        Column() {
          Text('odd')
            .fontSize(32)
            .id('id5')
        }
        .id('id10')
      }
    } else {
      Text('fail')
        .id('id6')
        .fontSize(32)
    }

    if(value) Text('odd2').fontSize(32).id('id7')

    List({space: 3}) {
      if (value) {
        ListItem() {
          Row() {
            Text().fontSize(20).margin({left:10})
          }.margin({left: 10, right: 10}).id('id11')
        }
        .id('id8')
      }
    }

    Tabs({ barPosition: BarPosition.Start, controller: null }) {
      if (value) {
        TabContent() {
          Text('111').width('100%').height('20').backgroundColor(Color.Pink)
        }
        .tabBar('pink')
        .id('id9')
      }
    }

    if (value) {
      if (value === 10) {
        Column() {
          Text('111')
        }
        XComponent({id: 'special', type: ''}).id('id12')
        Column() {
          Text('11')
        }
        .id('id13')
        Column() {
          Text('222')
        }
      }
    }
  }
}

@Builder
function testItemComponent(value: Object) {
  Column() {
    List() {
      ForEach(value as string[],
        (item: string) => {
          ListItem() {
            Column() {
              Text(item)
              Text(value as string)
              Text("hello  ForEach testItemComponent")
            }
          }
        }
      )
    }
    List() {
      LazyForEach (value as IDataSource,
        (row: string) => {
          ListItem() {
            Column() {
              Text(row)
              Text(value as string)
              Text("hello LazyForEach testItemComponent")
            }
          }
        },
        (row: string) => row)
    }
  }
}

@Builder
function testTabComponent(value: object) {
  Column() {
    Tabs({ barPosition: BarPosition.Start, index: 1, controller: null}) {
      TabContent() {
        Flex() {
          Column() {
            Text('text1')
              .height(100)
              .width(200)
            Text(value.toString())
              .height(100)
              .width(200)
          }
          .height(100)
          .width(200)
        }
        .height(100)
        .width(200)
      }
      .tabBar("TabBar")
      .height(100)
      .width(200)
      TabContent() {
        Text('text2')
      }
      .tabBar("TabBar 2")
      .height(100)
      .width(200)
    }
  }
}

class BasicDataSource implements IDataSource {
  private listeners: DataChangeListener[] = []

  public totalCount(): number {
    return 0
  }
  public getData(index: number): Object {
    return new Object()
  }

  registerDataChangeListener(listener: DataChangeListener): void {
    if (this.listeners.indexOf(listener) < 0) {
      console.info('add listener')
      this.listeners.push(listener)
    }
  }
  unregisterDataChangeListener(listener: DataChangeListener): void {
    const pos = this.listeners.indexOf(listener);
    if (pos >= 0) {
      console.info('remove listener')
      this.listeners.splice(pos, 1)
    }
  }

  notifyDataReload(): void {
    this.listeners.forEach(listener => {
      listener.onDataReloaded()
    })
  }
  notifyDataAdd(index: number): void {
    this.listeners.forEach(listener => {
      listener.onDataAdd(index)
    })
  }
  notifyDataChange(index: number): void {
    this.listeners.forEach(listener => {
      listener.onDataChange(index)
    })
  }
  notifyDataDelete(index: number): void {
    this.listeners.forEach(listener => {
      listener.onDataDelete(index)
    })
  }
  notifyDataMove(from: number, to: number): void {
    this.listeners.forEach(listener => {
      listener.onDataMove(from, to)
    })
  }
}

class MyDataSource extends BasicDataSource {
  private dataArray: string[] = ['/path/image0', '/path/image1', '/path/image2', '/path/image3']

  public totalCount(): number {
    return this.dataArray.length
  }
  public getData(index: number): Object {
    return this.dataArray[index]
  }

  public addData(index: number, data: string): void {
    this.dataArray.splice(index, 0, data)
    this.notifyDataAdd(index)
  }
  public pushData(data: string): void {
    this.dataArray.push(data)
    this.notifyDataAdd(this.dataArray.length - 1)
  }
}