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

// 反转摄像头_多机位_拍照_摄像

import DateTimeUtil from '../model/DateTimeUtil'
import Logger from '../model/Logger'
import cameraDemo from 'libentry.so'

import mediaLibrary from '@ohos.multimedia.mediaLibrary';

import image from '@ohos.multimedia.image';
import media from '@ohos.multimedia.media'
import MediaUtils from '../model/MediaUtils'
import deviceInfo from '@ohos.deviceInfo';
import AVRecorder from '@ohos.multimedia.media'
import fileio from '@ohos.fileio'

const CameraSize = {
  WIDTH: 1920,
  HEIGHT: 1080
}

globalThis.photoSettings = {
  quality: 0,          // 拍照质量
  rotation: 0,         // 照片方向
  mirror: false,       // 镜像使能
  latitude: 12.9698,   // 地理位置
  longitude: 77.7500,  // 地理位置
  altitude: 1000       // 地理位置
};

@Component
export struct modeSwitchPage {
  private tag: string = 'CAIHF modeSwitchPage:'
  private mediaUtil = MediaUtils.getInstance()
  private fileAsset: mediaLibrary.FileAsset = undefined
  private fd: number = -1
  @State videoId: string = ''
  @State mSurfaceId: string = ''
  private mReceiver: image.ImageReceiver = undefined
  private videoRecorder: media.AVRecorder = undefined
  private videoConfig: media.AVRecorderConfig = {
    audioSourceType: media.AudioSourceType.AUDIO_SOURCE_TYPE_MIC,
    videoSourceType: media.VideoSourceType.VIDEO_SOURCE_TYPE_SURFACE_YUV,
    profile: {
      audioBitrate: 48000,
      audioChannels: 2,
      audioCodec: media.CodecMimeType.AUDIO_AAC,
      audioSampleRate: 48000,
      fileFormat: media.ContainerFormatType.CFT_MPEG_4,
      videoBitrate: 512000,
      videoCodec: media.CodecMimeType.VIDEO_AVC,
      videoFrameWidth: 640,
      videoFrameHeight: 480,
      videoFrameRate: 30
    },
    url: '',
    rotation: 0
  };
  private photoRotationMap = {
    rotation0: 0,
    rotation90: 90,
    rotation180: 180,
    rotation270: 270,
  };
  // 相机摄像头
  @Link cameraDeviceIndex: number
  // surfaceID值
  @Prop surfaceId: string
  // 倒计时值
  @Link countdownNum: number
  // 倒计时定时器
  @State countTimerInt: number = -1
  @State countTimerOut: number = -1
  // 拍照缩略图
  @State imgThumbnail: string = undefined
  // 人像缩略图
  @State portraitThumbnail: string = undefined
  // 录制时间
  @State videoRecodeTime: number = 0
  // 录制时间定时器
  @State timer: number = undefined
  // 时间管理器
  @State dateTimeUtil: DateTimeUtil = new DateTimeUtil()
  // 选择模式
  @State modelBagCol: string = 'photo'
  // 选择相机或者拍照
  @State @Watch('onChangeIsModeBol') isModeBol: boolean = true
  // 视频缩略图
  @State videoThumbnail: image.PixelMap = undefined
  // // 暂停后点击停止 需要将暂停回复默认
  onChangeIsModeBol() {
  }
   // 倒计时拍照和录像
   countTakeVideoFn() {
     if (this.countdownNum) {
       // 清除定时器
       if (this.countTimerOut) {
         clearTimeout(this.countTimerOut)
       }
       if (this.countTimerInt) {
         clearInterval(this.countTimerInt)
       }
       // 开启定时器
       this.countTimerOut = setTimeout(() => {
         // 判断是录像还是拍照模式
         this.isVideoPhotoFn()
       }, this.countdownNum * 1000)
       // 开启计时器
       this.countTimerInt = setInterval(() => {
         this.countdownNum--
         if (this.countdownNum === 0) {
           clearInterval(this.countTimerInt)
         }
       }, 1000)
     } else {
       this.isVideoPhotoFn()
     }
   }
  async getVideoSurfaceID(){
    Logger.info(this.tag, `getVideoSurfaceID`)
    this.videoRecorder = await media.createAVRecorder()
    Logger.info(this.tag, `getVideoSurfaceID videoRecorder: ${this.videoRecorder}`)

    this.fileAsset = await this.mediaUtil.createAndGetUri(mediaLibrary.MediaType.VIDEO)
    Logger.info(this.tag, `getVideoSurfaceID fileAsset: ${this.fileAsset}`)

    this.fd = await this.mediaUtil.getFdPath(this.fileAsset)
    Logger.info(this.tag, `getVideoSurfaceID fd: ${this.fd}`)

    this.videoConfig.url = `fd://${this.fd}`
    Logger.info(this.tag, `getVideoSurfaceID videoConfig.url : ${this.videoConfig.url }`)

    if (deviceInfo.deviceType == 'default') {
      Logger.info(this.tag, `deviceType = default`)
      this.videoConfig.videoSourceType = media.VideoSourceType.VIDEO_SOURCE_TYPE_SURFACE_ES
    }
    if (deviceInfo.deviceType == 'phone') {
      Logger.info(this.tag, `deviceType = phone`)
      this.videoConfig.videoSourceType = media.VideoSourceType.VIDEO_SOURCE_TYPE_SURFACE_YUV
      this.videoConfig.profile.videoCodec = media.CodecMimeType.VIDEO_MPEG4;
      if (this.cameraDeviceIndex == 1) {
        this.videoConfig.rotation = this.photoRotationMap.rotation270;
      } else {
        this.videoConfig.rotation = this.photoRotationMap.rotation90;
      }
    }
    if (deviceInfo.deviceType == 'tablet') {
      Logger.info(this.tag, `deviceType = tablet`)
      this.videoConfig.videoSourceType = media.VideoSourceType.VIDEO_SOURCE_TYPE_SURFACE_YUV
    }

    this.videoConfig.profile.videoFrameWidth = cameraDemo.getVideoFrameWidth();
    this.videoConfig.profile.videoFrameHeight = cameraDemo.getVideoFrameHeight();
    this.videoConfig.profile.videoFrameRate = cameraDemo.getVideoFrameRate();

    Logger.info(this.tag, `getVideoSurfaceID 111`)
    await this.videoRecorder.prepare(this.videoConfig)
    Logger.info(this.tag, `getVideoSurfaceID 222`)
    this.videoId = await this.videoRecorder.getInputSurface()
    Logger.info(this.tag, `getVideoSurfaceID videoId: ${this.videoId}`)
  }

