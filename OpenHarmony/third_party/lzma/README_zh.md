# third_party_lzma

## 介绍

LZMA 是著名的LZ77压缩算法的改良版本, 最大化地提高了压缩比率, 保持了高压缩速度和解压缩时较低的内存需要。

LZMA2 基于 LZMA, 在压缩过程中提供了更好的多线程支持, 和其他改进优化。

7z 是一种数据压缩和文件档案的格式, 是7zip软件的主要文件格式 [**7z官网**](https://www.7-zip.org)。
7z 格式支持不同的压缩方式: LZMA, LZMA2 和其他， 同时也支持基于AES-256的对称加密。

XZ 是一种使用LZMA2数据压缩的文件格式, XZ格式带有额外的特性: SHA/CRC数据校验, 用于提升压缩比率的filters, 拆分blocks和streams。

## 软件架构

软件架构说明

| format/algorithm  | C | C++ | C# | Java |
| :------ | :---------| :----- | :----- | :----- |
| LZMA  压缩和解压缩                                 |  ✓         | ✓      |  ✓    |  ✓    |
| LZMA2 压缩和解压缩                                 |  ✓         | ✓      |       |        |
| XZ 压缩和解压缩                                    |  ✓         | ✓      |       |        |
| 7Z 解压缩                                          | ✓          | ✓      |       |        |
| 7Z 压缩                                            |            | ✓      |       |        |
| small SFXs for installers (7z decompression)      |  ✓         |         |       |        |
| SFXs and SFXs for installers (7z decompression)   |             | ✓      |       |        |

---

```bash
/third_party/lzma
├── Asm                             # asm files (optimized code for CRC calculation and Intel-AES encryption)
│   ├── arm
│   ├── arm64
│   └── x86
├── C                               # C files (compression / decompression and other)
│   └── Util
│       ├── 7z                      # 7z decoder program (decoding 7z files)
│       ├── Lzma                    # LZMA program (file->file LZMA encoder/decoder)
│       ├── LzmaLib                 # LZMA library (.DLL for Windows)
│       └── SfxSetup                # small SFX module for installers
├── CPP
│   ├── Common                      # common files for C++ projects
│   ├── Windows                     # common files for Windows related code
│   └── 7zip                        # files related to 7-Zip
│       ├── Archive                 # files related to archiving
│       │   ├── Common              # common files for archive handling
│       │   └── 7z                  # 7z C++ Encoder/Decoder
│       ├── Bundles                 # Modules that are bundles of other modules (files)
│       │   ├── Alone7z             # 7zr.exe: Standalone 7-Zip console program (reduced version)
│       │   ├── Format7zExtractR    # 7zxr.dll: Reduced version of 7z DLL: extracting from 7z/LZMA/BCJ/BCJ2.
│       │   ├── Format7zR           # 7zr.dll:  Reduced version of 7z DLL: extracting/compressing to 7z/LZMA/BCJ/BCJ2
│       │   ├── LzmaCon             # lzma.exe: LZMA compression/decompression
│       │   ├── LzmaSpec            # example code for LZMA Specification
│       │   ├── SFXCon              # 7zCon.sfx: Console 7z SFX module
│       │   ├── SFXSetup            # 7zS.sfx: 7z SFX module for installers
│       │   └── SFXWin              # 7z.sfx: GUI 7z SFX module
│       ├── Common                  # common files for 7-Zip
│       ├── Compress                # files for compression/decompression
│       ├── Crypto                  # files for encryption / decompression
│       └── UI                      # User Interface files
│           ├── Client7z            # Test application for 7za.dll, 7zr.dll, 7zxr.dll
│           ├── Common              # Common UI files
│           ├── Console             # Code for console program (7z.exe)
│           ├── Explorer            # Some code from 7-Zip Shell extension
│           ├── FileManager         # Some GUI code from 7-Zip File Manager
│           └── GUI                 # Some GUI code from 7-Zip
├── CS
│   └── 7zip
│       ├── Common                  # some common files for 7-Zip
│       └── Compress                # files related to compression/decompression
│           ├── LZ                  # files related to LZ (Lempel-Ziv) compression algorithm
│           ├── LZMA                # LZMA compression/decompression
│           ├── LzmaAlone           # file->file LZMA compression/decompression
│           └── RangeCoder          # Range Coder (special code of compression/decompression)
├── DOC
│   ├── 7zC.txt                     # 7z ANSI-C Decoder description
│   ├── 7zFormat.txt                # 7z Format description
│   ├── installer.txt               # information about 7-Zip for installers
│   ├── lzma-history.txt            # history of LZMA SDK
│   ├── lzma-sdk.txt                # LZMA SDK description
│   ├── lzma-specification.txt      # Specification of LZMA
│   ├── lzma.txt                    # LZMA compression description
│   └── Methods.txt                 # Compression method IDs for .7z
└── Java
    └── SevenZip
        └── Compression             # files related to compression/decompression
            ├── LZ                  # files related to LZ (Lempel-Ziv) compression algorithm
            ├── LZMA                # LZMA compression/decompression
            └── RangeCoder          # Range Coder (special code of compression/decompression)
```

## 证书

LZMA SDK is written and placed in the public domain by Igor Pavlov.

Some code in LZMA SDK is based on public domain code from another developers:

  1) PPMd var.H (2001): Dmitry Shkarin

  2) SHA-256: Wei Dai (Crypto++ library)

