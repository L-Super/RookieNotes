crate 有两种形式：二进制 crate 和库 crate。每个二进制 create 都必须有一个 `main` 函数，用于定义可执行文件运行时的行为。

一个 package 是由一个或多个 crate 组成，提供一组功能。包含一个 Cargo. toml 文件，用于描述如何构建这些 creates。

一个 package 可以包含任意数量的二进制 crate，但最多只能有一个库 crate。一个 package 必须至少包含一个 crate

创建 package：
```bash
$ cargo new my-project
     Created binary (application) `my-project` package
$ tree my-project
├── Cargo.toml
└── src
    └── main.rs
```
