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

	// 默认实现
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
### 用于开发输出的 `Debug`

`Debug` 特征可以让指定对象输出调试格式的字符串，通过在 `{}` 占位符中增加 `:?` 表明，例如`println!("show you some debug info: {:?}", MyObject);`.

`Debug` 特征允许以调试为目的来打印一个类型的实例，所以程序员可以在执行过程中看到该实例的具体信息。

### 等值比较的 `PartialEq` 和 `Eq`

`PartialEq` 特征可以比较一个类型的实例以检查是否相等，并开启了 `==` 和 `!=` 运算符的功能。

派生的 `PartialEq` 实现了 `eq` 方法。当 `PartialEq` 在结构体上派生时，只有_所有_ 的字段都相等时两个实例才相等，同时只要有任何字段不相等则两个实例就不相等。当在枚举上派生时，每一个成员都和其自身相等，且和其他成员都不相等。

### 次序比较的 `PartialOrd` 和 ` Ord `

`PartialOrd` 特征可以让一个类型的多个实例实现排序功能。实现了 `PartialOrd` 的类型可以使用 `<`、 `>`、`<=` 和 `>=` 操作符。一个类型想要实现 `PartialOrd` 的前提是该类型已经实现了 `PartialEq` 。

派生 `PartialOrd` 实现了 `partial_cmp` 方法，一般情况下其返回一个 `Option<Ordering>`，但是当给定的值无法进行排序时将返回 `None`。尽管大多数类型的值都可以比较，但一个无法产生顺序的例子是：浮点类型的非数字值。当在浮点数上调用 `partial_cmp` 时， `NaN` 的浮点数将返回 `None`。

当在结构体上派生时， `PartialOrd` 以在结构体定义中字段出现的顺序比较每个字段的值来比较两个实例。当在枚举上派生时，认为在枚举定义中声明较早的枚举项小于其后的枚举项。

### 复制值的 `Clone` 和 `Copy`

`Clone` 特征用于创建一个值的深拷贝（deep copy），复制过程可能包含代码的执行以及堆上数据的复制。查阅 [通过 Clone 进行深拷贝](https://course.rs/basic/ownership/ownership.html#%E5%85%8B%E9%9A%86%E6%B7%B1%E6%8B%B7%E8%B4%9D)获取有关 `Clone` 的更多信息。

派生 `Clone` 实现了 `clone` 方法，当为整个的类型实现 `Clone` 时，在该类型的每一部分上都会调用 `clone` 方法。这意味着类型中所有字段或值也必须实现了 `Clone`，这样才能够派生 `Clone` 。

`Copy` 特征允许你通过只拷贝存储在栈上的数据来复制值 (浅拷贝), 而无需复制存储在堆上的底层数据。查阅 [通过 Copy 复制栈数据](https://course.rs/basic/ownership/ownership.html#%E6%8B%B7%E8%B4%9D%E6%B5%85%E6%8B%B7%E8%B4%9D) 的部分来获取有关 `Copy` 的更多信息。

当一个类型的内部字段全部实现了 `Copy` 时，你就可以在该类型上派上 `Copy` 特征。一个类型如果要实现 `Copy` 它必须先实现 `Clone` ，因为一个类型实现 `Clone` 后，就等于顺便实现了 `Copy` 。
### 默认值的 `Default`

`Default` 特征会帮你创建一个类型的默认值。 派生 `Default` 意味着自动实现了 `default` 函数。 `default` 函数的派生实现调用了类型每部分的 `default` 函数，这意味着类型中所有的字段也必须实现了 `Default`，这样才能够派生 `Default` 。