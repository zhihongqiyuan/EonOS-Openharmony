/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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

import image from '@ohos.multimedia.image';
import WallpaperMar from '@ohos.wallpaper'
import {Log} from '@ohos/common'

const TAG = 'ScreenLock-WallpaperViewModel'

export default class WallpaperViewModel {
    private wallpaperData: image.PixelMap = undefined

    ViewModelInit(): void {
        Log.showDebug(TAG, "ViewModelInit");
        this.getScreenLockWallpaper()
    }

    ViewModelDestroy(): void {
        Log.showDebug(TAG, "ViewModelDestroy");
        this.freeScreenLockWallpaper();
    }

    getWallpaperData() {
        return this.wallpaperData;
    }

    private getScreenLockWallpaper() {
        Log.showInfo(TAG, 'getScreenLockWallpaper');
        WallpaperMar.getPixelMap(WallpaperMar.WallpaperType.WALLPAPER_LOCKSCREEN, (error, data) => {
            if (error != undefined && error != null) {
                Log.showError(TAG, 'getScreenLockWallpaper error:' + JSON.stringify(error));
                this.getScreenLockWallpaper()
            } else {
                Log.showDebug(TAG, 'getScreenLockWallpaper data:' + JSON.stringify(data));
                this.wallpaperData = data
            }
        })
    }

    private freeScreenLockWallpaper() {
        Log.showInfo(TAG, 'free ScreenLockWallpaper');
        if (typeof this.wallpaperData === 'undefined' || this.wallpaperData == null) {
            return;
        }
        this.wallpaperData.release().then(() => {
            Log.showDebug(TAG, 'release succeed');
        }).catch((err) => {
            Log.showDebug(TAG, `release failed ${err}`);
        })
    }
}
