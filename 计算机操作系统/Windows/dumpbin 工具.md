**Dumpbin** 是 Microsoft Visual Studio 提供的一个强大的命令行工具，用于分析二进制文件（如 EXE、DLL、OBJ、LIB 等）的内部结构。它能够显示文件的头信息、导入/导出表、依赖项等详细内容。

```
DUMPBIN [options] files...
```

常用选项：

- `/EXPORTS`  查看 DLL 的导出函数表
- `/IMPORTS`  查看文件的导入函数及依赖的 DLL
- `/HEADERS`  显示文件的 PE 头 information
- `/DEPENDENTS`  分析文件依赖的动态链接库
- `/SECTION`  查看特定节区的信息
- `/SUMMARY`  显示文件的摘要信息
- `/RELOCATIONS`  查看重定位表
- `/SYMBOLS`  查看符号表（适用于 OBJ 文件）

## 示例

查看导出函数接口：

```bash
dumpbin /EXPORTS npapi_plugin.dll
Microsoft (R) COFF/PE Dumper Version 14.50.35725.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file E:\...\npapi_plugin.dll

File Type: DLL

  Section contains the following exports for npapi_plugin.dll

    00000000 characteristics
    FFFFFFFF time date stamp
        0.00 version
           1 ordinal base
           4 number of functions
           4 number of names

    ordinal hint RVA      name

          1    0 0000F0B0 NP_GetEntryPoints
          2    1 0000F110 NP_GetValue
          3    2 0000F150 NP_Initialize
          4    3 0000F190 NP_Shutdown

  Summary

        1000 .data
       29000 .rdata
        2000 .reloc
        1000 .rsrc
       25000 .text
```

查看依赖库：

```bash
dumpbin /DEPENDENTS npapi_plugin.dll
Microsoft (R) COFF/PE Dumper Version 14.50.35725.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file E:\...\npapi_plugin.dll

File Type: DLL

  Image has the following dependencies:

    KERNEL32.dll
    USER32.dll
    MSVCP140.dll
    SHLWAPI.dll
    VCRUNTIME140.dll
    api-ms-win-crt-runtime-l1-1-0.dll
    api-ms-win-crt-heap-l1-1-0.dll
    api-ms-win-crt-math-l1-1-0.dll
    api-ms-win-crt-locale-l1-1-0.dll
    api-ms-win-crt-convert-l1-1-0.dll
    api-ms-win-crt-stdio-l1-1-0.dll

  Summary

        1000 .data
       29000 .rdata
        2000 .reloc
        1000 .rsrc
       25000 .text
```

查看  PE 文件头信息，可以获取很多信息，如：

- 基本属性
  - **文件类型**: 32 位 x86 DLL
  - **编译时间**: 2026 年 3 月 17 日 11:37 :28
  - **链接器版本**: 14.44（Visual Studio 2015-2017）
  - **子系统**: Windows GUI

```bash
dumpbin /headers npapi_plugin.dll
Microsoft (R) COFF/PE Dumper Version 14.50.35725.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file E:\...\npapi_plugin.dll

PE signature found

File Type: DLL // 文件类型

FILE HEADER VALUES
             14C machine (x86) // 目标架构
               5 number of sections // 节区数量
        69B8CC78 time date stamp Tue Mar 17 11:37:28 2026 // 编译时间戳
               0 file pointer to symbol table // 符号表偏移
               0 number of symbols // 符号数量
              E0 size of optional header // 可选头大小
            2102 characteristics // 文件特性
                   Executable
                   32 bit word machine
                   DLL

...
```

配合 findstr 命令查看 DLL 为 x64 还是 x86：

```
dumpbin /headers npapi_plugin.dll | findstr machine
             14C machine (x86)
                   32 bit word machine
```

输出示例：

- 32 位 DLL：`14C machine (x86)`
- 64 位 DLL：`8664 machine (x64)`
- ARM：`ARM` 或 `ARM64`



> https://learn.microsoft.com/zh-cn/cpp/build/reference/dumpbin-reference?view=msvc-170