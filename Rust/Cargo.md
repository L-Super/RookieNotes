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

### Cargo. toml
Cargo 使用 TOML（Tom's Obvious, Minimal Language）作为标准的配置格式。
```toml
[package]
name = "hello_cargo"
version = "0.1.0"
authors = ["Your Name <you@example.com>"]
edition = "2021"

[dependencies]
```
首行文本中的 `[package] ` 是一个区域标签，它表明接下来的语句会被用于配置当前的程序包。随着我们在这个文件中增加更多的信息，还会见识到更多其他的区域（section）。

紧随标签后的 3 行语句提供了 Cargo 编译这个程序时需要的配置信息，它们分别是程序名、版本号及作者信息。

在 Cargo 生成 Cargo. toml 的过程中，它会尝试着从环境变量中获得你的名字与电子邮箱，但如果这些生成的信息与实际情况不符，你也可以直接修改并保存这个文件。

最后一行文本中的[dependencies]同样是一个区域标签，它表明随后的区域会被用来声明项目的依赖。

在 Rust 中，我们把代码的集合称作包（crate）[1]。虽然目前的项目暂时还不需要使用任何的第三方包，但你可以在第 2 章的第一个实践项目中看到这个配置区域的用法。





- `cargo build` 可以构建项目
- `cargo run` 可以运行项目
- `cargo test` 可以测试项目
- `cargo doc` 可以为项目构建文档
- `cargo publish` 可以将库发布到 [crates.io](https://crates.io/)。