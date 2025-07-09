# 外设驱动HDI接口定义

## 简介

该仓库用于管理各模块HDI(Hardware Device Interface)接口定义，接口定义使用IDL语言描述并以`·idl`文件形式保存。


**图 1**  HDI原理图

![](figures/hdi-schematic.png)

使用IDL语法描述HDI接口并保存为`.idl`文件，`.idl`文件在编译过程中转换为C/C++语言的函数接口声明、客户端与服务端IPC相关过程代码，开发者只需要基于生成的`ifoo.h`函数接口实现具体服务功能即可。代码生成与编译功能已经集成在`//build/config/components/hdi/hdi.gni`编译模板，基于该编译模板编写`idl`文件的`BUILD.gn`就可以简单的生成客户端、服务端代码并编译为共享库。

## 目录

```
├── README.en.md
├── README.md
├── sensor                          #sensor HDI 接口定义
│   └── v1_0                        #sensor HDI 接口 v1.0版本定义
│       ├── BUILD.gn                #sensor idl文件编译脚本
│       ├── ISensorCallback.idl     #sensor callback 接口定义idl文件
│       ├── ISensorInterface.idl    #sensor interface 接口定义idl文件
│       └── SensorTypes.idl         #sensor 数据类型定义idl文件
├── audio                           #audio HDI 接口定义
│   └── ...
├── camera                          #camera HDI接口定义
├── codec                           #codec HDI接口定义
├── display                         #display HDI接口定义
├── face_auth                       #faceauth HDI接口定义
├── format                          #format HDI接口定义
├── input                           #input HDI接口定义
├── misc                            #misc HDI接口定义
├── pinauth                         #pinauth HDI接口定义
├── usb                             #usb HDI接口定义
├── fingerprint_auth                #fingerprintauth HDI接口定义
└── wlan                            #wlan HDI接口定义
```

## 使用说明

1. 使用IDL语法编写 `.idl` 文件

    - 参考上节目录结构创建对应模块/版本接口目录，初始版本定义为`v1_0`，如 `drivers/interface/foo/v1.0/`

    - 定义接口 `IFoo.idl`
        ```
        package ohos.hdi.foo.v1_0;

        import ohos.hdi.foo.v1_0.IFooCallback;
        import ohos.hdi.foo.v1_0.MyTypes;

        interface IFoo {
            Ping([in] String sendMsg, [out] String recvMsg);

            GetData([out] struct FooInfo info);

            SendCallbackObj([in] IFooCallback cbObj);
        }
        ```
    - 如果`interface`中用到了自定义数据类型，将自定义类型定义到`MyTypes.idl`
        ```
        package ohos.hdi.foo.v1_0;

        enum FooType {
            FOO_TYPE_ONE = 1,
            FOO_TYPE_TWO,
        };

        struct FooInfo {
            unsigned int id;
            String name;
            enum FooType type;
        };
        ```
    - 如果需要从服务端回调，可以定义`callback`接口类`IFooCallback.idl`
        ```
        package ohos.hdi.foo.v1_0;

        [callback] interface IFooCallback {
            PushData([in] String message);
        }
        ```

1. 编写 `idl`文件的`BUILD.gn`
    - 在上述`drivers/interface/foo/v1.0/`目录中添加`BUILD.gn`文件，内容参考如下：
        ```
        import("//build/config/components/hdi/hdi.gni")   # 编译idl必须要导入的模板
        hdi("foo") {                                # 目标名称，会生成两个so，分别对应 libfoo_client_v1.0.z.so 和 libfoo_stub_v1.0.z.so
            package = "ohos.hdi.foo.v1_0"           # 包名，必须与idl路径匹配
            module_name = "foo"                     # module_name控制dirver文件中驱动描 述符(struct HdfDriverEntry)的moduleName
            sources = [                             # 参与编译的idl文件
                "IFoo.idl",                         # 接口idl
                "IFooCallback.idl",                 # 用于回调的idl
                "MyTypes.idl",                      # 自定义类型idl
            ]
            language = "cpp"                        # 控制idl生成c或c++代码 可选择`c`或`cpp`
        }
        ```

