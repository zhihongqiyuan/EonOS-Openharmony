# Sensor<a name="EN-US_TOPIC_0000001148682248"></a>

-   [Introduction](#section11660541593)
-   [Directory Structure](#section44981327519)
-   [Usage](#section1581412211528)
-   [Repositories Involved](#section96071132185310)

## Introduction<a name="section11660541593"></a>

Start sensor, vibrator and other small device services.

## Directory Structure<a name="section44981327519"></a>

The sample code for importing the start module is as follows:

```
/base/sensors/start
├── etc
    └── init                # Place the startup file for the hsensor process
```

## Usage<a name="section1581412211528"></a>
The repository places the startup file of the sensors process to start sensor and small device services such as vibrator. Sensor and small device services such as vibrator share the sensors process.<br>
Service code for sensor and small device services such as vibrator is [sensors\_sensor](https://gitee.com/openharmony/sensors_sensor)  and [sensors\_miscdevice](https://gitee.com/openharmony/sensors_miscdevice) part compartments, respectively. when selecting a part, the product may select either or both parts as needed. This requires that the startup file of the sensors process be placed in a separate start-up part compartment, shared by the two parts, so that any part silo with the starter part can start the sensors process and prevent duplicate start-up of the process.<br>
Service startup profiles for sensor and small device services such as vibrator are in the sa_profile directory of [sensors\_sensor](https://gitee.com/openharmony/sensors_sensor)  and [sensors\_miscdevice](https://gitee.com/openharmony/sensors_miscdevice) part, respectively.The resulting system/profile/sensors.xml file is compiled, as follows.Of these, 3601 and 3602 serve the sensor and vibrator.If only the [sensors\_sensor](https://gitee.com/openharmony/sensors_sensor) code is compiled, the sensors.xml file contains only the configuration items for the 3601 service. The sensors.xml file starts the service when the sensors process is started.    
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

## Repositories Involved<a name="section96071132185310"></a>

Pan-sensor subsystem

[sensors\_sensor](https://gitee.com/openharmony/sensors_sensor)

[sensors\_miscdevice](https://gitee.com/openharmony/sensors_miscdevice)

**sensors\_start**
