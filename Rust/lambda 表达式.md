闭包（closures）或 lambda 表达式是可以保存在变量中或作为参数传递给其他函数的匿名函数。
具有无法命名的类型，不过，它们会实现特殊的 [`Fn`](https://doc.rust-lang.org/std/ops/trait.Fn.html)， [`FnMut`](https://doc.rust-lang.org/std/ops/trait.FnMut.html) 和 [`FnOnce`](https://doc.rust-lang.org/std/ops/trait.FnOnce.html) 特征。

三种形式：
```rust
// 完整标注的闭包定义
let add_one_v1 = |x: u32| -> u32 { x + 1 };
// 省略类型标注
let add_one_v2 = |x|             { x + 1 };
// 省略花括号
let add_one_v3 = |x|               x + 1  ;
```
## 捕获引用或所有权
```rust
let list = vec![1, 2, 3]; 
// 捕获 `list` 的不可变引用
let only_borrows = || println!("From closure: {list:?}");

// 捕获可变引用
let mut borrows_mutably = || list.push(7);

// 所有权移动到新线程中
thread::spawn(move || println!("From thread: {list:?}")) .join() .unwrap();
```
## `Fn` trait
一旦闭包从其定义的环境中捕获了某个值的引用或所有权（从而影响是否以及哪些内容会被移入闭包），闭包体中的代码就决定当该闭包随后被执行时，这些引用或值会如何处理（从而影响是否以及哪些内容会被从闭包中移出）。

闭包体可以执行以下任一操作：将已捕获的值移出闭包、修改已捕获的值、既不移动也不修改该值，或者一开始就不从环境中捕获任何内容。

闭包如何从环境中捕获并处理值，会影响它实现哪些 trait；而函数和结构体正是通过这些 trait 来指定它们可以使用哪类闭包。根据闭包体对这些值的处理方式，闭包会以叠加的方式自动实现这三个 `Fn` 系列 trait 中的一个、两个或全部：

1. `FnOnce` 适用于只能被调用一次的闭包。所有闭包至少都实现了这个 trait，因为所有闭包都能被调用。一个会将捕获的值从闭包体中移出的闭包只会实现 `FnOnce` trait，而不会实现其他 `Fn` 相关的 trait，因为它只能被调用一次。
2. `FnMut` 适用于不将捕获的值移出闭包体，但可能会修改捕获值的闭包。这类闭包可以被调用多次。
3. `Fn` 适用于既不将捕获的值移出闭包体，也不修改捕获值的闭包，同时也包括不从环境中捕获任何值的闭包。这类闭包可以被多次调用而不会改变其环境，这在会多次并发调用闭包的场景中十分重要。

```rust
fn apply_with_log(func: impl FnOnce(i32) -> i32, input: i32) -> i32 {
    println!("Calling function on {input}");
    func(input)
}

fn main() {
    let add_3 = |x| x + 3;
    println!("add_3: {}", apply_with_log(add_3, 10));
    println!("add_3: {}", apply_with_log(add_3, 20));

    let mut v = Vec::new();
    let mut accumulate = |x: i32| {
        v.push(x);
        v.iter().sum::<i32>()
    };
    println!("accumulate: {}", apply_with_log(&mut accumulate, 4));
    println!("accumulate: {}", apply_with_log(&mut accumulate, 5));

    let multiply_sum = |x| x * v.into_iter().sum::<i32>();
    println!("multiply_sum: {}", apply_with_log(multiply_sum, 3));
}
```
output:
```bash
Calling function on 10  
add_3: 13  
Calling function on 20  
add_3: 23  
Calling function on 4  
accumulate: 4  
Calling function on 5  
accumulate: 9  
Calling function on 3  
multiply_sum: 27
```

默认情况下，闭包会依据引用来捕获数据（如果可以的话）。`move` 关键字则可让闭包依据值来捕获数据。
```rust
fn make_greeter(prefix: String) -> impl Fn(&str) {
    return move |name| println!("{} {}", prefix, name);
}

fn main() {
    let hi = make_greeter("Hi".to_string());
    hi("there");
}
```
output:
```bash
Hi there
```