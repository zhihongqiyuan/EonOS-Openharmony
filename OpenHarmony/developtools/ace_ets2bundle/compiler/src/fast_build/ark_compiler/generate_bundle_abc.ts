/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

import { JSBUNDLE } from './common/ark_define';
import { BundlePreviewMode } from './bundle/bundle_preview_mode';
import { BundleBuildMode } from './bundle/bundle_build_mode';

/**
 * rollup generatebundle hook
 * @param {rollup OutputOptions} options
 * @param {rollup [fileName: string]: AssetInfo | ChunkInfo} bundle
 * @param {boolean} isWrite
 */
export function generateBundleAbc(options: any, bundle: any, isWrite: boolean) {
  if (bundle === null || this.share.projectConfig.compileMode !== JSBUNDLE) {
    return;
  }

  generateAbc(this, bundle);
}

function generateAbc(rollupObject: any, rollupBundleFileSet: any) {
  if (rollupObject.share.projectConfig.watchMode === 'true') {
    const bundlePreviewMode: BundlePreviewMode = new BundlePreviewMode(rollupObject, rollupBundleFileSet);
    bundlePreviewMode.generateAbc();
  } else {
    const bundleBuildMode: BundleBuildMode = new BundleBuildMode(rollupObject, rollupBundleFileSet);
    bundleBuildMode.generateAbc();
  }
}
