`HashMap<K, V>` 类型储存了一个键类型 `K` 对应一个值类型 `V` 的映射。`HashMap<K, V>` 类型储存了一个键类型 `K` 对应一个值类型 `V` 的映射。

```rust
use std::collections::HashMap;

fn main() {
    let mut page_counts = HashMap::new();
    page_counts.insert("Adventures of Huckleberry Finn".to_string(), 207);
    page_counts.insert("Grimms' Fairy Tales".to_string(), 751);
    page_counts.insert("Pride and Prejudice".to_string(), 303);

    if !page_counts.contains_key("Les Misérables") {
        println!(
            "We know about {} books, but not Les Misérables.",
            page_counts.len()
        );
    }

    for book in ["Pride and Prejudice", "Alice's Adventure in Wonderland"] {
        match page_counts.get(book) {
            Some(count) => println!("{book}: {count} pages"),
            None => println!("{book} is unknown."),
        }
    }

    // Use the .entry() method to insert a value if nothing is found.
    for book in ["Pride and Prejudice", "Alice's Adventure in Wonderland"] {
        let page_count: &mut i32 = page_counts.entry(book.to_string()).or_insert(0);
        *page_count += 1;
    }

    println!("{page_counts:#?}");
}
```

`HashMap` 有一个特有的 API，叫做 ` entry `，它获取我们想要检查的键作为参数。` entry ` 函数的返回值是一个枚举，` Entry `，它代表了可能存在也可能不存在的值。` Entry ` 的 ` or_insert ` 方法在键对应的值存在时就返回这个值的可变引用，如果不存在则将参数作为新值插入并返回新值的可变引用。`or_insert` 返回了 `&mut v` 引用，因此可以通过该可变引用直接修改 `map` 中对应的值

## 创建 HashMap
使用 new 方法创建：

```rust
let mut map = HashMap::new();
```
使用迭代器和 collect 方法创建：
```rust
fn main() {
    use std::collections::HashMap;

    let teams_list = vec![
        ("中国队".to_string(), 100),
        ("美国队".to_string(), 10),
        ("日本队".to_string(), 50),
    ];

    let teams_map: HashMap<_,_> = teams_list.into_iter().collect();
    
    println!("{:?}",teams_map)
}
```
`into_iter` 方法将列表转为迭代器，接着通过 `collect` 进行收集，需要注意的是，`collect` 方法在内部支持生成多种类型的目标集合，因此我们需要通过类型标注 `HashMap<_,_>` 来指示编译器收集为 `HashMap` 集合类型，具体的 `KV` 类型。

## 查询元素
```rust
let mut scores = HashMap::new();

scores.insert(String::from("Blue"), 10);
scores.insert(String::from("Yellow"), 50);

let team_name = String::from("Blue");
let score: Option<&i32> = scores.get(&team_name);
```
`get` 方法返回一个 `Option<&i32>` 类型：当查询不到时，会返回一个 `None`，查询到时返回 `Some(&i32)`
## 更新元素值
```rust
let mut scores = HashMap::new();
scores.insert("Blue", 10);

// 覆盖已有的值
let old = scores.insert("Blue", 20);

// 查询Yellow对应的值，若不存在则插入新值
let v = scores.entry("Yellow").or_insert(5);
}
```
