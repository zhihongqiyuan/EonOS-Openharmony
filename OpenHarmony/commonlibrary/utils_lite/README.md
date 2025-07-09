# commonlibrary/utils_lite<a name="EN-US_TOPIC_0000001121139825"></a>

-   [Overview](#section62661387114)
-   [Directory Structure](#section1464106163817)
-   [Usage](#section25021123178)
-   [Repositories Involved](#section4571352889)

## Overview<a name="section62661387114"></a>

The utils_lite repository stores basic components of OpenHarmony. These basic components are used by OpenHarmony subsystems and upper-layer applications.

The utils_lite library provides the following capabilities on different platforms:

-   LiteOS Cortex-M \(Hi3861 platform\): file operations and timer
-   LiteOS Cortex-A \(Hi3516 or Hi3518 platform\): timer and JavaScript APIs for device query and data storage

**Table  1**  Utils_lite capabilities

<a name="en-us_topic_0000001052623010_table206292206282"></a>
<table><thead align="left"><tr id="en-us_topic_0000001052623010_row8629020112819"><th class="cellrowborder" valign="top" width="21.22%" id="mcps1.2.4.1.1"><p id="en-us_topic_0000001052623010_p66291220192816"><a name="en-us_topic_0000001052623010_p66291220192816"></a><a name="en-us_topic_0000001052623010_p66291220192816"></a>Module</p>
</th>
<th class="cellrowborder" valign="top" width="24.77%" id="mcps1.2.4.1.2"><p id="en-us_topic_0000001052623010_p3629122014289"><a name="en-us_topic_0000001052623010_p3629122014289"></a><a name="en-us_topic_0000001052623010_p3629122014289"></a>Platform</p>
</th>
<th class="cellrowborder" valign="top" width="54.010000000000005%" id="mcps1.2.4.1.3"><p id="en-us_topic_0000001052623010_p206298206288"><a name="en-us_topic_0000001052623010_p206298206288"></a><a name="en-us_topic_0000001052623010_p206298206288"></a>Description</p>
</th>
</tr>
</thead>
<tbody>
<tr id="en-us_topic_0000001052623010_row116301920152816"><td class="cellrowborder" valign="top" width="21.22%" headers="mcps1.2.4.1.1 "><p id="en-us_topic_0000001052623010_p96300207286"><a name="en-us_topic_0000001052623010_p96300207286"></a><a name="en-us_topic_0000001052623010_p96300207286"></a>File operation</p>
</td>
<td class="cellrowborder" valign="top" width="24.77%" headers="mcps1.2.4.1.2 "><p id="en-us_topic_0000001052623010_p1463032012281"><a name="en-us_topic_0000001052623010_p1463032012281"></a><a name="en-us_topic_0000001052623010_p1463032012281"></a>LiteOS Cortex-M</p>
</td>
<td class="cellrowborder" valign="top" width="54.010000000000005%" headers="mcps1.2.4.1.3 "><p id="en-us_topic_0000001052623010_p163042052810"><a name="en-us_topic_0000001052623010_p163042052810"></a><a name="en-us_topic_0000001052623010_p163042052810"></a>Provides unified file operation interfaces that can be used on different underlying chip components.</p>
</td>
</tr>
<tr id="en-us_topic_0000001052623010_row1163022022812"><td class="cellrowborder" valign="top" width="21.22%" headers="mcps1.2.4.1.1 "><p id="en-us_topic_0000001052623010_p66308202284"><a name="en-us_topic_0000001052623010_p66308202284"></a><a name="en-us_topic_0000001052623010_p66308202284"></a>Timer</p>
</td>
<td class="cellrowborder" valign="top" width="24.77%" headers="mcps1.2.4.1.2 "><p id="en-us_topic_0000001052623010_p26301620192820"><a name="en-us_topic_0000001052623010_p26301620192820"></a><a name="en-us_topic_0000001052623010_p26301620192820"></a>LiteOS Cortex-M and LiteOS Cortex-A</p>
</td>
<td class="cellrowborder" valign="top" width="54.010000000000005%" headers="mcps1.2.4.1.3 "><p id="en-us_topic_0000001052623010_p2630172062815"><a name="en-us_topic_0000001052623010_p2630172062815"></a><a name="en-us_topic_0000001052623010_p2630172062815"></a>Provides unified timer operation interfaces that can be used on different underlying chip components.</p>
</td>
</tr>
<tr id="en-us_topic_0000001052623010_row363012202282"><td class="cellrowborder" valign="top" width="21.22%" headers="mcps1.2.4.1.1 "><p id="en-us_topic_0000001052623010_p4502105418285"><a name="en-us_topic_0000001052623010_p4502105418285"></a><a name="en-us_topic_0000001052623010_p4502105418285"></a>JavaScript API</p>
</td>
<td class="cellrowborder" valign="top" width="24.77%" headers="mcps1.2.4.1.2 "><p id="en-us_topic_0000001052623010_p199870135293"><a name="en-us_topic_0000001052623010_p199870135293"></a><a name="en-us_topic_0000001052623010_p199870135293"></a>LiteOS Cortex-A</p>
</td>
<td class="cellrowborder" valign="top" width="54.010000000000005%" headers="mcps1.2.4.1.3 "><p id="en-us_topic_0000001052623010_p1550005422814"><a name="en-us_topic_0000001052623010_p1550005422814"></a><a name="en-us_topic_0000001052623010_p1550005422814"></a>Provides JavaScript APIs for obtaining device information and storing data.</p>
</td>
</tr>
</tbody>
</table>

## Directory Structure<a name="section1464106163817"></a>

```
commonlibrary/utils_lite/              # Root directory
├── file                        # Implementation of the file system APIs
├── hals                        # HAL directory
│   └── file                    # Header files of the hardware abstraction layer for file operations
├── include                     # Header files of external APIs
├── js                          # JavaScript APIs
│   └── builtin
│       ├── common              # Builtin common function
│       ├── deviceinfokit       # Device information kit
│       ├── filekit             # File kit
│       └── kvstorekit          # KV store kit
├── kal                         # KAL directory
│   └── timer                   # KAL implementation of the timer
├── memory
│   └── include                 # Memory pool management APIs
└── timer_task                  # Timer implementation
```

## Usage<a name="section25021123178"></a>

-   **File operation**

    ```
    // Open or create a file.
    const char fileName[] = "testfile";
    int fd = UtilsFileOpen(fileName, O_RDWR_FS | O_CREAT_FS | O_TRUNC_FS, 0);
    
    // Write a specified length of data into a file with the specified file descriptor.
    const char defValue[] = "test case of file system.";
    int ret = UtilsFileWrite(fd, defValue, strlen(defValue));
    
    // Close a file with a specified file descriptor.
    UtilsFileClose(fd);
    
    // Obtain the file size.
    int fileLen = 0;
    ret = UtilsFileStat(fileName, &fileLen);
    printf("file size = %d\n", fileLen);
    
    // Adjust the read and write position offset in a file.
    int fd1 = UtilsFileOpen(fileName, O_RDWR_FS, 0);
    ret = UtilsFileSeek(fd1, 5, SEEK_SET_FS);
    
    // Read a specified length of data from a file with the specified file descriptor and write the data into the buffer.
    char buf[32] = {0};
    int readLen = UtilsFileRead(fd1, buf, 32);
    ret = UtilsFileClose(fd1);
    printf("read len = %d : buf = %s\n", readLen, buf);
    
    // Delete a specified file.
    ret = UtilsFileDelete(fileName);
    ```



## Repositories Involved<a name="section4571352889"></a>

[commonlibrary](https://gitee.com/openharmony/docs/blob/master/en/readme/utils.md)

[**commonlibrary\_utils\_lite**](https://gitee.com/openharmony/commonlibrary_utils_lite)