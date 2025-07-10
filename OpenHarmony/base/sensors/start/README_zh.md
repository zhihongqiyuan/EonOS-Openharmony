# Sensor组件<a name="ZH-CN_TOPIC_0000001148682248"></a>

-   [简介](#section11660541593)
-   [目录](#section44981327519)
-   [使用](#section1581412211528)
-   [相关仓](#section96071132185310)

## 简介<a name="section11660541593"></a>
sensors_start仓归属于[泛sensor服务子系统](https://gitee.com/openharmony/docs/blob/master/zh-cn/readme/%E6%B3%9BSensor%E5%AD%90%E7%B3%BB%E7%BB%9F.md)，该仓主要提供了泛sensor服务进程（即sensors）的启动文件。<br>

## 目录<a name="section44981327519"></a>

sensors_start仓的目录结构如下：

```
/base/sensors/start
├── etc
    └── init                # 放置sensors进程的启动文件
```

## 使用<a name="section1581412211528"></a>
泛sensor服务子系统提供了sensor服务和miscdevice服务，分别在[sensors\_sensor](https://gitee.com/openharmony/sensors_sensor)和[sensors\_miscdevice](https://gitee.com/openharmony/sensors_miscdevice)部件仓。产品根据需要可能选择其中一个服务或者两个服务都选择。因此将sensors进程的启动文件单独放置在sensors_start仓，由这两个服务共享，这样任何一个服务搭配启动部件就可以启动sensors进程，防止重复启动进程。<br>
sensor服务和miscdevice服务的启动配置文件分别在[sensors\_sensor](https://gitee.com/openharmony/sensors_sensor)和[sensors\_miscdevice](https://gitee.com/openharmony/sensors_miscdevice)仓中sa_profile目录下，编译后生成sensors.xml文件，内容如下所示：
```
<?xml version="1.0" encoding="utf-8"?>
<info>
    <process>sensors</process>
    <loadlibs>
        <libpath>libmiscdevice_service.z.so</libpath>
        <libpath>libsensor_service.z.so</libpath>
    </loadlibs>
    <systemability>
        <name>3602</name>
        <libpath>libmiscdevice_service.z.so</libpath>
        <run-on-create>true</run-on-create>
        <distributed>false</distributed>
        <dump-level>1</dump-level>
    </systemability>
    <systemability>
        <name>3601</name>
        <libpath>libsensor_service.z.so</libpath>
        <run-on-create>true</run-on-create>
        <distributed>false</distributed>
        <dump-level>1</dump-level>
    </systemability>
</info>
```
其中3601和3602分别为sensor服务和马达服务。若仅编译[sensors\_sensor](https://gitee.com/openharmony/sensors_sensor)仓代码，sensors.xml文件中则仅包含3601服务的配置项。启动sensors进程时会加载sensors.xml文件启动对应的服务。

## 相关仓<a name="section96071132185310"></a>

[泛sensor服务子系统](https://gitee.com/openharmony/docs/blob/master/zh-cn/readme/%E6%B3%9BSensor%E5%AD%90%E7%B3%BB%E7%BB%9F.md)

[sensors\_sensor](https://gitee.com/openharmony/sensors_sensor)

[sensors\_miscdevice](https://gitee.com/openharmony/sensors_miscdevice)

**sensors\_start**

