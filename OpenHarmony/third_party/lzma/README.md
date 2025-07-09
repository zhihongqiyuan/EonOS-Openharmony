# third_party_lzma

## Description

---
LZMA SDK provides the documentation, samples, header files,
libraries, and tools you need to develop applications that
use 7z / LZMA / LZMA2 / XZ compression.

LZMA is an improved version of famous LZ77 compression algorithm.
It was improved in way of maximum increasing of compression ratio,
keeping high decompression speed and low memory requirements for
decompressing.

LZMA2 is a LZMA based compression method. LZMA2 provides better
multithreading support for compression than LZMA and some other improvements.

7z is a file format for data compression and file archiving.
7z is a main file format for 7-Zip compression program (www.7-zip.org).
7z format supports different compression methods: LZMA, LZMA2 and others.
7z also supports AES-256 based encryption.

XZ is a file format for data compression that uses LZMA2 compression.
XZ format provides additional features: SHA/CRC check, filters for
improved compression ratio, splitting to blocks and streams

---

## Software Architecture

---
Source code:
| format/algorithm  | C | C++ | C# | Java |
| :------ | :---------| :----- | :----- | :----- |
| LZMA compression and decompression                |  ✓         | ✓      |  ✓    |  ✓    |
| LZMA2 compression and decompression               |  ✓         | ✓      |       |        |
| XZ compression and decompression                  |  ✓         | ✓      |       |        |
| 7z decompression                                  | ✓          | ✓      |       |        |
| 7z compression                                    |            | ✓      |       |        |
| small SFXs for installers (7z decompression)      |  ✓         |         |       |        |
| SFXs and SFXs for installers (7z decompression)   |            | ✓      |       |        |

---
Source code structure

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

---

## NOTICES / LICENSE

LZMA SDK is written and placed in the public domain by Igor Pavlov.

Some code in LZMA SDK is based on public domain code from another developers:

  1) PPMd var.H (2001): Dmitry Shkarin
  2) SHA-256: Wei Dai (Crypto++ library)

Anyone is free to copy, modify, publish, use, compile, sell, or distribute the
original LZMA SDK code, either in source code form or as a compiled binary, for
any purpose, commercial or non-commercial, and by any means.

LZMA SDK code is compatible with open source licenses, for example, you can
include it to GNU GPL or GNU LGPL code.

## Build

### ***UNIX/Linux version***

There are several options to compile 7-Zip with different compilers: gcc and clang.
Also 7-Zip code contains two versions for some critical parts of code: in C and in Assembler.
So if you compile the version with Assembler code, you will get faster 7-Zip binary.

7-Zip's assembler code uses the following syntax for different platforms:

#### *arm64: GNU assembler for ARM64 with preprocessor*

That systax of that arm64 assembler code in 7-Zip is supported by GCC and CLANG for ARM64.

#### *x86 and x86_64(AMD64)*

