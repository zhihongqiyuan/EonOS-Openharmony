# OpenCL<sup>TM</sup> API Headers

仓库包含C语言的OpenCL API。OpenCL扩展了GPU用于图形渲染之外的能力，将通用计算并行化。OpenCL可用于图像处理、AI、高性能计算等场景的加速。推荐在计算密集型的任务以及可以并行计算的场景使用OpenCL。

例如在图像处理场景，CPU进行编解码、滤镜等特效处理较慢，使用OpenCL有十倍的加速效果。

例如在AI推理场景，使用OpenCL可以将推理性能提高一倍。

OpenHarmony引入后，新增了一层封装层，动态链接查找OpenCL库，以此进行解耦。目前仅支持native层调用OpenCL。

## 目录结构

```
README.md               英文说明
README_zh.md            中文说明
LICENSE                 证书文件
CL/                     原CL头文件
include/                封装层CL头文件
src/                    封装层CL实现
```

## OpenHarmony如何集成OpenCL
### 1.头文件引入
```c
#define USE_OPENCL_WRAPPER
#include "opencl_wrapper.h"
```
### 2.BUILD.gn添加引用
```c
deps += ["//third_party/openCL:libcl"]
```
### 3.调用OpenCL函数过程举例
```c
// 准备OpenCL的kernel程序
const char* program_source =
      "__kernel void test_main(read_only image2d_t inputImage) {\n"
      "  ...\n"
      "}";
```

```c
// 通过wrapper层的函数判断是否cl初始化成功
// 如果返回false，说明没有实际的驱动
bool cl_ok = OHOS::InitOpenCL();

// 获取设备信息
cl_int err;
cl_platform_id platform_id;
cl_device_id device_id;
clGetPlatformIDs(1, &platform_id, NULL);
clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);
// 创建OpenCL上下文
context_ = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
queue_ = clCreateCommandQueueWithProperties(context_, device_id, 0, &err);
// 使用上述源码创建OpenCL程序
cl_program p = clCreateProgramWithSource(context, 1, &program_source, nullptr, nullptr);
// 创建kernel程序，对应上述源码中的函数名
kernel_ = clCreateKernel(program, "test_main", &err);
// 创建OpenCL可以识别的图片
cl_image_format image_format;
image_format.image_channel_order = CL_RGBA;
image_format.image_channel_data_type = CL_UNORM_INT8;
cl_image_desc desc = {CL_MEM_OBJECT_IMAGE2D, width, height};
cl_mem input_image = clCreateImage(context_, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, image_format, &desc, const_cast<void*(pixmap.addr()), &err);
// 将该图片传入kernel程序
err |= clSetKernelArg(kernel_, 0, sizeof(cl_mem), &input_image);
// 在GPU上执行OpenCL程序
err = clEnqueueNDRangeKernel(queue_, kernel_, 2, NULL, global,local, 0, NULL, NULL);
// 等待执行结束
clFinish(queue_);
// 读取执行结果，从GPU传输回CPU
err = clEnqueueReadBuffer(queue_, cl_errs, CL_TRUE, 0, pixmap.size(), errs, 0, NULL, NULL);
// 使用完毕需要释放cl内存
clReleaseMemObject(input_image);
// 程序不再需要执行的时候需要释放GPU资源(OpenCL上下文)
clReleaseKernel(kernel_);
clReleaseCommandQueue(queue_);
clReleaseContext(context_);
```

## OpenCL使用文档

API官方文档  https://registry.khronos.org/OpenCL/

API详细定义 https://registry.khronos.org/OpenCL/sdk/3.0/docs/man/html/

API一张图的使用说明 https://www.khronos.org/files/opencl30-reference-guide.pdf

## License

见 [LICENSE](LICENSE).

---

OpenCL and the OpenCL logo are trademarks of Apple Inc. used by permission by Khronos.