Anyone is free to copy, modify, publish, use, compile, sell, or distribute the
original LZMA SDK code, either in source code form or as a compiled binary, for any purpose, commercial or non-commercial, and by any means.

LZMA SDK code is compatible with open source licenses, for example, you can include it to GNU GPL or GNU LGPL code.

## 编译构建

### ***UNIX/Linux***

使用gcc和clang编译7-zip有多种选项，同时7-zip代码中两部分重要的代码: C和汇编。如果与汇编代码一起编译版本，会得到更快的7-zip二进制。7-zip的汇编代码遵循不同平台的语法。

#### *arm64*

gcc和clang arm64版本支持arm64汇编代码语法。

#### *x86 and x86_64(AMD64)*

Asmc Macro Assembler 和 JWasm 在Linux 系统上都支持MASM语法，但JWasm 不支持一些7-zip中使用的cpu指令。
如果你想编译更快的7zip，必须在Linux上安装Asmc Macro Assembler [https://github.com/nidud/asmc](https://github.com/nidud/asmc)

### ***构建命令***

目录中有两个主要文件用于编译
  makefile        - 使用nmake命令编译Windows版本的7zip
  makefile.gcc    - 使用make命令编译Linux/macOs版本的7zip

首先切换到包含 `makefile.gcc`的目录下:

```bash
    cd CPP/7zip/Bundles/Alone7z
```

```bash
    make -j -f makefile.gcc
```

另外在"CPP/7zip/"目录下的"*.mak"文件也可以与优化的代码同时编译，并且带有优化选项。比如:

```bash
  cd CPP/7zip/Bundles/Alone7z
  make -j -f ../../cmpl_gcc.mak
```

## **接口使用说明**

这部分描述了C语言实现的LZMA编码和解码函数

注意: 你也可以阅读参考 LZMA Specification (lzma-specification.txt from LZMA SDK)

你也可以查看使用LZMA编码和解码的案例:
  ***C/Util/Lzma/LzmaUtil.c***

### ***LZMA 压缩的文件格式***

```bash
Offset Size Description
  0     1   Special LZMA properties (lc,lp, pb in encoded form)
  1     4   Dictionary size (little endian)
  5     8   Uncompressed size (little endian). -1 means unknown size
 13         Compressed data
```

ANSI-C(American National Standards Institue) LZMA Decoder
请注意ANSI-C的接口在LZMA SDK 4.58版本发生了变更，如果你想使用旧的接口，你可以从sourceforge.net 网站下载之前的LZMA SDK版本。

使用 ANSI-C LZMA Decoder需要使用到以下文件:

```bash
  LzmaDec.h
  LzmaDec.c
  7zTypes.h
  Precomp.h
  Compiler.h
```

参考案例: C/Util/Lzma/LzmaUtil.c

LZMA decoding的内存要求

1. LZMA decoding函数局部变量的栈内存不超过200-400字节

2. LZMA Decoder使用字典缓冲区和内部state结构

3. 内部state结构size消耗state_size = (4 + (1.5 << (lc + lp))) KB by default (lc=3, lp=0), state_size = 16 KB.

### ***如何解压缩***

LZMA Decoder (ANSI-C version) 支持以下两种接口:

**1)** 单次调用: LzmaDecode

**2)** 多次调用：LzmaDec_DecodeToBuf(类似于zlib接口)

**你必须自己定义内存分配器:**

Example:

```c
void *SzAlloc(void *p, size_t size) { p = p; return malloc(size); }
void SzFree(void *p, void *address) { p = p; free(address); }
ISzAlloc alloc = { SzAlloc, SzFree };
```

You can use p = p; operator to disable compiler warnings.

#### ***单次调用***

1. 使用场景: RAM->RAM decompressing
2. 编译文件: LzmaDec.h + LzmaDec.c + 7zTypes.h
3. 编译宏: 不需要
4. 内存需要:

- Input buffer: compressed size
- Output buffer: uncompressed size
- LZMA Internal Structures: state_size (16 KB for default settings)

**Interface:**