  createImageReceiver() {
    try {
      // 多少数据 不能直接把buff ， 写进去
      // buffer大小固定死 直接写进去就已经确定了
      // 需要拿到dataSize 再进行写入
      this.mReceiver = image.createImageReceiver(CameraSize.WIDTH, CameraSize.HEIGHT, 2000, 8)
      Logger.info(this.tag, `createImageReceiver value: ${this.mReceiver} `)
      this.mReceiver.on('imageArrival', () => {
        Logger.info(this.tag, 'imageArrival start')
        this.mReceiver.readNextImage((err, image) => {
          Logger.info(this.tag, 'readNextImage start')
          if (err || image === undefined) {
            Logger.error(this.tag, 'readNextImage failed ')
            return
          }
          image.getComponent(4, (errMsg, img) => {
            Logger.info(this.tag, 'getComponent start')
            if (errMsg || img === undefined) {
              Logger.info(this.tag, 'getComponent failed ')
              return
            }
            // let buffer = new ArrayBuffer(2048)
            let buffer
            if (img.byteBuffer) {
              buffer = img.byteBuffer
            } else {
              Logger.error(this.tag, 'img.byteBuffer is undefined')
            }
            this.savePicture(buffer, image)
          })
        })
      })
    } catch {
      Logger.info(this.tag, 'savePicture err')
    }
  }

  // 读取图像
  async savePicture(buffer: ArrayBuffer, img: image.Image) {
    try {
      Logger.info(this.tag, 'savePicture start')
      let imgFileAsset = await this.mediaUtil.createAndGetUri(mediaLibrary.MediaType.IMAGE)
      let imgPhotoUri = imgFileAsset.uri
      Logger.info(this.tag, `photoUri = ${imgPhotoUri}`)
      let imgFd = await this.mediaUtil.getFdPath(imgFileAsset)
      Logger.info(this.tag, `fd = ${imgFd}`)
      await fileio.write(imgFd, buffer)
      await imgFileAsset.close(imgFd)
      await img.release()
      Logger.info(this.tag, 'save image End')
      if (this.handleTakePicture) {
        this.handleTakePicture(imgPhotoUri)
      }
    } catch (err) {
      Logger.info(this.tag, 'savePicture err' + JSON.stringify(err.message))
    }
  }

