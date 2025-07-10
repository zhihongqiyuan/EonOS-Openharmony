#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Copyright (c) 2024 Huawei Device Co., Ltd.
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

import re
import sys
import json
import pandas as pd

class SPDXLicenseMatcher:
    def __init__(self, input_excel_path, input_json_path):
        # Load Excel and SPDX JSON data
        self.df = pd.read_excel(input_excel_path)
        self.spdx_mapping = self._load_spdx_data(input_json_path)

    @staticmethod
    def _load_spdx_data(json_path):
        # Load SPDX JSON data with enhanced standardization on keys (case-insensitive, punctuation-free)
        with open(json_path, 'r', encoding='utf-8') as f:
            spdx_data = json.load(f)
            spdx_mapping = {re.sub(r'[^a-zA-Z0-9 ]', '', key.lower()): value for key, value in spdx_data.items()}
            return spdx_mapping

    @staticmethod
    def _normalize_license_name(name):
        """Further normalize license names by removing non-alphanumeric characters."""
        normalized_name = re.sub(r'[^a-zA-Z0-9 ]', '', name).lower()
        return normalized_name

    def copy_url_column(self):
        # Copy cc_url to match_url for reference
        self.df['match_url'] = self.df['cc_url']

    def match_license_column(self):
        # Map spdx_fixed_license_name column against SPDX data
        self.df['match_license'] = self.df['spdx_fixed_license_name'].apply(self._map_license)

    def _map_license(self, license_names):
        # Process multiple license names separated by semicolons
        license_keys = [self._normalize_license_name(name) for name in license_names.split(';')]
        matched_licenses = [self._find_license_match(key) for key in license_keys]
        
        # Filter out any None results and join by ';' to mimic input format
        matched_licenses = [license for license in matched_licenses if license]
        return ';'.join(matched_licenses) if matched_licenses else "No Match"

    def _find_license_match(self, key):
        # Attempt to find an exact match first
        if key in self.spdx_mapping:
            return self.spdx_mapping[key]
        
        # Fallback to fuzzy matching if no exact match found
        for spdx_key in self.spdx_mapping.keys():
            if all(word in spdx_key for word in key.split()):
                return self.spdx_mapping[spdx_key]
        return None

    def save_to_excel(self, output_excel_path):
        # Save the DataFrame to an Excel file
        self.df.to_excel(output_excel_path, index=False)
        print(f"[INFO] Final processed results saved to {output_excel_path}")



def main(input_excel_path, input_json_path, output_excel_path):
    matcher = SPDXLicenseMatcher(input_excel_path, input_json_path)
    #matcher.copy_url_column()
    matcher.match_license_column()
    matcher.save_to_excel(output_excel_path)

if __name__ == "__main__":
    input_excel_path = sys.argv[1]
    input_json_path = sys.argv[2]
    output_excel_path = sys.argv[3]
    main(input_excel_path, input_json_path, output_excel_path)

