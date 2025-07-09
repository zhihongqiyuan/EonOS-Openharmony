# libevdev - wrapper library for evdev input devices

libevdev是evdev设备的包装库。它将处理evdev设备时的常见任务移动到库中，并向调用方提供库接口，从而避免错误的ioctl等。

最终目标是libevdev包装了evdev设备可用的所有ioctl，因此不需要直接访问。


## 目录结构

```
README.md                   英文说明
README_zh.md                中文说明
export_include/libevdev/    API定义
include/                    引用头文件
libevdev/                   封装层实现
README.OpenSource           开源说明
```

## OpenHarmony如何集成libevdev
### 1.头文件引入
```c
#define EVDEV_H
#include <libevdev/libevdev.h>
```
### 2.BUILD.gn添加引用
```c
public_deps += ["//third_party/libevdev:libevdev"]
```
### 3.调用libevdev函数过程举例
```c
// 下面是一个简单的示例，展示了如何使用libevdev。此示例打开一个设备，检查相对坐标和鼠标左键，如果找到，则监听设备并打印输入事件

// 打开一个输入设备
struct libevdev *dev = NULL;
int fd;
int rc = 1;
 
fd = open("/dev/input/event0", O_RDONLY|O_NONBLOCK);
rc = libevdev_new_from_fd(fd, &dev);
if (rc < 0) {
        fprintf(stderr, "Failed to init libevdev (%s)\n", strerror(-rc));
        exit(1);
}
// 打印输入设备信息
printf("Input device name: \"%s\"\n", libevdev_get_name(dev));
printf("Input device ID: bus %#x vendor %#x product %#x\n",
       libevdev_get_id_bustype(dev),
       libevdev_get_id_vendor(dev),
       libevdev_get_id_product(dev));
// 检查相对坐标和鼠标左键
if (!libevdev_has_event_type(dev, EV_REL) ||
    !libevdev_has_event_code(dev, EV_KEY, BTN_LEFT)) {
        printf("This device does not look like a mouse\n");
        exit(1);
}
 
 // 监听该设备并打印输入事件
do {
        struct input_event ev;
        rc = libevdev_next_event(dev, LIBEVDEV_READ_FLAG_NORMAL, &ev);
        if (rc == 0)
                printf("Event: %s %s %d\n",
                       libevdev_event_type_get_name(ev.type),
                       libevdev_event_code_get_name(ev.type, ev.code),
                       ev.value);
} while (rc == 1 || rc == 0 || rc == -EAGAIN);
```

## libevdev使用文档

代码仓库 https://gitlab.freedesktop.org/libevdev/libevdev.git

API官方文档  http://www.freedesktop.org/software/libevdev/doc/latest/

API详细定义 https://www.freedesktop.org/software/libevdev/doc/latest/libevdev_8h.html

补丁包 https://gitlab.freedesktop.org/libevdev/libevdev/merge_requests/

Bug上报 https://gitlab.freedesktop.org/libevdev/libevdev/issues/

问题反馈 http://lists.freedesktop.org/mailman/listinfo/input-tools


## License

见 [LICENSE](MITLicense).

---

