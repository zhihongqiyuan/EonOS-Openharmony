/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

import { BusinessError } from '@ohos.base';
import ScreenshotManager from '@ohos.screenshot';
import WindowMar from '@ohos.window';
import ImageMar from '@ohos.multimedia.image';
import Constants from '../common/constants';
import Log from '../../../../../../../../common/src/main/ets/default/Log';
import Want from '@ohos.app.ability.Want';
import userFileManager from '@ohos.filemanagement.userFileManager';
import common from '@ohos.app.ability.common';
import file from '@ohos.file.fs';

const TAG = 'ScreenShot-ScreenShotModel';

const SCREEN_SHOT_PATH = 'Screenshots/';
const SCREENSHOT_PREFIX = 'Screenshot';
const PICTURE_TYPE = '.jpg';
const SAVE_IMAGE_DELAY = 300;
const OPTIONS_QUALITY = 100;
const CREATE_WINDOW_DELAY = 300;

export class ScreenShotModel {
  private captureImage: ImageMar.PixelMap | undefined = undefined;
  private imageFileName: string = '';

  async shotScreen() {
    Log.showInfo(TAG, 'shotScreen');
    await new Promise<number>((resolve) => setTimeout(resolve, CREATE_WINDOW_DELAY));
    ScreenshotManager.save().then(async (data) => {
      Log.showInfo(TAG, `ScreenshotManager.save data:${JSON.stringify(data)}`);
      if (!!data) {
        this.captureImage = data;
        AppStorage.Set('captureImage', data);
        this.saveImage(this.captureImage, {
          format: 'image/jpeg',
          quality: OPTIONS_QUALITY,
        });
      }
    }).catch((err: BusinessError) => {
      Log.showInfo(TAG, `ScreenshotManager.save err:${JSON.stringify(err)}`);
    });
  }

  async saveImage(pixelMap: ImageMar.PixelMap, options: ImageMar.PackingOption) {
    let fileAsset: userFileManager.FileAsset | undefined = undefined;
    let fd: number | undefined = undefined;

    const context = globalThis.shotScreenContext as common.ServiceExtensionContext;
    try {
      const userFileMgr = userFileManager.getUserFileMgr(context);
      this.imageFileName = SCREENSHOT_PREFIX + '_' + (new Date()).getTime() + PICTURE_TYPE;
      Log.showInfo(TAG, `imageName: ${this.imageFileName}`);

      const packer = ImageMar.createImagePacker();
      this.showWindow();
      await new Promise<number>((resolve) => setTimeout(resolve, SAVE_IMAGE_DELAY));
      const packedImg = await packer.packing(pixelMap, options);

      const createOption: userFileManager.PhotoCreateOptions = {
        subType: userFileManager.PhotoSubType.SCREENSHOT,
      };
      fileAsset = await userFileMgr.createPhotoAsset(this.imageFileName, createOption);

      AppStorage.setOrCreate('imageUri', fileAsset.uri);
      Log.showInfo(TAG, `fileAsset uri: ${fileAsset.uri}`);

      fd = await fileAsset.open('w');
      await file.write(fd, packedImg);
      await file.fsync(fd);
      AppStorage.setOrCreate('imageFilename', this.imageFileName);
      Log.showInfo(TAG, `SaveImage success`);
    } catch (error) {
      Log.showInfo(TAG, `SaveImage failed, cause: ${error}`);
    } finally {
      if (fileAsset && fd !== undefined) {
        await fileAsset.close(fd);
        Log.showInfo(TAG, `FileIo close, successful`);
      }
    }
  }

  showWindow() {
    Log.showInfo(TAG, 'showWindow');
    WindowMar.find(Constants.WIN_NAME).then((win) => {
      win.show(() => {
        Log.showInfo(TAG, 'window show');
      });
    });
  }

  dismiss(): void {
    Log.showInfo(TAG, 'dismiss');
    //close ability
    globalThis.shotScreenContext.terminateSelf((err: BusinessError) => {
      Log.showInfo(TAG, `terminateSelf finish err:${err}`);
      WindowMar.find(Constants.WIN_NAME).then((win) => {
        win.destroy(() => {
          Log.showInfo(TAG, 'destroy the window finish');
        });
      });
    });
  }

  openAbility(wantData: Want) {
    Log.showInfo(TAG, `openAbility want:${JSON.stringify(wantData)}`);
    globalThis.shotScreenContext.startAbility(wantData);
  }
}

let screenShotModel = new ScreenShotModel();

export default screenShotModel as ScreenShotModel;