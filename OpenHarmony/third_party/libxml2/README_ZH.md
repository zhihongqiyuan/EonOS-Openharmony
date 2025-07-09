# libxml2

#### 简介 & 软件架构

- [参考官方文档](https://gitlab.gnome.org/GNOME/libxml2/)

#### 使用说明

- [参考官方API文档](https://gitlab.gnome.org/GNOME/libxml2/-/blob/master/doc/tutorial/index.html)

#### patch包说明

以下patch包为libxml2开源库本身携带

Fix-memleaks-in-xmlXIncludeProcessFlags.patch
backport-parser-Fix-potential-memory-leak-in-xmlParseAttValue.patch
Fix-memory-leaks-for-xmlACatalogAdd.patch
Fix-memory-leaks-in-xmlACatalogAdd-when-xmlHashAddEntry-failed.patch
backport-CVE-2022-40303-Fix-integer-overflows-with-XML_PARSE_.patch
backport-CVE-2022-40304-Fix-dict-corruption-caused-by-entity-.patch
backport-schemas-Fix-null-pointer-deref-in-xmlSchemaCheckCOSS.patch
libxml2-multilib.patch

#### 参与贡献

[如何贡献](https://gitee.com/openharmony/docs/blob/HEAD/zh-cn/contribute/参与贡献.md)

[Commit message规范](https://gitee.com/openharmony/device_qemu/wikis/Commit%20message%E8%A7%84%E8%8C%83)

#### 相关仓
