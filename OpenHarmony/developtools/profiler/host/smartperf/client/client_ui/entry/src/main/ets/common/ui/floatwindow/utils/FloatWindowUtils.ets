/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
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
import wm from '@ohos.window';

export let floatingWindowOffsetX: number = 50;
export let floatingWindowOffsetY: number = 200;
export let titleWindowOffsetX: number = 300;
export let titleWindowOffsetY: number = 200;
export let lineChartWindowOffsetX: number = 30;
export let lineChartWindowOffsetY: number = 20;
export let windowWidth: number = 2560;
export let windowHeight: number = 1600;

export function initFloatWindow() {
  console.log('cm-floatWindowUtils-initFloatWindow1');
  createFloatWindow('sp_currentNow');
  createFloatWindow('sp_shellBackTemp');
  createFloatWindow('sp_ddrFrequency');
  createFloatWindow('sp_gpuFrequency');
  createFloatWindow('sp_cpu0Frequency');
  createFloatWindow('sp_cpu1Frequency');
  createFloatWindow('sp_cpu2Frequency');
  createFloatWindow('sp_RAM');
  createFloatWindow('sp_FPS');
  console.log('cm-floatWindowUtils-initFloatWindow2');
}
export function createFloatWindow(floatName: string) {
  //5.5SP2  2106 改成 8
  console.log('cm-floatWindowUtils-createFloatWindow1');
  wm.create(globalThis.abilityContext, floatName, wm.WindowType.TYPE_FLOAT).then((floatWin) => {
    console.log('cm-floatWindowUtils-createFloatWindow2');
    floatWin.moveTo(lineChartWindowOffsetX, lineChartWindowOffsetY).then(() => {
      floatWin.resetSize(360 * globalThis.coefficient, 270 * globalThis.coefficient).then(() => {
        floatWin.getProperties().then((property) => {
          property.isTransparent = false;
        });
        console.log('cm-floatWindowUtils-createFloatWindow3');
        if (floatName == 'sp_currentNow') {
          floatWin.loadContent('pages/CurrentNowLineChartPage').then(() => {
            floatWin.setBackgroundColor('#B3000000').then(() => {
              //透明
              console.log('xhq win setTransparent end.');
              floatWin.setBackgroundColor('#59020000').then(() => {
                //背景颜色
              });
            });
            floatWin.show().then(() => {
              floatWin.hide();
            });
          });
        } else if (floatName == 'sp_shellBackTemp') {
          floatWin.loadContent('pages/ShellBackTempLineChartPage').then(() => {
            floatWin.setBackgroundColor('#B3000000').then(() => {
              //透明
              console.log('xhq win setTransparent end.');
              floatWin.setBackgroundColor('#59020000').then(() => {
                //背景颜色
              });
            });
            floatWin.show().then(() => {
              floatWin.hide();
            });
          });
        } else if (floatName == 'sp_ddrFrequency') {
          floatWin.loadContent('pages/DDRLineChartPage').then(() => {
            floatWin.setBackgroundColor('#B3000000').then(() => {
              //透明
              console.log('xhq win setTransparent end.');
              floatWin.setBackgroundColor('#59020000').then(() => {
                //背景颜色
              });
            });
            floatWin.show().then(() => {
              floatWin.hide();
            });
          });
        } else if (floatName == 'sp_gpuFrequency') {
          console.log('cm-floatWindowUtils-sp_gpuFrequency1');
          floatWin.loadContent('pages/GPULineChartPage').then(() => {
            console.log('cm-floatWindowUtils-sp_gpuFrequency2');
            floatWin.setBackgroundColor('#B3000000').then(() => {
              //透明
              console.log('xhq win setTransparent end.');
              floatWin.setBackgroundColor('#59020000').then(() => {
                //背景颜色
              });
            });
            floatWin.show().then(() => {
              console.log('cm-floatWindowUtils-sp_gpuFrequency-hide1');
              floatWin.hide();
              console.log('cm-floatWindowUtils-sp_gpuFrequency-hide2');
            });
          });
        } else if (floatName == 'sp_cpu0Frequency') {
          floatWin.loadContent('pages/CPU0LineChartPage').then(() => {
            floatWin.setBackgroundColor('#B3000000').then(() => {
              //透明
              console.log('xhq win setTransparent end.');
              floatWin.setBackgroundColor('#59020000').then(() => {
                //背景颜色
              });
            });
            floatWin.show().then(() => {
              floatWin.hide();
            });
          });
        } else if (floatName == 'sp_cpu1Frequency') {
          floatWin.loadContent('pages/CPU1LineChartPage').then(() => {
            floatWin.setBackgroundColor('#B3000000').then(() => {
              //透明
              console.log('xhq win setTransparent end.');
              floatWin.setBackgroundColor('#59020000').then(() => {
                //背景颜色
              });
            });
            floatWin.show().then(() => {
              floatWin.hide();
            });
          });
        } else if (floatName == 'sp_cpu2Frequency') {
          floatWin.loadContent('pages/CPU2LineChartPage').then(() => {
            floatWin.setBackgroundColor('#B3000000').then(() => {
              //透明
              console.log('xhq win setTransparent end.');
              floatWin.setBackgroundColor('#59020000').then(() => {
                //背景颜色
              });
            });
            floatWin.show().then(() => {
              floatWin.hide();
            });
          });
        } else if (floatName == 'sp_RAM') {
          floatWin.loadContent('pages/RAMLineChartPage').then(() => {
            floatWin.setBackgroundColor('#B3000000').then(() => {
              //透明
              console.log('xhq win setTransparent end.');
              floatWin.setBackgroundColor('#59020000').then(() => {
                //背景颜色
              });
            });
            floatWin.show().then(() => {
              floatWin.hide();
            });
          });
        } else if (floatName == 'sp_FPS') {
          floatWin.loadContent('pages/FpsLineChartPage').then(() => {
            floatWin.setBackgroundColor('#B3000000').then(() => {
              //透明
              console.log('xhq win setTransparent end.');
              floatWin.setBackgroundColor('#59020000').then(() => {
                //背景颜色
              });
            });
            floatWin.show().then(() => {
              floatWin.hide();
            });
          });
        }
      });
    });
  });
}
export function destoryAllFloatWindow() {
  console.log('cm-floatWindowUtils-destoryAllFloatWindow');
  destroyFloatWindow('sp_currentNow');
  destroyFloatWindow('sp_shellBackTemp');
  destroyFloatWindow('sp_ddrFrequency');
  destroyFloatWindow('sp_gpuFrequency');
  destroyFloatWindow('sp_cpu0Frequency');
  destroyFloatWindow('sp_cpu1Frequency');
  destroyFloatWindow('sp_cpu2Frequency');
  destroyFloatWindow('sp_RAM');
  destroyFloatWindow('sp_FPS');
}
export function destroyFloatWindow(floatName: string) {
  console.log('cm-floatWindowUtils-destoryAllFloatWindow1');
  wm.find(floatName).then((fltWin) => {
    fltWin.destroy().then(() => {
      globalThis.showFPSLineChartWindow = false;
    });
  });
}
export function hideFloatWindow(floatName: string) {
  wm.find(floatName).then((fltWin) => {
    fltWin.hide();
  });
}
export function showFloatWindow(floatName: string) {
  wm.find(floatName).then((fltWin) => {
    fltWin.show();
  });
}
export function moveFloatWindow(floatName: string, offsetX: number, offsetY: number) {
  var xx =
      floatingWindowOffsetX + offsetX * 2 < 0
      ? 0
      : floatingWindowOffsetX + offsetX * 2 > windowWidth - 200
          ? windowWidth - 200
          : floatingWindowOffsetX + offsetX * 2;
  var yy =
      floatingWindowOffsetY + offsetY * 2 < 0
      ? 0
      : floatingWindowOffsetY + offsetY * 2 > windowHeight - 200
          ? windowHeight - 200
          : floatingWindowOffsetY + offsetY * 2;

  wm.find(floatName).then((fltWin) => {
    fltWin.moveTo(xx, yy);
  });
}
export function setFloatWindow(offsetX: number, offsetY: number) {
  floatingWindowOffsetX =
    floatingWindowOffsetX + offsetX * 2 < 0
    ? 0
    : floatingWindowOffsetX + offsetX * 2 > windowWidth - 200
        ? windowWidth - 200
        : floatingWindowOffsetX + offsetX * 2;
  floatingWindowOffsetY =
    floatingWindowOffsetY + offsetY * 2 < 0
    ? 0
    : floatingWindowOffsetY + offsetY * 2 > windowHeight - 200
        ? windowHeight - 200
        : floatingWindowOffsetY + offsetY * 2;
}
