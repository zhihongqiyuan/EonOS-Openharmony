# vendor_hihope

## 介绍

该仓库托管HiHope产品：海王星（Neptune）系列和大禹（DAYU）系列OpenHarmony智能硬件。

## 目录

```
vendor/hihope
├── neptune_iotlink_demo                  # Neptune100开发板
├── rk3568                                # DAYU200开发板
└── dayu210                               # DAYU210开发板
```

## 新建产品工程

这里以海王星（Neptune）开发板为例，用户可以将 "neptune_iotlink_demo"样例复制一份，然后进行裁剪或者修改，实现自己的产品工程，下面举例说明如何新建一个工程。

#### 新建产品工程

1、复制一份 vendor/hihope目录下的"neptune_iotlink_demo"放在同级目录下，并改名为自己产品工程的名称(例如：xxx_iotlink_demo)；

2、进入 xxx_iotlink_demo目录, 编辑config.json文件，修改product_name和product_adapter_dir：

```
"product_name": "xxx_iotlink_demo"
"product_adapter_dir": "//vendor/hihope/xxx_iotlink_demo/hals"
```

3、在config.json中可删除xts、kv_store、文件管理子系统，保留kernel、startup、hiviewdfx、distributedschedule等必要的子系统，可移除如下代码段：
```
"bin_list": [
      {
        "elf_name": "hihope",
        "enable": "true",
        "force_link_libs": [
          "bootstrap",
          "broadcast",
          "hctest",
          "module_ActsParameterTest",
          "module_ActsBootstrapTest",
          "module_ActsDfxFuncTest",
          "module_ActsHieventLiteTest",
          "module_ActsSamgrTest",
          "module_ActsUtilsFileTest",
          "module_ActsKvStoreTest"
        ]
      }
    ],

...

{
  "subsystem": "utils",
  "components": [
    {
      "component": "kv_store",
      "features": [
        "enable_ohos_utils_native_lite_kv_store_use_posix_kv_api = true"
        ]
    },
    { "component": "file", "features":[] }
  ]
},
{
  "subsystem": "xts",
  "components": [
    {
      "component": "xts_acts",
      "features":
        [
          "config_ohos_xts_acts_utils_lite_kv_store_data_path = \"/data\"",
          "enable_ohos_test_xts_acts_use_thirdparty_lwip = true"
        ]
    },
    { "component": "xts_tools", "features":[] }
  ]
}
```

4、编辑 "xxx_iotlink_demo/BUILD.gn"，group名：
```
group("xxx_iotlink_demo") {
}
```

5、在OpenHarmony源码根目录下，执行hb set, 出现自己添加的工程名称 "xxx_iotlink_demo"：
```
hihope
   neptune_iotlink_demo
 > xxx_iotlink_demo
```

至此，一个简单的产品工程搭建完成，用户可按此方法，搭建自己产品工程。

详细的产品编译构建适配流程，请参考[编译构建适配流程](https://gitee.com/openharmony/docs/blob/master/zh-cn/device-dev/subsystems/subsys-build-product.md)

## 贡献

[如何参与](https://gitee.com/openharmony/docs/blob/HEAD/zh-cn/contribute/%E5%8F%82%E4%B8%8E%E8%B4%A1%E7%8C%AE.md)

[Commit message规范](https://gitee.com/openharmony/device_qemu/wikis/Commit%20message%E8%A7%84%E8%8C%83?sort_id=4042860)

## 相关仓

* [device/board/hihope](https://gitee.com/openharmony/device_board_hihope)
* [device/soc/winnermicro](https://gitee.com/openharmony/device_soc_winnermicro)
* [device/soc/rockchip](https://gitee.com/openharmony/device_soc_rockchip)