# mtdev - Multitouch Protocol Translation Library

mtdev是一个独立的库，它将内核MT事件的所有变体转换为信号槽类型B协议。归为mtdev的事件可能来自任何MT设备，具体来说，包括无接触跟踪的TypeA设备、带接触跟踪的TypeA或带触点跟踪的TypeB设备。

作为Multitouch X驱动程序的一部分，大部分mtdev代码自2008年以来就已经存在。通过此软件包，手指跟踪和无缝MT协议处理在免费许可下可用。


## 目录结构

```
README.md                   英文说明
README_zh.md                中文说明
include/                    API定义
src/                        封装层实现
README.OpenSource           开源说明
```

## OpenHarmony如何集成mtdev
### 1.头文件引入
```c
#include <mtdev.h>
```
### 2.BUILD.gn添加引用
```c
public_deps += ["//third_party/mtdev:libmtdev"]
```
### 3.调用mtdev函数过程举例
```c
//全年毫秒事件时间
typedef uint64_t mstime_t;

static int use_event(const struct input_event *ev)
{
#if 0
	return ev->type == EV_ABS && mtdev_is_absmt(ev->code);
#else
	return 1;
#endif
}

//打印事件
static void print_event(const struct input_event *ev)
{
	static const mstime_t ms = 1000;
	static int slot;
	mstime_t evtime = ev->input_event_usec / ms + ev->input_event_sec * ms;
	if (ev->type == EV_ABS && ev->code == ABS_MT_SLOT)
		slot = ev->value;
	fprintf(stderr, "%012llx %02d %01d %04x %d\n",
		evtime, slot, ev->type, ev->code, ev->value);
}

#define CHECK(dev, name)			\
	if (mtdev_has_mt_event(dev, name))	\
		fprintf(stderr, "   %s\n", #name)

//检查属性
static void show_props(const struct mtdev *dev)
{
	fprintf(stderr, "supported mt events:\n");
	CHECK(dev, ABS_MT_SLOT);
	CHECK(dev, ABS_MT_TOUCH_MAJOR);
	CHECK(dev, ABS_MT_TOUCH_MINOR);
	CHECK(dev, ABS_MT_WIDTH_MAJOR);
	CHECK(dev, ABS_MT_WIDTH_MINOR);
	CHECK(dev, ABS_MT_ORIENTATION);
	CHECK(dev, ABS_MT_POSITION_X);
	CHECK(dev, ABS_MT_POSITION_Y);
	CHECK(dev, ABS_MT_TOOL_TYPE);
	CHECK(dev, ABS_MT_BLOB_ID);
	CHECK(dev, ABS_MT_TRACKING_ID);
	CHECK(dev, ABS_MT_PRESSURE);
	CHECK(dev, ABS_MT_DISTANCE);
}

//创建mtdev,检查prop，获取并打印mt事件
static void loop_device(int fd)
{
	struct mtdev dev;
	struct input_event ev;
	int ret = mtdev_open(&dev, fd);
	if (ret) {
		fprintf(stderr, "error: could not open device: %d\n", ret);
		return;
	}
	show_props(&dev);
	//当设备五秒钟未处于非活动状态时
	while (!mtdev_idle(&dev, fd, 5000)) {
		//提取所有可被处理的事件
		while (mtdev_get(&dev, fd, &ev, 1) > 0) {
			if (use_event(&ev))
				print_event(&ev);
		}
	}
	mtdev_close(&dev);
}

//主程序
int main(int argc, char *argv[])
{
	int fd;
	if (argc < 2) {
		fprintf(stderr, "Usage: mtdev <device>\n");
		return -1;
	}
	fd = open(argv[1], O_RDONLY | O_NONBLOCK);
	if (fd < 0) {
		fprintf(stderr, "error: could not open device\n");
		return -1;
	}
	if (ioctl(fd, EVIOCGRAB, 1)) {
		fprintf(stderr, "error: could not grab the device\n");
		return -1;
	}
	loop_device(fd);
	ioctl(fd, EVIOCGRAB, 0);
	close(fd);
	return 0;
}
```

## mtdev使用文档


API官方文档  http://bitmath.org/code/mtdev/


## License

见 [LICENSE](MIT(X11)License).

---

The mtdev library was released under the MIT licence in collaboration with Canonical, Ltd.