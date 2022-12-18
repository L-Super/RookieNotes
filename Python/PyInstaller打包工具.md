PyInstaller 将 Python 应用程序及其所有依赖项打包到一个包中。用户可以在不安装 Python 解释器或任何模块的情况下运行打包的应用程序。

PyInstaller 是针对 Windows、 MacOSX 和 Linux 进行测试的。然而，它不是一个交叉编译器，要制作一个 Windows 应用程序，你需要在 Windows 上运行 PyInstaller，要制作一个 Linux 应用程序，你需要在 Linux 上运行它，等等。

## 参数介绍

| -h，--help                  | 查看该模块的帮助信息                                         |
| --------------------------- | ------------------------------------------------------------ |
| -F，-onefile                | 产生单个的可执行文件                                         |
| -D，--onedir                | 产生一个目录（包含多个文件，依赖文件）作为可执行程序 (默认)  |
| -a，--ascii                 | 不包含 Unicode 字符集支持                                    |
| -d，--debug                 | 产生 debug 版本的可执行文件                                  |
| -w，--windowed，--noconsolc | 指定程序运行时不显示命令行窗口（仅对 Windows 有效），GUI界面时有用 |
| -c，--nowindowed，--console | 指定使用命令行窗口运行程序（仅对 Windows 有效）              |
| -o DIR，--out=DIR           | 指定 spec 文件的生成目录。如果没有指定，则默认使用当前目录来生成 spec 文件 |
| -p DIR，--path=DIR          | 设置 Python 导入模块的路径（和设置 PYTHONPATH 环境变量的作用相似）。也可使用路径分隔符（Windows 使用分号，Linux 使用冒号）来分隔多个路径 |
| -n NAME，--name=NAME        | 指定项目（产生的 spec）名字（程序名）。如果省略该选项，那么第一个脚本的主文件名将作为 spec 的名字 |
| -i --icon                   | 添加图标。如果输入的图像不是平台格式(Windows 上是 ico，Mac 上是 icns) ，PyInstaller 会尝试使用 Pillow 将图标转换成正确的格式(如果安装了 Pillow) |

## 安装

```
pip install pyinstaller
```

## 使用

+ 将 `main.py` 打包为可执行文件，并命名为 monitor

```
pyinstaller -F main.py -n monitor 
```

+ 添加图标

```
pyinstaller -F main.py -i logo.ico
```

+ 使用 `-D` 会生成很多依赖文件，类似于动态库形式，适合以框架形式编写工具代码

```
pyinstaller -D main.py -n monitor 
```

产生的文件：

```
_bz2.pyd*
_ctypes.pyd*
_decimal.pyd*
_elementtree.pyd*
_hashlib.pyd*
_lzma.pyd*
_queue.pyd*
_socket.pyd*
_ssl.pyd*
base_library.zip
libcrypto-1_1.dll*
libffi-8.dll*
libsodium-ac42d648.dll*
libssl-1_1.dll*
libzmq-v141-mt-4_3_4-0a6f51ca.dll*
monitor.exe*
msvcp140.dll*
psutil/
pyexpat.pyd*
python3.dll*
python311.dll*
pyzmq.libs/
select.pyd*
unicodedata.pyd*
VCRUNTIME140.dll*
VCRUNTIME140_1.dll*
zmq/
```

