# device_rockchip

## 公司介绍

瑞芯微电子股份有限公司（“瑞芯微”，股票代码：603893）成立于 2001 年，总部位于福州，在深圳、上海、北京、杭州、香港设有分/子公司，专注于集成电路设计与研发，目前已发展为领先的物联网（IoT）及人工智能物联网（AIoT）处理器芯片企业。瑞芯微拥有一支以系统级芯片、模拟电路芯片设计和算法研究为特长的研发团队，在处理器和数模混合芯片设计、多媒体处理、影像算法、人工智能、系统软件开发上具有丰富的经验和技术储备。瑞芯微主要产品除各类型处理器芯片外，还包括电源管理芯片、数模混合芯片、光电产品及开发板产品。

## 目录简介

```
device/soc/rockchip
├── common				# 公共目录
├── rk2206             	# RK2206芯片
├── rk3399             	# RK3399芯片
├── rk3568			   	# RK3568芯片	
├── rk3588              # RK3588芯片
├── ......
```

## 芯片简介

### RK2206芯片

#### 芯片简介

RK2206是一款低功耗、高集成度、基于单片机的无线局域网处理器，可应用于诸如物联网、可穿戴设备、智慧家居、云连接等应用领域。RK2206拥有如下特性：

| 硬件特性     | 技术能力                                              |
| ------------ | ----------------------------------------------------- |
| 处理器       | RK2206                                                |
| 架构         | ARM                                                   |
| CPU          | Cortex-M4F，主频高达200MHz                            |
| DSP          | Tensilica HiFi3 DSP，主频高达300MHz                   |
| WLAN         | 支持IEEE 802.11b/g/n制式，支持AP模式                  |
| 内存以及存储 | RAM 256KB、DTCM 192KB 、ROM16KB、PSRAM 8MB、Flash 8MB |
| 外部存储接口 | 支持SD3.0与MMC ver4.51                                |
| USB接口      | 1个USB OTG 2.0，1个USB转串口用于调试                  |
| GPIO         | 32个复用GPIO引脚                                      |
| SPI          | 2组                                                   |
| I2C          | 3组                                                   |
| I2S          | 2组                                                   |
| UART         | 3组                                                   |
| PWM          | 12组                                                  |
| ADC          | 8组                                                   |
| 主板供电     | USB 5V供电                                            |

#### 相关仓

* #### 小凌派-RK2206开发板

  小凌派-RK2206开发板主控器为瑞芯微高性能、高性价比的RK2206芯片，搭载OpenHarmony操作系统，内置WiFi/AP功能、NFC功能、液晶显示接口以及E53接口，E53接口兼容各类传感器模块，便于多样化的IoT物联网应用；目前小凌派-RK2006开发板已经拥有20+个成熟的应用案例，以及完善的教学课程，可广泛的应用于智慧城市、智能家居、智慧教学、智慧车载以及智慧医疗等多种场景。

  参考[小凌派-RK2206开发板简介](https://gitee.com/openharmony-sig/vendor-lockzhiner/tree/master/lingpi)

  * [device/board/lockzhiner](https://gitee.com/openharmony-sig/device_board_lockzhiner)
  * [vendor/lockzhiner](https://gitee.com/openharmony-sig/vendor-lockzhiner)

### rk3399芯片



