一般方式安装的版本比较低
```shell
sudo apt install cmake
```

可以去官网，复制[cmake-3.24.2-linux-x86_64.sh](https://github.com/Kitware/CMake/releases/download/v3.24.2/cmake-3.24.2-linux-x86_64.sh)链接，然后
```shell
wget https://github.com/Kitware/CMake/releases/download/v3.24.2/cmake-3.24.2-linux-x86_64.sh
```
下载到本地后，执行以下命令即可
```shell
chmod + x cmake-3.22.0-linux-x86_64.sh
sudo ./cmake-3.22.0-linux-x86_64.sh --exclude-subdir --prefix=/usr/local
```
验证
```shell
$ cmake --version
cmake version 3.24.2

CMake suite maintained and supported by Kitware (kitware.com/cmake).
```
