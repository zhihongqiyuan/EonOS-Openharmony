# 三方开源软件noto-cjk
## noto-cjk简介

- noto-cjk提供了简体中文、繁体中文、日文、韩文的字库合集。

- [参考官方文档](https://github.com/googlefonts/noto-cjk)

## 引入背景简述
在OpenHarmony系统中，为构建全球化语言字库能力，覆盖全球国家和地区，需引入该三方库来丰富日韩语言。


## 目录结构

```
Sans/       字体文件
Serif/      字体文件
README      README说明
```

## 仓库托管差异说明
背景：noto-cjk开源软件直接使用git仓存储二进制文件，整仓大小超出Gitee支持范围。

方案：当前通过重构git历史，lfs管理二进制文件的方式来解决问题。

影响：重构后保留Author、Date信息，commit中hash值会改变，lfs管理仓库会小很多。

基线[commit](https://gitee.com/openharmony-sig/third_party_noto-cjk/commit/9e05d2c74e0b61204cf7001ddb40dc318cd0a13d)

社区[Serif2.001](https://github.com/notofonts/noto-cjk/releases/tag/Serif2.001)

## OpenHarmony如何使用noto-cjk

在OpenHarmony中，开发者通过BUILD.gn编译脚本预置字体文件。

### 1. noto-cjk的编译

noto-cjk的编译入口在其根目录下的BUILD.gn中。简单示意如下：

```
ohos_prebuilt_etc("NotoSansJP-VF") {
  source = "Sans/Variable/TTF/Subset/NotoSansJP-VF.ttf"
  module_install_dir = "fonts"
  subsystem_name = "thirdparty"
  part_name = "noto-cjk"
}

group("fonts_notocjk") {
  deps = [
    ":NotoSansJP-VF",
  ]
}
```

### 2. 使用noto-cjk

在需要使用noto-cjk的模块构建配置中，增加对应依赖。

## 相关仓

[global_system_resources](https://gitee.com/openharmony/global_system_resources)

[third_party_notofonts](https://gitee.com/openharmony/third_party_notofonts)

## License

- [SIL OPEN FONT LICENSE Version 1.1](LICENSE)

## 参与贡献
[如何贡献](https://gitee.com/openharmony/docs/blob/HEAD/zh-cn/contribute/参与贡献.md)

[Commit message规范](https://gitee.com/openharmony/device_qemu/wikis/Commit%20message%E8%A7%84%E8%8C%83)
