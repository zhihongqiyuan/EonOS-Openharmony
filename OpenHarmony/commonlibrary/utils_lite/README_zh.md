# 轻量级公共基础库<a name="ZH-CN_TOPIC_0000001121139825"></a>

-   [简介](#section62661387114)
-   [目录](#section1464106163817)
-   [使用](#section25021123178)
-   [涉及仓](#section4571352889)

## 简介<a name="section62661387114"></a>

轻量级公共基础库存放OpenHarmony通用的基础组件。这些基础组件可被OpenHarmony各业务子系统及上层应用所使用。

轻量级公共基础库在不同平台上提供的能力：

-   LiteOS-M内核\(Hi3861平台\)：文件操作、定时器。
-   LiteOS-A内核\(Hi3516、Hi3518平台\)：定时器、JS API\(设备查询，数据存储\)。

**表 1**  轻量级公共基础库功能

<a name="zh-cn_topic_0000001052623010_table206292206282"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000001052623010_row8629020112819"><th class="cellrowborder" valign="top" width="21.22%" id="mcps1.2.4.1.1"><p id="zh-cn_topic_0000001052623010_p66291220192816"><a name="zh-cn_topic_0000001052623010_p66291220192816"></a><a name="zh-cn_topic_0000001052623010_p66291220192816"></a>模块</p>
</th>
<th class="cellrowborder" valign="top" width="24.77%" id="mcps1.2.4.1.2"><p id="zh-cn_topic_0000001052623010_p3629122014289"><a name="zh-cn_topic_0000001052623010_p3629122014289"></a><a name="zh-cn_topic_0000001052623010_p3629122014289"></a>平台支持</p>
</th>
<th class="cellrowborder" valign="top" width="54.010000000000005%" id="mcps1.2.4.1.3"><p id="zh-cn_topic_0000001052623010_p206298206288"><a name="zh-cn_topic_0000001052623010_p206298206288"></a><a name="zh-cn_topic_0000001052623010_p206298206288"></a>说明</p>
</th>
</tr>
</thead>
<tbody>
<tr id="zh-cn_topic_0000001052623010_row116301920152816"><td class="cellrowborder" valign="top" width="21.22%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000001052623010_p96300207286"><a name="zh-cn_topic_0000001052623010_p96300207286"></a><a name="zh-cn_topic_0000001052623010_p96300207286"></a>文件操作</p>
</td>
<td class="cellrowborder" valign="top" width="24.77%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000001052623010_p1463032012281"><a name="zh-cn_topic_0000001052623010_p1463032012281"></a><a name="zh-cn_topic_0000001052623010_p1463032012281"></a>LiteOS-M内核</p>
</td>
<td class="cellrowborder" valign="top" width="54.010000000000005%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000001052623010_p163042052810"><a name="zh-cn_topic_0000001052623010_p163042052810"></a><a name="zh-cn_topic_0000001052623010_p163042052810"></a>提供统一的文件操作接口，屏蔽对底层不同芯片组件的差异。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001052623010_row1163022022812"><td class="cellrowborder" valign="top" width="21.22%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000001052623010_p66308202284"><a name="zh-cn_topic_0000001052623010_p66308202284"></a><a name="zh-cn_topic_0000001052623010_p66308202284"></a>定时器</p>
</td>
<td class="cellrowborder" valign="top" width="24.77%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000001052623010_p26301620192820"><a name="zh-cn_topic_0000001052623010_p26301620192820"></a><a name="zh-cn_topic_0000001052623010_p26301620192820"></a>LiteOS-M内核、LiteOS-A内核</p>
</td>
<td class="cellrowborder" valign="top" width="54.010000000000005%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000001052623010_p2630172062815"><a name="zh-cn_topic_0000001052623010_p2630172062815"></a><a name="zh-cn_topic_0000001052623010_p2630172062815"></a>提供统一的定时器操作接口，屏蔽对底层不同芯片组件的差异。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001052623010_row363012202282"><td class="cellrowborder" valign="top" width="21.22%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000001052623010_p4502105418285"><a name="zh-cn_topic_0000001052623010_p4502105418285"></a><a name="zh-cn_topic_0000001052623010_p4502105418285"></a>JS API</p>
</td>
<td class="cellrowborder" valign="top" width="24.77%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000001052623010_p199870135293"><a name="zh-cn_topic_0000001052623010_p199870135293"></a><a name="zh-cn_topic_0000001052623010_p199870135293"></a>LiteOS-A内核</p>
</td>
<td class="cellrowborder" valign="top" width="54.010000000000005%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000001052623010_p1550005422814"><a name="zh-cn_topic_0000001052623010_p1550005422814"></a><a name="zh-cn_topic_0000001052623010_p1550005422814"></a>提供获取设备信息，数据存储的JS API。</p>
</td>
</tr>
</tbody>
</table>

## 目录<a name="section1464106163817"></a>

```
commonlibrary/utils_lite/              # 轻量级公共基础库根目录
├── file                        # 文件接口实现
├── hals                        # HAL目录
│   └── file                    # 文件操作硬件抽象层头文件
├── include                     # 公共基础库对外接口文件
├── js                          # JS API目录                 
│   └── builtin
│       ├── common              # Builtin公共函数
│       ├── deviceinfokit       # 设备信息Kit
│       ├── filekit             # 文件Kit
│       └── kvstorekit          # KV存储Kit
├── kal                         # KAL目录
│   └── timer                   # Timer的KAL实现
├── memory
│   └── include                 # 内存池管理接口
└── timer_task                  # Timer实现
```

## 使用<a name="section25021123178"></a>

-   **文件操作**

    ```
    // 打开或创建文件
    const char fileName[] = "testfile";
    int fd = UtilsFileOpen(fileName, O_RDWR_FS | O_CREAT_FS | O_TRUNC_FS, 0);
    
    // 向文件写入特定大小的数据
    const char defValue[] = "test case of file system.";
    int ret = UtilsFileWrite(fd, defValue, strlen(defValue));
    
    // 关闭文件
    UtilsFileClose(fd);
    
    // 获取文件大小
    int fileLen = 0;
    ret = UtilsFileStat(fileName, &fileLen);
    printf("file size = %d\n", fileLen);
    
    // 重新定位文件读/写偏移量
    int fd1 = UtilsFileOpen(fileName, O_RDWR_FS, 0);
    ret = UtilsFileSeek(fd1, 5, SEEK_SET_FS);
    
    // 读取特定长度的文件数据
    char buf[32] = {0};
    int readLen = UtilsFileRead(fd1, buf, 32);
    ret = UtilsFileClose(fd1);
    printf("read len = %d : buf = %s\n", readLen, buf);
    
    // 删除指定文件
    ret = UtilsFileDelete(fileName);
    ```



## 涉及仓<a name="section4571352889"></a>

[公共基础](https://gitee.com/openharmony/docs/blob/master/zh-cn/readme/%E5%85%AC%E5%85%B1%E5%9F%BA%E7%A1%80%E5%BA%93.md)

[**commonlibrary\_utils\_lite**](https://gitee.com/openharmony/commonlibrary_utils_lite)