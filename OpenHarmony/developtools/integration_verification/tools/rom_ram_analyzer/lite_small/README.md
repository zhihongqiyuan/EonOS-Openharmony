# rom_ram_analyzer

## 目的

分析各部件的rom占用,结果以xls和json格式进行保存

## 支持产品

理论上可以支持所有产品,只要在config.yaml中进行了配置即可,目前已配置产品包括:
1. ipcamera_hispark_taurus
1. ipcamera_hispark_taurus_linux 
1. wifiiot_hispark_pegasus
1. hispark_pegasus_mini_system
1. hispark_taurus_mini_system

## 代码思路

1. 扫描BUILD.gn文件,收集各个target的编译产物及其对应的component_name, subsystem_name信息,并存储到config.yaml中的gn_info_file字段指定的json文件中。如果BUILD.gn中没有查找到，则直接使用get_subsytem_component.py中预先收集好的数据（根据bundle.json）
2. 根据配置文件config.yaml扫描产品的编译产物目录,得到真实的编译产物信息(主要是大小)
3. 用真实的编译产物与从BUILD.gn中收集的信息进行匹配,从而得到编译产物-大小-所属部件的对应信息
4. 如果匹配失败,会直接利用grep到项目路径下进行模糊搜索,取出现次数top1的BUILD.gn,并根据该BUILD.gn文件去查找子系统和部件
5. 如果还搜索失败,则将其归属到NOTFOUND

## 说明

1. 关于UNDEFINED:如果target的声明中和get_subsystem_component.py(原作者:陈毓德)脚本的运行结果中都没有查找到子系统和部件信息,则为UNDEFINED
1. 关于NOTFOUND:表示对应的编译产物没有在BUILD.gn的扫描结果中匹配(包括模糊匹配)到
1. 本工具是基于gn的template进行匹配,如果新增了自定义的template,则需要相应在代码中进行配置
1. 由于本工具是进行的静态扫描,且部分gn文件中使用了较为复杂的gn语法,因此本工具的**准确率无法达到100%,结果仅供参考**
1. rk3568因为主要使用的是自定义的template,所以能够在编译阶段收集更多有效信息,因此建议使用standard目录下的脚本进行分析

**子系统及部件的查找过程**

1. 先查找target声明中的subsystem_name字段和part_name字段,如果查找到,则使用target声明中的作为结果
1. 否则到get_subsystem_component.py的运行结果(默认是会保存到sub_com_info.json文件)中查找
1. 如果上述两个地方都没有查找到,则使用UNDEFINED作为子系统名和部件名进行保存

**建议**

因为是静态扫描,因此过于复杂的gn语法会导致扫描BUILD.gn文件时无法确定其编译产物,典型的:在for循环中嵌套target声明.因此就本工具来说,不建议使用过于复杂的gn语法.

## 使用

前置条件：

1. 获取整个本文件所在的整个目录
1. 对系统进行编译
1. linux平台
1. python3
1. 安装requirements
    ```txt
    xlwt==1.3.0
    ```

1. `python3 rom_analysis.py --product_name {your_product_name} --oh_path {root_path_of_oh} [-g] [-s] [-b]`运行代码,其中-g表示直接使用上次扫描的BUILD.gn的结果,-s表示直接使用已有的子系统和部件信息,此二者默认都会重新扫描, -b表示在结果中添加各部件的baseline信息（根据bundle.json）.eg: `python3 rom_analysis.py --product_name ipcamera_hispark_taurus -b`.
1. 运行完毕会产生4个json文件及一个xls文件,如果是默认配置,各文件描述如下:
   - gn_info.json:BUILD.gn的分析结果
   - sub_com_info.json:从bundle.json中进行分析获得的各部件及其对应根目录的信息
   - {product_name}_product.json:该产品实际的编译产物信息,根据config.yaml进行收集
   - **{product_name}_result.json:各部件的rom大小分析结果**
   - **{product_name}_result.xls:各部件的rom大小分析结果**
   - rom_ram_baseline.json：各部件在bundle.json中定义的rom和ram的基线

## 新增对产品的支持

在config.yaml中进行配置即可,格式说明如下:
```yaml
ipcamera_hispark_taurus: # 产品名称,需要和命令行参数中的-p参数一致
  product_infofile: ipcamera_hispark_taurus_product.json # 保存编译产物信息的json文件
  output_name: ipcamera_hispark_taurus_result.json # 保存结果的文件的名字
  product_dir: # [required]
    root: out/hispark_taurus/ipcamera_hispark_taurus/rootfs # 待分析的编译产物的根目录
    relative: # 针对性分析的子目录,key无所谓,value应当是root的子目录. 作者通常是使用so作为动态库文件目录的key,bin作为可执行文件目录的key,a作为静态库文件目录的key
      bin: bin
      so: usr/lib
      etc: etc
    rest: True  # 是否将上面root目录下除了relative指定的目录归到etc并进行匹配
  query_order:  # 匹配顺序,key应当何relative字段中的key一致,value应当在上面的target_type字段中,脚本会按照配置的顺序对文件进行匹配.对于归类为etc的产品,会匹配target_type中的所有模板类型,找到即可.因此query_order中无需配置etc项.
    so: 
      - shared_library
      - ohos_shared_library
      - ohos_prebuilt_shared_library
      - lite_library
      - lite_component   
      - target
    bin:
      - executable
      - ohos_executable
      - lite_component
```

## 新增template

为了提高准确率,本工具是按照gn的template类型对BUILD.gn进行扫描,因此如果BUILD.gn中新增了template,需要在代码层数进行相应的更改.主要是在config.py中配置Processor,并在config.yaml中添加相应内容

## 如何提高准确率

1. 如果已知编译产物不可能从某些目录下的BUILD.gn产生,则可以将其对应目录加入到config.yaml的black_list,从而不对该目录下的BUILD.gn进行扫描,以减少出错概率
1. 对于已知检测错误的或NOTFOUND的编译产物，如果知道其正确的部件和子系统，可在config.yaml中的manual_config进行配置
