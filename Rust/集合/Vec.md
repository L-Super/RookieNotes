[`Vec`](https://doc.rust-lang.org/std/vec/struct.Vec.html) 是标准的可调整大小堆分配缓冲区。它包含的数据存储在堆中，这意味着在编译时不需要知道数据量，它可以在运行时增长或收缩。它在内存中彼此相邻地排列所有的值。vector 只能储存相同类型的值。

`Vec<T>` 结构是 3 个 word 的胖指针，包含：一个指向堆内存的指针 pointer、分配的堆内存的容量 capacity，以及数据在堆内存的长度 length

```rust
fn main() {
    let mut v1 = Vec::new();
    v1.push(42);
    println!("v1: len = {}, capacity = {}", v1.len(), v1.capacity());

    let mut v2 = Vec::with_capacity(v1.len() + 1);
    v2.extend(v1.iter());
    v2.push(9999);
    println!("v2: len = {}, capacity = {}", v2.len(), v2.capacity());

    // vec! 宏提供方便初始化
    let mut v3 = vec![0, 0, 1, 2, 3, 4];

    // 只保留偶数元素
    v3.retain(|x| x % 2 == 0);
    println!("{v3:?}");

    // 删除连续的重复。
    v3.dedup();
    println!("{v3:?}");
    
    let mut v1 = [11, 22].to_vec();
    // 将 v1 中的所有元素附加到 v 中
    v.append(&mut v1);
    // 截断到指定长度，多余的元素被删除
    v.truncate(1);
    
    let mut v = vec![11, 22, 33, 44, 55]; 
    // 删除指定范围的元素，同时获取被删除元素的迭代器, v: [11, 55], m: [22, 33, 44] 
    let mut m: Vec<_> = v.drain(1..=3).collect();
    // 指定索引处切分成两个 vec, m: [22], v2: [33, 44]
    let v2 = m.split_off(1); 
}
```
## 创建 Vec
使用 `Vec::new` 创建动态数组：
```rust
let v: Vec<i32> = Vec::new();
```
使用宏 `vec!` 来创建数组：
```rust
let v = vec![1, 2, 3];

let v = vec![0; 3]; // 默认值为 0，初始长度为 3
```
## 插入元素
```rust
let mut v = Vec::new();
v.push(1);

v.insert(2, 3); // 在指定索引插入数据，索引值不能大于 v 的长度
```
## 读取元素
有两种方法引用 vector 中储存的值：通过索引或使用 `get` 方法。
```rust
let v = vec![1, 2, 3, 4, 5];

let third: &i32 = &v[2];
println!("The third element is {third}");

let third: Option<&i32> = v.get(2);
match third {
    Some(third) => println!("The third element is {third}"),
    None => println!("There is no third element."),
}
```
当使用索引作为参数调用 `get` 方法时，会得到一个可以用于 `match` 的 `Option<&T>`。

Rust 提供了两种引用元素的方法区别：
```rust
let v = vec![1, 2, 3, 4, 5];

let does_not_exist = &v[100];
let does_not_exist = v.get(100);
```
对于第一个 `[]` 方法，当引用一个不存在的元素时 Rust 会造成 panic。这个方法更适合当程序认为尝试访问超过 vector 结尾的元素是一个严重错误的情况，这时应该使程序崩溃。

当 `get` 方法被传递了一个数组外的索引时，它不会 panic 而是返回 `None`。当偶尔出现超过 vector 范围的访问属于正常情况的时候可以考虑使用它。

## 遍历元素
如果想要依次访问 vector 中的每一个元素，我们可以遍历其所有的元素而无需通过索引一次一个的访问。
```rust
let v = vec![100, 32, 57];
for i in &v {
    println!("{i}");
}
```
遍历可变 vector 的每一个元素的可变引用以便能改变它们。
```rust
let mut v = vec![100, 32, 57];
for i in &mut v {
    *i += 50;
}
```


> https://rustwiki.org/zh-CN/std/vec/struct.Vec.html#