用于创建或操作“universal”（多架构）文件。可以查看 CPU 架构，提取特定架构，整合和拆分库文件

```
lipo input_file command [option...]
```


- 查看库支持的架构
```sh
$ lipo -info libcomm.a
Architectures in the fat file: libcomm.a are: arm64 x86_64

$ lipo -info libqgenericbearer.dylib
Architectures in the fat file: libqgenericbearer.dylib are: x86_64 arm64
```

> man lipo