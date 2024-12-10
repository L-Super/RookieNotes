特征定义了**一组可以被共享的行为，只要实现了特征，你就能使用这组行为**。

如果不同的类型具有相同的行为，那么我们就可以定义一个特征，然后为这些类型实现该特征。**定义特征**是把一些方法组合在一起，目的是定义一个实现某些目标所必需的行为的集合。

可以依据特征对类型进行抽象化处理。
- trait 定义了类型实现该 trait 所必须具备的一些方法。
- trait 在 `<type> { .. } 的 impl <trait>` 代码块中实现。

使用 `trait` 关键字来声明一个特征，`Pet` 是特征名。
```rust
struct Dog {
    name: String,
    age: i8,
}
struct Cat {
    lives: i8,
}

// 声明一个 trait
trait Pet {
    fn talk(&self) -> String;

    fn greet(&self) {
        println!("Oh you're a cutie! What's your name? {}", self.talk());
    }
}

// 提供其自定义行为的方法体
impl Pet for Dog {
    fn talk(&self) -> String {
        format!("Woof, my name is {}!", self.name)
    }
}

// 提供其自定义行为的方法体
impl Pet for Cat {
    fn talk(&self) -> String {
        String::from("Miau!")
    }
}

fn main() {
    let captain_floof = Cat { lives: 9 };
    let fido = Dog { name: String::from("Fido"), age: 5 };

    captain_floof.greet();
    fido.greet();
}
```

可使用默认实现，如果没有其他定义， `impl Pet for Dog {}` 需要指定一个空的 `impl` 块。
默认实现允许调用相同 trait 中的其他方法，哪怕这些方法没有默认实现。

孤儿规则：**如果你想要为类型 `A` 实现特征 `T`，那么 `A` 或者 `T` 至少有一个是在当前作用域中定义的！**
## trait 作为参数

如何使用 trait 来接受多种不同类型的参数。

定义了一个函数 `notify` 来调用其参数 `item` 上的 `greet` 方法，该参数是实现了 `Pet` trait 的某种类型。为此可以使用 `impl Trait` 语法

```rust
pub fn notify(item: &impl Pet) {
    println!("Breaking news! {}", item.greet());
}
```

对于 `item` 参数，我们指定了 `impl` 关键字和 trait 名称，而不是具体的类型。该参数支持任何实现了指定 trait 的类型。在 `notify` 函数体中，可以调用任何来自 `Pet` trait 的方法，比如 `greet`。

## 特征约束（Trait Bound） 语法

`impl Trait` 语法适用于直观的例子，它实际上是一种较长形式我们称为 *trait bound* 语法的语法糖：
```rust
pub fn notify<T: Pet>(item: &T) {
    println!("Breaking news! {}", item.greet());
}
```
trait bound 与泛型参数声明在一起，位于尖括号中的冒号后面。

`impl Trait` 适用于短小的例子。trait bound 则适用于更复杂的场景。
例如，可以获取两个实现了 `Summary` 的参数。

使用 `impl Trait` ：

```rust
pub fn notify(item1: &impl Pet, item2: &impl Pet) {
```

这适用于 `item1` 和 `item2` 允许是不同类型的情况（只要它们都实现了 `Pet`）。

如果希望强制它们都是相同类型，只有使用 trait bound 才可以：

```rust
pub fn notify<T: Pet>(item1: &T, item2: &T) {
```

泛型 `T` 被指定为 `item1` 和 `item2` 的参数限制，如此传递给参数 `item1` 和 `item2` 值的具体类型必须一致。

### 多重约束

如果 `notify` 需要显示 `item` 的格式化形式，同时也要使用 `greet` 方法，那么 `item` 就需要同时实现两个不同的 trait：`Display` 和 `Pet`。这可以通过 `+` 语法实现：

```rust
pub fn notify(item: &(impl Pet + Display)) {
```

`+` 语法也适用于泛型的 trait bound：

```rust
pub fn notify<T: Pet + Display>(item: &T) {
```

通过指定这两个 trait bound，`notify` 的函数体可以调用 `greet` 并使用 `{}` 来格式化 `item`。

### where 约束
通过 `where` 简化 trait bound

当特征约束变得很多时，函数的签名将变得很复杂。为此，Rust 有另一个在函数签名之后的 `where` 从句中指定 trait bound 的语法。

如：

```rust
fn some_function<T: Display + Clone, U: Clone + Debug>(t: &T, u: &U) -> i32 {}
```

使用 `where` 从句：

```rust
fn some_function<T, U>(t: &T, u: &U) -> i32
    where T: Display + Clone,
          U: Clone + Debug
{}
```
## 派生特征
系统可以自动为您的自定义类型实现支持的 trait。
派生功能是通过宏实现的，并且许多 crate 提供有用的[派生宏](宏.md)，以添加实用功能。
```rust
#[derive(Debug, Clone, Default)]
struct Player {
    name: String,
    strength: u8,
    hit_points: u8,
}

fn main() {
    let p1 = Player::default(); // Default trait adds `default` constructor.
    let mut p2 = p1.clone(); // Clone trait adds `clone` method.
    p2.name = String::from("EldurScrollz");
    // Debug trait adds support for printing with `{:?}`.
    println!("{:?} vs. {:?}", p1, p2);
}
```
常用 trait：
![](../images/Pasted%20image%2020241127111916.png)