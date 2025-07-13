Cargo 是 Rust 工具链中内置的构建系统及包管理器。

检查 cargo 是否正确安装及版本
```bash
cargo --version
```

## 使用 Cargo 创建项目
创建一个名为 hello_cargo 的项目
```bash
cargo new hello_cargo
cd hello_cargo
```
hello_cargo 目录结构：
```
hello_cargo/
├── Cargo.toml
└── src
    └── main.rs
```
同时还会初始化一个 Git 仓库并生成默认的 `.gitignore` 文件。

Cargo 会默认把所有的源代码文件保存到 src 目录下，而项目根目录只被用来存放诸如 README 文档、许可声明、配置文件等与源代码无关的文件。
### Cargo. toml
Cargo 使用 TOML（Tom's Obvious, Minimal Language）作为标准的配置格式。
```toml
[package]
name = "hello_cargo"
version = "0.1.0"
edition = "2021"

[dependencies]
```
首行文本中的 `[package] ` 是一个区域标签，它表明接下来的语句会被用于配置当前的程序包。随着我们在这个文件中增加更多的信息，还会见识到更多其他的区域（section）。

紧随标签后的 3 行语句提供了 Cargo 编译这个程序时需要的配置信息，它们分别是程序名、版本号及作者信息。

最后一行文本中的[dependencies]同样是一个区域标签，它表明随后的区域会被用来声明项目的依赖。

在 Rust 中，我们把代码的集合称作包（crate）[^1]。 Cargo 遵循的一个约定：`src/main.rs` 就是一个与包同名的二进制 crate 的 crate 根

除了在 toml 文件中手动添加依赖外，还可以使用命令进行添加：
```bash
cargo add regex #添加regex依赖(包)  
cargo remove regex #移除regex依赖(包)
```

## 使用 Cargo 构建和运行项目
通过下面的命令来完成构建任务：
```bash
$ cargo build
   Compiling hello_cargo v0.1.0 (/home/virtual/Desktop/rust_code/hello_cargo)
    Finished dev [unoptimized + debuginfo] target(s) in 0.21s
```

这个命令会将可执行程序生成在路径 `target/debug/hello_cargo`（或者 Windows 系统下的 `target\debug\hello_cargo.exe`）下。

可以通过如下的命令运行这个可执行程序：
```bash
$ ./target/debug/hello_cargo # or .\target\debug\hello_cargo.exe on Windows 
Hello, world!
```

首次使用命令 `cargo build` 构建的时候，它还会在项目根目录下创建一个名为 `Cargo.lock` 的新文件，这个文件记录了当前项目所有依赖库的具体版本号。你**最好不要手动编辑其中的内容，Cargo 可以帮助你自动维护它。**

可以使用 `cargo run` 命令来依次完成编译和运行任务：
```bash
$ cargo run
    Compiling hello_cargo v0.1.0 (/home/virtual/Desktop/rust_code/hello_cargo)
    Finished dev [unoptimized + debuginfo] target(s) in 0.13s
     Running `target/debug/hello_cargo`
Hello, world!!!
```

使用 `cargo run -- xxx`，可以传入参数运行。

Cargo 还提供了一个叫作 `cargo check` 的命令，你可以使用这个命令来快速检查当前的代码是否可以通过编译，而不需要花费额外的时间去真正生成可执行程序：

```bash
$ cargo check
   Compiling hello_cargo v0.1.0 (/home/virtual/Desktop/rust_code/hello_cargo)
    Finished dev [unoptimized + debuginfo] target(s) in 0.32 secs
```

通常来讲，由于 `cargo check` 跳过了生成可执行程序的步骤，所以它的运行速度要远远快于 `cargo build`。假如你在编码的过程中需要不断通过编译器检查错误，那么使用 `cargo check` 就会极大地加速这个过程。

## 以Release模式进行构建

使用命令：

```bash
cargo build --release
```

在优化模式下构建并生成可执行程序。它生成的可执行文件会被放置在 `target/release` 目录下。

这种模式会以更长的编译时间为代价来优化代码，从而使代码拥有更好的运行时性能。

假如你想要对代码的运行效率进行基准测试，那么请确保你会通过 `cargo run --release` 命令进行构建。


## cargo 常见命令

- `cargo build` 可以构建项目
- `cargo run` 可以运行项目
- `cargo test` 可以测试项目
- `cargo fmt` 格式化代码
- `cargo clippy` 检查语法规范
- `cargo doc` 可以为项目构建文档
- `cargo publish` 可以将库发布到 [crates.io](https://crates.io/)。





> [^1]: crate 是 Rust 中最小的编译单元，package 是单个或多个 crate 的集合，crate 和 package 都可以被叫作包，因为单个 crate 也是一个 package，但 package 通常倾向于多个 crate 的组合。本书中，crate 和 package 统一被翻译为包，只在两者同时出现且需要区别对待时，将 crate 译为单元包，将 package 译为包。