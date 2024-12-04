[`String`](https://doc.rust-lang.org/std/string/struct.String.html) 是标准堆分配的可扩容 UTF-8 编码字符串缓冲区。
内部是 `Vec<u8>`，String [源码](https://doc.rust-lang.org/src/alloc/string.rs.html#362-364)

```rust
fn main() {
    let mut s1 = String::new();
    s1.push_str("Hello");
    println!("s1: len = {}, capacity = {}", s1.len(), s1.capacity());

    let mut s2 = String::with_capacity(s1.len() + 1);
    s2.push_str(&s1);
    s2.push('!');
    println!("s2: len = {}, capacity = {}", s2.len(), s2.capacity());

    let s3 = String::from("中文");
    println!("s3: len = {}, number of chars = {}", s3.len(), s3.chars().count());
}
```

输出：
```
s1: len = 5, capacity = 8  
s2: len = 6, capacity = 6  
s3: len = 6, number of chars = 2
```

- `String::new` 会返回一个新的空字符串
- `String::with_capacity` 指定字符串容量
- `String::len` 会返回“String”的大小（以字节为单位，可能不同于以字符为单位的长度）。
- `String::chars` 会针对实际字符返回一个迭代器。

字符串字面值创建字符串：
```rust
let data = "initial contents";

let s = data.to_string();
// 该方法也可直接用于字符串字面值：
let s = "initial contents".to_string();

let s = String::from("initial contents");
```

以Unicode 字符的方式遍历字符串，最好的办法是使用 `chars` 方法：
```rust
for c in "中文".chars() {
    println!("{}", c);
}

output:
中
文
```
返回字符串的底层字节数组：
```rust
for b in "中国人".bytes() {
    println!("{}", b);
}

output:
228
184
173
229
155
189
228
186
186

```