# 公共产品形态配置<a name="ZH-CN_TOPIC_0000001079317008"></a>

-   [简介](#section11660541593)
-   [配置仓目录结构](#section113275517516)
-   [配置示例](#section178021418115315)
-   [常见问题说明](#section174312714582)
-   [相关仓](#section1371113476307)

## 简介<a name="section11660541593"></a>

一个完整的产品包括芯片组件部分和系统组件部分。芯片组件部分在vendor/{company}/{product}/目录下定义。本仓主要定义与芯片无关的通用系统组件形态配置。

一个完整的系统组件形态主要包括两个部分：

- 部件列表：系统组件支持的部件集合都在productdefine/common/products目录下通过系统组件形态配置文件xxx.json列举，该文件还可以配置部件的feature值。
- 部件部署的指令集：指令集定义都在productdefine/common/device目录下通过arm64.json类文件来定义，products下的形态配置文件可以通过target_cpu来指定。

## 配置仓目录结构<a name="section113275517516"></a>

```sh
productdefine/common
├── base                          # 最小系统部件集合
│   ├── mini_system.json          # 轻量系统最小部件集合
│   ├── small_system.json         # 小型系统最小部件集合
│   └── standard_system.json      # 标准系统最小部件集合
├── inherit                       # 可继承的部件模版
│   ├── chipset_common.json       # 芯片组件依赖最小部件集合
│   ├── headless.json             # 无UI系统最小部件集合
│   ├── ipcamera.json             # IPCamera部件集合, 与base/standard_system.json配合使用
│   ├── pc.json                   # PC必选部件集合，相比平板增加了打印框架和数据泄露保护部件
│   ├── default.json              # 通话类设备必选部件集合，相比rich.json全量配置裁剪了NFC、打印框架、数据泄露保护、msdp、人脸和指纹认证部件
│   ├── rich.json                 # 标准系统全量部件集合
│   ├── tablet.json               # 平板必选部件集合，相比手机裁剪了电话和位置子系统的所有部件
│   └── watch.json                # 运动表必选部件集合
└── products                      # 系统组件形态配置文件，配置文件名称与product name保持一致
    ├── ohos-sdk.json             # SDK部件集合
    ├── system-arm-default.json   # 32位系统组件部件集合
    └── system-arm64-default.json # 64位系统组件部件集合
```

inherit中的模版含义说明：

| 名称                | 含义                                                         | 使用产品                                          |      |
| ------------------- | ------------------------------------------------------------ | ------------------------------------------------- | ---- |
| chipset_common.json | 标准系统芯片组件部件集合。                                   | 各标准系统开发板芯片组件都使用此模版。            |      |
| headless.json       | 标准系统无头系统部件集合，支持无界面的FA安装及流转。         | 当前qemu-arm-linux-headless虚拟机平台使用此模版。 |      |
| ipcamera.json       | IPCamera部件集合，IP摄像头解决方案。                        | 当前hispark_taurus等开发板使用此模版。      |      |
| pc.json             | PC部件集合，个人PC解决方案。                                | 适用于使用标准系统的产品。      |      |
| default.json        | 通话类设备部件集合，手机解决方案。                           | 适用于使用标准系统的产品。      |      |
| rich.json           | 标准系统全量部件集合，具备全量功能。                         | 当前rk3568, hispark_phoenix等开发板使用此模版。   |      |
| tablet.json         | 平板部件集合，平板解决方案。                                | 适用于使用标准系统的产品。      |      |
| watch.json          | 运动表部件集合，运动表解决方案。                            | 适用于使用轻量系统的产品。   |      |


## 配置示例<a name="section178021418115315"></a>

**产品配置示例：** system-arm64-default.json

```json
{
  "version": "3.0",                       # 配置文件格式版本
  "product_name": "system-arm64-default", # 系统组件形态名称
  "device_company": "ohos",               # 系统组件形态厂商
  "target_cpu": "arm64",                  # 系统组件形态支持的指令集，读取device/下对应的指令集配置
  "board": "arm64",                       # 与target_cpu相同，读取device/下对应的指令集配置
  "type": "standard",                     # 系统类型，读取base下对应的系统配置
  "inherit": [ "" ],                      # 继承的部件列表配置, 可以继承通用的部件列表
  "subsystems":[                          # 产品的部件列表
    {
      "subsystem": "arkui",
      "components": [{
          "component": "napi",
          "features": [
            "napi_xxxx=xxx",
            ...
          ]
        }
      ]
    },
    ...
  ]
}
```

配置文件中各字段的含义如下：

| 字段           | 含义                                                         |
| -------------- | ------------------------------------------------------------ |
| version        | 配置文件版本号，当前都填"3.0"。                              |
| product_name   | 系统组件形态名称，可以通过此字段值作为build.sh --product-name的参数来完成系统组件的编译。<br/>系统组件的product_name命名采用system-{target_cpu}-{devicetype}格式，devicetype对应系统组件的形态名称，如tablet，pc等，默认为default。 |
| device_company | 系统组件形态厂商，默认均填"ohos"。                           |
| target_cpu     | 系统组件形态支持的指令集，会关联读取device/下对应的指令集配置。 |
| board          | 与target_cpu相同，读取device/下对应的指令集配置。            |
| type           | 轻量系统("mini")，小型系统("small")还是标准系统("standard")。 |
| inherit        | 继承的部件列表配置，可继承的部件配置列表都存在productdefine/common/inherit目录下。 |
| subsystems     | 支持的子系统部件列表。每个子系统由多个components组成；每个component包括component部件名称以及部件的features特性配置列表。<br/>部件的feature配置都是如下的名字对字符串：<br/>"{component_name}_xxx=xxx" |



**device配置示例：** arm64.json

需要提供一个形态配置中target_cpu同名的配置文件，配置device信息和board信息。

```json
{           
  "device_name": "arm64",          # device名
  "device_company": "openharmony", # device厂商名
  "target_os": "ohos",             # 在设备上部署的操作系统名
  "target_cpu": "arm64",           # 设备的cpu类型
  "kernel_version": "",
  "device_build_path": ""          # device对应的子系统路径，系统组件形态不用填写此字段。
}
```

device_build_path是device对应子系统路径，编译时会扫描对应目录下的部件配置。 

**系统组件部件类表的生成：**

1.  先加载inherit中的部件列表；
2.  加载subsystems中的部件列表；
5.  部件配置安装上述顺序加载，后面加载的配置可覆盖前面已加载的同名配置。

## 部件组合的继承关系<a name="section174312714582"></a>

在inherit目录下定义部件模版，系统组件形态可以通过inherit继承该模版中包含的所有部件，减少形态过多时，新增部件的配置工作。

由于每个部件包括部件名称以及feature信息，系统组件形态配置文件继承模版时会把这两个信息都继承下去。编译框架支持了对部件模版的**全量继承**以及**部分继承**能力。下面以sample.json模版为例介绍继承关系：

```json
[
  {
    "component": "A",
    "features": [ "feature_A=true" ]
  }, {
    "component": "B",
    "features": [ "feature_B=true" ]
  }
]
```

- 全量继承：

  系统组件形态配置文件直接inherit上述模版，则可完全继承模版中的部件以及部件的feature信息。

  ```json
  {
    ...
    "inherit": [ "inherit/sample.json" ],
    "subsystems": []
  }
  ```

- 部分继承：feature重载

  ```json
  {
    ...
    "inherit": [ "inherit/sample.json" ],
    "subsystems": [{
      ...
      "components": [{
          "component": "A",
          "features": [ "feature_A=false", "feature_A_pluse=false" ]
        }]
    }]
  }
  ```

  如上示例，系统组件形态配置文件中的重新定义了部件A的feature，此时会覆盖sample.json模版中部件A的配置。

## 常见问题说明<a name="section174312714582"></a>

**如何给系统组件新加一个部件？**

根据部件的归属的形态，修改productdefine/common/products/目录下的系统组件形态部件列表。如果是通用的部件，可以把部件添加到productdefine/common/inherits/目录下的模版中，这样所有继承该模版的系统组件形态都会生效。

新的部件配置格式示例如下：

```json
 "subsystems": [{
    ...
    "components": [{
        "component": "name",
        "features": [ "feature_A=false", "feature_A_pluse=false" ]
      }]
  }
```

**如何查看一个指定产品的最终的部件列表？**

在编译启动后，编译系统会解析配置文件，生成产品的全部的部件列表，输出为：out/preloader/\{产品名\}/parts.json。

## 相关仓<a name="section1371113476307"></a>

productdefine/common

