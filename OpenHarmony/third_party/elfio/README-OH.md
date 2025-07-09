# ELFIO

[![Build](https://travis-ci.com/serge1/ELFIO.svg?branch=master)](https://travis-ci.com/serge1/ELFIO)
![C/C++ CI](https://github.com/serge1/ELFIO/workflows/C/C++%20CI/badge.svg)
![CodeQL](https://github.com/serge1/ELFIO/workflows/CodeQL/badge.svg)
[![Documentation](https://img.shields.io/badge/doc-download-brightgreen)](http://elfio.sourceforge.net/elfio.pdf)
[![License](https://img.shields.io/badge/License-MIT-brightgreen.svg)](https://github.com/serge1/ELFIO/blob/master/COPYING)

ELFIO是一个只有头文件的C++库，用于读取和生成ELF二进制格式文件。它被用作一个独立的库，它不依赖于任何其他产品或项目。
它遵循ISO C++，可以在各种架构和编译器上编译。

虽然是用C++编写的，但是该库也提供了一个C语言接口封装器。

完整的库文档可以在【ELFIO-教程和用户手册】（http://elfio.sourceforge.net/elfio.pdf）中找到。


# 目录结构
.
├── elfio_array.hpp                      # array section访问类，提供对array section的处理
├── elfio_demo.cpp                       # 空源文件，目的是为了编译出动态库
├── elfio_dump.hpp                       # elf dump类，dump出elf文件的各类信息，例如：elf header、section headers、segment headers等
├── elfio_dynamic.hpp                    # dynamic section访问类，提供对dynamic section的处理
├── elfio_header.hpp                     # elf header类，解析elf header的section
├── elfio.hpp                            # elfio库的主要类，可以创建、加载、保存elf文件
├── elfio_modinfo.hpp                    # modinfo section访问类，提供对modinfo section的处理
├── elfio_note.hpp                       # note section访问类，提供对note section的处理
├── elfio_relocation.hpp                 # relocation section访问类，提供对relocation section的处理
├── elfio_section.hpp                    # section类
├── elfio_segment.hpp                    # segment类
├── elfio_strings.hpp                    # string section访问类，提供对string section的处理
├── elfio_symbols.hpp                    # symbol section访问类，提供对symbol section的处理
├── elfio_utils.hpp                      # elf工具类
├── elfio_version.hpp                    # 定义elfio库版本号
└── elf_types.hpp                        # elf类型类，定义了elf文件类型、文件版本等


# 使用说明

（一）加载elf文件

1.  调用elfio类的load\(\)接口加载elf文件。

    ELFIO::elfio elfIo;
    elfIo.load(fileName);


（二）获取section名称

1.  遍历elf文件的sections。
2.  调用section_impl的get_name\(\)接口获取section名称。

    ELFIO::elfio elfIo;
    for (const auto &section : elfIo.sections) {
        if (section->get_name() == "xxx")
    }


（三）获取section数据

1.  遍历elf文件的sections。
2.  调用section_impl的get_data\(\)接口获取section数据。

    ELFIO::elfio elfIo;
    for (const auto &section : elfIo_.sections) {
        if (section->get_name() == "xxx") {
            section->get_data();
        }
    }


（四）获取symbol section

1.  遍历elf文件的symbol sections。
2.  调用symbol_section_accessor的get_symbol\(\)接口获取symbol section。

    const symbol_section_accessor symbols(reader, psec);
    for (unsigned int i = 0; i < symbols.get_symbols_num(); ++i) {
        std::string name;
        Elf64_Addr value;
        Elf_Xword size;
        unsigned char bind;
        unsigned char type;
        Elf_Half section_index;
        unsigned char other;
        symbols.get_symbol(i, name, value, size, bind, type, section_index, other);
    }


（五）获取relocation section

1.  调用relocation_section_accessor的get_entry\(\)接口获取relocation section。

    Elf64_Addr offset;
    Elf64_Addr symbolValue;
    string symbolName;
    Elf_Word type;
    Elf_Sxword addend;
    Elf_Sxword calcValue;
    relocation_section_accessor reloc(elfIo_, sec);
    for (uint32_t i = 0; i < sec->get_size() / sec->get_entry_size(); i++) {
        reloc.get_entry(i, offset, symbolValue, symbolName, type, addend, calcValue);
    }
