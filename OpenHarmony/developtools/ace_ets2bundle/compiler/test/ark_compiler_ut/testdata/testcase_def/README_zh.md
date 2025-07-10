# TestData

#### 介绍

此文件夹下存储ets-loader测试使用的文件数据，API输出文件禁止上传(使用gitignore 屏蔽)。

#### 文件列表

| 文件                                          | 接口                   | 备注                                                |
| --------------------------------------------- | ---------------------- | --------------------------------------------------- |
| entry/preview/module.json                     | getPackageInfo         | 应用于preview模式，读取bundle name, module name信息 |
| entry/build/module.json                       | getPackageInfo         | 应用于build模式，读取bundle name, module name信息   |
| entry/build/sourceMaps.json                   | updateCachedSourceMaps | 应用于build模式(debug)，更新CacheSource文件         |
| entry/preview/sourceMaps.json                 | updateCachedSourceMaps | 应用于preview模式，更新CacheSource文件              |
| entry/build/loader.json                       | initArkProjectConfig   | 应用于build模式,给参数projectConfig.aceBuildJson赋值|
| entry/build/loader_aotMode.json               | initArkProjectConfig   | 应用于build模式(debug)，给参数aceBuildJson赋值      |
| entry/preview/loader.json                     | initArkProjectConfig   | 应用于preview模式，给参数buildJsonInfo赋值          |
| entry/build/hotReload/sourceMaps.map          | updateSourceMapFromFileList | 应用于hotReload模式，读取SourceMap信息         |
| updateSourceMap.json                          | updateSourceMap        | 应用于build模式，读取SourceMap信息                  |