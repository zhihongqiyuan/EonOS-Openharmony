# mtdev - Multitouch Protocol Translation Library

The mtdev is a stand-alone library which transforms all variants of kernel MT events to the slotted type B protocol. The events put into mtdev may be from any MT device, specifically type A without contact tracking, type A with contact tracking, or type B with contact tracking. 

The bulk of the mtdev code has been out there since 2008, as part of the Multitouch X Driver. With this package, finger tracking and seamless MT protocol handling is available under a free license.


## Directory structure

```
README.md                   English description
README_zh.md                Chinese description
include/                    API Definition
src/                        Encapsulation layer implementation
README.OpenSource           Open Source Description
```

## How OpenHarmony integrates mtdev
### 1.Header file import
```c
#include <mtdev.h>
```
### 2.BUILD.gn add Reference
```c
public_deps += ["//third_party/mtdev:libmtdev"]
```
### 3.Example of calling mtdev function
```c
//year-proof millisecond event time
typedef uint64_t mstime_t;

static int use_event(const struct input_event *ev)
{
#if 0
	return ev->type == EV_ABS && mtdev_is_absmt(ev->code);
#else
	return 1;
#endif
}

//print events
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

//check prop
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

//Create mtdev, check prop, get and print mt events
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
	//while the device has not been inactive for five seconds
	while (!mtdev_idle(&dev, fd, 5000)) {
		//extract all available processed events
		while (mtdev_get(&dev, fd, &ev, 1) > 0) {
			if (use_event(&ev))
				print_event(&ev);
		}
	}
	mtdev_close(&dev);
}

//main function
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

## mtdev Working with documents


Official API http://bitmath.org/code/mtdev/


## License

见 [LICENSE](MIT(X11)License).

---

The mtdev library was released under the MIT licence in collaboration with Canonical, Ltd.