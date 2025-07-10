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
 * This is a test case about the use of "!!" two-way sync binding in the component 'Rating', 'Search' and 'Swiper'.
 */

class MyDataSource implements IDataSource {
  private list: string[] = []

  constructor(list: string[]) {
    this.list = list
  }

  totalCount(): number {
    return this.list.length
  }

  getData(index: number): string {
    return this.list[index]
  }

  registerDataChangeListener(listener: DataChangeListener): void {
  }

  unregisterDataChangeListener() {
  }
}

@Entry
@Component
struct RatingExample {
  @State rating: number = 3.5

  build() {
    Column() {
      Column() {
        Rating({ rating: this.rating!!, indicator: false })
        Text('current score is ' + this.rating)
        SearchExample()
      }.width(360)
    }
  }
}

@ComponentV2
struct SearchExample {
  @Local changeValue: string = ''
  @Local submitValue: string = ''
  @Local swiperIndex: number = 2
  controller: SearchController = new SearchController()
  private swiperController: SwiperController = new SwiperController()
  private data: MyDataSource = new MyDataSource([])
  textAreaController: TextAreaController = new TextAreaController()

  aboutToAppear(): void {
    let list: string[] = []
    for (let i = 1; i <= 10; i++) {
      list.push(i+'');
    }
    this.data = new MyDataSource(list)
  }

  @Builder showSwiper() {
    Swiper(this.swiperController) {
      LazyForEach(this.data, (item: string) => {
        Text(item)
        TextArea({
          text: item!!,
          placeholder: 'The text area can hold an unlimited amount of text. input your word...',
          controller: this.textAreaController
        })
      }, (item: string) => item)
    }
    .index(this.swiperIndex!!)
  }

  build() {
    Column({space: 10}) {
      Text('onSubmit:' + this.submitValue).fontSize(18).margin(15)
      Text('onChange:' + this.changeValue).fontSize(18).margin(15)
      Search({ value: this.changeValue!!, placeholder: 'Type to search...', controller: this.controller })
        .searchButton('SEARCH')
        .onSubmit((value: string) => {
          this.submitValue = value
        })
      this.showSwiper()
    }
  }
}