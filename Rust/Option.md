 Rust 并没有空值，不过它拥有一个可以存在或不存在概念的枚举。这个枚举是 `Option<T>`
 
 `Option<T>` 可以存储“T”类型的值，或者不存储任何值。

```rust
enum Option<T> {
    None,
    Some(T), 
}
```