```c
  int LzmaDecode(Byte *dest, SizeT *destLen, const Byte *src, SizeT *srcLen,
      const Byte *propData, unsigned propSize, ELzmaFinishMode finishMode, 
      ELzmaStatus *status, ISzAlloc *alloc);
  In: 
    dest     - output data
    destLen  - output data size
    src      - input data
    srcLen   - input data size
    propData - LZMA properties  (5 bytes)
    propSize - size of propData buffer (5 bytes)
    finishMode - It has meaning only if the decoding reaches output limit (*destLen).
         LZMA_FINISH_ANY - Decode just destLen bytes.
         LZMA_FINISH_END - Stream must be finished after (*destLen).
                           You can use LZMA_FINISH_END, when you know that 
                           current output buffer covers last bytes of stream. 
    alloc    - Memory allocator.

  Out: 
    destLen  - processed output size 
    srcLen   - processed input size 

  Output:
    SZ_OK
      status:
        LZMA_STATUS_FINISHED_WITH_MARK
        LZMA_STATUS_NOT_FINISHED 
        LZMA_STATUS_MAYBE_FINISHED_WITHOUT_MARK
    SZ_ERROR_DATA - Data error
    SZ_ERROR_MEM  - Memory allocation error
    SZ_ERROR_UNSUPPORTED - Unsupported properties
    SZ_ERROR_INPUT_EOF - It needs more bytes in input buffer (src).
```

如果LZMA decoder 在输出缓冲区上限前到达并看到了end_marker, 返回OK,同时输出的destLen的值会比输出缓冲区的上限小。

你可以在完全解压缩后使用多重检查数据的完整性:

   1. 检查返回值和status变量
   2. 如果你已知未压缩的数据大小，检查 output(destLen) = uncompressedSize
   3. 如果你已知压缩后的数据大小，检查 output(srcLen) = compressedSize

#### ***根据状态多次调用 (类似于zlib接口)***

1. 使用场景: file->file decompressing
2. 编译文件: LzmaDec.h + LzmaDec.c + 7zTypes.h
3. 内存要求:

- Buffer for input stream: any size (for example, 16 KB)
- Buffer for output stream: any size (for example, 16 KB)
- LZMA Internal Structures: state_size (16 KB for default settings)
- LZMA dictionary (字典大小编码在LZMA properties header中)

使用流程：

**1)** 读取 LZMA properties (5 bytes) and uncompressed size (8 bytes, 小端序) 到 header:

```c
   unsigned char header[LZMA_PROPS_SIZE + 8];
   ReadFile(inFile, header, sizeof(header)
```

**2)** 使用"LZMA properties"分配创建 CLzmaDec(state + dictionary)

```c
  CLzmaDec state;
  LzmaDec_Constr(&state);
  res = LzmaDec_Allocate(&state, header, LZMA_PROPS_SIZE, &g_Alloc);
  if (res != SZ_OK)
    return res;
```

**3)** 初始化LzmaDec，在循环中调用LzmaDec_DecodeToBuf

```c
  LzmaDec_Init(&state);
  for (;;)
  {
    ... 
    int res = LzmaDec_DecodeToBuf(CLzmaDec *p, Byte *dest, SizeT *destLen, 
        const Byte *src, SizeT *srcLen, ELzmaFinishMode finishMode);
    ...
  }
```

**4)** 释放所有分配的结构

```c
  LzmaDec_Free(&state, &g_Alloc);
```

Look example code:
  C/Util/Lzma/LzmaUtil.c

### ***如何压缩数据***

1 编译文件:

```bash
  7zTypes.h
  Threads.h
  LzmaEnc.h
  LzmaEnc.c
  LzFind.h
  LzFind.c
  LzFindMt.h
  LzFindMt.c
  LzHash.h
```

2 内存需要:

- (dictSize * 11.5 + 6 MB) + state_size

Lzma Encoder 可使用两种内存分配器:

- alloc - for small arrays.
- allocBig - for big arrays.

例如，你可以在allocBig分配器中使用大RAM页(2 MB)来获得更快的压缩速度。需要注意的是Windows对于大RAM页的实现较差。alloc和allocBig也可以使用相同的分配器。

#### ***带有回调的单次压缩***

Look example code:
  C/Util/Lzma/LzmaUtil.c

使用场景: file->file compressing

**1)** 你必须实现接口的回调函数

```c
ISeqInStream
ISeqOutStream
ICompressProgress
ISzAlloc

static void *SzAlloc(void *p, size_t size) { p = p; return MyAlloc(size); }
static void SzFree(void *p, void *address) {  p = p; MyFree(address); }
static ISzAlloc g_Alloc = { SzAlloc, SzFree };

  CFileSeqInStream inStream;
  CFileSeqOutStream outStream;

  inStream.funcTable.Read = MyRead;
  inStream.file = inFile;
  outStream.funcTable.Write = MyWrite;
  outStream.file = outFile;
```

