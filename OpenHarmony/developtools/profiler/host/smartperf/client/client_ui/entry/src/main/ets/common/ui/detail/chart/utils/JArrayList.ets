/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
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

// 说明: 在 JAVA中 List是一套interface接口，继承自Collection
// AbstractList 实现了List接口 ，
// 最终ArrayList/ArrayQueue等，继承自AbstractList
// 因此这里可能需要进行修改
import { JList } from './JList';

export class JArrayList<T> implements JList<T> {
  public dataSouce: Array<T>;
  public listSize: number; // 列表的大小
  public pos: number; // 列表中当前的位置

  constructor() {
    this.dataSouce = [];
    this.listSize = 0; // 列表的大小
    this.pos = 0; // 列表中当前的位置
  }

  /**
   * 在列表的末尾添加新元素
   * @param {*} element 要添加的元素
   */
  append(element: T) {
    this.dataSouce[this.listSize++] = element;
  }

  add(element: T) {
    this.append(element);
    return this;
  }

  addAll(newArray: JList<T>) {
    let len = newArray.length();
    for (let i = 0; i < len; i++) {
      this.append(newArray.get(i));
    }
  }

  /**
   * 在列表中插入一个元素
   * @param {*} element
   * @param {*} after
   */
  insert(element: T) {
    this.dataSouce.push(element);
    this.listSize++;
  }

  /**
   * 在列表中移除一个元素
   * @param {*} element 要删除的元素
   */
  remove(element: T) {
    // 查找当前元素的索引
    const index = this.dataSouce.indexOf(element);
    if (index >= 0) {
      this.dataSouce.splice(index, 1);
      this.listSize--;
      return true;
    }
    return false;
  }

  /**
   * 判断给定的值是否在列表中
   */
  contains(element: T) {
    return this.dataSouce.indexOf(element) > -1;
  }
  indexOf(element: T): number {
    return this.dataSouce.indexOf(element);
  }

  /**
   * 将列表的当前位置设移动到第一个元素
   */
  front() {
    this.pos = 0;
  }

  /**
   * 将列表的当前位置移动到最后一个元素
   */
  end() {
    this.pos = this.listSize - 1;
  }

  /**
   * 将当前位置前移一位
   */
  prev() {
    if (this.pos > 0) {
      --this.pos;
    }
  }

  /**
   * 将当前位置向后移一位
   */
  next() {
    if (this.pos <= this.listSize - 1) {
      ++this.pos;
    }
  }

  /**
   * 返回列表的当前位置
   */
  currPos() {
    return this.pos;
  }

  /**
   * 将当前位置移动到指定位置
   * @param {*} position
   */
  moveTo(position) {
    this.pos = position;
  }

  /**
   * 返回当前位置的元素
   */
  getElement() {
    return this.dataSouce[this.pos];
  }
  /**
   * 返回指定位置的元素
   */
  get(pos: number) {
    return this.dataSouce[pos];
  }
  at(pos: number): T {
    return this.get(pos);
  }

  /**
   * 清楚列表中的元素
   */
  clear() {
    delete this.dataSouce;
    this.dataSouce = [];
    this.listSize = 0;
    this.pos = 0;
  }

  /**
   * 列表的长度
   */
  length(): number {
    return this.listSize;
  }
  size(): number {
    return this.listSize;
  }
  isEmpty(): boolean {
    return this.listSize == 0;
  }

  /**
   * 显示当前列表的元素
   */
  toArray(a?: T[]): T[] {
    if (a == null) {
      return this.dataSouce;
    }
    if (a.length < this.length()) {
      return [];
    }
  }
  /**
   * 显示当前列表的元素
   */
  toString(interval?: string): string {
    if (interval == null) {
      return this.dataSouce.join('');
    }
    return this.dataSouce.join(interval);
  }
}
