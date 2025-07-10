### SPDX License 匹配工具

该工具用于从Excel文件和JSON文件中提取、匹配并映射开源许可证信息。其核心功能包括将Excel文件中的`cc_url`字段复制到`match_url`字段中，并将`spdx_fixed_license_name`字段与JSON中的SPDX License键进行匹配，结果输出到Excel文件的`match_license`字段。

### 目录结构

```
plaintextCopy codeproject_root/
├── src/
│   └── spdx_license_matcher.py         # SPDX License 匹配工具代码
├── test/
│   └── test_spdx_license_matcher.py     # SPDX License 匹配工具的测试代码
└── data/
    ├── url_license_fixed.xlsx           # 输入的Excel数据文件
    └── spdx.json                        # 输入的SPDX License JSON文件
```

### 文件详细说明

- **src/spdx_license_matcher.py**：核心脚本文件，包含加载数据、列复制、许可证匹配及结果输出等功能。
- **test/test_spdx_license_matcher.py**：测试文件，使用`unittest`框架验证工具的核心功能是否正常工作。
- **data/oh_spdx_license_match.xlsx**和**data/spdx.json**：测试数据文件，分别包含Excel的许可URL数据和SPDX License的JSON映射数据。



#### 使用说明

1. 运行脚本需要传递三个参数：输入的Excel文件路径、JSON文件路径和输出Excel文件路径。
2. 命令示例如下：

```
python src/spdx_license_matcher.py data/oh_spdx_license_match.xlsx data/spdx.json data/output.xlsx
```

### 

#### 测试步骤

1. 在项目根目录运行测试命令：

```
python -m unittest discover -s test
```

1. 测试输出：
   - 测试代码会自动验证主要功能，包括列复制、许可证匹配、数据保存等。
   - 若所有测试通过，将显示“OK”。

### 注意事项

- **SPDX 数据格式**：确保JSON文件的格式符合SPDX标准。
- **Excel 文件格式**：输入Excel文件应包含`cc_url`和`spdx_fixed_license_name`列。
- **测试文件**：测试中创建的临时文件会在测试结束时自动删除。

------

### 常见问题

1. 许可证名称无法匹配：请检查输入Excel文件的`spdx_fixed_license_name`字段内容，确保名称拼写正确，并与JSON文件的键名称一致。
2. 多项许可匹配：对于含有多个以“分号”分隔的许可证名称，脚本会逐个进行匹配，并将结果以“分号”分隔的方式填入`match_license`列中。
