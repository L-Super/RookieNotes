`match` 控制流运算符，它允许我们将一个值与一系列的模式相比较，并根据相匹配的模式执行相应代码。模式可由字面值、变量、通配符和许多其他内容构成。

Rust 中的匹配是 **穷尽的**（exhaustive）：必须穷举到最后的可能性来使代码有效。

```rust
enum Coin { Penny, Nickel, Dime, Quarter, }

fn value_in_cents(coin: Coin) -> u8 {
    match coin {
        Coin::Penny => {
            println!("Lucky penny!");
            1
        }
        Coin::Nickel => 5,
        Coin::Dime => 10,
        Coin::Quarter => 25,
    }
}
```
在分支中运行多行代码，可以使用大括号；大括号后，和分支最后的逗号是可选的。
## 匹配结构体
```rust
struct Foo {
    x: (u32, u32),
    y: u32,
}

#[rustfmt::skip]
fn main() {
    let foo = Foo { x: (1, 2), y: 3 };
    match foo {
        Foo { x: (1, b), y } => println!("x.0 = 1, b = {b}, y = {y}"),
        Foo { y: 2, x: i }   => println!("y = 2, x = {i:?}"),
        Foo { y, .. }        => println!("y = {y}, other fields were ignored"),
    }
}
```
## 匹配枚举
还可用于将变量绑定到值的某些部分。
```rust
enum Result {
    Ok(i32),
    Err(String),
}

fn divide_in_two(n: i32) -> Result {
    if n % 2 == 0 {
        Result::Ok(n / 2)
    } else {
        Result::Err(format!("cannot divide {n} into two equal parts"))
    }
}

fn main() {
    let n = 100;
    match divide_in_two(n) {
        Result::Ok(half) => println!("{n} divided in two is {half}"),
        Result::Err(msg) => println!("sorry, an error happened: {msg}"),
    }
}
```

## 匹配 `Option<T>`
```rust
fn plus_one (x: Option<i32>) -> Option<i32> {
	match x {
		None => None,
		Some(i) => Some(i + 1),
	}
}

let five = Some(5);
let six = plus_one(five);
let none = plus_one(None);
```
## 匹配元祖
```rust
pub fn describe_number(n: i32) -> String {
    match (n, n % 2 == 0) {
        (0, _) => "Zero".to_string(),
        (x, true) if x > 0 => "Positive even".to_string(),
        (x, false) if x > 0 => "Positive odd".to_string(),
        (_, true) => "Negative even".to_string(),
        (_, false) => "Negative odd".to_string(),
    }
}
```
## 通配模式和 _ 占位符
注：通配分支必须放在最后，否则通配分支之后的其他分支将不会被匹配
```rust
let dice_roll = 9;
match dice_roll {
    3 => add_fancy_hat(),
    7 => remove_fancy_hat(),
    other => move_player(other),
}
```

 `_`是一个特殊的模式，可以匹配任意值而不绑定到该值。
```rust
 let dice_roll = 9;
 match dice_roll {
     3 => add_fancy_hat(),
     7 => remove_fancy_hat(),
     _ => (),
 }
```

## if let 表达式
[`if let` 表达式](https://doc.rust-lang.org/reference/expressions/if-expr.html#if-let-expressions) 能让你根据某个值是否与模式相匹配来执行不同的代码

匹配一个 `config_max` 变量中的 `Option<u8>` 值并只希望当值为 `Some` 成员时执行代码：
```rust
let config_max = Some(3u8);
match config_max {
    Some(max) => println!("The maximum is configured to be {}", max),
    _ => (),
}
```
如果值是 `Some`，我们希望打印出 `Some` 成员中的值，这个值被绑定到模式中的 `max` 变量里。对于 `None` 值我们不希望做任何操作。为了满足 `match` 表达式（穷尽性）的要求，必须在处理完这唯一的成员后加上 `_ => ()`，这样也要增加很多烦人的样板代码。

`if let` 这种更短的方式：

```rust
let config_max = Some(3u8);
if let Some(max) = config_max {
    println!("The maximum is configured to be {}", max);
}
```

`if let` 语法获取通过等号分隔的一个模式和一个表达式。它的工作方式与 `match` 相同，这里的表达式对应 `match` 而模式则对应第一个分支。模式是 `Some(max)`，`max` 绑定为 `Some` 中的值。接着可以在 `if let` 代码块中使用 `max` 了，就跟在对应的 `match` 分支中一样。模式不匹配时 `if let` 块中的代码不会执行。

使用 `if let` 意味着编写更少代码，更少的缩进和更少的样板代码。然而，这样会失去 `match` 强制要求的穷尽性检查。`match` 和 `if let` 之间的选择依赖特定的环境以及增加简洁度和失去穷尽性检查的权衡取舍。

 `if let` 和 `else` 表达式：
```rust
let mut count = 0;
if let Coin::Quarter(state) = coin {
    println!("State quarter from {:?}!", state);
} else {
    count += 1;
}
```