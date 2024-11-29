## 引入依赖
首先，在 `Cargo.toml` 中引入 tokio 依赖。
```toml
tokio = { version = "1", features = ["full"] }
```
main 函数
把 tokio 提供的一个属性宏标注在 main 函数上面，这样 main 函数前就可以加 async 修饰了。
```rust
#[tokio::main]      // tokio库里面提供的一个属性宏标注
async fn main() {   // 注意 main 函数前面有 async 
    println!("Hello world");
}
```

`#[tokio::main]` 其实就是把用 async 修饰的 main 函数展开：
```rust
fn main() {
    tokio::runtime::Builder::new_multi_thread()
        .enable_all()
        .build()
        .unwrap()
        .block_on(async {              // 注意这里block_on，里面是异步代码
            println!("Hello world");
        })
}
```
在 main 函数里构建一个 Runtime 实例，第二行是 tokio 库下 Runtime 模块的 Builder 类型里的 `new_multi_thread()` 函数，创建的是多线程版本的 Runtime 实例。`.enable_all()` 用于打开默认所有配置，`.build()` 用于真正创建实例，它返回一个用 Result 包起来的结果，`.unwrap()` 把这个 Result 解开，把 Runtime 实例拿出来，然后在这个实例上调用 `.block_on()` 函数。整个过程用的是链式调用风格。

`block_on()` 会执行异步代码，这样就把异步代码给加载到这个 Runtime 实例上并驱动起来了。

tokio 还可以基于当前系统线程创建单线程的 Runtime：
```rust
#[tokio::main(flavor = "current_thread")]  // 属性标注里面配置参数
async fn main() {
    println!("Hello world");
}
```
展开后：
```rust
fn main() {
    tokio::runtime::Builder::new_current_thread()  // 注意这一句
        .enable_all()
        .build()
        .unwrap()
        .block_on(async {
            println!("Hello world");
        })
}
```

## tokio 组件
tokio 发展到现在，已经是一个功能丰富、机制完善的 Runtime 框架了。它针对异步场景把 Rust 标准库里对应的类型和设施都重新实现了一遍。具体包含 6 个部分：
- Runtime 设施组件：你可以自由地配置创建基于系统单线程的 Runtime 和多线程的 Runtime。
- 轻量级任务 task：类似 Go 语言中的 Goroutine 这种轻量级线程，而不是操作系统层面的线程。
- 异步输入输出（I/O）：网络模块 net、文件操作模块 fs、signal 模块、process 模块等。
- 时间模块：定时器 Interval 等。
- 异步场景下的同步原语：channel、Mutex 锁等等。
- 在异步环境下执行计算密集型任务的方案 `spawn_blocking` 等等。

## 低层机制

![](../images/Pasted%20image%2020241129103155.png)
最底层是硬件、CPU 等。其上是操作系统，Linux、Windows、macOS 等。不同的操作系统会提供不同的异步抽象机制，比如 Linux 下有 epoll，macOS 下有 kqueue。
Tokio 的异步 Runtime 能力实际正是建立在操作系统的这些异步机制上的。
Tokio 的 reactor 用来接收从操作系统的异步框架中传回的消息事件，然后通知 tokio waker 把对应的任务唤醒，放回 tokio executor 中执行。
每一个任务会被抽象成一个 Future 来独立处理，而每一个 Future 在 Rust 中会被处理成一个结构体，用状态机的方式来管理。Tokio 中还实现了对这些任务的安排调度机制。
## 示例
### 文件写
```rust
use tokio::fs::File;
use tokio::io::AsyncWriteExt;     // 引入AsyncWriteExt trait

async fn doit() -> std::io::Result<()> {             
    let mut file = File::create("foo.txt").await.unwrap();  // 创建文件
    file.write_all(b"hello, world!").await.unwrap();        // 写入内容
    Ok(())
}

#[tokio::main]
async fn main() {
    let result = doit().await;   // 注意这里的.await
}
```
### 文件读
```rust
use tokio::fs::File;
use tokio::io::AsyncReadExt;   // 引入AsyncReadExt trait

async fn doit() -> std::io::Result<()> {
    let mut file = File::open("foo.txt").await.unwrap();  // 打开文件
    let mut contents = vec![];
    // 将文件内容读到contents动态数组里面，注意传入的是可变引用
    file.read_to_end(&mut contents).await.unwrap();  
    println!("len = {}", contents.len());
    Ok(())
}

#[tokio::main]
async fn main() {
    let result = doit().await;  // 注意这里的.await
    // process
}
```
### 定时器操作
```rust
use tokio::time;
use std::time::Duration;

#[tokio::main]
async fn main() {
    // 创建Interval实例
    let mut interval = time::interval(Duration::from_millis(10));
    // 滴答，立即执行
    interval.tick().await;
    // 滴答，这个滴答完成后，10ms过去了
    interval.tick().await;
    // 滴答，这个滴答完成后，20ms过去了
    interval.tick().await;
}
```