2. 实现 HDI 服务

    在上述步骤中idl编译后将在out目录`out/[product_name]/gen/drivers/interfaces/foo/v1_0`生成中间代码。

    - 实现HDI服务接口

        基于工具自动生成的`foo_interface_service.h`，实现其中的服务接口，并将相关源码编译为FooService.z.so。

        实现服务业务接口：
        ```
        namespace OHOS {
        namespace HDI {
        namespace Foo {
        namespace V1_0 {

        class FooService : public IFoo {  //继承接口类，并重写接口
        public:
            virtual ~FooService() {}

            int32_t Ping(const std::string& sendMsg, std::string& recvMsg) override;
            int32_t FooService::GetData(FooInfo& info) override;
            int32_t FooService::SendCallbackObj(const sptr<IFooCallback>& cbObj) override;
        };

        } // namespace V1_0
        } // namespace Foo
        } // namespace Hdi
        } // namespace OHOS
        ```

    - 实现驱动入口

        HDI服务发布是基于用户态HDF驱动框架，所以需要实现一个驱动入口。驱动实现代码参考已经在out目录中生成，如`out/gen/xxx/foo_interface_driver.cpp`，可以根据业务需要直接使用该文件或参考该文件按业务需要重新实现。
        然后将驱动入口源码编译为`libfoo_driver.z.so`（该名称无强制规定，与hcs配置中配套即可）。

3. 发布服务

    在产品hcs配置中声明HDI服务，以标准系统Hi3516DV300单板为例，HDF设备配置路径为`vendor/hisilicon/Hi3516DV300/hdf_config/uhdf/device_info.hcs`，在其中新增以下配置：
    ```
    fooHost :: host {
            hostName = "fooHost";
            priority = 50;
            fooDevice :: device {
                device0 :: deviceNode {
                    policy = 2;
                    priority = 100;
                    preload = 2;
                    moduleName = "libfoo_driver.z.so";
                    serviceName = "foo_service";
                }
            }
        }
    ```

4. 调用HDI服务

    - 客户端在BUILD.gn中增加依赖：
    `//drivers/interface/foo/v1.0:libfoo_proxy_1.0"`

    - 在代码中调用HDI接口（以CPP为例）
        ```
        #include <v1_0/ifoo_interface.h>

        int WorkFunc(void) {
            sptr<IFoo> foo = OHOS::HDI::Foo::V1_0::Foo::Get(); // 使用Foo对象的内置静态方法获取该服务客户端实例
            if (foo == nullptr) {
                // hdi service not exist, handle error
            }

            foo->Bar(); // do interface call
        }
        ```
        如果服务存在多实例可以通过指定实例名称的方法获取对应实例`Hdi::Foo::V1_0::Foo::GetInstance(const std::string& serviceName)`;

## 约定

1. idl 文件命名规则

    - idl 文件以大驼峰命名，与接口名称保持一致，一般以字母‘I’开头。
    - 接口描述文件以`.idl`作为后缀。

1. 接口命名规则

    | 类型   | 命名风格  |
    | -----  | ------- |
    | 类、结构体、枚举、联合体等类型名，包名 | 大驼峰 |
    | 方法 | 大驼峰 |
    | 函数参数，类、结构体和联合体中的成员变量 | 小驼峰 |
    | 宏，常量(const)，枚举值 | 全大写，下划线分割 |

1. 接口版本号命名规则

    HDI接口版本号使用语义化版本号定义，即[major].[minor]。
    - major版本号不同表示接口间不兼容；
    - minor版本不同但是major版本号相同表示接口相互兼容，这种情况下不允许修改以前接口的接口名称、参数类型/个数、返回值类型/个数。

## 相关仓

[驱动子系统](https://gitee.com/openharmony/docs/blob/master/zh-cn/readme/%E9%A9%B1%E5%8A%A8%E5%AD%90%E7%B3%BB%E7%BB%9F.md)


[drivers\_adapter](https://gitee.com/openharmony/drivers_adapter/blob/master/README_zh.md)


[drivers\_peripheral](https://gitee.com/openharmony/drivers_peripheral/blob/master/README_zh.md)
