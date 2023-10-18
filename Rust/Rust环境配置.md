## 安装

### 在Linux或macOS环境中安装Rust

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

