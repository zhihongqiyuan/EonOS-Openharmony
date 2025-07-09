# 三方开源软件notofonts
## notofonts简介

- notofonts提供全球所有语言字库合集（CJK和表情符号除外）。

- [参考官方文档](https://github.com/notofonts/notofonts.github.io)

## 引入背景简述
在OpenHarmony系统中，为构建全球化语言字库能力，覆盖全球国家和地区，需引入该三方库来丰富语言字库合集。

## 目录结构

```
docs/       文档
fonts/      字体文件
scripts/    脚本
README      README说明
```

## OpenHarmony如何使用notofonts

在OpenHarmony中，开发者通过BUILD.gn编译脚本预置字体文件。

### 1. notofonts的编译

notofonts的编译入口在其根目录下的BUILD.gn中。简单示意如下：

```
ohos_prebuilt_etc("NotoSansBengali") {
  source = "fonts/NotoSansBengali/googlefonts/variable-ttf/NotoSansBengali[wdth,wght].ttf"
  module_install_dir = "fonts"
  subsystem_name = "thirdparty"
  part_name = "notofonts"
}

group("fonts_notofonts") {
  deps = [
    ":NotoSansBengali",
  ]
}
```

### 2. 使用notofonts

在需要使用notofonts的模块构建配置中，增加对应依赖。

## 相关仓

[global_system_resources](https://gitee.com/openharmony/global_system_resources)

[third_party_noto-cjk](https://gitee.com/openharmony/third_party_noto-cjk)

## License

- [Apache License Version 2.0](LICENSE)
- [SIL OPEN FONT LICENSE Version 1.1](fonts/LICENSE)

## 参与贡献
[如何贡献](https://gitee.com/openharmony/docs/blob/HEAD/zh-cn/contribute/参与贡献.md)

[Commit message规范](https://gitee.com/openharmony/device_qemu/wikis/Commit%20message%E8%A7%84%E8%8C%83)
