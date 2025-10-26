`Mutex<T>` 是一个智能指针

```rust
use std::sync::Mutex;

fn main() {
    let m = Mutex::new(5);

    {
        let mut num = m.lock().unwrap();
        *num = 6;
    }

    println!("m = {m:?}");
}
```

使用 `lock` 方法来获取锁，从而可以访问互斥器中的数据。这个调用会阻塞当前线程，直到我们拥有锁为止。

`lock` 调用会**返回**一个叫做 `MutexGuard` 的智能指针。离开作用域时，自动释放锁。

在多个线程间共享 `Mutex<T>`：

```rust
use std::sync::{Arc, Mutex};
use std::thread;

fn main() {
    let counter = Arc::new(Mutex::new(0));
    let mut handles = vec![];

    for _ in 0..10 {
        let counter = Arc::clone(&counter);
        let handle = thread::spawn(move || {
            let mut num = counter.lock().unwrap();

            *num += 1;
        });
        handles.push(handle);
    }

    for handle in handles {
        handle.join().unwrap();
    }

    println!("Result: {}", *counter.lock().unwrap());
}
```

