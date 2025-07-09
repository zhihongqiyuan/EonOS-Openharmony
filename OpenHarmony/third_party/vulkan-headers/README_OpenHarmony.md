# Vulkan API Headers

仓库包含Vulkan API 和 API 注册表。Vulkan 是一个适用于高性能 3D 图形设备的低开销、跨平台 API。与 OpenGL ES (GLES) 一样，Vulkan 提供用于在应用中创建高品质实时图形的工具。与OpenGL ES相比使用 Vulkan 的优势明显，Vulkan可以大大降低 CPU 开销，另外Vulkan支持 SPIR-V 二进制Shader语言。

OpenHarmony引入后，在框架层新增加载程序（Vulkan-Loader），负责加GPU驱动实现、加载OpenHarmony平台的Vulkan WSI(Window System Integration, 窗口系统集成)实现，并对外开放Vulkan API。请参考[Vulkan-Loader](https://gitee.com/openharmony/third_party_vulkan-loader/blob/master/README_OpenHarmony.md)


## 南向开发和OpenHarmony发行版

设备成功运行Vulkan必需的条件：
    1. Vulkan-Loader，由OpenHarmony提供
    2. 实现了Vulkan API 的 VulKan 驱动程序，由 SoC 提供。

### Vulkan Loader

`third_party/vulkan-loader`中实现的Vulkan加载程序是Vulkan应用与Vulkan驱动之前的Wrapper层。安装在`/system/lib[64]/libvulkan.so`中。
Vulkan Loader 会提供标准 Vulkan API 函数符号、OpenHarmony WSI扩展的函数符号，以及许多其他可选扩展。


### Vulkan 驱动程序

Vulkan-Loader成功加载Vulkan驱动程序的必要条件
    1.要参考[LoaderDriverInterface](https://gitee.com/openharmony/third_party_vulkan-loader/blob/master/docs/LoaderDriverInterface.md)实现对应的接口;
    2.提供驱动程序描述json文件，参考[加载GPU驱动](https://gitee.com/openharmony/third_party_vulkan-loader/blob/master/README_OpenHarmony.md#%E5%8A%A0%E8%BD%BDgpu%E9%A9%B1%E5%8A%A8);
    3.实现OpenHarmony平台的扩展包括`VK_OHOS_native_buffer`和`VK_OHOS_external_memory`扩展。


## 北向应用开发者

### VK_OHOS_surface 扩展

    接口：vkCreateSurfaceOHOS
    VkResult vkCreateSurfaceOHOS(
        VkInstance                               instance,
        const VkSurfaceCreateInfoOHOS            pCreateInfo, 
        const VkAllocationCallbacks              pAllocator,
        VkSurfaceKHR                             pSurface)

    参数：
        instance是要关联Surface的实例。
        pCreateInfo是一个指向结构的指针，该VkSurfaceCreateInfoOHOS 结构包含影响表面对象创建的参数。
        pAllocator是用于在没有更多特定内存分配器可用时做为Surface对象分配的分配器。
        pSurface是一个指向VkSurfaceKHR句柄的指针，在该句柄中返回创建的Surface对象。


## Vulkan使用文档

API官方文档  https://registry.khronos.org/vulkan/


## License

见 [LICENSE](LICENSE).
