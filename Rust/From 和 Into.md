Rust标准库为可转换定义了两个特性：`From`和`Into`，位于`std::convert`模块中。
```rust
pub trait From<T> {
    fn from(value: T) -> Self;
}

pub trait<T> {
    fn into(self) -> T;
}
```
涉及**Supertrait**、**泛型**和**隐式约束**。

### `Supertrait` / `Subtrait`

`From: Sized`语法意味着`From`是一个**Subtrait**`Sized`：任何实现了`From`的类型也必须实现`Sized`。或者可以说`Sized`是`From`的**Supertrait**。
### 隐式约束
每次有泛型参数时，编译器都默认它实现了 `Sized`。

```rust
pub struct Foo<T> {
    inner: T,
}
````

实际上等同于：

```rust
pub struct Foo<T> 
where
    T: Sized,
    // ^^^^^^ 这被称为约束
    // 它指定此实现仅适用于类型T实现`Sized`
    // 你可以要求多个特性被实现使用+符号如`Sized + PartialEq<T>`
{
    inner: T,
}
```
可以通过**负约束**(`negative trait bound`) 来选择退出这个行为：
```rust
// 你也可以内联行约束，而不是使用`where`子句

pub struct Foo<Sized> {
    // ^^^^^^ 这是一个负约束
    // 它读作“T`可能不是”Sized”，并允许你绑定T到 DST（如`Foo<str>）
    inner: T,
}

```

## &str 到 String
```rust
let title = String::from("A title");
```

`From` 和 `Into` 是**对称特性**。特别是，任何实现了 `From` 的类型都会自动实现一个**空白实现** `Into`：
```rust
impl<T, U> Into<U> for T
where
    U: From<T>,
{
    fn into(self) -> U {
        U::from(self)
    }
```
如果类型`T`实现了`From<U>`，那么`Into<U> for T`会自动实现。这就是我们能写`let title = "A title".into();`的原因。