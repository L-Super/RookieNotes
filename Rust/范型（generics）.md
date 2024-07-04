Rust 支持泛型，泛型是具体类型或其他属性的抽象替代。它允许您对所使用或存储的类型抽象算法或数据结构 (例如排序或二叉树)。

## 范型函数
```rust
/// 根据n的值选择even或odd
fn pick<T>(n: i32, even: T, odd: T) -> T {
    if n % 2 == 0 {
        even
    } else {
        odd
    }
}

fn main() {
    println!("picked a number: {:?}", pick(97, 222, 333));
    println!("picked a tuple: {:?}", pick(28, ("dog", 1), ("cat", 2)));
}
```
输出：
```
```bash
picked a number: 333
picked a tuple: ("dog", 1)
```


- Rust 会根据参数类型和返回值推理出 T 的类型。
- 这与 C++ 模板类似，但 Rust 会立即编译部分通用函数，因此该函数必须对所有符合约束条件的类型都有效。例如，请尝试修改 `pick` 函数，如果 `n == 0`，则返回 `even + odd`。即使仅使用带有整数的“pick”实例化，Rust 仍会将其视为无效。C++ 可让您做到这一点。
## 范型类型
可以使用泛型对具体字段类型进行抽象化处理：
```rust
#[derive(Debug)]
struct Point<T> {
    x: T,
    y: T,
}

impl<T> Point<T> {
    fn coords(&self) -> (&T, &T) {
        (&self.x, &self.y)
    }

    // fn set_x(&mut self, x: T)
}

// 多个泛型类型参数
struct Point2<T, U> {
	x: T, 
	y: U, 
}

fn main() {
    let integer = Point { x: 5, y: 10 };
    let float = Point { x: 1.0, y: 4.0 };
    println!("{integer:?} and {float:?}");
    println!("coords: {:?}", integer.coords());
}
```
## 特征边界
使用泛型时，通常会想要利用类型来实现某些特性， 这样才能调用此特征的方法。

可以使用 `T: Trait` 或 `impl Trait` 执行此操作：
```rust
fn duplicate<T: Clone>(a: T) -> (T, T) {
    (a.clone(), a.clone())
}

// struct NotClonable;

fn main() {
    let foo = String::from("foo");
    let pair = duplicate(foo);
    println!("{pair:?}");
}
```

## impl Trait
与特征边界类似，`impl Trait` 语法可以在函数形参和返回值中使用：
```rust
// Syntactic sugar for:
//   fn add_42_millions<T: Into<i32>>(x: T) -> i32 {
fn add_42_millions(x: impl Into<i32>) -> i32 {
    x.into() + 42_000_000
}

fn pair_of(x: u32) -> impl std::fmt::Debug {
    (x + 1, x - 1)
}

fn main() {
    let many = add_42_millions(42_i8);
    println!("{many}");
    let many_more = add_42_millions(10_000_000);
    println!("{many_more}");
    let debuggable = pair_of(27);
    println!("debuggable: {debuggable:?}");
}
```
- 对形参来说，`impl Trait` 就像是具有特征边界的匿名泛型形参。
- 对返回值类型来说，它则意味着返回值类型就是实现该特征的某具体类型， 无需为该类型命名。如果您不想在公共 API 中公开该具体类型，便可 使用此方法。