  async getPhotoSurfaceID() {
    if(this.mReceiver) {
      Logger.info(this.tag, 'imageReceiver has been created')
    } else {
      this.createImageReceiver()
    }
    this.mSurfaceId = await this.mReceiver.getReceivingSurfaceId()
    if(this.mSurfaceId) {
      Logger.info(this.tag, `createImageReceiver mSurfaceId: ${this.mSurfaceId} `)
    } else {
      Logger.info(this.tag, `Get mSurfaceId failed `)
    }
  }

   // 判断录像或者照片模式
   async  isVideoPhotoFn() {
     await this.getPhotoSurfaceID()

     if (this.modelBagCol == 'photo') {
       cameraDemo.startPhotoOrVideo(this.modelBagCol, this.videoId, this.mSurfaceId)
     } else if (this.modelBagCol == 'video') {
       this.isModeBol = false
       if (this.timer) {
         clearInterval(this.timer)
       }
       // 开始录制
       await this.getVideoSurfaceID()
       cameraDemo.startPhotoOrVideo(this.modelBagCol, this.videoId, this.mSurfaceId)
       cameraDemo.videoOutputStart()
       this.videoRecorder.start()
     }
   }

  aboutToAppear() {
  }

  handleTakePicture = (thumbnail: string) => {
    this.imgThumbnail = thumbnail
    Logger.info(this.tag, `takePicture end , thumbnail: ${this.imgThumbnail}`)
  }

