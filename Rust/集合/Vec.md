[`Vec`](https://doc.rust-lang.org/std/vec/struct.Vec.html) 是标准的可调整大小堆分配缓冲区。它包含的数据存储在堆中，这意味着在编译时不需要知道数据量，它可以在运行时增长或收缩。
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
}
```

> https://rustwiki.org/zh-CN/std/vec/struct.Vec.html#