## 安装

### Linux 或 macOS 环境

打开命令行终端，输入命令：

```bash
curl https://sh.rustup.rs -sSf | sh
# 或者
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
```

这条命令会下载并执行一个脚本来安装 rustup 工具，进而安装最新的 Rust 稳定版本。

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

安装过程会自动将 Rust 工具链添加到环境变量 PATH 中，让配置立即生效：

```bash
source "$HOME/.cargo/env"
```

检查 Rust 是否已经被正确地安装：

```bash
rustc --version
```

可以在命令输出中以如下所示的格式依次看到最新稳定版本的版本号、当前版本的哈希码及版本的提交日期：

```bash
rustc 1.73.0 (cc66ad468 2023-10-03)
```


## Windows 环境

Rust 官网下载安装器 `rustup-init.exe`，需要同时安装 Visual Studio

## 更新与卸载

更新 Rust 版本：

```bash
rustup update
```

卸载 rustup 及 Rust 工具链：

```bash
rustup self uninstall
```

## hello_world

```rust
//main.rs
fn main() {
    println!("Hello, world!");
}
```

终端编译并运行：

```bash
$ rustc main.rs
$ ./main
Hello, world!
```

## Rust 生态系统
由许多工具组成，其中的主要工具包括：

- `rustc`：Rust 编译器，可将 `.rs` 文件转换为二进制文件和其他 中间格式。
    
- `cargo`:Rust 依赖项管理器和构建工具。Cargo 知道如何 下载托管在 [https://crates.io](https://crates.io/) 上的依赖项,并在构建项目时将它们 传递给 `rustc`。Cargo 还附带一个内置的 测试运行程序,用于执行单元测试。
    
- `rustup`：Rust 工具链安装程序和更新程序。发布新版本 Rust 时，此工具用于 安装并更新 `rustc` 和 `cargo`。 此外，`rustup` 还可以下载标准 库的文档。你可以同时安装多个版本的 Rust，并且 `rustup` 可让你根据需要在这些版本之间切换。