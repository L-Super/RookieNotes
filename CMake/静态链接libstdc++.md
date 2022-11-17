1. Clion使用Mingw配置编译，无法单独执行程序，会提示缺少libstdc++动态库。
2. Linux下，由于不同服务器可能编译器不同，会出现版本过低的问题，导致程序无法运行
```
[root@bogon monitor]# ./RaidMonitorServer 
./RaidMonitorServer: /lib64/libstdc++.so.6: version `CXXABI_1.3.9' not found (required by ./RaidMonitorServer)
./RaidMonitorServer: /lib64/libstdc++.so.6: version `GLIBCXX_3.4.26' not found (required by ./RaidMonitorServer)
./RaidMonitorServer: /lib64/libstdc++.so.6: version `GLIBCXX_3.4.22' not found (required by ./RaidMonitorServer)
./RaidMonitorServer: /lib64/libstdc++.so.6: version `GLIBCXX_3.4.21' not found (required by ./RaidMonitorServer)

```

所以，将其改为C++库静态链接
```cmake
target_link_libraries(${PROJECT_NAME} PRIVATE  -static-libstdc++)
```