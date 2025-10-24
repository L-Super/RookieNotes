为了实现消息传递并发，Rust 标准库提供了一个**信道**（*channel*）实现。

信道有两个组成部分：一个发送端（transmitter）和一个接收端（receiver）。

```rust
use std::sync::mpsc;
use std::thread;

fn main() {
    let (tx, rx) = mpsc::channel();

    thread::spawn(move || {
        let val = String::from("hi");
        tx.send(val).unwrap();
    });

    let received = rx.recv().unwrap();
    println!("Got: {received}");
}
```

`mpsc::channel` 函数创建一个新的信道；`mpsc` 是 **多生产者，单消费者**（*multiple producer, single consumer*）的缩写。

`mpsc::channel` 函数返回一个元组：第一个元素是发送侧，而第二个元素是接收侧。

`send` 方法返回一个 `Result<T, E>` 类型，所以如果接收端已经被丢弃了，将没有发送值的目标，所以发送操作会返回错误。

信道的接收端有两个有用的方法：`recv` 和 `try_recv`。

`recv` 会阻塞主线程执行直到从信道中接收一个值。`try_recv` 不会阻塞，它立刻返回一个 `Result<T, E>`：`Ok` 值包含可用的信息，而 `Err` 值代表此时没有任何消息。

发送多个消息：

```rust
use std::sync::mpsc;
use std::thread;
use std::time::Duration;

fn main() {
    let (tx, rx) = mpsc::channel();

    thread::spawn(move || {
        let vals = vec![
            String::from("hi"),
            String::from("from"),
            String::from("the"),
            String::from("thread"),
        ];

        for val in vals {
            tx.send(val).unwrap();
            thread::sleep(Duration::from_secs(1));
        }
    });

    // 将 rx 当作一个迭代器
    for received in rx {
        println!("Got: {received}");
    }
}
```

通过克隆发送端来创建多个生产者：

```rust
    // --snip--

    let (tx, rx) = mpsc::channel();

    let tx1 = tx.clone();
    thread::spawn(move || {
        let vals = vec![
            String::from("hi"),
            String::from("from"),
            String::from("the"),
            String::from("thread"),
        ];

        for val in vals {
            tx1.send(val).unwrap();
            thread::sleep(Duration::from_secs(1));
        }
    });

    thread::spawn(move || {
        let vals = vec![
            String::from("more"),
            String::from("messages"),
            String::from("for"),
            String::from("you"),
        ];

        for val in vals {
            tx.send(val).unwrap();
            thread::sleep(Duration::from_secs(1));
        }
    });

    for received in rx {
        println!("Got: {received}");
    }

    // --snip--
```

