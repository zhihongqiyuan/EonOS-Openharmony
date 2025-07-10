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
class Month {
  year: number
  month: number
  days: Array<number>

  constructor(year, month, days) {
    this.year = year;
    this.month = month;
    this.days = days;
  }
}

@Entry
@Component
struct MyComponent {
  private languages: string[] = ['ets', 'js', 'java']
  private weekNames: string[] = ['日', '一', '二', '三', '四', '五', '六']
  @State calendar: Month[] = [
    new Month(2020, 1, [...Array(31).keys()]),
    new Month(2020, 2, [...Array(28).keys()]),
    new Month(2020, 3, [...Array(31).keys()]),
    new Month(2020, 4, [...Array(30).keys()]),
    new Month(2020, 5, [...Array(31).keys()]),
    new Month(2020, 6, [...Array(30).keys()]),
  ]

  build() {
    Column() {
      Row(){
        ForEach(
          this.languages,
          (item: string, index: number) => {
            GridItem() {
              Text(item)
                .fontSize(30)
            }
          },
          (item: any, index: number) => item)
      }
      Row(){
        ForEach(
          this.weekNames,
          (item, index) => {
            GridItem() {
              if(item === '日'){
                Text(item)
                  .fontSize(20)
                  .fontColor(Color.Red)
              }else{
                Text(item)
                  .fontSize(10)
              }
            }
          })
      }
      Row() {
        Button('next month')
          .onClick(() => {
            this.calendar.shift()
            this.calendar.push({
              year: 2020,
              month: 7,
              days: [...Array(31)
                .keys()]
            })
          })
        ForEach(this.calendar,
          (item: Month) => {
            Text('month:' + item.month)
            Grid() {
              ForEach(item.days,
                (day: number) => {
                  GridItem() {
                    Text((day + 1).toString())
                      .fontSize(30)
                  }
                },
                (day: number) => day.toString())
            }
            .rowsGap(20)
          },
          (item: Month) => (item.year * 12 + item.month).toString())
      }
    }.width(100)
  }
}