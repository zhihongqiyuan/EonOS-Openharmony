# astc-encoder

ASTC（Adaptive Scalable Texture Compression）是由ARM和AMD联合开发的一种先进的纹理压缩技术。它已经被采纳为OpenGL和OpenGL ES API的官方Khronos扩展，并作为Vulkan API的一个标准可选功能。
ASTC Encoder（Adaptive Scalable Texture Compression Encoder）是ARM推出的一款使用ASTC纹理压缩标准对图像进行压缩和解压的参考代码。

OpenHarmony上引入ASTC主要用于图库缩略图和其他应用预置图的压缩。ASTC码流可以直接由GPU解码显示，降低传输数据量和CPU解码耗时。

## 目录结构

```
Docs             #文档
Source           #源代码
Test             #测试代码
Utils            #特定场景配置测试代码
jenkins          #构建脚本
CMakeLists.txt   #编译描述文件
LICENSE.txt      #版权声明
README.md        #软件说明
```


## OpenHarmony对于astc-encoder的适配

astc-encoder引入openharmony的thirdparty目录下，
使用OpenHarmony中依赖部件的方式进行编译。
1. 主干代码下载
   ```
   repo init -u https://gitee.com/openharmony/manifest.git -b master --no-repo-verify
   repo sync -c
   repo forall -c 'git lfs pull'
   ```
2. 在使用的模块进行依赖
   ```
   deps = [ "//third_party/astc-encoder:astc_encoder_shared" ]
   ```
3. 预处理
   ```
   ./build/prebuilts_download.sh
   ```
4. 编译
   ```
   ./build.sh --product-name rk3568 --ccache
   ```
编译生成物对应路径：`out/rk3568/thirdparty/astc-encoder/libastc_encoder_shared.z.so`。

## 许可证<a name="section126611612164217"></a>

本项目遵从[LICENCE](https://gitee.com/openharmony-sig/third_party_astc-encoder/blob/4e7db2eda5ac3a43496451ce26a347559715051b/LICENSE)中所描述的许可证。

## 相关仓
[astc-encoder](https://gitee.com/openharmony-sig/third_party_astc-encoder)
