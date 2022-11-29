GNU/Linux上，CMake默认生成Unix Makefile来构建项目： 

+ Makefile : make 将运行指令来构建项目。 
+ CMakefiles ：包含临时文件的目录，CMake用于检测操作系统、编译器等。此外，根据所选的生成器，它还包含特定的文件。 
+ cmake_install.cmake ：处理安装规则的CMake脚本，在项目安装时使用。 
+ CMakeCache.txt ：CMake缓存。CMake在重新运行配置时使用这个文件。