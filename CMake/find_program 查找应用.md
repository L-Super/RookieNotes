用于查找程序
`find_program (<VAR> name1 [path1 path2 ...])`

一般签名：
```
find_program (
          <VAR>
          name | NAMES name1 [name2 ...] [NAMES_PER_DIR]
          [HINTS [path | ENV var]... ]
          [PATHS [path | ENV var]... ]
          [REGISTRY_VIEW (64|32|64_32|32_64|HOST|TARGET|BOTH)]
          [PATH_SUFFIXES suffix1 [suffix2 ...]]
          [VALIDATOR function]
          [DOC "cache documentation string"]
          [NO_CACHE]
          [REQUIRED]
          [NO_DEFAULT_PATH]
          [NO_PACKAGE_ROOT_PATH]
          [NO_CMAKE_PATH]
          [NO_CMAKE_ENVIRONMENT_PATH]
          [NO_SYSTEM_ENVIRONMENT_PATH]
          [NO_CMAKE_SYSTEM_PATH]
          [NO_CMAKE_INSTALL_PREFIX]
          [CMAKE_FIND_ROOT_PATH_BOTH |
           ONLY_CMAKE_FIND_ROOT_PATH |
           NO_CMAKE_FIND_ROOT_PATH]
         )
```

Windows 上，`.exe` 和 `.com` 文件扩展名会自动添加到提供的文件名中

`HINTS` 和 `PATHS` 选项包含搜索文件的默认位置之外的其他位置。`PATH_SUFFIXES` 可以包含许多子目录，在这些位置的下面搜索相应的子目录。

`NO_..._PATH` 参数可以用来跳过相应的位置

示例：
在 `QtBinDir` 目录下查找 `windeployqt` 或 `macdeployqt`
```cmake
find_program(QT_DEPLOY_EXECUTABLE windeployqt macdeployqt HINTS "${QtBinDir}")
```

> [find_program](https://cmake-doc.readthedocs.io/zh-cn/latest/command/find_program.html)
