 Rust 并没有空值，不过它拥有一个可以存在或不存在概念的枚举。这个枚举是 `Option<T>`
 
 `Option<T>` 可以存储“T”类型的值，或者不存储任何值。

```rust
enum Option<T> {
    None,
    Some(T), 
}
```
`<T>` 意味着 `Option` 枚举的 `Some` 成员可以包含任意类型的数据，同时每一个用于 `T` 位置的具体类型使得 `Option<T>` 整体作为不同的类型。