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

// @Monitor case with @ObservedV2

@ObservedV2
class Info {
  @Trace name: string = "Tom";
  @Trace region: string = "North";
  @Trace job: string = "Teacher";
  age: number = 25;
  // name被@Trace装饰，能够监听变化
  @Monitor("name")
  onNameChange(monitor: IMonitor) {
    console.log("name change from " + monitor.value()?.before + " to " + monitor.value()?.now);
  }
  // age未被@Trace装饰，不能监听变化
  @Monitor("age")
  onAgeChange(monitor: IMonitor) {
    console.log("age change from " + monitor.value()?.before + " to " + monitor.value()?.now);
  }
  // region与job均被@Trace装饰，能够监听变化
  @Monitor("region", "job")
  onChange(monitor: IMonitor) {
    monitor.dirty.forEach((path: string) => {
      console.log(path + " change from " + monitor.value(path)?.before + " to " + monitor.value(path)?.now);
    })
  }
}
@Entry
@ComponentV2
struct Index {
  info: Info = new Info();
  build() {
    Column() {
      Button("change name")
        .onClick(() => {
          this.info.name = "Jack"; // 能够触发onNameChange方法
        })
      Button("change age")
        .onClick(() => {
          this.info.age = 26; // 不能够触发onAgeChange方法
        })
      Button("change region")
        .onClick(() => {
          this.info.region = "South"; // 能够触发onChange方法
        })
      Button("change job")
        .onClick(() => {
          this.info.job = "Driver"; // 能够触发onChange方法
        })
    }
  }
}