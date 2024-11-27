 Rust 并没有空值，不过它拥有一个可以存在或不存在概念的枚举。这个枚举是 `Option<T>`
 
 `Option<T>` 可以存储“T”类型的值，或者不存储任何值。

`Option<T>` 的定义：
```rust
enum Option<T> {
    None,
    Some(T), 
}
```
`<T>` 意味着 `Option` 枚举的 `Some` 成员可以包含任意类型的数据，同时每一个用于 `T` 位置的具体类型使得 `Option<T>` 整体作为不同的类型。

## 解包
当它是 `Some` 变体时，这些方法提取 `Option<T>`中包含的值。如果 `Option` 为 `None`：

- [`expect`](https://rustwiki.org/zh-CN/std/option/enum.Option.html#method.expect "method std::option::Option::expect") panics 带有提供的自定义消息
- [`unwrap`](https://rustwiki.org/zh-CN/std/option/enum.Option.html#method.unwrap "method std::option::Option::unwrap") panics 带有泛型信息
- [`unwrap_or`](https://rustwiki.org/zh-CN/std/option/enum.Option.html#method.unwrap_or "method std::option::Option::unwrap_or") 返回提供的默认值
- [`unwrap_or_default`](https://rustwiki.org/zh-CN/std/option/enum.Option.html#method.unwrap_or_default "method std::option::Option::unwrap_or_default") 返回类型 `T` 的默认值 (必须实现 [`Default`](https://rustwiki.org/zh-CN/std/default/trait.Default.html "trait std::default::Default") trait)
- [`unwrap_or_else`](https://rustwiki.org/zh-CN/std/option/enum.Option.html#method.unwrap_or_else "method std::option::Option::unwrap_or_else") 返回对提供的函数求值的结果


> https://rustwiki.org/zh-CN/std/option/index.html