There are 2 programs that supports MASM syntax in Linux.
Asmc Macro Assembler and JWasm. But JWasm now doesn't support some cpu instructions used in 7-Zip.
So you must install Asmc Macro Assembler in Linux, if you want to compile fastest version of 7-Zip  x86 and x86-64: [https://github.com/nidud/asmc](https://github.com/nidud/asmc)

### ***Building commands***

There are different binaries that can be compiled from 7-Zip source.
There are 2 main files in folder for compiling:
  makefile        - that can be used for compiling Windows version of 7-Zip with nmake command
  makefile.gcc    - that can be used for compiling Linux/macOS versions of 7-Zip with make command

At first you must change the current folder to folder that contains `makefile.gcc`:

```bash
    cd CPP/7zip/Bundles/Alone7z
```

Then you can compile `makefile.gcc` with the command:

```bash
    make -j -f makefile.gcc
```

Also there are additional "*.mak" files in folder "CPP/7zip/" that can be used to compile
7-Zip binaries with optimized code and optimzing options.

To compile with GCC without assembler:

```bash
  cd CPP/7zip/Bundles/Alone7z
  make -j -f ../../cmpl_gcc.mak
```

Also you can change some compiler options in the mak files:
  cmpl_gcc.mak
  var_gcc.mak
  warn_gcc.mak

## Interface Usage

This section describes LZMA encoding and decoding functions written in C language.

Note: you can read also LZMA Specification (lzma-specification.txt from LZMA SDK)

Also you can look source code for LZMA encoding and decoding:

  ***C/Util/Lzma/LzmaUtil.c***

### ***LZMA compressed file format***

```bash
Offset Size Description
  0     1   Special LZMA properties (lc,lp, pb in encoded form)
  1     4   Dictionary size (little endian)
  5     8   Uncompressed size (little endian). -1 means unknown size
 13         Compressed data
```

ANSI-C LZMA Decoder

Please note that interfaces for ANSI-C code were changed in LZMA SDK 4.58.
If you want to use old interfaces you can download previous version of LZMA SDK
from sourceforge.net site.

To use ANSI-C LZMA Decoder you need the following files:

```bash
  LzmaDec.h
  LzmaDec.c
  7zTypes.h
  Precomp.h
  Compiler.h
```

Look example code:
  C/Util/Lzma/LzmaUtil.c

Memory requirements for LZMA decoding

1. Stack usage of LZMA decoding function for local variables is not larger than 200-400 bytes.
2. LZMA Decoder uses dictionary buffer and internal state structure.
3. Internal state structure consumes state_size = (4 + (1.5 << (lc + lp))) KB by default (lc=3, lp=0), state_size = 16 KB.

### ***How To decompress data***

LZMA Decoder (ANSI-C version) now supports 2 interfaces:

**1)** Single-call Decompressing

**2)** Multi-call State Decompressing (zlib-like interface)

**You must use external allocator:**

Example:

```c
void *SzAlloc(void *p, size_t size) { p = p; return malloc(size); }
void SzFree(void *p, void *address) { p = p; free(address); }
ISzAlloc alloc = { SzAlloc, SzFree };
```

You can use p = p; operator to disable compiler warnings.

#### ***Single-call Decompressing***

1. When to use: RAM->RAM decompressing
2. Compile files: LzmaDec.h + LzmaDec.c + 7zTypes.h
3. Compile defines: no defines
4. Memory Requirements:

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

  If LZMA decoder sees end_marker before reaching output limit, it returns OK result,
  and output value of destLen will be less than output buffer size limit.

  You can use multiple checks to test data integrity after full decompression:

   1. Check Result and "status" variable.
   2. Check that output(destLen) = uncompressedSize, if you know real uncompressedSize.
   3. Check that output(srcLen) = compressedSize, if you know real compressedSize.
       You must use correct finish mode in that case.

#### ***Multi-call State Decompressing (zlib-like interface)***

1. When to use: file->file decompressing
2. Compile files: LzmaDec.h + LzmaDec.c + 7zTypes.h
3. Memory Requirements:

- Buffer for input stream: any size (for example, 16 KB)
- Buffer for output stream: any size (for example, 16 KB)
- LZMA Internal Structures: state_size (16 KB for default settings)
- LZMA dictionary (dictionary size is encoded in LZMA properties header)

**1)** read LZMA properties (5 bytes) and uncompressed size (8 bytes, little-endian) to header:

```c
   unsigned char header[LZMA_PROPS_SIZE + 8];
   ReadFile(inFile, header, sizeof(header)
```

**2)** Allocate CLzmaDec structures (state + dictionary) using LZMA properties

```c
  CLzmaDec state;
  LzmaDec_Constr(&state);
  res = LzmaDec_Allocate(&state, header, LZMA_PROPS_SIZE, &g_Alloc);
  if (res != SZ_OK)
    return res;
```

**3)** Init LzmaDec structure before any new LZMA stream. And call LzmaDec_DecodeToBuf in loop

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

**4)** Free all allocated structures

