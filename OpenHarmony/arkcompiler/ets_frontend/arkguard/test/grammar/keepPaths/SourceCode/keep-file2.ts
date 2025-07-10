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

export class Student {
  name: string;
  age: number;
  grade: number;

  constructor(name: string, age: number, grade: number) {
      this.name = name;
      this.age = age;
      this.grade = grade;
  }

  introduce() {
      console.log(`Hi, I'm ${this.name}. I'm ${this.age} years old and I'm in grade ${this.grade}.`);
  }
}

class Classroom {
  className: string;
  students: Student[];

  constructor(className: string, students: Student[]) {
      this.className = className;
      this.students = students;
  }

  listStudents() {
      console.log(`Class ${this.className} has the following students:`);
      this.students.forEach(student => {
          console.log(`${student.name}, age ${student.age}, grade ${student.grade}`);
      });
  }
}

const student1 = new Student('Alice', 15, 9);
const student2 = new Student('Bob', 14, 8);
const student3 = new Student('Charlie', 16, 10);

const classroom = new Classroom('Class A', [student1, student2, student3]);

classroom.listStudents();
