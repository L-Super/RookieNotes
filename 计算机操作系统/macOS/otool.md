打印目标文件相关信息的工具，类似于 [Linux ldd](Linux%20ldd.md)

```zsh
-f print the fat headers
-a print the archive header
-h print the mach header
-l print the load commands
-L print shared libraries used
-D print shared library id name
-t print the text section (disassemble with -v)
-x print all text sections (disassemble with -v)
-p <routine name>  start dissassemble from routine name
-s <segname> <sectname> print contents of section
-d print the data section
-o print the Objective-C segment
-r print the relocation entries
-S print the table of contents of a library (obsolete)
-T print the table of contents of a dynamic shared library (obsolete)
-M print the module table of a dynamic shared library (obsolete)
-R print the reference table of a dynamic shared library (obsolete)
-I print the indirect symbol table
-H print the two-level hints table (obsolete)
-G print the data in code table
-v print verbosely (symbolically) when possible
-V print disassembled operands symbolically
-c print argument strings of a core file
-X print no leading addresses or headers
-m don't use archive(member) syntax
-B force Thumb disassembly (ARM objects only)
-q use llvm's disassembler (the default)
-Q use otool(1)'s disassembler
-mcpu=arg use `arg' as the cpu for disassembly
-j print opcode bytes
-P print the info plist section as strings
-C print linker optimization hints
--version print the version of /Library/Developer/CommandLineTools/usr/bin/otool
```

示例：
```zsh
otool -L demo.app/Contents/MacOS/demo

demo:
	/System/Library/Frameworks/IOKit.framework/Versions/A/IOKit (compatibility version 1.0.0, current version 275.0.0)
	/System/Library/Frameworks/Metal.framework/Versions/A/Metal (compatibility version 1.0.0, current version 343.14.0)
	/System/Library/Frameworks/Cocoa.framework/Versions/A/Cocoa (compatibility version 1.0.0, current version 24.0.0)
	@rpath/QtWidgets.framework/Versions/5/QtWidgets (compatibility version 5.15.0, current version 5.15.13)
	@rpath/QtGui.framework/Versions/5/QtGui (compatibility version 5.15.0, current version 5.15.13)
	@rpath/QtNetwork.framework/Versions/5/QtNetwork (compatibility version 5.15.0, current version 5.15.13)
	@rpath/QtCore.framework/Versions/5/QtCore (compatibility version 5.15.0, current version 5.15.13)
	@rpath/libmtcomprehensivesegment.dylib (compatibility version 0.0.0, current version 0.0.0)
	@rpath/libcurl.dylib (compatibility version 12.0.0, current version 12.0.0)
	@rpath/libssl.dylib (compatibility version 1.1.0, current version 1.1.0)
	@rpath/libcrypto.dylib (compatibility version 1.1.0, current version 1.1.0)
	@rpath/libgio-2.0.0.dylib (compatibility version 7601.0.0, current version 7601.4.0)
	/usr/lib/libc++.1.dylib (compatibility version 1.0.0, current version 1700.255.0)
	/usr/lib/libSystem.B.dylib (compatibility version 1.0.0, current version 1345.100.2)
	/usr/lib/libobjc.A.dylib (compatibility version 1.0.0, current version 228.0.0)
```

通常会配合 `install_name_tool` 修改 rpath
