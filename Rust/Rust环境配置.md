## 安装

### Linux 或 macOS 环境

打开命令行终端，输入命令：

```bash
curl https://sh.rustup.rs -sSf | sh
# 或者
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
```

这条命令会下载并执行一个脚本来安装rustup工具，进而安装最新的Rust稳定版本。

下载过程中会出现下面的选项，直接回车默认配置即可。

```bash
Current installation options:


   default host triple: x86_64-unknown-linux-gnu
     default toolchain: stable (default)
               profile: default
  modify PATH variable: yes

1) Proceed with installation (default)
2) Customize installation
3) Cancel installation
>  
```



一旦安装成功，你将能够看到如下所示的输出：

```bash
Rust is installed now. Great!
```

安装过程会自动将Rust工具链添加到环境变量PATH中，让配置立即生效：

```bash
source "$HOME/.cargo/env"
```

检查Rust是否已经被正确地安装：

```bash
rustc --version
```

可以在命令输出中以如下所示的格式依次看到最新稳定版本的版本号、当前版本的哈希码及版本的提交日期：

```bash
rustc 1.73.0 (cc66ad468 2023-10-03)
```



## Windows环境

Rust官网下载安装器 `rustup-init.exe`，需要同时安装Visual Studio

## 更新与卸载

更新Rust版本：

```bash
rustup update
```

卸载rustup及Rust工具链：

```bash
rustup self uninstall
```

