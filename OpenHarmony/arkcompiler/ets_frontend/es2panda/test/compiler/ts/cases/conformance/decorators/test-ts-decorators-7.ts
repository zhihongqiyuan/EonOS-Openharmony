/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


function classDecorator1(ctor)
{
  print("classDecorator1")
}

function classDecorator2()
{
  return (ctor)=>{print("classDecorator2")}
}

function methodDecorator1(target, key, property)
{
  print("methodDecorator1")
}

function methodDecorator2()
{
  return (target, key, property)=>{print("methodDecorator2")}
}

function propertyDecorator1(target, key)
{
  print("propertyDecorator1")
}

function propertyDecorator2()
{
  return (target, key)=>{print("propertyDecorator2")}
}

function paramDecorator1(target, key, index)
{
  print("paramDecorator1");
}

function paramDecorator2()
{
  return (target, key, index)=>{print("paramDecorator2")}
}

@classDecorator1
@classDecorator2()
class C {
  @methodDecorator1
  @methodDecorator2()
  get x() {return 1;}

  @propertyDecorator1
  @propertyDecorator2()
  static b : number = 1;

  @propertyDecorator1
  @propertyDecorator2()
  a : number = 1;

  @methodDecorator1
  @methodDecorator2()
  static g1(a, @paramDecorator1 @paramDecorator2() b){}

  @methodDecorator1
  @methodDecorator2()
  g2(a, @paramDecorator1 @paramDecorator2() b){}

  constructor(a, b, @paramDecorator1 @paramDecorator2() c){}
}
