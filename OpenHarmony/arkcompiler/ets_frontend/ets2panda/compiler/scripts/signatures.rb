#!/usr/bin/env ruby
# Copyright (c) 2021-2024 Huawei Device Co., Ltd.
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

require 'ostruct'
require 'set'
require 'delegate'

module Signatures
  DEFINES = Hash.new()
  SIGNATURES = Hash.new()
  BUILTINS = Hash.new()
  PRIMITIVES = Hash.new()
  TYPEDESCRIPTORS = Hash.new()
  DYNAMIC = Hash.new()

  def wrap_data(data)
    refs = Hash.new()

    unless data
      return
    end

    data.defines.each do |define|
      refs[define.ref] = define.name
      Signatures::DEFINES[define.ref] = define
    end

    data.packages.each do |package|
        refs[package.ref] = package.name
    end

    data.primitives.each do |primitive|
        refs[primitive.ref] = primitive.name
        Signatures::PRIMITIVES[primitive.ref] = primitive.name
    end

    data.typedescriptors.each do |typedescriptor|
      refs[typedescriptor.ref] = typedescriptor.name
      Signatures::TYPEDESCRIPTORS[typedescriptor.ref] = typedescriptor.name
    end

    data.builtins.each do |builtin|
        sig = "%s.%s" % [refs[builtin.package], builtin.name]
        refs[builtin.ref] = sig
        Signatures::BUILTINS[builtin.ref] = [sig, builtin.name]
    end

    data.signatures.each do |signature|
        method_name = signature.method_name.start_with?('$') ? refs[signature.method_name[1..-1]] : signature.method_name
        sig = "%s.%s:" % [refs[signature.callee], method_name]

        signature.params.each do |param|
            sig += "%s;" % [refs[param]]
        end

        sig += "%s;" % [refs[signature.return_type]]

        Signatures::SIGNATURES[signature.ref] = sig
    end

    data.dynamiclangs.each do |lang|
      Signatures::DYNAMIC[lang.name] = OpenStruct.new(lang)
    end
  end
  module_function :wrap_data
end

def Gen.on_require(data)
    Signatures.wrap_data(data)
end

