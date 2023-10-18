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
同时还会初始化一个 Git 仓库并生成默认的 `.git`





- `cargo build` 可以构建项目
- `cargo run` 可以运行项目
- `cargo test` 可以测试项目
- `cargo doc` 可以为项目构建文档
- `cargo publish` 可以将库发布到 [crates.io](https://crates.io/)。