**2)** 创建CLzmaEncHandle对象

```c
  CLzmaEncHandle enc;

  enc = LzmaEnc_Create(&g_Alloc);
  if (enc == 0)
    return SZ_ERROR_MEM;
```

**3)** 初始化CLzmaEncProps属性

```c
  LzmaEncProps_Init(&props);
```

之后你可以改变这个结构里的一些属性

**4)** 把上一个步骤设置的属性设置给LZMA Encoder

```c
  res = LzmaEnc_SetProps(enc, &props);
```

**5)** 将编码的属性写入header

```c
    Byte header[LZMA_PROPS_SIZE + 8];
    size_t headerSize = LZMA_PROPS_SIZE;
    UInt64 fileSize;
    int i;

    res = LzmaEnc_WriteProperties(enc, header, &headerSize);
    fileSize = MyGetFileLength(inFile);
    for (i = 0; i < 8; i++)
      header[headerSize++] = (Byte)(fileSize >> (8 * i));
    MyWriteFileAndCheck(outFile, header, headerSize)
```

**6)** 调用编码函数

```c
      res = LzmaEnc_Encode(enc, &outStream.funcTable, &inStream.funcTable, 
        NULL, &g_Alloc, &g_Alloc);
```

**7)** 删除LZMA Encoder对象

```c
  LzmaEnc_Destroy(enc, &g_Alloc, &g_Alloc);
```

如果回调函数返回某些错误码，LzmaEnc_Encode 也会返回该错误码或者返回类似于SZ_ERROR_READ, SZ_ERROR_WRITE or SZ_ERROR_PROGRESS。

---

#### ***单次调用 RAM->RAM 压缩***

单次调用，RAM->RAM 压缩与设置回调的方式压缩类似, 但你需要提供指向buffers的指针而不是指向回调函数的指针。

```c
SRes LzmaEncode(Byte *dest, SizeT *destLen, const Byte *src, SizeT srcLen,
    const CLzmaEncProps *props, Byte *propsEncoded, SizeT *propsSize, int writeEndMark, 
    ICompressProgress *progress, ISzAlloc *alloc, ISzAlloc *allocBig);
Return code:
  SZ_OK               - OK
  SZ_ERROR_MEM        - Memory allocation error 
  SZ_ERROR_PARAM      - Incorrect paramater
  SZ_ERROR_OUTPUT_EOF - output buffer overflow
  SZ_ERROR_THREAD     - errors in multithreading functions (only for Mt version)
```

宏

```c
_LZMA_SIZE_OPT          - Enable some optimizations in LZMA Decoder to get smaller executable code.
_LZMA_PROB32            - It can increase the speed on some 32-bit CPUs, but memory usage for 
                        - some structures will be doubled in that case.
_LZMA_UINT32_IS_ULONG   - Define it if int is 16-bit on your compiler and long is 32-bit.
_LZMA_NO_SYSTEM_SIZE_T  - Define it if you don't want to use size_t type.
_7ZIP_PPMD_SUPPPORT     - Define it if you don't want to support PPMD method in AMSI-C .7z decoder.
```

C++版本的 LZMA Encoder/Decoder

C++版本的 LZMA 代码使用COM-LIKE接口。如果你想使用，可以了解下COM(Component Object Model)/OLE(Object Linking and Embedding)/DDE(Dynamic Data Exchange)的基础。

C++版本的 LZMA 代码部门仅仅只是将ANSI-C代码包装了.

注意：
如果你使用7zip目录下的C++代码，你必须检查你正确地使用new 运算符
MSVC 6.0 编译7-zip时，不会抛出 new 运算符的异常。所以7zip在 CPP\Common\NewHandler.cpp 重新定义了new operator

```cpp
operator new(size_t size)
{
  void *p = ::malloc(size);
  if (p == 0)
    throw CNewException();
  return p;
}
```

如果你使用的MSCV版本支持new运算符的异常抛出，你在编译7zip时可以忽略"NewHandler.cpp"。
所以使用标准的异常。实际上7zip的部分代码捕获的任何异常都会转换为HRESULT码。如果你调用7zip的COM interface 就不需要捕获CNewException.

### ***接口案例:***

Look example code : C/Util/Lzma/LzmaUtil.c

```bash
    cd C/Util/Lzma
    make -j -f makefile.gcc
    output: ./_o/7lzma
```

```bash
    LZMA-C 22.01 (x64) : Igor Pavlov : Public domain : 2022-07-15

    Usage:  lzma <e|d> inputFile outputFile
    e: encode file
    d: decode file
```

## 参与贡献

[https://sourceforge.net/p/sevenzip/_list/tickets](https://sourceforge.net/p/sevenzip/_list/tickets)

## 相关仓

[**developtools\hiperf**](https://gitee.com/openharmony/developtools_hiperf)
