/*
 * Copyright (c) 2022-2023 Huawei Device Co., Ltd.
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
 
import abilityAccessCtrl from '@ohos.abilityAccessCtrl';
import cameraDemo from 'libentry.so'
import Logger from '../model/Logger'
import { mainDialog } from '../Dialog/mainDialog'
import { dividerPage } from '../views/dividerPage'
import { settingDialog } from '../Dialog/settingDialog'
import { CountdownPage } from '../views/CountdownPage'
import { FlashingLightPage } from '../views/FlashingLightPage'
import { SlidePage } from '../views/SlidePage'
import { modeSwitchPage } from '../views/modeSwitchPage'
import { focusPage } from '../views/focusPage'
import { FocusAreaPage } from '../views/focusAreaPage'
import { Constants } from '../common/Constants'
import DisplayCalculator from '../common/DisplayCalculator'
import display from '@ohos.display'

const TAG: string = 'UI indexPage'

globalThis.settingDataObj = {
  mirrorBol: false, // 镜像使能 -> 关闭
  videoStabilizationMode: 0, // 视频防抖 -> 关闭
  exposureMode: 1, // 曝光模式 -> 自动
  focusMode: 2, // 对焦模式 -> 自动
  photoQuality: 1, // 拍照质量 -> 中
  locationBol: false, // 显示地理位置 -> 关闭
  photoFormat: 1, // 照片格式 -> JPG
  photoOrientation: 0, // 照片方向 -> 0
  photoResolution: 0, // 照片分辨率 -> 1920 * 1080
  videoResolution: 0, // 照片分辨率 -> 1920 * 1080
  videoFrame: 0, // 录像帧率 -> 15
  referenceLineBol: false // 分割线 -> 关闭
}

@Entry
@Component
struct Index {
  // XComponentCon控制器
  private mXComponentController: XComponentController = new XComponentController()
  // surfaceID值
  @State surfaceId: string = ''
  // 入口确定网络弹框
  private mainDialogController: CustomDialogController = new CustomDialogController({
    builder: mainDialog(),
    autoCancel: false,
    customStyle: true
  })

  // 选择模式
  @State modelBagCol: string = 'photo'
  // 曝光区域
  @State focusPointBol: boolean = false
  // 曝光区域手指点击坐标
  @State focusPointVal: Array<number> = [0, 0]
  // 刻度、焦距值 和 对焦框不能共存的显示
  @State exposureBol: boolean = true
  // 曝光值
  @State exposureNum: number = 0
  // 倒计时拍照 录像
  @State countdownNum: number = 0
  // 相机摄像头
  @State cameraDeviceIndex: number = 0
  @State xComponentWidth: number = 384;
  @State xComponentHeight: number = 450;
  private deviceType: string;
  private screenHeight: number;
  private screenWidth: number;

  // 设置弹框
  private settingDialogController: CustomDialogController = new CustomDialogController({
    builder: settingDialog({
      referenceLineBol: $referenceLineBol,
      surfaceId: this.surfaceId,
      cameraDeviceIndex: this.cameraDeviceIndex
    }),
    autoCancel: false,
    customStyle: true
  })

  // 参考线
  @State referenceLineBol: boolean = false
  @StorageLink('defaultAspectRatio') @Watch('initXComponentSize') defaultAspectRatio: number = Constants.MIN_ASPECT_RATIO;
  @State onShow: boolean = false
  atManager = abilityAccessCtrl.createAtManager();
  // 入口初始化函数
  async aboutToAppear() {
    await this.requestPermissionsFn();
    let mDisplay = display.getDefaultDisplaySync();
    this.screenWidth = px2vp(mDisplay.width);
    this.screenHeight = px2vp(mDisplay.height);
    // @ts-ignore
    this.deviceType = AppStorage.get<string>('deviceType');
    if (this.deviceType === Constants.TABLET) {
      this.defaultAspectRatio = Constants.MAX_ASPECT_RATIO;
    }
    this.initXComponentSize();
  }

  initXComponentSize(): void {
    let defaultSize = DisplayCalculator.calcSurfaceDisplaySize(this.screenWidth, this.screenHeight, this.defaultAspectRatio);
    this.xComponentWidth = defaultSize.width;
    this.xComponentHeight = defaultSize.height;
  }

  async aboutToDisAppear() {
    await cameraDemo.releaseCamera();
  }
  // 获取权限
  async requestPermissionsFn() {
    Logger.info(TAG, `requestPermissionsFn entry`)
    try {
      this.atManager.requestPermissionsFromUser(globalThis.abilityContext, [
        'ohos.permission.CAMERA',
        'ohos.permission.MICROPHONE',
        'ohos.permission.READ_MEDIA',
        'ohos.permission.WRITE_MEDIA'
      ]).then(() => {
        Logger.info(TAG, `request Permissions success!`)
        this.onShow = true
      })
        .catch((error) => {
          Logger.info(TAG, `requestPermissionsFromUser call Failed! error: ${error.code}`)
        })
    } catch (err) {
      Logger.info(TAG, `requestPermissionsFromUser call Failed! error: ${err.code}`)
    }
  }

  async onPageShow() {
    Logger.info(TAG, `onPageShow App`)
    if (this.surfaceId && this.onShow) {
      Logger.error(TAG, `initCamera`)
      cameraDemo.initCamera(this.surfaceId, globalThis.settingDataObj.focusMode, this.cameraDeviceIndex)
      Logger.error(TAG, `initCamera 111`)
    }
  }

  onPageHide() {
    Logger.info(TAG, `onPageHide App`)
     cameraDemo.releaseCamera();
    // CameraService.releaseCamera()
  }

  build() {
    Stack() {
      if (this.onShow) {
        // 画面
        XComponent({
          id: 'componentId',
          type: 'surface',
          controller: this.mXComponentController
        })
          .onLoad(async () => {
            Logger.info(TAG, 'onLoad is called');
            this.surfaceId = this.mXComponentController.getXComponentSurfaceId()
            Logger.info(TAG, `onLoad surfaceId: ${this.surfaceId}`)
            Logger.error(TAG, `initCamera 222`)
            cameraDemo.initCamera(this.surfaceId, globalThis.settingDataObj.focusMode, this.cameraDeviceIndex);
            globalThis.cameraDeviceIndex = this.cameraDeviceIndex;
            Logger.error(TAG, `initCamera 333`)
          })
          .backgroundColor(Color.Blue)
          .width('100%')
          .height('100%')

        // 参考线
        dividerPage({ referenceLineBol: this.referenceLineBol })
        // 设置图标
        Button() {
          Image($r('app.media.icon_camera_setting'))
            .width('120px').height('120px')
        }
        .width('120px')
        .height('120px')
        .backgroundColor('rgba(255,255,255,0.20)')
        .borderRadius('40px')
        .position({ x: '80%', y: '3%' })
        .onClick(() => {
          // 打开设置弹框
          this.settingDialogController.open()
        })
        // 曝光框和对焦框
        focusPage({
          focusPointBol: $focusPointBol,
          focusPointVal: $focusPointVal,
          exposureBol: $exposureBol,
          exposureNum: $exposureNum
        })
        // 曝光对焦手指点击区域
        FocusAreaPage({
          focusPointBol: $focusPointBol,
          focusPointVal: $focusPointVal,
          exposureBol: $exposureBol,
          exposureNum: $exposureNum,
          xComponentWidth: this.xComponentWidth,
          xComponentHeight: this.xComponentHeight
        })
        // 倒计时
        CountdownPage({ countdownNum: $countdownNum })
        // 闪光灯
        FlashingLightPage()
        // slide
        SlidePage()
        // 反转摄像头_多机位_拍照_摄像
        modeSwitchPage({
          surfaceId: this.surfaceId,
          cameraDeviceIndex: $cameraDeviceIndex,
          countdownNum: $countdownNum
        })
      }
    }
    .height('100%')
    .width('100%')
  }
}