```c
  LzmaDec_Free(&state, &g_Alloc);
```

Look example code:
  C/Util/Lzma/LzmaUtil.c

### ***How To compress data***

1 Compile files:

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

2 Memory Requirements:

- (dictSize * 11.5 + 6 MB) + state_size

3 Lzma Encoder can use two memory allocators:

- alloc - for small arrays.
- allocBig - for big arrays.

For example, you can use Large RAM Pages (2 MB) in allocBig allocator for better compression speed. Note that Windows has bad implementation for Large RAM Pages.
It's OK to use same allocator for alloc and allocBig.

#### ***Single-call Compression with callbacks***

Look example code:
  C/Util/Lzma/LzmaUtil.c

When to use: file->file compressing

**1)** you must implement callback structures for interfaces:

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

**2)** Create CLzmaEncHandle object;

```c
  CLzmaEncHandle enc;

  enc = LzmaEnc_Create(&g_Alloc);
  if (enc == 0)
    return SZ_ERROR_MEM;
```

**3)** initialize CLzmaEncProps properties;

```c
  LzmaEncProps_Init(&props);
```

  Then you can change some properties in that structure.

**4)** Send LZMA properties to LZMA Encoder

```c
  res = LzmaEnc_SetProps(enc, &props);
```

**5)** Write encoded properties to header

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

**6)** Call encoding function:

```c
      res = LzmaEnc_Encode(enc, &outStream.funcTable, &inStream.funcTable, 
        NULL, &g_Alloc, &g_Alloc);
```

**7)** Destroy LZMA Encoder Object

```c
  LzmaEnc_Destroy(enc, &g_Alloc, &g_Alloc);
```

If callback function return some error code, LzmaEnc_Encode also returns that code
or it can return the code like SZ_ERROR_READ, SZ_ERROR_WRITE or SZ_ERROR_PROGRESS.

---

#### ***Single-call RAM->RAM Compression***

Single-call RAM->RAM Compression is similar to Compression with callbacks,
but you provide pointers to buffers instead of pointers to stream callbacks:

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

Defines

```bash
_LZMA_SIZE_OPT          - Enable some optimizations in LZMA Decoder to get smaller executable code.
_LZMA_PROB32            - It can increase the speed on some 32-bit CPUs, but memory usage for 
                        - some structures will be doubled in that case.
_LZMA_UINT32_IS_ULONG   - Define it if int is 16-bit on your compiler and long is 32-bit.
_LZMA_NO_SYSTEM_SIZE_T  - Define it if you don't want to use size_t type.
_7ZIP_PPMD_SUPPPORT     - Define it if you don't want to support PPMD method in AMSI-C .7z decoder.
```

C++ LZMA Encoder/Decoder

C++ LZMA code use COM-like interfaces. So if you want to use it, you can study basics of COM/OLE.

C++ LZMA code is just wrapper over ANSI-C code.

C++ Notes

If you use some C++ code folders in 7-Zip (for example, C++ code for .7z handling),
you must check that you correctly work with "new" operator.

7-Zip can be compiled with MSVC 6.0 that doesn't throw "exception" from "new" operator.
So 7-Zip uses "CPP\Common\NewHandler.cpp" that redefines "new" operator:

```cpp
operator new(size_t size)
{
  void *p = ::malloc(size);
  if (p == 0)
    throw CNewException();
  return p;
}
```

If you use MSCV that throws exception for "new" operator, you can compile without
"NewHandler.cpp". So standard exception will be used. Actually some code of
7-Zip catches any exception in internal code and converts it to HRESULT code.
So you don't need to catch CNewException, if you call COM interfaces of 7-Zip.

### ***Interface Examples:***

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

## Contribution

[https://sourceforge.net/p/sevenzip/_list/tickets](https://sourceforge.net/p/sevenzip/_list/tickets)

## Repositories Involved

[**developtools\hiperf**](https://gitee.com/openharmony/developtools_hiperf)
