`Result` 与 `Option` 相似，但表示操作成功或失败，且每个操作的类型不同。这类似于表达式练习中定义的 `Res`，但是一个泛型：`Result<T, E>`，其中 `T` 用于 `Ok` 变体，`E` 出现在 `Err` 变体中。

```rust
enum Result<T, E> {
    Ok(T),
    Err(E),
}
```


```rust
use std::fs::File;
use std::io::Read;

fn main() {
    let file: Result<File, std::io::Error> = File::open("diary.txt");
    match file {
        Ok(mut file) => {
            let mut contents = String::new();
            if let Ok(bytes) = file.read_to_string(&mut contents) {
                println!("Dear diary: {contents} ({bytes} bytes)");
            } else {
                println!("Could not read file content");
            }
        }
        Err(err) => {
            println!("The diary could not be opened: {err}");
        }
    }
}
```