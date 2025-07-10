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
class ClassA {
	message: string
	constructor(message: string) {
		this.message = message
	}
}

@Entry
@Component
struct ChildComponent {
	@State arr: ClassA[] = [new ClassA('0'), new ClassA('1'), new ClassA('2')]
	count: number = 0;
	build() {
		List() {
			Repeat<ClassA>(this.arr)
				.each((obj: RepeatItem<ClassA>) => {
					ListItem() {
						Text(obj.item.message)
					}
				})
				.key((item: ClassA)=>{
					return JSON.stringify(item)
				})
				.virtualScroll({totalCount: this.arr.length})
				.templateId((item: ClassA, index: number)=>{
					return index.toString()
				})
				.template('1', (obj: RepeatItem<ClassA>) => {
					ListItem() {
						Text(obj.item.message)
					}
				})
		}
		.height(500)
	}
}