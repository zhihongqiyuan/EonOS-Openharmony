# -*- coding: utf-8 -*-
# Copyright (c) 2025 Huawei Device Co., Ltd.
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


import os
import re
import json
from functools import lru_cache
from collections import defaultdict

class BuildProcessor:
    
    def __init__(self, root_dir, ace_root):
        self.root_dir = root_dir
        self.ace_root = ace_root
        self.build_info = defaultdict(lambda: {"name": "", "source_list": [], "deps_list": [], "include_list": [], "config_list": []})
        self.data_json = []
        self.group_json = []
        self.type_deps = {}
        self.heads = {}
        
        self.unittest_pattern = re.compile(
            r'(ace|ohos)_unittest\("([^"]*)"\)\s*\{(.*?)(?=\})',
            re.DOTALL | re.MULTILINE
        )
        self.group_pattern = re.compile(
            r'group\("([^"]*)"\)\s*\{(.*?)(?=\})',
            re.DOTALL | re.MULTILINE
        )
        self.source_set_pattern = re.compile(
            r'ohos_source_set\("([^"]*)"\)\s*\{(.*?)(?=\})',
            re.DOTALL | re.MULTILINE
        )
        self.type_get_pattern = r'''
            if\s*\(type\s*==\s*"([^"]+)"\)
            \s*(\{
                (?:
                    [^{}]*
                    | (?R)
                )*
            \})
        '''

        self.sources_pattern = re.compile(r'sources\s*[+]?=\s*\[(.*?)\]', re.DOTALL)
        self.deps_pattern = re.compile(r'deps\s*[+]?=\s*\[(.*?)\]', re.DOTALL)
        self.includes_pattern = re.compile(r'include_dirs\s*[+]?=\s*\[(.*?)\]', re.DOTALL)
        self.configs_pattern = re.compile(r'configs\s*[+]?=\s*\[(.*?)\]', re.DOTALL)
        self.type_pattern = re.compile(r'type\s*=\s*"([^"]+)"')
    
    def execute(self):
        self.parse_build_gn("foundation/arkui/ace_engine/test/unittest/ace_unittest.gni", self.type_get_pattern)

        for root, _, files in os.walk(self.root_dir):
            if "BUILD.gn" in files:
                path = os.path.join(root, "BUILD.gn")
                self.parse_build_gn(path, self.unittest_pattern)
                self.parse_groups(path)
        
        self.parse_build_gn("foundation/arkui/ace_engine/test/unittest/BUILD.gn", self.source_set_pattern)

        for target in self.data_json:
            target["deps_list"] = self._get_deps_list(target)
            target["dep_h"] = [h for d in target["deps_list"] for h in self.process_file(d)]

        change_files, oh_fields = self.process_changes()
        
        if len(oh_fields) == 1 and oh_fields[0] == "arkui_ace_engine":
            print(" ".join(self.analyze_impact(change_files)))
        else:
            print(f"TDDarkui_ace_engine")
        self.generate_output()


    def parse_build_gn(self, file_path, pattern):
        content = self._read_file(file_path)
        processed_content = "\n".join(line.split("#")[0].rstrip()
                                    for line in content.splitlines())
        if pattern == self.type_get_pattern:
            block_content = self.extract_gn_block(content, 'if (type == "components")')
            self.set_type("components", block_content, file_path)
            block_content = self.extract_gn_block(content, 'if (type == "new")')
            self.set_type("new", block_content, file_path)
            block_content = self.extract_gn_block(content, 'if (type == "pipeline")')
            self.set_type("pipeline", block_content, file_path)
        else:
            for match in pattern.finditer(processed_content):
                self._process_unittest(match, file_path)


    def extract_gn_block(self, content, start_condition):
        pattern = re.compile(
            r'{}\s*{{'.format(re.escape(start_condition)), 
            re.DOTALL
        )
        match = pattern.search(content)
        if not match:
            return ""

        start_pos = match.end()
        brace_count = 1
        index = start_pos
        in_string = False
        in_comment = False
        quote_char = None
        escape = False

        while index < len(content) and brace_count > 0:
            char = content[index]

            if in_comment:
                if char == '\n':
                    in_comment = False
                index += 1
                continue
            if in_string:
                if escape:
                    escape = False
                elif char == '\\':
                    escape = True
                elif char == quote_char:
                    in_string = False
            else:
                if char == '#':
                    in_comment = True
                elif char in ('"', "'"):
                    in_string = True
                    quote_char = char
                elif char == '{':
                    brace_count += 1
                elif char == '}':
                    brace_count -= 1

            index += 1
        if brace_count != 0:
            return "" 
        return content[start_pos:index-1].strip()

    def set_type(self, type, content, file_path):
        base_path = os.path.dirname(file_path)
        deps = self._get_gn_content(self.deps_pattern, content, base_path)
        self.type_deps[type] = deps

    def process_file(self, file_path):
        if self.heads.get(file_path):
            return self.heads.get(file_path)
        content = self._read_file(file_path)
        self.heads[file_path] = {header for line in content.split('\n') 
                if (header := self._process_includes(line))}
        return self.heads[file_path]

    def parse_groups(self, file_path):
        content = self._read_file(file_path)
        processed_content = "\n".join(line.split("#")[0].rstrip() 
                                    for line in content.splitlines())
        
        for match in self.group_pattern.finditer(processed_content):
            self._process_group(match, file_path)

    def process_changes(self):
        change_info = self._read_json("change_info.json")
        openharmony_fields = [v["name"] for v in change_info.values() if "name" in v]
        
        change_files = []
        file_operations = {
            "added": lambda x: x,
            "rename": lambda x: [item for pair in x for item in pair],
            "modified": lambda x: x,
            "deleted": lambda x: x
        }
        
        for value in change_info.values():
            changed_files = value.get("changed_file_list", {})
            for op, processor in file_operations.items():
                if op in changed_files:
                    change_files.extend(processor(changed_files[op]))
        
        return (
            [os.path.join(self.ace_root, f) for f in change_files],
            openharmony_fields
        )

    def generate_output(self):
        with open("test_targets.json", "w") as f:
            json.dump(self.data_json, f, indent=2)
            
        with open("groups.json", "w") as f:
            json.dump(self.group_json, f, indent=2)

    def analyze_impact(self, change_files):
        tdd_data = self._read_json("developtools/integration_verification/tools/gated_check_in/ace_engine.json") or {}
        adapted_targets = set(tdd_data.get("adapted_test_targets", []))
        adapting_targets = set(tdd_data.get("adapting_test_targets", []))

        change_set = set(change_files)
        impacted = []
        
        for target in self.data_json:
            target_sets = {
                "source_list": set(target["source_list"]),
                "deps_list": set(target["deps_list"]),
                "includes_list": set(target["includes_list"]),
                "configs_list": set(target["configs_list"]),
                "source_h": set(target["source_h"]),
                "dep_h": set(target["dep_h"]),
                "includes_h": set(target["includes_h"]),
                "configs_h": set(target["configs_h"])
            }
            if any(change_set & s for s in target_sets.values()):
                if target["test_target"] not in adapting_targets:
                    impacted.append(target["test_target"])
        
        return self.ret_build_target(impacted, change_files)

    @lru_cache(maxsize=128)
    def _read_file(self, file_path):
        try:
            with open(file_path, 'r', encoding='utf-8') as f:
                return f.read()
        except Exception as e:
            return ""

    def ret_build_target(self, impacted, change_files):
        if not impacted:
            for file in change_files:
                if file.endswith(".h"):
                    return ["TDDarkui_ace_engine"]
            return ["foundation/arkui/ace_engine/test/unittest/adapter/ohos/entrance:container_test"]
        return impacted

    def _get_deps_list(self, target):
        ret = []
        for dep in target["deps_list"]:
            ret = list(set(ret + self._get_source_list(dep)))
            ret.append(os.path.join(dep.split(":", 1)[0], "BUILD.gn"))
        return ret

    def _get_source_list(self, dep):
        for target in self.data_json:
            if dep == target["test_target"]:
                return target["source_list"]
        return []

    def _process_includes(self, line):
        for pattern in [r'#include\s*"(.*?)"', r'#include\s*<(.*?)>']:
            match = re.match(pattern, line)
            if match and (header := match.group(1)).endswith('.h'):
                return header
        return None

    def _process_unittest(self, match, file_path):
        base_path = os.path.dirname(file_path)
        if base_path == "foundation/arkui/ace_engine/test/unittest":
            target_name = match.group(1)
            target_content = match.group(2)
        else:
            target_name = match.group(2)
            target_content = match.group(3)
        
        
        sources = self._get_gn_content(self.sources_pattern, target_content, base_path)
        sources.append(file_path)
        deps = self._get_gn_content(self.deps_pattern, target_content, base_path)
        includes = self._get_include_files(self._get_gn_content(self.includes_pattern, target_content, base_path))
        configs = self._get_gn_content(self.configs_pattern, target_content, base_path)
        
        source_h = {h for s in sources for h in self.process_file(s)}
        dep_h = {h for d in deps for h in self.process_file(d)}
        include_h = {h for s in includes for h in self.process_file(s)}
        config_h = {h for d in configs for h in self.process_file(d)}
        if match.group(1) == "ace":
            for match_ in self.type_pattern.finditer(target_content):
                deps += self.type_deps[match_.group(1)]
        build_target = f"{os.path.dirname(file_path)}:{target_name}"
        self.data_json.append({
            "test_target": build_target,
            "source_list": sources,
            "deps_list": deps,
            "includes_list": includes,
            "configs_list": configs,
            "source_h": list(source_h),
            "dep_h": list(dep_h),
            "includes_h": list(include_h),
            "configs_h": list(config_h)
        })

    def _get_include_files(self, includes_list):
        all_files = []
        for path in includes_list:
            for root, dirs, files in os.walk(path):
                for file in files:
                    all_files.append(os.path.join(root, file))
        return all_files

    def _process_group(self, match, file_path):
        group_name = match.group(1)
        group_content = match.group(2)
        base_path = os.path.dirname(file_path)
        
        deps = [self._normalize_path(d, base_path).replace("/:", ":")
                for d in self._get_gn_content(self.deps_pattern, group_content, "")]
        
        self.group_json.append({
            "group_name": f"{base_path}:{group_name}",
            "deps_list": deps
        })

    def _get_gn_content(self, pattern, content, base_path):
        all_matches = pattern.finditer(content)
        sources = []
        for match in all_matches:
            matched_content = match.group(1)
            sources.extend([
                self._normalize_path(s, base_path)
                for s in matched_content.split(',') if s.strip()
            ])
        return sources

    def _normalize_path(self, s, base_path):
        s = s.strip().strip('"')
        if '/' not in s:
            return os.path.join(base_path, s)
        return s.replace('$ace_root', self.ace_root)

    def _read_json(self, path):
        try:
            with open(path, 'r', encoding='utf-8') as f:
                return json.load(f)
        except Exception as e:
            return {}


if __name__ == "__main__":
    processor = BuildProcessor(
        root_dir="foundation/arkui/ace_engine",
        ace_root="foundation/arkui/ace_engine"
    )
    processor.execute()