  build() {
    if (this.isModeBol) {

      Column() {
        Text('拍照')
          .backgroundColor(this.modelBagCol === 'photo' ? $r('app.color.theme_color') : '')
          .size({ width: 64, height: 28 })
          .borderRadius(14)
          .fontSize(14)
          .fontColor(Color.White)
          .onClick(() => {
            // 重置相机
            cameraDemo.releaseSession()
            cameraDemo.initCamera(this.surfaceId, globalThis.settingDataObj.focusMode, this.cameraDeviceIndex)
            this.modelBagCol = 'photo'
          })
      }.position({ x: '20%', y: '75%' })

      Column() {
        Text('录像')
          .fontSize(14)
          .fontColor(Color.White)
          .borderRadius(14)
          .size({ width: 64, height: 28 })
          .backgroundColor(this.modelBagCol === 'video' ? $r('app.color.theme_color') : '')
          .onClick(() => {
            // 重置相机
            cameraDemo.releaseSession()
            cameraDemo.initCamera(this.surfaceId, globalThis.settingDataObj.focusMode, this.cameraDeviceIndex)
            this.modelBagCol = 'video'
          })
      }.position({ x: '40%', y: '75%' })

      // 图库
      Column() {
        Row() {
          if (this.modelBagCol === 'photo') {
            Image(this.imgThumbnail || $r('app.media.pic_avatar_radio02'))
              .aspectRatio(1)
              .objectFit(ImageFit.Fill)
              .border({ width: 2, color: 0xFFFFFF, radius: 40 })
              .width('200px')
              .height('200px')
          } else if (this.modelBagCol === 'portrait') {
            Image(this.portraitThumbnail || $r('app.media.pic_avatar_radio02'))
              .aspectRatio(1)
              .objectFit(ImageFit.Fill)
              .border({ width: 2, color: 0xFFFFFF, radius: 40 })
              .width('200px')
              .height('200px')
          } else {
            Image(this.videoThumbnail || $r('app.media.pic_avatar_radio02'))
              .aspectRatio(1)
              .objectFit(ImageFit.Fill)
              .border({ width: 2, color: 0xFFFFFF, radius: 40 })
              .width('200px')
              .height('200px')
          }
        }.onClick(() => {
          if (deviceInfo.deviceType == 'default') {
              globalThis.abilityContext.startAbility({
              bundleName: 'com.ohos.photos',
              abilityName: 'com.ohos.photos.MainAbility'
            })
          } else if (deviceInfo.deviceType == 'phone') {
              globalThis.abilityContext.startAbility({
              bundleName: 'com.huawei.hmos.photos',
              abilityName: 'com.huawei.hmos.photos.MainAbility'
            })
          }

        })
      }.position({ x: '10%', y: '82%' })

      // 拍照录像按键
      Column() {
        Row() {
          Button() {
            Text()
              .width('120px')
              .height('120px')
              .borderRadius('40px')
              .backgroundColor(this.modelBagCol == 'video' ? $r('app.color.theme_color') : Color.White)
          }
          .border({ width: 3, color: 0xFFFFFF, radius: 70 })
          .width('200px')
          .height('200px')
          .backgroundColor('rgba(255,255,255,0.20)')
          .onClick(() => {
            // 倒计时拍照录像—默认拍照录像
            this.countTakeVideoFn()
          })
        }
      }.position({ x: '40%', y: '82%' })

      // 前后置摄像头切换
      Column() {
          Row() {
            Button() {
              Image($r('app.media.switch_camera'))
                .width('120px').height('120px')
            }
            .width('200px')
            .height('200px')
            .backgroundColor('rgba(255,255,255,0.20)')
            .borderRadius('40px')
            .onClick(async () => {
              // 切换摄像头
              this.cameraDeviceIndex ? this.cameraDeviceIndex = 0 : this.cameraDeviceIndex = 1
              // 清除配置
              cameraDemo.releaseSession()
              // 开始预览
              cameraDemo.initCamera(this.surfaceId, globalThis.settingDataObj.focusMode, this.cameraDeviceIndex)
            })
          }
      }.position({ x: '70%', y: '82%' })
    } else {
       Column() {
         Row() {
           Text().size({ width: 12, height: 12 }).backgroundColor($r('app.color.theme_color')).borderRadius(6)
           Text(this.dateTimeUtil.getVideoTime(this.videoRecodeTime))
             .fontSize(30)
             .fontColor(Color.White)
             .margin({ left: 8 })
         }.offset({ x: -580, y: -180 })
       }.position({ x: 120, y: 450 })

       Column() {
         // 录像抓拍键
         Button() {
           Text().width('120px').height('120px').borderRadius('35px').backgroundColor(Color.White)
         }
         .border({ width: 2, color: 0xFFFFFF, radius: 45 })
         .width('200px')
         .height('200px')
         .backgroundColor('rgba(255,255,255,0.20)')
         .onClick(() => {
           cameraDemo.takePictureWithSettings(globalThis.photoSettings);
         })
       }.position({ x: '10%', y: '82%' })

       Column() {
         Row() {
           Column() {
             // 录像停止键
             Button() {
               Image($r('app.media.ic_camera_video_close')).size({ width: 25, height: 25 })
             }
             .width('120px')
             .height('120px')
             .backgroundColor($r('app.color.theme_color'))
             .onClick(() => {
               if (this.timer) {
                 clearInterval(this.timer)
               }
               // 停止录制
                 this.stopVideo().then(async (fileAsset) => {
                 this.videoRecodeTime = 0
                 this.isModeBol = true
                 try {
                   // 获取录制缩略图
                   this.videoThumbnail = await fileAsset.getThumbnail()
                   // 重置相机
                   cameraDemo.releaseSession()
                   cameraDemo.initCamera(this.surfaceId, globalThis.settingDataObj.focusMode, this.cameraDeviceIndex)
                 } catch (err) {
                   Logger.info(this.tag, 'videoThumbnail err----------:' + JSON.stringify(err.message))
                 }
               })
             })
           }
           .width('180px')
           .height('180px')
           .borderRadius('60px')
           .backgroundColor($r('app.color.theme_color'))
           .justifyContent(FlexAlign.SpaceAround)
         }
         .justifyContent(FlexAlign.Center)
         .border({ width: 3, color: 0xFFFFFF, radius: 70 })
         .width('200px')
         .height('200px')
         .backgroundColor('rgba(255,255,255,0.20)')
       }.position({ x: '40%', y: '82%' })

    }
  }

  async stopVideo() {
    try {
      if (this.videoRecorder) {
        await this.videoRecorder.stop()
        await this.videoRecorder.release()
      }
      cameraDemo.videoOutputStopAndRelease();
      if (this.fileAsset) {
        await this.fileAsset.close(this.fd)
        return this.fileAsset
      }
      Logger.info(this.tag, 'stopVideo end')
    } catch (err) {
      Logger.info(this.tag, 'stopVideo err: ' + JSON.stringify(err))
    }
  }
}
