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

module Keywords
  module_function

  def extensions
    @extensions
  end

  def build_tree(keys)
    tree = Hash.new()
    offset = 0

    while true
      has_more = false
      keys.each do |key|
          if key.length < offset + 1
            next
          end

        has_more = true
        func_name = key[0..offset];
        if tree.has_key?(func_name)
          tree[func_name].add(key[offset + 1])
        else
          tree[func_name] = Set[key[offset + 1]]
        end
      end

      if has_more
        offset += 1
        next
      end
      break
    end

    return tree
  end

  def wrap_data(data)
    @extensions = Hash.new()

    if !data || !data.extensions || !data.keywords
      return
    end

    data.extensions.each do |extension|
      @extensions[extension.name] = Hash.new()
      all_words = data.keywords.select do |desc|
        s_kw = Set.new(desc&.keyword)
        groups = [s_kw, Set.new(desc&.keyword_like), Set.new(desc&.custom_handler)]
        groups.combination(2).each { |a,b| raise "Conflicting keyword type" unless (a & b).empty? }

        active = Set.new();
        groups.each { |group| active.merge(group) }

        raise "Unknown extension" unless active.subset?(Set.new(data.extensions.map { |ext| ext.name }))
        active.include? extension.name
      end

      keywords = data.keywords.select do |desc|
        desc&.keyword&.include? extension.name or desc&.custom_handler&.include? extension.name
      end

      @extensions[extension.name]['keywords'] = keywords
      @extensions[extension.name]['keyword_starts'] = Set.new(keywords.map { |desc| desc.name[0] })
      @extensions[extension.name]['all_words'] = all_words
      @extensions[extension.name]['all_word_starts'] = Set.new(all_words.map { |desc| desc.name[0] })
      @extensions[extension.name]['tree'] = build_tree(all_words.map { |desc| desc.name })
    end
  end
end

def Gen.on_require(data)
  Keywords.wrap_data(data)
end

