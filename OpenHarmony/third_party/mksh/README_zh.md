# mksh	

mksh简而言之就是MirBSD Korn Shell。它是一个命令解释器，用于shell命令交互和shell脚本语言。它是Shell语言的超集（superset），同时也兼容原本的Korn shell。

在本文档的末尾已为您附上官方链接。 

## 目录结构

```
README_zh.md                中文说明
build.sh                    编译脚本
main.c                      程序入口
check.pl                    测试用例
README.OpenSource           开源说明
```

## OpenHarmony中如何使用mksh

### 1.如何使用mksh

需要执行`./bin/mksh`进入，然后才是mksh的shell。

### 2.BUILD.gn添加引用

```
public_deps += ["//third_party/mksh"]
```

### 3.调用mksh使用示例

```
OHOS # ./bin/mksh                                                                                                                                                                                                                      
OHOS:/$                                                                                                                             
OHOS:/$ echo $PATH                                                                                                                  
/usr/local/bin:/bin:/usr/bin                                                                                                        
OHOS:/$    
```

## mksh使用文档

代码仓库 https://github.com/MirBSD/mksh

官方文档 http://www.mirbsd.org/mksh.htm

mksh FAQ http://www.mirbsd.org/mksh-faq.htm