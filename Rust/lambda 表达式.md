闭包（closures）或 lambda 表达式是可以保存在变量中或作为参数传递给其他函数的匿名函数。
具有无法命名的类型，不过，它们会实现特殊的 [`Fn`](https://doc.rust-lang.org/std/ops/trait.Fn.html)， [`FnMut`](https://doc.rust-lang.org/std/ops/trait.FnMut.html) 和 [`FnOnce`](https://doc.rust-lang.org/std/ops/trait.FnOnce.html) 特征。

```rust
let add_one_v1 = |x: u32| -> u32 { x + 1 };
// 省略类型标注
let add_one_v2 = |x|             { x + 1 };
// 省略花括号
let add_one_v3 = |x|               x + 1  ;
```

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